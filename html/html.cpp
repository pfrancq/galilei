/*

	GALILEI Research Project

	GFilterHTML.cpp

	A HTML filter - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
// include files for R
#include <ghtmlconverter.h>
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
	: GFilter(fac)
{
	AddMIME("text/html");
}


//------------------------------------------------------------------------------
void GFilterHTML::Analyze(const RURI&,const RString& file,const RString& docxml)
{
	Doc=new GDocXML(docxml,file);

	//Convert html file to docxml structure
	GHTMLConverter Src(this,file,Doc);
	Src.Open(RIO::Read);

	Doc->AddIdentifier(file);

	// Save the structure and delete everything
	RXMLFile Out(docxml,Doc);
	Out.Open(RIO::Create);
	delete Doc;
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
