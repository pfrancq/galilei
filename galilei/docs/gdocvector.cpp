/*

	GALILEI Research Project

	GDocVector.cpp

	Document in the Vector model - Implementation.

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
#include <docs/gdocvector.h>
#include <profiles/gsubprofilevector.h>
#include <groups/ggroupvector.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GDocVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocVector::GDocVector(const char* url,const char* name,unsigned int id,GLang* lang,const char* mime,const char* u,const char* a,unsigned int f,unsigned int nbf) throw(bad_alloc)
	: GDoc(url,name,id,lang,mime,u,a,f,nbf),
	  GWeightInfos(100)
{
}


//------------------------------------------------------------------------------
bool GDocVector::IsDefined(void) const
{
	return(!GWeightInfos::IsEmpty());
}


//------------------------------------------------------------------------------
void GDocVector::ClearInfos(bool l)
{
	GDoc::ClearInfos(l);
	RemoveRefs();
	Clear();
}


//------------------------------------------------------------------------------
void GDocVector::AddInfo(GWeightInfo* info) throw(bad_alloc)
{
	InsertPtr(info);
}


//------------------------------------------------------------------------------
GWeightInfoCursor& GDocVector::GetWeightInfoCursor(void)
{
	GWeightInfoCursor *cur=GWeightInfoCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
double GDocVector::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//------------------------------------------------------------------------------
double GDocVector::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GDocVector*>(doc),otDoc,Lang));
}


//------------------------------------------------------------------------------
double GDocVector::Similarity(const GSubProfile* sub) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GSubProfileVector*>(sub)));
}


//------------------------------------------------------------------------------
double GDocVector::SimilarityIFF(const GSubProfile* sub) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GSubProfileVector*>(sub),otDocSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GDocVector::Similarity(const GGroup* grp) const
{
	return(GWeightInfos::Similarity(dynamic_cast<const GGroupVector*>(grp)));
}


//------------------------------------------------------------------------------
double GDocVector::SimilarityIFF(const GGroup* grp) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(dynamic_cast<const GGroupVector*>(grp),otDocGroup,Lang));
}


//------------------------------------------------------------------------------
void GDocVector::UpdateRefs(void) const throw(GException)
{
	if(!Lang) return;
	AddRefs(otDoc,Lang);
}


//------------------------------------------------------------------------------
void GDocVector::RemoveRefs(void) const throw(GException)
{
	if(!Lang) return;
	DelRefs(otDoc,Lang);
}


//------------------------------------------------------------------------------
GDocVector::~GDocVector(void) throw(GException)
{
	RemoveRefs();
}
