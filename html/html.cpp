/*

	GALILEI Research Project

	HTML.cpp

	A HTML filter - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2003 by Valery Vandaele.
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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <memory>


//------------------------------------------------------------------------------
// include files for R
#include <rxmlfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <html.h>
#include <gslot.h>



//------------------------------------------------------------------------------
//
// class GFilterHTML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterHTML::GFilterHTML(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac)
{
	AddMIME("text/html");
}


//------------------------------------------------------------------------------
void GFilterHTML::Analyze(GDoc*,const RURI& uri,RXMLParser* parser,GSlot* rec)
{
	try
	{
		parser->SetHTMLMode(true);
		parser->Open(uri,RIO::Read,"iso-8859-1");
		parser->Close();
	}
	catch(RIOException& e)
	{
		if(rec)
			rec->Warning(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::CreateParams(GPlugInFactory*)
{
}


//------------------------------------------------------------------------------
GFilterHTML::~GFilterHTML()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("HTML Filter","HTML Filter",GFilterHTML)
