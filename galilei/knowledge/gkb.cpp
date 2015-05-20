/*

	GALILEI Research Project

	GKB.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
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
// include files for GALILEI
#include <gkb.h>
#include <gstorage.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gpredicate.h>
#include <gstatement.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// GKB
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GKB::GKB(GSession* session)
	: Session(session), State(osLatest), Storage(0), SaveResults(true),
	  ConceptTypes(20), ConceptTypesByIds(20),
	  Concepts(50000,10000),
	  Predicates(30), PredicatesByIds(30), Statements(5000,5000)
{
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetObj(const GConceptType*,char id,bool null)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypesByIds[id];
	}
	catch(...)
	{
	}
	if((!type)&&(!null))
		mThrowGException("Unknown concept type "+RString::Number(id));
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetObj(const GConceptType*,tConceptCat cat,const RString& name,const RString& desc)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	GConceptType* type(0);
	try
	{
		type=ConceptTypes.GetPtr(name);
	}
	catch(...)
	{
	}
	if(!type)
	{
		ConceptTypes.InsertPtr(type=new GConceptType(Session,cat,cNoRef,name,desc,5000));
		Storage->AssignId(type);
		ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetObj(const GConceptType*,const RString& name,bool null)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypes.GetPtr(name);
	}
	catch(...)
	{
	}
	if((!type)&&(!null))
		mThrowGException("Unknown concept type '"+name+"'");
	return(type);
}


//-----------------------------------------------------------------------------
void GKB::InsertObj(const GConceptType*,tConceptCat cat,char id,const R::RString& name,const R::RString& desc)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	GConceptType* type=new GConceptType(Session,cat,id,name,desc,5000);
	if(!id)
		Storage->AssignId(type);

	ConceptTypes.InsertPtr(type);
	ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetObj(const GConcept*,size_t id)
{
	if(id>Concepts.GetMaxPos())
		mThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	GConcept* concept(Concepts[id]);
	if(!concept)
		mThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(concept);
}


//------------------------------------------------------------------------------
void GKB::DeleteObj(GConcept* concept)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	if((!concept)||(!concept->GetType()))
		mThrowGException("Cannot delete concept");
	Storage->DeleteConcept(concept);
	concept->GetType()->DeleteObj(concept);
	Concepts.DeletePtrAt(concept->GetId(),false);
}


//------------------------------------------------------------------------------
void GKB::ClearIndex(tObjType type)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	RCursor<GConcept> Concept(Concepts);
	for(Concept.Start();!Concept.End();Concept.Next())
		Concept()->ClearIndex(type);
	if(SaveResults)
		Storage->ClearIndex(type);
}


//-----------------------------------------------------------------------------
GPredicate* GKB::GetObj(const GPredicate*,size_t id,bool null)
{
	GPredicate* type=PredicatesByIds[id];
	if(!type)
	{
		if(!null)
			mThrowGException("Unknown relation type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GPredicate* GKB::GetObj(const GPredicate*,const RString& name,bool null)
{
	GPredicate* type=Predicates.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			mThrowGException("Unknown relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
RCursor<GPredicate> GKB::GetObjs(const GPredicate*) const
{
	return(RCursor<GPredicate>(Predicates));
}



//-----------------------------------------------------------------------------
GStatement* GKB::GetObj(const GStatement*,size_t id)
{
	GStatement* Statement(Statements[id]);
	if(!Statement)
		mThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(Statement);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetObj(const GConcept*,GConceptType* type,const R::RString& concept,bool null)
{
	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(type->GetObj(pConcept,concept,true));
	if(!ptr)
	{
		if(null)
			return(0);
		else
			mThrowGException("Unknown concept '"+concept+"' for the type '"+type->GetName()+"'");
	}
	return(ptr);
}


//------------------------------------------------------------------------------
GConcept* GKB::InsertObj(const GConcept*,GConceptType* type,const R::RString& concept)
{
  	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	if((concept.IsEmpty())||(!type))
		mThrowGException("Cannot insert the concept");

	bool InDirect(false);

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(type->GetObj(pConcept,concept,true));
	if(!ptr)
	{
		ptr=new GConcept(Session,concept,type);
		type->InsertObj(ptr);
		InDirect=true;
	}

	// Look if data has an identifier. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Storage->AssignId(ptr);
		InDirect=true;
	}

	// Finally, if an identifier has been assigned and/or a new one -> Direct
	if(InDirect)
		Concepts.InsertPtrAt(ptr,ptr->GetId(),true);

	return(ptr);
}


//------------------------------------------------------------------------------
GConcept* GKB::InsertObj(const GConcept& concept)
{
  	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	GConceptType* Type;
	if(!(Type=concept.GetType()))
		mThrowGException("Cannot insert the concept");

	bool InDirect(false);

	// Invalid concept are not inserted
	if(concept.IsEmpty())
		mThrowGException("Empty concept cannot be inserted into a dictionary - id="+RString::Number(concept.GetId()));

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(Type->GetObj(pConcept,concept.GetName(),true));
	if(!ptr)
	{
		ptr=concept.DeepCopy();
		Type->InsertObj(ptr);
		InDirect=true;
	}

	// Look if data has an identifier. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Storage->AssignId(ptr);
		InDirect=true;
	}

	// Finally, if an identifier has been assigned and/or a new one -> Direct
	if(InDirect)
		Concepts.InsertPtrAt(ptr,ptr->GetId(),true);

	return(ptr);
}


//-----------------------------------------------------------------------------
GPredicate* GKB::InsertObj(const GPredicate*,size_t id,const R::RString& name,const R::RString& desc)
{
  	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	bool InDirect(false);

	GPredicate* ptr(Predicates.GetPtr(name));
	if(!ptr)
	{
		ptr=new GPredicate(id,name,desc);
		InDirect=true;
		Predicates.InsertPtr(ptr);
	}

	if(ptr->GetId()==cNoRef)
	{
		Storage->AssignId(ptr);
		InDirect=true;
	}

	if(InDirect)
		PredicatesByIds.InsertPtrAt(ptr,ptr->GetId(),true);

	return(ptr);
}


//-----------------------------------------------------------------------------
void GKB::InsertObj(const GStatement*,size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	// Get the concept related to Xi
//	GObject* Xi(GetObj(xitype,xi));
	GObject* Xi(0);
	if(!Xi)
		mThrowGException("Object "+RString::Number(xi)+" is not a "+GetObjType(xitype,false,false));

	// Get the concept related to the object
	//GObject* Xj(GetObj(xjtype,xj));
	GObject* Xj(0);
	if(!Xj)
		mThrowGException("Object "+RString::Number(xj)+" is not a "+GetObjType(xjtype,false,false));

	// Find the predicate
	GPredicate* Predicate(PredicatesByIds[predicate]);
	if(!Predicate)
		mThrowGException("Predicate "+RString::Number(predicate)+" does not exist");

	// Insert the statement
	bool InDirect(true);
	GStatement* Statement(new GStatement(id,Predicate,Xi,Xj,weight));
//	Predicate->InsertStatement(Statement);

	// Look if data has an identifier. If not, assign one.
	if(Statement->GetId()==cNoRef)
	{
		Storage->AssignId(Statement);
		InDirect=true;
	}

	if(InDirect)
		Statements.InsertPtrAt(Statement,Statement->GetId(),true);
}


//------------------------------------------------------------------------------
GConcept* GKB::RenameObj(GConcept* concept,const R::RString& name)
{
  	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	if((!concept)||(!concept->GetType()))
		mThrowGException("Cannot rename concept");

	// Look if the new name is not  already in the dictionary
	GConcept* ptr=concept->GetType()->GetObj(pConcept,name,true);
	if(ptr==concept)
		return(concept);
	if(ptr)
	{
		// Both concept must be merge and the old one deleted
		DeleteObj(concept);
		return(ptr);
	}
	else
	{
		// Rename really the concept
		concept->GetType()->DeleteObj(concept);
		concept->Name=name;
		concept->GetType()->InsertObj(concept);
		Storage->SaveConcept(concept);
		return(concept);
	}
}


//------------------------------------------------------------------------------
GKB::~GKB(void)
{
}
