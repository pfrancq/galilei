/*

	GALILEI Research Project

	GStringTokenizer.h

	Classic String Tokenizer - Header.

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
#include <gstringtokenizer.h>
#include <gdocanalyze.h>


//------------------------------------------------------------------------------
//
// class GStringTokenizer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStringTokenizer::GStringTokenizer(GSession* session,GPlugInFactory* fac)
	: GTokenizer(session,fac)
{
}


//------------------------------------------------------------------------------
void GStringTokenizer::Start(void)
{
	GTokenizer::Start();
	State=Leading;
	SkipSep=FirstPos=cNoRef;
}


//------------------------------------------------------------------------------
void GStringTokenizer::TreatChar(GDocAnalyze* analyzer,const R::RChar& car)
{
	if(car.IsSpace())
	{
		// if Leading -> skip it
		if(State==Leading)
		{
			// Nothing to do
		}
		else if(State==Sep)
		{
			// The last character(s) was (were) a separation -> skip them
			State=End;
		}
		else
		{
			// The space should be added
			State=End;
			SkipSep=GetPos();
		}
	}
	else if	( (car=='\'') ||(car=='"')  ||
			  (car=='/')  ||(car=='\\') || (car=='-') || (car=='+') || (car=='_') || (car=='<') || (car=='>') ||
			  (car=='.')  ||(car==';')  || (car==',') || (car==':') || (car=='!') || (car=='?') || (car=='~') ||
			  (car=='=')  ||(car=='+')  || (car=='-') || (car=='*') || (car=='#') || (car=='|') || (car=='&') ||
			  (car=='(')  ||(car==')')  || (car=='[') || (car==']') || (car=='{') || (car=='}') )
	{
		// if Leading -> skip it
		if(State==Leading)
		{
			// Nothing to do
		}
		else
			AddChar(car);

		if(State!=Sep)
		{
			// Mark the first separation character
			State=Sep;
			SkipSep=GetPos()-1;
		}
	}
	else
	{
		// Other character
		State=Normal;
		if(FirstPos==cNoRef)
			FirstPos=GetPos();
		AddChar(car);
	}

	if(State==End)
	{
		analyzer->AddToken(Extract(FirstPos,SkipSep).ToLower());
		Start();
	}
}


//------------------------------------------------------------------------------
CREATE_TOKENIZER_FACTORY("Simple Text Tokenizer","Simple Text Tokenizer",GStringTokenizer)
