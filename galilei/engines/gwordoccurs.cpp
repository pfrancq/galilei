/*

	GALILEI Research Project

	GWordOccurs.cpp

	Word Occurences - Implementation.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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
#include <ginfo.h>
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
  : Word(word), Docs(50,50)
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
	Docs.InsertPtr(new GInfo(docid,1)); // 1==stem -> is fixed
}


//-----------------------------------------------------------------------------
void GWordOccurs::FilterDocs(R::RContainer<GInfo,true,false>& docs) const
{
	RContainer<GInfo,false,false> Rem(20,10);

	if(docs.GetNb())
	{
		// Remove all documents that are not in Docs
		RCursor<GInfo> Cur(docs);
/*		for(Cur.Start();!Cur.End();)
		{
			if(Docs.IsIn(Cur()))
				Cur.Next();
			else
				docs.DeletePtr(Cur());
		}*/
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Docs.IsIn(*Cur()))
				Rem.InsertPtr(Cur());
		}
		Cur.Set(Rem);
		for(Cur.Start();!Cur.End();Cur.Next())
			docs.DeletePtr(*Cur());
	}
	else
	{
		// Insert all the documents of Docs in docs.
		RCursor<GInfo> Cur(Docs);
		for(Cur.Start();!Cur.End();Cur.Next())
			docs.InsertPtr(new GInfo(*Cur()));
	}
}


//-----------------------------------------------------------------------------
GWordOccurs::~GWordOccurs(void)
{
}


