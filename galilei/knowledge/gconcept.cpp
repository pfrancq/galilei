/*

	GALILEI Research Project

	GConcept.cpp

	Concept - Implementation.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
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
	  NbRefDocs(concept->NbRefDocs), IfDocs(concept->IfDocs), IndexDocs(concept->IndexDocs),
	  NbRefProfiles(concept->NbRefProfiles), IfProfiles(concept->IfProfiles), IndexProfiles(concept->IndexProfiles),
	  NbRefCommunities(concept->NbRefCommunities), IfCommunities(concept->IfCommunities), IndexCommunities(concept->IndexCommunities),
	  NbRefTopics(concept->NbRefTopics), IfTopics(concept->IfTopics), IndexTopics(concept->IndexTopics),
	  NbRefClasses(concept->NbRefClasses), IfClasses(concept->IfClasses), IndexClasses(concept->IndexClasses)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(GSession* session,const RString& name,GConceptType* type)
	: GObject(session,cNoRef,name,otConcept), Type(type),
	  NbRefDocs(0), IfDocs(NAN), IndexDocs(0),
	  NbRefProfiles(0), IfProfiles(NAN), IndexProfiles(0),
	  NbRefCommunities(0), IfCommunities(NAN), IndexCommunities(0),
	  NbRefTopics(0), IfTopics(NAN), IndexTopics(0),
	  NbRefClasses(0), IfClasses(NAN), IndexClasses(0)
{
	if(!type)
		mThrowGException("Cannot create a concept no type");
}


//-----------------------------------------------------------------------------
GConcept::GConcept(GSession* session,size_t id, const R::RString& name, GConceptType* type,
		size_t refdocs, size_t idxdocs,
		size_t refprofiles, size_t idxprofiles,
		size_t refcommunities, size_t idxcommunities,
		size_t reftopics, size_t idxtopics,
		size_t refclasses, size_t idxclasses)
	: GObject(session,id,name,otConcept), Type(type),
	  NbRefDocs(refdocs), IfDocs(NAN), IndexDocs(idxdocs),
	  NbRefProfiles(refprofiles), IfProfiles(NAN), IndexProfiles(idxprofiles),
	  NbRefCommunities(refcommunities), IfCommunities(NAN), IndexCommunities(idxcommunities),
	  NbRefTopics(reftopics), IfTopics(NAN), IndexTopics(idxtopics),
	  NbRefClasses(refclasses), IfClasses(NAN), IndexClasses(idxclasses)
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
size_t GConcept::IncRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs++;
			IfDocs=NAN;
			return(NbRefDocs);
			break;
		case otProfile:
			NbRefProfiles++;
			IfProfiles=NAN;
			return(NbRefProfiles);
			break;
		case otCommunity:
			NbRefCommunities++;
			IfCommunities=NAN;
			return(NbRefCommunities);
			break;
		case otTopic:
			NbRefTopics++;
			IfTopics=NAN;
			return(NbRefTopics);
			break;
		case otClass:
			NbRefClasses++;
			IfClasses=NAN;
			return(NbRefClasses);
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
size_t GConcept::DecRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				mThrowGException("Cannot decrease null number of references for documents for concept "+RString::Number(Id));
			NbRefDocs--;
			IfDocs=NAN;
			return(NbRefDocs);
			break;
		case otProfile:
			if(!NbRefProfiles)
				mThrowGException("Cannot decrease null number of references for profiles for concept "+RString::Number(Id));
			NbRefProfiles--;
			IfProfiles=NAN;
			return(NbRefProfiles);
			break;
		case otCommunity:
			if(!NbRefCommunities)
				mThrowGException("Cannot decrease null number of references for groups for concept "+RString::Number(Id));
			NbRefCommunities--;
			IfCommunities=NAN;
			return(NbRefCommunities);
			break;
		case otTopic:
			if(!NbRefTopics)
				mThrowGException("Cannot decrease null number of references for topics for concept "+RString::Number(Id));
			NbRefTopics--;
			IfTopics=NAN;
			return(NbRefTopics);
			break;
		case otClass:
			if(!NbRefClasses)
				mThrowGException("Cannot decrease null number of references for classes for concept "+RString::Number(Id));
			NbRefClasses--;
			IfClasses=NAN;
			return(NbRefClasses);
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
void GConcept::ClearRefs(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			IfDocs=NAN;
			IndexDocs=0;
			break;
		case otProfile:
			NbRefProfiles=0;
			IfProfiles=NAN;
			break;
		case otCommunity:
			NbRefCommunities=0;
			IfCommunities=NAN;
			break;
		case otTopic:
			NbRefTopics=0;
			IfTopics=NAN;
			break;
		case otClass:
			NbRefClasses=0;
			IfClasses=NAN;
			break;
		default:
			mThrowGException(GALILEI::GetObjType(ObjType,true,true)+" have no references");
			break;
	}
}


//-----------------------------------------------------------------------------
size_t GConcept::GetRef(tObjType ObjType) const
{
	switch(ObjType)
	{
		case otDoc:
			return(NbRefDocs);
			break;
		case otProfile:
			return(NbRefProfiles);
			break;
		case otCommunity:
			return(NbRefCommunities);
			break;
		case otTopic:
			return(NbRefTopics);
			break;
		case otClass:
			return(NbRefClasses);
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
			break;
	}
	return(0);
}


//-----------------------------------------------------------------------------
double GConcept::GetIF(tObjType ObjType) const
{
	switch(ObjType)
	{
		case otDoc:
			if(IfDocs!=IfDocs)
			{
				if(Type->NbRefDocs&&NbRefDocs)
					const_cast<GConcept*>(this)->IfDocs=log10(static_cast<double>(Type->NbRefDocs)/static_cast<double>(NbRefDocs));
				else
					const_cast<GConcept*>(this)->IfDocs=0.0;
			}
			return(IfDocs);
			break;
		case otProfile:
			if(IfProfiles!=IfProfiles)
			{
				if(Type->NbRefProfiles&&NbRefProfiles)
					const_cast<GConcept*>(this)->IfProfiles=log10(static_cast<double>(Type->NbRefProfiles)/static_cast<double>(NbRefProfiles));
				else
					const_cast<GConcept*>(this)->IfProfiles=0.0;
			}
			return(IfProfiles);
			break;
		case otCommunity:
			if(IfCommunities!=IfCommunities)
			{
				if(Type->NbRefCommunities&&NbRefCommunities)
					const_cast<GConcept*>(this)->IfCommunities=log10(static_cast<double>(Type->NbRefCommunities)/static_cast<double>(NbRefCommunities));
				else
					const_cast<GConcept*>(this)->IfCommunities=0.0;
			}
			return(IfCommunities);
			break;
		case otTopic:
			if(IfTopics!=IfTopics)
			{
				if(Type->NbRefTopics&&NbRefTopics)
					const_cast<GConcept*>(this)->IfTopics=log10(static_cast<double>(Type->NbRefTopics)/static_cast<double>(NbRefTopics));
				else
					const_cast<GConcept*>(this)->IfTopics=0.0;
			}
			return(IfTopics);
			break;
		case otClass:
			if(IfClasses!=IfClasses)
			{
				if(Type->NbRefClasses&&NbRefClasses)
					const_cast<GConcept*>(this)->IfClasses=log10(static_cast<double>(Type->NbRefClasses)/static_cast<double>(NbRefClasses));
				else
					const_cast<GConcept*>(this)->IfClasses=0.0;
			}
			return(IfClasses);
			break;
		default:
			mThrowGException("Unknown type for concept "+RString::Number(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
void GConcept::Clear(void)
{
	Id=cNoRef;
	Name=RString::Null;
	NbRefDocs=0;
	IfDocs=NAN;
	IndexDocs=0;
	NbRefProfiles=0;
	IfProfiles=NAN;
	IndexProfiles=0;
	NbRefCommunities=0;
	IfCommunities=NAN;
	IndexCommunities=0;
	NbRefTopics=0;
	IfTopics=NAN;
	IndexTopics=0;
	NbRefClasses=0;
	IfClasses=NAN;
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
