/*

	Wikipedia Plug-Ins

	WikiDumpArticles.cpp

	Parse a Wikipedia Dump Article File - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <gsession.h>
#include <gdoc.h>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <wikidumparticles.h>
#include <../wikitoken/wikitoken.h>
#include <wikipedia.h>


//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;
static RTextFile Test2("/home/pfrancq/debug-wiki.txt","utf-8");



//-----------------------------------------------------------------------------
//
// class WikiDumpArticles
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WikiDumpArticles::WikiDumpArticles(Wikipedia* plugin,const RString& file)
	: RXMLParser(file,"utf-8"), PlugIn(plugin), TmpStrs(20), TmpStrs2(20)
{
	Title.SetLen(2048);
	Content.SetLen(1048576); // Set the buffer size to 1 Mb of words.
	Test2.Open(RIO::Create);
}


//-----------------------------------------------------------------------------
void WikiDumpArticles::Open(void)
{
	InPage=false;
	InText=false;
	InTitle=false;
	InRevision=false;
	Original=false;
	NbAnalyzedArticles=0;
	LangEn=GALILEIApp->GetPlugIn<GLang>("Lang","en");
	RXMLParser::Open(RIO::Read);
}


//-----------------------------------------------------------------------------
void WikiDumpArticles::BeginTagParsed(const RString&,const RString& lName,const RString&)
{
	if(lName=="page")
	{
		InPage=true;
		Original=true;
		Title.SetLen(0);
		WikiTokens::Get()->Clear();
	}
	else if(lName=="title")
	{
		InTitle=true;
		Title.SetLen(0);
	}
	else if(lName=="revision")
	{
		InRevision=true;
	}
	else if((!InRevision)&&(lName=="id"))
	{
		InText=true;
		Content.SetLen(0);
	}
	else if(lName=="redirect")
		Original=false;
	else if((lName=="text")||(lName=="timestamp"))
	{
		InText=true;
		Content.SetLen(0);
		ContentPos=GetPos();
	}
}


//-----------------------------------------------------------------------------
void WikiDumpArticles::EndTag(const RString& namespaceURI,const RString& lName,const RString& name)
{
	if(lName=="page")
	{
		PlugIn->GetSlot()->Interact();
		InPage=false;
		if(Original)
		{
			RString URL(Title);
			URL.Replace(' ','_');
			URL="https://en.wikipedia.org/wiki/"+URL;

			GDoc* Article(PlugIn->GetSession()->GetObj(pDoc,URL,true,true));
			if(!Article)
			{
				// Article must me created
				Article=new GDoc(PlugIn->GetSession(),URL,Title,RString::Number(ArticleId),LangEn,"wikipedia/dump");
				PlugIn->GetSession()->InsertObj(Article);
			}
			else
			{
				// Doc exist -> Look if updated
				if(Updated>Article->GetUpdated())
					Article->SetUpdated(Updated);
			}
			WikiTokens::Get()->SetDoc(Article);

			// Look if the article must be analyzed
			if(Article->MustCompute()||PlugIn->MustForceAnalyze())
			{
				cout<<"Treat "<<URL<<endl;

				// Create a title
				Token=WikiTokens::Get()->CreateToken();
				Token->Type=WikiToken::Title;
				Token->Pos=ContentPos;
				Token->SyntacticPos=0;
				Token->Content=Title;
				Token->Depth=0;
				Token=0;

				TreatWiki();

	//			// Remove all the last links where an outside Wikipedia
	//			size_t ToDel(0);
	//			WikiToken* Last(0);
	//			RCursor<WikiToken> Del(WikiTokens::Get()->GetTokens());
	//			for(Del.StartFromEnd();!Del.Begin();Del.Prev())
	//			{
	//				if(Del()->Type!=WikiToken::Link)
	//				{
	//					if(Last)
	//						break;
	//					Last=Del();
	//				}
	//				else
	//				{
	//					// Point to English
	//					if(!RChar::StrNCmp(&Del()->Content()[7],"en",2))
	//						break;
	//
	//					// Not wikipedia
	//
	//					if(RChar::StrNCmp(&Del()->Content()[10],"wikipedia",9)&&RChar::StrNCmp(&Del()->Content()[11],"wikipedia",9))
	//						break;
	//
	//					ToDel+=2;
	//					Last=0;
	//				}
	//			}
	//			for(ToDel++;--ToDel;)
	//				WikiTokens::Get()->DeleteLastToken();

				// If necessary -> print the tokens
				if(Article&&Debug)
				{
					Test2<<Article->GetName()<<endl;
					RCursor<WikiToken> Cur(WikiTokens::Get()->GetTokens());
					for(Cur.Start();!Cur.End();Cur.Next())
					{
						for(size_t i=Cur()->Depth+2;--i;)
							Test2<<"\t";
						switch(Cur()->Type)
						{
							case WikiToken::Text:
								Test2<<"T ";
								break;
							case WikiToken::Name:
								Test2<<"N ";
								break;
							case WikiToken::Section:
								Test2<<"S ";
								break;
							case WikiToken::Title:
								Test2<<"M ";
								break;
							case WikiToken::Link:
								Test2<<"L ";
								break;
						}
						Test2<<Cur()->Depth<<" "<<Cur()->SyntacticPos<<" "<<Cur()->Pos<<"\t"<<Cur()->Content<<endl;
					}
					Test2<<endl;
				}

				PlugIn->GetSession()->AnalyzeDoc(Article,0,true);
			}

			NbAnalyzedArticles++;
			if(PlugIn->GetNbArticles()&&(NbAnalyzedArticles>=PlugIn->GetNbArticles()))
				 StopAnalysis();
		}
		WikiTokens::Get()->SetDoc(0);
	}
	else if(lName=="revision")
	{
		InRevision=false;
	}
	else if((!InRevision)&&(lName=="id"))
	{
		InText=false;
		if(Content.IsEmpty())
			ArticleId=cNoRef;
		else
			ArticleId=Content.ToSizeT();
	}
	else if(lName=="title")
	{
		InTitle=false;
		PlugIn->GetSlot()->StartJob("Extract content from '"+Title+"'");
	}
	else if(lName=="timestamp")
	{
		InText=false;
		if(Content.IsEmpty())
			Updated=RDate::Null;
		else
			Updated.SetDate(Content.Mid(0,10)+" "+Content.Mid(11,8));
	}
	else if(lName=="text")
	{
		InText=false;
	}
}


//-----------------------------------------------------------------------------
void WikiDumpArticles::Text(const RString& text)
{
	if(Original)
	{
		if(InTitle)
			Title+=text;
		else if(InText)
			Content+=text;
	}
}


//------------------------------------------------------------------------------
void WikiDumpArticles::TreatWiki(void)
{
	CurSection=0;
	CurSyntacticPos=0;
	bool NewLine(true);

	Char.Set(Content);
	Char.Start();
	while(!Char.End())
	{
		if(Char().IsSpace())
		{
			Token=0;  // Terminate a token

			// Look if a new line
			if(Char()==10)          // UNIX case
			{
				NewLine=true;
				Char.Next();
			}
			else if(Char()==13) 	 // MAC and DOS
			{
				NewLine=true;
				Char.Next();

				// DOS only
				if((!Char.End())&&(Char()==10))
					Char.Next();
			}
			else
			{
				NewLine=false;
				Char.Next();
			}
		}
		else
		{
			if(NewLine&&(RChar::StrNCmp(Char.GetCurrent(),"==",2)==0))
				CreateTitle();
			else if(RChar::StrNCmp(Char.GetCurrent(),"[[",2)==0)
				CreateInternalLinks();
			else if(RChar::StrNCmp(Char.GetCurrent(),"{{",2)==0)
				CreateExternalLink();
			else if(Char()=='[')
			{
				// Link ?
				Token=0;  // Terminate a token
				Char.Next();   // Skip '['

				if(PlugIn->MustExtractExternalURI() &&
					 (!RChar::StrNCmp(Char.GetCurrent(),"http",4))  &&
					 ( ((*(Char.GetCurrent()+4))==':') || (((*(Char.GetCurrent()+4))=='s')&&((*(Char.GetCurrent()+5))==':')) )
					)
				{
					// Create the link
					Token=WikiTokens::Get()->CreateToken();
					Token->Type=WikiToken::Link;
					Token->Pos=ContentPos+Char.GetPos()-1;
					Token->SyntacticPos=CurSyntacticPos++;
					if(CurSection)
						Token->Depth=CurSection->Depth+1;

					while((!Char.End())&&(!Char().IsSpace())&&(Char()!=']'))
					{
						Token->Content+=Char();
						Char.Next();
					}

					Token=0;
				}
			}
			else if(RChar::StrNCmp(Char.GetCurrent(),"<!--",4)==0)
			{
				CurSyntacticPos++; // Consider the comment as a token

				// Skip the comment
				Char.Next(4);
				while((!Char.End())&&RChar::StrNCmp(Char.GetCurrent(),"-->",3))
					Char.Next();
				Char.Next(3);
			}
			else if(RChar::StrNCmp(Char.GetCurrent(),"<code",5)==0)
			{
				CurSyntacticPos++; // Consider the code as a token

				// Skip the tag
				Char.Next(5);
				while((!Char.End())&&RChar::StrNCmp(Char.GetCurrent(),"</code>",7))
					Char.Next();
				Char.Next(7);
			}
			else if(RChar::StrNCmp(Char.GetCurrent(),"<source",7)==0)
			{
				CurSyntacticPos++; // Consider the source code as a token

				// Skip the tag
				Char.Next(7);
				while((!Char.End())&&RChar::StrNCmp(Char.GetCurrent(),"</source>",9))
					Char.Next();
				Char.Next(9);
			}
			else if(RChar::StrNCmp(Char.GetCurrent(),"</ref>",6)==0)
			{
	/*					CurSyntacticPos++; // Consider a reference as a token

				// Skip including "</ref>"
				while((!Char.End())&&(RChar::StrNCmp(Char.GetCurrent(),"</ref>",6)))
					Char.Next();*/
				Char.Next(5);  // Skip /ref>
			}
			else if(RChar::StrNCmp(Char.GetCurrent(),"<ref",4)==0)
			{
				CurSyntacticPos++; // Consider a reference as a token

				// Skip including "</ref>"
				//while((!Char.End())&&(RChar::StrNCmp(Char.GetCurrent(),">",1)))
				while((!Char.End())&&(Char()!='>'))
					Char.Next();
				//Char.Next();  // Skip /
			}
			else if(Char()=='&')
			{
				// This a HTML-like code
				Char.Next(); // Skip '&'
				RString Code;
				while((!Char.End())&&(Char()!=';'))
				{
					Code+=Char();
					Char.Next();
				}

				if(!Char.End())
					Char.Next();
				bool IsCode;
				unsigned int TheCode(Code.ToUInt(IsCode));
				if(IsCode&&Token)
					Token->Content+=RChar(TheCode);
			}
			else if(Char().IsPunct())
			{
				Token=0;  // Terminate a token
				Char.Next();
			}
			else
			{
				// Normal Token
				if(!Token)
				{
					Token=WikiTokens::Get()->CreateToken();
					Token->Type=WikiToken::Text;
					Token->Pos=ContentPos+Char.GetPos();
					Token->SyntacticPos=CurSyntacticPos++;
					if(CurSection)
						Token->Depth=CurSection->Depth+1;
				}
				Token->Content+=Char();
				Char.Next();
			}

			NewLine=false; // The last character treated is not a new line
		}
	}
}


//------------------------------------------------------------------------------
void WikiDumpArticles::CreateTitle(void)
{
	RChar Sec[10];

	// Skip ==
	size_t Pos(Char.GetPos());
	Char.Next(2);
	Sec[0]='=';
	Sec[1]='=';

	// Compute the depth
	int Depth(0);
	while(Char()=='=')
	{
		Sec[Depth+2]='=';
		Depth++;
		if(Depth>5)
			break;
		Char.Next();
	}

	// If depth > 5 -> not a title
	if(Depth>5)
	{
		// Skip all '='
		while((!Char.End())&&(Char()=='='))
			Char.Next();
		return;
	}
	else
		Sec[Depth+2]=0;

	CurSection=WikiTokens::Get()->CreateToken();
	CurSection->Type=WikiToken::Section;
	CurSection->Pos=ContentPos+Pos;
	CurSection->SyntacticPos=CurSyntacticPos++;
	CurSection->Depth=Depth;

	// Read the title
	while(RChar::StrNCmp(Char.GetCurrent(),Sec,CurSection->Depth+2))
	{
		CurSection->Content+=Char();
		if(CurSection->Content.GetLen()>200)
		{
			cout<<CurSection->Content<<endl;
			return;
		}
		Char.Next();
	}

	// Skip ==
	while((!Char.End())&&(Char()=='='))
		Char.Next();
}


//------------------------------------------------------------------------------
void WikiDumpArticles::CreateExternalLink(void)
{
	// Skip the two characters
	size_t Pos(Char.GetPos());
	Char.Next(2);
	int NbBrackets(1);
	bool OnlyOneAccolade(true);

	while(!Char.End())
	{
		if(RChar::StrNCmp(Char.GetCurrent(),"{{",2)==0)
		{
			NbBrackets++;
			Char.Next(2);
			OnlyOneAccolade=false;
		}
		else if(RChar::StrNCmp(Char.GetCurrent(),"}}",2)==0)
		{
			NbBrackets--;
			Char.Next(2);

			if(!NbBrackets)
				break;
		}
		else
			Char.Next();
	}

	// If there is more than one pair of accolades -> Problem
	if(!OnlyOneAccolade)
	{
		CurSyntacticPos++; // Consider other links as a single token
		return;
	}


	//	cout<<"*"<<Content.Mid(Pos+2,4)<<"*"<<endl;
	if(Content.Mid(Pos+2,4)=="cite")
	{
//		cout<<"\t*"<<Content.Mid(Pos+6,80)<<"*"<<endl;

		// Parse the whole thing and search for last,first and title
		RString Name;
		bool Last(false), First(false);
		TmpStrs2.Clear();
		Content.Mid(Pos+6,Char.GetPos()-3-Pos-5).Split(TmpStrs2,'|');
		RCursor<RString> Cite(TmpStrs2);
		for(Cite.Start();!Cite.End();Cite.Next())
		{
			(*Cite())=Cite()->Trim();
			TmpStrs.Clear();
			Cite()->Split(TmpStrs,'=');
			if(TmpStrs.GetNb()==2)
			{
				(*TmpStrs[0])=TmpStrs[0]->Trim();
				//cout<<"\t*"<<(*TmpStrs[0])<<"*"<<endl;
				if(((*TmpStrs[0])=="last")||((*TmpStrs[0])=="first"))
				{
					//cout<<"*"<<(*Cite())<<"*"<<endl;
					(*TmpStrs[1])=TmpStrs[1]->Trim();
					if(((*TmpStrs[0])=="last"))
					{
						Name+=(*TmpStrs[1]);
						Last=true;
					}
					else
					{
						Name=(*TmpStrs[1])+" "+Name;
						First=true;
					}
					if(Last&&First)
					{
						Token=WikiTokens::Get()->CreateToken();
						Token->Type=WikiToken::Name;
						Token->Pos=ContentPos+Cite.GetPos();
						Token->SyntacticPos=CurSyntacticPos++;
						Token->Content=Name;  // Skip 'Main:'
						if(CurSection)
							Token->Depth=CurSection->Depth+1;
						//cout<<"\tName *"<<Name<<"*"<<endl;
					}
				}
				else if(((*TmpStrs[0])=="title"))
				{
					//cout<<"*"<<(*Cite())<<"*"<<endl;
					(*TmpStrs[1])=TmpStrs[1]->Trim();
					Token=WikiTokens::Get()->CreateToken();
					Token->Type=WikiToken::Text;
					Token->Pos=ContentPos+Cite.GetPos();
					Token->SyntacticPos=CurSyntacticPos++;
					Token->Content=(*TmpStrs[1]);
					if(CurSection)
						Token->Depth=CurSection->Depth+1;
					//cout<<"Title *"<<(*TmpStrs[1])<<"*"<<endl;
				}
				else if(((*TmpStrs[0])=="author"))
				{
					//cout<<"*"<<(*Cite())<<"*"<<endl;
					(*TmpStrs[1])=TmpStrs[1]->Trim();
					int Pos(TmpStrs[1]->Find(','));
					if(Pos!=-1)
					{
						// The name is in the form lastname, firstname
						RString First(TmpStrs[1]->Mid(Pos+1));
						RString Last(TmpStrs[1]->Mid(0,Pos));
						(*TmpStrs[1])=First.Trim()+" "+Last.Trim();
					}
					Token=WikiTokens::Get()->CreateToken();
					Token->Type=WikiToken::Name;
					Token->Pos=ContentPos+Cite.GetPos();
					Token->SyntacticPos=CurSyntacticPos++;
					Token->Content=(*TmpStrs[1]);
					if(CurSection)
						Token->Depth=CurSection->Depth+1;
					//cout<<"\tAuthor *"<<(*TmpStrs[1])<<"*"<<endl;
				}
			}
		}
		//cout<<endl<<endl;
	}
	else
		CurSyntacticPos++; // Consider other links as a single token
}


//------------------------------------------------------------------------------
void WikiDumpArticles::CreateInternalLinks(void)
{
	// Skip the two characters
	size_t Pos(Char.GetPos());
	Char.Next(2);
	int NbBrackets(1);
	size_t BeginLink(Char.GetPos()),Size(0);

	while(!Char.End())
	{
		if(RChar::StrNCmp(Char.GetCurrent(),"[[",2)==0)
		{
			NbBrackets++;
			Char.Next(2);
			BeginLink=Char.GetPos();
		}
		else if(RChar::StrNCmp(Char.GetCurrent(),"]]",2)==0)
		{
			// There must be a valid beginning position
			if(BeginLink!=cNoRef)
			{
				// Create the internal links
				TmpStrs.Clear();
				RString URI(Content.Mid(BeginLink,Char.GetPos()-BeginLink));
				//cout<<"Link *"<<URI<<"*"<<endl;
				URI.Split(TmpStrs,'|');
				RCursor<RString> Str(TmpStrs);
				size_t Intern(0);
				for(Str.Start();!Str.End();Str.Next())
				{
					// Add a main article link
					(*Str())=Str()->Trim();
					if(Str()->IsEmpty())
						continue;
					Token=WikiTokens::Get()->CreateToken();
					Token->Type=WikiToken::Link;
					Token->Pos=BeginLink+Size-ContentPos;//+Pos+Intern+Str.GetPos();
					Size+=URI.GetLen()+1;
					Token->SyntacticPos=CurSyntacticPos++;
					Str()->Replace(' ','_');
					Token->Content="https://en.wikipedia.org/wiki/"+(*Str());  // Skip 'Main:'
					Intern+=Str()->GetLen();
					if(CurSection)
						Token->Depth=CurSection->Depth+1;

					//cout<<"*"<<Token->Content<<"*"<<endl;
				}

				BeginLink=cNoRef;   // The last beginning position becomes invalid
			}

			// Skip the brackets
			NbBrackets--;
			Char.Next(2);

			// If no more brackets -> finish treating the link
			if(!NbBrackets)
				break;
		}
		else
			Char.Next();
	}
}
