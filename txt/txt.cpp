/*

	GALILEI Research Project

	TXT.cpp

	Filter for text Files - Implementation.

	Copyright 2001 by Valery Vandaele.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for GALILEI
#include <txt.h>
#include <rxmlfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
//
// class GFilterTXT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterTXT::GFilterTXT(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac)
{
	AddMIME("text/plain");
}


//-----------------------------------------------------------------------------
void GFilterTXT::Analyze(GDoc* doc,const RURI& uri,RXMLParser* parser,GSlot* )
{
	// Init Part
	StartStream(parser);
	RTextFile Src(uri,"utf-8");
	Src.Open(R::RIO::Read);
	AddDublinCoreMetaData("identifier",doc->GetURL()());
	WriteMetaDataStream(parser);
	ExtractTextualContent(Src,parser);
	EndStream(parser);
}


//------------------------------------------------------------------------------
void GFilterTXT::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GFilterTXT::~GFilterTXT(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("Text Filter","Text Filter",GFilterTXT)
