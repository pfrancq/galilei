/*

	Wikipedia Plug-Ins

	WParser.h

	Parse the Token extracted from Wikipedia - Implementation.

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
// DEBUG Mode
const bool Debug=false;


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <gsession.h>
#include <gdocanalyze.h>
#include <gdoc.h>
#include <ggalileiapp.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <wparser.h>
#include <../wikitoken/wikitoken.h>



//-----------------------------------------------------------------------------
//
// class WParser
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WParser::WParser(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), TermsSpace(0), URI(0)
{
	AddMIME("wikipedia/dump");
}


//-----------------------------------------------------------------------------
void WParser::ApplyConfig(void)
{
	// Assign the parameters
	ExtractStruct=FindParam<RParamValue>("ExtractStruct")->GetBool();
	ExtractMetadata=FindParam<RParamValue>("ExtractMetadata")->GetBool();
}


//-----------------------------------------------------------------------------
void WParser::Init(void)
{
}


//-----------------------------------------------------------------------------
void WParser::Clear(void)
{
}


//------------------------------------------------------------------------------
void WParser::Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI&)
{
	// Verify that the process is fine
	if(!WikiTokens::Get()->SameDoc(doc))
		mThrowGException("The Wikipedia notice currently analyzed was not extracted from the dump");

	// Get the concept types if necessary
	if(!TermsSpace)
		TermsSpace=Session->GetObj(pConceptType,ccText,"Terms","Terms");
	if(!URI)
		URI=Session->GetObj(pConceptType,ccLink,"URI","URI");

	// Initialize the analyze
	Clear();
	Analyzer=analyzer;

	RCursor<WikiToken> Cur(WikiTokens::Get()->GetTokens());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		switch(Cur()->Type)
		{
			case WikiToken::Text:
				Analyzer->ExtractDefaultText(Cur()->Content,
					                   Cur()->Pos,
					                   Cur()->Depth,
					                   Cur()->SyntacticPos);
				break;
			case WikiToken::Name:
				Analyzer->AddToken(Cur()->Content,
									 ttText,
									 0,   // Null concept
									 2.0,
									 Analyzer->GetDefaultText(),
			                   Cur()->Pos,
			                   Cur()->Depth,
			                   Cur()->SyntacticPos);

				break;
			case WikiToken::Section:
				Analyzer->ExtractDefaultText(Cur()->Content,
					               ttDivision,
					               4-Cur()->Depth,
					               Cur()->Pos,
										Cur()->Depth,
					               Cur()->SyntacticPos);
				break;
			case WikiToken::Title:
				Analyzer->ExtractDCMI("title",
					                   Cur()->Content,
					                   Cur()->Pos,
					                   Cur()->Depth,
					                   Cur()->SyntacticPos);
				break;
			case WikiToken::Link:
				Analyzer->ExtractDefaultURI(Cur()->Content,
					                Cur()->Pos,
					                Cur()->Depth,
					                Cur()->SyntacticPos);
				break;
		}
	}
}


//------------------------------------------------------------------------------
void WParser::CreateConfig(void)
{
	InsertParam(new RParamValue("ExtractStruct",true,"Extract the structure from Wikipedia pages?"));
	InsertParam(new RParamValue("ExtractMetadata",true,"Extract metadata tags?"));
}


//-----------------------------------------------------------------------------
WParser::~WParser(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("Wikipedia Filter","Wikipedia Filter",WParser)
