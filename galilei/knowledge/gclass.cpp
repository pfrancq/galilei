/*

	GALILEI Research Project

	GClass.cpp

	Class regrouping concepts - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
void GClass::PrivateInit(void)
{
	SetState(osNew);
	SaveDesc();
	AddRefs(Session,otClass);
	SetId(cNoRef);
}


//------------------------------------------------------------------------------
GClass::GClass(GSession* session,const R::RString& name)
	: R::RNode<GClasses,GClass,false>(), GDescriptionObject<GClass>(session,cNoRef,0,otClass,name,osNew)
{
}


//------------------------------------------------------------------------------
GClass::GClass(GSession* session,size_t id,size_t blockid,const R::RString& name)
	: R::RNode<GClasses,GClass,false>(), GDescriptionObject<GClass>(session,id,blockid,otClass,name,osNew)
{
}


//------------------------------------------------------------------------------
void GClass::Clear(void)
{
	RNode<GClasses,GClass,false>::Clear();
	GDescriptionObject<GClass>::Clear();
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass& grp) const
{
	return(R::CompareIds(Id,grp.Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass* grp) const
{
	if(!grp)
		return(1);
	return(R::CompareIds(Id,grp->Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const size_t id) const
{
	return(R::CompareIds(Id,id));
}


//------------------------------------------------------------------------------
void GClass::Update(GDescription& desc)
{
	// Remove its references
	DelRefs(Session,otClass);
	if(Session->HasIndex(pClass))
		Session->UpdateIndex(pClass,desc,Id,false);

	// Assign information
	State=osUpdated;
	GDescription::operator=(desc);
	desc.Clear();

	// Update its references
	AddRefs(Session,otClass);
	if(Session->HasIndex(pClass))
		Session->UpdateIndex(pClass,desc,Id,false);

	// Emit an event that it was modified
	Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
double GClass::GetUpOperationCost(void) const
{
	double Cost(0.0);

	if(Parent)
	{
		RCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Look if the parent has a vector for that concept
			GVector* Correspondance(Parent->GetVector(Vector()->GetMetaConcept()));
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
		RCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
			Cost+=Vector()->GetNb();
	}

	return(Cost);
}


//------------------------------------------------------------------------------
GClass::~GClass(void)
{
}
