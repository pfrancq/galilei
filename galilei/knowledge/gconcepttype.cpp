/*

	GALILEI Research Project

	GConceptType.cpp

	Concept Type - Implementation.

	Copyright 2006-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#include <math.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gconcepttype.h>
#include <gconcept.h>
#include <gsession.h>
#include <gontology.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GConceptType
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConceptType::GConceptType(size_t id,GOntology* ontology,const RString& name,const RString& desc,GLang* lang,size_t s)
	: GObject(id,name,otConceptType), RDblHashContainer<GConcept,false>(27,27,s,s/4),
	  Ontology(ontology), Description(desc), Lang(lang), NbRefDocs(0), NbRefProfiles(0),
	  NbRefCommunities(0), NbRefTopics(0), NbRefClasses(0)
{
}


//------------------------------------------------------------------------------
void GConceptType::SetReferences(size_t refdocs,size_t refprofiles,size_t refcommunities,size_t reftopics,size_t refclasses)
{
	NbRefDocs=refdocs;
	NbRefProfiles=refprofiles;
	NbRefCommunities=refcommunities;
	NbRefTopics=reftopics;
	NbRefClasses=refclasses;
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType& type) const
{
	return(Name.Compare(type.Name));
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType* type) const
{
	return(Name.Compare(type->Name));
}


//------------------------------------------------------------------------------
int GConceptType::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
bool GConceptType::IsIn(const RString& name) const
{
	return(RDblHashContainer<GConcept,false>::IsIn(name));
}


//------------------------------------------------------------------------------
GConcept* GConceptType::GetConcept(const RString& name) const
{
	return(RDblHashContainer<GConcept,false>::GetPtr(name));
}


//------------------------------------------------------------------------------
void GConceptType::IncRef(tObjType ObjType)
{
	size_t nb;

	switch(ObjType)
	{
		case otDoc:
			NbRefDocs++;
			nb=NbRefDocs;
			break;
		case otProfile:
			NbRefProfiles++;
			nb=NbRefProfiles;
			break;
		case otCommunity:
			NbRefCommunities++;
			nb=NbRefCommunities;
			break;
		case otTopic:
			NbRefTopics++;
			nb=NbRefTopics;
			break;
		case otClass:
			NbRefClasses++;
			nb=NbRefClasses;
		default:
			ThrowGException(GALILEI::GetObjType(ObjType,true,true)+" have no references");
			break;
	}
	if(Ontology->SaveResults)
		Ontology->Storage->SaveRefs(this,ObjType,nb);
}


//------------------------------------------------------------------------------
void GConceptType::DecRef(tObjType ObjType)
{
	size_t nb;

	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				ThrowGException("Cannot decrease null number of references for documents");
			NbRefDocs--;
			nb=NbRefDocs;
			break;
		case otProfile:
			if(!NbRefProfiles)
				ThrowGException("Cannot decrease null number of references for profiles");
			NbRefProfiles--;
			nb=NbRefProfiles;
			break;
		case otCommunity:
			if(!NbRefCommunities)
				ThrowGException("Cannot decrease null number of references for communities");
			NbRefCommunities--;
			nb=NbRefCommunities;
			break;
		case otTopic:
			if(!NbRefTopics)
				ThrowGException("Cannot decrease null number of references for topics");
			NbRefTopics--;
			nb=NbRefTopics;
			break;
		case otClass:
			if(!NbRefClasses)
				ThrowGException("Cannot decrease null number of references for classes");
			NbRefClasses--;
			nb=NbRefClasses;
			break;
		default:
			ThrowGException(GALILEI::GetObjType(ObjType,true,true)+" have no references");
			break;
	}
	if(Ontology->SaveResults)
		Ontology->Storage->SaveRefs(this,ObjType,nb);
}


//------------------------------------------------------------------------------
size_t GConceptType::GetRef(tObjType ObjType) const
{
	switch(ObjType)
	{
		case otDoc:
			return(NbRefDocs);
			break;
		case otProfile:
			return(NbRefProfiles);
			break;
		case otCommunity:
			return(NbRefCommunities);
			break;
		case otTopic:
			return(NbRefTopics);
			break;
		case otClass:
			return(NbRefClasses);
			break;
		case otAnyClass:
			return(NbRefDocs+NbRefProfiles+NbRefCommunities+NbRefTopics+NbRefClasses);
			break;
		default:
			ThrowGException(GALILEI::GetObjType(ObjType,true,true)+" have no references");
			break;
	}
	return(0);
}


//------------------------------------------------------------------------------
void GConceptType::ClearRef(tObjType ObjType)
{
	// Look once if the results must be saved
	bool Save(Ontology->MustSaveResults());

    // Parse the double hash table
    RCursor<RDblHashContainer<GConcept,false>::Hash> Cur(GetCursor());
    for(Cur.Start();!Cur.End();Cur.Next())
    {
       RCursor<RDblHashContainer<GConcept,false>::Hash2> Cur2(*Cur());
       for(Cur2.Start();!Cur2.End();Cur2.Next())
       {
          RCursor<GConcept> Cur3(*Cur2());
          for(Cur3.Start();!Cur3.End();Cur3.Next())
        	  Cur3()->ClearRef(ObjType);
       }
    }

	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otProfile:
			NbRefProfiles=0;
			break;
		case otCommunity:
			NbRefCommunities=0;
			break;
		case otTopic:
			NbRefTopics=0;
			break;
		case otClass:
			NbRefClasses=0;
			break;
		default:
			ThrowGException(GALILEI::GetObjType(ObjType,true,true)+" have no references");
			break;
	}

	// If necessary, put the references to 0. The storage should also reset all the references for the concepts.
	if(Save)
		Ontology->Storage->SaveRefs(this,ObjType,0);
}


//------------------------------------------------------------------------------
size_t GConceptType::IncRef(GConcept* concept,tObjType ObjType)
{
	size_t nb(concept->IncRef(ObjType));
	if(Ontology->SaveResults)
		Ontology->Storage->SaveRefs(concept,ObjType,nb);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GConceptType::DecRef(GConcept* concept,tObjType ObjType)
{
	size_t nb(concept->DecRef(ObjType));
	if(Ontology->SaveResults)
		Ontology->Storage->SaveRefs(concept,ObjType,nb);
	return(nb);
}


//------------------------------------------------------------------------------
GConceptType::~GConceptType(void)
{
}
