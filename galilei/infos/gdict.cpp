/*

	GALILEI Research Project

	GDict.cpp

	Dictionary - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <infos/gdata.h>
#include <infos/gdict.h>
#include <infos/gwordlist.h>
#include <infos/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDict::GDataTypes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDict::GDataTypes : public R::RContainer<GData,unsigned int,false,false>
{
public:
	GInfoType Type;

	GDataTypes(unsigned int ml,GInfoType type)
		: RContainer<GData,unsigned int,false,false>(ml+(ml/4),ml/4), Type(type) {}
	int Compare(const GDataTypes& d) const {return(Type-d.Type);}
	int Compare(const GDataTypes* d) const {return(Type-d->Type);}
	int Compare(const GInfoType t) const {return(Type-t);}
};



//------------------------------------------------------------------------------
//
// class GDict
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDict::GDict(const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st) throw(bad_alloc)
	: RDblHashContainer<GData,unsigned,27,27,true>(ml+(ml/4),ml/4), Direct(0),
	  MaxId(m+m/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false),
	  Stop(st), NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0), DataTypes(3)
{
	Direct=new GData*[MaxId];
	memset(Direct,0,MaxId*sizeof(GData*));
	DataTypes.InsertPtr(new GDataTypes(ml,infoWord));
	DataTypes.InsertPtr(new GDataTypes(ml,infoWordList));
}


//------------------------------------------------------------------------------
void GDict::Clear(void)
{
	RDblHashContainer<GData,unsigned,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GData*));
	UsedId=0;
	Loaded=false;
}


//------------------------------------------------------------------------------
void GDict::PutDirect(GData* data) throw(bad_alloc)
{
	GData **tmp;
	unsigned n;

	if(data->GetId()>UsedId) UsedId=data->GetId();
	if(data->GetId()>=MaxId)
	{
		n=data->GetId()+1000;
		tmp=new GData*[n];
		memcpy(tmp,Direct,MaxId*sizeof(GData*));
		delete[] Direct;
		Direct=tmp;
		MaxId=n;
	}
	Direct[data->GetId()]=data;
}


//------------------------------------------------------------------------------
unsigned int GDict::InsertData(const GData* data) throw(bad_alloc, GException)
{
	GData* ptr;
	bool InDirect=false;
	unsigned int Id;

	// Empty data are not inserted
	if(data->IsEmpty())
		throw GException("Empty data cannot be inserted into a dictionary");
	if(data->GetType()==infoNothing)
		throw GException("No typed data cannot be inserted into a dictionary");

	// Look if the data exists in the dictionary. If not, create and insert it.
	ptr=GetPtr<const GData*>(data);
	if(!ptr)
	{
		ptr=data->CreateCopy();
		InsertPtr(ptr);
		DataTypes.GetPtr<GInfoType>(ptr->GetType())->InsertPtr(ptr);
		InDirect=true;
	}

	// Look if data has an identificator. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Id=Lang->GetSession()->GetDicNextId(ptr,this);
		ptr->SetId(Id);
		InDirect=true;
	}
	else
		Id=ptr->GetId();

	// Finally, if an identificator has been assigned and/or a new one -> Direct
	if(InDirect)
		PutDirect(ptr);

	return(Id);
}


//------------------------------------------------------------------------------
void GDict::DeleteData(GData* data) throw(bad_alloc, GException)
{
	if((!data)||(data->GetId()>MaxId))
		throw GException("Cannot delete data");
	Direct[data->GetId()]=0;
	DataTypes.GetPtr<GInfoType>(data->GetType())->DeletePtr(data);
	DeletePtr(data);
}


//------------------------------------------------------------------------------
GData* GDict::GetData(const unsigned int id) const throw(GException)
{
	if(id>MaxId)
		throw GException("Cannot access data");
	return(Direct[id]);
}


//------------------------------------------------------------------------------
GDataCursor& GDict::GetDataCursor(GInfoType type) throw(bad_alloc,GException)
{
	GDataCursor* cur=GDataCursor::GetTmpCursor();
	GDataTypes* tp;

	tp=DataTypes.GetPtr<GInfoType>(type);
	if(!tp)
		throw GException("Unknown information type");
	cur->Set(tp);
	return(*cur);
}


//------------------------------------------------------------------------------
RString& GDict::GetName(void) const throw(bad_alloc)
{
	RString* tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//------------------------------------------------------------------------------
unsigned int GDict::GetNbDatas(GInfoType type) const throw(GException)
{
	GDataTypes* tp;

	tp=DataTypes.GetPtr<GInfoType>(type);
	if(!tp)
		throw GException("Unknown information type");
	return(tp->NbPtr);
}


//------------------------------------------------------------------------------
bool GDict::IsIn(const RString& name) const
{
	return(RDblHashContainer<GData,unsigned,27,27,true>::IsIn<const RString>(name));
}


//------------------------------------------------------------------------------
void GDict::IncRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(id>MaxId)
		throw GException("Cannot access data");
	Direct[id]->IncRef(ObjType);
}


//------------------------------------------------------------------------------
void GDict::DecRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(id>MaxId)
		throw GException("Cannot access data");
	Direct[id]->DecRef(ObjType);
}


//------------------------------------------------------------------------------
unsigned int GDict::GetRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(id>MaxId)
		throw GException("Cannot access data");
	return(Direct[id]->GetRef(ObjType));
}


//------------------------------------------------------------------------------
void GDict::IncRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs++;
			break;
		case otSubProfile:
			NbRefSubProfiles++;
			break;
		case otGroup:
			NbRefGroups++;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GDict::DecRef(tObjType ObjType) throw(GException)
{
	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				throw GException("Cannot decrease null number of references");
			NbRefDocs--;
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references");
			NbRefSubProfiles--;
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references");
			NbRefGroups--;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
unsigned int GDict::GetRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			return(NbRefDocs);
			break;
		case otSubProfile:
			return(NbRefSubProfiles);
			break;
		case otGroup:
			return(NbRefGroups);
			break;
		default:
			return(NbRefDocs+NbRefSubProfiles+NbRefGroups);
			break;
	}
	return(0);
}


//------------------------------------------------------------------------------
GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
