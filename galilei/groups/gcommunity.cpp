/*

	GALILEI Research Project

	GCommunity.cpp

	Community - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <gfdbk.h>
#include <gdescriptionobject.hh>
using namespace GALILEI;
using namespace R;
using namespace std;



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
GCommunity::GCommunity(GSession* session,const RString& name)
	: GGroup<GProfile,GCommunity,otCommunity>(session,cNoRef,0,name,RDate::GetToday(),RDate::Null)
{
}


//------------------------------------------------------------------------------
GCommunity::GCommunity(GSession* session,size_t id,size_t blockid,const RString& name,const RDate& u,const RDate& c)
	: GGroup<GProfile,GCommunity,otCommunity>(session,id,blockid,name,u,c)
{
}


//------------------------------------------------------------------------------
void GCommunity::GetRelevantDocs(GCommunityDocs& docs)
{
	//RContainer<GDocFragmentRank,false,true> RelevantDocs(100);       // Container of relevant documents.
	RContainer<GDocFragment,false,true> RelevantDocs(100);       // Container of relevant documents.
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
			GDoc* Doc(Session->GetObj(pDoc,Fdbks()->GetDocId()));
			if(Doc)
			{
				bool Find;
				size_t idx(RelevantDocs.GetIndex(GDocFragment::Search(Doc->GetId(),0,true),Find));
				if(!Find)
					RelevantDocs.InsertPtrAt(new GDocFragment(Doc,0,0),idx,false);
			}
		}
	}

	// Copy all the documents in docs
	RCursor<GDocFragment> Cur(RelevantDocs);
	for(Cur.Start();!Cur.End();Cur.Next())
		docs.InsertPtr(Cur());
}


//------------------------------------------------------------------------------
void GCommunity::Update(GDescription& desc)
{
	bool Save(Session->MustSaveResults());  // Must the results be saved on disk?
	bool NullDesc;                          // The description must not stayed in memory?

	// Look if the community is internal one : Modify the references and indexes
	GetVectors();  // Make sure the vectors are loaded
	if(Id!=cNoRef)
	{
		// Emit an event that it is about to be updated
		PostNotification(hCommunities[oeAboutToBeUpdated]);

		// Look if the index must be modified
		if(Save&&Session->DoCreateIndex(pCommunity))
			Session->UpdateIndex(pCommunity,*this,Id,false);
	}

	// The description must be saved only for external communities or when a description is already loaded
	if((Id==cNoRef)||Vectors)
	{
		GDescription::operator=(desc);
		NullDesc=false;
	}
	else
	{
		Vectors=desc.Vectors;
		NullDesc=true;
	}

	// Set the computed date and the status
	Computed.SetToday();
	State=osLatest;

	// Look if the community is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Look if the index must be modified and the description and tree saved
		if(Save)
		{
			if(Session->DoCreateIndex(pCommunity))
				Session->UpdateIndex(pCommunity,desc,Id,true);

			if(desc.IsDefined())
				Session->SaveDesc(pCommunity,*desc.Vectors,BlockId,Id);

			Session->GetStorage()->SaveObj(this);
		}

		// Emit an event that it was updated
		PostNotification(hCommunities[oeUpdated]);

		// Verify if description must stay in memory
		if(NullDesc)
			Vectors=0;
	}

	// Clear the description
	desc.Clear();

	// Data must be set to dirty
	if(Data)
		Data->Dirty();
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
	: RContainer<GDocFragment,true,false>(size), CommunityId(communityid)
{
}


//------------------------------------------------------------------------------
void GCommunityDocs::SetCommunityId(size_t communityid)
{
	CommunityId=communityid;
	RContainer<GDocFragment,true,false>::Clear();
}
