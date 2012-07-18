/*

	GALILEI Research Project

	GStemming.h

	Stemming Algorithms - Header.

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
#include <gstemming.h>
#include <ggalileiapp.h>
#include <glang.h>
#include <gdocanalyze.h>
#include <gdoc.h>



//------------------------------------------------------------------------------
//
// class GStemming
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStemming::GStemming(GSession* session,GPlugInFactory* fac)
	: GAnalyzer(session,fac)
{
}


//------------------------------------------------------------------------------
void GStemming::TreatTokens(GDocAnalyze* analyzer)
{
	GLang* Lang(analyzer->GetLang());

	if(Lang)
	{
		RCursor<GToken> Token(analyzer->GetTokens());
		for(Token.Start();!Token.End();Token.Next())
		{
			if(Token()->GetType()!=ttText)
				continue;

			RString Stem(Lang->GetStemming(Token()->GetToken()));
			analyzer->ReplaceToken(Token(),Stem);
		}
	}
}


//------------------------------------------------------------------------------
CREATE_ANALYZER_FACTORY("Stemming Algorithms","Stemming Algorithms",GStemming)
