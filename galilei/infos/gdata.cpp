/*

	GALILEI Research Project

	GData.cpp

	 (or stem) - Implementation.

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



//-----------------------------------------------------------------------------
// include file for Galilei
#include <infos/gdata.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GData::GData(const RString& name,GInfoType type) throw(bad_alloc)
	: Id(cNoRef), Name(name), Type(type), NbRefDocs(0), NbRefSubProfiles(0),
	  NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GData::GData(unsigned id,const RString& name,GInfoType type) throw(bad_alloc)
	: Id(id), Name(name), Type(type), NbRefDocs(0), NbRefSubProfiles(0),
	  NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GData::GData(const GData& d) throw(bad_alloc)
	: Id(d.Id), Name(d.Name), Type(d.Type), NbRefDocs(d.NbRefDocs),
	  NbRefSubProfiles(d.NbRefSubProfiles), NbRefGroups(d.NbRefGroups)
{
}


//-----------------------------------------------------------------------------
GData::GData(const GData* d) throw(bad_alloc)
  : Id(d->Id), Name(d->Name), Type(d->Type), NbRefDocs(d->NbRefDocs),
    NbRefSubProfiles(d->NbRefSubProfiles), NbRefGroups(d->NbRefGroups)
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
GData& GData::operator=(const GData& d) throw(bad_alloc)
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
RString& GData::GetName(void) const
{
	RString* tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//-----------------------------------------------------------------------------
void GData::IncRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs++;
			break;
		case otSubProfile:
			NbRefSubProfiles++;
			break;
		case otGroup:
			NbRefGroups++;
			break;
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void GData::DecRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs--;
			break;
		case otSubProfile:
			NbRefSubProfiles--;
			break;
		case otGroup:
			NbRefGroups--;
			break;
		default:
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


