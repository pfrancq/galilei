/*

	GALILEI Research Project

	GSubProfileVector.cpp

	Subprofile  in the Vector Model - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include<profiles/gsubprofilevector.h>
#include <groups/ggroupvector.h>
#include<infos/glang.h>
#include<infos/gweightinfo.h>
#include<docs/gdocvector.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfileVector::GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c) throw(bad_alloc)
  : GSubProfile(prof,id,lang,grp,a, state, c), GWeightInfos(60)
{
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GSubProfileVector::NewSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c)
{
	return(new GSubProfileVector(prof,id,lang,grp,a,state,c));
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::ClearFdbks(void)
{
	RemoveRefs();
	GWeightInfos::Clear();
	GSubProfile::ClearFdbks();
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileVector::IsDefined(void) const
{
	return(NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::AddWord(unsigned int id,double w) throw(bad_alloc)
{
	InsertPtr(new GWeightInfo(id,w));
}

//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::AddWordList(unsigned int id,double w) throw(bad_alloc)
{
	InsertPtr(new GWeightInfo(id,w,infoWordList));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::SimilarityIFF(const GDoc* doc) const
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GDocVector*>(doc),otDocSubProfile,Lang));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::Similarity(const GSubProfile* sub) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GSubProfileVector*>(sub))));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::SimilarityIFF(const GSubProfile* sub) const
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GSubProfileVector*>(sub)),otSubProfile,Lang));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::Similarity(const GGroup* grp) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GGroupVector*>(grp))));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfileVector::SimilarityIFF(const GGroup* grp) const
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GGroupVector*>(grp)),otSubProfileGroup,Lang));
}


//-----------------------------------------------------------------------------
GWeightInfoCursor& GALILEI::GSubProfileVector::GetWeightInfoCursor(void)
{
	GWeightInfoCursor *cur=GWeightInfoCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfileVector::GetNbNoNull(void) const
{
	return(NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::UpdateRefs(void) const
{
	AddRefs(otSubProfile,Lang);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileVector::RemoveRefs(void) const
{
	DelRefs(otSubProfile,Lang);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileVector::~GSubProfileVector(void)
{
	RemoveRefs();
}
