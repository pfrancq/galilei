/*

	GALILEI Research Project

	GDocProfSims.cpp

	Similarities between documents and subprofiles - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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
#include <docs/gdocprofsims.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <docs/gdocs.h>
#include <docs/gdoc.h>
#include <profiles/gsubprofile.h>
#include <profiles/gusers.h>
#include <sessions/gsession.h>
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
	int Compare(const GSim* s) const {return(SubId-s->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities for a given document.
class GSims : public RContainer<GSim,unsigned int,true,true>
{
public:
	unsigned int DocId;      // Identifier of the document

	GSims(unsigned int id,unsigned int max) throw(bad_alloc)
		: RContainer<GSim,unsigned int,true,true>(max,max/2), DocId(id) {}
	int Compare(const GSims* s) const {return(DocId-s->DocId);}
	int Compare(const unsigned int id) const {return(DocId-id);}
};



//------------------------------------------------------------------------------
//
//  GDocProfSims::GDocProfSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between documents and subprofiles of
// a given language.
class GDocProfSims::GDocProfSim
{
public:
	R::RContainer<GSims,unsigned int,true,true> Sims;  // Similarities.
	bool IFF;                                          // Use IFF factor.
	GLang* Lang;                                       // Language.

	// Constructor and Compare functions.
	GDocProfSim(GDocs* d, GSubProfileCursor& s,bool iff, GLang* l) throw(bad_alloc);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GDocProfSim* docProfSim) const {return(Lang->Compare(docProfSim->Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub) throw(bad_alloc);

	// Get the similarities between two profiles, i.e. the subprofiles of a same
	// language.
	double GetSim(const GDoc* doc ,const GSubProfile* sub);

	// Update the state of the sims between documents and subprofiles. If a
	// document or a If the subprofile has changed the corresponding sim will be
	// set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element is
	// created but not computed ( -> state to osModified ).
	void Update(GDocs* docs, GUsers* users,bool iff) throw(bad_alloc);

	// Destructor.
	~GDocProfSim(void) {}
};


//------------------------------------------------------------------------------
GDocProfSims::GDocProfSim::GDocProfSim(GDocs* d, GSubProfileCursor& s,bool iff,GLang* l) throw(bad_alloc)
	: Sims(d->GetNbDocs(l)+2,d->GetNbDocs(l)/2 +1), IFF(iff), Lang(l)
{
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int nbrSubProf;
	GSims* sim;

	Cur_d= d->GetDocsCursor(Lang);
	Cur_p=s;
	nbrSubProf = Cur_p.GetNb() ;
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		Sims.InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start() ;!Cur_p.End();Cur_p.Next())
			AnalyseSim(sim,Cur_d(),Cur_p());
	}
}


//------------------------------------------------------------------------------
void GDocProfSims::GDocProfSim::AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub) throw(bad_alloc)
{
	double tmp;

	if(doc->GetLang()!=sub->GetLang()) return;
	if(IFF)
		tmp=doc->SimilarityIFF(sub);
	else
		tmp=doc->Similarity(sub);
	if(fabs(tmp)<1e-10) return;
	sim->InsertPtr(new GSim(sub->GetId(),tmp,osUpdated));
}


//------------------------------------------------------------------------------
double GDocProfSims::GDocProfSim::GetSim(const GDoc* doc,const GSubProfile* sub)
{
	GSims* s;
	GSim* s2;

	s=Sims.GetPtr<unsigned int>(doc->GetId());
	if(!s) return(0.0);
	s2=s->GetPtr<unsigned int>(sub->GetId());
	if(!s2) return(0.0);
	if((s2->State==osUpdated)||(s2->State==osUpToDate)) return(s2->Sim);
	if(s2->State == osModified)
	{
		s2->State = osUpToDate;
		if(IFF)
			s2->Sim=doc->SimilarityIFF(sub);
		else
			s2->Sim=doc->Similarity(sub);
		return (s2->Sim);
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER
	return(0.0);
}


//------------------------------------------------------------------------------
void  GDocProfSims::GDocProfSim::Update(GDocs* docs,GUsers* users,bool iff) throw(bad_alloc)
{
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int nbrSubProf;
	GSims* sims;
	GSim* sim;
	tObjState stateDoc , stateSP;


	Cur_d = docs->GetDocsCursor(Lang);
	Cur_p = users->GetSubProfilesCursor(Lang);
	nbrSubProf = Cur_p.GetNb() ;

	if(iff == IFF)
	{
		// The type of similarity hasn't changed -> some values of sim can be UpToDate
		for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
		{

			sims = Sims.GetPtr<unsigned int>(Cur_d()->GetId());
			if(!sims) Sims.InsertPtr(sims = new GSims(Cur_d()->GetId(), nbrSubProf ) );
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
	else
	{
		// the type of similarity has changed => All the sim's values must be updated.
		// The type of similarity is stocked in the param IFF.
		IFF = iff;
		for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
		{
			sims= Sims.GetPtr<unsigned int>(Cur_d()->GetId());
			if(!sims) // If sims doesn't exist -> Insert a new sims
			{
				Sims.InsertPtr(sims = new GSims(Cur_d()->GetId(),nbrSubProf ) );
				for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
				{
					sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
				}
			}
			else
			{  // If Sims exists -> overwrite the state of each sims
				for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
				{
					sim = sims->GetPtr<unsigned int >(Cur_p()->GetId());
					if (!sim ) sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
	#warning pragm osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}



//------------------------------------------------------------------------------
//
//  GDocProfSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocProfSims::GDocProfSims::GDocProfSims(GSession* session,unsigned int s,bool iff) throw(bad_alloc)
	: Sims(s,s/2), Session(session), IFF(iff)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GDocProfSim(Session,Session->GetSubProfilesCursor(Lang),IFF,Lang));
	}
}


//------------------------------------------------------------------------------
void GDocProfSims::ReInit(void) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Sims.Clear();
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GDocProfSim(Session,Session->GetSubProfilesCursor(Lang),IFF,Lang));
	}
}


//------------------------------------------------------------------------------
void GDocProfSims::UseIFF(bool iff) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		GDocProfSim* docProfSim = Sims.GetPtr<GLang*>(Lang);
		docProfSim->Update(Session,Session,iff);
	}
}


//------------------------------------------------------------------------------
double GDocProfSims::GetSim(const GDoc* doc,const GSubProfile* sub) throw(GException)
{
	if(doc->GetLang()!=sub->GetLang())
		throw GException("Cannot compare a document and subprofile of a different language");
	GDocProfSim* docProfSim = Sims.GetPtr<GLang*>(doc->GetLang());
	return(docProfSim->GetSim(doc,sub));
}


//------------------------------------------------------------------------------
GDocProfSims::~GDocProfSims(void)
{
}

