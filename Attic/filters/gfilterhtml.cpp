/*

	GALILEI Research Project

	GFilterHTML.cpp

	A HTML filter - Implementation.

	(C) 2001 by Lamoral Julien

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#if unix
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
// class GFilterHTML
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GFilterHTML::GFilterHTML(GURLManager* mng)
: GFilter(mng), Buffer(0), Chars(50,5)
{
	AddMIME("text/html");
}


//---------------------------------------------------------------------------
bool GALILEI::GFilterHTML::Analyze(GDocXML* doc)
{
	int accessmode,handle;
	struct stat statbuf;

	// Initialisation
	Doc=doc;
	accessmode=O_RDONLY;
	#if !unix
		accessmode=O_BINARY;
	#endif
	handle=open(Doc->GetFile(),accessmode);
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	SkipSpaces();

	//Initialisation of the different variable
	InitCharContainer();
	
	for (int i=0;i<8;i++)
	{
		ClassementVector[i]=0;
	}
	
	NextWirteTag=true;

	// Begin  the parsing of the HTML Structure
	doc->AddNode(doc->GetTop(),meta=new RXMLTag("MetaData"));
	doc->AddNode(meta,act=new RXMLTag("URL"));
	act->InsertAttr(new RXMLAttr("value",Doc->GetURL()));
	doc->AddNode(meta,act=new RXMLTag("TypeDoc"));
	act->InsertAttr(new RXMLAttr("code","text/html"));

	// Traitement of Headers
	AnalyseHeader(doc);

	// Traitement of Content
	doc->AddNode(doc->GetTop(),content=new RXMLTag("Content"));
	AnalyseBody(doc);

	// Traitment of links
	doc->AddNode(doc->GetTop(),links=new RXMLTag("links"));
	// Done
	if (Begin)
	{
		delete[] Begin;
		Begin=0;
	}
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::AnalyseBody(GDocXML* doc)
{
	bool body=true;

	while (body)
	{
		//initialisation of body
		NextWirteTag= true;
		NextTag();
		// if  there is a h1 or .. before le body stop
		if (TagCompare("h1")||TagCompare("h2")||TagCompare("h3")||TagCompare("h4")||TagCompare("h5")||TagCompare("h6")||TagCompare("p")||TagCompare("/html"))
		{
			body=false;
		}
		else if (TagCompare("body"))
		{
			while((*Buffer)&&(!TagCompare("/body"))&&(!end))
			{
				// go to the next tag
				NextTag();
				//  if compare to ...  search to the good fonction
				if (TagCompare("h1")||TagCompare("h2")||TagCompare("h3")||TagCompare("h4")||TagCompare("h5")||TagCompare("h6")||TagCompare("p"))
				{
					
					GetValueCurentTag (OldTag.StrDup(), doc);
				}
				//for the element of table
				else if(TagCompare("td")||TagCompare("li")||TagCompare("hr"))
				{
				GetValueCurentTag ("p", doc);
				}
				else if (TagCompare("script"))
				{
					NextTag();
					while (!TagCompare("/script"))
					{
						NextTag();
					}
				}
				else if (TagCompare("/body"))
				{
					end=true;
				}
				else
				{
				}
			}
			body= false;
		}
	}
	
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::AnalyseHeader(GDocXML* doc)
{
// analyse of html header
	bool head=true;
	while (head)
	{
		NextTag();
		if (TagCompare("body")||TagCompare("h1")||TagCompare("h2")||TagCompare("h3")||TagCompare("h4")||TagCompare("h5")||TagCompare("h6")||TagCompare("p")||TagCompare("/html"))
		{
			head = false;
		}
		else if (TagCompare("head"))
		{
		   while(!TagCompare("/head"))
			{
				NextTag();
				if (TagCompare("title")||TagCompare("meta") )
				{
					if (TagCompare("title"))
					{
						GetValue(doc);
					}
					else // meta
					{
						GetMetaValue(doc);
					}
				
				}
				 // add here other head data
			}
			head= false;
		}
	}
}


//---------------------------------------------------------------------------
bool GALILEI::GFilterHTML::TagCompare(const char* current)
{
	if (TAG==current)
	{
		OldTag=TAG;
		return(true);
	}
	return(false);
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::GetValue(GDocXML* doc)
{
	bool ok=true;
	Pos=Buffer;
	// we are just next the <title> tag
	// if we are at </title> we go out and place the good value into the tag value
	while((*Buffer)&&ok)
	{
		if ((*Buffer)=='<')
		{
			ok=false;
		}
		else
		{
			Buffer++;
		}
	}

	// the buffer is "on" a '<'
	(*Buffer)=0;

	doc->AddNode(meta,act=new RXMLTag("Title"));
	AnalyzeBlock(Pos,act);
	// replace the buffer with'<'
	(*Buffer)='<';
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::GetValueCurentTag(char* curent,GDocXML* doc)
{
	SkipSpaces();
	bool ok2=false;
	bool ok4=true;
	ptrvalue=Buffer;
	end = false;

	// "save" in ptrvalue all the value of the currentTag
	// whit <a> </a> ,... on stop if we are on a  <h'x'> </p> </body>,....
	while((*Buffer)&&ok4)
	{
		if(!(*Buffer)) end=true;
		if ((*Buffer)=='<')
		{
			char* temporarybuffer;
			temporarybuffer = Buffer;
			temporarybuffer++;
			if ( (*temporarybuffer)=='h' ||(*temporarybuffer)=='p'||(*temporarybuffer)=='H'||(*temporarybuffer)=='P')
			{
				ok4=false;
				(*Buffer)=0;
				
			}
			else if((*temporarybuffer)=='t'||(*temporarybuffer)=='T')
			{
				temporarybuffer++;
				if ( (*temporarybuffer)=='d'||(*temporarybuffer)=='D')
				{
					ok4=false;
					(*Buffer)=0;
				}
				else
				{
					while ((*Buffer)&&(*Buffer!='>'))
					{
						Buffer++;
					}
					if ((*Buffer)) Buffer++;
				}
			}
			else if((*temporarybuffer)=='l'||(*temporarybuffer)=='L')
			{
				temporarybuffer++;
				if ( (*temporarybuffer)=='i'||(*temporarybuffer)=='I')
				{
					ok4=false;
					(*Buffer)=0;
				}
				else
				{
					while ((*Buffer)&&(*Buffer!='>'))
					{
						Buffer++;
					}
					if ((*Buffer)) Buffer++;
				}
			}
			else if (((*temporarybuffer)=='/'))
			{
				temporarybuffer++;
				if ( (*temporarybuffer)=='h'||(*temporarybuffer)=='p'||(*temporarybuffer)=='H'||(*temporarybuffer)=='P'||(*temporarybuffer)=='t'||(*temporarybuffer)=='T')
				{
				ok4=false;
				(*Buffer)=0;
				}
				else if((*temporarybuffer)=='t'||(*temporarybuffer)=='T')
				{	
					temporarybuffer++;
					if ( (*temporarybuffer)=='d'||(*temporarybuffer)=='D')
					{
						ok4=false;
						(*Buffer)=0;
					}
					else
					{
						while ((*Buffer)&&(*Buffer!='>'))
						{
							Buffer++;
						}
						if ((*Buffer)) Buffer++;
					}
				}
				else if((*temporarybuffer)=='l'||(*temporarybuffer)=='L')
				{	
					temporarybuffer++;
					if ( (*temporarybuffer)=='i'||(*temporarybuffer)=='I')
					{
						ok4=false;
						(*Buffer)=0;
					}
					else
					{
						while ((*Buffer)&&(*Buffer!='>'))
						{
							Buffer++;
						}
						if ((*Buffer)) Buffer++;
					}
				}
				else if (  (*temporarybuffer)=='b'||(*temporarybuffer)=='B')
				{
					temporarybuffer++;
					if ( (*temporarybuffer)=='o'||(*temporarybuffer)=='O' )
					{
						ok4=false;
						(*Buffer)=0;
					}
					else Buffer++;
				}
				else Buffer++;
			}	
			else
			{
				while ((*Buffer)&&(*Buffer!='>'))
				{
					Buffer++;
				}
				if ((*Buffer)) Buffer++;
				
			}
		}
		else Buffer++;
	}

	// we enter to Pos the interessant value into the 2inetresting tags and whe retire the unintresting inforation	
//	Pos=ptrvalue;
	RString sentence (200);
	while((*ptrvalue))
	{
		bool ok3=true;

		// if  char is a & it is a special html code
		if((*ptrvalue)=='&')
		{
			(*ptrvalue)=' ';
			ptrvalue++;
			hold=ptrvalue;
			while((*ptrvalue)&&ok3)
			{
				if((*ptrvalue)==';')
				{
					ok3=false;
					(*ptrvalue)=0;
				}
				else
				{
					ptrvalue++;
				}
			}
			CodeToChar* ptr;
			ptr = (Chars.GetPtr<const char*>(hold));
			// we search the corresponding value
			while(*hold)
			{
				(*hold)=' ';
				if ((*hold))hold++;
			}

			if (ptr)
			{
				sentence+=ptr->Return;
				(*ptrvalue)=ptr->Return;
				ptrvalue++;
			}
			else
			{
			 (*ptrvalue)=' ';
			 sentence+=' ';
			}
		}
		
		// if there is a tag it is a unintresting tag
		else if ((*ptrvalue)=='<')
		{
			while ((*ptrvalue)&&(*ptrvalue!='>'))
				{
					(*ptrvalue)=' ';
					ptrvalue++;
					
				}
				(*ptrvalue)=' ';
				ptrvalue++;	
		}
		else
		{
			sentence+=(*ptrvalue);
			ptrvalue++;
			ok2=true;
		}
	}

	
	// whe analyse only if there is a containt
	if (ok2)
	{
		
		Pos=sentence.StrDup();	
		bool contents=true;

		// we compare the current tag  .. and case the value of this tag
		// if the tag must be attached to the current we attach his  to
		// the precednent tag else we attach to the content level

		if (!strcmp(curent,"h1"))
		{
			contents= true;
			for (int i=0;i<8;i++)
			{
				ClassementVector[i]=0;
			}

			ClassementVector [1]=1;
			doc->AddNode(content,TagVector [1]=new RXMLTag("h1"));
			AnalyzeBlock(Pos,TagVector [1]);
		}

		if (!strcmp(curent,"h2"))
		{
			AddTag(2,curent, doc);
		}
		if (!strcmp(curent,"h3"))
		{
			AddTag(3,curent, doc);
		}
		if (!strcmp(curent,"h4"))
		{
			AddTag(4,curent, doc);
		}
		if (!strcmp(curent,"h5"))
		{
			AddTag(5,curent, doc);
		}
		if (!strcmp(curent,"h6"))
		{
			AddTag(6,curent, doc);
		}
		if (!strcmp(curent,"p"))
		{
		    AddTag(7,curent, doc);
		}
	}

	// on remet le buffer a <
	(*Buffer)='<';
}
//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::AddTag(int level,char* tag,GDocXML* doc)
{
	bool contents=true;
	int curTag;
	for (int i=1;i<level;i++)
	{
		if (ClassementVector[i]!=0)
		{
			contents=false;
			curTag=i;
		}
	}
	if(contents)
	{
		for (int i=0;i<level+1;i++)
		{
			ClassementVector[i]=0;
		}
		ClassementVector [level]=1;
		doc->AddNode(content,TagVector [level]=new RXMLTag(tag));
		AnalyzeBlock(Pos,TagVector [level]);
	}
	else
	{
		ClassementVector [level]=1;
		doc->AddNode(TagVector [curTag],TagVector [level]=new RXMLTag(tag));
		AnalyzeBlock(Pos,TagVector [level]);
	}

}

//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::InitCharContainer(void)
{
	//  intialisation of the  char container ... attention one & is remplace by a  blanc
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



//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::GetMetaValue (GDocXML* doc)
{

	RString temp2 (100);
	bool ok=true;
	bool ok1=false;
	bool ok2=true;

	// looking for meta name ( keywords or description and stop the buffer after the   ="
	while((*Buffer)&&ok2)
	{
		if ((*Buffer)=='"')
		{
			ok2 = false ;
			Buffer++;
		}
		else
		{
			Buffer++;
		}
	}
	ok2=true;
	// the information is the name of the metatag

	while((*Buffer)&&ok2)
	{
		if ((*Buffer)=='"')
		{
			ok2 = false ;
			Buffer++;
		}
		else
		{
			temp2+=(*Buffer);
			Buffer++;
		}
	}
	char* cur1;
	char* cur2;
	cur1="keywords";
	cur2="description";
	temp2.StrLwr();
	ok=false;
	if ((temp2==cur1)||(temp2==cur2))
	{
		TAG=temp2;
		ok2=true;
		ok=true;
		ok1=true;
	}

	while((*Buffer)&&ok2)
	{
		if ((*Buffer)=='"')
		{
			ok2 = false ;
			Buffer++;
		}
		else
		{
			Buffer++;
		}
	}

	Pos=Buffer;
	// we save the information into Pos
	while((*Buffer)&&ok)
	{
		if((*Buffer)==13||(*Buffer)==10||(*Buffer)=='\t')
		{
			(*Buffer)=' ';
			Buffer++;
		}
		if ((*Buffer)=='"')
		{
	 		ok = false ;
		}
		else
		{
	 		Buffer++;
		}
	}
	
	// the buffer is "on" a '"'
	(*Buffer)=0;

	if(ok1)
	{
		if (temp2==cur1)
		{
			doc->AddNode(meta,act=new RXMLTag("KeyWord"));
			AnalyzeKeywords(Pos,' ',act);
		}
		else if (temp2==cur2)
		{

		}
		doc->AddNode(meta,act=new RXMLTag("Resume"));
		AnalyzeBlock(Pos,act);
	}
	(*Buffer)='"';
	Buffer++;
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::NextTag (void)
{
	bool ok=true;
	end = false;

	while((*Buffer)&&ok)
	{
	 	SkipSpaces();
	 	if (*Buffer=='<')
	 	{
	 	ok = false;
	 	}
	 	Buffer++;
	 	if(!(*Buffer)) end=true;
	}

	RString temp5 (10);

	if (!ok)
	{
		while((*Buffer)&&(!ok))
		{
			if (IsSpace())
			{
				temp5.StrLwr();
				if (temp5=="meta")
				{
					ok = true;
				}
				else
				{
					while ((*Buffer)&&*Buffer!='>')
					{
						Buffer++;
					}
					ok = true;
				}
			}
			else if (*Buffer=='>')
				{
					ok = true;
				}
				else
				{
					temp5+=(*Buffer);
				}
			Buffer++;
			if(!(*Buffer)) end=true;
		}

		temp5.StrLwr();
		TAG=temp5;

	}
}


//---------------------------------------------------------------------------
bool GALILEI::GFilterHTML::IsSpace(void)
{
	if((*Buffer)==13||(*Buffer)==10||(*Buffer)=='\t'||(*Buffer)==' ')
		return(true);
	return(false);
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::SkipSpaces(void)
{
	while((*Buffer)&&IsSpace())
		Buffer++;
}


//---------------------------------------------------------------------------
GALILEI::GFilterHTML::~GFilterHTML()
{
}
