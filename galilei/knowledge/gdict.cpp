/*

	GALILEI Research Project

	GDic.cpp

	Dictionnary - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//---------------------------------------------------------------------------
// include file for GALILEI
#include <infos/gdata.h>
#include <infos/gdict.h>
#include <infos/gwordlist.h>
#include <infos/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//---------------------------------------------------------------------------
//
// class GDict::GDataTypes
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
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


//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDict::GDict(GSession* s,const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st) throw(bad_alloc)
	: RDblHashContainer<GData,unsigned,27,27,true>(ml+(ml/4),ml/4), Session(s), Direct(0),
	  /*NbGroupsList(0),*/ MaxId(m+m/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false), Stop(st),
	  DataTypes(3)//, GroupsList(500)
{
	for(unsigned int i=0;i<2;i++)
	{
		NbRefDocs[i]=0;
		NbRefSubProfiles[i]=0;
		NbRefGroups[i]=0;
	}
	Direct=new GData*[MaxId];
	memset(Direct,0,MaxId*sizeof(GData*));
	DataTypes.InsertPtr(new GDataTypes(ml,infoWord));
	DataTypes.InsertPtr(new GDataTypes(ml,infoWordList));
}


//---------------------------------------------------------------------------
void GDict::Clear(void)
{
	RDblHashContainer<GData,unsigned,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GData*));
	UsedId=0;
	Loaded=false;
}


//---------------------------------------------------------------------------
void GDict::PutDirect(GData* word) throw(bad_alloc)
{
	GData **tmp;
	unsigned n;

	if(word->GetId()>UsedId) UsedId=word->GetId();
	if(word->GetId()>=MaxId)
	{
		n=word->GetId()+1000;
		tmp=new GData*[n];
		memcpy(tmp,Direct,MaxId*sizeof(GData*));
		delete[] Direct;
		Direct=tmp;
		MaxId=n;
	}
	Direct[word->GetId()]=word;
}


//---------------------------------------------------------------------------
unsigned int GDict::InsertData(const GData* data) throw(bad_alloc, GException)
{
	GData* ptr;
	bool InDirect=false;
	unsigned int Id;

	// Empty datas are not inserted
	if(data->IsEmpty())
		throw GException("Empty datas cannot be inserted into a dictionary");
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
		Id=Session->GetDicNextId(ptr,this);
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


//---------------------------------------------------------------------------
void GDict::DeleteData(GData* data) throw(bad_alloc, GException)
{
	if((!data)||(data->GetId()>MaxId))
		throw GException("Cannot delete element");
	Direct[data->GetId()]=0;
	DataTypes.GetPtr<GInfoType>(data->GetType())->DeletePtr(data);
	DeletePtr(data);
}


//---------------------------------------------------------------------------
GData* GDict::GetData(const unsigned int id) const
{
	return(Direct[id]);
}


//---------------------------------------------------------------------------
GDataCursor& GDict::GetDataCursor(GInfoType type) throw(bad_alloc,GException)
{
	GDataCursor* cur=GDataCursor::GetTmpCursor();
	GDataTypes* tp;

	tp=DataTypes.GetPtr<GInfoType>(type);
	if(!tp)
		throw GException("No cursor for unknown information type");
	cur->Set(tp);
	return(*cur);
}


//---------------------------------------------------------------------------
unsigned int GDict::GetNbDatas(GInfoType type) const throw(GException)
{
	GDataTypes* tp;

	tp=DataTypes.GetPtr<GInfoType>(type);
	if(!tp)
		throw GException("No cursor for unknown information type");
	return(tp->NbPtr);
}


//---------------------------------------------------------------------------
bool GDict::IsIn(const RString& name2) const
{
	RString name(name2);
	return(RDblHashContainer<GData,unsigned,27,27,true>::IsIn<RString&>(name));
}


//---------------------------------------------------------------------------
/*unsigned int GDict::GetNbGroupsList(void) const
{
	return(GroupsList.NbPtr);
}
*/

//---------------------------------------------------------------------------
bool GDict::IsStopList(void) const
{
	return(Stop);
}


//---------------------------------------------------------------------------
int GDict::Compare(const GDict* dict) const
{
	return(Name.Compare(dict->Name));
}


//---------------------------------------------------------------------------
int GDict::Compare(const GDict& dict) const
{
	return(Name.Compare(dict.Name));
}


//---------------------------------------------------------------------------
int GDict::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
void GDict::IncRef(unsigned int id,tObjType ObjType,GInfoType /*WordType*/)
{
	Direct[id]->IncRef(ObjType);
}


//-----------------------------------------------------------------------------
void GDict::DecRef(unsigned int id,tObjType ObjType,GInfoType /*WordType*/)
{
	Direct[id]->DecRef(ObjType);
}


//-----------------------------------------------------------------------------
void GDict::IncRef(tObjType ObjType,GInfoType WordType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs[WordType]++;
			break;
		case otSubProfile:
			NbRefSubProfiles[WordType]++;
			break;
		case otGroup:
			NbRefGroups[WordType]++;
			break;
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void GDict::DecRef(tObjType ObjType,GInfoType WordType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs[WordType]--;
			break;
		case otSubProfile:
			NbRefSubProfiles[WordType]--;
			break;
		case otGroup:
			NbRefGroups[WordType]--;
			break;
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
unsigned int GDict::GetRef(unsigned int id,tObjType ObjType)
{
	return(Direct[id]->GetRef(ObjType));
}


//-----------------------------------------------------------------------------
unsigned int GDict::GetRef(tObjType ObjType,GInfoType WordType)
{
	switch(ObjType)
	{
		case otDoc:
			return(NbRefDocs[WordType]);
			break;
		case otSubProfile:
			return(NbRefSubProfiles[WordType]);
			break;
		case otGroup:
			return(NbRefGroups[WordType]);
			break;
		default:
			return(NbRefDocs[WordType]+NbRefSubProfiles[WordType]+NbRefGroups[WordType]);
			break;
	}
	return(0);
}


//---------------------------------------------------------------------------
GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
