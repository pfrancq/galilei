/*

	GALILEI Research Project

	GProfilesGroupsSimsCosinus.cpp

	Similarities between documents and groups - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gprofilesgroupssimscosinus.h>
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

	GSims(unsigned int id,unsigned int max)
		: RContainer<GSim,true,true>(max,max/2), DocId(id) {}
	int Compare(const GSims& s) const {return(DocId-s.DocId);}
	int Compare(const unsigned int id) const {return(DocId-id);}
};



//------------------------------------------------------------------------------
//
//  GProfilesGroupsSimsCosinus::GProfileGrpSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between documents and subprofiles of
// a given language.
class GProfilesGroupsSimsCosinus::GProfileGrpSim
{
public:
	R::RContainer<GSims,true,true>* Sims;  // Similarities.
	GLang* Lang;                                       // Language.
	GProfilesGroupsSimsCosinus* Manager;                             // Owner.

	// Constructor and Compare functions.
	GProfileGrpSim(GProfilesGroupsSimsCosinus* manager,GLang* l);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GProfileGrpSim& docProfSim) const {return(Lang->Compare(*docProfSim.Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GSubProfile* sub,const GGroup* grp);

	// Get the similarities between two profiles, i.e. the subprofiles of a same
	// language.
	double GetSim(const GSubProfile* sub,const GGroup* grp);

	// Update the state of the sims between documents and subprofiles. If a
	// document or a If the subprofile has changed the corresponding sim will be
	// set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element is
	// created but not computed ( -> state to osModified ).
	void Update(void);

	//change tye type of similarity (local <-> IFF)
	void Update(bool iff);


	// Destructor.
	~GProfileGrpSim(void) { delete Sims;}
};


//------------------------------------------------------------------------------
GProfilesGroupsSimsCosinus::GProfileGrpSim::GProfileGrpSim(GProfilesGroupsSimsCosinus* manager,GLang* l)
	: Sims(0), Lang(l), Manager(manager)
{
	R::RCursor<GSubProfile> Cur_d;
	RCursor<GGroup> Cur_p;
	unsigned int nbrSubProf,i;
	GSims* sim;

	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	RCursor<GSubProfile> d(Manager->Session->GetSubProfiles(Lang));
	for(d.Start(),i=0; !d.End(); d.Next())
		if(d()->GetId()>i)
			i=d()->GetId();
	Sims=new RContainer<GSims,true,true>(i+1,1);

	RCursor<GGroup> s(Manager->Session->GetGroups(l));
	Cur_d= d;
	Cur_p=s;
	nbrSubProf = Cur_p.GetNb();
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		Sims->InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
			AnalyseSim(sim,Cur_d(),Cur_p());
	}
}


//------------------------------------------------------------------------------
void GProfilesGroupsSimsCosinus::GProfileGrpSim::AnalyseSim(GSims* sim,const GSubProfile* sub,const GGroup* grp)
{
	double tmp;

	if(sub->GetLang()!=grp->GetLang()) return;
	if((!sub->IsDefined())||(!grp->IsDefined())) return;
	if(Manager->IDF)
		tmp=sub->SimilarityIFF(*grp,otSubProfileGroup,sub->GetLang());
	else
		tmp=sub->Similarity(*grp);
	if(fabs(tmp)<Manager->NullSimLevel) return;
	sim->InsertPtr(new GSim(grp->GetId(),tmp,osUpdated));
}


//------------------------------------------------------------------------------
double GProfilesGroupsSimsCosinus::GProfileGrpSim::GetSim(const GSubProfile* sub,const GGroup* grp)
{
	GSims* s;
	GSim* s2;

	//if memory is false, re-calculate similarity
	if(!Manager->Memory)
	{
		if(Manager->IDF)
			return(sub->SimilarityIFF(*grp,otSubProfileGroup,sub->GetLang()));
		else
			return(sub->Similarity(*grp));
	}

	s=Sims->GetPtr<unsigned int>(sub->GetId());
	if(!s) return(0.0);
	s2=s->GetPtr<unsigned int>(grp->GetId());
	if(!s2) return(0.0);
	if((s2->State==osUpdated)||(s2->State==osUpToDate)) return(s2->Sim);
	if(s2->State == osModified)
	{
		s2->State = osUpToDate;
		if(Manager->IDF)
			s2->Sim=sub->SimilarityIFF(*grp,otSubProfileGroup,sub->GetLang());
		else
			s2->Sim=sub->Similarity(*grp);
		if(fabs(s2->Sim)<Manager->NullSimLevel) s2->Sim=0.0;
		return (s2->Sim);
	}
	if (s2->State == osDeleteMem)  return (0.0);   //-------------------------A MODIFIER
	return(0.0);
}


//------------------------------------------------------------------------------
void  GProfilesGroupsSimsCosinus::GProfileGrpSim::Update(void)
{
	R::RCursor<GSubProfile> Cur_d;
	RCursor<GGroup> Cur_p;
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
		RCursor<GSubProfile> d(Manager->Session->GetSubProfiles(Lang));
		for(d.Start(),i=0; !d.End(); d.Next())
			if(d()->GetId()>i)
				i=d()->GetId();
		Sims=new RContainer<GSims,true,true>(i+1,1);
	}

	Cur_d = Manager->Session->GetSubProfiles(Lang);
	Cur_p = Manager->Session->GetGroups(Lang);
	nbrSubProf = Cur_p.GetNb();

	// The type of similarity hasn't changed -> some values of sim can be UpToDate
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		sims = Sims->GetPtr<unsigned int>(Cur_d()->GetId());
		if(!sims)
			Sims->InsertPtr(sims = new GSims(Cur_d()->GetId(), nbrSubProf ) );
		stateDoc = Cur_d()->GetState();                              // --------------------------------------------------------
		if ((stateDoc == osUpdated) || (stateDoc == osNewMem))      // The sub1 is modified -> all the sims must be recalculated
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
				if((stateSP == osUpdated) || (stateSP == osNewMem))    //The second profile has been modified -> state de sim(id1,id2) = modified
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
//  GProfilesGroupsSimsCosinus
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesGroupsSimsCosinus::GProfilesGroupsSimsCosinus::GProfilesGroupsSimsCosinus(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), Sims(10,5), IDF(true), Memory(false), NeedUpdate(false)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	Sims.Clear();
	R::RCursor<GLang> Langs(GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GProfileGrpSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesGroupsSimsCosinus::ApplyConfig(void)
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
void GProfilesGroupsSimsCosinus::Update(void)
{
	RCursor<GProfileGrpSim> Cur(Sims);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Update();
	NeedUpdate=false;
}


//------------------------------------------------------------------------------
double GProfilesGroupsSimsCosinus::GetMeasure(unsigned int id1,unsigned int id2,unsigned int)
{
	GSubProfile* sub=Session->GetSubProfile(id1,0);
	GGroup* grp=Session->GetGroup(id2,0);

	if(sub->GetLang()!=grp->GetLang())
		throw GException("Cannot compare a document and subprofile of a different language");
	if(!Memory)
	{
		double Sim;
		if(IDF)
			Sim=sub->SimilarityIFF(*grp,otSubProfileGroup,sub->GetLang());
		else
			Sim=sub->Similarity(*grp);
		if(fabs(Sim)<NullSimLevel) Sim=0.0;
		return(Sim);
	}
	if(NeedUpdate)
		Update();
	GProfileGrpSim* docProfSim = Sims.GetPtr<GLang*>(sub->GetLang());
	if(!docProfSim)
		throw GException("Language "+sub->GetLang()->GetName()+" is not active");
	return(docProfSim->GetSim(sub,grp));
}


//------------------------------------------------------------------------------
double GProfilesGroupsSimsCosinus::GetMinMeasure(const GLang*,unsigned int)
{
	return(MinSim);
}


//------------------------------------------------------------------------------
void GProfilesGroupsSimsCosinus::Event(GLang* lang, tEvent event)
{
	if(!Memory) return;
	switch(event)
	{
		case eObjNewMem:
			Sims.InsertPtr(new GProfileGrpSim(this,lang));
			break;
		case eObjDeleteMem:
			Sims.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesGroupsSimsCosinus::Event(GSubProfile* sub, tEvent event)
{
	if(!Memory) return;
	GProfileGrpSim* profSim;
	RCursor<GSims> Sub;

	switch(event)
	{
		case eObjNewMem:
		case eObjModified:
/*			profSim=Sims.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->AddModifiedProfile(sub);*/
			NeedUpdate=true;
			break;
		case eObjDeleteMem:
			profSim = Sims.GetPtr<const GLang*>(sub->GetLang());
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
				profSim->Sims->DeletePtr(sub->GetId());
			}
//			profSim->ModifiedProfs->DeletePtr(sub);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesGroupsSimsCosinus::Event(GGroup* grp, tEvent event)
{
	if(!Memory) return;
	GProfileGrpSim* profSim;
	RCursor<GSims> Sub;

	switch(event)
	{
		case eObjNewMem:
		case eObjModified:
/*			profSim=Sims.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->AddModifiedProfile(sub);*/
			NeedUpdate=true;
			break;
		case eObjDeleteMem:
			profSim = Sims.GetPtr<const GLang*>(grp->GetLang());
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
					Sub()->DeletePtr(grp->GetId());
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
void GProfilesGroupsSimsCosinus::CreateParams(GParams* params)
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
GProfilesGroupsSimsCosinus::~GProfilesGroupsSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles/Groups Similarities","Cosinus Method",GProfilesGroupsSimsCosinus)

