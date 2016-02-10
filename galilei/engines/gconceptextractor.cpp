/*

	GALILEI Research Project

	GConceptExtractor.h

	Concept Extractor - Header.

	Copyright 2010-2016 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI Projects
#include <gconceptextractor.h>
#include <gvector.h>
#include <gsession.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GConceptExtractorData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptExtractorData::GConceptExtractorData(void)
	: Tokens(10), Stems(10)
{
}


//------------------------------------------------------------------------------
void GConceptExtractorData::Reset(void)
{
	Tokens.Clear();

}

//------------------------------------------------------------------------------
GConceptExtractorData::~GConceptExtractorData(void)
{

}



//------------------------------------------------------------------------------
//
// class GConceptExtractor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptExtractor::GConceptExtractor(GSession* session)
	: Session(session), Datas(16)
{
}


//------------------------------------------------------------------------------
bool GConceptExtractor::ComputeStems(GConceptExtractorData* data,GLang* lang)
{
	data->Stems.Clear();

	RCursor<RString> Token(data->Tokens);
	for(Token.Start();!Token.End();Token.Next())
	{
		RString Stem(lang->GetStemming(*Token()));

		// Look if the stem is defined in the system
		GConcept* Concept(Session->GetObj(pConcept,Session->GetTokenType(),Stem,true));
		if(!Concept)
			return(false);  // No -> nothing to do

		// Look if the stem must be added
		bool Find;
		int Idx(data->Stems.GetIndex(*Concept,Find));
		if(!Find)
			data->Stems.InsertPtrAt(Concept,Idx,false);
	}
	return(true);
}


//------------------------------------------------------------------------------
void GConceptExtractor::Search(RString& str,bool multiple,RContainer<GVector,true,false>& results,size_t caller)
{
	// Clear the results
	RCursor<GVector> Vector(results);
	for(Vector.Start();!Vector.End();Vector.Next())
		Vector()->Clear();

	// Get an internal structure
	GConceptExtractorData* Data(Datas.Reserve(caller));
	Data->Reset();

	// Treat the search string
	if(multiple)
		str.Split(Data->Tokens,' ');
	else
		Data->Tokens.InsertPtr(new RString(str));

	// Look first for the raw version of the concept



	// Go through all languages
	RCastCursor<GPlugIn,GLang> Lang(GALILEIApp->GetPlugIns<GLang>("Lang"));
	for(Lang.Begin();!Lang.End();Lang.Next())
	{
		// Compute the stems
		if(ComputeStems(Data,Lang()))
		{
			if(Data->Tokens.GetNb()==1)
			{
				// Single token -> simply add it to the first vector
				if(results.GetNb()==0)
					results.InsertPtr(new GVector(Session->GetUnknown()));
				results[0]->InsertRef(new GConceptRef(Data->Stems[0]));
			}
			else
			{

			}
		}
	}

	// Release the internal structure
	Datas.Release(caller);
}