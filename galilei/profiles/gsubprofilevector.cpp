/*

	GALILEI Research Project

	GSubProfileVector.cpp

	Sub-Profile Description using a vector - Implementation.

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include<profiles/gsubprofilevector.h>
#include<langs/glang.h>
#include<infos/giword.h>
#include<infos/giwordweight.h>
#include<infos/giwordsweights.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfileVector::GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a) throw(bad_alloc)
  : GSubProfile(prof,id,lang,grp,a), Vector(0)
{
	Vector = new GIWordsWeights(Lang,60);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GSubProfileVector::NewSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a)
{
	return(new GSubProfileVector(prof,id,lang,grp,a));
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileVector::IsDefined(void) const
{
	// The Vector can't be empty.
	return(Vector&&(Vector->NbPtr));
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::AddWord(unsigned int id,double w) throw(bad_alloc)
{
	Vector->InsertPtr(new GIWordWeight(id,w));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::Similarity(const GSubProfile* desc) const
{
	double Sim=0.0;
	GIWordWeight** ptr=Vector->Tab;
	GIWordWeight** ptr2=((GSubProfileVector*)desc)->Vector->Tab;
	unsigned int i=Vector->NbPtr+1;
	unsigned int j=((GSubProfileVector*)desc)->Vector->NbPtr;
	double norm1=0.0;
	double norm2=0.0;

	while(--i)
	{
		while(j&&((*ptr2)->GetId()<(*ptr)->GetId()))
		{
			j--;
			norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
			ptr2++;
		}
		if(j&&((*ptr2)->GetId()==(*ptr)->GetId()))
		{
			j--;
			norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
			Sim+=(*ptr)->GetWeight()*(*ptr2)->GetWeight();
			ptr2++;
		}
		norm1+=(*ptr)->GetWeight()*(*ptr)->GetWeight();
		ptr++;
	}
	while(j)
	{
		j--;
		norm2+=(*ptr2)->GetWeight()*(*ptr2)->GetWeight();
		ptr2++;
	}
	Sim/=(sqrt(norm1)*sqrt(norm2));
	return(Sim);
}


//-----------------------------------------------------------------------------
GIWordWeightCursor& GALILEI::GSubProfileVector::GetVectorCursor(void)
{
	GIWordWeightCursor *cur=GIWordWeightCursor::GetTmpCursor();
	cur->Set(Vector);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfileVector::GetNbNoNull(void) const
{
	return(Vector->NbPtr);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileVector::~GSubProfileVector(void)
{
	if(Vector) delete Vector;
}
