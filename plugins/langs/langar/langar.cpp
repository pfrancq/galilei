/*

	GALILEI Research Project

	LangAR.cpp

	Arabic Language - Implementation.

	Copyright 2004-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by David Wartel.
	Copyright 2004 by Sarah Rolfo.
	Copyright 2004-2008 by the Universit√© Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langar.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class ArabicRule;
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GLangAR::ArabicRule
{
public:
	RString OldSuffix;
	RString NewSuffix;
	int BeforePos;
	int AfterPos;
	size_t NbMinLetters;
	int EqualityPos;
	RString Equality;
	int ForbiddenPos;
	RString ForbiddenLetter;
	bool CycleRule; // true if the rule has to be applied until it can not any more, false if the rule has to ben applied one once
	bool NextLevel; // must go to next level if rule is applied
	ArabicRule(const RString& os,const RString& ns,int before_pos,int after_pos,
			size_t nb_min_letters,int equality_pos, const RString& equality,int forbidden_pos, const RString& forbidden_letter,bool cycle_rule,bool next_level)
			: OldSuffix(os), NewSuffix(ns), BeforePos(before_pos), AfterPos(after_pos), NbMinLetters(nb_min_letters),
			EqualityPos(equality_pos), Equality(equality), ForbiddenPos(forbidden_pos), ForbiddenLetter(forbidden_letter), CycleRule(cycle_rule), NextLevel(next_level)
{
}

	int Compare(const ArabicRule&) const {return(-1);} // force rules to be sorted as entered;
	bool Apply(RString& kwd); //function to apply the rule -> returns true if a stemmer is applied
	int CheckConditions(RString kwd); // return (-1) if conditions are not verified, else return the position of the old suffix in the word kwd
};


//-----------------------------------------------------------------------------
int GLangAR::ArabicRule::CheckConditions(RString kwd)
{
	int position=-1;

	//find the correct position of the old suffix
	if(BeforePos!=-1)
		position=BeforePos; // position is readjusted to the correct old suffix in the word
	else if(AfterPos!=-1)
	{
		position=static_cast<int>(kwd.GetLen()-OldSuffix.GetLen()-AfterPos);
		if (position<0)
			return(-1);
	}
	if (position==-1)	//position is not hard-coded :find the first occurrence of the old suffix
	{
		position=kwd.FindStr(OldSuffix,0);
		if (position==-1)
			 return(-1);
	}

	// Checking before position conditions
	if((kwd.Mid(position, OldSuffix.GetLen()).Compare(OldSuffix)))
		return(-1); // Before position

	// checking before conditions
	if((AfterPos!=-1)&&(((kwd.GetLen()-OldSuffix.GetLen()-size_t(position))!=size_t(AfterPos))))
		return(-1);

	// Now verify if this old suffix is the old suffix to change (if old suffix appears many times in the word)
	// Check before position and after position.

	//check for equality index and letters
	if(EqualityPos!=-1)
		if((kwd.Mid(EqualityPos, Equality.GetLen()).Compare(Equality)))
			return(-1);

	//check for forbidden letter
	if((ForbiddenPos!=-1)&&(!(kwd.Mid(ForbiddenPos, ForbiddenLetter.GetLen()).Compare(ForbiddenLetter))))
		return(-1);

	return(position);
}


//-----------------------------------------------------------------------------
bool GLangAR::ArabicRule::Apply(RString& kwd)
{
	RString stemmedword;
	int position_stemmed;
	position_stemmed=CheckConditions(kwd);

	if (position_stemmed!=-1&&CycleRule)
	{
		while(position_stemmed!=-1)
		{
			stemmedword=kwd.Mid(0,position_stemmed);
			if (NewSuffix.GetLen())
				stemmedword+=NewSuffix;
			stemmedword+=kwd.Mid(position_stemmed+OldSuffix.GetLen());
			kwd=stemmedword;
			position_stemmed=CheckConditions(kwd);
		}
			return(true);
	}
	else if (position_stemmed!=-1)
	{
		if(position_stemmed!=-1)
		{
			stemmedword=kwd.Mid(0,position_stemmed);
			if (NewSuffix.GetLen())
				stemmedword+=NewSuffix;
			stemmedword+=kwd.Mid(position_stemmed+OldSuffix.GetLen());
			kwd=stemmedword;
		}
		return(true);
	}
	else return(false);
}



//-----------------------------------------------------------------------------
//
// class GLangAR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangAR::GLangAR(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Arabic","ar"), Rules0(5,5), Rules1(5,5), Rules2(5,5), Rules3(5,5), Rules4(5,5),
		 Rules5(5,5), Rules6(5,5), Rules7(5,5)
{
	// Rules 0
	Rules0.InsertPtr(new ArabicRule("Ŕé","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("Ŕč","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ŔŹ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("Ŕí","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("Ŕź","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("Ŕć","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ŔĆ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ŔĎ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ŔÄ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("`","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ōõ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ōĆ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ōü","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("¬į","",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("ō£","ōß",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("ōĘ","ōß",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("ō•","ōß",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("ō§Ŕą","ōß",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ōßō¶","ōß",3,1,5,0,"ōßŔĄ",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ōßō¶","Ŕä",-1,-1,5,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō¶ōß","ōß",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō¶","ōß",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō¶Ŕä","ōß",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","Ŕä",2,-1,5,4,"ōßō°",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","Ŕä",4,-1,0,6,"ōßō°",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","Ŕä",3,-1,0,5,"ōßō°",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","Ŕä",1,-1,0,3,"ōßō°",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","ōß",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ōßō¶Ŕä","",3,0,0,-1,"",0,"ōßŔĄ",false,false));
	Rules1.InsertPtr(new ArabicRule("Ŕäō°","ōß",-1,0,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("Ŕąōßō°","Ŕąō°",-1,0,5,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ō§","Ŕä",1,3,0,3,"ōßō°",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäōßō™","ōß",4,0,0,0,"ōßŔĄ",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäō©","ōß",4,0,0,0,"ōßŔĄ",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäōßō™","",-1,0,4,-1,"",0,"ō®",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäōßō™","ōß",2,0,0,-1,"",1,"Ŕą",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäō©","ōß",2,0,0,-1,"",1,"Ŕą",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäō©","",2,0,0,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäō©","Ŕä",3,0,0,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("Ŕäō©","",-1,0,5,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ō™ōßŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","",-1,0,7,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","ōßŔÜ",-1,0,0,0,"ŔąōßŔĄ",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","",-1,0,5,0,"ōßŔä",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","ōßŔÜ",3,0,0,0,"ŔĄŔĄ",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","ōßŔÜ",3,0,0,0,"Ŕą",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","ōßŔÜ",3,0,0,0,"ŔĀ",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","ōßŔÜ",3,0,0,0,"ō®",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßŔÜ","",-1,0,5,-1,"",0,"ōß",false,false));
	Rules2.InsertPtr(new ArabicRule("ōßō™","",-1,0,4,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ō©","",-1,0,3,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ŔČ","",-1,0,3,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßŔĄō®","ōßŔĄō®",0,3,0,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔąōßŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔąŔĄŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕąō≥Ŕä","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕąō≥ō™","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕąō≥ōß","",0,-1,6,-1,"",3,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕąō≥ŔÜ","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō®ōßŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉōßŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĀōßŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĀōßŔĄ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔąŔĀŔĄ","ŔĄ",0,-1,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĀŔĄō™","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄŔĄŔÜ","ŔÜ",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄŔĄ","",0,-1,4,-1,"",3,"ōßŔÜ",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄŔä","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄō™","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄōß","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĄŔÜ","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßŔĄ","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō≥Ŕä","",0,-1,4,-1,"",2,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("ō≥ō™","ō™",0,-1,6,3,"ōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō≥ō™","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō≥ōß","",0,-1,5,-1,"",2,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("ō≥ŔÜ","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔĀŔĄ","",0,-1,5,-1,"",4,"ŔäŔÜ",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔÉŔÖōß","",-1,0,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔáŔÖōß","",-1,0,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔÉŔÜ","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔáŔÖ","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔÜōß","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔáŔÜ","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™Ŕáōß","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔÉŔÖ","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÉŔÖōß","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔáŔÖōß","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™ŔÉ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™Ŕá","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™Ŕáōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÉŔÖ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßō™Ŕä","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÉŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔáŔÖ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔáŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÖōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßŔáŔÖ","ŔáŔÖ",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔáŔÖōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÜōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™Ŕä","",-1,0,4,-1,"",1,"Ŕä",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÉ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™Ŕá","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔäŔÜŔä","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™ŔÜŔä","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßŔÜŔä","ŔÜŔä",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÜŔä","",-1,0,5,-1,"",2,"Ŕä",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕáōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÜōß","",-1,0,5,-1,"",3,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÖ","ŔÉŔÖ",-1,0,5,1,"ŔÖ",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ōßŔÉŔÖ","ŔÉŔÖ",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÖ","",-1,0,5,0,"ōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÖ","",-1,0,5,-1,"",2,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÜ","ŔÉŔÜ",3,0,0,0,"Ŕą",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉŔÜ","",-1,0,5,-1,"",3,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔáŔÖ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔáŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","ŔÉ",-1,0,5,2,"ō™",0,"ŔÖō≥",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔäŔÉ","ŔÉ",3,0,0,0,"ōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","ŔÉ",3,0,0,0,"Ŕą",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","ŔÉ",3,0,0,0,"ŔĄ",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","ŔÉ",-1,0,6,1,"ōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","ŔÉ",3,0,0,0,"ŔÖ",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ŔÉ","",-1,0,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕá","Ŕá",-1,0,4,0,"ŔÖŔąōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕá","Ŕá",-1,0,5,1,"ŔÖŔąōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕá","",-1,0,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™","",-1,0,4,-1,"",0,"ō®",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™","",0,4,0,3,"ŔąŔÜ",2,"ōß",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕä","",0,4,0,3,"ŔąŔÜ",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™","",0,4,0,3,"Ŕąōß",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("Ŕä","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ō™","",0,4,0,3,"ŔäŔÜ",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔÜŔäŔäŔÜ","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔÜŔäŔąŔÜ","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔäŔÜ","ŔäŔÜ",2,0,0,0,"ō™",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔäŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔÜŔäōß","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔąŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔÜŔä","",-1,0,6,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßŔąŔÜ","ōßŔąŔÜ",-1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔąŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßŔäŔÜ","ōß",-1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("Ŕą","",0,-1,5,3,"ŔäŔÜ",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("Ŕą","",0,-1,5,2,"Ŕäōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔĀ","",0,-1,4,3,"ōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔĀ","",0,-1,5,3,"ŔäŔÜ",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔÜ","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßō°","ōß",-1,0,0,0,"ŔĀōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßō°","ōß",4,0,0,0,"ō®ōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßō°","ōß",3,0,0,0,"ōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("Ŕąōß","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßō°","ōß",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßō°","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("Ŕä","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ō™ŔÖōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜōß","",-1,0,4,-1,"",0,"ōß",false,false));
	Rules4.InsertPtr(new ArabicRule("ō™ŔÖ","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔäŔąŔÜ","ŔäŔÜ",1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔąŔÜ","",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ō™ŔÜ","",-1,0,3,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßŔÉŔÜ","ŔÉŔÜ",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜ","",-1,0,4,0,"ōßŔä",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōß","",0,-1,4,3,"ŔÜ",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔąōßŔÜ","Ŕą",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßŔÜ","ŔÜ",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜ","ŔÜ",3,0,0,1,"ōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜ","ŔÜ",4,0,0,0,"ŔÖŔąōß",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōßŔÖŔÜ","ōßŔÖŔÜ",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜ","ŔÜ",3,0,0,0,"Ŕą",2,"Ŕä",false,false));
	Rules4.InsertPtr(new ArabicRule("ŔÜ","",-1,0,4,-1,"",2,"Ŕä",false,false));
	Rules4.InsertPtr(new ArabicRule("Ŕäōß","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōß","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ōß","",2,1,0,0,"Ŕą",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĀōß","",0,3,0,4,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ōßŔĄō®","ō®",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĀōß","",0,-1,7,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ōß","",2,3,0,0,"ō™",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĀōß","ŔĀ",0,2,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","ō®",0,2,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","",0,5,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","",0,4,0,3,"ō™",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","",0,4,0,4,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","",0,3,0,4,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®","ō®",0,-1,3,1,"Ŕä",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ō®","ō®",0,-1,3,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōßŔą","Ŕą",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®","",0,-1,5,2,"Ŕäōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","",0,-1,8,6,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®ōß","ō®ōß",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®","ō®",0,3,0,2,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō®","",0,-1,4,-1,"",2,"Ŕä",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔąōßŔą","Ŕą",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕąōß","",0,-1,5,-1,"",3,"ōß",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕąōß","ōß",1,3,0,4,"Ŕä",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕąōß","Ŕą",0,3,0,-1,"",3,"ōß",false,false));
	Rules5.InsertPtr(new ArabicRule("ōß","",2,-1,4,0,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ōß","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō™ōß","",-1,0,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔąŔä","",0,-1,6,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕą","",0,-1,4,-1,"",2,"Ŕä",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĀ","",0,-1,4,2,"ŔäŔą",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĀ","",0,-1,4,-1,"",2,"Ŕä",false,false));
	Rules5.InsertPtr(new ArabicRule("ōß","",-1,0,4,-1,"",3,"ō™",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕą","",0,3,0,2,"ŔäŔÜ",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕą","",0,-1,4,2,"ŔäŔą",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕą","",0,-1,4,-1,"",2,"Ŕä",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÉ","",0,-1,4,2,"ŔäŔą",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÉ","",0,-1,4,-1,"",2,"Ŕä",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔĄ","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔąŔä","",3,0,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕä","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ōßŔä","Ŕą",0,3,0,3,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ō™","ō™",0,4,0,3,"Ŕä",1,"ō™",false,false));
	Rules5.InsertPtr(new ArabicRule("ō™","",0,-1,4,-1,"",2,"ōß",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÜŔąōß","ŔÜŔą",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÜŔäōß","ŔÜŔą",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÜ","ŔÜ",0,3,0,2,"ōß",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÜ","ŔÜ",0,-1,4,2,"Ŕä",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ŔÜ","",0,-1,4,-1,"",1,"ōß",false,false));
	Rules5.InsertPtr(new ArabicRule("Ŕą","",-1,0,4,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ŔÜō™Ŕä","ŔÜō™Ŕä",0,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",2,3,0,0,"ŔÖ",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕä","Ŕą",3,1,0,1,"ōß",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕä","",3,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",4,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕäōß","",2,1,0,1,"ō™",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",3,1,0,1,"ō™",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕäōß","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßōß","Ŕä",2,1,0,0,"ŔÖ",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßōß","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕą","Ŕą",3,1,0,0,"ŔÖ",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕä","",4,0,0,0,"ŔÖ",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßŔą","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",3,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",2,2,0,-1,"",0,"ō™",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßŔäōß","Ŕäōß",2,0,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕąōß","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕąōß","Ŕą",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßōß","Ŕą",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōßŔä","Ŕä",2,0,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ŔąŔä","Ŕä",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕą","Ŕą",2,1,0,0,"ŔÖ",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕą","",2,1,0,-1,"",0,"Ŕą",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕä","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕä","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("Ŕäōß","ōß",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",4,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ōß","",2,1,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("Ŕä","",3,1,0,0,"ŔÖ",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ōßŔą","ōßōß",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("Ŕä","",0,2,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ŔąŔä","ōßōß",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ōßŔČ","ōßōß",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ŔąŔČ","ōßōß",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ōß","",2,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("Ŕą","ōß",1,1,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("Ŕä","",2,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("Ŕä","",-1,0,4,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ōß","ōßōß",1,0,0,-1,"",-1,"",false,false));

	// Load rules;
//	LoadRules();
}


//-----------------------------------------------------------------------------
void GLangAR::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("ōĻōīōĪ"));
	stop.InsertPtr(new RString("ōĻō®ōĮ"));
	stop.InsertPtr(new RString("ōĻōĮōĮ"));
	stop.InsertPtr(new RString("ōĻōĮō©"));
	stop.InsertPtr(new RString("ōĻōīōĪō©"));
	stop.InsertPtr(new RString("ōĻōĮŔÖ"));
	stop.InsertPtr(new RString("ōĻōßŔÖ"));
	stop.InsertPtr(new RString("ōĻōßŔÖōß"));
	stop.InsertPtr(new RString("ōĻōĪŔĀōßō™"));
	stop.InsertPtr(new RString("ōĻŔÜ"));
	stop.InsertPtr(new RString("ōĻŔÜōĮ"));
	stop.InsertPtr(new RString("ōĻŔÖōßŔÜ"));
	stop.InsertPtr(new RString("ōĻŔÜōĮŔÖōß"));
	stop.InsertPtr(new RString("ōĻŔĄŔä"));
	stop.InsertPtr(new RString("ōĻŔĄŔäŔá"));
	stop.InsertPtr(new RString("ōĻŔĄŔäŔáōß"));
	stop.InsertPtr(new RString("ōĻŔÖŔĄŔäō©"));
	stop.InsertPtr(new RString("ō≤ŔäōßōĪō©"));
	stop.InsertPtr(new RString("ō≥ō®ō™ŔÖō®ōĪ"));
	stop.InsertPtr(new RString("ō≥ōßōĪōßŔäŔäŔĀŔą"));
	stop.InsertPtr(new RString("ō≥ŔÜō©"));
	stop.InsertPtr(new RString("ō≥ŔąōĪŔäōß"));
	stop.InsertPtr(new RString("ō≥ŔÜŔąōßō™"));
	stop.InsertPtr(new RString("ō™ōīōĪŔäŔÜ"));
	stop.InsertPtr(new RString("ō™ŔÖ"));
	stop.InsertPtr(new RString("ō™ŔÖŔąō≤"));
	stop.InsertPtr(new RString("ō∂ōĮ"));
	stop.InsertPtr(new RString("ō®ōĻōĮ"));
	stop.InsertPtr(new RString("ō®ōĻō∂"));
	stop.InsertPtr(new RString("ōßōĻōßōĮō©"));
	stop.InsertPtr(new RString("ōßōĻŔĄŔÜ"));
	stop.InsertPtr(new RString("ōßōĻŔĄŔÜō™"));
	stop.InsertPtr(new RString("ō≠ō≤ō®"));
	stop.InsertPtr(new RString("ō≠ō≤ŔäōĪōßŔÜ"));
	stop.InsertPtr(new RString("ō®ō≥ō®ō®"));
	stop.InsertPtr(new RString("ōßō≥ōĪōßō¶ŔäŔĄ"));
	stop.InsertPtr(new RString("ō≠ō≥ŔäŔÜ"));
	stop.InsertPtr(new RString("ō≠ō™ŔČ"));
	stop.InsertPtr(new RString("ōßō™ŔĀōßŔā"));
	stop.InsertPtr(new RString("ōĶōĪō®"));
	stop.InsertPtr(new RString("ōßōįōß"));
	stop.InsertPtr(new RString("ōßō≠ōĮ"));
	stop.InsertPtr(new RString("ōßōęōĪ"));
	stop.InsertPtr(new RString("ōļō≤ō©"));
	stop.InsertPtr(new RString("ō®ōĪō≥"));
	stop.InsertPtr(new RString("ō®ōßō≥ŔÖ"));
	stop.InsertPtr(new RString("ōßō¨ō™ŔÖōßōĻ"));
	stop.InsertPtr(new RString("ōļōĮōß"));
	stop.InsertPtr(new RString("ōīōģōĶōß"));
	stop.InsertPtr(new RString("ōĶō®ōßō≠"));
	stop.InsertPtr(new RString("ōßō∑ōßōĪ"));
	stop.InsertPtr(new RString("ōßōĪō®ōĻō©"));
	stop.InsertPtr(new RString("ō®ōļōĮōßōĮ"));
	stop.InsertPtr(new RString("ōßōģōĪŔČ"));
	stop.InsertPtr(new RString("ō®ōßōĪŔäō≥"));
	stop.InsertPtr(new RString("ōĪōßō®ŔäŔÜ"));
	stop.InsertPtr(new RString("ōīōĪŔā"));
	stop.InsertPtr(new RString("ō®ōßŔÜ"));
	stop.InsertPtr(new RString("ōßō®Ŕą"));
	stop.InsertPtr(new RString("ōßō¨ŔĄ"));
	stop.InsertPtr(new RString("ōļŔäōĪ"));
	stop.InsertPtr(new RString("ō≠ōĪŔÉō©"));
	stop.InsertPtr(new RString("ōĪō¶Ŕäō≥"));
	stop.InsertPtr(new RString("ō¨ōĮŔäōĮō©"));
	stop.InsertPtr(new RString("ōßō∑ŔĄōßŔā"));
	stop.InsertPtr(new RString("ō®ōīŔÉŔĄ"));
	stop.InsertPtr(new RString("ō®ō∑ŔąŔĄō©"));
	stop.InsertPtr(new RString("ōĶō≠ŔäŔĀō©"));
	stop.InsertPtr(new RString("ō≠ōßŔĄŔäōß"));
	stop.InsertPtr(new RString("ō®ŔÜ"));
	stop.InsertPtr(new RString("ō®Ŕá"));
	stop.InsertPtr(new RString("ōęŔÖ"));
	stop.InsertPtr(new RString("ōßŔĀ"));
	stop.InsertPtr(new RString("ōßŔÜ"));
	stop.InsertPtr(new RString("ōßŔą"));
	stop.InsertPtr(new RString("ōßŔä"));
	stop.InsertPtr(new RString("ō®Ŕáōß"));
	stop.InsertPtr(new RString("ō¨Ŕáō©"));
	stop.InsertPtr(new RString("ōĶŔĀōĪ"));
	stop.InsertPtr(new RString("ō≠Ŕäōę"));
	stop.InsertPtr(new RString("ōßŔÉōĮ"));
	stop.InsertPtr(new RString("ōßŔĄōß"));
	stop.InsertPtr(new RString("ōßŔÖōß"));
	stop.InsertPtr(new RString("ōßŔĄōĻō≥ŔÉōĪŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄōĻōĪōßŔā"));
	stop.InsertPtr(new RString("ōßŔĄōĻōßōĶŔÖō©"));
	stop.InsertPtr(new RString("ōßŔĄōĻōĪō®Ŕäō©"));
	stop.InsertPtr(new RString("ōßŔĄōĻōĪōßŔāŔä"));
	stop.InsertPtr(new RString("ōßŔĄōĻōĪōßŔāŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄōĻōßŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄōĻōßŔĄŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄōĻŔĄōßŔāōßō™"));
	stop.InsertPtr(new RString("ōßŔĄōĻŔÖŔĄ"));
	stop.InsertPtr(new RString("ōßŔÖō≥"));
	stop.InsertPtr(new RString("ōßŔĄō≥ōĻŔąōĮŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄō≥ōßōĻō©"));
	stop.InsertPtr(new RString("ōßŔĄō≥ō®ō™"));
	stop.InsertPtr(new RString("ōßŔĄō≥ōßō®Ŕā"));
	stop.InsertPtr(new RString("ōĪŔąō≥Ŕäōß"));
	stop.InsertPtr(new RString("ōßŔĄō≥ŔĄō∑ō©"));
	stop.InsertPtr(new RString("ōßŔĄō≥ŔĄō∑ōßō™"));
	stop.InsertPtr(new RString("ōßŔĄō≥ŔĄōßŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄō™ōĻōßŔąŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄō™ō≠ōĪŔäōĪ"));
	stop.InsertPtr(new RString("ōßŔĄō™ŔČ"));
	stop.InsertPtr(new RString("ōßŔÉō™Ŕąō®ōĪ"));
	stop.InsertPtr(new RString("ōĮŔąōĪō©"));
	stop.InsertPtr(new RString("ōßŔÉōęōĪ"));
	stop.InsertPtr(new RString("ōßŔäōßōĪ"));
	stop.InsertPtr(new RString("ōßŔäō∂ōß"));
	stop.InsertPtr(new RString("ōßŔĄō¨ō≤ōßō¶ōĪ"));
	stop.InsertPtr(new RString("ō≠ŔÖōßō≥"));
	stop.InsertPtr(new RString("ōßŔĄōßō≥ōĪōßō¶ŔäŔĄŔä"));
	stop.InsertPtr(new RString("ōßŔĄōßō≥ōĪōßō¶ŔäŔĄŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄōßō≥ō®ŔąōĻ"));
	stop.InsertPtr(new RString("ōßŔĄōßō≥ŔĄō≠ō©"));
	stop.InsertPtr(new RString("ōßŔĄōßō≥ŔĄōßŔÖŔäō©"));
	stop.InsertPtr(new RString("ōįŔÉōĪō™"));
	stop.InsertPtr(new RString("ōßŔĄōßō™ō≠ōßōĮ"));
	stop.InsertPtr(new RString("ōßŔĄōßō™ŔĀōßŔā"));
	stop.InsertPtr(new RString("ōęŔĄōßōęō©"));
	stop.InsertPtr(new RString("ōßŔĄō≠ōĪō®"));
	stop.InsertPtr(new RString("ōßŔĄōßō≠ōĮ"));
	stop.InsertPtr(new RString("ōßŔĄōįōßō™Ŕä"));
	stop.InsertPtr(new RString("ōßŔĄōīōĪō∑ō©"));
	stop.InsertPtr(new RString("ōßŔĄōßōĪō®ōĻōßō°"));
	stop.InsertPtr(new RString("ōßŔĄōļōĪō®Ŕäō©"));
	stop.InsertPtr(new RString("ōßŔĄōģōßōĪō¨Ŕäō©"));
	stop.InsertPtr(new RString("ōßŔĄōßōĪōĮŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄōīōĪŔā"));
	stop.InsertPtr(new RString("ōßŔäōĪōßŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄō≠ōĮŔąōĮ"));
	stop.InsertPtr(new RString("ōßŔĄōĪō¶Ŕäō≥"));
	stop.InsertPtr(new RString("ōßŔĄōßōģŔäōĪō©"));
	stop.InsertPtr(new RString("ōßŔĄōęōßŔÜŔä"));
	stop.InsertPtr(new RString("ōßŔĄōęōßŔÜŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄōßōęŔÜŔäŔÜ"));
	stop.InsertPtr(new RString("ōīŔÖōßŔĄ"));
	stop.InsertPtr(new RString("ō®ŔäōßŔÜ"));
	stop.InsertPtr(new RString("ōĮŔÖōīŔā"));
	stop.InsertPtr(new RString("ōßŔĄōįŔČ"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÜ"));
	stop.InsertPtr(new RString("ōßŔÖōßŔÖ"));
	stop.InsertPtr(new RString("ōßŔäōßŔÖ"));
	stop.InsertPtr(new RString("ōģŔĄōßŔĄ"));
	stop.InsertPtr(new RString("ōßŔĄōīŔäōģ"));
	stop.InsertPtr(new RString("ōßŔĄō¨Ŕäōī"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔąōĪ"));
	stop.InsertPtr(new RString("ōßŔĄō∂ŔĀō©"));
	stop.InsertPtr(new RString("ōßŔĄō¨ŔÖōĻō©"));
	stop.InsertPtr(new RString("ō®ŔäōĪŔäō≤"));
	stop.InsertPtr(new RString("ōßŔĄōßŔąō≥ō∑"));
	stop.InsertPtr(new RString("ōßŔĄōĪŔąō≥Ŕä"));
	stop.InsertPtr(new RString("ōßŔĄō®Ŕąō≥ŔÜō©"));
	stop.InsertPtr(new RString("ōßŔĄōĪŔąō≥Ŕäō©"));
	stop.InsertPtr(new RString("ō®ŔäōĪŔąō™"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÜō™ōģōßō®ōßō™"));
	stop.InsertPtr(new RString("ōßŔĄō®ŔĄōßōĮ"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔĀōßōĻ"));
	stop.InsertPtr(new RString("ōßŔĄōęŔĄōęōßō°"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÜō®ōßō°"));
	stop.InsertPtr(new RString("ōßŔĄōęŔĄōßōęōßō°"));
	stop.InsertPtr(new RString("ōßŔĄōßŔąōĪŔąō®Ŕä"));
	stop.InsertPtr(new RString("ō≠ŔąōßŔĄŔČ"));
	stop.InsertPtr(new RString("ōßŔĄōįŔäŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔąŔĄ"));
	stop.InsertPtr(new RString("ōßŔĄō≠ŔÉŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÖŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÖŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄōßŔąŔĄ"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔąŔĄō©"));
	stop.InsertPtr(new RString("ōßŔĄōģŔĄŔäō¨"));
	stop.InsertPtr(new RString("ōßŔĄōģŔÖŔäō≥"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÖŔäōĪŔÉŔä"));
	stop.InsertPtr(new RString("ōßŔĄōßŔÖŔäōĪŔÉŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔąŔĄŔä"));
	stop.InsertPtr(new RString("ōßŔĄōßŔąŔĄŔČ"));
	stop.InsertPtr(new RString("ōßŔĄōĮŔąŔĄŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄō≠ŔÉŔąŔÖō©"));
	stop.InsertPtr(new RString("ō®ŔäŔÜ"));
	stop.InsertPtr(new RString("ōįŔĄŔÉ"));
	stop.InsertPtr(new RString("ōĮŔąŔĄ"));
	stop.InsertPtr(new RString("ōĮŔąŔÜ"));
	stop.InsertPtr(new RString("ō≠ŔąŔĄ"));
	stop.InsertPtr(new RString("ō≠ŔäŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄŔĀ"));
	stop.InsertPtr(new RString("ōßŔĄŔČ"));
	stop.InsertPtr(new RString("ōßŔÜŔá"));
	stop.InsertPtr(new RString("ōßŔąŔĄ"));
	stop.InsertPtr(new RString("ō∂ŔÖŔÜ"));
	stop.InsertPtr(new RString("ō¨ŔÜŔąō®"));
	stop.InsertPtr(new RString("ōĮŔąŔĄō©"));
	stop.InsertPtr(new RString("ōßŔÜŔáōß"));
	stop.InsertPtr(new RString("ō¨ŔÖŔäōĻ"));
	stop.InsertPtr(new RString("ōßŔĄŔąō≤ōĪōßō°"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō™ō≠ōĮōę"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō™ō≠ōĮō©"));
	stop.InsertPtr(new RString("ōĮŔąŔĄōßōĪ"));
	stop.InsertPtr(new RString("ōßŔĄŔÜōßōĪ"));
	stop.InsertPtr(new RString("ōßŔĄŔąō∂ōĻ"));
	stop.InsertPtr(new RString("ōßŔĄŔāōĮō≥"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō≠ō™ŔĄō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōĶōĮōĪ"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō®ōßōĪōßō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōĶōĪŔä"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōßō∂Ŕä"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōĶōĪŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōĪō≠ŔĄō©"));
	stop.InsertPtr(new RString("ōßŔĄŔāōĮŔÖ"));
	stop.InsertPtr(new RString("ōßŔĄŔĄō¨ŔÜō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō¨ŔĄō≥"));
	stop.InsertPtr(new RString("ōßŔĄŔĀōĪŔÜō≥Ŕä"));
	stop.InsertPtr(new RString("ōßŔĄŔĀōĪŔÜō≥Ŕäō©"));
	stop.InsertPtr(new RString("ōßŔĄŔāōßŔáōĪō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōĮŔäŔÜō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖōßŔÜŔäōß"));
	stop.InsertPtr(new RString("ōßŔĄŔąō∑ŔÜŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄŔÖō¨ŔÖŔąōĻō©"));
	stop.InsertPtr(new RString("ōßŔĄŔĄŔá"));
	stop.InsertPtr(new RString("ōßŔĄŔĀŔĄō≥ō∑ŔäŔÜŔä"));
	stop.InsertPtr(new RString("ōßŔĄŔĀŔĄō≥ō∑ŔäŔÜŔäō©"));
	stop.InsertPtr(new RString("ōßŔĄŔĀŔĄō≥ō∑ŔäŔÜŔäŔäŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄŔąŔāō™"));
	stop.InsertPtr(new RString("ōßŔĄŔÖŔāōĪōĪ"));
	stop.InsertPtr(new RString("ōßŔĄŔāŔąōßō™"));
	stop.InsertPtr(new RString("ōßŔĄŔÜŔáōßō¶Ŕä"));
	stop.InsertPtr(new RString("ōßŔĄŔÖŔāō®ŔĄ"));
	stop.InsertPtr(new RString("ōßŔĄŔÖŔÜō∑Ŕāō©"));
	stop.InsertPtr(new RString("ōßŔĄŔąŔĄōßŔäōßō™"));
	stop.InsertPtr(new RString("ōßŔĄŔÖŔĀōßŔąō∂ōßō™"));
	stop.InsertPtr(new RString("ōßŔĄŔÖŔĄŔÉ"));
	stop.InsertPtr(new RString("ōßŔĄŔäŔÖŔÜ"));
	stop.InsertPtr(new RString("ōßŔĄŔäŔąŔÖ"));
	stop.InsertPtr(new RString("ōßŔäŔĄŔąŔĄ"));
	stop.InsertPtr(new RString("ōßŔĄŔÉŔąŔäō™"));
	stop.InsertPtr(new RString("Ŕą6"));
	stop.InsertPtr(new RString("ŔāōĮ"));
	stop.InsertPtr(new RString("ŔĄōß"));
	stop.InsertPtr(new RString("ŔÖōß"));
	stop.InsertPtr(new RString("ŔÖōĻ"));
	stop.InsertPtr(new RString("Ŕąō≤ōßōĪō©"));
	stop.InsertPtr(new RString("Ŕąō≤ŔäōĪ"));
	stop.InsertPtr(new RString("ŔÖō≥ōßō°"));
	stop.InsertPtr(new RString("Ŕāō™ŔĄ"));
	stop.InsertPtr(new RString("ŔÉōĪō©"));
	stop.InsertPtr(new RString("ŔÖōĶōĪ"));
	stop.InsertPtr(new RString("Ŕáōįōß"));
	stop.InsertPtr(new RString("ŔĀōßō≤"));
	stop.InsertPtr(new RString("ŔÉō£ō≥"));
	stop.InsertPtr(new RString("Ŕäōßō≥ōĪ"));
	stop.InsertPtr(new RString("ŔāōĪōßōĪ"));
	stop.InsertPtr(new RString("ŔÖōĶōĮōĪ"));
	stop.InsertPtr(new RString("Ŕąōßō≠ōĮ"));
	stop.InsertPtr(new RString("Ŕāō∑ōßōĻ"));
	stop.InsertPtr(new RString("ŔÖōĶōßōĮōĪ"));
	stop.InsertPtr(new RString("ŔÖō®ōßōĪōßō©"));
	stop.InsertPtr(new RString("ŔÖō®ōßōĪŔÉ"));
	stop.InsertPtr(new RString("Ŕąōßō∂ōßŔĀ"));
	stop.InsertPtr(new RString("Ŕąōßō∂ōßŔĀō™"));
	stop.InsertPtr(new RString("ŔĀōĪōßŔÜō≥"));
	stop.InsertPtr(new RString("ŔąōßōīŔÜō∑ŔÜ"));
	stop.InsertPtr(new RString("ŔĀōßŔÜ"));
	stop.InsertPtr(new RString("Ŕāō®ŔĄ"));
	stop.InsertPtr(new RString("ŔāōßŔĄ"));
	stop.InsertPtr(new RString("ŔÉōßŔÜ"));
	stop.InsertPtr(new RString("ŔĄōĮŔČ"));
	stop.InsertPtr(new RString("ŔÜō≠Ŕą"));
	stop.InsertPtr(new RString("ŔáōįŔá"));
	stop.InsertPtr(new RString("ŔąōßŔÜ"));
	stop.InsertPtr(new RString("ŔÖō≠ŔÖōĮ"));
	stop.InsertPtr(new RString("ŔąōßŔÉōĮ"));
	stop.InsertPtr(new RString("ŔäōįŔÉōĪ"));
	stop.InsertPtr(new RString("ŔÖō¨ŔĄō≥"));
	stop.InsertPtr(new RString("ŔĀōĪŔÜō≥ōß"));
	stop.InsertPtr(new RString("ŔÉōĪŔäō≥ō™ŔąŔĀōĪ"));
	stop.InsertPtr(new RString("ŔÉōßŔÜō™"));
	stop.InsertPtr(new RString("ŔąōßŔąō∂ō≠"));
	stop.InsertPtr(new RString("ŔĄō®ŔÜōßŔÜ"));
	stop.InsertPtr(new RString("ŔÖōßŔäŔą"));
	stop.InsertPtr(new RString("ŔÖōĮŔäŔÜō©"));
	stop.InsertPtr(new RString("ŔÖō¨ŔÖŔąōĻō©"));
	stop.InsertPtr(new RString("ŔÉōßŔÜŔąŔÜ"));
	stop.InsertPtr(new RString("ŔĀŔČ"));
	stop.InsertPtr(new RString("ŔÉŔĄ"));
	stop.InsertPtr(new RString("ŔĄŔÖ"));
	stop.InsertPtr(new RString("ŔĄŔÜ"));
	stop.InsertPtr(new RString("ŔĄŔá"));
	stop.InsertPtr(new RString("ŔÖŔÜ"));
	stop.InsertPtr(new RString("ŔáŔą"));
	stop.InsertPtr(new RString("ŔáŔä"));
	stop.InsertPtr(new RString("ŔāŔąō©"));
	stop.InsertPtr(new RString("ŔÉŔÖōß"));
	stop.InsertPtr(new RString("ŔĄŔáōß"));
	stop.InsertPtr(new RString("ŔÖŔÜōį"));
	stop.InsertPtr(new RString("ŔąŔāōĮ"));
	stop.InsertPtr(new RString("ŔąŔĄōß"));
	stop.InsertPtr(new RString("ŔÜŔĀō≥Ŕá"));
	stop.InsertPtr(new RString("ŔÖŔąō≥ŔÉŔą"));
	stop.InsertPtr(new RString("ŔÖŔāō™ŔĄ"));
	stop.InsertPtr(new RString("ŔĄŔāōßō°"));
	stop.InsertPtr(new RString("ŔĄŔÉōĪō©"));
	stop.InsertPtr(new RString("ŔÜŔāō∑ō©"));
	stop.InsertPtr(new RString("ŔāŔąōßō™"));
	stop.InsertPtr(new RString("ŔÖŔāōßō®ŔĄ"));
	stop.InsertPtr(new RString("ŔĄŔÜōĮŔÜ"));
	stop.InsertPtr(new RString("ŔáŔÜōßŔÉ"));
	stop.InsertPtr(new RString("ŔąŔāōßŔĄ"));
	stop.InsertPtr(new RString("ŔąŔÉōßŔÜ"));
	stop.InsertPtr(new RString("ŔÖŔÜō∑Ŕāō©"));
	stop.InsertPtr(new RString("ŔÖŔÜōłŔÖō©"));
	stop.InsertPtr(new RString("ŔÜŔáōßŔäō©"));
	stop.InsertPtr(new RString("ŔąŔÉōßŔĄō©"));
	stop.InsertPtr(new RString("ŔąŔāōßŔĄō™"));
	stop.InsertPtr(new RString("ŔąŔÉōßŔÜō™"));
	stop.InsertPtr(new RString("ŔĄŔĄōßŔÖŔÖ"));
	stop.InsertPtr(new RString("ŔĀŔäŔá"));
	stop.InsertPtr(new RString("ŔÉŔĄŔÖ"));
	stop.InsertPtr(new RString("ŔĄŔÉŔÜ"));
	stop.InsertPtr(new RString("ŔąŔĀŔä"));
	stop.InsertPtr(new RString("ŔąŔāŔĀ"));
	stop.InsertPtr(new RString("ŔąŔĄŔÖ"));
	stop.InsertPtr(new RString("ŔąŔÖŔÜ"));
	stop.InsertPtr(new RString("ŔąŔáŔą"));
	stop.InsertPtr(new RString("ŔąŔáŔä"));
	stop.InsertPtr(new RString("ŔäŔąŔÖ"));
	stop.InsertPtr(new RString("ŔĀŔäŔáōß"));
	stop.InsertPtr(new RString("ŔÖŔÜŔáōß"));
	stop.InsertPtr(new RString("ŔÖŔĄŔäōßōĪ"));
	stop.InsertPtr(new RString("ŔĄŔąŔÉōßŔĄō©"));
	stop.InsertPtr(new RString("ŔäŔÉŔąŔÜ"));
	stop.InsertPtr(new RString("ŔäŔÖŔÉŔÜ"));
	stop.InsertPtr(new RString("ŔÉŔĄŔäŔÜō™ŔąŔÜ"));
	stop.InsertPtr(new RString("ŔÖŔĄŔäŔąŔÜ"));
	stop.InsertPtr(new RString("ŔäŔąŔĄŔäŔą"));
	stop.InsertPtr(new RString("ŔäŔąŔÜŔäŔą"));
	stop.InsertPtr(new RString("ŔÜŔäŔąŔäŔąōĪŔÉ"));
}


//------------------------------------------------------------------------------
void GLangAR::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GLangAR::ApplyRules(RString& kwd,RContainer<ArabicRule,true,false>& rules)
{
	RCursor<ArabicRule> Cur(rules);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Apply(kwd))
		{
			if(Cur()->NextLevel)
				return;
		}
	}
}

//-----------------------------------------------------------------------------
RString GALILEI::GLangAR::GetStemming(const RString& kwd)
{
	// Transform kwd in lower case and store it in res.
	RString res(kwd.ToLower());
	if(res.GetLen()>50)
		return(res);

	// Do the different steps of the Porter algorithm.
	ApplyRules(res,Rules0);
	ApplyRules(res,Rules1);
	ApplyRules(res,Rules2);
	ApplyRules(res,Rules3);
	ApplyRules(res,Rules4);
	ApplyRules(res,Rules5);
	ApplyRules(res,Rules6);
	ApplyRules(res,Rules7);

	// Put the result in res and return it.
	return(res);
}


//-----------------------------------------------------------------------------
GALILEI::GLangAR::~GLangAR(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("ar","Arabic",GLangAR)
