/*

	GALILEI Research Project

	GIndexer.cpp

	Dictionary - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#include <gindexer.h>
#include <gwordoccurs.h>
#include <glang.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIndexer::GIndexer(unsigned ml,GLangManager* langs)
	: RDblHashContainer<GWordOccurs,true>(27,27,ml+(ml/4),ml/4), Langs(langs)
{
}


//------------------------------------------------------------------------------
void GIndexer::Clear(void)
{
	RDblHashContainer<GWordOccurs,true>::Clear();
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
		DeletePtr(*ptr);
}


//------------------------------------------------------------------------------
bool GIndexer::IsIndexed(const RString& word) const
{
	return(RDblHashContainer<GWordOccurs,true>::IsIn<const RString>(word));
}


//------------------------------------------------------------------------------
GWordOccurs* GIndexer::GetWord(const RString& word) const
{
	return(RDblHashContainer<GWordOccurs,true>::GetPtr<const RString>(word));
}


//------------------------------------------------------------------------------
class Stem
{
public:
	RContainer<RString,true,true> Forms;
	RString Name;
	Stem(const RString& name) : Forms(10,10),Name(name) {}
	int Compare(const Stem& ptr) const {return(Name.Compare(ptr.Name));}
	int Compare(const RString& name) const {return(Name.Compare(name));}
};

//------------------------------------------------------------------------------
void GIndexer::RunQuery(R::RString query,RVectorInt<unsigned int,true>& docs) const
{

	const RChar* ptr;
	unsigned int pos;
	unsigned int len;
	RString stem,Word;
	RContainer<Stem,true,true> Stems(30,15);
	GWordOccurs* word;
	Stem* New;
	bool Found;

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
		RCursor<GFactoryLang> Cur=Langs->GetFactories();
		Word=query.Mid(pos,len);
		for(Cur.Start(),New=0;!Cur.End();Cur.Next())
		{
			if(!Cur()->GetPlugin())
				continue;
			if(!New)
				New=Stems.GetInsertPtr<RString>(Word);
			stem=Cur()->GetPlugin()->GetStemming(Word);
			New->Forms.GetInsertPtr<RString>(stem);
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
	RCursor<Stem> Cur(Stems);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<RString> Cur2(Cur()->Forms);
		for(Cur2.Start(),Found=false;!Cur2.End();Cur2.Next())
		{
			word=GetPtr<RString>(*Cur2());
			if(word)
			{
				word->FilterDocs(docs);
				Found=true;
			}
		}
		if(!Found)
		{
			// A word was not found -> return empty container
			docs.Clear();
			break;
		}
	}
}


//------------------------------------------------------------------------------
GIndexer::~GIndexer(void)
{
}
