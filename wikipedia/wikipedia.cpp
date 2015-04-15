/*

	Wikipedia Plug-Ins

	Wikipedia.cpp

	Build a GALILEI database for Wikipedia - Header

	Copyright 2012 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <gslot.h>
#include <gdoc.h>
#include <gprofile.h>
#include <guser.h>
#include <gfdbk.h>
#include <gsession.h>
#include <ggalileiapp.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <wikipedia.h>
#include <wikidumparticles.h>


//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;
static RTextFile* Test(0);



//------------------------------------------------------------------------------
//
// class Wikipedia
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Wikipedia::Wikipedia(GSession* session,GPlugInFactory* fac)
	: GTool(session,fac), LangEn(0), CurTag("ThisIsASampleTagByPascalFrancq")
{
	if(Debug)
	{
		Test=new RTextFile("/home/pfrancq/test.txt","utf-8");
		Test->Open(RIO::Create);
	}
	Line.SetLen(2048); // Set a buffer size of 2048
	Content.SetLen(1048576); // Set the buffer size to 1 Mb of words.
}


//-----------------------------------------------------------------------------
void Wikipedia::ApplyConfig(void)
{
	Dir=FindParam<RParamValue>("Dir")->Get();
	NbArticles=FindParam<RParamValue>("NbArticles")->GetUInt();
	NbContributions=FindParam<RParamValue>("NbContributions")->GetUInt();
	ForceAnalyze=FindParam<RParamValue>("ForceAnalyze")->GetBool();
	try
	{
		Dump.SetDate(FindParam<RParamValue>("Dump")->Get());
	}
	catch(...)
	{
		Dump.SetToday();
	}
	DumpStr=RString::Number(Dump.GetYear());
	if(Dump.GetMonth()<10)
		DumpStr+="0";
	DumpStr+=RString::Number(Dump.GetMonth());
	if(Dump.GetDay()<10)
		DumpStr+="0";
	DumpStr+=RString::Number(Dump.GetDay());
	GTool::ApplyConfig();
}


//------------------------------------------------------------------------------
void Wikipedia::Run(GSlot* slot)
{
	Slot=slot;
	Slot->StartJob("Import Wikipedia Articles");
	if(!LangEn)
		LangEn=GALILEIApp->GetPlugIn<GLang>("Lang","en");
	NbTreatedArticles=NbAnalyzedArticles=NbFdbks=0;
	if(NbArticles)
		AnalyzePages();
	Slot->StartJob("Import Wikipedia Contributions");
	if(NbContributions)
		AnalyzeLogs();
	Slot->EndJob(RString::Number(NbTreatedArticles)+" documents ("+RString::Number(NbAnalyzedArticles)+" were analyzed) and "+RString::Number(NbFdbks)+" contributions imported.");
}


//------------------------------------------------------------------------------
void Wikipedia::AnalyzePages(void)
{
	RChar Cur;
	RString Cmd;
	bool ReadCmd(true);

	// Initialization
	InPage=false;
	InText=false;
	NbLines=0;
	NbTreatedArticles=0;

	// Open the file
	WikiDumpArticles Test2(this,Dir+RFile::GetDirSeparator()+"enwiki-"+DumpStr+"-pages-articles.xml");
	Test2.Open();
	return;
	RTextFile Articles(Dir+RFile::GetDirSeparator()+"enwiki-"+DumpStr+"-pages-articles.xml","utf-8");
	Articles.Open(RIO::Read);
	while(!Articles.End())
	{
		// Read character
		Cur=Articles.GetChar();

		// Depends whenever we are in a article or not
		if(InPage)
		{
			if(ReadCmd)
			{
				if(Cur=='>')
				{
					// Look if it is the begin of a page
					if(Cmd=="/page")
					{
						InPage=false;
						if(Debug)
						 (*Test)<<Content<<std::endl<<std::endl<<std::endl<<std::endl;
					}
					else
						Content+='<'+Cmd+'>';
					ReadCmd=false;
				}
				else
				{
					Cmd+=Cur;
				}
			}
			else
			{
				if(Cur=='<')
				{
					Cmd.Clear();
					ReadCmd=true;
				}
				else
					Content+=Cur;
			}
		}
		else
		{
			if(ReadCmd)
			{
				if(Cur=='>')
				{
					// Look if it is the begin of a page
					if(Cmd=="page")
					{
						InPage=true;
						Content.Clear();
						NbTreatedArticles++;
					}
					ReadCmd=false;
				}
				else
				{
					Cmd+=Cur;
				}
			}
			else
			{
				if(Cur=='<')
				{
					Cmd.Clear();
					ReadCmd=true;
				}
			}
		}


		// Verify if number of articles are read
		if(NbArticles&&(NbTreatedArticles>NbArticles))
			break;
	}
}


//------------------------------------------------------------------------------
//void Wikipedia::AnalyzePages(void)
//{
//	// Initialization
//	InPage=false;
//	InText=false;
//	NbLines=0;
//
//	RTextFile Articles(Dir+RFile::GetDirSeparator()+"enwiki-"+DumpStr+"-pages-articles.xml","utf-8");
//	//RTextFile Articles("/home/pfrancq/notice.txt","utf-8");
//	Articles.Open(RIO::Read);
//	while(!Articles.End())
//	{
//		Act=Articles.GetPos();
//		Articles.GetLine(Line);
//		NbLines++;
//		off_t Rest(NbLines%5000);
//		if(!Rest)
//		{
//			if(NbLines/1000000000>0)
//				Tmp=RString::Number(NbLines/1000000000)+"T";
//			else if(NbLines/1000000>0)
//				Tmp=RString::Number(NbLines/1000000)+"G";
//			else
//				Tmp=RString::Number(NbLines/1000)+"M";
//
//			Slot->StartJob("Import Wikipedia Articles ("+RString::Number(NbTreatedArticles)+","+Tmp+")");
//		}
//		if(InPage)
//		{
//			AnalyzeInPage();
//		}
//		else
//		{
//			if(Line.Begins("<page>",true))
//			{
//				InPage=true;
//				if(!Redirect)
//					NbTreatedArticles++;
//				Redirect=false;
//				WikiTokens::Get()->Clear();
//				Id=0;
//				Article=0;
//				Rest=NbTreatedArticles%100;
//				if(!Rest)
//				{
//					if(NbLines/1000000000>0)
//						Tmp=RString::Number(NbLines/1000000000)+"T";
//					else if(NbLines/1000000>0)
//						Tmp=RString::Number(NbLines/1000000)+"G";
//					else
//						Tmp=RString::Number(NbLines/1000)+"M";
//
//					Slot->StartJob("Import Wikipedia Articles ("+RString::Number(NbTreatedArticles)+","+Tmp+")");
//				}
//			}
//		}
//		if(NbArticles&&(NbTreatedArticles>NbArticles))
//			break;
//	}
//}


//------------------------------------------------------------------------------
void Wikipedia::AnalyzeInPage(void)
{
	if(InText)
	{
		if(Line.Ends("</text>",true))
		{
			InText=false;

			// Look for the good position
			Size=Act+Line.GetLen();
			const RChar* Car(&Line()[Line.GetLen()-1]);
			size_t PosCar(Line.GetLen());
			while(PosCar&&((*Car)!='<'))
			{
				Car--;
				Size--;
				PosCar--;
			}
			Size-=Pos-1; // Skip '<'

			// Remove all the last links where an outside Wikipedia
			size_t ToDel(0);
			WikiToken* Last(0);
			RCursor<WikiToken> Del(WikiTokens::Get()->GetTokens());
			for(Del.StartFromEnd();!Del.Begin();Del.Prev())
			{
				if(Del()->Type!=WikiToken::Link)
				{
					if(Last)
						break;
					Last=Del();
				}
				else
				{
					// Point to english
					if(!RChar::StrNCmp(&Del()->Content()[7],"en",2))
						break;

					// Not wikipedia

					if(RChar::StrNCmp(&Del()->Content()[10],"wikipedia",9)&&RChar::StrNCmp(&Del()->Content()[11],"wikipedia",9))
						break;

					ToDel+=2;
					Last=0;
				}
			}
			for(ToDel++;--ToDel;)
				WikiTokens::Get()->DeleteLastToken();

			// If necessary -> print the tokens
			if(Article&&Debug)
			{
				(*Test)<<Article->GetURI()()<<endl;
				RCursor<WikiToken> Cur(WikiTokens::Get()->GetTokens());
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					for(size_t i=Cur()->Depth+2;--i;)
						(*Test)<<"\t";
					switch(Cur()->Type)
					{
						case WikiToken::Text:
							(*Test)<<"T ";
							break;
						case WikiToken::Section:
							(*Test)<<"S ";
							break;
						case WikiToken::Title:
							(*Test)<<"M ";
							break;
						case WikiToken::Link:
							(*Test)<<"L ";
							break;
					}
					(*Test)<<Cur()->Depth<<" "<<Cur()->SyntacticPos<<" "<<Cur()->Pos<<"\t"<<Cur()->Content<<endl;
				}
			}

			if(Article->MustCompute()||ForceAnalyze)
			{
				Session->AnalyzeDoc(Article);
				NbTreatedArticles++;
			}
		}
		else
			TreatWiki();
	}
	else
	{
		if(Line.Ends("</page>",true))
		{
			InPage=false;
			WikiTokens::Get()->SetDoc(0);
		}
		else if(Line.Begins("<redirect",true))
			Redirect=true;
		else if(Line.Begins("<title>",true))
		{
			ExtractFromTag(Line,ArticleTitle);
		}
		else if(Line.Begins("<timestamp>",true))
		{
			ExtractFromTag(Line,Tmp);
			ArticleUpdated.SetDate(Tmp);
		}
		else if((!Redirect)&&Line.Begins("<text",true)&&(!Line.Ends("</page>",true)))
		{
			State=sText;
			LookDoc(true);

			// Look if the Wiki must be analyzed
			if(Status!=OK)
			{
				InText=true;

				// Create a title
				WikiToken* Token(WikiTokens::Get()->CreateToken());
				Token->Type=WikiToken::Title;
				Token->Pos=0;
				Token->SyntacticPos=0;
				Token->Content=ArticleTitle;
				Token->Depth=0;

				// Look for the good position
				Pos=Act;
				const RChar* Car(Line());
				while(((*Car)!=0)&&((*Car)!='>'))
				{
					Car++;
					Pos++;
				}
				Pos++; // Skip '>'

				CurPos=0;
				CurSyntacticPos=1;
				CurSection=0;
			}
		}
		else if((!Id)&&Line.Begins("<id>",true))
		{
			ExtractFromTag(Line,Tmp);
			Id=Tmp.ToSizeT();
		}
	}
}


//------------------------------------------------------------------------------
bool TestTag(const RChar* car,char c1,char c2)
{
	if((*car)==c1)
	{
		car++;
		if((!car->IsNull())&&((*car)==c2))
			return(true);
		else
			return(false);
	}
	else
		return(false);
}


//------------------------------------------------------------------------------
bool Wikipedia::CreateLink(void)
{
	if(NbBrackets||(State!=sLink)||(CurLink.Content.IsEmpty()))
		return(false);

	WikiToken* Token;

	// Look for a namespace
	RString Namespace;
	int i(CurLink.Content.Find(':'));
	if(i==-1)
		Namespace="http://en.wikipedia.org/wiki/";  // Default namespace
	else
	{
		// Look for a language
		Namespace=CurLink.Content.Mid(0,i);
		RString Lang;
		int j(CurLink.Content.Find(':',i+1));
		if(j==-1)
		{
			Lang="en"; // No language
			CurLink.Content=CurLink.Content.Mid(i+1);
		}
		else
		{
			Lang=CurLink.Content.Mid(i+1,j-i);
			CurLink.Content=CurLink.Content.Mid(j+1);
		}

		if((Namespace.GetLen()==2)||(Namespace.GetLen()==3))
			Namespace="http://"+Namespace+".wikipedia.org/wiki/";  // It is a language
		else if(Namespace=="Wikipedia")
			Namespace="http://"+Lang+".wikipedia.org/wiki/";
		else if(Namespace=="Category")
			Namespace="http://"+Lang+".wikipedia.org/wiki/Category:";
		else if(Namespace=="Wiktionary")
			Namespace="http://"+Lang+".wiktionary.org/wiki/";
		else return(false);
	}

	if(CurLink.Content[0]=='#')
		return(false);  // Do not treat internal links

	// Look for another name for the link
	i=CurLink.Content.Find('|');

	// Create the link
	Token=WikiTokens::Get()->CreateToken();
	Token->Type=WikiToken::Link;
	Token->Pos=CurLink.Pos;
	Token->SyntacticPos=CurSyntacticPos++;
	if(CurSection)
		Token->Depth=CurSection->Depth+1;

	// In fact, there is a link and a text content
	WikiToken* Text=WikiTokens::Get()->CreateToken();
	Text->Type=WikiToken::Text;
	Text->SyntacticPos=CurSyntacticPos++;
	if(CurSection)
		Text->Depth=CurSection->Depth+1;

	// Build the content depending of the presence of |
	if(i!=-1)
	{
		Token->Content=Namespace+WikiToken::GetWikiTitle(CurLink.Content.Mid(0,i));
		Text->Pos=CurLink.Pos+i+3;
		Text->Content=CurLink.Content.Mid(i+1);
		if(Text->Content.IsEmpty()||Text->Content==" ")
			Text->Content=CurLink.Content.Mid(0,i);
	}
	else
	{
		Token->Content=Namespace+WikiToken::GetWikiTitle(CurLink.Content);
		Text->Pos=CurLink.Pos+2;
		Text->Content=CurLink.Content;
	}

	return(true);
}


//------------------------------------------------------------------------------
void Wikipedia::TreatWiki(void)
{
	WikiToken* Token;
	RChar Code[20];

	if((State==sText)&&Line.Begins("=="))
	{
		// Create the token
		CurSection=Token=WikiTokens::Get()->CreateToken();
		CurSection->Type=WikiToken::Section;
		CurSection->Pos=CurPos;
		CurSection->SyntacticPos=CurSyntacticPos++;

		// A Title
		const RChar* ptr(Line());
		ptr+=2; // Skip '=='
		while((!ptr->IsNull())&&((*ptr)=='='))
		{
			CurSection->Depth++;
			ptr++; CurPos++; // Skip '='
		}

		// Read The title
		while((!ptr->IsNull())&&((*ptr)!='='))
		{
			CurSection->Content+=(*ptr);
			ptr++; CurPos++; // Next Character
		}
	}
	else
	{
		// Normal text
		const RChar* ptr(Line());
		WikiToken* Token(0);

		while(!ptr->IsNull())
		{
			if(State==sText)
			{
				// Look what it is the current token to treat
				if(ptr->IsSpace())
				{
					Token=0;  // Terminate a token
					ptr++; CurPos++;  // Skip the space
				}
				else if(TestTag(ptr,'[','['))
				{
					// This is a link
					Token=0;
					State=sLink;
					BracketCar1=']';
					BracketCar2=']';
					NbBrackets=1;

					CurLink.Clear();
					ptr+=2; CurPos+=2; // Skip '[['

					// If 'File:' -> Skip it
					if(!RChar::StrNCmp(ptr,"File:",5))
					{
						ptr+=5; CurPos+=5; // Skip 'File:'
						CurLink.Pos=0;
						IncludedLinks=true;
					}
					else
					{
						IncludedLinks=false;
						CurLink.Pos=CurPos;
					}
				}
				else if(TestTag(ptr,'{','{'))
				{
					// This is a citation
					Token=0;
					State=sCite;
					BracketCar1='}';
					BracketCar2='}';
					NbBrackets=1;

					// Test {{Main
					CurLink.Clear();
					if(!RChar::StrNCmp(ptr+2,"Main",4))
					{
						CurLink.Pos=CurPos;
						CurLink.SyntacticPos=CurSyntacticPos++;
					}

					ptr+=2; CurPos+=2;  // Skip the two characters
				}
				else if(TestTag(ptr,'{','|'))
				{
					// This is a table
					Token=0;
					State=sTable;
					BracketCar1='|';
					BracketCar2='}';
					NbBrackets=1;

					CurLink.Clear();
					CurLink.Pos=CurPos;
					ptr+=2; CurPos+=2;  // Skip the two characters

				}
				else if((*ptr)=='&')
				{
					// This a HTML-like code
					Token=0;  // Terminate a token
					ptr++; CurPos++;   // Skip '&'

					// Look for the code
					RChar* Hold=Code;
					while((!ptr->IsNull())&&((*ptr)!=';'))
					{
						(*(Hold++))=(*(ptr++));
						CurPos++;
					}
					(*Hold)=0;

					if(!ptr->IsNull())
					{
						ptr++; CurPos++;   // Skip ';'
					}

					// Look for '<'
					if((!ptr->IsNull())&&(!RChar::StrCmp(Code,"lt")))
					{
						if(!RChar::StrNCmp(ptr,"ref",3))      // Look if we have '<ref'
							State=sRef;
						else if(!RChar::StrNCmp(ptr,"!--",3)) // Look if we have '<!--'
							State=sComment;
						else if(!RChar::StrNCmp(ptr,"div",3))      // Look if we have '<div'
							State=sDiv;
						else
						{
							// Find which tag and put it in CurTag
							State=sTag;
							CurTag="&lt;/";
							while((!ptr->IsNull())&&(!ptr->IsSpace())&&((*ptr)!='&')&&((*ptr)!='/'))
							{
								CurTag+=(*ptr);
								ptr++; CurPos++;   // Next character
							}
						}

						if(State!=sComment)
						{
							// Look if the closing '>' ('&gt;' is preceeded by a '/'.
							// If so -> return to the textual State
							RChar LastCar(0);
							while((!ptr->IsNull())&&(RChar::StrNCmp(ptr,"&gt;",4)))
							{
								LastCar=(*ptr);
								ptr++; CurPos++;   // Next character
							}
							if(!ptr->IsNull())
							{
								ptr+=4; CurPos+=4;   // Skip '&gt;'
								if(LastCar==RChar('/'))
									State=sText;
							}
						}
					}
				}
				else if((*ptr)=='[')
				{
					// Link ?
					Token=0;  // Terminate a token
					ptr++; CurPos++;   // Skip '&'

					if(  (!RChar::StrNCmp(ptr,"http",4))  &&  ( ((*(ptr+4))==':') || (((*(ptr+4))=='s')&&((*(ptr+5))==':')) )  )
					{
						// Create the link
						Token=WikiTokens::Get()->CreateToken();
						Token->Type=WikiToken::Link;
						Token->Pos=CurPos;
						Token->SyntacticPos=CurSyntacticPos++;
						if(CurSection)
							Token->Depth=CurSection->Depth+1;

						while((!ptr->IsNull())&&(!ptr->IsSpace())&&((*ptr)!=']'))
						{
							Token->Content+=(*ptr);
							ptr++; CurPos++;   // Next character
						}
					}

				}
				else if(ptr->IsPunct())
				{
					Token=0;  // Terminate a token
					ptr++; CurPos++;   // Skip the punctation
				}
				else
				{
					// Normal Token
					if(!Token)
					{
						Token=WikiTokens::Get()->CreateToken();
						Token->Type=WikiToken::Text;
						Token->Pos=CurPos;
						Token->SyntacticPos=CurSyntacticPos++;
						if(CurSection)
							Token->Depth=CurSection->Depth+1;
					}
					Token->Content+=(*ptr);
					ptr++; CurPos++;   // Next character
				}
			}
			else
			{
				if(State==sRef)
				{
					// Wait until &lt;/ref&gt; is found
					while((!ptr->IsNull())&&RChar::StrNCmp(ptr,"&lt;/ref&gt;",12))
					{
						ptr++;
						CurPos++;
					}

					if(!ptr->IsNull())
					{
						// &lt;/ref&gt; was found -> Skip it
						ptr+=12;
						CurPos+=12;
						State=sText;
					}
				}
				else if(State==sTag)
				{
					// Wait until CurTag is found
					while((!ptr->IsNull())&&RChar::StrNCmp(ptr,CurTag,CurTag.GetLen()))
					{
						ptr++;
						CurPos++;
					}

					if(!ptr->IsNull())
					{
						// Cur Tag was found -> Skip everything until '&gt;' including it.
						while((!ptr->IsNull())&&RChar::StrNCmp(ptr,"&gt;",4))
						{
							ptr++;
							CurPos++;
						}

						ptr+=4;
						CurPos+=4;
						State=sText;
					}
				}
				else if(State==sDiv)
				{

					// Wait until &lt;/div&gt; is found
					while((!ptr->IsNull())&&RChar::StrNCmp(ptr,"&lt;/div&gt;",12))
					{
						ptr++;
						CurPos++;
					}

					if(!ptr->IsNull())
					{
						// &lt;/div&gt; was found -> Skip it
						ptr+=12;
						CurPos+=12;
						State=sText;
					}
				}
				else if(State==sComment)
				{

					// Wait until --&gt; is found
					while((!ptr->IsNull())&&RChar::StrNCmp(ptr,"--&gt;",6))
					{
						ptr++;
						CurPos++;
					}

					if(!ptr->IsNull())
					{
						// --&gt; was found -> Skip it
						ptr+=6;
						CurPos+=6;
						State=sText;
					}
				}
				else if((State==sTable)&&TestTag(ptr,'{','|'))
				{
					NbBrackets++;         // Increment the number of imbricate brackets
					ptr+=2; CurPos+=2;    // Skip the two characters

				}
				else if((State==sLink)&&TestTag(ptr,'[','['))
				{
					if(IncludedLinks)
					{
						NbBrackets++;         // Increment the number of imbricate brackets
						ptr+=2; CurPos+=2;    // Skip the two characters
					}
					else
					{
						// Until now it was a false link
						Token=WikiTokens::Get()->CreateToken();
						Token->Type=WikiToken::Text;
						Token->Pos=CurLink.Pos;
						Token->SyntacticPos=CurLink.SyntacticPos;
						Token->Content=CurLink.Content;
						if(CurSection)
							Token->Depth=CurSection->Depth+1;
						State=sText; // It will catch the '[[' the next time
					}
				}
				else if((State==sCite)&&TestTag(ptr,'{','{'))
				{
					NbBrackets++;         // Increment the number of imbricate brackets
					ptr+=2; CurPos+=2;    // Skip the two characters

				}
				else if(TestTag(ptr,BracketCar1,BracketCar2))
				{
					ptr+=2; CurPos+=2; // Skip the two characters
					NbBrackets--;
					if((!CreateLink())&&(State==sCite)&&(CurLink.Pos!=0))
					{
						// Add a main article link
						Token=WikiTokens::Get()->CreateToken();
						Token->Type=WikiToken::Link;
						Token->Pos=CurLink.Pos;
						Token->SyntacticPos=CurLink.SyntacticPos;
						Token->Content=CurLink.Content.Mid(5);  // Skip 'Main:'
						if(CurSection)
							Token->Depth=CurSection->Depth+1;
					}
					if(!NbBrackets)
						State=sText;
					Token=0;
				}
				else
				{
					if((CurLink.Pos!=0)&&((State==sLink)||(State==sCite)))
						CurLink.Content+=(*ptr);
					ptr++;
					CurPos++;
				}
			}

			if((ptr->IsNull())&&(State==sLink))
			{
				// Suppose that the current link is not a valid one -> restart from where it was
				CurPos=CurLink.Pos;
				Line=CurLink.Content;
				ptr=Line();
				State=sText;
			}
		}

	}
}


//------------------------------------------------------------------------------
void Wikipedia::AnalyzeLogs(void)
{
	RString Tmp,Line,User;
	size_t UserId(0);
	bool InPage(false);
	bool InContributor(false);

	RTextFile Logs(Dir+RFile::GetDirSeparator()+"enwiki-"+DumpStr+"-stub-meta-history.xml","utf-8");
	Logs.Open(RIO::Read);

	while(!Logs.End())
	{
		Line=Logs.GetLine();

		if(InPage)
		{
			if(InContributor)
			{
				if(Line.Ends("</contributor>",true))
				{
					InContributor=false;
					if(UserId&&Article)
					{
						GProfile* Profile(GetProfile(UserId,User));
						Session->InsertFdbk(Profile->GetId(),Article->GetId(),ftRelevant,ArticleUpdated);
					}
					UserId=0;
				}
				else if(Line.Begins("<id>",true))
				{
					ExtractFromTag(Line,Tmp);
					UserId=Tmp.ToSizeT();
				}
				else if(Line.Begins("<username>",true))
				{
					ExtractFromTag(Line,User);
				}
			}
			else
			{
				if(Line.Ends("</page>",true))
				{
					InPage=false;
				}
				else if((!Id)&&Line.Begins("<id>",true))
				{
					ExtractFromTag(Line,Tmp);
					Id=Tmp.ToSizeT();
					LookDoc(false);
					if(Article)
					  cout<<"Document "<<Id<<endl;
				}
				else if(Line.Begins("<timestamp>",true))
				{
					ExtractFromTag(Line,Tmp);
					ArticleUpdated.SetDate(Tmp);
				}
				else if(Line.Begins("<contributor>",true))
				{
					InContributor=true;
					UserId=0;
					User.Clear();
				}
			}
		}
		else if(Line.Begins("<page>",true))
		{
			InPage=true;
			Article=0;
			Id=0;
		}

		NbFdbks++;
		size_t Rest(NbFdbks%1000);
		if(!Rest)
			Slot->StartJob("Import Wikipedia Contributions ("+RString::Number(NbFdbks)+")");

		if(NbContributions&&(NbFdbks>NbContributions))
			return;
	}
}


//------------------------------------------------------------------------------
void Wikipedia::ExtractFromTag(const RString& line,RString& text) const
{
	text.Clear();
	const RChar* Car(line());

	// Find '>'
	while(((*Car)!=0)&&((*Car)!='>'))
		Car++;
	if((*Car)!=0)
		Car++;

	// Skip spaces
	while(((*Car)!=0)&&(Car->IsSpace()))
		Car++;

	// Add character until '<'
	while(((*Car)!=0)&&((*Car)!='<'))
		text+=(*(Car++));
}


//------------------------------------------------------------------------------
void Wikipedia::LookDoc(bool create)
{
	Article=Session->GetObj(pDoc,Id,true,true);
	if(!Article)
	{
		if(!create)
		{
			Article=0;
			return;
		}

		// Doc must me created
		RURI URI("http://en.wikipedia.org/wiki/"+WikiToken::GetWikiTitle(ArticleTitle));
		Article=new GDoc(Session,URI,ArticleTitle,Id,0,0,LangEn,"wikipedia/dump",0,RDate::Null,ArticleUpdated,RDate::Null);
		Session->InsertObj(Article);
		Status=New;
	}
	else
	{
		// Doc exist -> Look if updated
		if((ArticleUpdated>Article->GetUpdated())||Article->MustCompute())
		{
			Status=Update;
			Article->SetUpdated(ArticleUpdated);
		}
		else
			Status=OK;
	}
	WikiTokens::Get()->SetDoc(Article);
}


//------------------------------------------------------------------------------
GProfile* Wikipedia::GetProfile(size_t profid,RString& name)
{
	GProfile* Prof(Session->GetObj(pProfile,profid,true,true));
	if(!Prof)
	{
		// Create a user
		GUser* User(new GUser(Session,profid,name,name,1));
		Session->InsertObj(User);

		// Create a profile
		Prof=new GProfile(Session,User,ptExpertise,profid,0,name,0,RDate::Null,RDate::Null,RDate::Null,true,0.0,0,50);
		Session->InsertObj(Prof);
	}
	return(Prof);
}


//------------------------------------------------------------------------------
void Wikipedia::CreateConfig(void)
{
	InsertParam(new RParamValue("Dir",""));
	InsertParam(new RParamValue("Dump",""));
	InsertParam(new RParamValue("NbArticles",20000,"Number of articles to treat. If null, all articles are treated."));
	InsertParam(new RParamValue("NbContributions",1000,"Number of contributions to treat. If null, all contributions are treated."));
	InsertParam(new RParamValue("ForceAnalyze",false,"Must all the articles be analyzed or only the new and the updated ones."));
}


//------------------------------------------------------------------------------
Wikipedia::~Wikipedia(void)
{
	if(Debug)
	{
		delete Test;
		Test=0;
	}
}


//------------------------------------------------------------------------------
CREATE_TOOL_FACTORY("Wikipedia","Import Dump","Import Wikipedia Dump",Wikipedia)
