/*

	GALILEI Research Project

	gwordlist.cpp

	Representation of a list of words - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

	Authors:
		Nicolas Kumps (nkumps@ulb.ac.be).

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
#include <infos/gwordlist.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
//
// class GWordList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GWordList::GWordList(void)
	: GWord(), List(2)
{
//	List=new RContainer<GWord,unsigned,false,true>(2);
//	Type=infoWordList;
}


//-----------------------------------------------------------------------------
GWordList::GWordList(unsigned int word)
	: GWord(itou(word)), List(2)
{
//	List=new RContainer<GWord,unsigned,false,true>(2);
//	Type=infoWordList;
}


//-----------------------------------------------------------------------------
GWordList::GWordList(unsigned int id,const RString& word)
	: GWord(id,word), List(2)
{
//	List=new RContainer<GWord,unsigned,false,true>(2);
//	Type=infoWordList;
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
GWordList::GWordList(GWordList* wordlist)
	: GWord(wordlist->GetId(),wordlist->GetWord()), List(2)
{
//	List=new RContainer<GWord,unsigned,false,true>(2);
//	Type=infoWordList;
}


//-----------------------------------------------------------------------------
void GWordList::InsertWord(GWord* word)
{
	List.InsertPtr(word);
}


//-----------------------------------------------------------------------------
/*void GWordList::SetType(GInfoType type)
{
	Type=type;
}*/


//-----------------------------------------------------------------------------
GWordCursor& GWordList::GetWordCursor(void)
{
	GWordCursor *cur=GWordCursor::GetTmpCursor();
	cur->Set(List);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GWordList::SetId(unsigned int id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
GWordList::~GWordList(void)
{
}
