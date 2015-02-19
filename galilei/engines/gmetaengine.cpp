/*

	GALILEI Research Project

	GMetaEngine.cpp

	Meta-Engine - Implementation.

   Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#include <gengine.h>
#include <gdoc.h>
#include <gsession.h>
#include <gmetaengine.h>
#include <gfilter.h>
#include <gdocfragment.h>
#include <gdocref.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::GMetaEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), RDownloadFile(), Results(200)
{
}


//------------------------------------------------------------------------------
void GMetaEngine::AddResult(size_t docid,size_t pos,size_t first,size_t last,double ranking,const GEngine* engine)
{
	if((ranking<0)||(ranking>1))
		mThrowGException("Ranking must be included in [0,1]");

	// Find the document reference
	GDoc* Doc(Session->GetObj(pDoc,docid));
	RURI URI(Doc->GetURI());
	if(!Doc)
		mThrowGException("Unknown document");
	GDocRef* Ref(Results.GetInsertPtr(Doc));

	// Insert the fragment
	Ref->AddFragment(pos,first,last,ranking,engine->GetName());
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRef> GMetaEngine::GetResults(void)
{
	return(R::RCursor<GDocRef>(Results));
}


//-----------------------------------------------------------------------------
size_t GMetaEngine::GetNbResults(void) const
{
	return(Results.GetNb());
}


//------------------------------------------------------------------------------
GMetaEngine::~GMetaEngine(void)
{
}
