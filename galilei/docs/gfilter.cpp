/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Implementation.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfilter.h>
#include <docs/gfiltermanager.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilter::GFilter(GFactoryFilter* fac) throw(bad_alloc)
	: GPlugin<GFactoryFilter>(fac), Doc(0)
{
}


// //---------------------------------------------------------------------------
void GFilter::AddMIME(const char* name) throw(bad_alloc)
{
	GetFactory()->GetMng()->AddMIME(name,this);
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeBlock(char* block,RXMLTag* attach) throw(bad_alloc,GException)
{
	char* ptr;
	char* hold;
	RXMLTag* sent;
	int len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	len=strlen(block);
	if(!len) return;

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

		// Seach sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctation and the next
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
						memcpy(hold,ptr,(len+1)*sizeof(char));
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
				// Skip the punctation
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
				attach->AddTag(sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeBlock(RChar* block,RXMLTag* attach) throw(bad_alloc,GException)
{
	RChar* ptr;
	RChar* hold;
	RXMLTag* sent;
	int len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	len=RChar::StrLen(block);
	if(!len) return;

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

		// Seach sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctation and the next
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
				// Skip the punctation
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
				attach->AddTag(sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeKeywords(char* list,char sep,RXMLTag* attach) throw(bad_alloc,GException)
{
	char* ptr;
	RXMLTag* kwd;
	unsigned int len;

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
			attach->AddTag(kwd=new RXMLTag("docxml:keyword"));
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
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeKeywords(RChar* list,RChar sep,RXMLTag* attach) throw(bad_alloc,GException)
{
	RChar* ptr;
	RXMLTag* kwd;
	unsigned int len;

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
			attach->AddTag(kwd=new RXMLTag("docxml:keyword"));
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
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
