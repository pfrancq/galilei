/*

	GALILEI Research Project

	GFilterSGML.cpp

	A SGML filter - Implementation.

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
#include <string.h>
#include <iostream> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>

//---------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rxmltag.h>
#include <rstd/rtextfile.h>
using namespace R;


//---------------------------------------------------------------------------
// include files for GALILEI
#include <gfiltersgml.h>
#include <filters/codetochar.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class Tag
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GALILEI::GFilterSGML::Tag
{
public:
	enum tTag{tNULL,tDOC,tDOCNO,tTITLE,tDATE,tPUBLI,tSOURCE,tTEXT,tPAR,tSUBJECT,tAUTHOR,tSUMMARY,tNOTE,tDESCRIPT,tRIGHTS,tLANG};
	RString Name;
	RString XMLName;
	tTag Type;
	bool Head;
	int Level;
	bool Ins;

	Tag(const char* n,const char* x,tTag t,bool h,int l,bool i)
		: Name(n), XMLName(x), Type(t),Head(h), Level(l),Ins(i) {}
	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
	int Compare(const char* t) const {return(Name.Compare(t));}
};



//---------------------------------------------------------------------------
//
// class GFilterSGML
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GFilterSGML::GFilterSGML(GFactoryFilter* fac)
	: GFilter(fac/*"SGML Filter",*/), Tags(0),
	 Buffer(0), Chars(50,5)
{
	AddMIME(fac->GetMng(),"text/sgml");
	InitCharContainer();
	Tags=new RContainer<Tag,unsigned int,true,true>(10,5);
	Tags->InsertPtr(new Tag("DOC","",Tag::tDOC,false,8,false));
	Tags->InsertPtr(new Tag("DOCNO","",Tag::tDOCNO,false,8,true));
	Tags->InsertPtr(new Tag("HL","",Tag::tTITLE,true,8,true));
	Tags->InsertPtr(new Tag("DD","",Tag::tDATE,false,8,true));
	Tags->InsertPtr(new Tag("SO","",Tag::tPUBLI,false,8,true));
	Tags->InsertPtr(new Tag("IN","",Tag::tSUBJECT,false,8,true));
//	Tags->InsertPtr(new Tag("DATELINE","docxml:date",Tag::tDATELINE,false,8,true));
	Tags->InsertPtr(new Tag("TEXT","",Tag::tTEXT,false,8,true));
	Tags->InsertPtr(new Tag("PAR","docxml:p",Tag::tPAR,false,8,true));
	Tags->InsertPtr(new Tag("AUTHOR","",Tag::tAUTHOR,false,8,true));
	Tags->InsertPtr(new Tag("FIRST","",Tag::tPUBLI,false,8,true));
	Tags->InsertPtr(new Tag("SECOND","",Tag::tSOURCE,false,8,true));
	Tags->InsertPtr(new Tag("NOTE","",Tag::tDESCRIPT,false,8,true));
	Tags->InsertPtr(new Tag("HEAD","",Tag::tTITLE,false,8,true));
	Tags->InsertPtr(new Tag("DESCRIPT","",Tag::tDESCRIPT,false,8,true));
	Tags->InsertPtr(new Tag("LEADPARA","",Tag::tSUBJECT,false,8,true));
	Tags->InsertPtr(new Tag("HEADLINE","",Tag::tTITLE,false,8,true));
	Tags->InsertPtr(new Tag("BYLINE","",Tag::tAUTHOR,false,8,true));
	Tags->InsertPtr(new Tag("EDITION","",Tag::tSOURCE,false,8,true));
	Tags->InsertPtr(new Tag("NAME","",Tag::tPUBLI,false,8,true));
	Tags->InsertPtr(new Tag("PUBDATE","",Tag::tDATE,false,8,true));
	Tags->InsertPtr(new Tag("COPYRGHT","",Tag::tRIGHTS,false,8,true));
	Tags->InsertPtr(new Tag("LANGUAGE","",Tag::tLANG,false,8,true));
	Tags->InsertPtr(new Tag("DATELINE","",Tag::tSOURCE,false,8,true));
	Tags->InsertPtr(new Tag("JOURNAL","",Tag::tPUBLI,false,8,true));
	Tags->InsertPtr(new Tag("TITLE","",Tag::tTITLE,false,8,true));
	Tags->InsertPtr(new Tag("SUMMARY","",Tag::tTEXT,false,8,true));
	Tags->InsertPtr(new Tag("ABSTRACT","docxml:p",Tag::tTEXT,false,8,true));
	
	
	
	
	
	

//	Tags->InsertPtr(new Tag("H1","docxml:h1",Tag::tH1,1,true));
//	Tags->InsertPtr(new Tag("H2","docxml:h2",Tag::tH2,2,true));
//	Tags->InsertPtr(new Tag("H3","docxml:h3",Tag::tH3,3,true));
//	Tags->InsertPtr(new Tag("H4","docxml:h4",Tag::tH4,4,true));
//	Tags->InsertPtr(new Tag("H5","docxml:h5",Tag::tH5,5,true));
//	Tags->InsertPtr(new Tag("H6","docxml:h6",Tag::tH6,6,true));

}

//---------------------------------------------------------------------------
void GALILEI::GFilterSGML::AnalyseDoc(void)
{
	RXMLTag* meta ;
	Tag::tTag LastType=Tag::tNULL;;
	char* OldBlock;
	char* OldParams;

// Init Part.
	meta=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/sgml");


	// Parse it.
	NextValidTag();
	while((*Pos)&&CurTag)
	{
		switch(LastType)
		{
			case Tag::tTITLE:
				AnalyzeBlock(OldBlock,Doc->AddTitle());
				break;

			case Tag::tAUTHOR:
				AnalyzeBlock(OldBlock,Doc->AddCreator());
				break;
			

			case Tag::tSUBJECT:
				AnalyzeBlock(OldBlock,Doc->AddSubject());
				break;

			case Tag::tDESCRIPT:
				AnalyzeBlock(OldBlock,Doc->AddDescription());
				break;

			case Tag::tPUBLI:
				AnalyzeBlock(OldBlock,Doc->AddPublisher());
				break;

//			case Tag::t:
//				AnalyzeBlock(OldBlock,Doc->AddContributor());
//				break;

			case Tag::tDATE:
				AnalyzeBlock(OldBlock,Doc->AddDate());
				break;

			case Tag::tSOURCE:
				AnalyzeBlock(OldBlock,Doc->AddSource());
				break;

			case Tag::tLANG:
				AnalyzeBlock(OldBlock,Doc->AddLanguage());
				break;

//			case Tag::t:
//				AnalyzeBlock(OldBlock,Doc->AddRelation());
//				break;

//			case Tag::t:
//				AnalyzeBlock(OldBlock,Doc->AddCoverage());
//				break;

			case Tag::tRIGHTS:
				AnalyzeBlock(OldBlock,Doc->AddRights());
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
		if((bEndTag&&(CurTag->Type==Tag::tDOC)) )
		{
			return;
		}

		NextValidTag();
		if( CurTag->Type == Tag::tTEXT)
		{
			AnalyseContent();
		}
	}
}


//---------------------------------------------------------------------------
bool GALILEI::GFilterSGML::Analyze(GDocXML* doc)
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
	AnalyseDoc();

	// Done
	if(Buffer)
	{
		delete[] Buffer;
		Buffer=0;
	}
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GFilterSGML::InitCharContainer(void)
{
	Chars.InsertPtr(new CodeToChar("Ucirc",'Û'));
	Chars.InsertPtr(new CodeToChar("gt",'>'));
	Chars.InsertPtr(new CodeToChar("lt",'<'));
	Chars.InsertPtr(new CodeToChar("equals",'='));

	Chars.InsertPtr(new CodeToChar("amp",'&'));
	Chars.InsertPtr(new CodeToChar("plus",'+'));
	Chars.InsertPtr(new CodeToChar("minus",'-'));
	Chars.InsertPtr(new CodeToChar("lsqb",'['));
	Chars.InsertPtr(new CodeToChar("rsqb",']'));
	Chars.InsertPtr(new CodeToChar("less",'-'));

	
}

void GALILEI::GFilterSGML::AnalyseContent(void)
{
	RXMLTag *actuelT, *contentT;
	RXMLTag** ptr;
	RXMLTag* openT[9];

	char *oldBlock, *insert;
	int level;

// Init Part
	memset(openT,0,9*sizeof(RXMLTag*));
	openT[0]=contentT=Doc->GetContent();
	level= MinOpenLevel= 0;
	oldBlock=0;

	//Parse It
	while((*Pos)&&(CurTag))
	{
		if(bEndTag)
		{
			insert = Block;
			oldBlock=0;
			Block=Pos;

			if(insert)
			{
				if(level == 0) // if parent node == docxml:content -> create a docxml:p to contain the bloc.
				{
					level++;
					openT[level]=new RXMLTag("docxml:p");
					Doc->AddNode(openT[level-1],openT[level]);
					AnalyzeBlock(insert,openT[level] );
					level--;
				}
				else
				{
					AnalyzeBlock(insert,openT[level] );
				}
			}
			if ( (level >0) &&(CurTag->Type!=Tag::tTEXT) )
			{
				level--;
			}
			if ((CurTag->Type == Tag::tTEXT)&&(bEndTag) )
			{
				contentT->DeleteEmptyTags(Doc);
				return; // fin du text!!
			}
		}
		else
		{
			insert= oldBlock;
			oldBlock = Block;
			ptr=&openT[level];
			actuelT=(*ptr);
			
			if(insert)
			{
				if(level == 0) // if parent node == docxml:content -> create a docxml:p to contain the bloc.
				{
					level++;
					openT[level]=new RXMLTag("docxml:p");
					Doc->AddNode(openT[level-1],openT[level]);
					AnalyzeBlock(insert,openT[level] );
					level--;
				}
				else
				{
					AnalyzeBlock(insert,openT[level] );
				}
			}
			if((CurTag->Ins)&&(CurTag->Type!=Tag::tTEXT))
			{
				//tmpT = new RXMLTag(CurTag->XMLName);
				level++;
				openT[level]=new RXMLTag(CurTag->XMLName);
				Doc->AddNode(actuelT,openT[level]);
			}
		}
		NextValidTag();
	}
}

//---------------------------------------------------------------------------
void GALILEI::GFilterSGML::ReplaceCode(void)
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
void GALILEI::GFilterSGML::NextTag(void)
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
			(*Pos)=0;
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
void GALILEI::GFilterSGML::NextValidTag(void)
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
//		else if((CurTag->Type==Tag::tSCRIPT)&&(!bEndTag))
//		{
//			char* SkipScript=SkipTag;
//			BlockLen=TagLen;
//			while((!CurTag)||(!((CurTag->Type==Tag::tSCRIPT)&&bEndTag)))
//			{
//				NextTag();
//				CurTag=Tags->GetPtr<const char*>(BeginTag);
//				BlockLen+=TagLen;
//			}
//			memset(SkipScript,' ',BlockLen*sizeof(char));
//			CurTag=0;
//			Block=0;
//		}
	} while(!CurTag);

	// Return
	BlockLen=0;
	if((!bEndTag)/*||(!MinOpenLevel)*/)
	{
		Block=Pos;
	}
}


//------------------------------------------------------------------------------
void GFilterSGML::CreateParams(GParams*)
{
}


//---------------------------------------------------------------------------
GALILEI::GFilterSGML::~GFilterSGML()
{
	if(Tags) delete Tags;
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("SGML Filter",GFilterSGML)
