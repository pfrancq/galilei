/*

	GALILEI Research Project

	GWord.cpp

	Word - Implementation.

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



//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include file for Galilei
#include <langs/gword.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GWord
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(void) throw(bad_alloc)
  : Id(cNoRef), Word(), NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const RString& word) throw(bad_alloc)
  : Id(cNoRef), Word(word), NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(unsigned id,const RString& word) throw(bad_alloc)
  : Id(id), Word(word), NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GWord::GWord(const GWord& word) throw(bad_alloc)
  : Id(word.Id), Word(word.Word), NbRefDocs(0), NbRefSubProfiles(0), NbRefGroups(0)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const GWord& word) const
{
	return(Word.Compare(word.Word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const GWord* word) const
{
	return(Word.Compare(word->Word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const RString& word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const char* word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
int GALILEI::GWord::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
void GALILEI::GWord::IncRef(tObjType ObjType)
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
}


//-----------------------------------------------------------------------------
void GALILEI::GWord::DecRef(tObjType ObjType)
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
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GWord::GetRef(tObjType ObjType)
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
