

//---------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>

//---------------------------------------------------------------------------
// include file for HyperPRISME
#include "Gdict.h"
using namespace HyperPRISME;



//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDict::GDict(const RString& name,GLang *lang,unsigned max,unsigned maxletter) throw(bad_alloc)
  : RHashContainer<GWord,unsigned,27,true>(maxletter+(maxletter/4),maxletter/4), Direct(0),
    MaxId(max+max/4), UsedId(0),Lang(lang), Name(name), Desc(""), Loaded(false)
{
  Direct=new GWord*[MaxId];
  memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
GDict::GDict(const RString& name,const RString& desc,GLang* lang) throw(bad_alloc)
  : RHashContainer<GWord,unsigned,27,true>(1000,1000), Direct(0), MaxId(10000),
    UsedId(0),Lang(lang), Name(name), Desc(desc), Loaded(false)
{
  Direct=new GWord*[MaxId];
  memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
void GDict::Clear(void)
{
  RHashContainer<GWord,unsigned,27,true>::Clear();
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
unsigned GDict::GetId(const RString& word) throw(bad_alloc)
{
  GWord Word(word),*ptr;

  ptr=GetInsertPtr(Word);
  if(ptr->Id==cNoRef)
  {
      ptr->Id=NextId(word);
      PutDirect(ptr);
  }
  return(ptr->Id);
}


//---------------------------------------------------------------------------
GDict::~GDict(void)
{
  if(Direct) delete[] Direct;
}



