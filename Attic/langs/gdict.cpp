/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

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
// include file for galilei
#include <gsessions/gsession.h>
#include <glangs/gdict.h>
#include <glangs/gword.h>
#include <glangs/glang.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GDict
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GDict::GDict(GSession* _owner,const RString& name,GLang *lang,unsigned max,unsigned maxletter) throw(bad_alloc)
  : RHashContainer<GWord,unsigned,27,true>(maxletter+(maxletter/4),maxletter/4), Direct(0), owner(_owner),
    MaxId(max+max/4), UsedId(0),Lang(lang), Name(name), Desc(""), Loaded(false)
{
  Direct=new GWord*[MaxId];
  memset(Direct,0,MaxId*sizeof(GWord*));
}


//---------------------------------------------------------------------------
GDict::GDict(GSession* _owner,const RString& name,const RString& desc,GLang* lang) throw(bad_alloc)
  : RHashContainer<GWord,unsigned,27,true>(1000,1000), Direct(0),owner(_owner), MaxId(10000),
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

  if(ptr->Id == cNoRef)
  {
      ptr->Id=owner->DicNextId(word);
      PutDirect(ptr);
  }
  return(ptr->Id);
}


//---------------------------------------------------------------------------
GDict::~GDict(void)
{
  if(Direct) delete[] Direct;
}



