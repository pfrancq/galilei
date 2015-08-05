/*

	GALILEI Research Project

	GSearchToken.cpp

	Search Token - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
// include files for GALILEI
#include <gsearchtoken.h>
#include <gconcept.h>
#include <glang.h>
#include <gsession.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GSearchToken
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSearchToken::GSearchToken(GConcept* concept,tType type)
	: Concept(concept), Type(type)
{
}


//------------------------------------------------------------------------------
GSearchToken::GSearchToken(GSession* session,const R::RString& token,tType type)
	: Concept(0), Type(type)
{
	RString Token(token.Trim());
	switch(Type)
	{
		case tTerm:
		case tStem:
		{
			Token=Token.ToLower();
			GConceptType* Terms(session->GetTokenType());
			Concept=session->GetObj(pConcept,Terms,Token,true);
			break;
		}

		case tRawConcept:
		{
			// First look if the concept type starts with a quote
			int Start(0);   // Suppose that it is not the case
			RString Tmp;
			if(Token[0]=='"')
			{
				// Yes -> find the next quote
				Start=Token.Find('"',1);
				if(Start==-1)
					mThrowGException("No ending quote for the concept type");
				Tmp=Token.Mid(1,Start-1)+Token.Mid(Start+1);
			}
			else
				Tmp=Token;
			int Pos(Token.Find(':',Start));
			if(Pos==-1)
				mThrowGException("No double point separates the concept type from the concept");
			RString ConceptType(Tmp.Mid(0,Pos).Trim());
			RString Name(Tmp.Mid(Pos+1).Trim());
			GConceptType* Type(session->GetObj(pConceptType,ConceptType,true));
			if(Type)
				Concept=session->GetObj(pConcept,Type,Name,true);
			break;
		}
	}
}


//------------------------------------------------------------------------------
int GSearchToken::Compare(const GSearchToken& token) const
{
	if(!Concept)
	{
		if(!token.Concept)
			return(0);
		return(-1);
	}
	return(Concept->Compare(token.Concept));
}


//------------------------------------------------------------------------------
int GSearchToken::Compare(const R::RString& token) const
{
	if(!Concept)
		return(0);
	return(Concept->Compare(token));
}
