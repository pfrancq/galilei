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
void GALILEI::GFilterHTML::AnalyseBody(GDocXML* doc)
{
	bool body=true;
	char *tmpchar;

	while (body)
	{
		//initialisation of body
		NextWirteTag= true;
		NextTag();
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
				if (TagCompare("h1"))
				{
					tmpchar="h1";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("h2"))
				{
					tmpchar="h2";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("h3"))
				{
					tmpchar="h3";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("h4"))
				{
					tmpchar="h4";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("h5"))
				{
					tmpchar="h5";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("h6"))
				{
					tmpchar="h6";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("p"))
				{
					tmpchar="p";
					GetValueCurentTag (tmpchar, doc);
				}
				else if (TagCompare("meta"))
				{
				}
				else if (TagCompare("/body"))
				{
				end=true;
				}
				else if (TagCompare("/h1"))
				{
					tmpchar="h1";
				}
				else if (TagCompare("/h2"))
				{
					tmpchar="h2";
					
				}
				else if (TagCompare("/h3"))
				{
					tmpchar="h3";
				}
				else if (TagCompare("/h4"))
				{
					tmpchar="h4";
				}
				else if (TagCompare("/h5"))
				{
					tmpchar="h5";
				}
				else if (TagCompare("/h6"))
				{
					tmpchar="h6";
				}
				else if (TagCompare("/p"))
				{
					tmpchar="p";
					
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
			else if (((*temporarybuffer)=='/'))
			{
				temporarybuffer++;
				if ( (*temporarybuffer)=='h'||(*temporarybuffer)=='p'||(*temporarybuffer)=='H'||(*temporarybuffer)=='P')
				{
				ok4=false;
				(*Buffer)=0;
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
	Pos=ptrvalue;	
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
				(*ptrvalue)= ptr->Return;
			}
			else
			{
			 (*ptrvalue)=' ';
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
			ptrvalue++;
			ok2=true;
		}
	}
	// whe analyse only if there is a containt
	if (ok2)
	{
		bool contents=true;
		int curTag;

		// we compare the current tag  .. and case the value of this tag
		// if the tag must be attached to the current we attach his  to
		//the precednent tag else we attach to the contentrand nivo

		if (curent==("h1"))
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

		if (curent==("h2"))
		{
			contents=true;
			for (int i=1;i<2;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
				
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [2]=1;
				doc->AddNode(content,TagVector [2]=new RXMLTag("h2"));
				AnalyzeBlock(Pos,TagVector [2]);
			}
			else
			{
				ClassementVector [2]=1;
				doc->AddNode(TagVector [curTag],TagVector [2]=new RXMLTag("h2"));
				AnalyzeBlock(Pos,TagVector [2]);
			}
		}
		if (curent==("h3"))
		{
			contents=true;
			for (int i=1;i<3;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [3]=1;
				doc->AddNode(content,TagVector [3]=new RXMLTag("h3"));
				AnalyzeBlock(Pos,TagVector [3]);
			}
			else
			{
				ClassementVector [3]=1;
				doc->AddNode(TagVector [curTag],TagVector [3]=new RXMLTag("h3"));
				AnalyzeBlock(Pos,TagVector [3]);
			}
			
		}
		if (curent==("h4"))
		{
			contents=true;
			for (int i=1;i<4;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
				
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [4]=1;
				doc->AddNode(content,TagVector [4]=new RXMLTag("h4"));
				AnalyzeBlock(Pos,TagVector [4]);
			}
			else
			{
				ClassementVector [4]=1;
				doc->AddNode(TagVector [curTag],TagVector [4]=new RXMLTag("h4"));
				AnalyzeBlock(Pos,TagVector [4]);
			}
		}
		if (curent==("h5"))
		{
			contents=true;
			for (int i=1;i<5;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
				
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [5]=1;
				doc->AddNode(content,TagVector [5]=new RXMLTag("h5"));
				AnalyzeBlock(Pos,TagVector [5]);
			}
			else
			{
				ClassementVector [5]=1;
				doc->AddNode(TagVector [curTag],TagVector [5]=new RXMLTag("h5"));
				AnalyzeBlock(Pos,TagVector [5]);
			}
			
		}
		if (curent==("h6"))
		{
			contents=true;
			for (int i=1;i<6;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [6]=1;
				doc->AddNode(content,TagVector [6]=new RXMLTag("h6"));
				AnalyzeBlock(Pos,TagVector [6]);
			}
			else
			{
				ClassementVector [6]=1;
				doc->AddNode(TagVector [curTag],TagVector [6]=new RXMLTag("h6"));
				AnalyzeBlock(Pos,TagVector [6]);
			}
			
		}
		if (curent==("p"))
		{
			contents=true;
			for (int i=1;i<7;i++)
			{
				if (ClassementVector[i]!=0)
				{
					contents=false;
					curTag=i;
				}
			}
			if(contents)
			{
				for (int i=0;i<8;i++)
				{
					ClassementVector[i]=0;
				}
				ClassementVector [7]=1;
				doc->AddNode(content,TagVector [7]=new RXMLTag("p"));
				AnalyzeBlock(Pos,TagVector [7]);
			}
			else
			{
				ClassementVector [7]=1;
				doc->AddNode(TagVector [curTag],TagVector [7]=new RXMLTag("p"));
				AnalyzeBlock(Pos,TagVector [7]);
			}
		}
	}

	// on remet le buffer a <
	(*Buffer)='<';
}


//---------------------------------------------------------------------------
void GALILEI::GFilterHTML::InitCharContainer(void)
{
	//  intialisation of the  char container ... attention one & is remplace by a  blanc
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
	for (int i=0;i<7;i++)
	{
		h[i]=0;
	}
	for (int i=0;i<8;i++)
	{
		ClassementVector[i]=0;
	}
	p=0;
	NextWirteTag=true;

	// Begin  the parsing of the HTML Structure
	doc->AddNode(doc->GetTop(),meta=new RXMLTag("MetaData"));
	doc->AddNode(meta,act=new RXMLTag("URL"));
	act->InsertAttr(new RXMLAttr("value",Doc->GetURL()));
	doc->AddNode(meta,act=new RXMLTag("TypeDoc"));
	act->InsertAttr(new RXMLAttr("code","html"));

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
