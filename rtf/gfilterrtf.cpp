/*

	GALILEI Research Project

	GFilterRTF.cpp

	A RTF filter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Kumps Nicolas (nkumps@ulb.ac.be)

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
#include <gfilterrtf.h>
#include <docs/gfiltermanager.h>
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFilterRTF
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GALILEI::GFilterRTF::GFilterRTF(GFactoryFilter* fac)
	: GFilter(fac), Tags(0),
	 Chars(50,5)
{
	AddMIME("text/rtf");
	InitCharContainer();
	Tags=new RContainer<Tag,unsigned int,true,true>(10,5);

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
void GFilterRTF::AnalyseMeta(RString str, Tag* t)
{
	int id=str.FindStr(t->Name);
	if(id<0) return;
	id+=t->Name.GetLen();

	switch(t->Type)
	{
		case Tag::tTITLE :
			AnalyzeBlock(str.Mid(id),Doc->AddTitle());
			break;

		case Tag::tAUTHOR :
			AnalyzeBlock(str.Mid(id),Doc->AddCreator());
			break;

		case Tag::tSUBJECT :
			AnalyzeBlock(str.Mid(id),Doc->AddSubject());
			break;

		case Tag::tPUBLI :
			AnalyzeBlock(str.Mid(id),Doc->AddPublisher());
			break;

		case Tag::tDESCRIPT :
			AnalyzeKeywords(str.Mid(id),' ',Doc->AddSubject());
			break;

		case Tag::tDATE :
			unsigned int idValD,idValE;
			RString date("");
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
			AnalyzeBlock(date,Doc->AddDate());
			break;
	}
}


//------------------------------------------------------------------------------
void GFilterRTF::AnalyseText(R::RString str)
{
	RString par("");
	RString readTag("");

	Tag* t;
	RXMLTag* part;
	RXMLTag* tag;
	RCharCursor cur(str);
	bool tagFound=false;

	str=ReplaceCodes(str);
	part = Doc->GetContent();
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
					Doc->AddTag(part,tag=new RXMLTag("docxml:p"));
					AnalyzeBlock(par,tag);
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
					Doc->AddTag(part,tag=new RXMLTag("docxml:p"));
					AnalyzeBlock(par,tag);
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
		Doc->AddTag(part,tag=new RXMLTag("docxml:p"));
		AnalyzeBlock(par,tag);
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
					AnalyseText(str.Mid(0,openedPos-1));

				FindBlock(str.Mid(openedPos+1,endPos-openedPos-1),true);
				AnalyseText(str.Mid(endPos+1));
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
							AnalyseMeta(str.Mid(openedPos+1,endPos-openedPos-1),t);
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
		//text has not been treatde yet.
		AnalyseText(str);
	}
}

//------------------------------------------------------------------------------
bool GFilterRTF::Analyze(GDocXML* doc) throw(bad_alloc,GException)
{
	RXMLTag* part;
	bool Stop;
	bool Read;
	RString NextLine;
	RString Line;
	RString Text;

	try
	{
		// Init Part
		Doc=doc;
		RTextFile Src(Doc->GetFile());
		Stop=Src.Eof();

		// Create the metaData tag and the first information
		part=Doc->GetMetaData();
		Doc->AddIdentifier(Doc->GetURL());

		//Treat Content
		part=Doc->GetContent();
		Read=false;
		Text = Src.GetUntilEnd();

		FindBlock(Text);
	}
	catch(bad_alloc)
	{
		throw;
	}
	catch(GException)
	{
		throw;
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(...)
	{
		throw GException("Unexcepted exception");
	}

	Doc->GetContent()->DeleteEmptyTags(Doc);
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
	Chars.InsertPtr(new CodeToChar("\\uc",' '));
	Chars.InsertPtr(new CodeToChar("\\uc1",' '));
	Chars.InsertPtr(new CodeToChar("\\uc2",' '));
	Chars.InsertPtr(new CodeToChar("\\uc3",' '));
}


//------------------------------------------------------------------------------
RString GALILEI::GFilterRTF::ReplaceCodes(RString str)
{
	RCharCursor cur(str);
	CodeToCharCursor codeCur;
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

