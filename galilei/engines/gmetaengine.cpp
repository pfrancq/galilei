/*

	GALILEI Research Project

	GMetaEngine.cpp

	Meta Engine for extraction of results from different search engines - Implementation.

	Copyright 2004 by the Université Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <engines/gengine.h>
#include <engines/gmetaengine.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::GMetaEngine(GFactoryMetaEngine* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryMetaEngine>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GMetaEngine::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GMetaEngine::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
GMetaEngine::~GMetaEngine(void)
{
}
