/*

	GALILEI Research Project

	GDict.cpp

	Dictionary - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
class GDict::GDataTypes : public R::RContainer<GData,false,false>
{
public:
	GInfoType Type;

	GDataTypes(unsigned int ml,GInfoType type)
		: RContainer<GData,false,false>(ml+(ml/4),ml/4), Type(type) {}
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
GDict::GDict(const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st) throw(std::bad_alloc)
	: RDblHashContainer<GData,27,27,true>(ml+(ml/4),ml/4), Direct(0),
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
	RDblHashContainer<GData,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GData*));
	UsedId=0;
	Loaded=false;
}


//------------------------------------------------------------------------------
void GDict::PutDirect(GData* data) throw(std::bad_alloc)
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
unsigned int GDict::InsertData(const GData* data) throw(std::bad_alloc, GException)
{
	GData* ptr;
	bool InDirect=false;

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
		Lang->GetSession()->AssignId(ptr,this);
		InDirect=true;
	}

	// Finally, if an identificator has been assigned and/or a new one -> Direct
	if(InDirect)
		PutDirect(ptr);

	return(ptr->GetId());
}


//------------------------------------------------------------------------------
void GDict::DeleteData(GData* data) throw(std::bad_alloc, GException)
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
GDataCursor GDict::GetDataCursor(GInfoType type) throw(std::bad_alloc,GException)
{
	GDataCursor cur;
	GDataTypes* tp;

	tp=DataTypes.GetPtr<GInfoType>(type);
	if(!tp)
		throw GException("Unknown information type");
	cur.Set(tp);
	return(cur);
}


//------------------------------------------------------------------------------
RString GDict::GetName(void) const throw(std::bad_alloc)
{
	return(Name);
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
	return(RDblHashContainer<GData,27,27,true>::IsIn<const RString>(name));
}


//------------------------------------------------------------------------------
void GDict::IncRef(unsigned int id,tObjType ObjType) throw(GException)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	data->IncRef(ObjType);
}


//------------------------------------------------------------------------------
void GDict::DecRef(unsigned int id,tObjType ObjType) throw(GException)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	data->DecRef(ObjType);
}


//------------------------------------------------------------------------------
unsigned int GDict::GetRef(unsigned int id,tObjType ObjType) throw(GException)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	return(data->GetRef(ObjType));
}


//------------------------------------------------------------------------------
void GDict::IncRef(tObjType ObjType) throw(GException)
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
			throw GException ("Unkown type to increase");
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
				throw GException("Cannot decrease null number of references for documents");
			NbRefDocs--;
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references for subprofiles");
			NbRefSubProfiles--;
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups");
			NbRefGroups--;
			break;
		default:
			throw GException ("Unkown type to decrease");
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
