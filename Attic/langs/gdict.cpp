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
#include <langs/gdict.h>
#include <langs/gword.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GDict::GDict(GSession* s,const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st) throw(bad_alloc)
	: RDblHashContainer<GWord,unsigned,27,27,true>(ml+(ml/4),ml/4), Session(s), Direct(0),
	  MaxId(m+m/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false), Stop(st),
	  NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0)
{
	Direct=new GWord*[MaxId];
	memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
void GALILEI::GDict::Clear(void)
{
	RDblHashContainer<GWord,unsigned,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GWord*));
	UsedId=0;
	Loaded=false;
}


//---------------------------------------------------------------------------
void GALILEI::GDict::PutDirect(GWord* word) throw(bad_alloc)
{
	GWord **tmp;
	unsigned n;

	if(word->Id>UsedId) UsedId=word->Id;
	if(word->Id>=MaxId)
	{
		n=word->Id+1000;
		tmp=new GWord*[n];
		memcpy(tmp,Direct,MaxId*sizeof(GWord*));
		delete[] Direct;
		Direct=tmp;
		MaxId=n;
	}
	Direct[word->Id]=word;
}


//---------------------------------------------------------------------------
void GALILEI::GDict::Put(unsigned id,const RStd::RString& word) throw(bad_alloc)
{
	GWord Word(id,word),*ptr;
	ptr=GetInsertPtr<GWord>(Word,true);
	PutDirect(ptr);
}


//---------------------------------------------------------------------------
unsigned GALILEI::GDict::GetId(const RString& word) throw(bad_alloc)
{
	GWord Word(word),*ptr;

	ptr=GetInsertPtr(Word);
	if(ptr->Id == cNoRef)
	{
		ptr->Id=Session->GetDicNextId(word,this);
		PutDirect(ptr);
	}
	return(ptr->Id);
}


//---------------------------------------------------------------------------
const char* GALILEI::GDict::GetWord(const unsigned int id) const
{
	return(Direct[id]->GetWord());
}


//---------------------------------------------------------------------------
GWord* GALILEI::GDict::GetElement(const unsigned int id) const
{
	return(Direct[id]);
}


//---------------------------------------------------------------------------
int GALILEI::GDict::Compare(const GDict* dict) const
{
	return(Name.Compare(dict->Name));
}


//---------------------------------------------------------------------------
int GALILEI::GDict::Compare(const GDict& dict) const
{
	return(Name.Compare(dict.Name));
}


//---------------------------------------------------------------------------
int GALILEI::GDict::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GDict::IncRef(unsigned int id,tObjType ObjType)
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
	Direct[id]->IncRef(ObjType);
}


//-----------------------------------------------------------------------------
void GALILEI::GDict::DecRef(unsigned int id,tObjType ObjType)
{
	switch(ObjType)
	{
		case otDoc:
			NbRefDocs--;
			break;
		case otSubProfile:
			NbRefSubProfiles--;
			break;
		case otGroup:
			NbRefGroups--;
			break;
		default:
			break;
	}
	Direct[id]->DecRef(ObjType);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GDict::GetRef(unsigned int id,tObjType ObjType)
{
	return(Direct[id]->GetRef(ObjType));
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GDict::GetRef(tObjType ObjType)
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


//---------------------------------------------------------------------------
GALILEI::GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
