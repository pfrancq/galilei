/*

	GALILEI Research Project

	GFilterHTML.cpp

	A HTML filter - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

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
#include <string.h>
#include <iostream.h> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>


//---------------------------------------------------------------------------
// include files for GALILEI
#include "gfilterhtml.h"
#include <filters/codetochar.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class Tag
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GALILEI::GFilterHTML::Tag
{
public:
	enum tTag{tNULL,tHTML,tSCRIPT,tHEAD,tTITLE,tMETA,tBODY,tH1,tH2,tH3,tH4,tH5,tH6,tP,tTD};
	RString Name;
	RString XMLName;
	tTag Type;
	bool Head;
	int Level;
	bool Ins;

	Tag(const char* n,const char* x,tTag t,bool h,int l,bool i)
		: Name(n), XMLName(x), Type(t), Head(h), Level(l),Ins(i) {}
	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
	int Compare(const char* t) const {return(Name.Compare(t));}
};



//---------------------------------------------------------------------------
//
// class GFilterHTML
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GFilterHTML::GFilterHTML(GURLManager* mng)
	: GFilter(mng,"HTML Filter","text/html","$Revision$"), Tags(0),
	 Buffer(0), Chars(50,5)
{
	AddMIME("text/html");
	InitCharContainer();
	Tags=new RContainer<Tag,unsigned int,true,true>(10,5);
	Tags->InsertPtr(new Tag("HEAD","",Tag::tHEAD,true,8,false));
	Tags->InsertPtr(new Tag("SCRIPT","",Tag::tSCRIPT,true,8,false));
	Tags->InsertPtr(new Tag("META","",Tag::tMETA,true,8,true));
	Tags->InsertPtr(new Tag("TITLE","Title",Tag::tTITLE,true,8,true));
	Tags->InsertPtr(new Tag("BODY","",Tag::tBODY,false,8,false));
	Tags->InsertPtr(new Tag("H1","H1",Tag::tH1,false,1,true));
	Tags->InsertPtr(new Tag("H2","H2",Tag::tH2,false,2,true));
	Tags->InsertPtr(new Tag("H3","H3",Tag::tH3,false,3,true));
	Tags->InsertPtr(new Tag("H4","H4",Tag::tH4,false,4,true));
	Tags->InsertPtr(new Tag("H5","H5",Tag::tH5,false,5,true));
	Tags->InsertPtr(new Tag("H6","H6",Tag::tH6,false,6,true));
	Tags->InsertPtr(new Tag("P","P",Tag::tP,false,7,true));
	Tags->InsertPtr(new Tag("TD","P",Tag::tTD,false,7,true));
	Tags->InsertPtr(new Tag("LI","P",Tag::tTD,false,7,true));
	Tags->InsertPtr(new Tag("DIV","P",Tag::tP,false,7,true));
}


//---------------------------------------------------------------------------
bool GALILEI::GFilterHTML::Analyze(GDocXML* doc)
{
	int accessmode,handle;
	struct stat statbuf;

	// Initialisation
	Doc=doc;
	accessmode=O_RDONLY;
	#ifndef _BSD_SOURCE
		accessmode=O_BINARY;
	#endif
	handle=open(Doc->GetFile(),accessmode);
	fstat(handle, &statbuf);
	Block=Pos=Buffer=new char[statbuf.st_size+1];
	TagLen=0;
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	close(handle);
	SkipSpaces();

	// Traitement of the document
	AnalyseHeader();
	AnalyseBody();

	// Done
	if(Block)
	{
		delete[] Buffer;
		Buffer=0;
	}
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::InitCharContainer(void)
{
	Chars.InsertPtr(new CodeToChar("Ucirc",'�'));
	Chars.InsertPtr(new CodeToChar("nbsp",' '));
	Chars.InsertPtr(new CodeToChar("#190",'�'));
	Chars.InsertPtr(new CodeToChar("Aacute",'�'));
	Chars.InsertPtr(new CodeToChar("Ugrave",'�'));
	Chars.InsertPtr(new CodeToChar("iacute",'�'));
	Chars.InsertPtr(new CodeToChar("nbsp",' '));
	Chars.InsertPtr(new CodeToChar("Acirc",'�'));
	Chars.InsertPtr(new CodeToChar("Uuml",'�'));
	Chars.InsertPtr(new CodeToChar("icirc",'�'));
	Chars.InsertPtr(new CodeToChar("Yacute",'�'));
	Chars.InsertPtr(new CodeToChar("#161",'�'));
	Chars.InsertPtr(new CodeToChar("Agrave",'�'));
	Chars.InsertPtr(new CodeToChar("aacute",'�'));
	Chars.InsertPtr(new CodeToChar("igrave",'�'));
	Chars.InsertPtr(new CodeToChar("Aring",'�'));
	Chars.InsertPtr(new CodeToChar("acirc",'�'));
	Chars.InsertPtr(new CodeToChar("igrave",'�'));
	Chars.InsertPtr(new CodeToChar("Atilde",'�'));
	Chars.InsertPtr(new CodeToChar("#180",'�'));
	Chars.InsertPtr(new CodeToChar("#191",'�'));
	Chars.InsertPtr(new CodeToChar("Auml",'�'));
	Chars.InsertPtr(new CodeToChar("aelig",'�'));
	Chars.InsertPtr(new CodeToChar("iuml",'�'));
	Chars.InsertPtr(new CodeToChar("Ccedil",'�'));
	Chars.InsertPtr(new CodeToChar("agrave",'�'));
	Chars.InsertPtr(new CodeToChar("#171",'�'));
	Chars.InsertPtr(new CodeToChar("ETH",'�'));
	Chars.InsertPtr(new CodeToChar("aring",'�'));
	Chars.InsertPtr(new CodeToChar("#175",'�'));
	Chars.InsertPtr(new CodeToChar("Eacute",'�'));
	Chars.InsertPtr(new CodeToChar("atilde",'�'));
	Chars.InsertPtr(new CodeToChar("#181",'�'));
	Chars.InsertPtr(new CodeToChar("Ecirc",'�'));
	Chars.InsertPtr(new CodeToChar("auml",'�'));
	Chars.InsertPtr(new CodeToChar("#183",'*'));
	Chars.InsertPtr(new CodeToChar("Egrave",'�'));
	Chars.InsertPtr(new CodeToChar("#166",'�'));
	Chars.InsertPtr(new CodeToChar("lt",'<'));
	Chars.InsertPtr(new CodeToChar("Euml",'�'));
	Chars.InsertPtr(new CodeToChar("ccedil",'�'));
	Chars.InsertPtr(new CodeToChar("gt",'>'));
	Chars.InsertPtr(new CodeToChar("Iacute",'�'));
	Chars.InsertPtr(new CodeToChar("#184",'�'));
	Chars.InsertPtr(new CodeToChar("ntilde",'�'));
	Chars.InsertPtr(new CodeToChar("Icirc",'�'));
	Chars.InsertPtr(new CodeToChar("oacute",'�'));
	Chars.InsertPtr(new CodeToChar("Igrave",'�'));
	Chars.InsertPtr(new CodeToChar("#169",'�'));
	Chars.InsertPtr(new CodeToChar("raquo",'�'));
	Chars.InsertPtr(new CodeToChar("laquo",'�'));
	Chars.InsertPtr(new CodeToChar("ocirc",'�'));
	Chars.InsertPtr(new CodeToChar("Iuml",'�'));
	Chars.InsertPtr(new CodeToChar("#164",'�'));
	Chars.InsertPtr(new CodeToChar("#172",'�'));
	Chars.InsertPtr(new CodeToChar("Ntilde",'�'));
	Chars.InsertPtr(new CodeToChar("#176",'�'));
	Chars.InsertPtr(new CodeToChar("#170",'�'));
	Chars.InsertPtr(new CodeToChar("Oacute",'�'));
	Chars.InsertPtr(new CodeToChar("#247",'�'));
	Chars.InsertPtr(new CodeToChar("#186",'�'));
	Chars.InsertPtr(new CodeToChar("Ocirc",'�'));
	Chars.InsertPtr(new CodeToChar("eacute",'�'));
	Chars.InsertPtr(new CodeToChar("oslash",'�'));
	Chars.InsertPtr(new CodeToChar("Ograve",'�'));
	Chars.InsertPtr(new CodeToChar("ecirc",'�'));
	Chars.InsertPtr(new CodeToChar("otilde",'�'));
	Chars.InsertPtr(new CodeToChar("Oslash",'�'));
	Chars.InsertPtr(new CodeToChar("egrave",'�'));
	Chars.InsertPtr(new CodeToChar("ouml",'�'));
	Chars.InsertPtr(new CodeToChar("Otilde",'�'));
	Chars.InsertPtr(new CodeToChar("eth",'�'));
	Chars.InsertPtr(new CodeToChar("#182",'�'));
	Chars.InsertPtr(new CodeToChar("Ouml",'�'));
	Chars.InsertPtr(new CodeToChar("euml",'�'));
	Chars.InsertPtr(new CodeToChar("#177",'�'));
	Chars.InsertPtr(new CodeToChar("THORN",'�'));
	Chars.InsertPtr(new CodeToChar("#189",'�'));
	Chars.InsertPtr(new CodeToChar("#163",'�'));
	Chars.InsertPtr(new CodeToChar("Uacute",'�'));
	Chars.InsertPtr(new CodeToChar("#188",'�'));
	Chars.InsertPtr(new CodeToChar("#187",'�'));
	Chars.InsertPtr(new CodeToChar("#174",'�'));
	Chars.InsertPtr(new CodeToChar("#185",'�'));
	Chars.InsertPtr(new CodeToChar("szlig",'�'));
	Chars.InsertPtr(new CodeToChar("#167",'�'));
	Chars.InsertPtr(new CodeToChar("#178",'�'));
	Chars.InsertPtr(new CodeToChar("thorn",'�'));
	Chars.InsertPtr(new CodeToChar("#173",'�'));
	Chars.InsertPtr(new CodeToChar("#179",'�'));
	Chars.InsertPtr(new CodeToChar("#215",'�'));
	Chars.InsertPtr(new CodeToChar("uacute",'�'));
	Chars.InsertPtr(new CodeToChar("quot",'�'));
	Chars.InsertPtr(new CodeToChar("#165",'�'));
	Chars.InsertPtr(new CodeToChar("ucirc",'�'));
	Chars.InsertPtr(new CodeToChar("uuml",'�'));
	Chars.InsertPtr(new CodeToChar("yuml",'�'));
	Chars.InsertPtr(new CodeToChar("ugrave",'�'));
	Chars.InsertPtr(new CodeToChar("yacute",'�'));
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::AnalyseBody(void)
{
	RXMLTag* content;
	RXMLTag* act;
	RXMLTag* Open[9];    // Remember all tag open.
	RXMLTag** ptr;
	int Level;
	char* OldBlock;
	char* Ins;
	int i;

	// Init Part
	memset(Open,0,9*sizeof(RXMLTag*));
	Open[0]=content=Doc->GetContent();
	Level=0;
	OldBlock=0;
	MinOpenLevel=0;

	// Parse it
	while((*Pos)&&(CurTag))
	{
		if(bEndTag)
		{
			Ins=Block;
			OldBlock=0;
			Block=Pos;
			// Find the lowest tag open and assign the text to it.
			for(ptr=&Open[7],Level=7;!(*ptr);ptr--,Level--);
			act=(*ptr);
//			if(!act->NbPtr)
//			{
//			}
			// All the lowest tag and himself (not if Content) are closed.
			Level=CurTag->Level;
			if(!Level)
				Level=1;
		}
		else
		{
			Ins=OldBlock;
			OldBlock=Block;
			// Find the highest open tag above the current one
			for(ptr=&Open[CurTag->Level-1],Level=CurTag->Level-1;!(*ptr);ptr--,Level--);
			if(Ins)
			{
				// Text to attach. If the lowest level is the content,
				// create a 'P' tag else attach to it.
				if(Level)
					act=(*ptr);
				else
					act=0;
			}
			if((CurTag->Ins)&&(CurTag->Type!=Tag::tBODY))
				Doc->AddNode(*ptr,Open[CurTag->Level]=new RXMLTag(CurTag->XMLName));
			else
			{
				OldBlock=0;
			}
//			if(CurTag->Type==Tag::tBODY)
//				Block=Pos;
			if(!act)
				act=Open[CurTag->Level];
			// All the lowest tag are closed.
			Level=CurTag->Level+1;
		}
		if(Ins)
		{
			AnalyzeBlock(Ins,act);
		}
		// All the tag lower than 'Level' are closed.
		for(ptr=&Open[7],i=7;i>=Level;ptr--,i--)
			(*ptr)=0;
		// find highest tag not null
		for(MinOpenLevel=i-1,ptr--;(*ptr);ptr--,MinOpenLevel--);
		NextValidTag();
	}

	// Delete all the empty tags of the content.
	content->DeleteEmptyTags();
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::AnalyseHeader(void)
{
	RXMLTag* meta;
	Tag::tTag LastType=Tag::tNULL;;
	char* OldBlock;
	char* OldParams;

	// Init Part.
	meta=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/html");

	// Parse it.
	NextValidTag();
	while((*Pos)&&CurTag)
	{
		switch(LastType)
		{
			 case Tag::tTITLE:
				AnalyzeBlock(OldBlock,Doc->AddTitle());
				break;
			case Tag::tMETA:
				ReadMetaTag(OldParams,meta);
				break;
			default:
				break;
		};
		if(bEndTag)
			LastType=Tag::tNULL;
		else
		{
			OldBlock=Block;
			OldParams=Params;
			LastType=CurTag->Type;
		}
		if((bEndTag&&(CurTag->Type==Tag::tHEAD))||(!CurTag->Head)) return;
		NextValidTag();
	}
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::ReadMetaTag(char* params,RXMLTag* /*metaData*/)
{
	char* ptr;
	char delimiter;
	char* name;    // Name of the tag.
	char* content; // Content of the tag.
	bool bSpaces;

	// Read the name of the META Data
	if(!params) return;
	ptr=params;
	while((*ptr)&&((*ptr)!='=')&&(!isspace(*ptr)))
		(*(ptr++))=RString::ToUpper(*ptr);
	bSpaces=isspace(*ptr);
	(*(ptr++))=0;
	if(bSpaces)
	{
		while((*ptr)&&((*ptr)!='='))
			ptr++;
		ptr++;    // Skip '=';
	}

	// The name must be 'HTTP-EQUIV'
	if((!(*ptr))||(strcmp(params,"HTTP-EQUIV"))) return;

	// Skip spaces and read the delimiter which must be a ' or a "
	while((*ptr)&&(isspace(*ptr)))
		ptr++;
	delimiter=(*(ptr++));
	if((delimiter!='\'')&&(delimiter!='"')) return;

	// Read the type of HTTP-EQUIV
	name=ptr;
	while((*ptr)&&((*ptr)!=delimiter))
		(*(ptr++))=RString::ToUpper(*ptr);
	(*(ptr++))=0;  // Skip the second delimiter

	// Search for 'CONTENT'
	while((*ptr)&&(RString::ToUpper(*ptr)!='C'))
		ptr++;
	if((!(*ptr))||strncasecmp(ptr,"CONTENT",7)) return;
	ptr+=7;

	// Search '='
	while((*ptr)&&((*ptr)!='='))
		ptr++;
	ptr++;

	// Skip spaces and read the delimiter which must be a ' or a "
	while((*ptr)&&(isspace(*ptr)))
		ptr++;
	delimiter=(*(ptr++));
	if((delimiter!='\'')&&(delimiter!='"')) return;

	// Read the type of HTTP-EQUIV
	content=ptr;
	while((*ptr)&&((*ptr)!=delimiter))
		ptr++;
	(*(ptr++))=0;  // Skip the second delimiter

	if(!strcmp(name,"DESCRIPTION"))
	{
		AnalyzeBlock(content,Doc->AddSubject());
	}
	else if(!strcmp(name,"KEYWORDS"))
	{
		AnalyzeKeywords(content,',',Doc->AddSubject());
	}
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::ReplaceCode(void)
{
	char* ptr=Pos+1;
	char tmp[11];
	char* ptr2=tmp;
	int len;
	CodeToChar *c;

	// Search maximal 10 letter ending with a ';' or spaces
	len=0;
	while((*ptr)&&((*ptr)!=';')&&(!isspace(*ptr))&&(len<10))
	{
		(*(ptr2++))=(*(ptr++));
		len++;
	}

	// If not ';' -> not a code
	if((!(*ptr))||((*ptr)!=';')) return;
	ptr++; // Skip ';'.

	// Find the code
	(*ptr2)=0;
	c=Chars.GetPtr<const char*>(tmp);
	if(!c) return;

	// Replace character.
	(*Pos)=c->GetChar();

	// Search end of word from end of the code.
	len=1;
	ptr2=ptr;
	while((*ptr2)&&(!isspace(*ptr2)))
	{
		ptr2++;
		len++;
	}

	// Move end of the word to skip the code and replace by spaces.
	memcpy(Pos+1,ptr,len*sizeof(char));
	len=c->GetLen()+1;
	memset(ptr2-len,' ',len*sizeof(char));
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::NextTag(void)
{
	bool bParams;            // Look if there are parameters.

beginread:

	// Find Begin of Tag ('<')
	while((*Pos)&&((*Pos)!='<'))
	{
		if((*Pos)=='&')
			ReplaceCode();
		Pos++;
		BlockLen++;
	}

	// Skip '<'
	if(!(*Pos))
	{
		CurTag=0;
		return;
	}
	SkipTag=Pos;
	(*SkipTag)=0;    // If valid tag, don't treat it.
	TagLen=1;
	Pos++;

	// Is it a comment?
	if(!strncmp(Pos,"!--",3))
	{
		char* SkipComments=Pos-1;
		unsigned len=1;
		Pos+=3; TagLen+=3; len+=3;// Skip '!--'
		// Search end of comments
		do
		{
			while((*Pos)&&((*Pos)!='-'))
			{
				Pos++;
				TagLen++;
				len++;
			}
			if(*Pos) // Skip '-'
			{
				Pos++;
				TagLen++;
				len++;
			}
		} while((*Pos)&&strncmp(Pos,"->",2));
		Pos+=2; TagLen+=2; len+=2;// Skip '->'
		memset(SkipComments,' ',len*sizeof(char));
		goto beginread;
	}
	
	// Is it a ending tag ('</')?
	bEndTag=((*Pos)==('/'));
	if(bEndTag)
	{
		Pos++; TagLen++; // Skip '/'.
	}

	// Skip spaces between '<' or '</' and the name of te tag.
	while((*Pos)&&isspace(*Pos))
	{
		Pos++;
		TagLen++;
	}

	// Find end of tag name (In uppercase) -> End if space or '>' or '/'
	BeginTag=Pos;
	while((*Pos)&&((*Pos)!='>')&&(!(((*Pos)=='/')&&((*(Pos+1))=='>')))&&(!isspace(*Pos)))
	{
		if((*Pos)=='&')
			ReplaceCode();
		(*(Pos++))=RString::ToUpper(*Pos);
		TagLen++;
	}
	bParams=isspace(*Pos); // If name ending with a space -> Possible parameters.
	(*(Pos++))=0;
	TagLen++;

	// If Possible parameters
	if(bParams)
	{
		// Slip spaces after name of the tag.
		while((*Pos)&&isspace(*Pos))
		{
			Pos++;
			TagLen++;
		}

		// If next character is '/' or '>' -> No parameters for the tag.
		bParams=(!(((*Pos)=='/')&&((*(Pos+1))=='>'))&&((*Pos)!='>'));

		// Parameters are store until  '/>' or '>'
		if(bParams)
		{
			Params=Pos;
			while((*Pos)&&((*Pos)!='>')&&(!(((*Pos)=='/')&&((*(Pos+1))=='>'))))
			{
				if((*Pos)=='&')
					ReplaceCode();
				Pos++;
				TagLen++;
			}
		}
		else
			Params=0;

		// Skip '>' or '/'.
		Pos++;
		TagLen++;
	}
	else
		Params=0;

	// Skip '>' or '/>'
	if((*Pos)=='>')
	{
		TagLen++;
		Pos++;
	}
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::NextValidTag(void)
{
	// Look if it is a known tag. If not, replace all by spaces and continue.
	do
	{
		if(!(*Pos)) return;
		NextTag();
		CurTag=Tags->GetPtr<const char*>(BeginTag);
		if(!CurTag)
		{
			memset(SkipTag,' ',TagLen*sizeof(char));
		}
		else if((CurTag->Type==Tag::tSCRIPT)&&(!bEndTag))
		{
			char* SkipScript=SkipTag;
			BlockLen=TagLen;
			while((!CurTag)||(!((CurTag->Type==Tag::tSCRIPT)&&bEndTag)))
			{
				NextTag();
				CurTag=Tags->GetPtr<const char*>(BeginTag);
				BlockLen+=TagLen;
			}
			memset(SkipScript,' ',BlockLen*sizeof(char));
			CurTag=0;
			Block=0;
		}
	} while(!CurTag);

	// Return
	BlockLen=0;
	if((!bEndTag)/*||(!MinOpenLevel)*/)
	{
		Block=Pos;
	}
}


//---------------------------------------------------------------------------
GALILEI::GFilterHTML::~GFilterHTML()
{
	if(Tags) delete Tags;
}
