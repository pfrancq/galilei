/*

	GALILEI Research Project

	GWord.cpp

	Word (or stem) - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include file for Galilei
#include <gword.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GWord
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GWord::GWord(void)
  : GData(cNoRef,"",infoWord,0,0,0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const RString& word)
  : GData(cNoRef,word,infoWord,0,0,0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(unsigned id,const RString& word,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
  : GData(id,word,infoWord,refdocs,refsubprofiles,refgroups)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const GWord& word)
  : GData(word)
{
}


//-----------------------------------------------------------------------------
GData* GWord::CreateCopy(void) const
{
	GWord* ptr=new GWord(*this);
	return(ptr);
}


//-----------------------------------------------------------------------------
GALILEI::GWord::~GWord(void)
{
}
