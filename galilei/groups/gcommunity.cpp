/*

	GALILEI Research Project

	GCommunity.cpp

	Community - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gcommunity.h>
#include <gdoc.h>
#include <gsuggestion.h>
#include <gfdbk.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GFdbkRef
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFdbkRef
{
public:
	GFdbk* Doc;
	double Sim;

	GFdbkRef(GFdbk* d,double s) : Doc(d), Sim(s) {}
	int Compare(const GFdbkRef&) const {return(-1);}
	int Compare(const GFdbk* d) const
	{
		return(CompareIds(Doc->GetDocId(),d->GetDocId()));
	}
};



//------------------------------------------------------------------------------
//
//  GCommunity
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GCommunity::GCommunity(const RString& name)
	: GGroup<GProfile,GCommunity,otCommunity>(cNoRef,0,name,RDate::GetToday(),RDate::Null)
{
}


//------------------------------------------------------------------------------
GCommunity::GCommunity(size_t id,size_t blockid,const RString& name,const RDate& u,const RDate& c)
	: GGroup<GProfile,GCommunity,otCommunity>(id,blockid,name,u,c)
{
}


//------------------------------------------------------------------------------
void GCommunity::GetRelevantDocs(GCommunityDocs& docs)
{
	RContainer<GDocRanking,false,true> RelevantDocs(100);       // Container of relevant documents.
	docs.SetCommunityId(GetId());

	// Go through the profiles
	RCursor<GProfile> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		// Go through the assessments of the profile
		RCursor<GFdbk> Fdbks(Prof()->GetFdbks());
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify if the document is relevant.
			if(Fdbks()->GetFdbk()!=ftRelevant) continue;

			// Insert the document in RelevantDocs
			RelevantDocs.GetInsertPtr(Fdbks()->GetDocId());
		}
	}

	// Copy all the documents in docs
	RCursor<GDocRanking> Cur(RelevantDocs);
	for(Cur.Start();!Cur.End();Cur.Next())
		docs.InsertPtr(Cur());
}


//------------------------------------------------------------------------------
GCommunity::~GCommunity(void)
{
}



//------------------------------------------------------------------------------
//
//  GCommunityDocs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GCommunityDocs::GCommunityDocs(size_t size,size_t communityid)
	: RContainer<GDocRanking,true,false>(size), CommunityId(communityid)
{
}


//------------------------------------------------------------------------------
void GCommunityDocs::SetCommunityId(size_t communityid)
{
	CommunityId=communityid;
	RContainer<GDocRanking,true,false>::Clear();
}
