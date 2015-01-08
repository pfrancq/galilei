/*

	GALILEI Research Project

	GTopic.cpp

	Topic - Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#include <gtopic.h>
#include <gdoc.h>
#include <glang.h>
#include <gdescriptionobject.hh>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GTopic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTopic::GTopic(GSession* session,const RString& name)
	: GGroup<GDoc,GTopic,otTopic>(session,cNoRef,0,name,RDate::GetToday(),RDate::Null)
{
}


//------------------------------------------------------------------------------
GTopic::GTopic(GSession* session,size_t id,size_t blockid,const RString& name,const RDate& u,const RDate& c)
	: GGroup<GDoc,GTopic,otTopic>(session,id,blockid,name,u,c)
{
}


//------------------------------------------------------------------------------
void GTopic::Update(GDescription& desc)
{
	bool Save(Session->MustSaveResults());  // Must the results be saved on disk?
	bool NullDesc;                          // The description must not stayed in memory?

	// Look if the topic is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Emit an event that it is about to be updated
		PostNotification(hTopics[oeAboutToBeUpdated]);

		// Look if the index must be modified
		if(Save&&Session->DoCreateIndex(pTopic))
			Session->UpdateIndex(pTopic,desc,Id,false);
	}

	// The description must be saved only for external topics or when a description is already loaded
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

	// Look if the topic is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Look if the index must be modified and the description and tree saved
		if(Save)
		{
			if(Session->DoCreateIndex(pTopic))
				Session->UpdateIndex(pTopic,desc,Id,true);

			if(desc.IsDefined())
				Session->SaveDesc(pTopic,*desc.Vectors,BlockId,Id);

			Session->GetStorage()->SaveObj(this);
		}

		// Emit an event that it was updated
		PostNotification(hTopics[oeUpdated]);

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
GTopic::~GTopic(void)
{
}
