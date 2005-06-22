/*
	GALILEI Research Project

	GPostDoc.cpp

	Generic Documents Post-Analysis Method - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors
		 Kumps Nicolas (nkumps@ulb.ac.be)

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
// include file for GALILEI
#include <docs/gpostdoc.h>
#include <docs/gpostdocmanager.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GPostDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostDoc::GPostDoc(GFactoryPostDoc* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryPostDoc>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GPostDoc::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GPostDoc::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
GPostDoc::~GPostDoc(void)
{
}


//------------------------------------------------------------------------------
//
//  GFactoryPostDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFactoryPostDoc::GFactoryPostDoc(GPostDocManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>(mng,n,f,"GFactoryPostDoc")
{
	//insert a "Level" parameter
	InsertPtr(new GParamUInt("Level",mng->GetNb()));
}


//------------------------------------------------------------------------------
int GFactoryPostDoc::Compare(const GFactoryPostDoc& f) const
{
	int a=GetUInt("Level")-f.GetUInt("Level");
	if(!a)
		a=Name.Compare(f.Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPostDoc::Compare(const GFactoryPostDoc* f) const
{
	int a=GetUInt("Level")-f->GetUInt("Level");
	if(!a)
		a=Name.Compare(f->Name);
	return(a);
}


//------------------------------------------------------------------------------
int GFactoryPostDoc::Compare(unsigned int level) const
{
	return(GetUInt("Level")-level);
}


//------------------------------------------------------------------------------
int GFactoryPostDoc::sortOrder(const void* a,const void* b)
{
	unsigned int alevel=(*((GFactoryPostDoc**)(a)))->GetUInt("Level");
	unsigned int blevel=(*((GFactoryPostDoc**)(b)))->GetUInt("Level");

	if(alevel==blevel)
		return((*((GFactoryPostDoc**)(a)))->Name.Compare((*((GFactoryPostDoc**)(b)))->Name));
	if(alevel>blevel)
		return(1);
	else
		return(-1);
}
