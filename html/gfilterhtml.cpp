/*                         .

	GALILEIResearch Project

	gfilterhtml.cpp

	A XML representation of a document - Header.

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

//---------------------------------------------------------------------------
//
// class GFilterHTML
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
using namespace GALILEI;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
GFilterHTML::GFilterHTML(const RString& url,GURLManager* mng)
: GFilter(url,mng), Buffer(0), Chars(50,5)
{
	
	// Initialisation
	
	int accessmode,handle;
	struct stat statbuf;
	accessmode=O_RDONLY;
	#if !unix
		accessmode=O_BINARY;
	#endif
	handle=open(url,accessmode);
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;

	
	// intialisation of Buffer
	SkipSpaces();
	
	char *tmpchar;
	
	
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
	
	
	// Begin  the parsing of the HTML Strucuture
	
	
	s.AddNode(0,top=new RXMLTag("Doc"));
	
	
	s.AddNode(top,meta=new RXMLTag("MetaData"));
	
	
	s.AddNode(meta,act=new RXMLTag("URL"));
	act->InsertAttr(new RXMLAttr("value",URL.StrDup()));
	
	s.AddNode(meta,act=new RXMLTag("TypeDoc"));
	act->InsertAttr(new RXMLAttr("code","html"));
	
	
	// traitement of Headrers
	
	bool head=true;
	while (head)
	{
		NextTag();
		if (TagCompare("head"))
		{
		   while(!TagCompare("/head"))
			{
			    NextTag();
				if (TagCompare("title")|TagCompare("meta") )
				{
					if (TagCompare("title"))
					{	
						GetValue();	
					}
					else // meta
					{
						GetMetaValue();
					}
				
				}
				 // rajouter pour autre donne du head
			}
			head= false;
		}
	}
	
	// traitement of Content
		
	s.AddNode(top,content=new RXMLTag("Content"));
		
	bool body=true;
	while (body)
	{
		//initialisation of body
		NextWirteTag= true;
		NextTag();
		if (TagCompare("body"))
		{
		while((*Buffer)&&(!TagCompare("/body"))&&(!end))
			{
				NextTag();
				if (TagCompare("h1"))
				{
					tmpchar="h1";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("h2"))
				{
					tmpchar="h2";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("h3"))
				{
					tmpchar="h3";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("h4"))
				{
					tmpchar="h4";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("h5"))
				{
					tmpchar="h5";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("h6"))
				{
					tmpchar="h6";
					GetValueCurentTag (tmpchar);
				}
				else if (TagCompare("p"))
				{
					tmpchar="p";
					GetValueCurentTag (tmpchar);
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
				
				
				// rajouter pour autre donne utile du body
			}
			body= false;
				
		}
	}
	
	NextWirteTag= true;
	s.AddNode(top,links=new RXMLTag("links"));
	RXMLFile("/home/jlamoral/test",&s,RTextFile::Create);
   if (1==1)
	{
		cout << "jkll" << endl;
	}	
}




//---------------------------------------------------------------------------
void GFilterHTML::CloseallTag (int level)
{
	int i;
	for (i=level+1;i<6;i++)
	{
		if(h[i]!=0)
		{
			for(int j=0;j<=h[i];j++)
			{
			//temp+=("h");
			//temp+=(h[i]);
			}
		}
		h[i]=0;
	}



}


//---------------------------------------------------------------------------
bool GFilterHTML::TagCompare (const char* current)
{	
	if (TAG==current)
	{
		OldTag=TAG;
		return true;
	}	
	return false;

}



//---------------------------------------------------------------------------
void GFilterHTML::GetValue ()
{
	
	bool ok=true;
	Pos=Buffer;
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
	
	s.AddNode(meta,act=new RXMLTag("Title"));
	AnalyzeBlock(Pos,act);
	// replace the buffer with'<'
	(*Buffer)='<';
	

}

//---------------------------------------------------------------------------
void GFilterHTML::GetValueCurentTag (char* curent)
{
	
	SkipSpaces();
	bool ok2=false;
	bool ok4=true;
	ptrvalue=Buffer;
	end = false;
	
	// "save" in ptrvalue all the value of the currentTag
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
		
		
	Pos=ptrvalue;	
	while((*ptrvalue))
	{
		
		bool ok3=true;
				
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
			delete ptr;
		
		}
		
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
	
	
	if (ok2)
	{
	bool contents=true;
	int curTag;
	
		if (curent==("h1"))
		{
			contents= true;
			for (int i=0;i<8;i++)
			{
				ClassementVector[i]=0;
			}
			
			ClassementVector [1]=1;
			s.AddNode(content,TagVector [1]=new RXMLTag("h1"));
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
				s.AddNode(content,TagVector [2]=new RXMLTag("h2"));
				AnalyzeBlock(Pos,TagVector [2]);
			}
			else
			{
				ClassementVector [2]=1;
				s.AddNode(TagVector [curTag],TagVector [2]=new RXMLTag("h2"));
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
				s.AddNode(content,TagVector [3]=new RXMLTag("h3"));
				AnalyzeBlock(Pos,TagVector [3]);
			}
			else
			{
				ClassementVector [3]=1;
				s.AddNode(TagVector [curTag],TagVector [3]=new RXMLTag("h3"));
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
				s.AddNode(content,TagVector [4]=new RXMLTag("h4"));
				AnalyzeBlock(Pos,TagVector [4]);
			}
			else
			{
				ClassementVector [4]=1;
				s.AddNode(TagVector [curTag],TagVector [4]=new RXMLTag("h4"));
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
				s.AddNode(content,TagVector [5]=new RXMLTag("h5"));
				AnalyzeBlock(Pos,TagVector [5]);
			}
			else
			{
				ClassementVector [5]=1;
				s.AddNode(TagVector [curTag],TagVector [5]=new RXMLTag("h5"));
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
				s.AddNode(content,TagVector [6]=new RXMLTag("h6"));
				AnalyzeBlock(Pos,TagVector [6]);
			}
			else
			{
				ClassementVector [6]=1;
				s.AddNode(TagVector [curTag],TagVector [6]=new RXMLTag("h6"));
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
				s.AddNode(content,TagVector [7]=new RXMLTag("p"));
				AnalyzeBlock(Pos,TagVector [7]);
			}
			else
			{
				ClassementVector [7]=1;
				s.AddNode(TagVector [curTag],TagVector [7]=new RXMLTag("p"));
				AnalyzeBlock(Pos,TagVector [7]);
			
			}
			
		}
	}
	// delete temp;
	// on remet le buffer a <
	(*Buffer)='<';
	
}

//---------------------------------------------------------------------------
void GFilterHTML::InitCharContainer (void)
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
bool GFilterHTML::Analyze(GDocXML* doc)
{
//doc=s;
return(true);
}


//---------------------------------------------------------------------------
void GFilterHTML::GetMetaValue ()
{
	RString temp2 (100);
	bool ok=true;
	bool ok1=false;
	bool ok2=true;
	
	// looking for meta name ( keywords or description and stop the buffer after the   ="
	// mettre ds TAG Keywords ou Resume
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
    if ((temp2==cur1)|temp2==cur2)
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
		
		s.AddNode(meta,act=new RXMLTag("KeyWord"));
		AnalyzeKeywords(Pos,' ',act);
		}
		else if (temp2==cur2)
		{
		
		}
		s.AddNode(meta,act=new RXMLTag("Resume"));
		AnalyzeBlock(Pos,act);
	}
	(*Buffer)='"';
	Buffer++;


}



//---------------------------------------------------------------------------
void GFilterHTML::WriteTag (char* name,char* Attribut)
{

	char * Attribute;
	Attribute=Attribut;
	while((*Attribute)&&((*Attribute)==13||(*Attribute)==10||(*Attribute)=='\t'||(*Attribute)==' '))
	{
		Attribute++;
	}
	Attribut=Attribute;
	
	if (*Attribut)
	{
		NextWirteTag= true;
		cout << "<" << name << "  " << Attribut <<">" << "\n"<< endl;
		
	}
	else
	{
	// ne pas ecrire le tag suivant
	 NextWirteTag= false;
	}
}

//---------------------------------------------------------------------------
void GFilterHTML::WriteEndTag (char* name)
{

	if (NextWirteTag)
	{
		cout << "<" << "/" << name << ">" << "\n"<< endl;
	}
	NextWirteTag= false;
}

//---------------------------------------------------------------------------
void GFilterHTML::NextTag (void)
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
					while (*Buffer!='>')
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
bool GFilterHTML::IsSpace(void)
{
	if((*Buffer)==13||(*Buffer)==10||(*Buffer)=='\t'||(*Buffer)==' ')
		return(true);
	return(false);
}


//---------------------------------------------------------------------------
void GFilterHTML::SkipSpaces(void)
{
	while((*Buffer)&&IsSpace())
		Buffer++;
}


//---------------------------------------------------------------------------
void GFilterHTML::InitWords(void)
{

	
}


GFilterHTML::~GFilterHTML()
{
if (Begin)delete[] Begin;


}





