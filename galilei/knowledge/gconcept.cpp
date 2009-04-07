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
#include <grelation.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GConcept
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConcept::GConcept(void)
	: Id(cNoRef), Name(RString::Null), Type(0), NbRefDocs(0), IfDocs(NAN), IndexDocs(9223372036854775807),
	  NbRefProfiles(0), IfProfiles(NAN), NbRefCommunities(0), IfCommunities(NAN),
	  NbRefTopics(0), IfTopics(NAN)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const GConcept* concept)
	: Id(concept->Id), Name(concept->GetName()), Type(concept->Type),
	  NbRefDocs(concept->NbRefDocs), IfDocs(concept->IfDocs), IndexDocs(concept->IndexDocs),
	  NbRefProfiles(concept->NbRefProfiles), IfProfiles(concept->IfProfiles),
	  NbRefCommunities(concept->NbRefCommunities), IfCommunities(concept->IfCommunities),
	  NbRefTopics(concept->NbRefTopics), IfTopics(concept->IfTopics)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const RString& name,GConceptType* type)
	: Id(cNoRef), Name(name), Type(type), NbRefDocs(0), IfDocs(NAN), IndexDocs(9223372036854775807),
	  NbRefProfiles(0), IfProfiles(NAN), NbRefCommunities(0), IfCommunities(NAN),
	  NbRefTopics(0), IfTopics(NAN)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(size_t id,const RString& name,GConceptType* type,size_t refdocs,off_t indexdocs,size_t refprofiles,size_t refcommunities,size_t reftopics)
	: Id(id), Name(name), Type(type), NbRefDocs(refdocs), IfDocs(NAN), IndexDocs(indexdocs),
	  NbRefProfiles(refprofiles), IfProfiles(NAN), NbRefCommunities(refcommunities), IfCommunities(NAN),
	  NbRefTopics(reftopics), IfTopics(NAN)
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
int GConcept::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//-----------------------------------------------------------------------------
bool GConcept::operator==(const GConcept& c) const
{
	if((Type!=c.Type)||(Id!=c.Id))
		return(false);
	return(true);
}


//-----------------------------------------------------------------------------
bool GConcept::operator!=(const GConcept& c) const
{
	if((Type==c.Type)&&(Id==c.Id))
		return(false);
	return(true);
}


//-----------------------------------------------------------------------------
bool GConcept::operator<(const GConcept& c) const
{
	if((Type<c.Type)||((Type==c.Type)&&((Id<c.Id))))
		return(true);
	return(false);
}


//-----------------------------------------------------------------------------
bool GConcept::operator>(const GConcept& c) const
{
	if((Type>c.Type)||((Type==c.Type)&&((Id>c.Id))))
		return(true);
	return(false);
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
void GConcept::SetName(const R::RString& name)
{
	if(Id!=cNoRef)
		return;
	Name=name;
	NbRefDocs=0;
	IfDocs=NAN;
	NbRefProfiles=0;
	IndexDocs=9223372036854775807;
	IfProfiles=NAN;
	NbRefCommunities=0;
	IfCommunities=NAN;
	NbRefTopics=0;
	IfTopics=NAN;
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
		default:
			throw GException("Unknown type to increase concept "+RString::Number(Id));
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
				throw GException("Cannot decrease null number of references for documents for concept "+RString::Number(Id));
			NbRefDocs--;
			IfDocs=NAN;
			return(NbRefDocs);
			break;
		case otProfile:
			if(!NbRefProfiles)
				throw GException("Cannot decrease null number of references for profiles for concept "+RString::Number(Id));
			NbRefProfiles--;
			IfProfiles=NAN;
			return(NbRefProfiles);
			break;
		case otCommunity:
			if(!NbRefCommunities)
				throw GException("Cannot decrease null number of references for groups for concept "+RString::Number(Id));
			NbRefCommunities--;
			IfCommunities=NAN;
			return(NbRefCommunities);
			break;
		case otTopic:
			if(!NbRefTopics)
				throw GException("Cannot decrease null number of references for topics for concept "+RString::Number(Id));
			NbRefTopics--;
			IfTopics=NAN;
			return(NbRefTopics);
			break;
		default:
			throw GException("Unknown type to decrease concept "+RString::Number(Id));
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
		default:
			return(NbRefDocs+NbRefProfiles+NbRefCommunities);
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
		default:
			throw GException("GConcept::GetIf : Unknown type for concept "+RString::Number(Id));
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
			IndexDocs=9223372036854775807;
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
		default:
			NbRefDocs=0;
			IfDocs=NAN;
			NbRefProfiles=0;
			IfProfiles=NAN;
			NbRefCommunities=0;
			IfCommunities=NAN;
			NbRefTopics=0;
			IfTopics=NAN;
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
	IndexDocs=9223372036854775807;
	NbRefProfiles=0;
	IfProfiles=NAN;
	NbRefCommunities=0;
	IfCommunities=NAN;
	NbRefTopics=0;
	IfTopics=NAN;
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
