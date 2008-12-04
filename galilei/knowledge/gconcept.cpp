/*

	GALILEI Research Project

	GConcept.cpp

	Concept - Implementation.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for GALILEI
#include <gconcept.h>
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
	: Id(cNoRef), Name(RString::Null), Type(0), NbRefDocs(0),
	  NbRefProfiles(0), NbRefGroups(0), NbRefTopics(0)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const GConcept* concept)
	: Id(concept->Id), Name(concept->GetName()), Type(concept->Type), NbRefDocs(concept->NbRefDocs),
	  NbRefProfiles(concept->NbRefProfiles), NbRefGroups(concept->NbRefGroups), NbRefTopics(concept->NbRefTopics)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const RString& name,GConceptType* type)
	: Id(cNoRef), Name(name), Type(type), NbRefDocs(0),
	  NbRefProfiles(0), NbRefGroups(0), NbRefTopics(0)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(size_t id,const RString& name,GConceptType* type,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics)
	: Id(id), Name(name), Type(type), NbRefDocs(refdocs),
	  NbRefProfiles(refprofiles), NbRefGroups(refgroups), NbRefTopics(reftopics)
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
	NbRefProfiles=0;
	NbRefGroups=0;
	NbRefTopics=0;
}


//-----------------------------------------------------------------------------
size_t GConcept::IncRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs++;
			return(NbRefDocs);
			break;
		case otProfile:
			NbRefProfiles++;
			return(NbRefProfiles);
			break;
		case otCommunity:
			NbRefGroups++;
			return(NbRefGroups);
			break;
		case otTopic:
			NbRefTopics++;
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
			return(NbRefDocs);
			break;
		case otProfile:
			if(!NbRefProfiles)
				throw GException("Cannot decrease null number of references for profiles for concept "+RString::Number(Id));
			NbRefProfiles--;
			return(NbRefProfiles);
			break;
		case otCommunity:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups for concept "+RString::Number(Id));
			NbRefGroups--;
			return(NbRefGroups);
			break;
		case otTopic:
			if(!NbRefTopics)
				throw GException("Cannot decrease null number of references for topics for concept "+RString::Number(Id));
			NbRefTopics--;
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
			return(NbRefGroups);
			break;
		case otTopic:
			return(NbRefTopics);
			break;
		default:
			return(NbRefDocs+NbRefProfiles+NbRefGroups);
			break;
	}
	return(0);
}


//-----------------------------------------------------------------------------
void GConcept::ClearRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otProfile:
			NbRefProfiles=0;
			break;
		case otCommunity:
			NbRefGroups=0;
			break;
		case otTopic:
			NbRefTopics=0;
			break;
		default:
			NbRefDocs=0;
			NbRefProfiles=0;
			NbRefGroups=0;
			break;
	}
}


//-----------------------------------------------------------------------------
void GConcept::Clear(void)
{
	Id=cNoRef;
	Name=RString::Null;
	NbRefDocs=0;
	NbRefProfiles=0;
	NbRefGroups=0;
	NbRefTopics=0;
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
