/*

	GALILEI Research Project

	GEngineDoc.cpp

	Class to store the results of the extraction from a search engine - Implementation.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#include <genginedoc.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GEngineDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngineDoc::GEngineDoc(RString url,RString title,RString description,int rank, RString engine)
	:Url(url),Title(title),Description(description),GlobalRank(0),GlobalRank2(0),Rankings(10,5)
{
	Rankings.InsertPtr(new GRanking(rank,engine));
}


//------------------------------------------------------------------------------
void GEngineDoc::AddRanking(GRanking* r)
{
	Rankings.InsertPtr(r);
}


//------------------------------------------------------------------------------
void GEngineDoc::AddRanking(int rank,R::RString engine)
{
	Rankings.InsertPtr(new GRanking(rank,engine));
}

//------------------------------------------------------------------------------
int GEngineDoc::Compare(const GEngineDoc* d) const
{
	return(Url.Compare(d->GetUrl()));
}


//------------------------------------------------------------------------------
int GEngineDoc::Compare(const R::RString url) const
{
	return(Url.Compare(url));
}


//------------------------------------------------------------------------------
int GEngineDoc::Compare(const GEngineDoc& d) const
{
	return(Url.Compare(d.GetUrl()));
}


//-----------------------------------------------------------------------------
R::RCursor<GRanking> GEngineDoc::GetRankingsCursor(void) throw(GException)
{
	R::RCursor<GRanking> cur(Rankings);
	return(cur);
}


//------------------------------------------------------------------------------
GEngineDoc::~GEngineDoc(void)
{
}
