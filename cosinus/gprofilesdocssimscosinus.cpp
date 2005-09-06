/*

	GALILEI Research Project

	GProfilesDocsSimsCosinus.cpp

	Similarities between documents and subprofiles - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include standard api files
#include <math.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpluginmanagers.h>
#include <gprofilesdocssimscosinus.h>
#include <glang.h>
#include <gdoc.h>
#include <gsubprofile.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing a similarity with a subprofile.
class GSim
{
public:
	unsigned int SubId;      // Identifier of the subprofile.
	double Sim;              // Similarity between the document and the profile.
	tObjState State;         // State of the similarity.

	GSim(unsigned int id,double s,tObjState state = osUpToDate)
		: SubId(id), Sim(s),State(state) {}
	int Compare(const GSim& s) const {return(SubId-s.SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities for a given document.
class GSims : public RContainer<GSim,true,true>
{
public:
	unsigned int DocId;      // Identifier of the document

	GSims(unsigned int id,unsigned int max) throw(std::bad_alloc)
		: RContainer<GSim,true,true>(max,max/2), DocId(id) {}
	int Compare(const GSims& s) const {return(DocId-s.DocId);}
	int Compare(const unsigned int id) const {return(DocId-id);}
};



//------------------------------------------------------------------------------
//
//  GProfilesDocsSimsCosinus::GDocProfSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between documents and subprofiles of
// a given language.
class GProfilesDocsSimsCosinus::GDocProfSim
{
public:
	R::RContainer<GSims,true,true>* Sims;  // Similarities.
	GLang* Lang;                                       // Language.
	GProfilesDocsSimsCosinus* Manager;                             // Owner.

	// Constructor and Compare functions.
	GDocProfSim(GProfilesDocsSimsCosinus* manager,GLang* l);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GDocProfSim& docProfSim) const {return(Lang->Compare(*docProfSim.Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub);

	// Get the similarities between two profiles, i.e. the subprofiles of a same
	// language.
	double GetSim(const GDoc* doc ,const GSubProfile* sub);

	// Update the state of the sims between documents and subprofiles. If a
	// document or a If the subprofile has changed the corresponding sim will be
	// set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element is
	// created but not computed ( -> state to osModified ).
	void Update(void);

	//change tye type of similarity (local <-> IFF)
	void Update(bool iff);


	// Destructor.
	~GDocProfSim(void) { delete Sims;}
};


//------------------------------------------------------------------------------
GProfilesDocsSimsCosinus::GDocProfSim::GDocProfSim(GProfilesDocsSimsCosinus* manager,GLang* l)
	: Sims(0), Lang(l), Manager(manager)
{
	R::RCursor<GDoc> Cur_d;
	RCursor<GSubProfile> Cur_p;
	unsigned int nbrSubProf,i;
	GSims* sim;

	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	RCursor<GDoc> d(Manager->Session->GetDocs(Lang));
	for(d.Start(),i=0; !d.End(); d.Next())
		if(d()->GetId()>i)
			i=d()->GetId();
	Sims=new RContainer<GSims,true,true>(i+1,1);

	RCursor<GSubProfile> s(Manager->Session->GetSubProfiles(l));
	Cur_d= d;
	Cur_p=s;
	nbrSubProf = Cur_p.GetNb() ;
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		Sims->InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start() ;!Cur_p.End();Cur_p.Next())
			AnalyseSim(sim,Cur_d(),Cur_p());
	}
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::GDocProfSim::AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub)
{
	double tmp;

	if(doc->GetLang()!=sub->GetLang()) return;
	if((!doc->IsDefined())||(!sub->IsDefined())) return;
	tmp=Manager->ComputeSim(doc,sub);
	if(fabs(tmp)<Manager->NullSimLevel) return;
	sim->InsertPtr(new GSim(sub->GetId(),tmp,osUpdated));
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsCosinus::GDocProfSim::GetSim(const GDoc* doc,const GSubProfile* sub)
{
	GSims* s;
	GSim* s2;

	s=Sims->GetPtr<unsigned int>(doc->GetId());
	if(!s) return(0.0);
	s2=s->GetPtr<unsigned int>(sub->GetId());
	if(!s2) return(0.0);
	if((s2->State==osUpdated)||(s2->State==osUpToDate)) return(s2->Sim);
	if(s2->State == osModified)
	{
		s2->State = osUpToDate;
		s2->Sim=Manager->ComputeSim(doc,sub);
		return (s2->Sim);
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER
	return(0.0);
}


//------------------------------------------------------------------------------
void  GProfilesDocsSimsCosinus::GDocProfSim::Update(void)
{
	R::RCursor<GDoc> Cur_d;
	RCursor<GSubProfile> Cur_p;
	unsigned int nbrSubProf;
	GSims* sims;
	GSim* sim;
	tObjState stateDoc , stateSP;

	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	if(!Sims)
	{
		unsigned int i;
		RCursor<GDoc> d(Manager->Session->GetDocs(Lang));
		for(d.Start(),i=0; !d.End(); d.Next())
			if(d()->GetId()>i)
				i=d()->GetId();
		Sims=new RContainer<GSims,true,true>(i+1,1);
	}

	Cur_d = Manager->Session->GetDocs(Lang);
	Cur_p = Manager->Session->GetSubProfiles(Lang);
	nbrSubProf = Cur_p.GetNb() ;

	// The type of similarity hasn't changed -> some values of sim can be UpToDate
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		sims = Sims->GetPtr<unsigned int>(Cur_d()->GetId());
		if(!sims)
			Sims->InsertPtr(sims = new GSims(Cur_d()->GetId(), nbrSubProf ) );
		stateDoc = Cur_d()->GetState();                              // --------------------------------------------------------
		if ((stateDoc == osUpdated) || (stateDoc == osCreated))      // The sub1 is modified -> all the sims must be recalculated
		{
			for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
			{
				sim = sims->GetPtr<unsigned int>(Cur_p()->GetId());
				if(!sim) sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
				else sim->State = osModified;
			}
		}                                   //----------------------------------------------------------------
		else if(stateDoc == osUpToDate)       // The sub1 hasn't changed -> somes profiles sims can be unchanged
		{
			for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
			{
				stateSP = Cur_p()->GetState();
				if((stateSP == osUpdated) || (stateSP == osCreated))    //The second profile has been modified -> state de sim(id1,id2) = modified
				{
					sim = sims->GetPtr<unsigned int>(Cur_p()->GetId());
					if (!sim) sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
}



//------------------------------------------------------------------------------
//
//  GProfilesDocsSimsCosinus
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesDocsSimsCosinus::GProfilesDocsSimsCosinus::GProfilesDocsSimsCosinus(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), Sims(10,5), IDF(true), Memory(false), NeedUpdate(false)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	Sims.Clear();
	R::RCursor<GLang> Langs(GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GDocProfSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::ApplyConfig(void)
{
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	ISF=Factory->GetBool("ISF");
	IDF=Factory->GetBool("IDF");
	Memory=Factory->GetBool("Memory");
	Debug=Factory->GetBool("DebugSim");
	DebugMinSim=Factory->GetBool("DebugMinSim");
	MinSim=Factory->GetDouble("MinSim");
	AutomaticMinSim=Factory->GetBool("AutomaticMinSim");
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::Update(void)
{
	RCursor<GDocProfSim> Cur(Sims);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Update();
	NeedUpdate=false;
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsCosinus::ComputeSim(const GDoc* doc,const GSubProfile* sub) const
{
	double sim;

	if((!ISF)&&(!IDF))
		sim=doc->Similarity(*sub);
	else
	{
		if(ISF)
		{
			if(IDF)
				sim=doc->SimilarityIFF2(*sub,otDoc,otDocSubProfile,sub->GetLang());
			else
				sim=doc->SimilarityIFF(*sub,otDocSubProfile,sub->GetLang());
		}
		else
			sim=doc->SimilarityIFF(*sub,otDoc,sub->GetLang());
	}
	if(fabs(sim)<NullSimLevel)
		sim=0.0;
	return(sim);
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsCosinus::GetMeasure(unsigned int id1,unsigned int id2,unsigned int)
{
	GDoc* doc=Session->GetDoc(id1,0);
	GSubProfile* sub=Session->GetSubProfile(id2,0);

	if(doc->GetLang()!=sub->GetLang())
		throw GException("Cannot compare a document and subprofile of a different language");

	//if memory is false, re-calculate similarity
	if(!Memory)
		return(ComputeSim(doc,sub));

	if(NeedUpdate)
		Update();
	GDocProfSim* docProfSim = Sims.GetPtr<GLang*>(doc->GetLang());
	if(!docProfSim)
		throw GException("Language "+doc->GetLang()->GetName()+" is not active");
	return(docProfSim->GetSim(doc,sub));
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsCosinus::GetMinMeasure(const GLang*,unsigned int)
{
	return(MinSim);
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::Event(GLang* lang, tEvent event)
{
	if(!Memory) return;
	switch(event)
	{
		case eObjCreated:
			Sims.InsertPtr(new GDocProfSim(this,lang));
			break;
		case eObjDeleted:
			Sims.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::Event(GDoc* doc, tEvent event)
{
	if(!Memory) return;
	GDocProfSim* profSim;
	RCursor<GSims> Sub;

	switch(event)
	{
		case eObjCreated:
		case eObjModified:
/*			profSim=Sims.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->AddModifiedProfile(sub);*/
			NeedUpdate=true;
			break;
		case eObjDeleted:
			profSim = Sims.GetPtr<const GLang*>(doc->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(profSim->Sims)
			{
/*				Sub.Set(*profSim->Sims);
				for(Sub.Start();!Sub.End();Sub.Next())
				{
					if(Sub()->SubId>sub->GetProfile()->GetId())
						continue;
					if(sub->GetProfile()->GetId()<Sub()->GetMaxPos())
						Sub()->DeletePtrAt(sub->GetProfile()->GetId(),false);
				}*/
//				if(doc->GetId()<profSim->Sims->GetMaxPos())
				profSim->Sims->DeletePtr(doc->GetId());
			}
//			profSim->ModifiedProfs->DeletePtr(sub);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::Event(GSubProfile* sub, tEvent event)
{
	if(!Memory) return;
	GDocProfSim* profSim;
	RCursor<GSims> Sub;

	switch(event)
	{
		case eObjCreated:
		case eObjModified:
/*			profSim=Sims.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->AddModifiedProfile(sub);*/
			NeedUpdate=true;
			break;
		case eObjDeleted:
			profSim = Sims.GetPtr<const GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(profSim->Sims)
			{
				Sub.Set(*profSim->Sims);
				for(Sub.Start();!Sub.End();Sub.Next())
				{
/*					if(Sub()->SubId>sub->GetProfile()->GetId())
						continue;
					if(sub->GetProfile()->GetId()<Sub()->GetMaxPos())*/
					Sub()->DeletePtr(sub->GetId());
				}
/*				if(sub->GetProfile()->GetId()<profSim->Sims->GetMaxPos())
					profSim->Sims->DeletePtrAt(sub->GetProfile()->GetId(),false);*/
			}
//			profSim->ModifiedProfs->DeletePtr(sub);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesDocsSimsCosinus::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamDouble("NullSimLevel",0.00001));
	params->InsertPtr(new GParamBool("Memory",false));
	params->InsertPtr(new GParamBool("ISF",true));
	params->InsertPtr(new GParamBool("IDF",true));
	params->InsertPtr(new GParamBool("DebugSim",false));
	params->InsertPtr(new GParamBool("DebugMinSim",false));
	params->InsertPtr(new GParamDouble("MinSim",0.05));
	params->InsertPtr(new GParamBool("AutomaticMinSim",true));
}


//------------------------------------------------------------------------------
GProfilesDocsSimsCosinus::~GProfilesDocsSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles/Documents Similarities","Cosinus Method",GProfilesDocsSimsCosinus)
