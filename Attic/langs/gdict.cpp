/*

	GALILEI Research Project

	GDic.cpp

	Dictionnary - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//---------------------------------------------------------------------------
// include file for GALILEI
#include <glangs/gdict.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GDict::GDict(GSession* session,const RString& name,const RString& desc,GLang *lang,unsigned max,unsigned maxletter) throw(bad_alloc)
  : RHashContainer<GWord,unsigned,27,true>(maxletter+(maxletter/4),maxletter/4), Session(session), Direct(0),
    MaxId(max+max/4), UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false)
{
	Direct=new GWord*[MaxId];
	memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
void GALILEI::GDict::Clear(void)
{
	RHashContainer<GWord,unsigned,27,true>::Clear();
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
unsigned GALILEI::GDict::GetId(const RString& word) throw(bad_alloc)
{
	GWord Word(word),*ptr;

	ptr=GetInsertPtr(Word);
	if(ptr->Id == cNoRef)
	{
		ptr->Id=Session->DicNextId(word);
		PutDirect(ptr);
	}
	return(ptr->Id);
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


//---------------------------------------------------------------------------
GALILEI::GDict::~GDict(void)
{
	if(Direct) delete[] Direct;
}
