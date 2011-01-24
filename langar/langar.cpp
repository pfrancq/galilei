/*

	GALILEI Research Project

	LangAR.cpp

	Arabic Language - Implementation.

	Copyright 2004-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by David Wartel.
	Copyright 2004 by Sarah Rolfo.
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
	Rules0.InsertPtr(new ArabicRule("َ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ً","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ُ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ْ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ِ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ٍ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ٌ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ّ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("ـ","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("`","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("؛","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("،","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("؟","",-1,-1,0,-1,"",-1,"",true,true));
	Rules0.InsertPtr(new ArabicRule("°","",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("أ","ا",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("آ","ا",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("إ","ا",-1,-1,0,-1,"",-1,"",true,true));
	Rules1.InsertPtr(new ArabicRule("ؤو","ا",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ائ","ا",3,1,5,0,"ال",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ائ","ي",-1,-1,5,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ئا","ا",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ئ","ا",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ئي","ا",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ي",2,-1,5,4,"اء",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ي",4,-1,0,6,"اء",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ي",3,-1,0,5,"اء",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ي",1,-1,0,3,"اء",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ا",-1,-1,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ائي","",3,0,0,-1,"",0,"ال",false,false));
	Rules1.InsertPtr(new ArabicRule("يء","ا",-1,0,0,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("واء","وء",-1,0,5,-1,"",-1,"",false,false));
	Rules1.InsertPtr(new ArabicRule("ؤ","ي",1,3,0,3,"اء",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("يات","ا",4,0,0,0,"ال",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ية","ا",4,0,0,0,"ال",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("يات","",-1,0,4,-1,"",0,"ب",false,false));
	Rules2.InsertPtr(new ArabicRule("يات","ا",2,0,0,-1,"",1,"و",false,false));
	Rules2.InsertPtr(new ArabicRule("ية","ا",2,0,0,-1,"",1,"و",false,false));
	Rules2.InsertPtr(new ArabicRule("ية","",2,0,0,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ية","ي",3,0,0,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ية","",-1,0,5,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("تان","",-1,0,5,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","",-1,0,7,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","ان",-1,0,0,0,"وال",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","",-1,0,5,0,"اي",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","ان",3,0,0,0,"لل",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","ان",3,0,0,0,"و",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","ان",3,0,0,0,"ف",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","ان",3,0,0,0,"ب",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ان","",-1,0,5,-1,"",0,"ا",false,false));
	Rules2.InsertPtr(new ArabicRule("ات","",-1,0,4,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ة","",-1,0,3,-1,"",-1,"",false,false));
	Rules2.InsertPtr(new ArabicRule("ى","",-1,0,3,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("الب","الب",0,3,0,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("وال","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ولل","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("وسي","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("وست","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("وسا","",0,-1,6,-1,"",3,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("وسن","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("بال","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("كال","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("فال","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("فال","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("وفل","ل",0,-1,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("فلت","",0,-1,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("للن","ن",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("لل","",0,-1,4,-1,"",3,"ان",false,false));
	Rules3.InsertPtr(new ArabicRule("لي","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("لت","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("لا","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("لن","",0,-1,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ال","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("سي","",0,-1,4,-1,"",2,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("ست","ت",0,-1,6,3,"ا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ست","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("سا","",0,-1,5,-1,"",2,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("سن","",0,-1,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("فل","",0,-1,5,-1,"",4,"ين",false,false));
	Rules3.InsertPtr(new ArabicRule("اتكما","",-1,0,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتهما","",-1,0,7,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتكن","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتهم","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتنا","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتهن","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتها","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتكم","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تكما","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تهما","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتك","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اته","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تها","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تكم","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اتي","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تكن","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تهم","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تهن","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("كما","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اهم","هم",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("هما","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تنا","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تي","",-1,0,4,-1,"",1,"ي",false,false));
	Rules3.InsertPtr(new ArabicRule("تك","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ته","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("يني","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("تني","",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اني","ني",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ني","",-1,0,5,-1,"",2,"ي",false,false));
	Rules3.InsertPtr(new ArabicRule("ها","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("نا","",-1,0,5,-1,"",3,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("كم","كم",-1,0,5,1,"م",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("اكم","كم",-1,0,6,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("كم","",-1,0,5,0,"ا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("كم","",-1,0,5,-1,"",2,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("كن","كن",3,0,0,0,"و",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("كن","",-1,0,5,-1,"",3,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("هم","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("هن","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","ك",-1,0,5,2,"ت",0,"مس",false,false));
	Rules3.InsertPtr(new ArabicRule("يك","ك",3,0,0,0,"ا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","ك",3,0,0,0,"و",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","ك",3,0,0,0,"ل",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","ك",-1,0,6,1,"ا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","ك",3,0,0,0,"م",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ك","",-1,0,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ه","ه",-1,0,4,0,"موا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ه","ه",-1,0,5,1,"موا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ه","",-1,0,4,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ت","",-1,0,4,-1,"",0,"ب",false,false));
	Rules3.InsertPtr(new ArabicRule("ت","",0,4,0,3,"ون",2,"ا",false,false));
	Rules3.InsertPtr(new ArabicRule("ي","",0,4,0,3,"ون",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ت","",0,4,0,3,"وا",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ي","",-1,0,5,-1,"",-1,"",false,false));
	Rules3.InsertPtr(new ArabicRule("ت","",0,4,0,3,"ين",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ينيين","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ينيون","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("يين","ين",2,0,0,0,"ت",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("يين","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ينيا","",-1,0,8,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("يون","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("يني","",-1,0,6,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اون","اون",-1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ون","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اين","ا",-1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("و","",0,-1,5,3,"ين",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("و","",0,-1,5,2,"يا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ف","",0,-1,4,3,"ا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ف","",0,-1,5,3,"ين",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ين","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اء","ا",-1,0,0,0,"فا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اء","ا",4,0,0,0,"با",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اء","ا",3,0,0,0,"ا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("وا","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اء","ا",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اء","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ي","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("تما","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("نا","",-1,0,4,-1,"",0,"ا",false,false));
	Rules4.InsertPtr(new ArabicRule("تم","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("يون","ين",1,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ون","",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("تن","",-1,0,3,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("اكن","كن",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ن","",-1,0,4,0,"اي",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ا","",0,-1,4,3,"ن",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("وان","و",2,0,0,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ان","ن",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ن","ن",3,0,0,1,"ا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ن","ن",4,0,0,0,"موا",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("امن","امن",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ن","ن",3,0,0,0,"و",2,"ي",false,false));
	Rules4.InsertPtr(new ArabicRule("ن","",-1,0,4,-1,"",2,"ي",false,false));
	Rules4.InsertPtr(new ArabicRule("يا","",-1,0,5,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ا","",-1,0,4,-1,"",-1,"",false,false));
	Rules4.InsertPtr(new ArabicRule("ا","",2,1,0,0,"و",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("فا","",0,3,0,4,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("الب","ب",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("فا","",0,-1,7,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ا","",2,3,0,0,"ت",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("فا","ف",0,2,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","ب",0,2,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","",0,5,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","",0,4,0,3,"ت",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","",0,4,0,4,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","",0,3,0,4,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ب","ب",0,-1,3,1,"ي",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("بب","ب",0,-1,3,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("باو","و",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ب","",0,-1,5,2,"يا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","",0,-1,8,6,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("با","با",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ب","ب",0,3,0,2,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ب","",0,-1,4,-1,"",2,"ي",false,false));
	Rules5.InsertPtr(new ArabicRule("واو","و",0,-1,5,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("وا","",0,-1,5,-1,"",3,"ا",false,false));
	Rules5.InsertPtr(new ArabicRule("وا","ا",1,3,0,4,"ي",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("وا","و",0,3,0,-1,"",3,"ا",false,false));
	Rules5.InsertPtr(new ArabicRule("ا","",2,-1,4,0,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ا","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("تا","",-1,0,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("وي","",0,-1,6,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("و","",0,-1,4,-1,"",2,"ي",false,false));
	Rules5.InsertPtr(new ArabicRule("ف","",0,-1,4,2,"يو",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ف","",0,-1,4,-1,"",2,"ي",false,false));
	Rules5.InsertPtr(new ArabicRule("ا","",-1,0,4,-1,"",3,"ت",false,false));
	Rules5.InsertPtr(new ArabicRule("و","",0,3,0,2,"ين",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("و","",0,-1,4,2,"يو",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("و","",0,-1,4,-1,"",2,"ي",false,false));
	Rules5.InsertPtr(new ArabicRule("ك","",0,-1,4,2,"يو",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ك","",0,-1,4,-1,"",2,"ي",false,false));
	Rules5.InsertPtr(new ArabicRule("ل","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("وي","",3,0,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ي","",0,-1,4,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("اي","و",0,3,0,3,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ت","ت",0,4,0,3,"ي",1,"ت",false,false));
	Rules5.InsertPtr(new ArabicRule("ت","",0,-1,4,-1,"",2,"ا",false,false));
	Rules5.InsertPtr(new ArabicRule("نوا","نو",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("نيا","نو",0,1,0,-1,"",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ن","ن",0,3,0,2,"ا",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ن","ن",0,-1,4,2,"ي",-1,"",false,false));
	Rules5.InsertPtr(new ArabicRule("ن","",0,-1,4,-1,"",1,"ا",false,false));
	Rules5.InsertPtr(new ArabicRule("و","",-1,0,4,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("نتي","نتي",0,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",2,3,0,0,"م",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ي","و",3,1,0,1,"ا",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ي","",3,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",4,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("يا","",2,1,0,1,"ت",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",3,1,0,1,"ت",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("يا","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("اا","ي",2,1,0,0,"م",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("اا","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("و","و",3,1,0,0,"م",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ي","",4,0,0,0,"م",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("او","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",3,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",2,2,0,-1,"",0,"ت",false,false));
	Rules6.InsertPtr(new ArabicRule("ايا","يا",2,0,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("وا","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("وا","و",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("اا","و",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("اي","ي",2,0,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("وي","ي",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("و","و",2,1,0,0,"م",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("و","",2,1,0,-1,"",0,"و",false,false));
	Rules6.InsertPtr(new ArabicRule("ي","",2,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ي","",1,2,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("يا","ا",1,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",4,1,0,-1,"",-1,"",false,false));
	Rules6.InsertPtr(new ArabicRule("ا","",2,1,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ي","",3,1,0,0,"م",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("او","اا",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ي","",0,2,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("وي","اا",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("اى","اا",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("وى","اا",1,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ا","",2,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("و","ا",1,1,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ي","",2,0,0,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ي","",-1,0,4,-1,"",-1,"",false,false));
	Rules7.InsertPtr(new ArabicRule("ا","اا",1,0,0,-1,"",-1,"",false,false));

	// Load rules;
//	LoadRules();
}


//-----------------------------------------------------------------------------
void GLangAR::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("عشر"));
	stop.InsertPtr(new RString("عبد"));
	stop.InsertPtr(new RString("عدد"));
	stop.InsertPtr(new RString("عدة"));
	stop.InsertPtr(new RString("عشرة"));
	stop.InsertPtr(new RString("عدم"));
	stop.InsertPtr(new RString("عام"));
	stop.InsertPtr(new RString("عاما"));
	stop.InsertPtr(new RString("عرفات"));
	stop.InsertPtr(new RString("عن"));
	stop.InsertPtr(new RString("عند"));
	stop.InsertPtr(new RString("عمان"));
	stop.InsertPtr(new RString("عندما"));
	stop.InsertPtr(new RString("علي"));
	stop.InsertPtr(new RString("عليه"));
	stop.InsertPtr(new RString("عليها"));
	stop.InsertPtr(new RString("عملية"));
	stop.InsertPtr(new RString("زيارة"));
	stop.InsertPtr(new RString("سبتمبر"));
	stop.InsertPtr(new RString("ساراييفو"));
	stop.InsertPtr(new RString("سنة"));
	stop.InsertPtr(new RString("سوريا"));
	stop.InsertPtr(new RString("سنوات"));
	stop.InsertPtr(new RString("تشرين"));
	stop.InsertPtr(new RString("تم"));
	stop.InsertPtr(new RString("تموز"));
	stop.InsertPtr(new RString("ضد"));
	stop.InsertPtr(new RString("بعد"));
	stop.InsertPtr(new RString("بعض"));
	stop.InsertPtr(new RString("اعادة"));
	stop.InsertPtr(new RString("اعلن"));
	stop.InsertPtr(new RString("اعلنت"));
	stop.InsertPtr(new RString("حزب"));
	stop.InsertPtr(new RString("حزيران"));
	stop.InsertPtr(new RString("بسبب"));
	stop.InsertPtr(new RString("اسرائيل"));
	stop.InsertPtr(new RString("حسين"));
	stop.InsertPtr(new RString("حتى"));
	stop.InsertPtr(new RString("اتفاق"));
	stop.InsertPtr(new RString("صرب"));
	stop.InsertPtr(new RString("اذا"));
	stop.InsertPtr(new RString("احد"));
	stop.InsertPtr(new RString("اثر"));
	stop.InsertPtr(new RString("غزة"));
	stop.InsertPtr(new RString("برس"));
	stop.InsertPtr(new RString("باسم"));
	stop.InsertPtr(new RString("اجتماع"));
	stop.InsertPtr(new RString("غدا"));
	stop.InsertPtr(new RString("شخصا"));
	stop.InsertPtr(new RString("صباح"));
	stop.InsertPtr(new RString("اطار"));
	stop.InsertPtr(new RString("اربعة"));
	stop.InsertPtr(new RString("بغداد"));
	stop.InsertPtr(new RString("اخرى"));
	stop.InsertPtr(new RString("باريس"));
	stop.InsertPtr(new RString("رابين"));
	stop.InsertPtr(new RString("شرق"));
	stop.InsertPtr(new RString("بان"));
	stop.InsertPtr(new RString("ابو"));
	stop.InsertPtr(new RString("اجل"));
	stop.InsertPtr(new RString("غير"));
	stop.InsertPtr(new RString("حركة"));
	stop.InsertPtr(new RString("رئيس"));
	stop.InsertPtr(new RString("جديدة"));
	stop.InsertPtr(new RString("اطلاق"));
	stop.InsertPtr(new RString("بشكل"));
	stop.InsertPtr(new RString("بطولة"));
	stop.InsertPtr(new RString("صحيفة"));
	stop.InsertPtr(new RString("حاليا"));
	stop.InsertPtr(new RString("بن"));
	stop.InsertPtr(new RString("به"));
	stop.InsertPtr(new RString("ثم"));
	stop.InsertPtr(new RString("اف"));
	stop.InsertPtr(new RString("ان"));
	stop.InsertPtr(new RString("او"));
	stop.InsertPtr(new RString("اي"));
	stop.InsertPtr(new RString("بها"));
	stop.InsertPtr(new RString("جهة"));
	stop.InsertPtr(new RString("صفر"));
	stop.InsertPtr(new RString("حيث"));
	stop.InsertPtr(new RString("اكد"));
	stop.InsertPtr(new RString("الا"));
	stop.InsertPtr(new RString("اما"));
	stop.InsertPtr(new RString("العسكرية"));
	stop.InsertPtr(new RString("العراق"));
	stop.InsertPtr(new RString("العاصمة"));
	stop.InsertPtr(new RString("العربية"));
	stop.InsertPtr(new RString("العراقي"));
	stop.InsertPtr(new RString("العراقية"));
	stop.InsertPtr(new RString("العام"));
	stop.InsertPtr(new RString("العالم"));
	stop.InsertPtr(new RString("العلاقات"));
	stop.InsertPtr(new RString("العمل"));
	stop.InsertPtr(new RString("امس"));
	stop.InsertPtr(new RString("السعودية"));
	stop.InsertPtr(new RString("الساعة"));
	stop.InsertPtr(new RString("السبت"));
	stop.InsertPtr(new RString("السابق"));
	stop.InsertPtr(new RString("روسيا"));
	stop.InsertPtr(new RString("السلطة"));
	stop.InsertPtr(new RString("السلطات"));
	stop.InsertPtr(new RString("السلام"));
	stop.InsertPtr(new RString("التعاون"));
	stop.InsertPtr(new RString("التحرير"));
	stop.InsertPtr(new RString("التى"));
	stop.InsertPtr(new RString("اكتوبر"));
	stop.InsertPtr(new RString("دورة"));
	stop.InsertPtr(new RString("اكثر"));
	stop.InsertPtr(new RString("ايار"));
	stop.InsertPtr(new RString("ايضا"));
	stop.InsertPtr(new RString("الجزائر"));
	stop.InsertPtr(new RString("حماس"));
	stop.InsertPtr(new RString("الاسرائيلي"));
	stop.InsertPtr(new RString("الاسرائيلية"));
	stop.InsertPtr(new RString("الاسبوع"));
	stop.InsertPtr(new RString("الاسلحة"));
	stop.InsertPtr(new RString("الاسلامية"));
	stop.InsertPtr(new RString("ذكرت"));
	stop.InsertPtr(new RString("الاتحاد"));
	stop.InsertPtr(new RString("الاتفاق"));
	stop.InsertPtr(new RString("ثلاثة"));
	stop.InsertPtr(new RString("الحرب"));
	stop.InsertPtr(new RString("الاحد"));
	stop.InsertPtr(new RString("الذاتي"));
	stop.InsertPtr(new RString("الشرطة"));
	stop.InsertPtr(new RString("الاربعاء"));
	stop.InsertPtr(new RString("الغربية"));
	stop.InsertPtr(new RString("الخارجية"));
	stop.InsertPtr(new RString("الاردن"));
	stop.InsertPtr(new RString("الشرق"));
	stop.InsertPtr(new RString("ايران"));
	stop.InsertPtr(new RString("الحدود"));
	stop.InsertPtr(new RString("الرئيس"));
	stop.InsertPtr(new RString("الاخيرة"));
	stop.InsertPtr(new RString("الثاني"));
	stop.InsertPtr(new RString("الثانية"));
	stop.InsertPtr(new RString("الاثنين"));
	stop.InsertPtr(new RString("شمال"));
	stop.InsertPtr(new RString("بيان"));
	stop.InsertPtr(new RString("دمشق"));
	stop.InsertPtr(new RString("الذى"));
	stop.InsertPtr(new RString("الان"));
	stop.InsertPtr(new RString("امام"));
	stop.InsertPtr(new RString("ايام"));
	stop.InsertPtr(new RString("خلال"));
	stop.InsertPtr(new RString("الشيخ"));
	stop.InsertPtr(new RString("الجيش"));
	stop.InsertPtr(new RString("الدور"));
	stop.InsertPtr(new RString("الضفة"));
	stop.InsertPtr(new RString("الجمعة"));
	stop.InsertPtr(new RString("بيريز"));
	stop.InsertPtr(new RString("الاوسط"));
	stop.InsertPtr(new RString("الروسي"));
	stop.InsertPtr(new RString("البوسنة"));
	stop.InsertPtr(new RString("الروسية"));
	stop.InsertPtr(new RString("بيروت"));
	stop.InsertPtr(new RString("الانتخابات"));
	stop.InsertPtr(new RString("البلاد"));
	stop.InsertPtr(new RString("الدفاع"));
	stop.InsertPtr(new RString("الثلثاء"));
	stop.InsertPtr(new RString("الانباء"));
	stop.InsertPtr(new RString("الثلاثاء"));
	stop.InsertPtr(new RString("الاوروبي"));
	stop.InsertPtr(new RString("حوالى"));
	stop.InsertPtr(new RString("الذين"));
	stop.InsertPtr(new RString("الدول"));
	stop.InsertPtr(new RString("الحكم"));
	stop.InsertPtr(new RString("الامم"));
	stop.InsertPtr(new RString("الامن"));
	stop.InsertPtr(new RString("الاول"));
	stop.InsertPtr(new RString("الدولة"));
	stop.InsertPtr(new RString("الخليج"));
	stop.InsertPtr(new RString("الخميس"));
	stop.InsertPtr(new RString("الاميركي"));
	stop.InsertPtr(new RString("الاميركية"));
	stop.InsertPtr(new RString("الدولي"));
	stop.InsertPtr(new RString("الاولى"));
	stop.InsertPtr(new RString("الدولية"));
	stop.InsertPtr(new RString("الحكومة"));
	stop.InsertPtr(new RString("بين"));
	stop.InsertPtr(new RString("ذلك"));
	stop.InsertPtr(new RString("دول"));
	stop.InsertPtr(new RString("دون"));
	stop.InsertPtr(new RString("حول"));
	stop.InsertPtr(new RString("حين"));
	stop.InsertPtr(new RString("الف"));
	stop.InsertPtr(new RString("الى"));
	stop.InsertPtr(new RString("انه"));
	stop.InsertPtr(new RString("اول"));
	stop.InsertPtr(new RString("ضمن"));
	stop.InsertPtr(new RString("جنوب"));
	stop.InsertPtr(new RString("دولة"));
	stop.InsertPtr(new RString("انها"));
	stop.InsertPtr(new RString("جميع"));
	stop.InsertPtr(new RString("الوزراء"));
	stop.InsertPtr(new RString("المتحدث"));
	stop.InsertPtr(new RString("المتحدة"));
	stop.InsertPtr(new RString("دولار"));
	stop.InsertPtr(new RString("النار"));
	stop.InsertPtr(new RString("الوضع"));
	stop.InsertPtr(new RString("القدس"));
	stop.InsertPtr(new RString("المحتلة"));
	stop.InsertPtr(new RString("المصدر"));
	stop.InsertPtr(new RString("المباراة"));
	stop.InsertPtr(new RString("المصري"));
	stop.InsertPtr(new RString("الماضي"));
	stop.InsertPtr(new RString("المصرية"));
	stop.InsertPtr(new RString("المرحلة"));
	stop.InsertPtr(new RString("القدم"));
	stop.InsertPtr(new RString("اللجنة"));
	stop.InsertPtr(new RString("المجلس"));
	stop.InsertPtr(new RString("الفرنسي"));
	stop.InsertPtr(new RString("الفرنسية"));
	stop.InsertPtr(new RString("القاهرة"));
	stop.InsertPtr(new RString("المدينة"));
	stop.InsertPtr(new RString("المانيا"));
	stop.InsertPtr(new RString("الوطنية"));
	stop.InsertPtr(new RString("المجموعة"));
	stop.InsertPtr(new RString("الله"));
	stop.InsertPtr(new RString("الفلسطيني"));
	stop.InsertPtr(new RString("الفلسطينية"));
	stop.InsertPtr(new RString("الفلسطينيين"));
	stop.InsertPtr(new RString("الوقت"));
	stop.InsertPtr(new RString("المقرر"));
	stop.InsertPtr(new RString("القوات"));
	stop.InsertPtr(new RString("النهائي"));
	stop.InsertPtr(new RString("المقبل"));
	stop.InsertPtr(new RString("المنطقة"));
	stop.InsertPtr(new RString("الولايات"));
	stop.InsertPtr(new RString("المفاوضات"));
	stop.InsertPtr(new RString("الملك"));
	stop.InsertPtr(new RString("اليمن"));
	stop.InsertPtr(new RString("اليوم"));
	stop.InsertPtr(new RString("ايلول"));
	stop.InsertPtr(new RString("الكويت"));
	stop.InsertPtr(new RString("و6"));
	stop.InsertPtr(new RString("قد"));
	stop.InsertPtr(new RString("لا"));
	stop.InsertPtr(new RString("ما"));
	stop.InsertPtr(new RString("مع"));
	stop.InsertPtr(new RString("وزارة"));
	stop.InsertPtr(new RString("وزير"));
	stop.InsertPtr(new RString("مساء"));
	stop.InsertPtr(new RString("قتل"));
	stop.InsertPtr(new RString("كرة"));
	stop.InsertPtr(new RString("مصر"));
	stop.InsertPtr(new RString("هذا"));
	stop.InsertPtr(new RString("فاز"));
	stop.InsertPtr(new RString("كأس"));
	stop.InsertPtr(new RString("ياسر"));
	stop.InsertPtr(new RString("قرار"));
	stop.InsertPtr(new RString("مصدر"));
	stop.InsertPtr(new RString("واحد"));
	stop.InsertPtr(new RString("قطاع"));
	stop.InsertPtr(new RString("مصادر"));
	stop.InsertPtr(new RString("مباراة"));
	stop.InsertPtr(new RString("مبارك"));
	stop.InsertPtr(new RString("واضاف"));
	stop.InsertPtr(new RString("واضافت"));
	stop.InsertPtr(new RString("فرانس"));
	stop.InsertPtr(new RString("واشنطن"));
	stop.InsertPtr(new RString("فان"));
	stop.InsertPtr(new RString("قبل"));
	stop.InsertPtr(new RString("قال"));
	stop.InsertPtr(new RString("كان"));
	stop.InsertPtr(new RString("لدى"));
	stop.InsertPtr(new RString("نحو"));
	stop.InsertPtr(new RString("هذه"));
	stop.InsertPtr(new RString("وان"));
	stop.InsertPtr(new RString("محمد"));
	stop.InsertPtr(new RString("واكد"));
	stop.InsertPtr(new RString("يذكر"));
	stop.InsertPtr(new RString("مجلس"));
	stop.InsertPtr(new RString("فرنسا"));
	stop.InsertPtr(new RString("كريستوفر"));
	stop.InsertPtr(new RString("كانت"));
	stop.InsertPtr(new RString("واوضح"));
	stop.InsertPtr(new RString("لبنان"));
	stop.InsertPtr(new RString("مايو"));
	stop.InsertPtr(new RString("مدينة"));
	stop.InsertPtr(new RString("مجموعة"));
	stop.InsertPtr(new RString("كانون"));
	stop.InsertPtr(new RString("فى"));
	stop.InsertPtr(new RString("كل"));
	stop.InsertPtr(new RString("لم"));
	stop.InsertPtr(new RString("لن"));
	stop.InsertPtr(new RString("له"));
	stop.InsertPtr(new RString("من"));
	stop.InsertPtr(new RString("هو"));
	stop.InsertPtr(new RString("هي"));
	stop.InsertPtr(new RString("قوة"));
	stop.InsertPtr(new RString("كما"));
	stop.InsertPtr(new RString("لها"));
	stop.InsertPtr(new RString("منذ"));
	stop.InsertPtr(new RString("وقد"));
	stop.InsertPtr(new RString("ولا"));
	stop.InsertPtr(new RString("نفسه"));
	stop.InsertPtr(new RString("موسكو"));
	stop.InsertPtr(new RString("مقتل"));
	stop.InsertPtr(new RString("لقاء"));
	stop.InsertPtr(new RString("لكرة"));
	stop.InsertPtr(new RString("نقطة"));
	stop.InsertPtr(new RString("قوات"));
	stop.InsertPtr(new RString("مقابل"));
	stop.InsertPtr(new RString("لندن"));
	stop.InsertPtr(new RString("هناك"));
	stop.InsertPtr(new RString("وقال"));
	stop.InsertPtr(new RString("وكان"));
	stop.InsertPtr(new RString("منطقة"));
	stop.InsertPtr(new RString("منظمة"));
	stop.InsertPtr(new RString("نهاية"));
	stop.InsertPtr(new RString("وكالة"));
	stop.InsertPtr(new RString("وقالت"));
	stop.InsertPtr(new RString("وكانت"));
	stop.InsertPtr(new RString("للامم"));
	stop.InsertPtr(new RString("فيه"));
	stop.InsertPtr(new RString("كلم"));
	stop.InsertPtr(new RString("لكن"));
	stop.InsertPtr(new RString("وفي"));
	stop.InsertPtr(new RString("وقف"));
	stop.InsertPtr(new RString("ولم"));
	stop.InsertPtr(new RString("ومن"));
	stop.InsertPtr(new RString("وهو"));
	stop.InsertPtr(new RString("وهي"));
	stop.InsertPtr(new RString("يوم"));
	stop.InsertPtr(new RString("فيها"));
	stop.InsertPtr(new RString("منها"));
	stop.InsertPtr(new RString("مليار"));
	stop.InsertPtr(new RString("لوكالة"));
	stop.InsertPtr(new RString("يكون"));
	stop.InsertPtr(new RString("يمكن"));
	stop.InsertPtr(new RString("كلينتون"));
	stop.InsertPtr(new RString("مليون"));
	stop.InsertPtr(new RString("يوليو"));
	stop.InsertPtr(new RString("يونيو"));
	stop.InsertPtr(new RString("نيويورك"));
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
