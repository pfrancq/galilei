/*

	GALILEI Research Project

	GConcpet.cpp

	Concept - Implementation.

	Copyright 2006-2007 by the Université Libre de Bruxelles.

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
	  NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(const RString& name,GConceptType* type)
	: Id(cNoRef), Name(name), Type(type), NbRefDocs(0),
	  NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GConcept::GConcept(unsigned int id,const RString& name,GConceptType* type,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
	: Id(id), Name(name), Type(type), NbRefDocs(refdocs),
	  NbRefSubProfiles(refsubprofiles), NbRefGroups(refgroups)
{
}


//-----------------------------------------------------------------------------
/*GConcept::GConcept(const GConcept& c)
	: Id(c.Id), Name(c.Name), Type(c.Type), NbRefDocs(c.NbRefDocs),
	  NbRefSubProfiles(c.NbRefSubProfiles), NbRefGroups(c.NbRefGroups)
{
}
*/

//-----------------------------------------------------------------------------
int GConcept::Compare(const GConcept& c) const
{
	return(Id-c.Id);
}


//-----------------------------------------------------------------------------
int GConcept::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int GConcept::Compare(size_t id) const
{
	return(Id-id);
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
GConcept& GConcept::operator=(const GConcept& c)
{
	Id=c.Id;
	Name=c.Name;
	Type=c.Type;
	NbRefDocs=c.NbRefDocs;
	NbRefSubProfiles=c.NbRefSubProfiles;
	NbRefGroups=c.NbRefGroups;
	return(*this);
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
			return(++NbRefDocs);
			break;
		case otSubProfile:
			return(++NbRefSubProfiles);
			break;
		case otGroup:
			return(++NbRefGroups);
			break;
		default:
			throw GException ("Unkown type to increase concept "+RString::Number(Id));
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
			return(--NbRefDocs);
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references for subprofiles for concept "+RString::Number(Id));
			return(--NbRefSubProfiles);
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups for concept "+RString::Number(Id));
			return(--NbRefGroups);
			break;
		default:
			throw GException ("Unkown type to decrease concept "+RString::Number(Id));
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
		case otSubProfile:
			return(NbRefSubProfiles);
			break;
		case otGroup:
			return(NbRefGroups);
			break;
		case otDocSubProfile:
			return(NbRefDocs+NbRefSubProfiles);
			break;
		case otDocGroup:
			return(NbRefDocs+NbRefGroups);
			break;
		case otSubProfileGroup:
			return(NbRefSubProfiles+NbRefGroups);
			break;
		default:
			return(NbRefDocs+NbRefSubProfiles+NbRefGroups);
			break;
	}
	return(0);
}


//-----------------------------------------------------------------------------
void GConcept::Clear(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otSubProfile:
			NbRefSubProfiles=0;
			break;
		case otGroup:
			NbRefGroups=0;
			break;
		case otDocSubProfile:
			NbRefDocs=0;
			NbRefSubProfiles=0;
			break;
		case otDocGroup:
			NbRefDocs=0;
			NbRefGroups=0;
			break;
		case otSubProfileGroup:
			NbRefSubProfiles=0;
			NbRefGroups=0;
			break;
		default:
			NbRefDocs=0;
			NbRefSubProfiles=0;
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
	NbRefSubProfiles=0;
	NbRefGroups=0;
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
