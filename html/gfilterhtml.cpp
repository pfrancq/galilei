/*

	GALILEI Research Project

	GFilterHTML.cpp

	A HTML filter - Implementation.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gfilterhtml.h>
#include <filters/gurlmanager.h>
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class Tag
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilterHTML::Tag
{
public:
	enum tTag{tNULL,tHTML,tSCRIPT,tHEAD,tTITLE,tMETA,tBODY,tLINK,tBASE,tH1,tH2,tH3,tH4,tH5,tH6,tP,tTD};
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



//------------------------------------------------------------------------------
//
// class GFilterHTML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterHTML::GFilterHTML(GFactoryFilter* fac)
	: GFilter(fac), Tags(0),
	 Buffer(0), Chars(50,5),Base(0)
{
	AddMIME(fac->GetMng(),"text/html");
	InitCharContainer();
	Tags=new RContainer<Tag,unsigned int,true,true>(10,5);
	Tags->InsertPtr(new Tag("HEAD","",Tag::tHEAD,true,8,false));
	Tags->InsertPtr(new Tag("SCRIPT","",Tag::tSCRIPT,true,8,false));
	Tags->InsertPtr(new Tag("META","",Tag::tMETA,true,8,true));
	Tags->InsertPtr(new Tag("TITLE","docxml:title",Tag::tTITLE,true,8,true));
	Tags->InsertPtr(new Tag("BASE","",Tag::tBASE,true,8,true));
	Tags->InsertPtr(new Tag("BODY","docxml:p",Tag::tBODY,false,7,false));
	Tags->InsertPtr(new Tag("H1","docxml:h1",Tag::tH1,false,1,true));
	Tags->InsertPtr(new Tag("H2","docxml:h2",Tag::tH2,false,2,true));
	Tags->InsertPtr(new Tag("H3","docxml:h3",Tag::tH3,false,3,true));
	Tags->InsertPtr(new Tag("H4","docxml:h4",Tag::tH4,false,4,true));
	Tags->InsertPtr(new Tag("H5","docxml:h5",Tag::tH5,false,5,true));
	Tags->InsertPtr(new Tag("H6","docxml:h6",Tag::tH6,false,6,true));
	Tags->InsertPtr(new Tag("P","docxml:p",Tag::tP,false,7,true));
	Tags->InsertPtr(new Tag("TD","docxml:p",Tag::tTD,false,7,true));
	Tags->InsertPtr(new Tag("LI","docxml:p",Tag::tTD,false,7,true));
	Tags->InsertPtr(new Tag("DIV","docxml:p",Tag::tP,false,7,true));
	Tags->InsertPtr(new Tag("A","",Tag::tLINK,false,8,true));

	
}


//------------------------------------------------------------------------------
bool GFilterHTML::Analyze(GDocXML* doc)
{
	int accessmode,handle;
	struct stat statbuf;

	// Initialisation
	Base=0;
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
	CurTag=NextTag=0;
	Params=NextParams=0;
	bEndTag=bEndNextTag=false;

	// Traitement of the document
	AnalyseHeader();
	AnalyseBody();

	// Done
	if(Buffer)
	{
		delete[] Buffer;
		Buffer=0;
	}
	return(true);
}


//------------------------------------------------------------------------------
void GFilterHTML::InitCharContainer(void)
{
	Chars.InsertPtr(new CodeToChar("Ucirc",'Û'));
	Chars.InsertPtr(new CodeToChar("nbsp",' '));
	Chars.InsertPtr(new CodeToChar("#190",'¾'));
	Chars.InsertPtr(new CodeToChar("Aacute",'Á'));
	Chars.InsertPtr(new CodeToChar("Ugrave",'Ù'));
	Chars.InsertPtr(new CodeToChar("iacute",'í'));
	Chars.InsertPtr(new CodeToChar("nbsp",' '));
	Chars.InsertPtr(new CodeToChar("Acirc",'Â'));
	Chars.InsertPtr(new CodeToChar("Uuml",'Ü'));
	Chars.InsertPtr(new CodeToChar("icirc",'î'));
	Chars.InsertPtr(new CodeToChar("Yacute",'Ý'));
	Chars.InsertPtr(new CodeToChar("#161",'¡'));
	Chars.InsertPtr(new CodeToChar("Agrave",'À'));
	Chars.InsertPtr(new CodeToChar("aacute",'á'));
	Chars.InsertPtr(new CodeToChar("igrave",'ì'));
	Chars.InsertPtr(new CodeToChar("Aring",'Å'));
	Chars.InsertPtr(new CodeToChar("acirc",'â'));
	Chars.InsertPtr(new CodeToChar("igrave",'ì'));
	Chars.InsertPtr(new CodeToChar("Atilde",'Ã'));
	Chars.InsertPtr(new CodeToChar("#180",'´'));
	Chars.InsertPtr(new CodeToChar("#191",'¿'));
	Chars.InsertPtr(new CodeToChar("Auml",'Ä'));
	Chars.InsertPtr(new CodeToChar("aelig",'æ'));
	Chars.InsertPtr(new CodeToChar("iuml",'ï'));
	Chars.InsertPtr(new CodeToChar("Ccedil",'Ç'));
	Chars.InsertPtr(new CodeToChar("agrave",'à'));
	Chars.InsertPtr(new CodeToChar("#171",'«'));
	Chars.InsertPtr(new CodeToChar("ETH",'Ð'));
	Chars.InsertPtr(new CodeToChar("aring",'å'));
	Chars.InsertPtr(new CodeToChar("#175",'¯'));
	Chars.InsertPtr(new CodeToChar("Eacute",'É'));
	Chars.InsertPtr(new CodeToChar("atilde",'ã'));
	Chars.InsertPtr(new CodeToChar("#181",'µ'));
	Chars.InsertPtr(new CodeToChar("Ecirc",'Ê'));
	Chars.InsertPtr(new CodeToChar("auml",'ä'));
	Chars.InsertPtr(new CodeToChar("#183",'*'));
	Chars.InsertPtr(new CodeToChar("Egrave",'È'));
	Chars.InsertPtr(new CodeToChar("#166",'¦'));
	Chars.InsertPtr(new CodeToChar("lt",'<'));
	Chars.InsertPtr(new CodeToChar("Euml",'Ë'));
	Chars.InsertPtr(new CodeToChar("ccedil",'ç'));
	Chars.InsertPtr(new CodeToChar("gt",'>'));
	Chars.InsertPtr(new CodeToChar("Iacute",'Í'));
	Chars.InsertPtr(new CodeToChar("#184",'¸'));
	Chars.InsertPtr(new CodeToChar("ntilde",'ñ'));
	Chars.InsertPtr(new CodeToChar("Icirc",'Î'));
	Chars.InsertPtr(new CodeToChar("oacute",'ó'));
	Chars.InsertPtr(new CodeToChar("Igrave",'Ì'));
	Chars.InsertPtr(new CodeToChar("#169",'©'));
	Chars.InsertPtr(new CodeToChar("raquo",'»'));
	Chars.InsertPtr(new CodeToChar("laquo",'«'));
	Chars.InsertPtr(new CodeToChar("ocirc",'ô'));
	Chars.InsertPtr(new CodeToChar("Iuml",'Ï'));
	Chars.InsertPtr(new CodeToChar("#164",'¤'));
	Chars.InsertPtr(new CodeToChar("#172",'¬'));
	Chars.InsertPtr(new CodeToChar("Ntilde",'Ñ'));
	Chars.InsertPtr(new CodeToChar("#176",'°'));
	Chars.InsertPtr(new CodeToChar("#170",'ª'));
	Chars.InsertPtr(new CodeToChar("Oacute",'Ó'));
	Chars.InsertPtr(new CodeToChar("#247",'÷'));
	Chars.InsertPtr(new CodeToChar("#186",'º'));
	Chars.InsertPtr(new CodeToChar("Ocirc",'Ô'));
	Chars.InsertPtr(new CodeToChar("eacute",'é'));
	Chars.InsertPtr(new CodeToChar("oslash",'ø'));
	Chars.InsertPtr(new CodeToChar("Ograve",'Ò'));
	Chars.InsertPtr(new CodeToChar("ecirc",'ê'));
	Chars.InsertPtr(new CodeToChar("otilde",'õ'));
	Chars.InsertPtr(new CodeToChar("Oslash",'Ø'));
	Chars.InsertPtr(new CodeToChar("egrave",'è'));
	Chars.InsertPtr(new CodeToChar("ouml",'ö'));
	Chars.InsertPtr(new CodeToChar("Otilde",'Õ'));
	Chars.InsertPtr(new CodeToChar("eth",'ð'));
	Chars.InsertPtr(new CodeToChar("#182",'¶'));
	Chars.InsertPtr(new CodeToChar("Ouml",'Ö'));
	Chars.InsertPtr(new CodeToChar("euml",'ë'));
	Chars.InsertPtr(new CodeToChar("#177",'±'));
	Chars.InsertPtr(new CodeToChar("THORN",'Þ'));
	Chars.InsertPtr(new CodeToChar("#189",'½'));
	Chars.InsertPtr(new CodeToChar("#163",'£'));
	Chars.InsertPtr(new CodeToChar("Uacute",'Ú'));
	Chars.InsertPtr(new CodeToChar("#188",'¼'));
	Chars.InsertPtr(new CodeToChar("#187",'»'));
	Chars.InsertPtr(new CodeToChar("#174",'®'));
	Chars.InsertPtr(new CodeToChar("#185",'¹'));
	Chars.InsertPtr(new CodeToChar("szlig",'ß'));
	Chars.InsertPtr(new CodeToChar("#167",'§'));
	Chars.InsertPtr(new CodeToChar("#178",'²'));
	Chars.InsertPtr(new CodeToChar("thorn",'þ'));
	Chars.InsertPtr(new CodeToChar("#173",'­'));
	Chars.InsertPtr(new CodeToChar("#179",'³'));
	Chars.InsertPtr(new CodeToChar("#215",'×'));
	Chars.InsertPtr(new CodeToChar("uacute",'ú'));
	Chars.InsertPtr(new CodeToChar("quot",'¨'));
	Chars.InsertPtr(new CodeToChar("#165",'¥'));
	Chars.InsertPtr(new CodeToChar("ucirc",'û'));
	Chars.InsertPtr(new CodeToChar("uuml",'ü'));
	Chars.InsertPtr(new CodeToChar("yuml",'ÿ'));
	Chars.InsertPtr(new CodeToChar("ugrave",'ù'));
	Chars.InsertPtr(new CodeToChar("yacute",'ý'));
}


//------------------------------------------------------------------------------
void GFilterHTML::AnalyseBody(void)
{
	RXMLTag* content;    // Pointer to the content part of DocXML.
	RXMLTag* links;      // Pointer to the links part of DocXML.
	RXMLTag* act;        // Actual tag representing a paragraph
	RXMLTag* Open[9];    // Remember all tag open.
	int Level;           // Actual level of tags.
	int i;
	RXMLTag** ptr;

	// Init Part
	memset(Open,0,9*sizeof(RXMLTag*));
	Open[0]=content=Doc->GetContent();
	links= Doc->GetLinks();
	Level=0;
	MinOpenLevel=0;

	// Parse it
	while(*Pos)
	{
		// Read Next Tag
		NextValidTag();
		if(!CurTag) continue;

		// If the current tag is a open link
		// -> Insert the link and read next tag
//		if((CurTag->Type == Tag::tLINK)&&(!bEndTag))
//		{
//			AnalyseLink();
//			continue;
//		}

		// Find the DocXML tag where the current block is to insert
		if(bEndTag)
		{
			// Closing HTML tag
			// Once the text was inserted -> the DocXML tag must be closed.
			// Of course, all the lowest DocXML tags are closed also.
			// Ex.: If docxml:H2 is closed -> then also close docxml:H3, ..., docxml:P
			if(CurTag->Level)
			{
				for(ptr=&Open[7],i=7;i>=CurTag->Level;ptr--,i--)
					(*ptr)=0;
			}

			// Find the lowest DocXML tag that was opened -> it will contain the text.
			for(ptr=&Open[7],Level=7;!(*ptr);ptr--,Level--);
			act=(*ptr);
		}
		else
		{
			// Opening HTML Tag
			// If there are opened DocXML tags from a lowest level -> close them
			for(ptr=&Open[7],i=7;i>CurTag->Level;ptr--,i--)
				(*ptr)=0;

			// Find the highest opened DocXML tag above the current one
			// Create a new DocXML tag and insert it in the highest opened DocXML tag.
			for(ptr=&Open[CurTag->Level-1],Level=CurTag->Level-1;!(*ptr);ptr--,Level--);
			Doc->AddNode(*ptr,act=Open[CurTag->Level]=new RXMLTag(CurTag->XMLName));
		}

		// Analyse the block and insert it in the current DocXML tag.
		// If block contains only spaces -> do not insert it.
		if(bBlockIns&&Block)
		{
			// If this tag is the content -> a new tag must be created
			if(act==content)
				Doc->AddNode(*ptr,act=Open[7]=new RXMLTag("docxml:p"));
			AnalyzeBlock(Block,act);
		}
	}

	// Delete all the nodes that contains nothing
	content->DeleteEmptyTags(Doc);
}


//------------------------------------------------------------------------------
void GFilterHTML::AnalyseLink(char* params,char* block,bool ins)
{
	RXMLTag* metalink;


	metalink = AnalyseLinkParams(params);
	if(metalink)
	{
		Doc->AddFormat("text/html",metalink);
		if(ins&&block)
			AnalyzeBlock(block,Doc->AddTitle(metalink));
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::AnalyseHeader(void)
{
	RXMLTag* meta;

	// Init Part.
	meta=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/html");

	// Parse it.
	NextValidTag();
	while((*Pos)&&CurTag)
	{
		if(!bEndTag)
		{
			switch(CurTag->Type)
			{
				 case Tag::tTITLE:
					AnalyzeBlock(Block,Doc->AddTitle());
					break;
				case Tag::tMETA:
					ReadMetaTag(Params,meta);
					break;
				case Tag::tBASE:
					AnalyseBase(Params);
					break;
				default:
					break;
			}
		}
		if((bEndTag&&(CurTag->Type==Tag::tHEAD))||(!NextTag->Head))
			return;
		NextValidTag();
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::AnalyseBase(char* params)
{
	char* ptr;
	char delimiter;
	char* content;
	bool bSpaces;

	if (!params) return;
	ptr= params;

	while(*ptr)
	{
		// Read the name of the tag
		while ((*ptr)&&((*ptr)!='=')&&(!isspace(*ptr)))
			(*(ptr++))=toupper(*ptr);
		bSpaces=isspace(*ptr);
		(*(ptr++))=0;
		if (bSpaces)
		{
			while((*ptr)&&((*ptr)!='='))
				ptr++;
			ptr++;    // Skip '=';
		}
		if((!(*ptr))||(strcmp(params,"HREF"))) {cout <<"another tag"<<endl;}

		// Skip spaces and read the delimiter which must be a ' or a "
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
		delimiter=(*(ptr++));
		if((delimiter!='\'')&&(delimiter!='"')) return;

		// Read the content of HREF or CLASS
		content=ptr;
		while((*ptr)&&((*ptr)!=delimiter))
			ptr++ ;

		(*(ptr++))=0;  // Skip the second delimiter
	
		 if (!strcmp(params,"HREF"))
		{
			 Base=content;
		}
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
			params=ptr;
	}
}


//------------------------------------------------------------------------------
RXMLTag* GFilterHTML::AnalyseLinkParams(char* params)
{
	RXMLTag* metaLink;

	char* ptr;
	char delimiter;
	char* content; // content of the tag
	char* urlG; // url of the link
	bool bSpaces,ancre;


	metaLink=0;
	if (!params) return metaLink ;
	ptr= params;

	while (*(ptr))
	{
		ancre=false;

		// Read the name of the tag
		while ((*ptr)&&((*ptr)!='=')&&(!isspace(*ptr)))
			(*(ptr++))=toupper(*ptr);
		bSpaces=isspace(*ptr);
		(*(ptr++))=0;
		if (bSpaces)
		{
			while((*ptr)&&((*ptr)!='='))
				ptr++;
			ptr++;    // Skip '=';
		}

		// The name must be 'HREF' or 'CLASS'
		if((!(*ptr))||((strcmp(params,"HREF"))&&(strcmp(params,"CLASS")))) return metaLink;

		// Skip spaces and read the delimiter which must be a ' or a "
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
		delimiter=(*(ptr++));
		if((delimiter!='\'')&&(delimiter!='"')) return metaLink;

		// Read the content of HREF or CLASS
		content=ptr;
		while((*ptr)&&((*ptr)!=delimiter))
		{                                   
			if ((*ptr) == '#')   // verify if the link point to the actuel page
				ancre= true;
			ptr++ ;
		}
		(*(ptr++))=0;  // Skip the second delimiter

			//if the link is a local anchor or a link to a mail -> drop this link
		if (ancre)
		{
			return metaLink ;
		}
		if  (!strncasecmp(content,"mailto:",7))
		{
			return metaLink;
		}
		if ((!strcmp(params,"HREF"))&&(!ancre))
		{
			if (!metaLink)  metaLink = Doc->AddLink();
			urlG= ConstructURL(content);
			Doc->AddIdentifier(urlG,metaLink);
		}
		else if (!strcmp(params,"CLASS"))
		{
			if (!metaLink)  metaLink = Doc->AddLink();
			Doc->AddType(content,metaLink);
		}

		//skip spaces
		while (*(ptr)&&(isspace(*ptr)))
			ptr++;
		params=ptr;
		ptr=params;
	}
	return metaLink;
}


//------------------------------------------------------------------------------
char* GFilterHTML::ConstructURL(char* u)
{

	char* urlG=0;      // the URL to  be returned
	char* ptr=0;
	char* base=0;
	char* endBase=0;
	
	RString urlTmp;
	urlG=new char[250];

	if (!Base)
	{
		// var initialisation
		urlTmp = Doc->GetURL();
		base = new char[urlTmp.GetLen()+1];
		strcpy(base,urlTmp);

		endBase= base;
		// find end of string
		while (*endBase)
		{
			endBase++;
		}
		// find the URL base (base -fileName ex:  /var/www/.../content/file.html -> /var/www/.../content )
		while ((*(endBase-1)) != '/')
		{
			endBase--;
		}
		(*endBase)=0;           
	}
	else
	{
		base=strdup(Base);
		endBase=base;
		//endBase=base=Base;
	}
  
	ptr=u;

	// Skip spaces
	while((*ptr)&&(isspace(*ptr)))
		ptr++;
	// if complete url -> no change
	if (!strncmp(ptr,"http://",7))
	{
		urlG=ptr;
		return urlG;
	}
	if (!strncmp(ptr,"ftp://",6))
	{
		urlG = ptr;
		return urlG;
	}

	// if url begins with a '.'
	while ((*ptr)== '.' )
	{
		ptr++;
		// this directory
		if ((*ptr)== '/')
		{
			ptr++;
		}
		else if (!strncmp(ptr,"./",2))
		{
			ptr+=2;
			if ((*(endBase-1)) =='/')
				endBase--;
			while ((*(endBase-1))!='/')
			{
				endBase--;
			}
			(*endBase)=0;
		}
	}

	if ( (strlen(base)+strlen(ptr)) <250 )
	{
		strcpy(urlG,base);
		strcat(urlG,ptr);
	}

	delete[] base;

	return urlG;
}


//------------------------------------------------------------------------------
void GFilterHTML::ReadMetaTag(char* params,RXMLTag* /*metaData*/)
{
	RXMLTag* link=0;
	char* ptr;
	char delimiter;
	char* name;    // Name of the tag.
	char* content; // Content of the tag.
	bool bSpaces;

	// Read the name of the META Data
	if(!params) return;
	ptr=params;
	while((*ptr)&&((*ptr)!='=')&&(!isspace(*ptr)))
		(*(ptr++))=toupper(*ptr);
	bSpaces=isspace(*ptr);
	(*(ptr++))=0;
	if(bSpaces)
	{
		while((*ptr)&&((*ptr)!='='))
			ptr++;
		ptr++;    // Skip '=';
	}

	// The name must be 'HTTP-EQUIV' or 'NAME'
	if((!(*ptr))||((strcmp(params,"HTTP-EQUIV"))&&(strcmp(params,"NAME")))) return;

	// Skip spaces and read the delimiter which must be a ' or a "
	while((*ptr)&&(isspace(*ptr)))
		ptr++;
	delimiter=(*ptr);
	if((delimiter!='\'')&&(delimiter!='"')) return;
	ptr++;

	// Read the type of HTTP-EQUIV or NAME
	name=ptr;
	while((*ptr)&&((*ptr)!=delimiter))
		(*(ptr++))=toupper(*ptr);
	(*(ptr++))=0;  // Skip the second delimiter

	// Search for 'CONTENT'
	while((*ptr)&&(toupper(*ptr)!='C'))
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
	delimiter=(*ptr);
	if((delimiter!='\'')&&(delimiter!='"')) return;
	ptr++;

	// Read the type of HTTP-EQUIV
	content=ptr;
	while((*ptr)&&((*ptr)!=delimiter))
		ptr++;
	(*(ptr++))=0;  // Skip the second delimiter

	if (!strcmp(params,"HTTP-EQUIV")) //metaData with parameter "HTTP-EQUIV"
	{
		if (!strcmp(name,"REFRESH"))
		{
			// Search for 'URL'
			while((*ptr)&&(toupper(*ptr)!='U'))
				ptr++;
			if((!(*ptr))||strncasecmp(ptr,"URL",3)) return;
			content+=3;
			while(((*content)== '=')||(isspace(*content)))
			{
				content++;
			}
			link= Doc->AddLink();
			content = ConstructURL(content);
			Doc->AddIdentifier(content,link);
			Doc->AddFormat("text/html",link);
			Doc->AddType("REFRESH",link);
		}
	}
	else if (!strcmp(params,"NAME"))  //metaData with parameter "NAME"
	{
		if (!strcmp(name,"DESCRIPTION"))
		{
			AnalyzeBlock(content,Doc->AddDescription());
		}
		else if (!strcmp(name,"KEYWORDS"))
		{
			AnalyzeKeywords(content,',',Doc->AddSubject());
		}
		else if (!strcmp(name,"TITLE"))
		{
			AnalyzeBlock(content,Doc->AddTitle());
		}
		else if (!strcmp(name,"AUTHOR"))
		{
			Doc->AddCreator(content);
		}
		else if (!strcmp(name,"GENARATOR"))
		{
			Doc->AddContributor(content);
		}
		else if (!strcmp(name,"COPYRIGHT"))
		{
			Doc->AddRights(content);
		}
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::ReplaceCode(void)
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


//------------------------------------------------------------------------------
void GFilterHTML::ReadNextTag(void)
{
	bool bParams;            // Look if there are parameters.

beginread:

	// Find Begin of Tag ('<')
	while((*Pos)&&((*Pos)!='<'))
	{
		if((*Pos)=='&')
		{
			ReplaceCode();
			bBlockIns=true;
		}
		else
			bBlockIns=bBlockIns||(!isspace(*Pos));
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
		(*(Pos++))=toupper(*Pos);
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
			//cerr << " les parametres "<< Params<<"   "<< *Params <<endl;
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


//------------------------------------------------------------------------------
GFilterHTML::Tag* GFilterHTML::GetValidTag(void)
{
	Tag* t;
	bool RemBlockIns;

	do
	{
		if(!(*Pos)) return(0);
		RemBlockIns=bBlockIns;
		ReadNextTag();
		t=Tags->GetPtr<const char*>(BeginTag);
		if(!t)
		{
			memset(SkipTag,' ',TagLen*sizeof(char));
		}
		else
		{
			if((t->Type==Tag::tSCRIPT)&&(!bEndTag))
			{
				char* SkipScript=SkipTag;
				BlockLen=TagLen;
				RemBlockIns=bBlockIns;
				while((!t)||(!((t->Type==Tag::tSCRIPT)&&bEndTag)))
				{
					ReadNextTag();
					t=Tags->GetPtr<const char*>(BeginTag);
					BlockLen+=TagLen;
				}
				memset(SkipScript,' ',BlockLen*sizeof(char));
				t=0;
				bBlockIns=RemBlockIns;
			}
		}
	}
	while(!t);
	return(t);
}


//------------------------------------------------------------------------------
void GFilterHTML::NextValidTag(void)
{
	char* CurParams;  // Pointer to current Params;
	bool bEndCurTag;
	char* OldBlock;
	char* OldSkipTag;
	unsigned int OldTagLen;
	bool bOldEndTag;
	char* OldParams;
	bool OldIns;

	// Next Tag becomes the current tag
	// Store in Block and Params the parameters of the current tag
	CurTag=NextTag;
	CurParams=NextParams;
	Block=Pos;
	bBlockIns=false;
	bEndCurTag=bEndNextTag;

	// Look for a known tag.
	// All other tags are replaced by spaces.
	NextTag=GetValidTag();

testlinks:
	// Treat Links
	if((NextTag)&&(NextTag->Type==Tag::tLINK))
	{
		// Remember Previous Block Position
		OldBlock=Block;
		Block=Pos;
		OldSkipTag=SkipTag;
		OldTagLen=TagLen;
		bOldEndTag=bEndTag;
		OldParams=Params;
		OldIns=bBlockIns;
		bBlockIns=false;

		// ReadNext Tag
		NextTag=GetValidTag();

		// If open link -> Analyse it
		if(!bOldEndTag)
			AnalyseLink(OldParams,Block,bBlockIns);

		// Skip the link tag.
		memset(OldSkipTag,' ',OldTagLen*sizeof(char));

		// Continue old Block
		Block=OldBlock;
		bBlockIns=OldIns;
		goto testlinks;
	}

	// Remember Params
	NextParams=Params;
	Params=CurParams;
	bEndNextTag=bEndTag;
	bEndTag=bEndCurTag;

	// If not CurTag and there is something to read -> read another tag.
	if((!CurTag)&&(*Pos))
		NextValidTag();
}


//------------------------------------------------------------------------------
void GFilterHTML::CreateParams(GParams*)
{
}


//------------------------------------------------------------------------------
GFilterHTML::~GFilterHTML()
{
	if(Tags) delete Tags;
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("HTML Filter",GFilterHTML)
