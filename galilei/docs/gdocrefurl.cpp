/*
	GALILEI Research Project

	GDocRefUrl.cpp

	Document descibe by URL - Implementation.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors
		Vandaele Valery (vavdaele@ulb.ac.be)

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
//include file for galilei
#include <gdocrefurl.h>
#include <docs/gdoc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// GDocRefUrl
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocRefUrl::GDocRefUrl(GDoc* d) : Doc(d)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GDocRefUrl::Compare(const GDocRefUrl* doc) const
{
	RStd::RString tmp =Doc->GetURL();
	return (tmp.Compare(doc->GetURL()));
}


//-----------------------------------------------------------------------------
int GALILEI::GDocRefUrl::Compare(const GDocRefUrl& doc) const
{
	RStd::RString tmp =Doc->GetURL();
	return (tmp.Compare(doc.GetURL()));
}


//-------------------------------------------------------------- --------------
int GALILEI::GDocRefUrl::Compare(const GDoc* doc) const
{
	RStd::RString tmp =Doc->GetURL();
	return (tmp.Compare(doc->GetURL()));
}


//-----------------------------------------------------------------------------
int GALILEI::GDocRefUrl::Compare(const GDoc& doc) const
{
	RStd::RString tmp =Doc->GetURL();
	return (tmp.Compare(doc.GetURL()));
}

//-----------------------------------------------------------------------------
int GALILEI::GDocRefUrl::Compare(const char* url) const
{
	RStd::RString tmp =Doc->GetURL();
	return (tmp.Compare(url));
}


//-----------------------------------------------------------------------------
GALILEI::GDocRefUrl::~GDocRefUrl(void)
{
}

