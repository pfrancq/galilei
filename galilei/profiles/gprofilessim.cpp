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

	GSim(unsigned int id,double s) : Id(id), Sim(s) {}
	int Compare(const GSim* s) const {return(Id-s->Id);}
	int Compare(const GSim& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
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
GALILEI::GProfilesSim::GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global) throw(bad_alloc)
	: Sims(s->NbPtr,s->NbPtr/2), GlobalSim(global)
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
			AnalyseSim(sim,Cur1(),Cur2());
	}
}


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global) throw(bad_alloc)
	: Sims(s.NbPtr,s.NbPtr/2), GlobalSim(global)
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
			AnalyseSim(sim,Cur1(),Cur2());
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
	sim->InsertPtr(new GSim(sub2->GetId(),tmp));
}


//-----------------------------------------------------------------------------
double GALILEI::GProfilesSim::GetSim(unsigned int i,unsigned int j)
{
	GSims* s;
	GSim* s2;
	unsigned int tmp;
	double sim;

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
	sim=s2->Sim;
	return(sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GProfilesSim::GetSim(const GSubProfile* s1,const GSubProfile* s2)
{
	return(GetSim(s1->GetId(),s2->GetId()));
}


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::~GProfilesSim(void)
{
}
