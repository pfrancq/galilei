/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
GALILEI::GFilter::GFilter(const RString& url,GURLManager* mng)
	: Manager(mng),URL(url), Doc(0)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GFilter::AddExt(const RString& ext)
{
	Manager->AddMIME(ext,this);
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
	ptr=block;
	len=strlen(block);

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
			attach->AddTag(sent=new RXMLTag("Sentence"));
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
			attach->AddTag(kwd=new RXMLTag("Keyword"));
			if(*ptr)
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("Value",list);
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GFilter::~GFilter(void)
{
}
