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
	else if(InRevision&&(lName=="id"))
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
			if(Article->MustCompute()||PlugIn->MustForceAnalyze())
				PlugIn->GetSession()->AnalyzeDoc(Article);

			Test2<<"https://en.wikipedia.org/wiki/"+Title<<endl;
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
	else if(InRevision&&(lName=="id"))
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