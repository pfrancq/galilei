/*

	GALILEI Research Project

	GFilterRTF.cpp

	A RTF filter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Kumps Nicolas (nkumps@ulb.ac.be)

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
// include files for GALILEI
#include <gfilterrtf.h>
#include <docs/gfiltermanager.h>
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class Tag
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//class GALILEI::GFilterRTF::Tag
//{
//public:
//	enum tTag{tNULL,tPAR};
//	RString Name;
//	RString XMLName;
//	tTag Type;
//	bool Head;
//	int Level;
//	bool Ins;

//	Tag(const char* n,const char* x,tTag t,bool h,int l,bool i)
//		: Name(n), XMLName(x), Type(t),Head(h), Level(l),Ins(i) {}
//	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
//	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
//	int Compare(const char* t) const {return(Name.Compare(t));}
//};



//------------------------------------------------------------------------------
//
// class GFilterRTF
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GALILEI::GFilterRTF::GFilterRTF(GFactoryFilter* fac)
	: GFilter(fac), Tags(0),
	 Buffer(0), Chars(50,5)
{
	AddMIME("text/rtf");
	InitCharContainer();
	Tags=new RContainer<Tag,unsigned int,true,true>(10,5);

	Tags->InsertPtr(new Tag("\\par","docxml:p",Tag::tPAR,false,8,false));
	Tags->InsertPtr(new Tag("\\PAR","docxml:p",Tag::tPAR,false,8,false));
	Tags->InsertPtr(new Tag("\\pard","docxml:p",Tag::tPAR,false,8,false));
	Tags->InsertPtr(new Tag("\\PARD","docxml:p",Tag::tPAR,false,8,false));
	Tags->InsertPtr(new Tag("\\pict","",Tag::tSKIP,false,8,false));
	Tags->InsertPtr(new Tag("\\PICT","",Tag::tSKIP,false,8,false));
	Tags->InsertPtr(new Tag("\\shp","",Tag::tSKIP,false,8,false));
	Tags->InsertPtr(new Tag("\\SHP","",Tag::tSKIP,false,8,false));
	Tags->InsertPtr(new Tag("\\*\\datafield","",Tag::tSKIP,false,8,false));
	Tags->InsertPtr(new Tag("\\*\\DATAFIELD","",Tag::tSKIP,false,8,false));
//	Tags->InsertPtr(new Tag("\\field","",Tag::tSKIPTO,false,8,false));
//	Tags->InsertPtr(new Tag("\\FIELD","",Tag::tSKIPTO,false,8,false));
//	Tags->InsertPtr(new Tag("\\fldrslt","",Tag::tEND,false,8,false));
//	Tags->InsertPtr(new Tag("\\FLDRSLT","",Tag::tEND,false,8,false));
	Tags->InsertPtr(new Tag("\\author","",Tag::tAUTHOR,false,8,false));
	Tags->InsertPtr(new Tag("\\AUTHOR","",Tag::tAUTHOR,false,8,false));
	Tags->InsertPtr(new Tag("\\operator","",Tag::tOPERATOR,false,8,false));
	Tags->InsertPtr(new Tag("\\OPERATOR","",Tag::tOPERATOR,false,8,false));
	Tags->InsertPtr(new Tag("\\title","",Tag::tTITLE,false,8,false));
	Tags->InsertPtr(new Tag("\\TITLE","",Tag::tTITLE,false,8,false));
	Tags->InsertPtr(new Tag("\\subject","",Tag::tSUBJECT,false,8,false));
	Tags->InsertPtr(new Tag("\\SUBJECT","",Tag::tSUBJECT,false,8,false));

}

//------------------------------------------------------------------------------
bool GFilterRTF::ExtractCmd(Tag::tTag* tagtype)
{
	char* ptr=Pos+1;
	char tmp[50];
	char* ptr2=tmp;
	int len;
	len=45;
	(*tagtype)=Tag::tNULL;

	(*(ptr2++))=(*Pos);
	while((*ptr)&&((*ptr)!='\\')&&((*ptr)!='{')&&((*ptr)!='}')&&(!isspace(*ptr))&&(len--))
		(*(ptr2++))=(*(ptr++));
	(*ptr2)=0;
	if (Tags->IsIn(tmp))
		(*tagtype)=Tags->GetPtr<const char*>(tmp)->Type;
	while((*ptr)&&((*ptr)!=' ')&&((*ptr)!='{')&&((*ptr)!='}'))
		ptr++;
	Pos=ptr;
	if(!(*ptr))
	{
		(*tagtype)=Tag::tNULL;
		return(false);
	}
	if(((*tagtype)==Tag::tNULL)||((*tagtype==Tag::tSKIPTO)))
		return(false);
	else return(true);
}


//------------------------------------------------------------------------------
void GFilterRTF::DeleteCmd()
{
	char* ptr=Pos;
	int len;

	len=0;
	while((*ptr)&&((*ptr)!=' ')&&((*ptr)!='{')&&((*ptr)!='}'))
	{
		ptr++;
		len++;
	}
	(*Pos)=' ';
	memset(ptr-len,' ',len*sizeof(char));
}


//------------------------------------------------------------------------------
void GFilterRTF::SkipTag()
{
	bool end=false;
	Tag::tTag tagtype=Tag::tNULL;
	int len=0;
	char* mem;
	while((!end)&&(*Pos))
	{
		while((*Pos)&&((*Pos)!='\\'))
		{
			Pos++;
			len++;
		}
		mem=Pos;
		ExtractCmd(&tagtype);
		Pos=mem;
		if(tagtype==Tag::tEND)
			end=true;
		Pos++;
		len++;
	}
	Pos--;
	len--;
	memset(Pos-len,' ',len*sizeof(char));

}


//------------------------------------------------------------------------------
bool GFilterRTF::Analyze(GDocXML* doc) throw(bad_alloc,GException)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Header;
	bool Paragraph;
	char tmp;
	char* findstr;
	char* del;
	char* mem;
	int accessmode,handle;
	bool empty=true;
	Tag::tTag newtagtype,oldtagtype;
	struct stat statbuf;

	// Init Part
	Doc=doc;
	accessmode=O_RDONLY;
	#ifndef _BSD_SOURCE
		accessmode=O_BINARY;
	#endif
	handle=open(Doc->GetFile(),accessmode);
	if(handle==-1)
		throw GException("file not found : "+Doc->GetFile());
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	close(handle);
	SkipSpaces();

	// Create the metaData tag and the first information
	part=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/rtf");
	// Look for the content
	part=Doc->GetContent();
	Pos=Begin; // Remember the first line which is not a command
	while(*Pos)
	{
		SkipSpaces();
		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=false;
		newtagtype=oldtagtype=Tag::tNULL;
		while ((!Paragraph)&&(*Pos)&&(oldtagtype!=Tag::tSKIP))
		{
			while((*Pos)&&((*Pos)!='\\'))
				Pos++;
			Begin=Pos;
			if(!(*Pos)) return(true);
			else
			{
				Paragraph=ExtractCmd(&oldtagtype);
				if(oldtagtype==Tag::tSKIP) Paragraph=false;
			}
		}
		if(!(*Pos)) return(true);
		Begin=Pos;
		empty=true;
		while((*Pos)&&Paragraph)
		{
			// Read a Line
			while((*Pos)&&((*Pos)!='\\')&&((*Pos)!='{')&&((*Pos)!='}'))
			{
				if(!isspace(*Pos)) empty=false;
				Pos++;
			}
			if(!(*Pos))
				Paragraph=false;
			else
			{
				Paragraph=ReplaceCode();
				mem=Pos;
				if(!Paragraph)
				{
					Paragraph=(!(ExtractCmd(&newtagtype)));
					if(!Paragraph)
					{
//						if(newtagtype==Tag::tSKIPTO) SkipTag();
						Pos=mem;
					}
					else
					{
						Pos=mem;
						DeleteCmd();
					}
				}
			}

		}
		switch(oldtagtype)
		{
			case Tag::tPAR :
				if(!empty) part->AddTag(tag=new RXMLTag("docxml:p"));
				break;
			case Tag::tNULL :
				part->AddTag(tag=new RXMLTag(""));
				Paragraph=false;
				break;
			default :
				break;
		};
		if(!(*Pos)) return(true);
		Pos--;
		(*(Pos++))=0;
		if((newtagtype!=Tag::tSKIP)&&(oldtagtype!=Tag::tSKIP))
		{
			switch(oldtagtype)
			{
				case Tag::tTITLE:
					Doc->AddTitle(Begin);
					break;
				case Tag::tAUTHOR:
					Doc->AddCreator(Begin);
					break;
				case Tag::tOPERATOR:
					break;
				case Tag::tSUBJECT:
					Doc->AddSubject(Begin);
					break;
				default:
					if(!empty) AnalyzeBlock(Begin,tag);
				break;
			}
		}
	}

	// Done Part
	if(Buffer)
	{
		delete[] Buffer;
		Buffer=0;
	}

	// Return OK
	return(true);
}


//------------------------------------------------------------------------------
void GALILEI::GFilterRTF::InitCharContainer(void)
{
	Chars.InsertPtr(new CodeToChar("\\u224\\\'00\\\'E0",'à'));
	Chars.InsertPtr(new CodeToChar("\\u225\\\'00\\\'E1",'á'));
	Chars.InsertPtr(new CodeToChar("\\u226\\\'00\\\'E2",'â'));
	Chars.InsertPtr(new CodeToChar("\\u228\\\'00\\\'E4",'ä'));
	Chars.InsertPtr(new CodeToChar("\\u230\\\'00\\\'E6",'?'));
	Chars.InsertPtr(new CodeToChar("\\u231\\\'00\\\'E7",'ç'));
	Chars.InsertPtr(new CodeToChar("\\u232\\\'00\\\'E8",'è'));
	Chars.InsertPtr(new CodeToChar("\\u233\\\'00\\\'E9",'é'));
	Chars.InsertPtr(new CodeToChar("\\u234\\\'00\\\'EA",'ê'));
	Chars.InsertPtr(new CodeToChar("\\u235\\\'00\\\'EB",'ë'));
	Chars.InsertPtr(new CodeToChar("\\u236\\\'00\\\'EC",'ì'));
	Chars.InsertPtr(new CodeToChar("\\u237\\\'00\\\'ED",'í'));
	Chars.InsertPtr(new CodeToChar("\\u238\\\'00\\\'EE",'î'));
	Chars.InsertPtr(new CodeToChar("\\u239\\\'00\\\'EF",'ï'));
	Chars.InsertPtr(new CodeToChar("\\u242\\\'00\\\'F2",'ò'));
	Chars.InsertPtr(new CodeToChar("\\u243\\\'00\\\'F3",'ó'));
	Chars.InsertPtr(new CodeToChar("\\u244\\\'00\\\'F4",'ô'));
	Chars.InsertPtr(new CodeToChar("\\u246\\\'00\\\'F6",'ö'));
	Chars.InsertPtr(new CodeToChar("\\u249\\\'00\\\'F9",'ù'));
	Chars.InsertPtr(new CodeToChar("\\u250\\\'00\\\'FA",'ú'));
	Chars.InsertPtr(new CodeToChar("\\u251\\\'00\\\'FB",'û'));
	Chars.InsertPtr(new CodeToChar("\\u253\\\'00\\\'FD",'ü'));
	Chars.InsertPtr(new CodeToChar("\\\'e0",'à'));
	Chars.InsertPtr(new CodeToChar("\\\'e1",'á'));
	Chars.InsertPtr(new CodeToChar("\\\'e2",'â'));
	Chars.InsertPtr(new CodeToChar("\\\'e4",'ä'));
	Chars.InsertPtr(new CodeToChar("\\\'e6",'?'));
	Chars.InsertPtr(new CodeToChar("\\\'e7",'ç'));
	Chars.InsertPtr(new CodeToChar("\\\'e8",'è'));
	Chars.InsertPtr(new CodeToChar("\\\'e9",'é'));
	Chars.InsertPtr(new CodeToChar("\\\'ea",'ê'));
	Chars.InsertPtr(new CodeToChar("\\\'eb",'ë'));
	Chars.InsertPtr(new CodeToChar("\\\'ec",'ì'));
	Chars.InsertPtr(new CodeToChar("\\\'ed",'í'));
	Chars.InsertPtr(new CodeToChar("\\\'ee",'î'));
	Chars.InsertPtr(new CodeToChar("\\\'ef",'ï'));
	Chars.InsertPtr(new CodeToChar("\\\'f2",'ò'));
	Chars.InsertPtr(new CodeToChar("\\\'f3",'ó'));
	Chars.InsertPtr(new CodeToChar("\\\'f4",'ô'));
	Chars.InsertPtr(new CodeToChar("\\\'f6",'ö'));
	Chars.InsertPtr(new CodeToChar("\\\'f9",'ù'));
	Chars.InsertPtr(new CodeToChar("\\\'fa",'ú'));
	Chars.InsertPtr(new CodeToChar("\\\'fb",'û'));
	Chars.InsertPtr(new CodeToChar("\\\'fd",'ü'));
	Chars.InsertPtr(new CodeToChar("\\\'E0",'à'));
	Chars.InsertPtr(new CodeToChar("\\\'E1",'á'));
	Chars.InsertPtr(new CodeToChar("\\\'E2",'â'));
	Chars.InsertPtr(new CodeToChar("\\\'E4",'ä'));
	Chars.InsertPtr(new CodeToChar("\\\'E6",'?'));
	Chars.InsertPtr(new CodeToChar("\\\'E7",'ç'));
	Chars.InsertPtr(new CodeToChar("\\\'E8",'è'));
	Chars.InsertPtr(new CodeToChar("\\\'E9",'é'));
	Chars.InsertPtr(new CodeToChar("\\\'EA",'ê'));
	Chars.InsertPtr(new CodeToChar("\\\'EB",'ë'));
	Chars.InsertPtr(new CodeToChar("\\\'EC",'ì'));
	Chars.InsertPtr(new CodeToChar("\\\'ED",'í'));
	Chars.InsertPtr(new CodeToChar("\\\'EE",'î'));
	Chars.InsertPtr(new CodeToChar("\\\'EF",'ï'));
	Chars.InsertPtr(new CodeToChar("\\\'F2",'ò'));
	Chars.InsertPtr(new CodeToChar("\\\'F3",'ó'));
	Chars.InsertPtr(new CodeToChar("\\\'F4",'ô'));
	Chars.InsertPtr(new CodeToChar("\\\'F6",'ö'));
	Chars.InsertPtr(new CodeToChar("\\\'F9",'ù'));
	Chars.InsertPtr(new CodeToChar("\\\'FA",'ú'));
	Chars.InsertPtr(new CodeToChar("\\\'FB",'û'));
	Chars.InsertPtr(new CodeToChar("\\\'FD",'ü'));
	Chars.InsertPtr(new CodeToChar("\\rquote",'\''));
	Chars.InsertPtr(new CodeToChar("\\rdblquote",'\"'));
	Chars.InsertPtr(new CodeToChar("\\RQUOTE",'\''));
	Chars.InsertPtr(new CodeToChar("\\RDBLQUOTE",'\"'));
	Chars.InsertPtr(new CodeToChar("{",' '));
	Chars.InsertPtr(new CodeToChar("}",' '));

}


//------------------------------------------------------------------------------
bool GALILEI::GFilterRTF::ReplaceCode(void)
{
	char* ptr=Pos+1;
	char tmp[20];
	char* ptr2=tmp;
	int len,clen;
	CodeToChar *c;

	// Search 12 letters
	len=0;
	clen=12;
	(*(ptr2++))=(*Pos);
	if(((*Pos)!='{')&&((*Pos)!='}'))
		while((clen)&&(*ptr)&&(!isspace(*ptr)))
		{
			(*(ptr2++))=(*(ptr++));
			len++;
			clen--;
		}

	if(!(*ptr)) return(false);

	// Find the code
	(*ptr2)=0;
	c=Chars.GetPtr<const char*>(tmp);
	//if(!c) return(false);
//----------------test------------------
	if((!c)&&(len>=4))
	{
		ptr=Pos+1;
		ptr2=tmp;
		len=0;
		clen=3;
		(*(ptr2++))=(*Pos);
		if(((*Pos)!='{')&&((*Pos)!='}'))
			while((clen)&&(*ptr)&&(!isspace(*ptr)))
			{
				(*(ptr2++))=(*(ptr++));
				len++;
				clen--;
			}
		(*ptr2)=0;
		c=Chars.GetPtr<const char*>(tmp);
	}
	if(!c) return(false);
//--------------------------------------

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
	len=c->GetLen()-1;
	memset(ptr2-len,' ',len*sizeof(char));
	return(true);
}


//------------------------------------------------------------------------------
void GFilterRTF::CreateParams(GParams*)
{
}


//------------------------------------------------------------------------------
GALILEI::GFilterRTF::~GFilterRTF()
{
	if(Tags) delete Tags;
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("RTF Filter",GFilterRTF)

