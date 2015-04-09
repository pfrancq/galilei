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
const bool Debug=true;
static RTextFile Test2("/home/pfrancq/debug-wiki.txt","utf-8");


//-----------------------------------------------------------------------------
//
// class WikiDumpArticles
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WikiDumpArticles::WikiDumpArticles(Wikipedia* plugin,const RString& file)
	: RXMLParser(file,"utf-8"), PlugIn(plugin)
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
		Title.Clear();
		WikiTokens::Get()->Clear();
	}
	else if(lName=="title")
	{
		InTitle=true;
		Title.Clear();
	}
	else if(lName=="revision")
	{
		InRevision=true;
	}
	else if((!InRevision)&&(lName=="id"))
	{
		InText=true;
		Content.Clear();
	}
	else if(lName=="redirect")
		Original=false;
	else if((lName=="text")||(lName=="timestamp"))
	{
		InText=true;
		Content.Clear();
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
			GDoc* Article(PlugIn->GetSession()->GetObj(pDoc,ArticleId,true,true));
			if(!Article)
			{
				// Article must me created
				Article=new GDoc(PlugIn->GetSession(),"https://en.wikipedia.org/wiki/"+Title,Title,ArticleId,0,0,LangEn,"wikipedia/dump",0,RDate::Null,Updated,RDate::Null);
				PlugIn->GetSession()->InsertObj(Article);
			}
			else
			{
				// Doc exist -> Look if updated
				if(Updated>Article->GetUpdated())
					Article->SetUpdated(Updated);
			}
			WikiTokens::Get()->SetDoc(Article);

			TreatWiki();

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
				Test2<<Article->GetURI()()<<endl;
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

			if(Article->MustCompute()||PlugIn->MustForceAnalyze())
				PlugIn->GetSession()->AnalyzeDoc(Article,0,true);

			NbAnalyzedArticles++;
			if(PlugIn->GetNbArticles()&&(NbAnalyzedArticles>PlugIn->GetNbArticles()))
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
		PlugIn->GetSlot()->StartJob("Treat '"+Title+"'");
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
	WikiToken* Token;
	CurSection=0;
	tState State(sText);  // Suppose we read a text
	CurSyntacticPos=0;

	RChar* Cur(Content());
	while(!Cur->IsNull())
	{
		switch(State)
		{
			case sText:
			{
				if(RChar::StrNCmp(Cur,"==",2)==0)
					CreateTitle(Cur);
				else
				{
					Cur++;
				}
				break;
			}

			default:
				Cur++;
				break;
		}
	}
}


//------------------------------------------------------------------------------
void WikiDumpArticles::CreateTitle(RChar* &car)
{
	RChar Sec[10];

	// Skip ==
	car+=2;
	Sec[0]='=';
	Sec[1]='=';

	// Compute the depth
	int Depth(0);
	while((*car)=='=')
	{
		Sec[Depth+2]='=';
		Depth++;
		if(Depth>5)
			break;
		car++;
	}

	// If depth > 5 -> not a title
	if(Depth>5)
	{
		// Skip all '='
		while((!car->IsNull())&&((*car)=='='))
			car++;
		return;
	}
	else
		Sec[Depth+2]=0;

	CurSection=WikiTokens::Get()->CreateToken();
	CurSection->Type=WikiToken::Section;
	CurSection->Pos=GetPos();
	CurSection->SyntacticPos=CurSyntacticPos++;
	CurSection->Depth=Depth;

	// Read the title
	while(RChar::StrNCmp(car,Sec,CurSection->Depth+2))
	{
		CurSection->Content+=(*car);
		car++;
	}

	// Skip ==
	while((!car->IsNull())&&((*car)=='='))
		car++;

}
