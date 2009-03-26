/*

	GALILEI Research Project

	HTML.cpp

	A HTML filter - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <gfilter.h>
#include <gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFilterHTML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterHTML::GFilterHTML(GFactoryFilter* fac)
	: GFilter(fac), R::RHTMLFile()
{
	AddMIME("text/html");
	Doc=0;
}


//------------------------------------------------------------------------------
void GFilterHTML::Analyze(const RURI&,const RURI& file,const RURI& docxml)
{
	try
	{
		RXMLStruct Doc;
		Open(file,&Doc,RIO::Read,"iso-8859-1");
		RXMLFile Out(docxml,&Doc);
		Out.Open(RIO::Create);
	}
	catch(RIOException& e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::CreateParams(RConfig*)
{
}


//------------------------------------------------------------------------------
GFilterHTML::~GFilterHTML()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("HTML Filter",GFilterHTML)
