/*

	GALILEI Research Project

	GIndexer.cpp

	Dictionary - Implementation.

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



//------------------------------------------------------------------------------
// include file for GALILEI
#include <engines/gindexer.h>
#include <engines/gwordoccurs.h>
#include <infos/glangmanager.h>
#include <infos/glang.h>
#include <infos/ginfo.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIndexer::GIndexer(unsigned ml,GLangManager* langs)
	: RDblHashContainer<GWordOccurs,27,27,true>(ml+(ml/4),ml/4), Langs(langs)
{
}


//------------------------------------------------------------------------------
void GIndexer::Clear(void)
{
	RDblHashContainer<GWordOccurs,27,27,true>::Clear();
}


//------------------------------------------------------------------------------
void GIndexer::InsertWord(R::RString word)
{
	GWordOccurs* ptr;

	// Look if the word exists. If not, create and insert it.
	ptr=GetPtr<const RString>(word);
	if(!ptr)
		InsertPtr(new GWordOccurs(word));
}


//------------------------------------------------------------------------------
void GIndexer::DeleteData(R::RString word)
{
	GWordOccurs* ptr;

	// Look if the word exists. If yes, delete it.
	ptr=GetPtr<const RString>(word);
	if(ptr)
		DeletePtr(ptr);
}


//------------------------------------------------------------------------------
bool GIndexer::IsIndexed(const RString& word) const
{
	return(RDblHashContainer<GWordOccurs,27,27,true>::IsIn<const RString>(word));
}


//------------------------------------------------------------------------------
GWordOccurs* GIndexer::GetWord(const RString& word) const
{
	return(RDblHashContainer<GWordOccurs,27,27,true>::GetPtr<const RString>(word));
}


//------------------------------------------------------------------------------
void GIndexer::RunQuery(R::RString query,R::RContainer<GInfo,true,false>& docs) const
{
	const RChar* ptr;
	unsigned int pos;
	unsigned int len;
	RString stem;
	RContainer<RString,true,true> Stems(30,15);
	GWordOccurs* word;

	// Init
	docs.Clear();

	// Parse the query to find all the words to search for
	for(ptr=query(),pos=0,len=0;!ptr->IsNull();)
	{
		// Read a word
		while((!ptr->IsNull())&&(!ptr->IsSpace()))
		{
			ptr++;
			len++;
		}

		// For each existing language, insert the corresponding stem
		RCursor<GFactoryLang> Cur=Langs->GetLangsCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(!Cur()->GetPlugin())
				continue;
			stem=Cur()->GetPlugin()->GetStemming(query.Mid(pos,len));
			Stems.GetInsertPtr<RString>(stem);
		}
		pos+=len;
		len=0;

		// Skip spaces
		while((!ptr->IsNull())&&(ptr->IsSpace()))
		{
			ptr++;
			pos++;
		}
	}


	// Go trough each stems to look for the words.
	RCursor<RString> Cur(Stems);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		word=GetPtr<RString>(Cur());
/*		if(!word)
		{
			// a word Was not found -> return empty container
			docs.Clear();
			break;
		}*/
		if(word)
			word->FilterDocs(docs);
	}
}


//------------------------------------------------------------------------------
GIndexer::~GIndexer(void)
{
}
