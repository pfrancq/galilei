/*

	GALILEI Research Project

	GDocsSim.h

	Similarities between documents - Implementation

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
// include files for GALILEI
#include <docs/gdocssim.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
using namespace GALILEI;
using namespace RIO;


//-----------------------------------------------------------------------------
//
// class GSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GDocsSim::GSim
{
public:
	unsigned int Id1;
	unsigned int Id2;
	double Sim;

	GSim(void) : Id1(0), Id2(0), Sim(0.0) {}
	GSim(const GSim& r) : Id1(r.Id1), Id2(r.Id2), Sim(r.Sim) {}
	GSim(unsigned int id1,unsigned id2,double s) : Id1(0), Id2(0), Sim(s)
	{
		if(id1<id2)
		{
			Id1=id1;
			Id2=id2;
		}
		else
		{
			Id1=id2;
			Id2=id1;
		}
	}
	int Compare(const GSim* rec) const
	{
		int i=Id1-rec->Id1;
		if(i) return(i);
		return(Id2-rec->Id2);
	}
	int Compare(const GSim& rec) const
	{
		int i=Id1-rec.Id1;
		if(i) return(i);
		return(Id2-rec.Id2);
	}
	GSim& operator=(const GSim& src)
	{
		Id1=src.Id1;
		Id2=src.Id2;
		Sim=src.Sim;
		return(*this);
	}
	void Write(RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>& f)
	{
		f<<Id1<<Id2<<Sim;
	}
	void Read(RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>& f)
	{
		f>>Id1>>Id2>>Sim;
	}
};



//-----------------------------------------------------------------------------
//
// class GDocsSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocsSim::GDocsSim(const char* filename,GDocs* d,bool global) throw(bad_alloc,RString)
	: Sims(0)
{
	GDocCursor Cur1;
	GDocCursor Cur2;

	try
	{
		Sims=new RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>(filename,RIO::Read);
	}
	catch(...)
	{
		// File doesn't exist -> Create it
		Cur1.Set(d);
		Cur2.Set(d);
		ComputeSims(filename,Cur1,Cur2,global);
		Sims=new RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>(filename,RIO::Read);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GDocsSim::GDocsSim(const char* filename,GDocs& d,bool global) throw(bad_alloc,RString)
	: Sims(0)
{
	GDocCursor Cur1;
	GDocCursor Cur2;

	try
	{
		Sims=new RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>(filename,RIO::Read);
	}
	catch(...)
	{
		// File doesn't exist -> Create it
		Cur1.Set(d);
		Cur2.Set(d);
		ComputeSims(filename,Cur1,Cur2,global);
		Sims=new RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>(filename,RIO::Read);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocsSim::ComputeSims(const char* filename,GDocCursor& Cur1,GDocCursor& Cur2,bool global) throw(bad_alloc,RString)
{
	unsigned int i,j;
	RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true> f(filename,RIO::Create);
	GSim r;

	for(Cur1.Start(),i=0,j=Cur1.GetNb();--j;Cur1.Next(),i++)
	{
		r.Id1=Cur1()->GetId();
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			if(Cur1()->GetLang()!=Cur2()->GetLang()) continue;
			if(global)
				r.Sim=Cur1()->GlobalSimilarity(Cur2());
			else
				r.Sim=Cur1()->Similarity(Cur2());
			if(r.Sim)
			{
				r.Id2=Cur2()->GetId();
				r.Write(f);
			}
		}
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GDocsSim::GetSim(unsigned int i,unsigned int j)
{
	unsigned int tmp;
	GSim c;

	if(i==j) return(1.0);
	if(i>j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
	c.Id1=i;
	c.Id2=j;
	if(!Sims->GetRec(c))
		return(0.0);
	return(c.Sim);
}


//-----------------------------------------------------------------------------
double GALILEI::GDocsSim::GetSim(const GDoc* d1,const GDoc* d2)
{
	return(GetSim(d1->GetId(),d2->GetId()));
}


//-----------------------------------------------------------------------------
GALILEI::GDocsSim::~GDocsSim(void)
{
	if(Sims)
		delete Sims;
}
