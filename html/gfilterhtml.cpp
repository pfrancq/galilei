/*

	GALILEI Research Project

	GFilterHTML.cpp

	A HTML filter - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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


/*
//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <string.h>
#include <iostream> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
*/

//------------------------------------------------------------------------------
// include files for R
#include <ghtmlconverter.h>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <gfilterhtml.h>
#include <docs/gfiltermanager.h>
#include <docs/gdocxml.h>
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
bool GFilterHTML::Analyze(GDocXML* doc) throw(bad_alloc,GException)
{
	try
	{
		Doc=doc;
		Doc->AddIdentifier(Doc->GetFile());

		//Convert html file to docxml structure
		GHTMLConverter Src(this,Doc->GetFile(),Doc);
		Src.Process();

		//Delete Empty Tags
		Doc->GetContent()->DeleteEmptyTags(Doc);
	}
	catch(RIOException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	return true;
}


//------------------------------------------------------------------------------
void GFilterHTML::CreateParams(GParams*)
{
}


//------------------------------------------------------------------------------
GFilterHTML::~GFilterHTML()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("HTML Filter",GFilterHTML)
