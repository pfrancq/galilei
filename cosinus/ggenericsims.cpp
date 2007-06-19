/*

	GALILEI Research Project

	GenericSims.cpp

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
// include files for ANSI C/C++
#include<math.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include "ggenericsims.h"



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
	unsigned int Id;         // Identifier
	double Sim;              // Similarity
	tObjState State;         // State of the similarity.

	GSim(unsigned int id)
		: Id(id), Sim(0.0),State(osModified) {}
	int Compare(const GSim& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
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
	unsigned int Id;      // Identifier of the document

	GSims(unsigned int id)
		: RContainer<GSim,true,true>(5000,5000), Id(id) {}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};



//------------------------------------------------------------------------------
//
//  GenericSims::GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between documents and subprofiles of
// a given language.
class GGenericSims::GElementSims
{
public:
	R::RContainer<GSims,true,true>* Sims;             // Similarities.
	GLang* Lang;                                      // Language.
	GGenericSims* Manager;                             // Owner.
	bool NeedUpdate;
	
	// Constructor and Compare functions.
	GElementSims(GGenericSims* manager,GLang* l);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GElementSims& sim) const {return(Lang->Compare(*sim.Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GDoc* doc,const GGroup* grp);

	// Get the similarities between two profiles, i.e. the subprofiles of a same
	// language.
	double GetSim(size_t id1,size_t id2);

	// Update the state of the sims between documents and subprofiles. If a
	// document or a If the subprofile has changed the corresponding sim will be
	// set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element is
	// created but not computed ( -> state to osModified ).
	void Update(void);

	// Destructor.
	~GElementSims(void) { delete Sims;}
};


//------------------------------------------------------------------------------
GGenericSims::GElementSims::GElementSims(GGenericSims* manager,GLang* l)
	: Sims(0), Lang(l), Manager(manager), NeedUpdate(true)
{
	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;
}


//------------------------------------------------------------------------------
double GGenericSims::GElementSims::GetSim(size_t id1,size_t id2)
{
	GSims* s;
	GSim* s2;

	s=Sims->GetPtr<unsigned int>(id1);
	if(!s)
		Sims->InsertPtr(s=new GSims(id1));
	s2=s->GetPtr<unsigned int>(id2);
	if(!s2)
		s->InsertPtr(s2=new GSim(id2));
	if((s2->State==osUpdated)||(s2->State==osUpToDate)) return(s2->Sim);
	if(s2->State == osModified)
	{
		s2->State = osUpToDate;
		s2->Sim=Manager->Compute(Lang,id1,id2);
		if(fabs(s2->Sim)<Manager->NullSimLevel) s2->Sim=0.0;
		return (s2->Sim);
	}
	cerr<<"Problem in GGenericSims"<<endl;
	return(0.0);
}


//------------------------------------------------------------------------------
void GGenericSims::GElementSims::Update(void)
{
	// If memory is false, no update is needed
	// since sims are claulctaed each time
	if((!Manager->Memory)||(!Manager->Session)||(!NeedUpdate))
		return;

	//initialize the container of GSims (calculate size)
	if(!Sims)
		Sims=new RContainer<GSims,true,true>(Manager->GetMaxId1(Lang),500);

	RCursor<GSims> Cur1(*Sims);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		RCursor<GSim> Cur2(*Cur1());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->State=osModified;
	}
	
	
	NeedUpdate=false;
}



//------------------------------------------------------------------------------
//
//  GenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGenericSims::GGenericSims(GFactoryMeasure* fac,bool d,bool g,bool s)
	: GMeasure(fac), GSignalHandler(), Sims(10,5), Memory(false), Doc(d), Group(g), SubProfile(s)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	Sims.Clear();
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GElementSims(this,Langs()));
}


//-----------------------------------------------------------------------------
void GGenericSims::ApplyConfig(void)
{
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	Memory=Factory->GetBool("Memory");
}


//------------------------------------------------------------------------------
void GGenericSims::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	GLang* lang=va_arg(ap,GLang*);
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	double* res=va_arg(ap,double*);

	if(!Memory)
	{
		(*res)=Compute(lang,id1,id2);
		va_end(ap);
		return;
	}

	// Get the language
	GElementSims* Sim=Sims.GetPtr<GLang*>(lang);
	if(!Sim)
		throw GException("Language not defined");
	if(Sim->NeedUpdate)
		Sim->Update();
		
	(*res)=Sim->GetSim(id1,id2);
	va_end(ap);
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GLang* lang, tEvent event)
{
	if(!Memory) return;
	switch(event)
	{
		case eObjNewMem:
			Sims.InsertPtr(new GElementSims(this,lang));
			break;
		case eObjDeleteMem:
			Sims.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GDoc* doc, tEvent)
{
	if((!Memory)||(!Doc)) return;
	if(!doc->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(doc->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GGroup* grp, tEvent)
{
	if((!Memory)||(!Group)) return;
	if(!grp->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(grp->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GSubProfile* sub, tEvent)
{
	if((!Memory)||(!SubProfile)) return;
	if(!sub->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(sub->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("Memory",false));
}


//------------------------------------------------------------------------------
GGenericSims::~GGenericSims(void)
{
	GSession::DeleteHandler(this);
}
