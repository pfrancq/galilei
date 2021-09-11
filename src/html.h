/*

	GALILEI Research Project

	HTML.h

	HTML Result - Header.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef HTMLH
#define HTMLH


//------------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rstring.h>


//------------------------------------------------------------------------------
// forward declaration
struct FCGX_Request;


//------------------------------------------------------------------------------
class HTML
{
	FCGX_Request& Request;

public:
	HTML(FCGX_Request& request);
	FCGX_Request& GetRequest(void) const {return(Request);}
	void WriteTitle(const R::RString& title);
	void WriteForm(const R::RString& session,const R::RString& query);
	void WriteH1(const R::RString& what);
	void WriteP(int level,const R::RString& what,const R::RString& style=R::RString::Null);
	void WriteDiv(int level,const R::RString& what,const R::RString& style=R::RString::Null);
	void Write(int level,const R::RString& what);
	~HTML(void);
};


//------------------------------------------------------------------------------
#endif

