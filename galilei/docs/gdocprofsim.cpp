/*

	GALILEI Research Project

	GProfilesSim.cpp

	Similarities between Profiles - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <rstd/rcontainer.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
//#include <profiles/gprofilessim.h>
#include <docs/gdocprofsim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gusers.h>
#include <langs/glang.h>
#include <docs/gdocs.h>
#include <docs/gdoc.h>



using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// class GSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSim
{
public:
	unsigned int Id;         // identifier of the second profile
	double Sim;              // Similarity between the profiles.
	tObjState State;         //  State of the similarity.


	GSim(unsigned int id,double s,tObjState state = osUpToDate) : Id(id), Sim(s),State(state)  {;}
	int Compare(const GSim* s) const {return(Id-s->Id);}
	int Compare(const GSim& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
	void changeValue(double sim){Sim = sim;}

};



//-----------------------------------------------------------------------------
//
// class GSims
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GDocProfSim::GSims : public RStd::RContainer<GSim,unsigned int,true,true>
{
public:
	unsigned int Id;         // Identifier of the first profile

	GSims(unsigned int id,unsigned int max) throw(bad_alloc);
	int Compare(const GSims* s) const {return(Id-s->Id);}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};


//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::GSims::GSims(unsigned int id,unsigned int max) throw(bad_alloc)
	: RStd::RContainer<GSim,unsigned int,true,true>(max,max/2), Id(id)
{
}


//-----------------------------------------------------------------------------
//
// class GDocProfSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::GDocProfSim(GDocs* d, RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global,GLang* l) throw(bad_alloc)
		: Sims(d->GetNbDocs(l)+2,d->GetNbDocs(l)/2+1), GlobalSim(global), Lang(l) 
 {
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int  nbrSubProf;
	GSims* sim;

	Cur_d= d->GetDocsCursor(Lang);
	Cur_p.Set(s);
	nbrSubProf = Cur_p.GetNb() ;
	for(Cur_d.Start();!Cur_d.End() ;Cur_d.Next())
	{
		Sims.InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
		{
			AnalyseSim(sim,Cur_d(),Cur_p());
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::GDocProfSim(GDocs* d, RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global,GLang* l) throw(bad_alloc)
     : Sims(d->GetNbDocs(l)+2,d->GetNbDocs(l)/2 +1), GlobalSim(global), Lang(l)
{
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int nbrSubProf;
	GSims* sim;

	Cur_d= d->GetDocsCursor(Lang);
	Cur_p.Set(s);
	nbrSubProf = Cur_p.GetNb() ;
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		Sims.InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start() ;!Cur_p.End();Cur_p.Next())
		{
			AnalyseSim(sim,Cur_d(),Cur_p());
		}
	}
}

//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::GDocProfSim(GDocCursor* d, GSubProfileCursor* s,bool global,GLang* l) throw(bad_alloc)
	: Sims(100,50), GlobalSim(global), Lang(l)
{
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int nbrSubProf;
	GSims* sim;

	Cur_d = *d;
	Cur_p=*s;
	nbrSubProf = Cur_p.GetNb() ;
	for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
	{
		Sims.InsertPtr(sim=new GSims(Cur_d()->GetId(),nbrSubProf));
		for(Cur_p.Start() ;!Cur_p.End();Cur_p.Next())
			AnalyseSim(sim,Cur_d(),Cur_p());
	}
}

//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::GDocProfSim(GDocs* d, GSubProfileCursor& s,bool global,GLang* l) throw(bad_alloc)
	: Sims(d->GetNbDocs(l)+2,d->GetNbDocs(l)/2 +1), GlobalSim(global), Lang(l)
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




//-----------------------------------------------------------------------------
void GALILEI::GDocProfSim::AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub)
{
	double tmp;
	if (doc->GetLang() == sub->GetLang())
	{
		if(GlobalSim)
			tmp=doc->GlobalSimilarity(sub);
		else
			tmp=doc->Similarity(sub);
		if(fabs(tmp)<1e-10) return;
		sim->InsertPtr(new GSim(sub->GetId(),tmp,osUpdated));
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GDocProfSim::GetSim(GDocs* docs,GUsers* users, unsigned int i,unsigned int j)
{
	GSims* s;
	GSim* s2;
	

	if(docs->GetPtr<unsigned int>(i)->GetLang()->Compare(Lang) ) return (0.0);   // if the langages are differents
	s=Sims.GetPtr<unsigned int>(i);
	if(!s) return(0.0);
	s2=s->GetPtr<unsigned int>(j);
	if(!s2) return(0.0);

	if( (s2->State == osUpdated) || (s2->State == osUpToDate))  return s2->Sim;

	if(s2->State == osModified)
	{
		s2->State = osUpToDate ;
		if (GlobalSim)
		{
			s2->Sim=docs->GetDoc(i,Lang)->GlobalSimilarity(users->GetSubProfile(j,Lang));
			return (s2->Sim);
		}
		else
		{
			s2->Sim=docs->GetDoc(i,Lang)->Similarity(users->GetSubProfile(j,Lang));
			return (s2->Sim);
		}
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDocProfSim::GetSim(const GDoc* doc,const GSubProfile* sub)
{
	GSims* s;
	GSim* s2;
	int i,j;

	if(doc->GetLang()->Compare(sub->GetLang()) ) return (0.0);                      // if the langages are differents

	i = doc->GetId();
	j = sub->GetId();

	
	s=Sims.GetPtr<unsigned int>(i);
	if(!s) return(0.0);
	s2=s->GetPtr<unsigned int>(j);
	if(!s2) return(0.0);

	if( (s2->State == osUpdated) || (s2->State == osUpToDate))  return s2->Sim;

	if(s2->State == osModified)
	{
		s2->State = osUpToDate ;
		if (GlobalSim)
		{
			s2->Sim=doc->GlobalSimilarity(sub);
			return ( s2->Sim);
		}
		else
		{
			s2->Sim=doc->Similarity(sub);
			return ( s2->Sim);
		}

	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
int GALILEI::GDocProfSim::Compare(const GLang* l) const
{
	return(Lang->Compare(l));
}


//-----------------------------------------------------------------------------
int GALILEI::GDocProfSim::Compare(const GDocProfSim& docProfSim) const
{
	return(Lang->Compare(docProfSim.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GDocProfSim::Compare(const GDocProfSim* docProfSim) const
{
	return(Lang->Compare(docProfSim->Lang));
}


//-----------------------------------------------------------------------------
tObjState GALILEI::GDocProfSim::GetState(unsigned int doc_id, unsigned int prof_id)
{
	GSims* sims;
	GSim* sim;
	
	sims = Sims.GetPtr<unsigned int>(doc_id);
	if (!sims) return osUnknow;
	sim = sims->GetPtr<unsigned int>(prof_id);
	if (!sim) return osUnknow;

	return sim->State ;

}

//-----------------------------------------------------------------------------
void  GALILEI::GDocProfSim::UpdateDocProfSim(GDocs* docs,GUsers* users,bool global)throw(bad_alloc)
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
	
	if(global == GlobalSim) // The type of similarity hasn't changed -> some values of sim can be UpToDate
	{

		for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
		{

			sims = Sims.GetPtr<unsigned int>(Cur_d()->GetId());
			if(!sims) Sims.InsertPtr(sims = new GSims(Cur_d()->GetId(), nbrSubProf ) );

			stateDoc = Cur_d()->GetState();                                                            // --------------------------------------------------------
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
					stateSP = Cur_p()->GetProfile()->GetState();
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
	else  // the type of similarity has changed => All the sim's values must be updated. //The type of similarity is stocked in the param GlobalSim
	{
		GlobalSim = global;
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
			else{  // If Sims exists -> overwrite the state of each sims
				for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
				{
					sim = sims->GetPtr<unsigned int >(Cur_p()->GetId());
					if (!sim ) sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//-----------------------------------------------------------------------------
void  GALILEI::GDocProfSim::UpdateDocProfSim(GDocs* docs , GUsers& users,bool global)throw(bad_alloc)
{
	GDocCursor Cur_d;
	GSubProfileCursor Cur_p;
	unsigned int nbrSubProf;
	GSims* sims;
	GSim* sim;
	tObjState stateDoc , stateSP;


	Cur_d = docs->GetDocsCursor(Lang);
	Cur_p = users.GetSubProfilesCursor(Lang);
	nbrSubProf = Cur_p.GetNb() ;

	if(global == GlobalSim) // The type of similarity hasn't changed -> some values of sim can be UpToDate
	{

		for(Cur_d.Start();!Cur_d.End();Cur_d.Next())
		{

			sims = Sims.GetPtr<unsigned int>(Cur_d()->GetId());
			if(!sims) Sims.InsertPtr(sims = new GSims(Cur_d()->GetId(), nbrSubProf ) );

			stateDoc = Cur_d()->GetState();                                                            // --------------------------------------------------------
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
					stateSP = Cur_p()->GetProfile()->GetState();
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
	else  // the type of similarity has changed => All the sim's values must be updated. //The type of similarity is stocked in the param GlobalSim
	{
		GlobalSim = global;
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
			else{  // If Sims exists -> overwrite the state of each sims
				for(Cur_p.Start();!Cur_p.End();Cur_p.Next())
				{
					sim = sims->GetPtr<unsigned int >(Cur_p()->GetId());
					if (!sim ) sims->InsertPtr(sim = new GSim(Cur_p()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//-----------------------------------------------------------------------------
GALILEI::GDocProfSim::~GDocProfSim(void)
{
}
 
