/*

	GALILEI Research Project

	GFdbk.cpp

	Feedback - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gfdbk.h>
#include <gprofile.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFdbk
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFdbk::GFdbk(size_t docid,tFdbkType fdbk,const RDate& done,const R::RDate& computed)
  : DocId(docid), Fdbk(fdbk), Done(done), Computed(computed)
{
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const GFdbk& profdoc) const
{
	return(CompareIds(DocId,profdoc.DocId));
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const size_t id) const
{
	return(CompareIds(DocId,id));
}


//------------------------------------------------------------------------------
void GFdbk::NewFdbk(tFdbkType fdbk,const RDate& date)
{
	Fdbk=fdbk;
	Done=date;
}


//------------------------------------------------------------------------------
RDate GFdbk::GetDone(void) const
{
	return(Done);
}


//------------------------------------------------------------------------------
RDate GFdbk::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
bool GFdbk::MustUse(const GProfile* profile) const
{
	return((Done>profile->GetComputed())||(Computed>profile->GetComputed()));
}


//------------------------------------------------------------------------------
void GFdbk::HasUpdate(void)
{
	Computed.SetToday();
}


//------------------------------------------------------------------------------
tFdbkType GFdbk::ErrorFdbk(tFdbkType fdbk,double PercErr,RRandom& rand)
{
	double random=rand.GetValue()*100+1.0;

	// If there is Random change the judgment.
	if(random<PercErr)
	{
		random=rand.GetValue()*100+1.0;;
		switch(fdbk)
		{
			case ftRelevant:
				if(random<25.0)
					return(ftIrrelevant);
				else
					return(ftFuzzyRelevant);
			case ftFuzzyRelevant:
				if(random<50.0)
					return(ftRelevant);
				else
					return(ftIrrelevant);
			case ftIrrelevant:
				if(random<25.0)
					return(ftRelevant);
				else
					return(ftFuzzyRelevant);
			default:
				ThrowGException(GetFdbkType(fdbk,true)+" is not supported");
		}
	}
	return(fdbk);
}


//------------------------------------------------------------------------------
GFdbk::~GFdbk(void)
{
}
