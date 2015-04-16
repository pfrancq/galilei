/*

	GALILEI Research Project

	GStringTokenizer.h

	Classic String Tokenizer - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
#include <gsession.h>
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
	: GTokenizer(session,fac), Schemes(30), URISpace(0)
{
	InitSchemes();
}


//------------------------------------------------------------------------------
void GStringTokenizer::InitSchemes(void)
{
	Schemes.InsertPtr(new RString("about"));
	Schemes.InsertPtr(new RString("http"));
	Schemes.InsertPtr(new RString("mailto"));
}


//------------------------------------------------------------------------------
void GStringTokenizer::CreateConfig(void)
{
	InsertParam(new RParamValue("ExtractNonLetter",true,"Tokens containing non-letters can be information entities?"));
	InsertParam(new RParamValue("ExtractURI",true,"Extract URI as tokens representing Links?"));
	InsertParam(new RParamValue("Filtering",true,"Should the word be filtered?"));
	InsertParam(new RParamValue("MaxNonLetter",5,"Maximum number of non-letter characters to consider a token as valid."));
	InsertParam(new RParamValue("MaxConsecutiveOccurs",3,"Maximum consecutive occurrences of a same character to consider a word valid."));
	InsertParam(new RParamValue("NormalRatio",0.3,"Minimal ratio of the number of letters on the total number of characters to consider a token as a valid."));
}


//-----------------------------------------------------------------------------
void GStringTokenizer::ApplyConfig(void)
{
	ExtractNonLetter=FindParam<RParamValue>("ExtractNonLetter")->GetBool();
	ExtractURI=FindParam<RParamValue>("ExtractURI")->GetBool();
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
	PosDoublePoint=0;
	Pos1At=0;
	Pos2At=0;
}


//------------------------------------------------------------------------------
bool GStringTokenizer::IsURI(R::RString& str)
{
	if(!PosDoublePoint)
	{
		// Verify if it is a normal email address

		// Is there one and only one at sign ?
		if((!Pos1At)||(Pos2At))
			return(false);

		// Scan the characters
		bool SearchPoint(false);
		RCharCursor Car(str);
		for(Car.StartFromEnd();!Car.Begin();Car.Prev())
		{
			// Is at least one point after the at sign
			if(SearchPoint&&(Car()==RChar('.')))
			{
				if(Car.GetPos()<=Pos1At)
					return(false);
				SearchPoint=false;
			}

			// No other non alphanumeric character are accepted except '.' and '@'
			if((!Car().IsAlNum())&&(Car()!=RChar('.'))&&(Car()!=RChar('@')))
				return(false);
		}

		str="mailto:"+str;
		return(true);
	}

	// Look at the scheme name
	RString Schema(str.Mid(0,PosDoublePoint-1));
	if(Schemes.IsIn(Schema))
		return(true);

	return(false);
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

				// If they aren't accepted -> not a valid token
				if(!ExtractNonLetter)
					ValidToken=false;

				if(car==RChar(':'))
					PosDoublePoint=GetPos();
				else if(car==RChar('@'))
				{
					if(Pos1At)
						Pos2At=GetPos();
					Pos1At=GetPos();
				}
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

				// If  they aren't accepted -> sure it is not a valid token
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
		if(Debug)
			cout<<"EndToken *"+Extract(FirstPos,SkipSep)+"*"<<endl;

		// Verify if it is a valid token
		if(ValidToken)
		{
			RString Token(Extract(FirstPos,SkipSep));
			if(((PosDoublePoint&&(PosDoublePoint<=SkipSep))||Pos1At)&&IsURI(Token))
			{
				GVector* Cur(analyzer->GetCurrentVector());
				GTokenOccur* Occur(analyzer->AddToken(Token,analyzer->GetDefaultURI(),ttLink));
				if(!Occur->GetToken()->GetConcept())
				{
					if(!URISpace)
						URISpace=Session->GetObj(pConceptType,ccLink,"URI","Uniform Resource Identifier");
					Occur->GetToken()->SetConcept(Session->InsertObj(pConcept,URISpace,Token));
				}
				analyzer->SetCurrentVector(Cur);
			}
			else if(NbNonLetters<=MaxNonLetter)
			{
				// Verify the ratio
				double Size(SkipSep-FirstPos);
				if(((Size-NbNonLetters)/Size)>=NormalRatio)
				{
					if(Debug)
						cout<<"Add Token *"+Token.ToLower()+"*"<<endl;
					analyzer->AddToken(Token);
				}
				else
					analyzer->SkipToken();
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
