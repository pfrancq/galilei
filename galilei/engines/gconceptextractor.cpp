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
void GConceptExtractor::AddResult(GConcept* concept,R::RContainer<GConceptRef,true,true>& results,double weight)
{
	if(!concept)
		return;
	bool Find;
	size_t Idx(results.GetIndex(concept,Find));
	if(Find)
	{
		GConceptRef* Ref(results[Idx]);
		if(Ref->GetWeight()<weight)
			Ref->SetWeight(weight);
	}
	else
		results.InsertPtrAt(new GConceptRef(concept,weight),Idx);
}


//------------------------------------------------------------------------------
void GConceptExtractor::AddExpression(const RString& expr,RContainer<GConceptRef,true,true>& results,double weight)
{
	RCursor<GConceptType> Type(Session->GetObjs(pConceptType));
	for(Type.Start();!Type.End();Type.Next())
	{
		if((Type()==Session->GetTokenType())||(Type()->GetName().Mid(2)=="Stopwords"))
			continue;

		AddResult(Session->GetObj(pConcept,Type(),expr,true),results,weight);
	}
}


//------------------------------------------------------------------------------
size_t GConceptExtractor::Search(RContainer<RString,true,false>& tokens,RContainer<GConceptRef,true,true>& results,size_t caller)
{
	// Clear the results
	results.Clear();

	// Get an internal structure
	GConceptExtractorData* Data(Datas.Reserve(caller));
	Data->Reset();

	// Compute the lower version of each tokens and an aggregate version
	Data->Aggregate.Clear();
	RCursor<RString> Token(tokens);
	int nb(0);
	for(Token.Start();!Token.End();Token.Next())
	{
		if(Token()->IsEmpty())
			continue;

		if(nb++)
			Data->Aggregate+=" ";

		RString* Str(new RString(Token()->ToLower().Trim()));
		Data->Aggregate+=(*Str);
		Data->Tokens.InsertPtr(Str);
	}

	if(!Data->Tokens.GetNb())
	{
		Datas.Release(caller);
		return(0);
	}

	// Look first for the raw version of the concept
	if(Data->Tokens.GetNb()==1)
	{
		// Simply look in the token types
		AddResult(Session->GetObj(pConcept,Session->GetTokenType(),Data->Aggregate,true),results,1.0);
	}
	else
		AddExpression(Data->Aggregate,results,1.0);

	// Go through all languages
	RCastCursor<GPlugIn,GLang> Lang(GALILEIApp->GetPlugIns<GLang>("Lang"));
	for(Lang.Start();!Lang.End();Lang.Next())
	{
		// Compute the stems
		Lang()->GetStemming(Data->Tokens,Data->Stems);

		// If nothing found -> next language
		if(!Data->Stems.GetNb())
			continue;

		if(Data->Stems.GetNb()==1)
		{
			// Simply look in the token types
			AddResult(Session->GetObj(pConcept,Session->GetTokenType(),*Data->Stems[0],true),results,1.0);
		}
		else
		{
			// Build the complete expression and search for it
			Data->Aggregate.Clear();
			RCursor<RString> Stem(Data->Stems);
			int nb(0);
			for(Stem.Start();!Stem.End();Stem.Next())
			{
				if(nb++)
					Data->Aggregate+=" ";
				Data->Aggregate+=(*Stem());
			}
			AddExpression(Data->Aggregate,results,1.0);
		}
	}

	// Release the internal structure and return
	Datas.Release(caller);
	return(results.GetNb());
}