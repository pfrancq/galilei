/*

	GALILEI Research Project

	GProfilesBehaviours.cpp

	Agreement and DisAgreement Ratios for subprofiles - Implementation.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for GALILEI
#include <profiles/gprofilesbehaviours.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gusers.h>
#include <infos/glang.h>
#include <sessions/gsession.h>
#include <infos/glangmanager.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GBehaviour
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing the ratios with a subprofile.
class GBehaviour
{
public:

	unsigned int SubId;        // Identifier of the second profile.
	double AgreementRatio;     // Agreement ratio.
	double DisagreementRatio;  // Disagreement ratio.
	tObjState State;           // State of the behaviour.

	GBehaviour(unsigned int id,double agree, double disagree,tObjState state = osUpToDate)
		: SubId(id), AgreementRatio(agree), DisagreementRatio(disagree),State(state) {}
	int Compare(const GBehaviour* b) const {return(SubId-b->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GBehaviours
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the ratios for a given subprofile.
class GBehaviours : public R::RContainer<GBehaviour,unsigned int,true,false>
{
public:
	unsigned int SubId;         // Identifier of the first profile

	GBehaviours(unsigned int id,unsigned int max) throw(bad_alloc)
		: RContainer<GBehaviour,unsigned int,true,false>(max,max/2), SubId(id) {}
	int Compare(const GBehaviours* b) const {return(SubId-b->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GProfilesBehaviours::GProfilesBehaviour
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the ratios between subprofiles of a given language.
class GProfilesBehaviours::GProfilesBehaviour
{
public:

	R::RContainer<GBehaviours,unsigned int,true,false>* Behaviours;     // Ratios
	GLang* Lang;                                                        // Language.
	unsigned int NbModified;                                            // Number of modified subprofiles
	unsigned int MinSameDocs;                                           // Minimum number fo documents for Agreement
	unsigned int MinDiffDocs;                                           // Minimum number fo documents for Disagreement
	GProfilesBehaviours* Manager;                                       //manger
	// Identificators of modified profiles
	RContainer<GSubProfile,unsigned int,false,true>* ModifiedProfs;

	// Constructor and Compare methods.
	GProfilesBehaviour(GProfilesBehaviours* manager,GSubProfileCursor& s,GLang* lang, unsigned int minsamedocs, unsigned int mindiffdocs) throw(bad_alloc);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesBehaviour* p) const {return(Lang->Compare(p->Lang));}


	// Analyse the behaviour of the two subprofiles and insert it in behaviours if necessary.
	void AnalyseBehaviour(GBehaviours* behaviours,GSubProfile* sub1,GSubProfile* sub2);

	// Get the agreement and disagreement ratios between two profiles.
	GBehaviour* GetRatio(GSubProfile* s1,GSubProfile* s2);
	double GetAgreementRatio(GSubProfile* s1,GSubProfile* s2);
	double GetDisagreementRatio(GSubProfile* s1,GSubProfile* s2);

	// Update the state of the profiles behaviours : If the subprofile has changed
	// the corresponding sim will be set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element
	// is created but not computed ( -> state to osModified ).
	void Update(void) throw(bad_alloc);

	// Add a subprofile to the listof the modified one.
	void AddModifiedProfile(GSubProfile* s);

	// add a new GBehaviours fo a new subprofile subprof
	GBehaviours* AddNewBehaviours(GSubProfile* subprof);

	// Destructor.
	virtual ~GProfilesBehaviour(void) {}
};


//------------------------------------------------------------------------------
GProfilesBehaviours::GProfilesBehaviour::GProfilesBehaviour(GProfilesBehaviours* manager, GSubProfileCursor& s,GLang* l, unsigned int minsamedocs, unsigned int mindiffdocs) throw(bad_alloc)
	: Lang(l), MinSameDocs(minsamedocs), MinDiffDocs(mindiffdocs), Manager(manager)
{
	GSubProfileCursor Cur1, Cur2;
	unsigned int i,j, pos;
	GBehaviours* behaviours;

	//initialize the container of GSBehavirous (calculate size)
	for (s.Start(), i=0; !s.End(); s.Next())
		if (s()->GetProfile()->GetId()>i)
			i=s()->GetProfile()->GetId();
	Behaviours=new RContainer<GBehaviours,unsigned int,true,false>(i+1,1);

	//initialize table of modified subprofiles;
	ModifiedProfs=new RContainer<GSubProfile,unsigned int,false,true>(5,1);

	//builds the left inferior triangular matrix.
	Cur1=s;
	Cur2=s;
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		pos=Cur1()->GetProfile()->GetId();
		behaviours=new GBehaviours(Cur1()->GetProfile()->GetId(),pos);
		Behaviours->InsertPtrAt(behaviours,pos);
		for(Cur2.Start(), j=0;j<i;Cur2.Next(),j++)
 			behaviours->InsertPtrAt(new GBehaviour(Cur2()->GetProfile()->GetId(),0.0,0.0,osModified),Cur2()->GetProfile()->GetId());
	}
}


//------------------------------------------------------------------------------
void GProfilesBehaviours::GProfilesBehaviour::AnalyseBehaviour(GBehaviours* behaviours,GSubProfile* sub1,GSubProfile* sub2)
{
	unsigned int pos, nbcommon, nbsame, nbdiff;
	double pcsame, pcdiff;

	pcsame=pcdiff=0.0;
	nbsame=sub1->GetCommonOKDocs(sub2);
	nbdiff=sub1->GetCommonDiffDocs(sub2);
	nbcommon=sub1->GetCommonDocs(sub2);
	if (nbcommon&&nbcommon>=MinDiffDocs)
		pcdiff=nbdiff/nbcommon;
	if (nbcommon&&nbcommon>=MinSameDocs)
		pcsame=nbsame/nbcommon;
	pos=sub2->GetProfile()->GetId();
	behaviours->InsertPtrAt(new GBehaviour(pos,pcsame, pcdiff,osUpdated), pos);
}


//------------------------------------------------------------------------------
GBehaviour* GProfilesBehaviours::GProfilesBehaviour::GetRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GBehaviours* b;
	GBehaviour* b2;
	int i,j,tmp;
	double okratio, diffratio, nbcommon;

	okratio=diffratio=nbcommon=0.0;
	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	if(i==j) return(0);
	if(i<j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
	b=Behaviours->GetPtr<unsigned int>(i);
	if(!b) return(0);
	b2=b->GetPtr<unsigned int>(j);
	if(!b2) return(0);

	if((b2->State == osUpdated)||(b2->State == osUpToDate))
		return(b2);

	if(b2->State == osModified)
	{
		okratio=diffratio=0.0;
		b2->State=osUpToDate;
		nbcommon=sub1->GetCommonDocs(sub2);
		if (nbcommon&&nbcommon>=MinSameDocs)
			okratio=sub1->GetCommonOKDocs(sub2)/nbcommon;
		if (nbcommon&&nbcommon>=MinDiffDocs)
			diffratio=sub1->GetCommonDiffDocs(sub2)/nbcommon;
		 b2->AgreementRatio=okratio;
		 b2->DisagreementRatio=diffratio;
 		 return(b2);
	}

	if (b2->State==osDelete)
		return(0);     //-------------------------A MODIFIER

	return(0);
}


//------------------------------------------------------------------------------
double GProfilesBehaviours::GProfilesBehaviour::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GBehaviour* b;

	if(sub1->GetId()==sub2->GetId()) return(1.0);
	b=GetRatio(sub1,sub2);
	if(!b) return(0.0);
	return(b->AgreementRatio);
}


//------------------------------------------------------------------------------
double GProfilesBehaviours::GProfilesBehaviour::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GBehaviour* b;

	if(sub1->GetId()==sub2->GetId()) return(0.0);
	b=GetRatio(sub1,sub2);
	if(!b) return(0.0);
	return(b->DisagreementRatio);
}


//------------------------------------------------------------------------------
void GProfilesBehaviours::GProfilesBehaviour::AddModifiedProfile(GSubProfile* s)
{
	ModifiedProfs->InsertPtr(s);
}


//------------------------------------------------------------------------------
void  GProfilesBehaviours::GProfilesBehaviour::Update(void) throw(bad_alloc)
{
	GBehaviours* behaviours;

	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if(!Manager->GetMemory())
		return;


	// change status of modified subprofiles and add Behaviours of created subprofiles
	for (ModifiedProfs->Start(); !ModifiedProfs->End(); ModifiedProfs->Next())
	{
		if (!(*ModifiedProfs)()->IsDefined()) continue;
		behaviours = Behaviours->GetPtrAt((*ModifiedProfs)()->GetProfile()->GetId());
		if(!behaviours)
			behaviours=AddNewBehaviours((*ModifiedProfs)());
		for (behaviours->Start(); !behaviours->End(); behaviours->Next())
			(*behaviours)()->State=osModified;
	}

	//reset the number of modified subprofiles.
	ModifiedProfs->Clear();
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}



//------------------------------------------------------------------------------
GBehaviours*  GProfilesBehaviours::GProfilesBehaviour::AddNewBehaviours(GSubProfile* sub)
{
	GBehaviours* behaviours, *tmpbehaviours;
	GSubProfileCursor subcur;

	if (!sub->IsDefined()) return 0;
	behaviours=new GBehaviours(sub->GetProfile()->GetId(),sub->GetProfile()->GetId());
	Behaviours->InsertPtrAt(behaviours, sub->GetProfile()->GetId());
        subcur=Manager->GetSession()->GetSubProfilesCursor(Lang);

	for (subcur.Start(); !subcur.End(); subcur.Next())
	{
		if (subcur()->GetProfile()->GetId()<sub->GetProfile()->GetId())
		{
			AnalyseBehaviour(behaviours, sub,subcur());
		}
		if (subcur()->GetProfile()->GetId()>sub->GetProfile()->GetId())
		{
			tmpbehaviours=Behaviours->GetPtrAt(subcur()->GetProfile()->GetId());
			if(tmpbehaviours)
				AnalyseBehaviour(tmpbehaviours, subcur(), sub);
		}
	}
	return behaviours;

}
//------------------------------------------------------------------------------
//
//   GProfilesBehaviours
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesBehaviours::GProfilesBehaviours(GSession* session,bool memory) throw(bad_alloc)
	: Ratios(10,5), Session(session), Memory(memory)
{
	GFactoryLangCursor Langs;
	GLang* Lang;
	unsigned int agree,disagree;

	agree=Session->GetSessionParams()->GetUInt("SameBehaviourMinDocs");
	disagree=Session->GetSessionParams()->GetUInt("DiffBehaviourMinDocs");
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Ratios.InsertPtr(new GProfilesBehaviour(this,Session->GetSubProfilesCursor(Lang),Lang,agree,disagree));
	}
}


//------------------------------------------------------------------------------
void GProfilesBehaviours::ReInit(void) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;
	unsigned int agree,disagree;

	if (!Memory) return;

	Ratios.Clear();
	agree=Session->GetSessionParams()->GetUInt("SameBehaviourMinDocs");
	disagree=Session->GetSessionParams()->GetUInt("DiffBehaviourMinDocs");
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Ratios.InsertPtr(new GProfilesBehaviour(this,Session->GetSubProfilesCursor(Lang),Lang,agree,disagree));
	}
}


//------------------------------------------------------------------------------
void GProfilesBehaviours::Update(void) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		GProfilesBehaviour* r = Ratios.GetPtr<GLang*>(Lang);
		r->Update();
	}
}


//------------------------------------------------------------------------------
double GProfilesBehaviours::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");
	GProfilesBehaviour* profBehaviour = Ratios.GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetAgreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesBehaviours::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");
	GProfilesBehaviour* profBehaviour = Ratios.GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetDisagreementRatio(sub1,sub2));
}



//------------------------------------------------------------------------------
void GProfilesBehaviours::AddModifiedProfile(GSubProfile* sub) throw(bad_alloc,GException)
{
	GProfilesBehaviour* profBehaviour;

	profBehaviour = Ratios.GetPtr<GLang*>(sub->GetLang());
	if(!profBehaviour)
		throw GException("Language not defined");
	profBehaviour->AddModifiedProfile(sub);
}


//------------------------------------------------------------------------------
GProfilesBehaviours::~GProfilesBehaviours(void)
{
}
