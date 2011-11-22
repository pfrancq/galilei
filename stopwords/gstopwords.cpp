/*

	GALILEI Research Project

	GStopWords.h

	Stopwords Filter - Header.

	Copyright 2011 by Pascal Francq (pascal@francq.info).

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
#include <gtexttoken.h>



//------------------------------------------------------------------------------
//
// class GStopWords
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStopWords::GStopWords(GSession* session,GPlugInFactory* fac)
	: GAnalyzer(session,fac), ToDelete(200), NbStops(300), NbDiffStops(300)
{
}


//------------------------------------------------------------------------------
void GStopWords::CreateConfig(void)
{
	InsertParam(new RParamValue("StaticLang",false,"Should the language of the document be automatically re-analyzed?"));
	InsertParam(new RParamValue("MinStopWords",0.09,"Minimum percentage of stop words needed to consider a language."));
}


//-----------------------------------------------------------------------------
void GStopWords::ApplyConfig(void)
{
	StaticLang=FindParam<RParamValue>("StaticLang")->GetBool();
	MinStopWords=FindParam<RParamValue>("MinStopWords")->GetDouble();
}



//------------------------------------------------------------------------------
void GStopWords::TreatTokens(GDocAnalyze* analyzer)
{
	// Initialize
	Langs=GALILEIApp->GetPlugIns<GLang>("Lang");

	NbStops.Init(Langs.GetNb(),0);
	R::RNumCursor<size_t> Stops(NbStops);
	NbDiffStops.Init(Langs.GetNb(),0);
	R::RNumCursor<size_t> DiffStops(NbDiffStops);
	ToDelete.Clear();

	// Parse the tokens
	RCursor<GTextToken> Token(analyzer->GetTokens());
	for(Token.Start();!Token.End();Token.Next())
	{
		bool Leave(true);

		// Look for each language, if it is a stop word
		for(Langs.Start(),Stops.Start(),DiffStops.Start();!Langs.End();Langs.Next(),Stops.Next(),DiffStops.Next())
		{
			if(Langs()->InStop(Token()->GetToken()))
			{
				// It is a stopword
				if(Leave)
				{
					ToDelete.InsertPtr(Token());
					Leave=false;
				}
				Stops()++;
				DiffStops()++;
			}
		}
	}

	// Delete the stopwords
	RCursor<GTextToken> Delete(ToDelete);
	for(Delete.Start();!Delete.End();Delete.Next())
		analyzer->DeleteToken(Delete());

	// if the language should not be detect -> return
	if(analyzer->GetDoc()->GetLang()&&StaticLang)
		return;

	// Initialize some other variables
	double Frac;
	double NbTokens(analyzer->GetTokens().GetNb());
	GLang* Lang(0);
	size_t NbStops(0);

	// Search for the language with the maximal number of stopwords
	for(Langs.Start(),Stops.Start(),DiffStops.Start();!Langs.End();Langs.Next(),Stops.Next(),DiffStops.Next())
	{
		Frac=static_cast<double>(DiffStops())/NbTokens;
//		cout<<"  "<<CurLangs()->GetCode()<<" : "<<Frac<<endl;
		if((Stops()>NbStops)&&(Frac>=MinStopWords))
		{
			Lang=Langs();
			NbStops=Stops();
		}
	}
	analyzer->SetLang(Lang);
}


//------------------------------------------------------------------------------
CREATE_ANALYZER_FACTORY("Stopwords Filter","Stopwords Filter",GStopWords)
