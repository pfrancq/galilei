/*

	GALILEI Research Project

	gdoc.cpp

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

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


// include files for ANSI C/C++
#include <ctype.h>
#include <string.h>
#include <fstream.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------------------------------------------------------
// include files for Galilei
#include <gdocs/gdoc.h>
#include <gsessions/gsession.h>
#include <glib.h>

using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

GDoc::GDoc (GDocs *owner,const RString& url) throw(bad_alloc)
  : Owner(owner),Words(0),NbWords(0),NbDiffWords(),Lang(0),URL(url),Content(0), Calc(true), bSave(true)
{
  Words=new GWordOccurs(this,500);
}


//---------------------------------------------------------------------------

int GDoc::Compare(const GDoc& doc)
{
  return(Id-doc.Id);
}


//---------------------------------------------------------------------------
int GDoc::Compare(const GDoc* doc)
{
  return(Id-doc->Id);
}


//---------------------------------------------------------------------------
int GDoc::Compare(const unsigned id)
{
  return(Id-id);
}



//---------------------------------------------------------------------------
unsigned char GDoc::GetChar(void)
{
 char *ptr2;
 char tmp=0;
 static char code[6];

 Buffer++;   // Skip '&'
 if(IsSpace()) return(0);
 ptr2=code;
 while((*Buffer)&&(*Buffer)!=';'&&(*Buffer)!='<'&&(*Buffer)!='>'&&!IsSpace())
 (*(ptr2++))=(*(Buffer++));
 (*ptr2)=0;
 if(*Buffer) Buffer++;  // Skip ';'
 if((*code)=='#')   // a chr
 {
 ptr2=code+1;
 tmp=(char)atoi((char*)ptr2);



 if (! GLib::IsAlpha(tmp)) tmp=0;

 }
 else        // Test some strings
 {
    if(!strcmp((char*)code,"eacute")) tmp='é';
    if(!strcmp((char*)code,"egrave")) tmp='è';
    if(!strcmp((char*)code,"agrave")) tmp='à';
  }
  return(tmp);
}


//---------------------------------------------------------------------------
bool GDoc::IsSpace(void)
{
  if((*Buffer)==13||(*Buffer)==10||(*Buffer)=='\t'||(*Buffer)==' ')
    return(true);
  return(false);
}


//---------------------------------------------------------------------------
void GDoc::SkipSpaces(void)
{
  while((*Buffer)&&IsSpace())
    Buffer++;
}


//---------------------------------------------------------------------------
void GDoc::InitWords(void)
{
  Buffer=Content;
  (*Word)=0;
  SkipSpaces();
}


//---------------------------------------------------------------------------
bool GDoc::NextWord(void)
{
	char *ptr,tmp;
	long Open;
	bool Cont=true;

  // Init
  	InitNextWord:
 	memset(Word,0,51);
  	tmp=0;
  	if(!(*Buffer)) return(false);

  // Skip No words

  while((*Buffer)&&!GLib::IsAlpha(*Buffer)&&Cont)
  {
    switch(*Buffer)
    {
      case '<':
      Open=1;
      Buffer++;   // Skip '<'
       while(Open)
        {
          switch(*Buffer)
          {
            case 0:
              return(false);

            case '<':   // New One
              Open++;
              break;

            case '>': // Close One
              Open--;
              break;
          }
          Buffer++;
        }
        break;

      case '&':
        tmp=GetChar();
        if(tmp&&(!IsSpace())) Cont=false;
        break;

      default:
        Buffer++;
        break;
    }
    SkipSpaces();
  }

  // Read Word
  ptr=Word;
  if(tmp) (*(ptr++))=tmp;
  Cont=true;
  Open=1;

  while((*Buffer)&&Cont)
  {
    if(GLib::IsAlpha(*Buffer))
      (*(ptr++))=(*(Buffer++));
    else
      switch(*Buffer)
      {
        case '-':
          if(GLib::IsAlpha(*(Buffer+1))) (*(ptr++))=(*Buffer); else Open=false;
          Buffer++; // Skip '-'
          break;

      case '&':
        tmp=GetChar();
        if(tmp) (*(ptr++))=tmp; else Cont=false;
        break;

        default:
          Cont=false;
      }
  }
  (*ptr)=0;

  // End
  SkipSpaces();
  if(!(*Word)) goto InitNextWord;
  GLib::ToASCII7(Word);
  return(true);
}


//---------------------------------------------------------------------------
void GDoc::Analyse(char* body) throw(GException)
{
  unsigned LastNbWords=0,ActNbWords;
  unsigned i;
  GLang** lang;
  GDict *dic,*stop;        // A dictionnary
  GWord Next;
  unsigned NextId;
  GWordOccur *Occur;

	// Attach Content
  Content=body;

  // Which Language
  for(i=Owner->Session->Langs->NbPtr+1,lang=Owner->Session->Langs->Tab;--i;lang++)    // Do it for each language
  {
    ActNbWords=0;
    dic=Owner->Session->GetStop(*lang);
    InitWords();
    while(NextWord())
    {
      Next.Word.StrLwr((char*)Word);
      if(dic->IsIn(Next)) ActNbWords++;
    }
    if(ActNbWords>LastNbWords)
    {
      Lang=(*lang);
      LastNbWords=ActNbWords;
      stop=dic;
    }
  }

	// Continue to analyse
	if(!Lang) goto endanalyse;
 	dic=Owner->Session->GetDic(Lang);
 	if(!dic) goto endanalyse;
	if(!dic->Loaded) goto endanalyse;

	// Analyse
	Words->Clear();
	NbWords=NbDiffWords=0;
	InitWords();
	while(NextWord())
	{
		NbWords++;		// Increase number of words
		Next.Word.StrLwr((char*)Word);
		if(!stop->IsIn(Next))
		{
			// Not in Stop List
			NextId=dic->GetId(Next.Word);
			Occur=Words->GetPtr(NextId,this);
			if(!Occur->Occur) NbDiffWords++;
			Occur->Occur++;
		}
	}
	Calc=false;


	// Detach Content
endanalyse:
	Content=0;
}


//---------------------------------------------------------------------------
GDoc::~GDoc(void)
{
  if(Words) delete Words;
  if(Content) delete Content;
}




