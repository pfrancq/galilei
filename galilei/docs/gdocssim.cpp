/*

	GALILEI Research Project

	GDocsSim.h

	Similarities between documents - Implementation

	(C) 2002 by P. Francq.

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
// include files for GALILEI
#include <docs/gdocssim.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
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
	unsigned int Id;         // identifier of the second document.
	double Sim;              // Similarity between the documents.

	GSim(unsigned int id,double s) : Id(id), Sim(s) {};
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
class GALILEI::GDocsSim::GSims : public RStd::RContainer<GSim,unsigned int,true,true>
{
public:
	unsigned int Id;         // Identifier of the first document.

	GSims(unsigned int id,unsigned int max) throw(bad_alloc);
	int Compare(const GSims* s) const {return(Id-s->Id);}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};

//-----------------------------------------------------------------------------
GALILEI::GDocsSim::GSims::GSims(unsigned int id,unsigned int max) throw(bad_alloc)
	: RStd::RContainer<GSim,unsigned int,true,true>(max,max/2), Id(id)
{
}



//-----------------------------------------------------------------------------
//
// class GDocsSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocsSim::GDocsSim(GDocs* d,bool global) throw(bad_alloc)
	: Sims(d->GetNbDocs(),d->GetNbDocs()/2)
{
	GDocCursor Cur1;
	GDocCursor Cur2;
	unsigned int i,j;
	GSims* sim;

	Cur1.Set(d);
	Cur2.Set(d);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		Sims.InsertPtr(sim=new GSims(Cur1()->GetId(),j));
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			if(Cur1()->GetLang()!=Cur2()->GetLang()) continue;
			if(global)
				sim->InsertPtr(new GSim(Cur2()->GetId(),Cur1()->GlobalSimilarity(Cur2())));
			else
				sim->InsertPtr(new GSim(Cur2()->GetId(),Cur1()->Similarity(Cur2())));
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GDocsSim::GDocsSim(GDocs& d,bool global) throw(bad_alloc)
	: Sims(d.GetNbDocs(),d.GetNbDocs()/2)
{
	GDocCursor Cur1;
	GDocCursor Cur2;
	unsigned int i,j;
	GSims* sim;

	Cur1.Set(d);
	Cur2.Set(d);
	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		Sims.InsertPtr(sim=new GSims(Cur1()->GetId(),j));
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			if(Cur1()->GetLang()!=Cur2()->GetLang()) continue;
			if(global)
				sim->InsertPtr(new GSim(Cur2()->GetId(),Cur1()->GlobalSimilarity(Cur2())));
			else
				sim->InsertPtr(new GSim(Cur2()->GetId(),Cur1()->Similarity(Cur2())));
		}
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GDocsSim::GetSim(unsigned int i,unsigned int j)
{
	GSims* s;
	GSim* s2;
	unsigned int tmp;

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
	return(s2->Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GDocsSim::GetSim(const GDoc* d1,const GDoc* d2)
{
	return(GetSim(d1->GetId(),d2->GetId()));
}


//-----------------------------------------------------------------------------
GALILEI::GDocsSim::~GDocsSim(void)
{
}
