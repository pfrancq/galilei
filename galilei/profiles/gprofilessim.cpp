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
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gusers.h>
#include <langs/glang.h>



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


	GSim(unsigned int id,double s,tObjState state = osUpToDate) : Id(id), Sim(s),State(state)  {}
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
class GALILEI::GProfilesSim::GSims : public RStd::RContainer<GSim,unsigned int,true,true>
{
public:
	unsigned int Id;         // Identifier of the first profile

	GSims(unsigned int id,unsigned int max) throw(bad_alloc);
	int Compare(const GSims* s) const {return(Id-s->Id);}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GSims::GSims(unsigned int id,unsigned int max) throw(bad_alloc)
	: RStd::RContainer<GSim,unsigned int,true,true>(max,max/2), Id(id)
{
}


//-----------------------------------------------------------------------------
//
// class GProfilesSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global,GLang* l) throw(bad_alloc)
	: Sims(s->NbPtr,s->NbPtr/2), GlobalSim(global), Lang(l)
{
	GSubProfileCursor Cur1;
	GSubProfileCursor Cur2;
	unsigned int i,j;
	GSims* sim;

	Cur1.Set(s);
	Cur2.Set(s);
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		Sims.InsertPtr(sim=new GSims(Cur1()->GetId(),j));
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			AnalyseSim(sim,Cur1(),Cur2());

		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global,GLang* l) throw(bad_alloc)
	: Sims(s.NbPtr,s.NbPtr/2), GlobalSim(global), Lang(l)
{
	GSubProfileCursor Cur1;
	GSubProfileCursor Cur2;
	unsigned int i,j;
	GSims* sim;

	Cur1.Set(s);
	Cur2.Set(s);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		Sims.InsertPtr(sim=new GSims(Cur1()->GetId(),j));
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			AnalyseSim(sim,Cur1(),Cur2());
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GProfilesSim(GSubProfileCursor& s,bool global) throw(bad_alloc)
	: Sims(s.GetNb(),s.GetNb()/2), GlobalSim(global)
{
	GSubProfileCursor Cur1;
	GSubProfileCursor Cur2;
	unsigned int i,j;
	GSims* sim;

	Cur1=s;
	Cur2=s;
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		Sims.InsertPtr(sim=new GSims(Cur1()->GetId(),j));
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
			AnalyseSim(sim,Cur1(),Cur2());
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfilesSim::AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2)
{
	double tmp;

	if(GlobalSim)
		tmp=sub1->GlobalSimilarity(sub2);
	else
		tmp=sub1->Similarity(sub2);
	if(fabs(tmp)<1e-10) return;   
	sim->InsertPtr(new GSim(sub2->GetId(),tmp,osUpdated));
}


//-----------------------------------------------------------------------------
double GALILEI::GProfilesSim::GetSim(GUsers* users, unsigned int i,unsigned int j)
{
	GSims* s;
	GSim* s2;
	unsigned int tmp;

	if(i==j) return(1.0);
	if(i>j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
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
			  s2->Sim=users->GetSubProfile(i,Lang)->GlobalSimilarity(users->GetSubProfile(j,Lang));
			  return (s2->Sim);
		}
		else
		{
			 s2->Sim=users->GetSubProfile(i,Lang)->Similarity(users->GetSubProfile(j,Lang));
			 return (s2->Sim);
		}
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER
	
	return(0.0);
}


//-----------------------------------------------------------------------------          
double GALILEI::GProfilesSim::GetSim(const GSubProfile* sub1,const GSubProfile* sub2)
{
	GSims* s;
	GSim* s2;
	int i,j,tmp;


	i = sub1->GetId();
	j = sub2->GetId();

	if(i==j) return (1.0);
	if (i>j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
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
			s2->Sim=sub1->GlobalSimilarity(sub2);
			return ( s2->Sim);
		}
		else
		{
			 s2->Sim=sub1->Similarity(sub2);
			 return ( s2->Sim);
		}
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GLang* l) const
{
	return(Lang->Compare(l));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GProfilesSim& profilesSim) const
{
	return(Lang->Compare(profilesSim.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GProfilesSim* profilesSim) const
{
	return(Lang->Compare(profilesSim->Lang));
}


//-----------------------------------------------------------------------------
tObjState GALILEI::GProfilesSim::GetState(unsigned int id1, unsigned int id2)
{
	GSims* sims;
	GSim* sim;
	int tmp;
	
	if (id1>id2)
	{
		tmp=id1;
		id1=id2;
		id2=tmp;
	}
	
	sims = Sims.GetPtr<unsigned int>(id1);
	if (!sims) return osUnknow;
	sim = sims->GetPtr<unsigned int>(id2);
	if (!sim) return osUnknow;

	return sim->State ;
	
}

//-----------------------------------------------------------------------------
void  GALILEI::GProfilesSim::UpdateProfSim(GUsers* users,bool global,GLang* lang)throw(bad_alloc)
{

	Lang = lang;
	GSubProfileCursor Cur1;
	GSubProfileCursor Cur2;
	unsigned int i,j;
	GSims* sims;
	GSim* sim;
	tObjState stateSP;


	Cur1 = users->GetSubProfilesCursor(Lang);
	Cur2 = users->GetSubProfilesCursor(Lang);

	if(global == GlobalSim) // The type of similarity hasn't changed -> some values of sim can be UpToDate
	{
		if(!Cur1.GetNb()) return;
		for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		{

			sims = Sims.GetPtr<unsigned int>(Cur1()->GetId());
			if(!sims) Sims.InsertPtr(sims = new GSims(Cur1()->GetId() ,j) );

			stateSP = Cur1()->GetProfile()->GetState();                                                            // --------------------------------------------------------
			if ((stateSP == osUpdated) || (stateSP == osCreated))      // The sub1 is modified -> all the sims must be recalculated
			{
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sim = sims->GetPtr<unsigned int>(Cur2()->GetId());
					if(!sim) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}                                   //----------------------------------------------------------------
			else if(stateSP == osUpToDate)       // The sub1 hasn't changed -> somes profiles sims can be unchanged
			{
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					stateSP = Cur2()->GetProfile()->GetState();
					if((stateSP == osUpdated) || (stateSP == osCreated))    //The second profile has been modified -> state de sim(id1,id2) = modified
					{
						sim = sims->GetPtr<unsigned int>(Cur2()->GetId());
						if (!sim) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
						else sim->State = osModified;
					}
				}
			}
		}
	}
	else  // the type of similarity has changed => All the sim's values must be updated. //The type of similarity is stocked in the param GlobalSim
	{
		GlobalSim = global;
		if(!Cur1.GetNb()) return;
		for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		{
			sims= Sims.GetPtr<unsigned int>(Cur1()->GetId());
			if(!sims) // If sims doesn't exist -> Insert a new sims
			{
				Sims.InsertPtr(sims = new GSims(Cur1()->GetId(),j) );
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
				}
			}
			else{  // If Sims exists -> overwrite each sims
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sim = sims->GetPtr<unsigned int >(Cur2()->GetId());
					if (!sim ) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//-----------------------------------------------------------------------------
void  GALILEI::GProfilesSim::UpdateProfSim(GUsers& users,bool global,GLang* lang)throw(bad_alloc)
{
	Lang = lang;
	GlobalSim = global;
	GSubProfileCursor Cur1;
	GSubProfileCursor Cur2;
	unsigned int i,j;
	GSims* sims;
	GSim* sim;
	tObjState stateSP;

	Cur1 = users.GetSubProfilesCursor(Lang);
	Cur2 = users.GetSubProfilesCursor(Lang);
	if(global == GlobalSim) // The type of similarity hasn't changed -> some values of sim can be UpToDate
	{
		for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		{
			sims = Sims.GetPtr<unsigned int>(Cur1()->GetId());
			if(!sims) Sims.InsertPtr(sims = new GSims(Cur1()->GetId() ,j) );

			stateSP = Cur1()->GetProfile()->GetState();                                                            // --------------------------------------------------------
			if ((stateSP == osUpdated) || (stateSP == osCreated))      // The sub1 is modified -> all the sims must be recalculated
			{
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sim = sims->GetPtr<unsigned int>(Cur2()->GetId());
					if(!sim) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}                                   //----------------------------------------------------------------
			else if(stateSP == osUpToDate)       // The sub1 hasn't changed -> somes profiles sims can be unchanged
						{
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					stateSP = Cur2()->GetProfile()->GetState();
					if((stateSP == osUpdated) || (stateSP == osCreated))    //The second profile has been modified -> state de sim(id1,id2) = modified
					{
						sim = sims->GetPtr<unsigned int>(Cur2()->GetId());
						if (!sim) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
						else sim->State = osModified;
					}
				}
			}
		}
	}
	else  // the type of similarity has changed => All the sim's values must be updated.
	{
		for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
		{
			sims= Sims.GetPtr<unsigned int>(Cur1()->GetId());
			if(!sims) // If sims doesn't exist -> Insert a new sims
			{
				Sims.InsertPtr(sims = new GSims(Cur1()->GetId(),j) );
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
				}
			}
			else{  // If Sims exists -> overwrite each sims
				for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
				{
					sim = sims->GetPtr<unsigned int >(Cur2()->GetId());
					if (!sim ) sims->InsertPtr(sim = new GSim(Cur2()->GetId(),0,osModified));
					else sim->State = osModified;
				}
			}
		}
	}
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::~GProfilesSim(void)
{
}
