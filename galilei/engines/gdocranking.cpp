/*

	GALILEI Research Project

	GSuggestion.cpp

	Suggestion - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocranking.h>
#include <gsession.h>
#include <gdoc.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocRanking
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocRanking::GDocRanking(size_t docid,double ranking,const R::RString info)
	: DocId(docid), Ranking(ranking), Info(info)
{
}


//------------------------------------------------------------------------------
int GDocRanking::Compare(const GDocRanking& ranking) const
{
	return(CompareIds(DocId,ranking.DocId));
}


//------------------------------------------------------------------------------
int GDocRanking::Compare(size_t docid) const
{
	return(CompareIds(DocId,docid));
}


//------------------------------------------------------------------------------
RURI GDocRanking::GetURI(GSession* session) const
{
	GDoc* Doc(session->GetObj(pDoc,DocId));
	return(Doc->GetURI());
}


//------------------------------------------------------------------------------
RString GDocRanking::GetName(GSession* session) const
{
	GDoc* Doc(session->GetObj(pDoc,DocId));
	return(Doc->GetName());
}


//------------------------------------------------------------------------------
int GDocRanking::SortOrderRanking(const void* a,const void* b)
{
	double af=(*((GDocRanking**)(a)))->Ranking;
	double bf=(*((GDocRanking**)(b)))->Ranking;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}
