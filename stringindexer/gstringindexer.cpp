/*

	GALILEI Research Project

	GStringIndexer.h

	String Indexer - Header.

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
#include <gstringindexer.h>
#include <ggalileiapp.h>
#include <gsession.h>
#include <gdocanalyze.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <gdoc.h>



//------------------------------------------------------------------------------
//
// class GStringIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStringIndexer::GStringIndexer(GSession* session,GPlugInFactory* fac)
	: GAnalyzer(session,fac),TermsSpace(0)
{
}


//------------------------------------------------------------------------------
void GStringIndexer::CreateConfig(void)
{
	InsertParam(new RParamValue("MinWordSize",3,"Minimum length of a word to be taken as information entity."));
	InsertParam(new RParamValue("NonLetterWords",true,"Words containing non-letters can be information entities?"));
	InsertParam(new RParamValue("Filtering",true,"Should the word be filtered?"));
	InsertParam(new RParamValue("NbSameOccur",3,"Maximum number of times a same letter can be repeated in a row to consider the corresponding word as valid one."));
	InsertParam(new RParamValue("NormalRatio",0.3,"Minimum percentage of letters in a word to consider it as a valid one."));
	InsertParam(new RParamValue("MinOccurs",1,"Minimal number of occurrences of a word to be considered as a valid one."));
}


//-----------------------------------------------------------------------------
void GStringIndexer::ApplyConfig(void)
{
	MinWordSize=FindParam<RParamValue>("MinWordSize")->GetUInt();
	NonLetterWords=FindParam<RParamValue>("NonLetterWords")->GetBool();
	Filtering=FindParam<RParamValue>("Filtering")->GetBool();
	NbSameOccur=FindParam<RParamValue>("NbSameOccur")->GetUInt();
	NormalRatio=FindParam<RParamValue>("NormalRatio")->GetDouble();
	MinOccurs=FindParam<RParamValue>("MinOccurs")->GetUInt();
}


//------------------------------------------------------------------------------
void GStringIndexer::TreatTokens(GDocAnalyze* analyzer)
{
	if(!TermsSpace)
		TermsSpace=Session->GetInsertConceptType(Session->GetInsertConceptCat("Text"),"Terms","Terms");

	RCursor<GTextToken> Token(analyzer->GetTokens());
	for(Token.Start();!Token.End();Token.Next())
	{
		// Get the term
		RString Term(Token()->GetToken());

		// Look if the token is not too small or has enough occurrences
		if((Term.GetLen()<MinWordSize)||(Token()->GetNbOccurs()<MinOccurs))
			continue;

		// Parse the terms to verify the other constraints
		bool Ok(true);       // Suppose the terms is OK
		size_t NoAlpha(0);   // Number of non-alphabetic characters
		size_t ActSame(0);   // How many identical characters are actually in a row
		RChar Last(0);       // Reference character
		for(const RChar* ptr=Term();!ptr->IsNull();ptr++)
		{
			// Verify that it may contain non alphanumeric character
			if(!ptr->IsAlpha())
			{
				if(!NonLetterWords)
				{
					Ok=false;
					break;
				}
				NoAlpha++;
			}

			// If filtering -> verify the maximum number of consecutive characters
			if(Filtering)
			{
				if(Last==(*ptr))
				{
					ActSame++;
					if(ActSame>NbSameOccur)
					{
						Ok=false;
						break;
					}
				}
				else
				{
					Last=(*ptr);
					ActSame=1;
				}
			}
		}

		// If something went wrong -> Skip it
		if(!Ok)
			continue;

		// Verify if the ratio is respected
		if(Filtering&&(static_cast<double>(Term.GetLen()-NoAlpha)/static_cast<double>(Term.GetLen())<NormalRatio))
			continue;

		// OK -> Assign a concept to it
		GConcept* Concept(TermsSpace->GetInsertConcept(Term));
		Token()->SetConcept(Concept);
	}
}


//------------------------------------------------------------------------------
CREATE_ANALYZER_FACTORY("String Indexer","String Indexer",GStringIndexer)
