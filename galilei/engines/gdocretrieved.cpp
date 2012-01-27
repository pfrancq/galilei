/*

	GALILEI Research Project

	GDocRetrieved.cpp

	Relevant Document for a Search - Implementation.

   Copyright 2003-2012 by Pascal Francq.
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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocretrieved.h>
#include <gdoc.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDocRetrieved
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocRetrieved::GDocRetrieved(size_t docid,const R::RString uri,const R::RString title,const RString description,double ranking,const RString engine)
	: DocId(docid), URI(uri), Title(title), Description(description), Ranking(0), Rankings(10,5)
{
	Rankings.InsertPtr(new GDocRanking(DocId,ranking,engine));
}


//------------------------------------------------------------------------------
int GDocRetrieved::Compare(const GDocRetrieved& d) const
{
	return(URI.Compare(d.URI));
}


//------------------------------------------------------------------------------
int GDocRetrieved::Compare(const RString& uri) const
{
	return(URI.Compare(uri));
}


//------------------------------------------------------------------------------
void GDocRetrieved::AddRanking(double ranking,const R::RString engine)
{
	Rankings.InsertPtr(new GDocRanking(DocId,ranking,engine));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRanking> GDocRetrieved::GetRankings(void) const
{
	return(R::RCursor<GDocRanking>(Rankings));
}


//------------------------------------------------------------------------------
GDocRetrieved::~GDocRetrieved(void)
{
}
