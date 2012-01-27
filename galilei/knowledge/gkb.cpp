/*

	GALILEI Research Project

	GKB.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
GKB::GKB(void)
	: Storage(0), SaveResults(true),
	  ConceptCats(10), ConceptCatsByIds(10),
	  ConceptTypes(20), ConceptTypesByIds(20),
	  Concepts(50000,10000),
	  Predicates(30), PredicatesByIds(30), Statements(5000,5000)

{
}


//-----------------------------------------------------------------------------
GConceptCat* GKB::GetConceptCat(char id,bool null)
{
	GConceptCat* cat(0);
	try
	{
		cat=ConceptCatsByIds[id];
	}
	catch(...)
	{
	}
	if((!cat)&&(!null))
		ThrowGException("Unknown concept category "+RString::Number(id));
	return(cat);
}


//-----------------------------------------------------------------------------
GConceptCat* GKB::GetInsertConceptCat(const RString& name)
{
	GConceptCat* cat(0);
	try
	{
		cat=ConceptCats.GetPtr(name);
	}
	catch(...)
	{
	}
	if(!cat)
	{
		ConceptCats.InsertPtr(cat=new GConceptCat(dynamic_cast<GSession*>(this),cNoRef,name));
		Storage->AssignId(cat);
		ConceptCatsByIds.InsertPtrAt(cat,cat->GetId(),true);
	}
	return(cat);
}


//-----------------------------------------------------------------------------
GConceptCat* GKB::GetConceptCat(const RString& name,bool null)
{
	GConceptCat* cat(0);
	try
	{
		cat=ConceptCats.GetPtr(name);
	}
	catch(...)
	{
	}
	if((!cat)&&(!null))
		ThrowGException("Unknown concept category '"+name+"'");
	return(cat);
}


//-----------------------------------------------------------------------------
void GKB::InsertConceptCat(char id,const R::RString& name)
{
	GConceptCat* cat=new GConceptCat(dynamic_cast<GSession*>(this),id,name);
	if(!id)
		Storage->AssignId(cat);

	ConceptCats.InsertPtr(cat);
	ConceptCatsByIds.InsertPtrAt(cat,cat->GetId(),true);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetConceptType(char id,bool null)
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
		ThrowGException("Unknown concept type "+RString::Number(id));
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetInsertConceptType(GConceptCat* cat,const RString& name,const RString& desc)
{
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
		ConceptTypes.InsertPtr(type=new GConceptType(dynamic_cast<GSession*>(this),cat,cNoRef,name,desc,5000));
		Storage->AssignId(type);
		ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetConceptType(const RString& name,bool null)
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
		ThrowGException("Unknown concept type '"+name+"'");
	return(type);
}


//-----------------------------------------------------------------------------
void GKB::InsertConceptType(GConceptCat* cat,char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics,size_t refclasses)
{
	GConceptType* type=new GConceptType(dynamic_cast<GSession*>(this),cat,id,name,desc,5000);
	if(!id)
		Storage->AssignId(type);

	type->SetReferences(refdocs,refprofiles,refgroups,reftopics,refclasses);
	ConceptTypes.InsertPtr(type);
	ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetConcept(size_t id)
{
	if(id>Concepts.GetMaxPos())
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	GConcept* concept(Concepts[id]);
	if(!concept)
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(concept);
}


//------------------------------------------------------------------------------
R::RCursor<GConcept> GKB::GetConcepts(void) const
{
	return(RCursor<GConcept>(Concepts));
}


//------------------------------------------------------------------------------
void GKB::DeleteConcept(GConcept* concept)
{
	if((!concept)||(!concept->GetType()))
		ThrowGException("Cannot delete concept");
	Storage->DeleteConcept(concept);
	concept->GetType()->DeleteConcept(concept);
	Concepts.DeletePtrAt(concept->GetId(),false);
}


//------------------------------------------------------------------------------
void GKB::ClearIndex(GSession* session,tObjType type)
{
	RCursor<GConcept> Concept(Concepts);
	for(Concept.Start();!Concept.End();Concept.Next())
		Concept()->ClearIndex(type);
	if(session->SaveResults)
		session->GetStorage()->ClearIndex(type);
}


//------------------------------------------------------------------------------
void GKB::ClearRefs(GSession* session,tObjType type)
{
	RCursor<GConcept> Concept(Concepts);
	for(Concept.Start();!Concept.End();Concept.Next())
		Concept()->ClearRefs(type);

	RCursor<GConceptType> Types(ConceptTypes);
	for(Types.Start();!Types.End();Types.Next())
		Types()->ClearRefs(type);

	// If necessary, put the references to 0.
	if(session->MustSaveResults())
		session->Storage->ClearRefs(type);
}


//-----------------------------------------------------------------------------
GPredicate* GKB::GetPredicate(size_t id,bool null)
{
	GPredicate* type=PredicatesByIds[id];
	if(!type)
	{
		if(!null)
			ThrowGException("Unknown relation type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GPredicate* GKB::GetPredicate(const RString& name,bool null)
{
	GPredicate* type=Predicates.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			ThrowGException("Unknown relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
RCursor<GPredicate> GKB::GetPredicates(void) const
{
	return(RCursor<GPredicate>(Predicates));
}



//-----------------------------------------------------------------------------
GStatement* GKB::GetStatement(size_t id)
{
	GStatement* Statement(Statements[id]);
	if(!Statement)
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(Statement);
}


//------------------------------------------------------------------------------
GConcept* GKB::InsertConcept(GConceptType* type,const R::RString& concept)
{
	if((concept.IsEmpty())||(!type))
		ThrowGException("Cannot insert the concept");

	bool InDirect(false);

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(type->GetConcept(concept));
	if(!ptr)
	{
		ptr=new GConcept(reinterpret_cast<GSession*>(this),concept,type);
		type->InsertConcept(ptr);
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
GConcept* GKB::InsertConcept(const GConcept* concept)
{
	GConceptType* Type;
	if((!concept)||(!(Type=concept->GetType())))
		ThrowGException("Cannot insert the concept");

	bool InDirect(false);

	// Invalid concept are not inserted
	if(concept->IsEmpty())
		ThrowGException("Empty concept cannot be inserted into a dictionary - id="+RString::Number(concept->GetId()));

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(Type->GetConcept(concept->GetName()));
	if(!ptr)
	{
		ptr=concept->DeepCopy();
		Type->InsertConcept(ptr);
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
GPredicate* GKB::InsertPredicate(size_t id,const R::RString& name,const R::RString& desc)
{
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
void GKB::InsertStatement(size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight)
{
	// Get the concept related to Xi
//	GObject* Xi(GetObj(xitype,xi));
	GObject* Xi(0);
	if(!Xi)
		ThrowGException("Object "+RString::Number(xi)+" is not a "+GetObjType(xitype,false,false));

	// Get the concept related to the object
	//GObject* Xj(GetObj(xjtype,xj));
	GObject* Xj(0);
	if(!Xj)
		ThrowGException("Object "+RString::Number(xj)+" is not a "+GetObjType(xjtype,false,false));

	// Find the predicate
	GPredicate* Predicate(PredicatesByIds[predicate]);
	if(!Predicate)
		ThrowGException("Predicate "+RString::Number(predicate)+" does not exist");

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
GConcept* GKB::RenameConcept(GConcept* concept,const R::RString& name)
{
	if((!concept)||(!concept->GetType()))
		ThrowGException("Cannot rename concept");

	// Look if the new name is not  already in the dictionary
	GConcept* ptr=concept->GetType()->GetConcept(name);
	if(ptr==concept)
		return(concept);
	if(ptr)
	{
		// Both concept must be merge and the old one deleted
		ptr->NbRefDocs+=concept->NbRefDocs;
		ptr->NbRefProfiles+=concept->NbRefProfiles;
		ptr->NbRefCommunities+=concept->NbRefCommunities;
		ptr->NbRefTopics+=concept->NbRefTopics;
		DeleteConcept(concept);
		return(ptr);
	}
	else
	{
		// Rename really the concept
		concept->GetType()->DeleteConcept(concept);
		concept->Name=name;
		concept->GetType()->InsertConcept(concept);
		Storage->SaveConcept(concept);
		return(concept);
	}
}


//------------------------------------------------------------------------------
GKB::~GKB(void)
{
}
