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
#include <langs/gwordlist.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDict::GDict(GSession* s,const RString& name,const RString& desc,GLang *lang,unsigned m,unsigned ml,bool st) throw(bad_alloc)
	: RDblHashContainer<GWord,unsigned,27,27,true>(ml+(ml/4),ml/4), Session(s), Direct(0),
	  NbGroupsList(0), MaxId(m+m/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false), Stop(st),
	  GroupsList(500)
{
	for(unsigned int i=0;i<2;i++)
	{
		NbRefDocs[i]=0;
		NbRefSubProfiles[i]=0;
		NbRefGroups[i]=0;
	}
	Direct=new GWord*[MaxId];
	memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
void GDict::Clear(void)
{
	RDblHashContainer<GWord,unsigned,27,27,true>::Clear();
	memset(Direct,0,MaxId*sizeof(GWord*));
	UsedId=0;
	Loaded=false;
}


//---------------------------------------------------------------------------
void GDict::PutDirect(GWord* word) throw(bad_alloc)
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
void GDict::Put(unsigned id,const R::RString& word) throw(bad_alloc)
{
	GWordList* wordlist;
	GWord Word(id,word),*ptr;
	const RChar* tmp;
	const RChar* tmp2;
	RString grp("grouplist");
	int i;

	if(word.IsEmpty()) return;
	tmp=word();
	tmp2=grp();
	for(i=grp.GetLen()+1;(--i)&&(!tmp->IsNull());tmp++,tmp2++)
	{
		if((*tmp)!=(*tmp2)) break;
	}
	if(!i)
	{
		ptr=GetInsertPtr<GWord>(Word,true);
		ptr->SetType(tWordList);
		wordlist=new GWordList(id,word);
		wordlist->SetType(tWordList);
		GroupsList.InsertPtr(wordlist);
		NbGroupsList++;
	}
	else
		ptr=GetInsertPtr<GWord>(Word,true);
	PutDirect(ptr);
}

//---------------------------------------------------------------------------
void GDict::InsertNewWordList(GWordList& wordlist,bool save)
{
	GWord *ptr;
//	wordlist.Start();
//	ptr=wordlist();
//	RString temp(ptr->GetWord());
//	wordlist.Next();
//	ptr=wordlist();
//	temp+='&';
//	temp+='&';
//	temp+=ptr->GetWord();
//	GWord word(temp);

	ptr=GetInsertPtr(wordlist);
	if(ptr->Id == cNoRef)
	{
		if(save) ptr->Id=Session->GetDicNextId(wordlist.GetWord(),this);
		else ptr->Id=UsedId+1;
		ptr->SetType(tWordList);
		PutDirect(ptr);
		wordlist.SetId(ptr->Id);
		GroupsList.InsertPtr(&wordlist);
		NbGroupsList++;
	}

}

//---------------------------------------------------------------------------
unsigned GDict::GetId(const RString& word) throw(bad_alloc)
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
const char* GDict::GetWord(const unsigned int id) const
{
	return(Direct[id]->GetWord());
}


//---------------------------------------------------------------------------
GWord* GDict::GetElement(const unsigned int id) const
{
	return(Direct[id]);
}


//---------------------------------------------------------------------------
unsigned int GDict::GetNbGroupsList(void) const
{
	return(GroupsList.NbPtr);
}


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
void GDict::IncRef(unsigned int id,tObjType ObjType,GWordType /*WordType*/)
{
	Direct[id]->IncRef(ObjType);
}


//-----------------------------------------------------------------------------
void GDict::DecRef(unsigned int id,tObjType ObjType,GWordType /*WordType*/)
{
	Direct[id]->DecRef(ObjType);
}


//-----------------------------------------------------------------------------
void GDict::IncRef(tObjType ObjType,GWordType WordType)
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
void GDict::DecRef(tObjType ObjType,GWordType WordType)
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
unsigned int GDict::GetRef(tObjType ObjType,GWordType WordType)
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
