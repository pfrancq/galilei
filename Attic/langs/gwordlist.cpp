/*

  gwordlist.cpp

  Description - Implementation.

  (c) 2002 by N. Kumps (nkumps@ulb.ac.be)

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include file for Galilei
#include <langs/gdict.h>
#include <langs/gword.h>
#include <langs/gwordlist.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GWordList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWordList::GWordList(void)
	: GWord()
{
	List=new RContainer<GWord,unsigned,false,true>(2);
	Type=tWordList;
}


//-----------------------------------------------------------------------------
GALILEI::GWordList::GWordList(unsigned int word)
	: GWord(itoa(word))
{
	List=new RContainer<GWord,unsigned,false,true>(2);
	Type=tWordList;
}


//-----------------------------------------------------------------------------
GALILEI::GWordList::GWordList(unsigned int id,const RStd::RString& word)
	: GWord(id,word)
{
	List=new RContainer<GWord,unsigned,false,true>(2);
	Type=tWordList;
//	WordsId[0]=cNoRef;
//	WordsId[1]=cNoRef;
//	Type=tWordList;
//	Id=id;
//	Word=word;
//	RString tmp;
//	unsigned int len=0;
//	unsigned int i=0;
//	bool end=false;
//	char* begin;
//	char* ptr=word.StrDup();
//	begin=ptr;
//	while((*ptr)&& !(end))
//	{
//		if((strchr("&",*ptr)))
//		{
//			tmp.Copy(begin,len);
//			end=true;
//			WordsId[i++]=dic->GetId(tmp);
//		}
//		ptr++;
//		len++;
//	}
//	WordsId[i]=dic->GetId(ptr);


}
//-----------------------------------------------------------------------------
GALILEI::GWordList::GWordList(GWordList* wordlist)
	: GWord(wordlist->GetId(),wordlist->GetWord())
{
	List=new RContainer<GWord,unsigned,false,true>(2);
	Type=tWordList;
}


//-----------------------------------------------------------------------------
void GALILEI::GWordList::InsertWord(GWord* word)
{
	List->InsertPtr(word);
}


//-----------------------------------------------------------------------------
void GALILEI::GWordList::SetType(GWordType type)
{
	Type=type;
}


//-----------------------------------------------------------------------------
void GALILEI::GWordList::SetId(unsigned int id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
GALILEI::GWordList::~GWordList(void)
{
}
