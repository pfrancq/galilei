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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilter.h>
#include <urlmanagers/gurlmanager.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GFilter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GFilter::GFilter(GURLManager*,const char* name,const char* mimes,const char* ver)
	: Doc(0), Name(name), MIMES(mimes), Version(strlen(ver))
{
	const char* ptr=ver;
	const char* begin;
	int len;

	if((*ptr)=='$')
	{
		// Version is in CVS format
		while(!isdigit(*ptr))
			ptr++;
		begin=ptr;
		len=0;
		while((*ptr)!=' ')
		{
			ptr++;
			len++;
		}
		Version.Copy(begin,len);
	}
	else
		Version=ver;
}


//-----------------------------------------------------------------------------
void GALILEI::GFilter::AddMIME(GURLManager* mng,const char* name)
{
	if(!mng) return;
	mng->AddMIME(name,this);
}


//-----------------------------------------------------------------------------
void GALILEI::GFilter::AnalyzeBlock(char* block,RXMLTag* attach)
{
	char* ptr;
	char* hold;
	RXMLTag* sent;
	int len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	ptr=block;
	len=strlen(block);
	if(!len) return;

	// Search Sentences
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
			attach->AddTag(sent=new RXMLTag("docxml:sentence"));
			//sent->InsertAttr("Value",block);
			sent->AddContent(block);
			block=ptr;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GFilter::AnalyzeKeywords(char* list,char sep,RXMLTag* attach)
{
	char* ptr;
	RXMLTag* kwd;

	ptr=list;
	while(*ptr)
	{
		// Skip Spaces.
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
		list=ptr;

		// Search the next keywords.
		while((*ptr)&&((*ptr)!=sep))
			ptr++;
		if(list!=ptr)
		{
			attach->AddTag(kwd=new RXMLTag("docxml:keyword"));
			if(*ptr)
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("docxml:value",list);
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GFilter::~GFilter(void)
{
}
