/*

	GALILEI Research Project

	GDocAnalyse.cpp

	Analyse a document - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocanalyse.h>
#include <docs/gdocvector.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>

using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Constance
const unsigned int MaxWordLen=50;



//-----------------------------------------------------------------------------
//
// class GDocAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocAnalyse::GDocAnalyse(GFactoryDocAnalyse* fac) throw(bad_alloc)
	: GPlugin<GFactoryDocAnalyse>(fac), Session(0)
{
}


//-----------------------------------------------------------------------------
void GDocAnalyse::Connect(GSession* session)
{
	Session=session;
}


//-----------------------------------------------------------------------------
void GDocAnalyse::Disconnect(GSession*)
{
	Session=0;
}


//-----------------------------------------------------------------------------
void GDocAnalyse::Analyse(GDocXML*,GDoc* doc,R::RContainer<GDoc,unsigned int,false,true>*) throw(GException)
{
	Doc= dynamic_cast<GDocVector*>(doc) ;
}


//-----------------------------------------------------------------------------
void GDocAnalyse::UpdateFdbks(GLang* oldlang, GLang* newlang)
{
	GProfDocCursor profdoccursor;

	// if the old lang and the new lang are not defined.
	if (!oldlang&&!newlang)
		return;

	// if the oldlang is different to the new lang.
	if (oldlang!=newlang)
	{
		profdoccursor=Doc->GetProfDocCursor();
		for (profdoccursor.Start(); !profdoccursor.End(); profdoccursor.Next())
			profdoccursor()->GetProfile()->DispatchFdbks(profdoccursor(), oldlang,Session);
	}
}


//-----------------------------------------------------------------------------
GDocAnalyse::~GDocAnalyse(void)
{
}
