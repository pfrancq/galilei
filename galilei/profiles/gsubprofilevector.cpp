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
#include<docs/gdoc.h>
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
	return(Vector->Similarity((static_cast<const GSubProfileVector*>(desc))->Vector));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::Similarity(const GDoc* doc) const
{
	return(Vector->Similarity(doc->GetWordWeights()));
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
