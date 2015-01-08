/*

	GALILEI Research Project

	GConcept.cpp

	Concept - Implementation.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for C/C++ ANSI
#include <cmath>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gconcept.h>
#include <gconcepttype.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GConcept
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConcept::GConcept(const GConcept* concept)
	: GObject(concept), Type(concept->Type),
	  IndexDocs(concept->IndexDocs),
	  IndexProfiles(concept->IndexProfiles),
	  IndexCommunities(concept->IndexCommunities),
	  IndexTopics(concept->IndexTopics),
	  IndexClasses(concept->IndexClasses)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(GSession* session,const RString& name,GConceptType* type)
	: GObject(session,cNoRef,name,otConcept), Type(type),
	  IndexDocs(0),
	  IndexProfiles(0),
	  IndexCommunities(0),
	  IndexTopics(0),
	  IndexClasses(0)
{
	if(!type)
		mThrowGException("Cannot create a concept no type");
}


//-----------------------------------------------------------------------------
GConcept::GConcept(GSession* session,size_t id, const R::RString& name, GConceptType* type,
		size_t idxdocs,
		size_t idxprofiles,
		size_t idxcommunities,
		size_t idxtopics,
		size_t idxclasses)
	: GObject(session,id,name,otConcept), Type(type),
	  IndexDocs(idxdocs),
	  IndexProfiles(idxprofiles),
	  IndexCommunities(idxcommunities),
	  IndexTopics(idxtopics),
	  IndexClasses(idxclasses)
{
}


//-----------------------------------------------------------------------------
int GConcept::Compare(const GConcept& c) const
{
	return(GetName().Compare(c.GetName()));
}


//-----------------------------------------------------------------------------
int GConcept::Compare(const R::RString& name) const
{
	return(GetName().Compare(name));
}


//-----------------------------------------------------------------------------
GConcept* GConcept::DeepCopy(void) const
{
	return(new GConcept(this));
}


//-----------------------------------------------------------------------------
void GConcept::SetId(size_t id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
size_t GConcept::GetIndex(tObjType ObjType) const
{
	switch(ObjType)
	{
		case otDoc:
			return(IndexDocs);
		case otProfile:
			return(IndexProfiles);
		case otCommunity:
			return(IndexCommunities);
		case otTopic:
			return(IndexTopics);
		case otClass:
			return(IndexClasses);
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
	}
}


//-----------------------------------------------------------------------------
void GConcept::SetIndex(tObjType ObjType,size_t index)
{
	switch(ObjType)
	{
		case otDoc:
			IndexDocs=index;
			break;
		case otProfile:
			IndexProfiles=index;
			break;
		case otCommunity:
			IndexCommunities=index;
			break;
		case otTopic:
			IndexTopics=index;
			break;
		case otClass:
			IndexClasses=index;
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
	}
	if(Session->MustSaveResults())
		Session->GetStorage()->SaveIndex(this,ObjType,index);
}


//-----------------------------------------------------------------------------
void GConcept::ClearIndex(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			IndexDocs=0;
			break;
		case otProfile:
			IndexProfiles=0;
			break;
		case otCommunity:
			IndexCommunities=0;
			break;
		case otTopic:
			IndexTopics=0;
			break;
		case otClass:
			IndexClasses=0;
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
	}
}


//-----------------------------------------------------------------------------
void GConcept::Clear(void)
{
	Id=cNoRef;
	Name=RString::Null;
	IndexDocs=0;
	IndexProfiles=0;
	IndexCommunities=0;
	IndexTopics=0;
	IndexClasses=0;
	Type=0;
}


//-----------------------------------------------------------------------------
bool GConcept::IsEmpty(void) const
{
	return(GetName().IsEmpty());
}


//-----------------------------------------------------------------------------
GConcept::~GConcept(void)
{
}
