/*

	GALILEI Research Project

	GWordOccurs.cpp

	Word Occurences - Implementation.

	Copyright 2004-2007 by the Université ibre de Bruxelles.

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
#include <gconcept.h>
#include <gwordoccurs.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GWordOccurs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GWordOccurs::GWordOccurs(R::RString word)
  : Word(word), Docs(50)
{
}


//-----------------------------------------------------------------------------
int GWordOccurs::Compare(const GWordOccurs& word) const
{
	return(Word.Compare(word.Word));
}


//-----------------------------------------------------------------------------
int GWordOccurs::Compare(const GWordOccurs* word) const
{
	return(Word.Compare(word->Word));
}


//-----------------------------------------------------------------------------
int GWordOccurs::Compare(const RString& word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
int GWordOccurs::Compare(const char* word) const
{
	return(Word.Compare(word));
}


//-----------------------------------------------------------------------------
void GWordOccurs::AddDoc(unsigned int docid)
{
	Docs.Insert(docid);
}


//-----------------------------------------------------------------------------
void GWordOccurs::FilterDocs(R::RVectorInt<true>& docs) const
{
	RVectorInt<true>& Cur=const_cast<RVectorInt<true>&>(Docs);
	
	if(docs.GetNbInt())
	{
		// Remove all documents that are not in Docs	
		for(Cur.Start();!Cur.End();Cur.Next())
			if(docs.IsIn(Docs()))
				docs.Delete(Docs());
	}
	else
	{
		// Insert all the documents of Docs in docs.
		for(Cur.Start();!Cur.End();Cur.Next())
			docs.Insert(Docs());
	}
}


//-----------------------------------------------------------------------------
GWordOccurs::~GWordOccurs(void)
{
}


