/*

	R Project Library

	GPostDocManager.cpp

	Documents Post-Analysis Methods Manager - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gpostdocmanager.h>
#include <docs/gpostdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GPostDocManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostDocManager::GPostDocManager(const char* path,bool dlg) throw(GException)
	: RContainer<GFactoryPostDoc,true,true>(10,5) ,GPluginManager("PostDoc",path)
{

	RString Path(path);
	Path+="/postdoc";

	LoadPlugins<GFactoryPostDoc,GFactoryPostDocInit,GPostDocManager>(this,Path.Latin1(),API_POSTDOC_VERSION, dlg);
}


//------------------------------------------------------------------------------
void GPostDocManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostDoc> Cur;
	GPostDoc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GPostDocManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostDoc> Cur;
	GPostDoc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryPostDoc> GPostDocManager::GetPostDocsCursor(void)
{
	R::RCursor<GFactoryPostDoc> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GPostDocManager::~GPostDocManager(void)
{
}
