/*

	GALILEI Research Project

	GPreProfile.cpp

	Generic Pre-Profiling Computing Method - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel(dwartel@ulb.ac.be).

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
#include <profiles/gpreprofile.h>
#include <profiles/gpreprofilemanager.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GPreProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPreProfile::GPreProfile(GFactoryPreProfile* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryPreProfile>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GPreProfile::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GPreProfile::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
GPreProfile::~GPreProfile(void)
{
}


//------------------------------------------------------------------------------
//
//  GFactoryPreProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFactoryPreProfile::GFactoryPreProfile(GPreProfileManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPreProfile,GPreProfile,GPreProfileManager>(mng,n,f,"GFactoryPreProfile")
{
	//insert a "Level" parameter
	InsertPtr(new GParamUInt("Level",mng->GetNb()));
}


//------------------------------------------------------------------------------
int GFactoryPreProfile::Compare(const GFactoryPreProfile& f) const
{
	int a=GetUInt("Level")-f.GetUInt("Level");
	if(!a)
		a=Name.Compare(f.Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPreProfile::Compare(const GFactoryPreProfile* f) const
{
	int a=GetUInt("Level")-f->GetUInt("Level");
	if(!a)
		a=Name.Compare(f->Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPreProfile::Compare(unsigned int level) const
{
	return(GetUInt("Level")-level);
}


//------------------------------------------------------------------------------
int GFactoryPreProfile::sortOrder(const void* a,const void* b)
{
	unsigned int alevel=(*((GFactoryPreProfile**)(a)))->GetUInt("Level");
	unsigned int blevel=(*((GFactoryPreProfile**)(b)))->GetUInt("Level");

	if(alevel==blevel)
		return((*((GFactoryPreProfile**)(a)))->Name.Compare((*((GFactoryPreProfile**)(b)))->Name));
	if(alevel>blevel)
		return(1);
	else
		return(-1);
}
