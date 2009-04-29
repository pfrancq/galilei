/*

	GALILEI Research Project

	GOntology.h

	Ontology - Implementation.

	Copyright 2008-2009 by Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gontology.h>
#include <ggalileiapp.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <glang.h>
#include <gstorage.h>
#include <gsession.h>
#include <gpredicate.h>
#include <gstatement.h>
#include <gclass.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GOntology
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GOntology::GOntology(size_t nbconcepts)
	: ConceptTypes(20), ConceptTypesByIds(20), Concepts(nbconcepts>50000?nbconcepts:50000,10000),
	  Predicates(30), PredicatesByIds(30), Statements(5000,5000)
{
}


//-----------------------------------------------------------------------------
GObject* GOntology::GetObject(size_t id,tObjType objtype)
{
	switch(objtype)
	{
		case otConcept:
			return(Concepts[id]);

		default:
			throw GException("GOntology::GetObject(size_t,tObjType): '"+GetObjType(objtype)+"' is not a valid type");
	}
}


//-----------------------------------------------------------------------------
void GOntology::ClearRef(tObjType type)
{
	RCursor<GConceptType> Types(ConceptTypes);
	for(Types.Start();!Types.End();Types.Next())
		Types()->ClearRef(type);
}


//-----------------------------------------------------------------------------
RCursor<GConceptType> GOntology::GetConceptTypes(void) const
{
	return(RCursor<GConceptType>(ConceptTypes));
}


//-----------------------------------------------------------------------------
GConceptType* GOntology::GetConceptType(char id,bool null)
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
		throw GException("GSession::GetConceptType(size_t,bool): Unknown concept type "+RString::Number(id));
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GOntology::GetConceptType(const RString& name,bool null)
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
		throw GException("GSession::GetConceptType(const RString&,bool); Unknown concept type '"+name+"'");
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GOntology::GetInsertConceptType(const RString& name,const RString& desc)
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
		ConceptTypes.InsertPtr(type=new GConceptType(0,this,name,desc,0,5000));
		Storage->AssignId(type);
		ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
	}
	return(type);
}


//-----------------------------------------------------------------------------
void GOntology::InsertConceptType(char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics)
{
	RString code(name.Mid(0,2));
	GLang* Lang(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn(code,false));
	GConceptType* type=new GConceptType(id,this,name,desc,Lang,5000);
	if(!id)
		Storage->AssignId(type);
	type->SetReferences(refdocs,refprofiles,refgroups,reftopics);
	ConceptTypes.InsertPtr(type);
	ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
}


//------------------------------------------------------------------------------
GConcept* GOntology::InsertConcept(const GConcept* concept)
{
	GConceptType* Type;
	if((!concept)||(!(Type=concept->GetType())))
		throw GException("GOntology::InsertConcept: Cannot insert concept");

	bool InDirect(false);

	// Invalid concept are not inserted
	if(concept->IsEmpty())
		throw GException("GOntology::InsertConcept: Empty concept cannot be inserted into a dictionary - id="+RString::Number(concept->GetId()));

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(Type->GetPtr(*concept));
	if(!ptr)
	{
		ptr=concept->DeepCopy();
		Type->InsertPtr(ptr);
		InDirect=true;
	}

	// Look if data has an identifier. If not, assignconst one.
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
R::RCursor<GConcept> GOntology::GetConcepts(void) const
{
	return(R::RCursor<GConcept>(Concepts));
}


//------------------------------------------------------------------------------
GConcept* GOntology::GetConcept(size_t id)
{
	GConcept* concept(Concepts[id]);
	if(!concept)
		throw GException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(concept);
}


//------------------------------------------------------------------------------
void GOntology::DeleteConcept(GConcept* concept)
{
	if((!concept)||(!concept->GetType()))
		throw GException("GOntology::DeleteConcept: Cannot delete concept");
	Storage->DeleteConcept(concept);
	concept->GetType()->DeletePtr(*concept);
	Concepts.DeletePtrAt(concept->GetId(),false);
}


//------------------------------------------------------------------------------
GConcept* GOntology::RenameConcept(GConcept* concept,const R::RString& name)
{
	if((!concept)||(!concept->GetType()))
		throw GException("GOntology::RenameConcept: Cannot rename concept");

	// Look if the new name is not  already in the dictionary
	GConcept* ptr=concept->GetType()->GetPtr(name);
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
		concept->GetType()->DeletePtr(*concept);
		concept->Name=name;
		concept->GetType()->InsertPtr(concept);
		Storage->SaveConcept(concept);
		return(concept);
	}
}


//-----------------------------------------------------------------------------
RCursor<GPredicate> GOntology::GetPredicates(void) const
{
	return(RCursor<GPredicate>(Predicates));
}


//-----------------------------------------------------------------------------
GPredicate* GOntology::GetPredicate(size_t id,bool null)
{
	GPredicate* type=PredicatesByIds[id];
	if(!type)
	{
		if(!null)
			throw GException("Unknown relation type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GPredicate* GOntology::GetPredicate(const RString& name,bool null)
{
	GPredicate* type=Predicates.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			throw GException("Unknown relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GPredicate* GOntology::InsertPredicate(size_t id,const R::RString& name,const R::RString& desc)
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
void GOntology::InsertStatement(size_t id,size_t subject,tObjType subjecttype,size_t predicate,size_t object,tObjType objecttype,double weight)
{
	// Get the concept related to the subject
	GObject* Subject(GetObject(subject,subjecttype));
	if(!Subject)
		throw GException("Object "+RString::Number(subject)+" does not exist");

	// Get the concept related to the object
	GObject* Object(GetObject(object,objecttype));
	if(!Object)
		throw GException("Object "+RString::Number(object)+" does not exist");

	// Find the predicate
	GPredicate* Predicate(PredicatesByIds[predicate]);
	if(!Predicate)
		throw GException("Predicate "+RString::Number(predicate)+" does not exist");

	// Insert the statement
	bool InDirect(true);
	GStatement* Statement(new GStatement(id,Subject,Predicate,Object,weight));
	Predicate->InsertStatement(Statement);

	// Look if data has an identifier. If not, assign one.
	if(Statement->GetId()==cNoRef)
	{
		Storage->AssignId(Statement);
		InDirect=true;
	}

	if(InDirect)
		Statements.InsertPtrAt(Statement,Statement->GetId(),true);
}


//-----------------------------------------------------------------------------
GStatement* GOntology::GetStatement(size_t id)
{
	GStatement* Statement(Statements[id]);
	if(!Statement)
		throw GException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(Statement);
}


//------------------------------------------------------------------------------
GOntology::~GOntology(void)
{
}