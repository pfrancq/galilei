/*

	GALILEI Research Project

	GGroupVector.cpp

	Group in the Vector Model- Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#include <groups/ggroupvector.h>
#include <profiles/gsubprofilevector.h>
#include <docs/gdocvector.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GGroupVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupVector::GGroupVector(unsigned int id,GLang* lang,bool com) throw(bad_alloc)
	: GGroup(id,lang,com), GWeightInfos(60)
{
}


//------------------------------------------------------------------------------
GGroupVector::GGroupVector(GLang* lang,bool com) throw(bad_alloc)
	: GGroup(lang,com), GWeightInfos(60)
{
}


//------------------------------------------------------------------------------
bool GGroupVector::IsDefined(void) const
{
	return(!GWeightInfos::IsEmpty());
}


//------------------------------------------------------------------------------
void GGroupVector::AddInfo(GWeightInfo* info) throw(bad_alloc)
{
	GWeightInfos::InsertPtr(info);
}


//------------------------------------------------------------------------------
GWeightInfoCursor& GGroupVector::GetWeightInfoCursor(void)
{
	GWeightInfoCursor *cur=GWeightInfoCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
double GGroupVector::Similarity(const GGroup* desc) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GGroupVector*>(desc))));
}


//------------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GGroup* desc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GGroupVector*>(desc)),otGroup,Lang));
}


//------------------------------------------------------------------------------
double GGroupVector::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//------------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GDocVector*>(doc),otDocGroup,Lang));
}


//------------------------------------------------------------------------------
double GGroupVector::Similarity(const GSubProfile* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GSubProfileVector*>(doc)));
}


//------------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GSubProfile* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GSubProfileVector*>(doc),otSubProfileGroup,Lang));
}


//------------------------------------------------------------------------------
void GGroupVector::UpdateRefs(void) const throw(GException)
{
	if(Community)
		AddRefs(otGroup,Lang);
}


//------------------------------------------------------------------------------
void GGroupVector::RemoveRefs(void) const throw(GException)
{
	if(Community)
		DelRefs(otGroup,Lang);
}


//------------------------------------------------------------------------------
GGroupVector::~GGroupVector(void) throw(GException)
{
	RemoveRefs();
}
