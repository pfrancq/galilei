/*

	GALILEI Research Project

	GStopWords.h

	Stopwords Treatment - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <gstopwords.h>
#include <ggalileiapp.h>
#include <glang.h>
#include <gdoc.h>
#include <gdocanalyze.h>
#include <gtoken.h>



//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;



//------------------------------------------------------------------------------
//
// class GStopWords
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStopWords::GStopWords(GSession* session,GPlugInFactory* fac)
	: GAnalyzer(session,fac), ToDelete(200), NbDiffStops(300)
{
}


//------------------------------------------------------------------------------
void GStopWords::CreateConfig(void)
{
	InsertParam(new RParamValue("RemoveStopwords",true,"Should the stopwords of the document be removed?"));
	InsertParam(new RParamValue("StaticLang",false,"Should the language of the document be automatically re-analyzed?"));
	InsertParam(new RParamValue("MinStopWords",0.09,"Minimum percentage of stop words needed to consider a language."));
}


//-----------------------------------------------------------------------------
void GStopWords::ApplyConfig(void)
{
	RemoveStopwords=FindParam<RParamValue>("RemoveStopwords")->GetBool();
	StaticLang=FindParam<RParamValue>("StaticLang")->GetBool();
	MinStopWords=FindParam<RParamValue>("MinStopWords")->GetDouble();
}



//------------------------------------------------------------------------------
void GStopWords::TreatTokens(GDocAnalyze* analyzer)
{
	bool NotGuessLang(analyzer->GetDoc()->GetLang()&&StaticLang);

	// If Verify if something must be done
	if((!RemoveStopwords)&&NotGuessLang)
		return;

	// Initialize
	Langs=GALILEIApp->GetPlugIns<GLang>("Lang");
	NbDiffStops.Init(Langs.GetNb(),0);
	R::RNumCursor<size_t> DiffStops(NbDiffStops);
	ToDelete.Clear();
	size_t NbAlphaTokens(0);

	// Parse the tokens
	RCursor<GToken> Token(analyzer->GetTokens());
	for(Token.Start();!Token.End();Token.Next())
	{
		if(Token()->GetType()!=ttText)
			continue;

		if(Debug)
			cout<<"Test: *"+Token()->GetToken()+"*"<<endl;
		bool Leave(true);

		// If not an alphabetic token -> not a stopword and not a word
		if(!Token()->IsAlpha())
			continue;
		NbAlphaTokens++;

		// Look for each language, if it is a stop word
		for(Langs.Start(),DiffStops.Start();!Langs.End();Langs.Next(),DiffStops.Next())
		{
			if(Langs()->InStop(Token()->GetToken()))
			{
				// It is a stopword
				if(Leave)
				{
					if(RemoveStopwords)
						ToDelete.InsertPtr(Token());
					Leave=false;
				}
				DiffStops()++;
			}
		}
	}

	// Delete the stopwords
	if(RemoveStopwords)
	{
		RCursor<GToken> Delete(ToDelete);
		for(Delete.Start();!Delete.End();Delete.Next())
			analyzer->DeleteToken(Delete());
	}

	// if the language should not be detect -> return
	if(NotGuessLang)
		return;

	// Search for the language with the maximal number of stopwords
	GLang* Lang(0);
	size_t NbStops(0);
	for(Langs.Start(),DiffStops.Start();!Langs.End();Langs.Next(),DiffStops.Next())
	{
		double Frac(static_cast<double>(DiffStops())/NbAlphaTokens);
		if((DiffStops()>NbStops)&&(Frac>=MinStopWords))
		{
			Lang=Langs();
			NbStops=DiffStops();
		}
	}
	analyzer->SetLang(Lang);
}


//------------------------------------------------------------------------------
CREATE_ANALYZER_FACTORY("Stopword Treatment","Stopword Treatment",GStopWords)
