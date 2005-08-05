/*

	GALILEI Research Project

	GWordList.cpp

	List of words - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Nicolas Kumps (nkumps@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//------------------------------------------------------------------------------
// include file for Galilei
#include <gwordlist.h>
#include <gword.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GWordList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWordList::GWordList(unsigned int id,const RString& name,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
	: GData(id,name,infoWordList,refdocs,refsubprofiles,refgroups), RContainer<GWord,false,true>(2)
{
}


//------------------------------------------------------------------------------
GWordList::GWordList(const GWordList& list)
	: GData(list), RContainer<GWord,false,true>(list)
{
}


//------------------------------------------------------------------------------
void GWordList::InsertWord(const GWord* word)
{
	InsertPtr(word);
}


//------------------------------------------------------------------------------
R::RCursor<GWord> GWordList::GetWords(void) const
{
	R::RCursor<GWord> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
GData* GWordList::CreateCopy(void) const
{
	GWordList* ptr=new GWordList(*this);
	return(ptr);
}


//------------------------------------------------------------------------------
GWordList::~GWordList(void)
{
}
