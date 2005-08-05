/*

	GALILEI Research Project

	GDict.cpp

	Dictionary - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
#include <gdata.h>
#include <gdict.h>
#include <gwordlist.h>
#include <glang.h>
#include <gsession.h>
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
	tInfoType Type;

	GDataTypes(unsigned int ml,tInfoType type)
		: RContainer<GData,false,false>(ml+(ml/4),ml/4), Type(type) {}
	int Compare(const GDataTypes& d) const {return(Type-d.Type);}
	int Compare(const GDataTypes* d) const {return(Type-d->Type);}
	int Compare(const tInfoType t) const {return(Type-t);}
};



//------------------------------------------------------------------------------
//
// class GDict
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDict::GDict(const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
	: RDblHashContainer<GData,27,27,true>(ml+(ml/4),ml/4), Direct(0),
	  MaxId(m+m/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false),
	  Stop(st), NbRefDocs(refdocs), NbRefSubProfiles(refsubprofiles), NbRefGroups(refgroups), DataTypes(3)
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
void GDict::PutDirect(GData* data)
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
unsigned int GDict::InsertData(const GData* data)
{
	GData* ptr;
	bool InDirect=false;

	// Empty data are not inserted

	if(data->IsEmpty())
	{
		RString tmp="Empty data cannot be inserted into a dictionary - id="+RString(data->GetId());
		throw GException(tmp);
	}
	if(data->GetType()==infoNothing)
		throw GException("No typed data cannot be inserted into a dictionary");

	// Look if the data exists in the dictionary. If not, create and insert it.
	ptr=GetPtr<const GData*>(data);
	if(!ptr)
	{
		ptr=data->CreateCopy();
		InsertPtr(ptr);
		DataTypes.GetPtr<tInfoType>(ptr->GetType())->InsertPtr(ptr);
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
void GDict::DeleteData(GData* data)
{
	if((!data)||(data->GetId()>MaxId))
		throw GException("Cannot delete data");
	Direct[data->GetId()]=0;
	DataTypes.GetPtr<tInfoType>(data->GetType())->DeletePtr(data);
	DeletePtr(data);
}


//------------------------------------------------------------------------------
GData* GDict::GetData(const unsigned int id) const
{
	if(id>MaxId)
		throw GException("Cannot access data");
	return(Direct[id]);
}


//------------------------------------------------------------------------------
R::RCursor<GData> GDict::GetDatas(tInfoType type)
{
	R::RCursor<GData> cur;
	GDataTypes* tp;

	tp=DataTypes.GetPtr<tInfoType>(type);
	if(!tp)
		throw GException("Unknown information type");
	cur.Set(*tp);
	return(cur);
}


//------------------------------------------------------------------------------
RString GDict::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
unsigned int GDict::GetNbDatas(tInfoType type) const
{
	GDataTypes* tp;

	tp=DataTypes.GetPtr<tInfoType>(type);
	if(!tp)
		throw GException("Unknown information type");
	return(tp->GetNb());
}


//------------------------------------------------------------------------------
bool GDict::IsIn(const RString& name) const
{
	return(RDblHashContainer<GData,27,27,true>::IsIn<const RString>(name));
}


//------------------------------------------------------------------------------
GData* GDict::GetData(const RString& name) const
{
	return(RDblHashContainer<GData,27,27,true>::GetPtr<const RString>(name));
}


//------------------------------------------------------------------------------
unsigned int GDict::IncRef(unsigned int id,tObjType ObjType)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	return(data->IncRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GDict::DecRef(unsigned int id,tObjType ObjType)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	return(data->DecRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GDict::GetRef(unsigned int id,tObjType ObjType)
{
	GData* data;

	if((id>MaxId)||(!(data=Direct[id])))
		throw GException("Cannot access data");
	return(data->GetRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GDict::IncRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			return(++NbRefDocs);
			break;
		case otSubProfile:
			return(++NbRefSubProfiles);
			break;
		case otGroup:
			return(++NbRefGroups);
			break;
		default:
			throw GException ("Unkown type to increase");
			break;
	}
}


//------------------------------------------------------------------------------
unsigned int GDict::DecRef(tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				throw GException("Cannot decrease null number of references for documents");
			return(--NbRefDocs);
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references for subprofiles");
			return(--NbRefSubProfiles);
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups");
			return(--NbRefGroups);
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
void GDict::Clear(tObjType ObjType)
{
	RContainer<GData,true,true>*** ptr;
	RContainer<GData,true,true>** ptr2;
	unsigned int i,j;

	for(i=27+1,ptr=Hash;--i;ptr++)
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			RCursor<GData> datas(**ptr2);
			for(datas.Start();!datas.End();datas.Next())
				datas()->Clear(ObjType);
		}

	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otSubProfile:
			NbRefSubProfiles=0;
			break;
		case otGroup:
			NbRefGroups=0;
			break;
		case otDocSubProfile:
			NbRefDocs=0;
			NbRefSubProfiles=0;
			break;
		case otDocGroup:
			NbRefDocs=0;
			NbRefGroups=0;
			break;
		case otSubProfileGroup:
			NbRefSubProfiles=0;
			NbRefGroups=0;
			break;
		default:
			NbRefDocs=0;
			NbRefSubProfiles=0;
			NbRefGroups=0;
			break;
	}
}


//------------------------------------------------------------------------------
GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
