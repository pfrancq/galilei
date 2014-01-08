/*

	GALILEI Research Project

	GDescriptionObject.hh

	Object represented by a list of weighted information entities - Implementation.

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
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GDescriptionObject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C>
	GDescriptionObject<C>::GDescriptionObject(GSession* session,size_t id,size_t blockid,tObjType objtype,const R::RString& name)
	: GObject(session,id,name,objtype), GDescription(), State(osLatest), BlockId(blockid)
{
}


//------------------------------------------------------------------------------
template<class C>
	bool GDescriptionObject<C>::LoadVectors(void)
{
	if(BlockId)
	{
		tObjState old(State);
		State=osOnDemand;                        // The object is on-demand of loading
		Session->LoadDesc(static_cast<C*>(this),static_cast<C*>(this)->Vectors,BlockId,Id);  // Load the object
		State=old;                               // Put the old state back
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
template<class C>
	void GDescriptionObject<C>::AddRefs(GSession* session,tObjType ObjType) const
{
	// Get the vector (if necessary)
	R::RConstCursor<GVector> Vector(GetVectors());
	if(!Vectors->GetNb()) return;

	// Init
	R::RVectorBool Types(session->GetNbConceptTypes()+1);
	Types.Init(session->GetNbConceptTypes()+1,true);
	R::RVectorBool Concepts(session->GetNbConcepts()+1);
	Concepts.Init(session->GetNbConcepts()+1,true);

	// Parse the vectors
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		R::RConstCursor<GConceptRef> ptr(Vector()->GetRefs());
		if(!ptr.GetNb())
			continue;

		GConceptType* type(Vector()->GetMetaConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> A new object uses this concept type.
			type->IncRef(ObjType);
			Types[TypeId]=false;
		}

		// IncRef for the concept
		size_t ConceptId(Vector()->GetMetaConcept()->GetId());
		if(Concepts[ConceptId])
		{
			// Yes -> A new object uses this meta-concept.
			type->IncRef(Vector()->GetMetaConcept(),ObjType);
			Concepts[ConceptId]=false;
		}

		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(Types[TypeId])
			{
				// Yes -> A new object uses this concept type.
				type->IncRef(ObjType);
				Types[TypeId]=false;
			}

			// IncRef for the concept
			size_t ConceptId(ptr()->GetConcept()->GetId());
			if(Concepts[ConceptId])
			{
				// Yes -> A new object uses this concept.
				type->IncRef(ptr()->GetConcept(),ObjType);
				Concepts[ConceptId]=false;
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GDescriptionObject<C>::DelRefs(GSession* session,tObjType ObjType) const
{
	// Get the vector (if necessary)
	R::RConstCursor<GVector> Vector(GetVectors());
	if(!Vectors->GetNb()) return;

	// Init
	R::RVectorBool Types(session->GetNbConceptTypes()+1);
	Types.Init(session->GetNbConceptTypes()+1,true);
	R::RVectorBool Concepts(session->GetNbConcepts()+1);
	Concepts.Init(session->GetNbConcepts()+1,true);

	// Parse the vectors
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		R::RConstCursor<GConceptRef> ptr(Vector()->GetRefs());
		if(!ptr.GetNb())
			continue;

		// Reference of the concept associated with the vector
		GConceptType* type(Vector()->GetMetaConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> An old object uses this concept type.
			type->DecRef(ObjType);
			Types[TypeId]=false;
		}

		// DecRef for the concept
		size_t ConceptId(Vector()->GetMetaConcept()->GetId());
		if(Concepts[ConceptId])
		{
			// Yes -> An old object uses this meta-concept.
			type->DecRef(Vector()->GetMetaConcept(),ObjType);
			Concepts[ConceptId]=false;
		}

		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(Types[TypeId])
			{
				// Yes -> An old object uses this concept type.
				type->DecRef(ObjType);
				Types[TypeId]=false;
			}

			// DecRef for the concept
			size_t ConceptId(ptr()->GetConcept()->GetId());
			if(Concepts[ConceptId])
			{
				// Yes -> An old object uses this concept.
				type->DecRef(ptr()->GetConcept(),ObjType);
				Concepts[ConceptId]=false;
			}
		}
	}
}


//------------------------------------------------------------------------------
/*template<class C>
	void GDescriptionObject<C>::Copy(GConcept* concept,const R::RContainer<GConceptRef,false,true>& list)
{
	State=osUpToDate;
	GDescription::Copy(concept,list);
}
*/


//------------------------------------------------------------------------------
template<class C>
	void GDescriptionObject<C>::Clear(bool disk)
{
	GDescription::Clear();
	if(disk)
		BlockId=0;
}


//------------------------------------------------------------------------------
template<class C>
	GDescriptionObject<C>::~GDescriptionObject(void)
{
}
