/*

	GALILEI Research Project

	GFilter.cpp

	Generic Filter for documents - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
// include files for GALILEI
#include <ggalileiapp.h>
#include <gdocxml.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilter::GFilter(GPluginFactory* fac)
	: GPlugin(fac), Doc(0)
{
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(const char* name)
{
	GALILEIApp->AddMIME(name,this);
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(RString name)
{
	GALILEIApp->AddMIME(name,this);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(char* block,RXMLTag* attach)
{
	char* ptr;
	char* hold;
	RXMLTag* sent=0;
	size_t len(strlen(block)),i(0);
	int NbWords;
	bool EndSentence;

	// Look at block
	if(!len) return(attach);

	// Search Sentences
	ptr=block;
	while(*ptr)
	{
		// Skip leading spaces.
		while((*ptr)&&(isspace(*ptr)))
		{
			ptr++;
			len--;
		}
		block=ptr;

		// Search sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctuation and the next
		// word, no sentence created.
		while((*ptr)&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			while((*ptr)&&(!IsEndSentence(*ptr)))
			{
				// If the next characters are spaces, replace them all by a
				// unique space.
				if(isspace(*ptr))
				{
					(*(ptr++))=' ';    // Space character always a ' '.
					i=(--len);
					hold=ptr;
					while((*ptr)&&(isspace(*ptr)))
					{
						len--;
						ptr++;
					}
					if(i>len)      // Look if there more than one space character.
					{
						memmove(hold,ptr,(len+1)*sizeof(char));
						ptr=hold;
					}
					NbWords++;
				}
				else
				{
					ptr++;
					len--;
				}
			}
			// Verify if it is a correct end of sentence
			if((*ptr)&&((NbWords<1)||(!isspace(*(ptr+1)))))
			{
				// Skip the punctuation
				ptr++;
				len--;
			}
			else
				EndSentence=true;      // End of sentence.
		}
		if(*ptr)       // Skip Punctuation.
		{
			len--;
			(*(ptr++))=0;
		}

		// Insert sentence
		if(*block)
		{
			// Verify that it is not a single '.'
			if((!ispunct(*block))||(*(block+1)))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(RChar* block,RXMLTag* attach)
{
	RChar* ptr;
	RChar* hold;
	RXMLTag* sent=0;
	size_t len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	len=RChar::StrLen(block);
	if(!len) return(attach);

	// Search Sentences
	ptr=block;
	while(!ptr->IsNull())
	{
		// Skip leading spaces.
		while((!ptr->IsNull())&&(ptr->IsSpace()))
		{
			ptr++;
			len--;
		}
		block=ptr;

		// Search sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctuation and the next
		// word, no sentence created.
		while((!ptr->IsNull())&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			//while((!ptr->IsNull())&&(!ptr->IsPunct()))
			while((!ptr->IsNull())&&(!IsEndSentence(*ptr)))
			{
				// If the next characters are spaces, replace them all by a
				// unique space.
				if(ptr->IsSpace())
				{
					(*(ptr++))=RChar(' ');    // Space character always a ' '.
					i=(--len);
					hold=ptr;
					while((!ptr->IsNull())&&(ptr->IsSpace()))
					{
						len--;
						ptr++;
					}
					if(i>len)      // Look if there more than one space character.
					{
						memcpy(hold,ptr,(len+1)*sizeof(RChar));
						ptr=hold;
					}
					NbWords++;
				}
				else
				{
					ptr++;
					len--;
				}
			}
			// Verify if it is a correct end of sentence
			if((!ptr->IsNull())&&((NbWords<1)||(!(ptr+1)->IsSpace())))
			{
				// Skip the punctuation
				ptr++;
				len--;
			}
			else
				EndSentence=true;      // End of sentence.
		}
		if(!ptr->IsNull())       // Skip Punctuation.
		{
			len--;
			(*(ptr++))=0;
		}

		// Insert sentence
		if(!block->IsNull())
		{
			// Verify that it is not a single '.'
			if((!block->IsPunct())||((block+1)->IsNull()))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(const RString& block,RXMLTag* attach)
{
	RXMLTag* sent=0;
	size_t pos;
	int NbWords;
	bool EndSentence;
	RString sentence;
	RCharCursor Cur(block);

	// Look at block
	if(!block.GetLen()) return(attach);

	// Search Sentences
	while(!Cur.End())
	{
		// Skip leading spaces.
		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();

		// Search sentence
		NbWords=0;
		EndSentence=false;
		pos=Cur.GetPos();

		// If Only 1 word or no space between a punctuation and the next
		// word, no sentence created.
		while((!Cur.End())&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			while((!Cur.End())&&(!IsEndSentence(Cur())))
			{
				// If the next characters are spaces, it is a word
				if(Cur().IsSpace())
				{
					// Add the word
					if(!sentence.IsEmpty())
						sentence+=' ';
					sentence+=block.Mid(pos,Cur.GetPos()-pos);

					// Next word can be read
					while((!Cur.End())&&(Cur().IsSpace()))
						Cur.Next();
					NbWords++;
					pos=Cur.GetPos();
				}
				else
					Cur.Next();
			}

			// Verify if it is a correct end of sentence
			if((!Cur.End())&&(Cur.GetPos()+1<Cur.GetNb())&&(!Cur[Cur.GetPos()+1].IsNull())&&/*((NbWords<1)||*/ (!Cur[Cur.GetPos()+1].IsSpace()))
				Cur.Next(); // Skip the punctuation
			else
			{
				EndSentence=true;      // End of sentence.
				if(!sentence.IsEmpty())
					sentence+=' ';
				sentence+=block.Mid(pos,Cur.GetPos()-pos);
				pos=Cur.GetPos();
			}
		}

		if(!Cur.End())
		{
			Cur.Next();
			pos=Cur.GetPos();
		}

		// Insert sentence
		if(!sentence.IsEmpty())
		{
			// Verify that it is not a single '.'
			if((!sentence[static_cast<size_t>(0)].IsPunct())||(sentence.GetLen()>1))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(sentence);
			}
			sentence.Clear();
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(char* list,char sep,RXMLTag* attach)
{
	char* ptr;
	RXMLTag* kwd=0;
	size_t len;

	ptr=list;
	while(*ptr)
	{
		// Skip Spaces.
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
		list=ptr;

		// Search the next keywords.
		len=0;
		while((*ptr)&&(!isspace(*ptr))&&((*ptr)!=sep))
		{
			ptr++;
			len++;
		}
		if(len)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			if(*ptr)
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("docxml:value",list);
		}
		else
		{
			ptr++;
			list=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(RChar* list,RChar sep,RXMLTag* attach)
{
	RChar* ptr;
	RXMLTag* kwd=0;
	size_t len;

	ptr=list;
	while(!ptr->IsNull())
	{
		// Skip Spaces.
		while((!ptr->IsNull())&&(ptr->IsSpace()))
			ptr++;
		list=ptr;

		// Search the next keywords.
		len=0;
		while((!ptr->IsNull())&&(ptr->IsSpace())&&((*ptr)!=sep))
		{
			ptr++;
			len++;
		}
		if(len)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			if(!ptr->IsNull())
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("docxml:value",list);
		}
		else
		{
			ptr++;
			list=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(const RString& list,RChar sep,RXMLTag* attach)
{
	RCharCursor Cur;
	RXMLTag* kwd;
	size_t pos;

	Cur.Set(list);
	while(!Cur.End())
	{
		// Skip Spaces.
		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();

		// Search the next keywords.
		pos=Cur.GetPos();
		while((!Cur.End())&&(!Cur().IsSpace())&&(Cur()!=sep))
			Cur.Next();
		if(Cur.GetPos()-pos)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			kwd->InsertAttr("docxml:value",list.Mid(pos,Cur.GetPos()-pos));
		}
		else
		{
			if(!Cur.End())
				Cur.Next();
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
