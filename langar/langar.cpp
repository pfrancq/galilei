/*

	GALILEI Research Project

	LangAR.cpp

	Arabic Language - Implementation.

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
	size_t Id;
	size_t Level;
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
	ArabicRule(size_t id,size_t level,RString os,RString ns,int before_pos,int after_pos,
			size_t nb_min_letters,int equality_pos, RString equality,int forbidden_pos, RString forbidden_letter,bool cycle_rule,bool next_level);
	int Compare(const ArabicRule&) const {return(-1);} // force rules to be sorted as entered;
	bool Apply(RString& kwd); //function to apply the rule -> returns true if a stemmer is applied
	int CheckConditions(RString kwd); // return (-1) if condtions are not verified, else return the position of the old suffix in the word kwd
	~ArabicRule(void);
};


//-----------------------------------------------------------------------------
GLangAR::ArabicRule::ArabicRule(size_t id,size_t level, RString os, RString ns,  int before_pos, int after_pos,
		size_t nb_min_letters, int equality_pos, RString equality, int forbidden_pos, RString forbidden_letter, bool cycle_rule, bool next_level)
	: Id(id), Level(level), OldSuffix(os), NewSuffix(ns), BeforePos(before_pos), AfterPos(after_pos), NbMinLetters(nb_min_letters),
		EqualityPos(equality_pos), Equality(equality), ForbiddenPos(forbidden_pos), ForbiddenLetter(forbidden_letter), CycleRule(cycle_rule), NextLevel(next_level)
{
}



//-----------------------------------------------------------------------------
int GLangAR::ArabicRule::CheckConditions(RString kwd)
{
	int position=-1;

	//find the correct position of the old suffix
	if(BeforePos!=-1)
		position=BeforePos; // postion is reajusted to the correct old suffix in the word
	else if(AfterPos!=-1)
	{
		position=static_cast<int>(kwd.GetLen()-OldSuffix.GetLen()-AfterPos);
		if (position<0)
			return(-1);
	}
	if (position==-1)	//position is not hard-coded :find the first occurence of the old suffix
	{
		position=kwd.FindStr(OldSuffix,0);
		if (position==-1)
			 return(-1);
	}

	// checking before pos condtions
	if((kwd.Mid(position, OldSuffix.GetLen()).Compare(OldSuffix)))
		return(-1); //before pos

	// checking before condtions
	if(AfterPos!=-1&&(((kwd.GetLen()-OldSuffix.GetLen()-position)!=AfterPos)))
		return(-1);

	//now verify if this old suffix is the old suffix to change (if old suffix appears many times in the word)
	//check before postion and after position.
	//check miminum of letters
	if(kwd.GetLen()<NbMinLetters)
	 	return(-1);

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
GLangAR::ArabicRule::~ArabicRule(void)
{
	if(OldSuffix) delete[] OldSuffix;
	if(NewSuffix) delete[] NewSuffix;
}



//-----------------------------------------------------------------------------
//
// class GLangAR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangAR::GLangAR(GFactoryLang* fac)
	: GLang(fac,"Arabic","ar"), Rules1(0), Rules2(0), Rules3(0), Rules4(0),
		 Rules5(0), Rules6(0), Rules7(0)
{
 	// Create Rules
	Rules0=new RContainer<ArabicRule,true,false>(5,5);
	Rules1=new RContainer<ArabicRule,true,false>(5,5);
	Rules2=new RContainer<ArabicRule,true,false>(5,5);
	Rules3=new RContainer<ArabicRule,true,false>(5,5);
	Rules4=new RContainer<ArabicRule,true,false>(5,5);
	Rules5=new RContainer<ArabicRule,true,false>(5,5);
	Rules6=new RContainer<ArabicRule,true,false>(5,5);
	Rules7=new RContainer<ArabicRule,true,false>(5,5);


	//Load rules;
	LoadRules();
}


//------------------------------------------------------------------------------
void GLangAR::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("RulesFile","/etc/galilei/arabic_rule.csv"));
	params->InsertParam(new RParamValue("Encoding","utf-8"));
}


//-----------------------------------------------------------------------------
void GALILEI::GLangAR::LoadRules(void)
{
	RString* tab;
	RString w;
	RChar separator(char(0x23)); // semi-colon ';' separator
	size_t tmp, i, nbargs=13;
	int sep;
	ArabicRule* rule;

	try
	{
		//Read Rules
		RTextFile rules_file(Factory->Get("RulesFile"), Factory->Get("Encoding").Latin1());
		rules_file.Open(RIO::Read);

		//treat on read line
		while (!rules_file.End())
		{
			w=rules_file.GetWord();

			//load one rule
			tab=new RString[nbargs];
			tmp=0;
			for (i=0; i<nbargs;i++)
			{
				sep=w.Find(separator,tmp);
				if( sep==-1)
				{
					tab[i++]=w.Mid(tmp);
					break;
				}
				else
				{
					tab[i]=w.Mid(tmp, (sep-tmp));
					tmp=sep+1;
				}
			}
			if (i<nbargs)
				cout <<" not enough argument found in arabic rules loading..."<<i << "arguments found ("<<nbargs<<" needed)"<<endl;

			//insert rule
			int before_pos=(!tab[4].GetLen())? -1: atoi(tab[4].Latin1());;
			int after_pos= (!tab[5].GetLen())? -1: atoi(tab[5].Latin1());
			int equality_pos=(!tab[7].GetLen())? -1: atoi(tab[7].Latin1())-1;
			int forbidden_pos=(!tab[10].GetLen())? -1: atoi(tab[10].Latin1())-1;;
			RString old_suffix=(tab[2].GetLen())? tab[2].Mid(1,tab[2].GetLen()-2) : tab[2];//remove the " characters
			RString new_suffix=(tab[3].GetLen())? tab[3].Mid(1,tab[3].GetLen()-2) : tab[3]; //remove the " characters
			RString equality=(tab[8].GetLen())? tab[8].Mid(1,tab[8].GetLen()-2) : tab[8]; //remove the " characters
			RString forbidden_letter=(tab[9].GetLen())? tab[9].Mid(1,tab[9].GetLen()-2) : tab[9]; //remove the " characters;
			bool cycle_rule=(!tab[11].GetLen())? 0: atoi(tab[11].Latin1()); //remove the " characters;
			bool next_level=(!tab[12].GetLen())? 0: atoi(tab[12].Latin1()); //remove the " characters;
			rule=new ArabicRule(atoi(tab[1].Latin1()),atoi(tab[0].Latin1()),old_suffix,new_suffix,before_pos, after_pos,
				atoi(tab[6].Latin1()),equality_pos,equality,forbidden_pos,forbidden_letter, cycle_rule, next_level);
			switch(rule->Level)
			{
				case(0):
					Rules0->InsertPtr(rule);
					break;
				case(1):
					Rules1->InsertPtr(rule);
					break;
				case(2):
					Rules2->InsertPtr(rule);
					break;
				case(3):
					Rules3->InsertPtr(rule);
					break;
				case(4):
					Rules4->InsertPtr(rule);
					break;
				case(5):
					Rules5->InsertPtr(rule);
					break;
				case(6):
					Rules6->InsertPtr(rule);
					break;
				case(7):
					Rules7->InsertPtr(rule);
					break;
				default:
					cout <<" wrong arabic rule level"<<endl;
					break;
			}
			if(tab)
				delete[] tab;
		}
	}
	catch(...)
	{
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GLangAR::ApplyRules(RString& kwd,RContainer<ArabicRule,true,false>* rules)
{
	RString tmp=kwd.Mid(0,strlen(kwd));

	RCursor<ArabicRule> Cur(*rules);
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
RString GALILEI::GLangAR::GetStemming(const RString& _kwd)
{
	RString res;
	RString kwd;

	kwd=_kwd;
	return kwd;

	// Do the different steps of the Porter algorithm.
	ApplyRules(kwd,Rules0);
	ApplyRules(kwd,Rules1);
	ApplyRules(kwd,Rules2);
	ApplyRules(kwd,Rules3);
	ApplyRules(kwd,Rules4);
	ApplyRules(kwd,Rules5);
	ApplyRules(kwd,Rules6);
	ApplyRules(kwd,Rules7);

	// Put the result in res and return it.
	res=kwd;
	return(res);
}


//-----------------------------------------------------------------------------
GALILEI::GLangAR::~GLangAR(void)
{
	if (Rules0)	delete Rules0;
	if (Rules1)	delete Rules1;
	if (Rules2)	delete Rules2;
	if (Rules3)	delete Rules3;
	if (Rules4)	delete Rules4;
	if (Rules5)	delete Rules5;
	if (Rules6)	delete Rules6;
	if (Rules7)	delete Rules7;
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Arabic",GLangAR,"ar")
