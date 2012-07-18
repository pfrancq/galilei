/*

	GALILEI Research Project

	GStringTokenizer.h

	Classic String Tokenizer - Header.

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
#include <gstringtokenizer.h>
#include <gdocanalyze.h>



//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;



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
void GStringTokenizer::CreateConfig(void)
{
	InsertParam(new RParamValue("ExtractNonLetter",true,"Tokens containing non-letters can be information entities?"));
	InsertParam(new RParamValue("Filtering",true,"Should the word be filtered?"));
	InsertParam(new RParamValue("MaxNonLetter",5,"Maximum number of non-letter characters to consider a token as valid."));
	InsertParam(new RParamValue("MaxConsecutiveOccurs",3,"Maximum consecutive occurrences of a same character to consider a word valid."));
	InsertParam(new RParamValue("NormalRatio",0.3,"Minimal ratio of the number of letters on the total number of characters to consider a token as a valid."));
}


//-----------------------------------------------------------------------------
void GStringTokenizer::ApplyConfig(void)
{
	ExtractNonLetter=FindParam<RParamValue>("ExtractNonLetter")->GetBool();
	Filtering=FindParam<RParamValue>("Filtering")->GetBool();
	MaxConsecutiveOccurs=FindParam<RParamValue>("MaxConsecutiveOccurs")->GetUInt();
	MaxNonLetter=FindParam<RParamValue>("MaxNonLetter")->GetUInt();
	NormalRatio=FindParam<RParamValue>("NormalRatio")->GetDouble();
}


//------------------------------------------------------------------------------
void GStringTokenizer::Start(void)
{
	GTokenizer::Start();
	State=StartToken;
	SkipSep=FirstPos=cNoRef;
	ValidToken=true;
	NbNonLetters=0;
	LastCar=0;
	NbCurOccurs=0;
}


//------------------------------------------------------------------------------
bool GStringTokenizer::TreatChar(GDocAnalyze* analyzer,const R::RChar& car)
{
	bool TokenStart(false);

	if((!car)||(car.IsSpace()))
	{
		// if Leading -> skip it
		if(State==StartToken)
		{
			// Nothing to do
		}
		else if(State==Sep)
		{
			// The last character(s) was (were) a separation -> skip them
			State=EndToken;
		}
		else
		{
			// The space should be added
			State=EndToken;
			SkipSep=GetPos();
		}
	}
	else
	{
		// Verify consecutive characters
		if(LastCar==car)
		{
			NbCurOccurs++;

			// Verify if the maximum number is exceeded ?
			if(NbCurOccurs>MaxConsecutiveOccurs)
				ValidToken=false;
		}
		else
		{
			LastCar=car;
			NbCurOccurs=0;
		}

		if (car.IsPunct())
		{
			// if Leading -> skip it
			if(State==StartToken)
			{
				// Nothing to do
			}
			else
			{
				AddChar(car);
				NbNonLetters++;

				// If  they aren't accepted -> not a valid token
				if(!ExtractNonLetter)
					ValidToken=false;
			}

			if(State!=Sep)
			{
				// Mark the first separation character
				State=Sep;
				SkipSep=GetPos()-1;
			}
		}
		else
		{
			if(State==StartToken)
				TokenStart=true;

			// Treat non-alphabetical characters
			if(!car.IsAlpha())
			{
				NbNonLetters++;
				// If  they aren't accepted -> not a valid token
				if(!ExtractNonLetter)
					ValidToken=false;
			}

			// Other character
			State=Normal;
			if(FirstPos==cNoRef)
				FirstPos=GetPos();
			AddChar(car);
		}
	}

	if(State==EndToken)
	{
		// Verify if it is a valid token
		if(ValidToken&&(NbNonLetters<=MaxNonLetter))
		{
			// Verify the ratio
			RString Token(Extract(FirstPos,SkipSep).ToLower());
			double Size(SkipSep-FirstPos);
			if(((Size-NbNonLetters)/Size)>=NormalRatio)
			{
				if(Debug)
					cout<<"Add Token *"+Token+"*"<<endl;
				analyzer->AddToken(Token);
			}
			else
				analyzer->SkipToken();
		}
		else
			analyzer->SkipToken();
		Start();
	}
	return(TokenStart);
}


//------------------------------------------------------------------------------
CREATE_TOKENIZER_FACTORY("Simple Text Tokenizer","Simple Text Tokenizer",GStringTokenizer)
