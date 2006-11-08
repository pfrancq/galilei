/*

	GALILEI Research Project

	GDict.cpp

	Concepts Dictionary - Implementation.

	Copyright 2001-2006 by the Université Libre de Bruxelles.

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
// include file for GALILEI
#include <gconcept.h>
#include <gdict.h>
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDict
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDict::GDict(GLang *lang,unsigned int type,unsigned m,unsigned ml)
	: RDblHashContainer<GConcept,27,27,true>(ml+(ml/4),ml/4), Lang(lang), Type(type),
	  Direct(0), MaxId(m+m/4), UsedId(0), Loaded(false)
{
	Direct=new GConcept*[MaxId];
	memset(Direct,0,MaxId*sizeof(GConcept*));
}


//------------------------------------------------------------------------------
void GDict::Clear(void)
{
	RDblHashContainer<GConcept,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GConcept*));
	UsedId=0;
	Loaded=false;
}


//------------------------------------------------------------------------------
int GDict::Compare(const GDict& dict) const
{
	return(Type-dict.Type);
}


//------------------------------------------------------------------------------
int GDict::Compare(unsigned int type) const
{
	return(Type-type);
}


//------------------------------------------------------------------------------
void GDict::PutDirect(GConcept* concept)
{
	GConcept **tmp;
	unsigned n;

	if(concept->GetId()>UsedId) UsedId=concept->GetId();
	if(concept->GetId()>=MaxId)
	{
		n=concept->GetId()+1000;
		tmp=new GConcept*[n];
		memcpy(tmp,Direct,MaxId*sizeof(GConcept*));
		delete[] Direct;
		Direct=tmp;
		MaxId=n;
	}
	Direct[concept->GetId()]=concept;
}


//------------------------------------------------------------------------------
GConcept* GDict::InsertConcept(const GConcept* concept)
{
	GConcept* ptr;
	bool InDirect=false;

	// Empty data are not inserted
	if(concept->IsEmpty())
	{
		RString tmp="Empty concept cannot be inserted into a dictionary - id="+RString(concept->GetId());
		throw GException(tmp);
	}
	if(concept->GetType()==infoNothing)
		throw GException("No typed concept cannot be inserted into a dictionary");

	// Look if the data exists in the dictionary. If not, create and insert it.
	ptr=GetPtr(*concept);
	if(!ptr)
	{
		ptr=new GConcept(*concept);
		InsertPtr(ptr);
		InDirect=true;
	}

	// Look if data has an identificator. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Lang->GetSession()->AssignId(ptr,this);
		InDirect=true;
	}

	// Finally, if an identificator has been assigned and/or a new one -> Direct
	if(InDirect)
		PutDirect(ptr);

	return(ptr);
}


//------------------------------------------------------------------------------
void GDict::DeleteConcept(GConcept* concept)
{
	if((!concept)||(concept->GetId()>MaxId))
		throw GException("Cannot delete concept");
	Direct[concept->GetId()]=0;
	if(concept->GetId()==UsedId)
	{
		GConcept** concepts=&Direct[UsedId];
		for(concepts--,UsedId--;UsedId&&(!(*concepts));UsedId--);
	}
	Lang->GetSession()->GetStorage()->DeleteConcept(concept);
	DeletePtr(*concept);
}


//------------------------------------------------------------------------------
GConcept* GDict::GetConcept(const unsigned int id) const
{
	if(id>MaxId)
		throw GException("Cannot access concept");
	return(Direct[id]);
}


//------------------------------------------------------------------------------
bool GDict::IsIn(const RString& name) const
{
	return(RDblHashContainer<GConcept,27,27,true>::IsIn(name));
}


//------------------------------------------------------------------------------
GConcept* GDict::GetConcept(const RString& name) const
{
	return(RDblHashContainer<GConcept,27,27,true>::GetPtr(name));
}


//------------------------------------------------------------------------------
unsigned int GDict::IncRef(unsigned int id,tObjType ObjType)
{
	GConcept* concept;

	if((id>MaxId)||(!(concept=Direct[id])))
		throw GException("Cannot access concept");
	return(concept->IncRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GDict::DecRef(unsigned int id,tObjType ObjType)
{
	GConcept* concept;

	if((id>MaxId)||(!(concept=Direct[id])))
		throw GException("Cannot access concept");
	return(concept->DecRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GDict::GetRef(unsigned int id,tObjType ObjType)
{
	GConcept* concept;

	if((id>MaxId)||(!(concept=Direct[id])))
		throw GException("Cannot access concept");
	return(concept->GetRef(ObjType));
}


//------------------------------------------------------------------------------
GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
