/*

	GALILEI Research Project

	GStringIndexer.h

	String Indexer - Header.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
	: GAnalyzer(session,fac), TermsSpace(0)
{
}


//------------------------------------------------------------------------------
void GStringIndexer::CreateConfig(void)
{
	InsertParam(new RParamValue("MinSize",3,"Minimum length of a token to be taken as information entity."));
	InsertParam(new RParamValue("MinOccurs",2,"Minimal number of occurrences of a token to be considered as a valid one."));
	InsertParam(new RParamValue("ApplyRulesMetadata",true,"Apply the filtering rules to the metadata?"));
}


//-----------------------------------------------------------------------------
void GStringIndexer::ApplyConfig(void)
{
	MinSize=FindParam<RParamValue>("MinSize")->GetUInt();
	MinOccurs=FindParam<RParamValue>("MinOccurs")->GetUInt();
	ApplyRulesMetadata=FindParam<RParamValue>("ApplyRulesMetadata")->GetBool();
}


//------------------------------------------------------------------------------
void GStringIndexer::TreatTokens(GDocAnalyze* analyzer)
{
	if(!TermsSpace)
		TermsSpace=Session->GetInsertConceptType(ccText,"Terms","Terms");

	RCursor<GToken> Token(analyzer->GetTokens());
	for(Token.Start();!Token.End();Token.Next())
	{
		if(Token()->GetType()!=ttText)
			continue;

		// Get the term
		RString Term(Token()->GetToken());

		// Look if the token is not too small or has enough occurrences
		if(
			((!ApplyRulesMetadata)&&(!Token()->IsUsed(ccMetadata)))        // We must not apply the rule to the metadata and token is used in the metadata
			||
			(ApplyRulesMetadata&&((Term.GetLen()<MinSize)||(Token()->GetNbOccurs()<MinOccurs)))
		   )
			continue;

		// OK -> Assign a concept to it
		GConcept* Concept(TermsSpace->GetInsertConcept(Term));
		Token()->SetConcept(Concept);
	}
}


//------------------------------------------------------------------------------
CREATE_ANALYZER_FACTORY("String Indexer","String Indexer",GStringIndexer)
