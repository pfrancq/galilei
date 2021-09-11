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
// include files for FastCGI
#include <fcgiapp.h>


//------------------------------------------------------------------------------
// include files for current project
#include <html.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class HTML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HTML::HTML(FCGX_Request& request)
	: Request(request)
{
	 FCGX_PutS(
		"Content-type: text/html\r\n"
		"\r\n"
		"<?xml version='1.0' encoding='UTF-8'?>\r\n"
		"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>\r\n"
		"<head>\r\n"
		"\t<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>\r\n",
		 Request.out);
}


//------------------------------------------------------------------------------
void HTML::WriteTitle(const R::RString& title)
{
	 FCGX_PutS(
		"\t<title>"+title+"</title>\r\n"
		"</head>\r\n"
		"<body>\r\n",
		 Request.out);
}


//------------------------------------------------------------------------------
void HTML::WriteForm(const R::RString& session,const R::RString& query)
{
	FCGX_PutS(
		"\t<div style='text-align: center;'>\r\n"
		"\t\t<form action='upgalilei.exe' method='get'>\r\n"
		"\t\t\t<table><tr>\r\n"
		"\t\t\t\t<td>\r\n"
		"\t\t\t\t\t<img src='../img/elisit.png' style='width: 40px; height: 40px;'/>\r\n"
		"\t\t\t\t</td>\r\n"
		"\t\t\t\t<td>\r\n"
		"\t\t\t\t\t<input type='hidden' name='session' value='"+session+"'/>\r\n"
		"\t\t\t\t\t<input type='hidden' name='cmd' value='search'/>\r\n"
		"\t\t\t\t\t<input type='search' name='query' autofocus size='80' value='"+query+"'/>\r\n"
		"\t\t\t\t\t<input type='submit' value='Rechercher'>\r\n"
		"\t\t\t\t</td>\r\n"
		"\t\t\t</tr></table>\r\n"
		"\t\t</form>\r\n"
		"\t</div>\r\n"
		"\t<br/>\r\n",
		Request.out);
}


//------------------------------------------------------------------------------
void HTML::WriteH1(const R::RString& what)
{
	FCGX_PutS("\t<h1>"+what+"</h1>\r\n",Request.out);
}


//------------------------------------------------------------------------------
void HTML::WriteP(int level,const R::RString& what,const R::RString& style)
{
	RString Style;
	if(!style.IsEmpty())
		Style=" style=\""+style+"\"";
	Write(level,"<p"+Style+">"+what+"</p>");
}


//------------------------------------------------------------------------------
void HTML::WriteDiv(int level,const R::RString& what,const R::RString& style)
{
	RString Style;
	if(!style.IsEmpty())
		Style=" style=\""+style+"\"";
	Write(level,"<div"+Style+">"+what+"</div>");
}


//------------------------------------------------------------------------------
void HTML::Write(int level,const R::RString& what)
{
	for(int i=0;i<=level;i++)
		FCGX_PutS("\t",Request.out);
	FCGX_PutS(what+"\r\n",Request.out);
}


//------------------------------------------------------------------------------
HTML::~HTML(void)
{
	FCGX_PutS("</body>\r\n</html>",Request.out);
	FCGX_Finish_r(&Request);
}
