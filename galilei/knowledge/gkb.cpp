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
	  TokenType(0), DCMIType(0), NamedEntityType(0), URIType(0), TaxonomyType(0),
	  Text(0), NamedEntity(0), URI(0),
	  IsA(0), Synonym(0), PartOf(0),
	  ConceptTypes(1,40,20),
	  Concepts(1,200000,50000,150000,10,5),
	  StatementsByIds(100000,50000)
{
}


//-----------------------------------------------------------------------------
// General methods


//------------------------------------------------------------------------------
GConceptType* GKB::GetTokenType(void) const
{
	if(!TokenType)
		const_cast<GKB*>(this)->TokenType=const_cast<GKB*>(this)->InsertObj(pConceptType,ccText,"Tokens","Tokens");
	return(TokenType);
}


//------------------------------------------------------------------------------
GConceptType* GKB::GetDCMIType(void) const
{
	// Search for the concept types corresponding to the DMCI
	if(!DCMIType)
	{
		// Verify that all concepts are OK.
		const_cast<GKB*>(this)->DCMIType=const_cast<GKB*>(this)->InsertObj(pConceptType,ccMetadata,"http://purl.org/dc/elements/1.1","Dublin Core Metadata Initiative (DMCI)");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"contributor");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"coverage");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"creator");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"date");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"description");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"format");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"identifier");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"language");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"publisher");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"relation");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"rights");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"source");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"subject");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"title");
		const_cast<GKB*>(this)->InsertObj(pConcept,DCMIType,"type");
	}
	return(DCMIType);
}


//------------------------------------------------------------------------------
GConceptType* GKB::GetURIType(void) const
{
	if(!URIType)
		const_cast<GKB*>(this)->URIType=const_cast<GKB*>(this)->InsertObj(pConceptType,ccLink,"URI","Uniform Resource Identifier");
	return(URIType);
}


//------------------------------------------------------------------------------
GConceptType* GKB::GetNamedEntityType(void) const
{
	if(!NamedEntityType)
		const_cast<GKB*>(this)->NamedEntityType=const_cast<GKB*>(this)->InsertObj(pConceptType,ccText,"Named Entities","Named Entities");
	return(NamedEntityType);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetTaxonomyType(void) const
{
	if(!TaxonomyType)
	{
		const_cast<GKB*>(this)->TaxonomyType=const_cast<GKB*>(this)->InsertObj(pConceptType,ccPredicate,"Taxonomy","Taxonomy relations");
	}
	return(TaxonomyType);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetText(void) const
{
	if(!Text)
		const_cast<GKB*>(this)->Text=const_cast<GKB*>(this)->InsertObj(pConcept,Session->InsertObj(pConceptType,ccText,"Text blocks","Blocks of text"),"*");
	return(Text);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetNamedEntity(void) const
{
	if(!NamedEntity)
		const_cast<GKB*>(this)->NamedEntity=const_cast<GKB*>(this)->InsertObj(pConcept,GetNamedEntityType(),"*");
	return(NamedEntity);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetURI(void) const
{
	if(!URI)
		const_cast<GKB*>(this)->URI=const_cast<GKB*>(this)->InsertObj(pConcept,GetURIType(),"*");
	return(URI);
}


//-----------------------------------------------------------------------------
GConcept* GKB::GetIsA(void) const
{
	if(!IsA)
	{
		const_cast<GKB*>(this)->IsA=const_cast<GKB*>(this)->InsertObj(pConcept,GetTaxonomyType(),"IsA");
	}
	return(IsA);
}


//-----------------------------------------------------------------------------
GConcept* GKB::GetSynonym(void) const
{
	if(!Synonym)
	{
		const_cast<GKB*>(this)->Synonym=const_cast<GKB*>(this)->InsertObj(pConcept,GetTaxonomyType(),"Synonym");
	}
	return(Synonym);
}


//-----------------------------------------------------------------------------
GConcept* GKB::GetPartOf(void) const
{
	if(!PartOf)
	{
		const_cast<GKB*>(this)->PartOf=const_cast<GKB*>(this)->InsertObj(pConcept,GetTaxonomyType(),"PartOf");
	}
	return(PartOf);
}


//------------------------------------------------------------------------------
void GKB::ClearIndex(tObjType type)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	RCursor<GConcept> Concept(Concepts.GetObjs());
	for(Concept.Start();!Concept.End();Concept.Next())
		Concept()->ClearIndex(type);
	if(SaveResults)
		Storage->ClearIndex(type);
}


//-----------------------------------------------------------------------------
// Concept Type methods

//-----------------------------------------------------------------------------
GConceptType* GKB::GetObj(const GConceptType*,char id,bool null)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypes[id];
	}
	catch(...)
	{
	}
	if((!type)&&(!null))
		mThrowGException("Unknown concept type "+RString::Number(id));
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::GetObj(const GConceptType*,const RString& name,bool null)
{
	GConceptType* type(ConceptTypes.GetObj(name));
	if((!type)&&(!null))
		mThrowGException("Unknown concept type '"+name+"'");
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GKB::InsertObj(const GConceptType*,tConceptCat cat,size_t id,const RString& name,const RString& desc,bool& newone)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	GConceptType* type(ConceptTypes.GetObj(name));
	if(!type)
	{
		type=new GConceptType(Session,cat,id,name,desc,5000);
		if((!id)||(id==cNoRef))
		{
			Storage->AssignId(type);
			newone=true;
		}
		else
			newone=false;
		ConceptTypes.InsertObj(type);
	}
	else
		newone=false;

	return(type);
}


//-----------------------------------------------------------------------------
// Concept methods

//------------------------------------------------------------------------------
GConcept* GKB::GetObj(const GConcept*,size_t id)
{
	GConcept* concept(Concepts[id]);
	if(!concept)
		mThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(concept);
}


//------------------------------------------------------------------------------
GConcept* GKB::GetObj(const GConcept*,GConceptType* type,const R::RString& concept,bool null)
{
	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept Search(Session,concept,type);
	GConcept* ptr(Concepts.GetObj(Search));
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
GConcept* GKB::InsertObj(const GConcept*,GConceptType* type,const R::RString& concept,bool& newone)
{
  	if(!Storage)
		 mThrowGException("No storage");

	if(!type)
		mThrowGException("No concept type");

	if(concept.IsEmpty())
		mThrowGException("Cannot insert an empty concept");

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept Search(Session,concept,type);
	GConcept* ptr(Concepts.GetObj(Search));
	if(!ptr)
	{
		ptr=new GConcept(Session,concept,type);
		if(ptr->GetId()==cNoRef)
		{
			Storage->AssignId(ptr);
			newone=true;
		}
		else
			newone=false;
		type->InsertObj(ptr);
		Concepts.InsertObj(ptr);
	}
	newone=false;

	return(ptr);
}


//------------------------------------------------------------------------------
GConcept* GKB::InsertObj(const GConcept& concept,bool& newone)
{
  	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	GConceptType* Type;
	if(!(Type=concept.GetType()))
		mThrowGException("No concept type");

	// Invalid concept are not inserted
	if(concept.IsEmpty())
		mThrowGException("Cannot insert an empty concept");

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(Concepts.GetObj(concept));
	if(!ptr)
	{
		ptr=new GConcept(concept);
		// Look if data has an identifier. If not, assign one.
		if(ptr->GetId()==cNoRef)
		{
			Storage->AssignId(ptr);
			newone=true;
		}
		else
			newone=false;
		Type->InsertObj(ptr);
		Concepts.InsertObj(ptr);
	}
	else
		newone=false;

	return(ptr);
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
		Storage->SaveObj(concept);
		return(concept);
	}
}


//------------------------------------------------------------------------------
void GKB::DeleteObj(GConcept* concept)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	if((!concept)||(!concept->GetType()))
		mThrowGException("Cannot delete concept");
	Storage->DeleteObj(concept);
	concept->GetType()->DeleteObj(concept);
	Concepts.DeleteObj(concept);
}


//-----------------------------------------------------------------------------
// Statements methods

//-----------------------------------------------------------------------------
GStatement* GKB::GetObj(const GStatement*,size_t id,bool null)
{
	GStatement* Statement(0);
	if(id<=StatementsByIds.GetMaxPos())
		Statement=StatementsByIds[id];
	if((!Statement)&&(!null))
		mThrowGException("'"+RString::Number(id)+"' is not a valid statement identifier");
	return(Statement);
}


//-----------------------------------------------------------------------------
GStatement* GKB::GetObj(const GStatement*,GObject* subject,GObject* predicate,GObject* value,bool null)
{
	if(!subject)
		mThrowGException("Statement must have a valid subject");
	if(!predicate)
		mThrowGException("Statement must have a valid predicate");
	if(!value)
		mThrowGException("Statement must have a valid value");

	GStatement Search(cNoRef,subject,predicate,value,0.0);
	bool Find;
	size_t Idx;
	if(!subject->Subjects)
		Find=false;
	else
		Idx=subject->Subjects->GetIndex(Search,Find);
	if(!Find)
	{
		if(!null)
			mThrowGException("Cannot find the statement defined by the triple ("+
					GetObjType(subject->GetObjType(),false,false)+" with identifier "+RString::Number(subject->GetId())+","+
					GetObjType(predicate->GetObjType(),false,false)+" with identifier "+RString::Number(predicate->GetId())+","+
					GetObjType(value->GetObjType(),false,false)+" with identifier "+RString::Number(value->GetId())+")");
		return(0);
	}
	return((*subject->Subjects)[Idx]);
}


//-----------------------------------------------------------------------------
GStatement* GKB::InsertObj(const GStatement*,size_t id,GObject* subject,GObject* predicate,GObject* value,double weight,bool& newone)
{
	if(!Storage)
		 mThrowGException("Storage is not a valid pointer");

	if(!subject)
		mThrowGException("Statement must have a valid subject");
	if(!predicate)
		mThrowGException("Statement must have a valid predicate");
	if(!value)
		mThrowGException("Statement must have a valid value");

	// Statement to compute
	GStatement* Ret(0);

	// Look if we have a valid identifier
	if(id!=cNoRef)
	{
		if(StatementsByIds.GetMaxPos()<id)
		{
			Ret=new GStatement(id,subject,predicate,value,weight);
			Ret->GetSubject()->InsertObj(seSubject,Ret);
			Ret->GetPredicate()->InsertObj(sePredicate,Ret);
			Ret->GetValue()->InsertObj(seValue,Ret);
			StatementsByIds.InsertPtrAt(Ret,id,true);
			newone=false;
			return(Ret);
		}

		Ret=StatementsByIds[id];
		GStatement Search(cNoRef,subject,predicate,value,weight);
		if(Ret->Compare(Search))
		{
			// Not the same statements -> Replace the old one by the new one
			Ret->GetSubject()->DeleteObj(seSubject,Ret);
			Ret->GetPredicate()->DeleteObj(sePredicate,Ret);
			Ret->GetValue()->DeleteObj(seValue,Ret);
			Ret=new GStatement(id,subject,predicate,value,weight);
			StatementsByIds.InsertPtrAt(Ret,id,true);
			Ret->GetSubject()->InsertObj(seSubject,Ret);
			Ret->GetPredicate()->InsertObj(sePredicate,Ret);
			Ret->GetValue()->InsertObj(seValue,Ret);
			newone=true;

		}
		else
		{
			Ret->SetWeight(weight);
			newone=false;
		}

		return(Ret);
	}

	// Look if the statement exist
	Ret=GetObj(pStatement,subject,predicate,value,true);
	if(Ret)
	{
		Ret->SetWeight(weight);
		newone=false;
	}
	else
	{
		newone=true;
		Ret=new GStatement(id,subject,predicate,value,weight);
		Ret->GetSubject()->InsertObj(seSubject,Ret);
		Ret->GetPredicate()->InsertObj(sePredicate,Ret);
		Ret->GetValue()->InsertObj(seValue,Ret);

		// Look if data has an identifier. If not, assign one.
		if(Ret->GetId()==cNoRef)
			Storage->AssignId(Ret);

		StatementsByIds.InsertPtrAt(Ret,Ret->GetId(),true);
	}

	return(Ret);
}


//------------------------------------------------------------------------------
GKB::~GKB(void)
{
}
