/*

	GALILEI Research Project

	GConcept.cpp

	Concept - Implementation.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
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
	  NbRefProfiles(concept->NbRefProfiles), IfProfiles(concept->IfProfiles),
	  NbRefCommunities(concept->NbRefCommunities), IfCommunities(concept->IfCommunities),
	  NbRefTopics(concept->NbRefTopics), IfTopics(concept->IfTopics),
	  NbRefClasses(concept->NbRefClasses), IfClasses(concept->IfClasses)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const RString& name,GConceptType* type)
	: GObject(cNoRef,name,otConcept), Type(type), NbRefDocs(0), IfDocs(NAN), IndexDocs(0),
	  NbRefProfiles(0), IfProfiles(NAN), NbRefCommunities(0), IfCommunities(NAN),
	  NbRefTopics(0), IfTopics(NAN), NbRefClasses(0), IfClasses(NAN)
{
	if(!type)
		ThrowGException("Cannot create a concept no type");
}


//-----------------------------------------------------------------------------
GConcept::GConcept(size_t id,const RString& name,GConceptType* type,size_t refdocs,size_t indexdocs,size_t refprofiles,size_t refcommunities,size_t reftopics,size_t refclasses)
	: GObject(id,name,otConcept), Type(type), NbRefDocs(refdocs), IfDocs(NAN), IndexDocs(indexdocs),
	  NbRefProfiles(refprofiles), IfProfiles(NAN), NbRefCommunities(refcommunities), IfCommunities(NAN),
	  NbRefTopics(reftopics), IfTopics(NAN), NbRefClasses(refclasses), IfClasses(NAN)
{
}


//-----------------------------------------------------------------------------
int GConcept::Compare(const GConcept& c) const
{
	return(Name.Compare(c.Name));
}


//-----------------------------------------------------------------------------
int GConcept::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
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
			ThrowGException("Unknown type for concept "+RString::Number(Id));
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
				ThrowGException("Cannot decrease null number of references for documents for concept "+RString::Number(Id));
			NbRefDocs--;
			IfDocs=NAN;
			return(NbRefDocs);
			break;
		case otProfile:
			if(!NbRefProfiles)
				ThrowGException("Cannot decrease null number of references for profiles for concept "+RString::Number(Id));
			NbRefProfiles--;
			IfProfiles=NAN;
			return(NbRefProfiles);
			break;
		case otCommunity:
			if(!NbRefCommunities)
				ThrowGException("Cannot decrease null number of references for groups for concept "+RString::Number(Id));
			NbRefCommunities--;
			IfCommunities=NAN;
			return(NbRefCommunities);
			break;
		case otTopic:
			if(!NbRefTopics)
				ThrowGException("Cannot decrease null number of references for topics for concept "+RString::Number(Id));
			NbRefTopics--;
			IfTopics=NAN;
			return(NbRefTopics);
			break;
		case otClass:
			if(!NbRefClasses)
				ThrowGException("Cannot decrease null number of references for classes for concept "+RString::Number(Id));
			NbRefClasses--;
			IfClasses=NAN;
			return(NbRefClasses);
			break;
		default:
			ThrowGException("Unknown type for concept "+RString::Number(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
void GConcept::ClearRef(tObjType ObjType)
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
			ThrowGException("'"+GALILEI::GetObjType(ObjType)+"' is not a valid type");
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
		case otAnyClass:
			return(NbRefDocs+NbRefProfiles+NbRefCommunities+NbRefTopics+NbRefClasses);
			break;
		default:
			ThrowGException("Unknown type for concept "+RString::Number(Id));
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
			ThrowGException("Unknown type for concept "+RString::Number(Id));
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
	NbRefCommunities=0;
	IfCommunities=NAN;
	NbRefTopics=0;
	IfTopics=NAN;
	NbRefClasses=0;
	IfClasses=NAN;
	Type=0;
}


//-----------------------------------------------------------------------------
bool GConcept::IsEmpty(void) const
{
	return(Name.IsEmpty());
}


//-----------------------------------------------------------------------------
GConcept::~GConcept(void)
{
}
