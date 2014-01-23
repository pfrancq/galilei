/*

	GALILEI Research Project

	GClass.cpp

	Class regrouping concepts - Implementation.

	Copyright 2009-2014 by Pascal Francq (pascal@francq.info).

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
// include files for GALILEI
#include <gclass.h>
#include <gsession.h>
#include <gclasses.h>
#include <gdescriptionobject.hh>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GClass::GClass(GSession* session,const RString& name)
	: R::RNode<GClasses,GClass,false>(), GDescriptionObject<GClass>(session,cNoRef,0,otClass,name),
	  Computed(RDate::Null)
{
}


//------------------------------------------------------------------------------
GClass::GClass(GSession* session,size_t id,size_t blockid,const RString& name,const RDate& c)
	: R::RNode<GClasses,GClass,false>(), GDescriptionObject<GClass>(session,id,blockid,otClass,name),
	  Computed(c)
{
}


//------------------------------------------------------------------------------
void GClass::Clear(bool disk)
{
	RNode<GClasses,GClass,false>::Clear();
	GDescriptionObject<GClass>::Clear(disk);
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass& theclass) const
{
	return(R::CompareIds(Id,theclass.Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass* theclass) const
{
	return(R::CompareIds(Id,theclass->Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const size_t id) const
{
	return(R::CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GClass::Update(GDescription& desc)
{
	bool Save(Session->MustSaveResults());  // Must the results be saved on disk?
	bool NullDesc;                          // The description must not stayed in memory?

	// Look if the class is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Emit an event that it is about to updated
		PostNotification(hClasses[oeAboutToBeUpdated]);

		// Look if the index must be modified
		if(Save&&Session->DoCreateIndex(pClass))
			Session->UpdateIndex(pClass,desc,Id,false);
	}

	// The description must be saved only for external classes or when a description is already loaded
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

	// Look if the class is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Look if the index must be modified and the description and tree saved
		if(Save)
		{
			if(Session->DoCreateIndex(pClass))
				Session->UpdateIndex(pClass,desc,Id,true);

			if(desc.IsDefined())
				Session->SaveDesc(pClass,*desc.Vectors,BlockId,Id);

			Session->GetStorage()->SaveObj(this);
		}

		// Emit an event that it was updated
		PostNotification(hClasses[oeUpdated]);

		// Verify if description must stay in memory
		if(NullDesc)
			Vectors=0;
	}

	// Clear the description
	desc.Clear();
}


//------------------------------------------------------------------------------
double GClass::GetUpOperationCost(void) const
{
	double Cost(0.0);

	if(Parent)
	{
		RConstCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Look if the parent has a vector for that concept
			const GVector* Correspondance(Parent->GetVector(Vector()->GetMetaConcept()));
			if(Correspondance)
			{
				// The cost is the difference between the number of information entities of
				// the current class with its parents (it is of course supposed that all the
				// information entities of the parent are in the current class).
				Cost+=(Vector()->GetNb()-Correspondance->GetNb());
			}
			else
				Cost+=Vector()->GetNb();
		}
	}
	else
	{
		// No parent -> all the concepts references are to be 'added'.
		RConstCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
			Cost+=Vector()->GetNb();
	}

	return(Cost);
}


//------------------------------------------------------------------------------
GClass::~GClass(void)
{
}
