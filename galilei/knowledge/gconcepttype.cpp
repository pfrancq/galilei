/*

	GALILEI Research Project

	GConceptType.cpp

	Concept Type - Implementation.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GConceptType
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConceptType::GConceptType(unsigned int id,GSession* session,const RString& name,const RString& desc,GLang* lang,size_t s,size_t s2)
	: GDebugObject(name), RDblHashContainer<GConcept,true>(27,27,s2+(s2/4),s2/4), Id(id), Session(session),
	  Description(desc), Lang(lang), Direct(0), MaxId(s+s/4), UsedId(0),
	  Loaded(false)
{
	if((Id==cNoRef)&&(GSession::Get()))
		GSession::Get()->AssignId(this);		
	Direct=new GConcept*[MaxId];
	memset(Direct,0,MaxId*sizeof(GConcept*));
}

	
//------------------------------------------------------------------------------
void GConceptType::SetReferences(size_t refdocs,size_t refprofiles,size_t refgroups)
{
	NbRefDocs=refdocs;
	NbRefProfiles=refprofiles;
	NbRefGroups=refgroups;
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType& type) const
{
	return(Id-type.Id);
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType* type) const
{
	return(Id-type->Id);
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(size_t id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GConceptType::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
void GConceptType::SetId(size_t id)
{
	Id=id;
}


//------------------------------------------------------------------------------
void GConceptType::Load(void) const
{
	if((!Loaded)&&Session&&Session->GetStorage())
	{
		Session->GetStorage()->LoadConcepts(const_cast<GConceptType*>(this));
		const_cast<GConceptType*>(this)->Loaded=true;
	}
}


//------------------------------------------------------------------------------
RString GConceptType::GetDebugInfo(const RString& info)
{
	// Look what to do
	bool Idf=(info.FindStr("idf")!=-1);
	bool Ipf=(info.FindStr("ipf")!=-1);
	bool Igf=(info.FindStr("igf")!=-1);
	if((!Idf)&&(!Ipf)&&(!Igf))
		return(RString::Null);
	
	Load(); // Load the concepts if necessary
	
	RString str;
	GConcept** ptr;
	size_t i;
	for(i=MaxId+1,ptr=Direct;--i;ptr++)
	{
		if(!(*ptr))
			continue;
		
		// Suppose we reserved 32 characters for names
		RString name=(*ptr)->GetName();
		str+=name;
		if(name.GetLen()<32)
		{
			for(size_t j=32-name.GetLen()+1;--j;)
				str+=' ';
		}
		if(Idf)	
			str+="\t"+RString::Number(GetIF((*ptr)->GetId(),otDoc));		
		if(Ipf)	
			str+="\t"+RString::Number(GetIF((*ptr)->GetId(),otProfile));
		if(Igf)	
			str+="\t"+RString::Number(GetIF((*ptr)->GetId(),otGroup));		
		str+="\n";
	}
	return(str);
}


//------------------------------------------------------------------------------
void GConceptType::Clear(void)
{
	RDblHashContainer<GConcept,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GConcept*));
	UsedId=0;
	Loaded=false;
	SetReferences(0,0,0);
}


//------------------------------------------------------------------------------
void GConceptType::PutDirect(GConcept* concept)
{
	GConcept **tmp;
	unsigned n;

	if(concept->GetId()>UsedId) UsedId=concept->GetId();
	if(concept->GetId()>=MaxId)
	{
		n=concept->GetId()+1000;
		tmp=new GConcept*[n];
		memcpy(tmp,Direct,MaxId*sizeof(GConcept*));
		memset(&tmp[MaxId],0,(n-MaxId)*sizeof(GConcept*));
		delete[] Direct;
		Direct=tmp;
		MaxId=n;
	}
	Direct[concept->GetId()]=concept;
}


//------------------------------------------------------------------------------
GConcept* GConceptType::InsertConcept(const GConcept* concept)
{
	GConcept* ptr;
	bool InDirect=false;

	// Empty data are not inserted
	if(concept->IsEmpty())
	{
		RString tmp="Empty concept cannot be inserted into a dictionary - id="+RString(concept->GetId());
		throw GException(tmp);
	}
	if(concept->GetType()!=this)
		throw GException("Concept has not the correct type");

	// Look if the data exists in the dictionary. If not, create and insert it.
	ptr=GetPtr(*concept);
	if(!ptr)
	{
		ptr=concept->DeepCopy();
		InsertPtr(ptr);
		InDirect=true;
	}

	// Look if data has an identificator. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Session->AssignId(ptr);
		InDirect=true;
	}

	// Finally, if an identificator has been assigned and/or a new one -> Direct
	if(InDirect)
		PutDirect(ptr);

	return(ptr);
}


//------------------------------------------------------------------------------
void GConceptType::DeleteConcept(GConcept* concept)
{
	if((!concept)||(concept->GetId()>MaxId))
		throw GException("Cannot delete concept");
	Direct[concept->GetId()]=0;
	if(concept->GetId()==UsedId)
	{
		GConcept** concepts=&Direct[UsedId];
		for(concepts--,UsedId--;UsedId&&(!(*concepts));UsedId--);
	}
	Session->GetStorage()->DeleteConcept(concept);
	DeletePtr(*concept);
}


//------------------------------------------------------------------------------
GConcept* GConceptType::GetConcept(size_t id) const
{
	if(id>MaxId)
		throw GException("Cannot access "+Description+" "+RString::Number(id)+">"+RString::Number(MaxId));
	return(Direct[id]);
}


//------------------------------------------------------------------------------
bool GConceptType::IsIn(const RString& name) const
{
	return(RDblHashContainer<GConcept,true>::IsIn(name));
}


//------------------------------------------------------------------------------
GConcept* GConceptType::GetConcept(const RString& name) const
{
	return(RDblHashContainer<GConcept,true>::GetPtr(name));
}


//------------------------------------------------------------------------------
double GConceptType::GetIF(size_t id,tObjType ObjType)
{
	double nb=Direct[id]->GetRef(ObjType);
	if(nb>0.0)
		return(log10(GetRef(ObjType)/nb));
	return(0.0);
}


//------------------------------------------------------------------------------
void GConceptType::IncRef(size_t id,tObjType ObjType)
{
	GConcept* concept;

	if(id>MaxId)
		throw GException("Cannot access "+Description+" "+RString::Number(id)+">"+RString::Number(MaxId));	
	if(!(concept=Direct[id]))
		throw GException(Description+" "+RString::Number(id)+" does not exist");
	size_t nb=concept->IncRef(ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(concept,ObjType,nb);
}


//------------------------------------------------------------------------------
void GConceptType::DecRef(size_t id,tObjType ObjType)
{
	GConcept* concept;

	if(id>MaxId)
		throw GException("Cannot access "+Description+" "+RString::Number(id)+">"+RString::Number(MaxId));	
	if(!(concept=Direct[id]))
		throw GException(Description+" "+RString::Number(id)+" does not exist");
	size_t nb=concept->DecRef(ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(concept,ObjType,nb);
}


//------------------------------------------------------------------------------
size_t GConceptType::GetRef(size_t id,tObjType ObjType)
{
	GConcept* concept;

	if(id>MaxId)
		throw GException("Cannot access "+Description+" "+RString::Number(id)+">"+RString::Number(MaxId));	
	if(!(concept=Direct[id]))
		throw GException(Description+" "+RString::Number(id)+" does not exist");
	return(concept->GetRef(ObjType));
}


//------------------------------------------------------------------------------
void GConceptType::IncRef(tObjType ObjType)
{
	unsigned int nb;

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
		case otGroup:
			NbRefGroups++;
			nb=NbRefGroups;
			break;
		default:
			throw GException ("Unkown type to increase");
			break;
	}
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(this,ObjType,nb);
}


//------------------------------------------------------------------------------
void GConceptType::DecRef(tObjType ObjType)
{
	unsigned int nb;

	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				throw GException("Cannot decrease null number of references for documents");
			NbRefDocs--;
			nb=NbRefDocs;
			break;
		case otProfile:
			if(!NbRefProfiles)
				throw GException("Cannot decrease null number of references for profiles");
			NbRefProfiles--;
			nb=NbRefProfiles;
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups");
			NbRefGroups--;
			nb=NbRefGroups;
			break;
		default:
			throw GException("Unkown type to decrease");
			break;
	}
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(this,ObjType,nb);
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
		case otGroup:
			return(NbRefGroups);
			break;
		default:
			return(NbRefDocs+NbRefProfiles+NbRefGroups);
			break;
	}
	return(0);
}


//------------------------------------------------------------------------------
void GConceptType::Clear(tObjType ObjType)
{
	GConcept** ptr;
	size_t i;
	
	// Look once if the results must be saved
	Load(); // Load the concepts if necessary
	
	for(i=MaxId+1,ptr=Direct;--i;ptr++)
	{
		if(!(*ptr)) continue;
		(*ptr)->Clear(ObjType);
	}
	
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otProfile:
			NbRefProfiles=0;
			break;
		case otGroup:
			NbRefGroups=0;
			break;
		case otDocProfile:
			NbRefDocs=0;
			NbRefProfiles=0;
			break;
		case otDocGroup:
			NbRefDocs=0;
			NbRefGroups=0;
			break;
		case otProfileGroup:
			NbRefProfiles=0;
			NbRefGroups=0;
			break;
		default:
			NbRefDocs=0;
			NbRefProfiles=0;
			NbRefGroups=0;
			break;
	}
	
	// If necessary, put the references to 0. The storage should also reset all the references for the concepts
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(this,ObjType,0);
}


//------------------------------------------------------------------------------
GConceptType::~GConceptType(void)
{
	if(Direct) delete[] Direct;
}
