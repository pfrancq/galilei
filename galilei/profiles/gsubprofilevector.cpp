/*

	GALILEI Research Project

	GSubProfileVector.cpp

	Subprofile  in the Vector Model - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include<profiles/gsubprofilevector.h>
#include <groups/ggroupvector.h>
#include<infos/glang.h>
#include<infos/gweightinfo.h>
#include<docs/gdocvector.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GSubProfileVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfileVector::GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c) throw(bad_alloc)
  : GSubProfile(prof,id,lang,grp,a, state, c), GWeightInfos(60)
{
}



//------------------------------------------------------------------------------
void GSubProfileVector::ClearFdbks(void) throw(bad_alloc)
{
	RemoveRefs();
	GWeightInfos::Clear();
	GSubProfile::ClearFdbks();
}


//------------------------------------------------------------------------------
bool GSubProfileVector::IsDefined(void) const
{
	return(NbPtr);
}


//-------------------------------------------------------------------------------
void GSubProfileVector::AddInfo(GWeightInfo* info) throw(bad_alloc)
{
	InsertPtr(info);
}


//------------------------------------------------------------------------------
double GSubProfileVector::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//------------------------------------------------------------------------------
double GSubProfileVector::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GDocVector*>(doc),otDocSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GSubProfileVector::Similarity(const GSubProfile* sub) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GSubProfileVector*>(sub))));
}


//------------------------------------------------------------------------------
double GSubProfileVector::SimilarityIFF(const GSubProfile* sub) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GSubProfileVector*>(sub)),otSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GSubProfileVector::Similarity(const GGroup* grp) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GGroupVector*>(grp))));
}


//------------------------------------------------------------------------------
double GSubProfileVector::SimilarityIFF(const GGroup* grp) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GGroupVector*>(grp)),otSubProfileGroup,Lang));
}


//------------------------------------------------------------------------------
GWeightInfoCursor& GSubProfileVector::GetWeightInfoCursor(void)
{
	GWeightInfoCursor *cur=GWeightInfoCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
unsigned int GSubProfileVector::GetNbNoNull(void) const
{
	return(NbPtr);
}


//------------------------------------------------------------------------------
void GSubProfileVector::UpdateRefs(void) const throw(GException)
{
	AddRefs(otSubProfile,Lang);
}


//------------------------------------------------------------------------------
void GSubProfileVector::RemoveRefs(void) const throw(GException)
{
	DelRefs(otSubProfile,Lang);
}


//------------------------------------------------------------------------------
GSubProfileVector::~GSubProfileVector(void) throw(GException)
{
	RemoveRefs();
}
