/*

	GALILEI Research Project

	GGroupVector.cpp

	Group in the Vector Model- Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroupvector.h>
#include <profiles/gsubprofilevector.h>
#include <docs/gdocvector.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGroupVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupVector::GGroupVector(const unsigned int id,GLang* lang) throw(bad_alloc)
	: GGroup(id,lang), GWeightInfos(60)
{
}


//-----------------------------------------------------------------------------
void GGroupVector::UpdateRefs(void) const
{
	AddRefs(otGroup,Lang);
}


//-----------------------------------------------------------------------------
void GGroupVector::RemoveRefs(void) const
{
	DelRefs(otGroup,Lang);
}


//-----------------------------------------------------------------------------
unsigned int GGroupVector::GetNbNoNull(void) const
{
	return(GWeightInfos::NbPtr);
}


//-----------------------------------------------------------------------------
double GGroupVector::Similarity(const GGroup* desc) const
{
	return(GWeightInfos::Similarity((dynamic_cast<const GGroupVector*>(desc))));
}


//-----------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GGroup* desc) const
{
	return(GWeightInfos::SimilarityIFF((dynamic_cast<const GGroupVector*>(desc)),otGroup,Lang));
}


//-----------------------------------------------------------------------------
double GGroupVector::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//-----------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GDoc* doc) const
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GDocVector*>(doc),otDocGroup,Lang));
}


//-----------------------------------------------------------------------------
double GGroupVector::Similarity(const GSubProfile* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GSubProfileVector*>(doc)));
}


//-----------------------------------------------------------------------------
double GGroupVector::SimilarityIFF(const GSubProfile* doc) const
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GSubProfileVector*>(doc),otSubProfileGroup,Lang));
}


//-----------------------------------------------------------------------------
GWeightInfoCursor& GGroupVector::GetWeightInfoCursor(void)
{
	GWeightInfoCursor *cur=GWeightInfoCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GGroupVector::AddWord(GWeightInfo* word)
{
	GWeightInfos::InsertPtr(word);
}


//-----------------------------------------------------------------------------
void GGroupVector::AddWordList(unsigned id,double w)
{
	GWeightInfos::InsertPtr(new GWeightInfo(id,w,infoWordList));
}

//-----------------------------------------------------------------------------
GGroupVector::~GGroupVector(void)
{
	RemoveRefs();
}
