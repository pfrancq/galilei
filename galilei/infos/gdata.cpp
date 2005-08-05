/*

	GALILEI Research Project

	GData.cpp

	 (or stem) - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
// include file for Galilei
#include <gdata.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GData::GData(unsigned id,const RString& name,tInfoType type,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
	: Id(id), Name(name), Type(type), NbRefDocs(refdocs), NbRefSubProfiles(refsubprofiles),
	  NbRefGroups(refgroups)
{
}


//-----------------------------------------------------------------------------
GData::GData(const GData& d)
	: Id(d.Id), Name(d.Name), Type(d.Type), NbRefDocs(d.NbRefDocs),
	  NbRefSubProfiles(d.NbRefSubProfiles), NbRefGroups(d.NbRefGroups)
{
}


//-----------------------------------------------------------------------------
int GData::Compare(const GData& d) const
{
	return(Name.Compare(d.Name));
}


//-----------------------------------------------------------------------------
int GData::Compare(const GData* d) const
{
	return(Name.Compare(d->Name));
}


//-----------------------------------------------------------------------------
int GData::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int GData::Compare(const char* name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int GData::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
GData& GData::operator=(const GData& d)
{
	Id=d.Id;
	Name=d.Name;
	Type=d.Type;
	NbRefDocs=d.NbRefDocs;
	NbRefSubProfiles=d.NbRefSubProfiles;
	NbRefGroups=d.NbRefGroups;
	return(*this);
}


//-----------------------------------------------------------------------------
void GData::SetId(unsigned int id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
RString GData::GetName(void) const
{
	return(Name);
}


//-----------------------------------------------------------------------------
unsigned int GData::IncRef(tObjType ObjType)
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
			throw GException ("Unkown type to increase data "+itou(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
unsigned int GData::DecRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				throw GException("Cannot decrease null number of references for documents for data "+itou(Id));
			return(--NbRefDocs);
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references for subprofiles for data "+itou(Id));
			return(--NbRefSubProfiles);
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups for data "+itou(Id));
			return(--NbRefGroups);
			break;
		default:
			throw GException ("Unkown type to decrease data "+itou(Id));
			break;
	}
}


//-----------------------------------------------------------------------------
unsigned int GData::GetRef(tObjType ObjType) const
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
void GData::Clear(tObjType ObjType)
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
void GData::Clear(void)
{
	Id=cNoRef;
	Name="";
	NbRefDocs=0;
	NbRefSubProfiles=0;
	NbRefGroups=0;
	Type=infoNothing;
}


//-----------------------------------------------------------------------------
bool GData::IsEmpty(void) const
{
	return(Name.IsEmpty());
}


//-----------------------------------------------------------------------------
GData::~GData(void)
{
}


