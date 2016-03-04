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
#include <gstatement.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GConceptExtractorData::cComplexConcept
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GConceptExtractorData::cComplexConcept
{
public:

	GConcept* Master;

	RContainer<GConcept,false,true> Members;

	cComplexConcept(GConcept* master)
		: Master(master), Members(5)
	{

	}


	int Compare(const cComplexConcept& concept) const
	{
		return(Master->Compare(concept.Master->GetId()));
	}

	int Compare(const GConcept* concept) const
	{
		if(!concept)
			return(-1);
		return(Master->Compare(*concept));
	}
};



//------------------------------------------------------------------------------
//
// class GConceptExtractorData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptExtractorData::GConceptExtractorData(void)
	: Tokens(10), Stems(10), Complex(10)
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
void GConceptExtractor::SearchComplexConcept(const R::RString& token,GConceptExtractorData* data)
{
	GConcept* Token(Session->GetObj(pConcept,Session->GetTokenType(),token,true));
	if(!Token)
		return;

	// Go trough each predicate of the type PartOf
	RCursor<GStatement> Statement(Token->GetObjs(pStatement,seSubject));
	for(Statement.Start();!Statement.End();Statement.Next())
	{
		if(Statement()->GetPredicate()!=Session->GetPartOf())
			continue;

		GConcept* Value(dynamic_cast<GConcept*>(Statement()->GetValue()));
		if(!Value)
			continue;

		// Add the concept in data->Complex
		GConceptExtractorData::cComplexConcept* Complex;
		bool Find;
		size_t idx(data->Complex.GetIndex(Value,Find));
		if(Find)
			Complex=data->Complex[idx];
		else
			data->Complex.InsertPtr(Complex=new GConceptExtractorData::cComplexConcept(Value),idx,false);

		idx=Complex->Members.GetIndex(*Token,Find);
		if(!Find)
			Complex->Members.InsertPtrAt(Token,idx,false);
	}
}


//------------------------------------------------------------------------------
size_t GConceptExtractor::Search(const RString& str,RContainer<GConceptRef,true,true>& results,size_t caller)
{
	// Clear the results
	results.Clear();

	// Get an internal structure
	GConceptExtractorData* Data(Datas.Reserve(caller));
	Data->Reset();

	// Get the individual tokens of the string if lower case
	RString Str(str.Trim());
	Str.ToLower().Split(Data->Tokens,' ');
	RCursor<RString> Token(Data->Tokens);
	for(Token.Start();!Token.End();Token.Next())
		(*Token())=Token()->Trim();

	// If no tokens -> No results, of course!
	if(Data->Tokens.GetNb()==0)
		return(0);

	// Search first for the string as such
	{
		RCursor<GConceptType> Type(Session->GetObjs(pConceptType));
		for(Type.Start();!Type.End();Type.Next())
		{
			// If the string is made of one token -> Look in the tokens dictionaries
			// If the string is made of multiple tokens -> Look in the metadata dictionaries
			if( ((Data->Tokens.GetNb()>1)&&(Type()->GetCategory()!=ccMetadata)) || ((Data->Tokens.GetNb()==1)&&(Type()->GetCategory()!=ccToken)) )
				continue;

			AddResult(Session->GetObj(pConcept,Type(),Str,true),results,1.0);
		}
	}

	// Go through all languages to find the concepts containing some of the tokens
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
			// Look for all concepts containing these stems
			Data->Complex.Clear();
			RCursor<RString> Stem(Data->Stems);
			for(Stem.Start();!Stem.End();Stem.Next())
				SearchComplexConcept(*Stem(),Data);

			// Add all the concepts found
			RCursor<GConceptExtractorData::cComplexConcept> Found(Data->Complex);
			for(Found.Start();!Found.End();Found.Next())
			{
				// Count number of parts
				double NbParts(0.0);
				R::RCursor< GStatement>	Statement(Found()->Master->GetObjs(pStatement,seValue));
				for(Statement.Start();!Statement.End();Statement.Next())
					if(Statement()->GetPredicate()==Session->GetPartOf())
						NbParts++;
				NbParts=static_cast<double>(Found()->Members.GetNb())/NbParts;
				if(NbParts>0.1)
					AddResult(Found()->Master,results,NbParts);
			}
		}
	}

	// Release the internal structure and return
	Datas.Release(caller);
	return(results.GetNb());
	return(0);
}


//------------------------------------------------------------------------------
void GConceptExtractor::Expand(GSearchQuery*,GSearchQueryNode*,size_t)
{
}



//------------------------------------------------------------------------------
GConceptExtractor::~GConceptExtractor(void)
{
}
