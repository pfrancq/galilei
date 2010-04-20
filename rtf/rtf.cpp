/*

	GALILEI Research Project

	RTF.cpp

	A RTF filter - Implementation.

	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by Nicolas Kumps.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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


//---------------------------------------------------------------------------
// include files for GALILEI
#include <rtf.h>
#include <gdoc.h>



//------------------------------------------------------------------------------
//
// class GFilterRTF
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterRTF::GFilterRTF(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), Tags(0), Chars(50,5)
{
	AddMIME("text/rtf");
	AddMIME("application/rtf");
	InitCharContainer();
	Tags=new RContainer<Tag,true,true>(10,5);

	//document Headers
	Tags->InsertPtr(new Tag("\\rtf",Tag::tMAIN));
	Tags->InsertPtr(new Tag("\\fonttbl",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\colortbl",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\stylesheet",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\listtable",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\listoverridetable",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\info",Tag::tMETA));
	Tags->InsertPtr(new Tag("\\listtable",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\listtable",Tag::tSKIP));

	//tag for info part
	Tags->InsertPtr(new Tag("\\author",Tag::tAUTHOR));

	//Tags->InsertPtr(new Tag("\\operator","",Tag::tOPERATOR,false,8,false));
	Tags->InsertPtr(new Tag("\\title",Tag::tTITLE));
	Tags->InsertPtr(new Tag("\\subject",Tag::tSUBJECT));
	Tags->InsertPtr(new Tag("\\company",Tag::tPUBLI));
	Tags->InsertPtr(new Tag("\\keywords",Tag::tDESCRIPT));
	Tags->InsertPtr(new Tag("\\creatim",Tag::tDATE));

	//Header & footer display
	Tags->InsertPtr(new Tag("\\header",Tag::tSKIP));
	Tags->InsertPtr(new Tag("\\footer",Tag::tSKIP));

	//bullet and numbering info
	Tags->InsertPtr(new Tag("\\pnseclvl",Tag::tSKIP));

	//Text content
	Tags->InsertPtr(new Tag("\\par",Tag::tPAR));
	Tags->InsertPtr(new Tag("\\pard",Tag::tPAR));

	Tags->InsertPtr(new Tag("\\fs20",Tag::tTEXT));
}


//------------------------------------------------------------------------------
GFilterRTF::Tag* GFilterRTF::FindTag(RString str)
{
	RString curTag("");
	RCharCursor strCur(str);
	Tag* t;
	t=0;
	bool begFound=false;
	bool endFound=false;

	//Find the first possible tag if tag begin with \* skiop these chars
	for(strCur.Start();!strCur.End();strCur.Next())
	{
		if(begFound)
		{
			if((strCur()=='\\')||(strCur()=='{')||(strCur()==' '))
			{
				endFound=true;
				t=Tags->GetPtr<const RString>(curTag);
				return (t);
			}
			else
			{
				if((strCur()=='*')&&(curTag=="\\"))
				{
					begFound=false;
					curTag="";
				}
				else
				{
					curTag+=strCur();
					t=Tags->GetPtr<const RString>(curTag);
					if(t)
						return (t);
				}
			}
		}
		else
			if(strCur()=='\\')
			{
				begFound=true;
				curTag+=strCur();
			}

	}

	//IF tag is nto found in for loop return NULL
	return t;
}


//------------------------------------------------------------------------------
void GFilterRTF::AnalyzeMeta(RString str, Tag* t)
{
	RString date("");

	int id=str.FindStr(t->Name);
	if(id<0) return;
	id+=t->Name.GetLen();

	switch(t->Type)
	{
		case Tag::tTITLE :
			AddDublinCoreMetaData("title",str.Mid(id));
			break;

		case Tag::tAUTHOR :
			AddDublinCoreMetaData("author",str.Mid(id));
			break;

		case Tag::tSUBJECT :
			AddDublinCoreMetaData("subject",str.Mid(id));
			break;

		case Tag::tPUBLI :
			AddDublinCoreMetaData("publisher",str.Mid(id));
			break;

		case Tag::tDESCRIPT :
			AddDublinCoreMetaData("subject",str.Mid(id),GFilter::Keywords,' ');
			break;

		case Tag::tDATE :
			unsigned int idValD,idValE;
			//find year
			idValD=str.FindStr("\\yr")+3;
			idValE=str.Find('\\',idValD);
			date+=str.Mid(idValD,idValE-idValD);
			date+="-";

			//Find month
			idValD=str.FindStr("\\mo")+3;
			idValE=str.Find('\\',idValD);
			date+=str.Mid(idValD,idValE-idValD);
			date+="-";

			//Find day
			idValD=str.FindStr("\\dy")+3;
			idValE=str.Find('\\',idValD);
			date+=str.Mid(idValD,idValE-idValD);
			AddDublinCoreMetaData("date",str.Mid(id));
			break;

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GFilterRTF::AnalyzeText(RString str)
{
	RString par("");
	RString readTag("");

	Tag* t;
	RCharCursor cur(str);
	bool tagFound=false;

	str=ReplaceCodes(str);
	for(cur.Start();!cur.End();cur.Next())
	{
		if(!(cur()=='\\')&& !((cur()==' ')&(tagFound)))
		{
			if(!tagFound)
				par+=cur();
			else
				readTag+=cur();
		}
		if(cur()=='\\')
		{
			if(!readTag.IsEmpty())
			{
				t=Tags->GetPtr<const RString>(readTag);
				if((t)&&(t->Type==Tag::tPAR)&&(!par.IsEmpty()))
				{
					StartParagraph(Parser);
					AnalyzeBlock(par,Parser);
					EndParagraph(Parser);
					par="";
				}
			}
			readTag="";
			readTag+=cur();
			tagFound=true;
		}
		if((tagFound)&&(cur()==' '))
		{
			if(!readTag.IsEmpty())
			{
				t=Tags->GetPtr<const RString>(readTag);
				if((t)&&(t->Type==Tag::tPAR)&&(!par.IsEmpty()))
				{
					StartParagraph(Parser);
					AnalyzeBlock(par,Parser);
					EndParagraph(Parser);
					par="";
				}
			}
			readTag="";
			tagFound=false;
		}
	}

	//test If some text has not been added yet to structure
	if(!par.IsEmpty())
	{
		StartParagraph(Parser);
		AnalyzeBlock(par,Parser);
		EndParagraph(Parser);
	}
}


//------------------------------------------------------------------------------
void GFilterRTF::FindBlock(RString str,bool text)
{
	Tag* t;
	unsigned int openedPos,nbOpened,nbClosed;
	RCharCursor strCur(str);
	bool first=true;
	bool found=false;
	RString curTag("");

	openedPos=nbOpened=nbClosed=0;

	for(strCur.Start();!strCur.End();strCur.Next())
	{
		if(strCur()=='{')
		{
			if(first)
			{
				first=false;
				openedPos=strCur.GetPos();
			}
			nbOpened++;
		}
		if(strCur()=='}')
		{
			nbClosed++;
		}
		if((!first)&&(nbOpened==nbClosed))
		{
			unsigned int endPos=0;
			unsigned int test;
			endPos=strCur.GetPos();
			test=endPos-openedPos;
			found=true;
			if(text)
			{
				if(openedPos>1)
					AnalyzeText(str.Mid(0,openedPos-1));

				FindBlock(str.Mid(openedPos+1,endPos-openedPos-1),true);
				AnalyzeText(str.Mid(endPos+1));
			}
			//Recursif on the sub block
			t=FindTag(str.Mid(openedPos+1,endPos-openedPos-1));
			if(!t)
			{
				FindBlock(str.Mid(openedPos+1,endPos-openedPos-1));
			}
			else
			{
				if(!(t->Type==Tag::tSKIP))
				{
					if(t->Type==Tag::tTEXT)
					{
						FindBlock(str.Mid(openedPos+1,endPos-openedPos-1),true);
					}
					else
					{
						if((t->Type==Tag::tTITLE)||(t->Type==Tag::tAUTHOR)||(t->Type==Tag::tSUBJECT)||(t->Type==Tag::tPUBLI)||(t->Type==Tag::tDESCRIPT)||(t->Type==Tag::tDATE))
						{
							AnalyzeMeta(str.Mid(openedPos+1,endPos-openedPos-1),t);
						}
						FindBlock(str.Mid(openedPos+1,endPos-openedPos-1));
					}
				}
			}
			first=true;
		}
	}
	if((!found)&&(text))
	{
		//text has not been treated yet.
		AnalyzeText(str);
	}
}


//------------------------------------------------------------------------------
void GFilterRTF::Analyze(GDoc* doc,const RURI& uri,RXMLParser* parser,GSlot*)
{
	// Init Part
	StartStream(parser);
	RTextFile Src(uri);
	Src.Open(R::RIO::Read);

	// Create the metaData tag and the first information
	AddDublinCoreMetaData("identifier",doc->GetURL()());
	WriteMetaDataStream(parser);
	Parser=parser;
	FindBlock(Src.GetUntilEnd());
	EndStream(parser);
}


//------------------------------------------------------------------------------
void GFilterRTF::InitCharContainer(void)
{
	#warning Automatic code extraction must be possible
	Chars.InsertPtr(new CodeToChar("\\u224\\\'00\\\'E0",0x00E0));
	Chars.InsertPtr(new CodeToChar("\\u225\\\'00\\\'E1",0X00E1));
	Chars.InsertPtr(new CodeToChar("\\u226\\\'00\\\'E2",0x00E2));
	Chars.InsertPtr(new CodeToChar("\\u228\\\'00\\\'E4",0x00E4));
	Chars.InsertPtr(new CodeToChar("\\u230\\\'00\\\'E6",0x00E6));
	Chars.InsertPtr(new CodeToChar("\\u231\\\'00\\\'E7",0x00E7));
	Chars.InsertPtr(new CodeToChar("\\u232\\\'00\\\'E8",0x00E8));
	Chars.InsertPtr(new CodeToChar("\\u233\\\'00\\\'E9",0x00E9));
	Chars.InsertPtr(new CodeToChar("\\u234\\\'00\\\'EA",0x00EA));
	Chars.InsertPtr(new CodeToChar("\\u235\\\'00\\\'EB",0x00EB));
	Chars.InsertPtr(new CodeToChar("\\u236\\\'00\\\'EC",0x00EC));
	Chars.InsertPtr(new CodeToChar("\\u237\\\'00\\\'ED",0x00ED));
	Chars.InsertPtr(new CodeToChar("\\u238\\\'00\\\'EE",0x00EE));
	Chars.InsertPtr(new CodeToChar("\\u239\\\'00\\\'EF",0x00EF));
	Chars.InsertPtr(new CodeToChar("\\u242\\\'00\\\'F2",0x00F2));
	Chars.InsertPtr(new CodeToChar("\\u243\\\'00\\\'F3",0x00F3));
	Chars.InsertPtr(new CodeToChar("\\u244\\\'00\\\'F4",0x00F4));
	Chars.InsertPtr(new CodeToChar("\\u246\\\'00\\\'F6",0x00F6));
	Chars.InsertPtr(new CodeToChar("\\u249\\\'00\\\'F9",0x00F9));
	Chars.InsertPtr(new CodeToChar("\\u250\\\'00\\\'FA",0x00FA));
	Chars.InsertPtr(new CodeToChar("\\u251\\\'00\\\'FB",0x00FB));
	Chars.InsertPtr(new CodeToChar("\\u253\\\'00\\\'FD",0x00FD));
	Chars.InsertPtr(new CodeToChar("\\\'e0",0x00E0));
	Chars.InsertPtr(new CodeToChar("\\\'e1",0X00E1));
	Chars.InsertPtr(new CodeToChar("\\\'e2",0x00E2));
	Chars.InsertPtr(new CodeToChar("\\\'e4",0x00E4));
	Chars.InsertPtr(new CodeToChar("\\\'e6",0x00E6));
	Chars.InsertPtr(new CodeToChar("\\\'e7",0x00E7));
	Chars.InsertPtr(new CodeToChar("\\\'e8",0x00E8));
	Chars.InsertPtr(new CodeToChar("\\\'e9",0x00E9));
	Chars.InsertPtr(new CodeToChar("\\\'ea",0x00EA));
	Chars.InsertPtr(new CodeToChar("\\\'eb",0x00EB));
	Chars.InsertPtr(new CodeToChar("\\\'ec",0x00EC));
	Chars.InsertPtr(new CodeToChar("\\\'ed",0x00ED));
	Chars.InsertPtr(new CodeToChar("\\\'ee",0x00EE));
	Chars.InsertPtr(new CodeToChar("\\\'ef",0x00EF));
	Chars.InsertPtr(new CodeToChar("\\\'f2",0x00F2));
	Chars.InsertPtr(new CodeToChar("\\\'f3",0x00F3));
	Chars.InsertPtr(new CodeToChar("\\\'f4",0x00F4));
	Chars.InsertPtr(new CodeToChar("\\\'f6",0x00F6));
	Chars.InsertPtr(new CodeToChar("\\\'f9",0x00F9));
	Chars.InsertPtr(new CodeToChar("\\\'fa",0x00FA));
	Chars.InsertPtr(new CodeToChar("\\\'fb",0x00FB));
	Chars.InsertPtr(new CodeToChar("\\\'fd",0x00FD));
	Chars.InsertPtr(new CodeToChar("\\\'E0",0x00E0));
	Chars.InsertPtr(new CodeToChar("\\\'E1",0x00E1));
	Chars.InsertPtr(new CodeToChar("\\\'E2",0x00E2));
	Chars.InsertPtr(new CodeToChar("\\\'E4",0x00E4));
	Chars.InsertPtr(new CodeToChar("\\\'E6",0x00E6));
	Chars.InsertPtr(new CodeToChar("\\\'E7",0x00E7));
	Chars.InsertPtr(new CodeToChar("\\\'E8",0x00E8));
	Chars.InsertPtr(new CodeToChar("\\\'E9",0x00E9));
	Chars.InsertPtr(new CodeToChar("\\\'EA",0x00EA));
	Chars.InsertPtr(new CodeToChar("\\\'EB",0x00EB));
	Chars.InsertPtr(new CodeToChar("\\\'EC",0x00EC));
	Chars.InsertPtr(new CodeToChar("\\\'ED",0x00ED));
	Chars.InsertPtr(new CodeToChar("\\\'EE",0x00EE));
	Chars.InsertPtr(new CodeToChar("\\\'EF",0x00EF));
	Chars.InsertPtr(new CodeToChar("\\\'F2",0x00F2));
	Chars.InsertPtr(new CodeToChar("\\\'F3",0x00F3));
	Chars.InsertPtr(new CodeToChar("\\\'F4",0x00F4));
	Chars.InsertPtr(new CodeToChar("\\\'F6",0x00F6));
	Chars.InsertPtr(new CodeToChar("\\\'F9",0x00F9));
	Chars.InsertPtr(new CodeToChar("\\\'FA",0x00FA));
	Chars.InsertPtr(new CodeToChar("\\\'FB",0x00FB));
	Chars.InsertPtr(new CodeToChar("\\\'FD",0x00FD));
	Chars.InsertPtr(new CodeToChar("\\rquote",0x0027));
	Chars.InsertPtr(new CodeToChar("\\rdblquote",0x0022));
	Chars.InsertPtr(new CodeToChar("\\RQUOTE",0x0027));
	Chars.InsertPtr(new CodeToChar("\\RDBLQUOTE",0x0022));
	Chars.InsertPtr(new CodeToChar("\\uc",' '));
	Chars.InsertPtr(new CodeToChar("\\uc1",' '));
	Chars.InsertPtr(new CodeToChar("\\uc2",' '));
	Chars.InsertPtr(new CodeToChar("\\uc3",' '));
}


//------------------------------------------------------------------------------
RString GFilterRTF::ReplaceCodes(RString str)
{
	RCharCursor cur(str);
	R::RCursor<CodeToChar> codeCur;
	RString code("");
	RString resStr("");
	CodeToChar *c;
	int curPos=0;
	int posS;
	posS=0;

	while(true)
	{
		curPos=str.Find('\\',curPos);
		if(curPos<0) {resStr+=str.Mid(posS); return resStr;}
		code=str.Mid(curPos,13);

		c=Chars.GetPtr<const RString>(code);
		if(c)
		{
			resStr+=str.Mid(posS,curPos-posS);
			resStr+=c->GetChar();
			curPos+=12;
			posS=curPos+1;
		}
		else
		{
			code=str.Mid(curPos,4);
			c=Chars.GetPtr<const RString>(code);
			if(c)
			{
				resStr+=str.Mid(posS,curPos-posS);
				resStr+=c->GetChar();
				curPos+=3;
				posS=curPos+1;
			}
			else
			{
				codeCur.Set(Chars);
				int id;
				for(codeCur.Start();!codeCur.End();codeCur.Next())
				{
					id=-1;
					str.FindStr(codeCur()->GetCode(),curPos);
					if(id>=0)
					{
						resStr+=str.Mid(posS,curPos-posS);
						resStr+=codeCur()->GetChar();
						curPos+=codeCur()->GetLen();
						posS=curPos+1;
					}
				}
			}
		}
		curPos++;
	}
}


//------------------------------------------------------------------------------
void GFilterRTF::CreateParams(GPlugInFactory*)
{
}


//------------------------------------------------------------------------------
GFilterRTF::~GFilterRTF()
{
	if(Tags) delete Tags;
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("RTF Filter","RTF Filter",GFilterRTF)

