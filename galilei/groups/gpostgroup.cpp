/*

	GALILEI Research Project

	GPostGroup.cpp

	Generic Post-Group Computing Method - Implementation.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be).

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
#include <groups/gpostgroup.h>
#include <groups/gpostgroupmanager.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GPostGroup
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostGroup::GPostGroup(GFactoryPostGroup* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryPostGroup>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GPostGroup::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GPostGroup::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
GPostGroup::~GPostGroup(void)
{
}

//------------------------------------------------------------------------------
//
//  GFactoryPostGroup
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFactoryPostGroup::GFactoryPostGroup(GPostGroupManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPostGroup,GPostGroup,GPostGroupManager>(mng,n,f)
{
	//insert a "Level" parameter
	InsertPtr(new GParamUInt("Level",mng->NbPtr));
}


//------------------------------------------------------------------------------
int GFactoryPostGroup::Compare(const GFactoryPostGroup& f) const 
{
	int a=GetUInt("Level")-f.GetUInt("Level");
	if(!a) 
		a=Name.Compare(f.Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPostGroup::Compare(const GFactoryPostGroup* f) const 
{
	int a=GetUInt("Level")-f->GetUInt("Level");
	if(!a) 
		a=Name.Compare(f->Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPostGroup::Compare(unsigned int level) const 
{
	return(GetUInt("Level")-level);
}


//------------------------------------------------------------------------------
int GFactoryPostGroup::sortOrder(const void* a,const void* b)
{
	unsigned int alevel=(*((GFactoryPostGroup**)(a)))->GetUInt("Level");
	unsigned int blevel=(*((GFactoryPostGroup**)(b)))->GetUInt("Level");

	if(alevel==blevel)
		return((*((GFactoryPostGroup**)(a)))->Name.Compare((*((GFactoryPostGroup**)(b)))->Name));
	if(alevel>blevel)
		return(1);
	else
		return(-1);
}
