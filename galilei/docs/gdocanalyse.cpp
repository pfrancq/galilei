/*

	GALILEI Research Project

	GDocAnalyse.cpp

	AGeneric Document Analysis - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocanalyse.h>
#include <docs/gdocvector.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// Constance
const unsigned int MaxWordLen=50;



//------------------------------------------------------------------------------
//
// class GDocAnalyse
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocAnalyse::GDocAnalyse(GFactoryDocAnalyse* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryDocAnalyse>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GDocAnalyse::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GDocAnalyse::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
void GDocAnalyse::UpdateFdbks(GLang* oldlang, GLang* newlang, GDoc* doc) throw(GException)
{
	GProfDocCursor profdoccursor;

	if(!doc)
		throw GException("No document selected");

	// if the old lang and the new lang are not defined.
	if (!oldlang&&!newlang)
		return;

	// if the oldlang is different to the new lang.
	if (oldlang!=newlang)
	{
		profdoccursor=doc->GetProfDocCursor();
		for (profdoccursor.Start(); !profdoccursor.End(); profdoccursor.Next())
			profdoccursor()->GetProfile()->DispatchFdbks(profdoccursor(), oldlang,Session);
	}
}


//------------------------------------------------------------------------------
GDocAnalyse::~GDocAnalyse(void)
{
}
