/*

	GALILEI Research Project

	LangNL.cpp

	Dutch Language - Implementation.

	Copyright 2001 by Nicolas Kumps.
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
// include files for ANSI C/C++
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langnl.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class PorterRule;
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GLangNL::DutchPorterRule
{
public:
	enum tCondition{CondNothing,CondEndsWithCons,CondNotc,CondNote,CondUndouble};

	char* OldSuffix;
	char* NewSuffix;
	int OldOffset;
	int NewOffset;
	int MinRootSize;
	tCondition Condition;
	bool Next;

	DutchPorterRule(const char* os,const char* ns,int oo,int no,int mr=-1,tCondition c=CondNothing,bool n=false);
	int Compare(const DutchPorterRule&) const {return(-1);}
	~DutchPorterRule(void);
};


//-----------------------------------------------------------------------------
GLangNL::DutchPorterRule::DutchPorterRule(const char* os,const char* ns,int oo,int no,int mr,tCondition c,bool n)
	: OldSuffix(0), NewSuffix(0), OldOffset(oo), NewOffset(no), MinRootSize(mr),
	  Condition(c), Next(n)
{
	size_t l;
	l=strlen(os);
	OldSuffix=new char[l+1];
	if(l)
		memcpy(OldSuffix,os,sizeof(char)*(l+1));
	else
		(*OldSuffix)=0;
	l=strlen(ns);
	NewSuffix=new char[l+1];
	if(l)
		memcpy(NewSuffix,ns,sizeof(char)*(l+1));
	else
		(*NewSuffix)=0;
}


//-----------------------------------------------------------------------------
GLangNL::DutchPorterRule::~DutchPorterRule(void)
{
	if(OldSuffix) delete[] OldSuffix;
	if(NewSuffix) delete[] NewSuffix;
}



//-----------------------------------------------------------------------------
//
// class GLangNL
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangNL::GLangNL(GFactoryLang* fac)
	: GLang(fac,"Dutch","nl"), Rules1a(0), Rules1b(0), Rules1c(0), Rules2a(0),
	  Rules2aa(0), Rules3a(0), Rules3b(0), Rules4(0), Rules5(0)
{
	//Rules 1a
	Rules1a=new RContainer<DutchPorterRule,true,false>(1,2);
	Rules1a->InsertPtr(new DutchPorterRule("heden","heid",4,3,0));


	// Rules 1b
	Rules1b=new RContainer<DutchPorterRule,true,false>(2,2);
	Rules1b->InsertPtr(new DutchPorterRule("ene","",2,-1,0,DutchPorterRule::CondEndsWithCons));
	Rules1b->InsertPtr(new DutchPorterRule("en","",1,-1,0,DutchPorterRule::CondEndsWithCons));

	// Rules 1c
	Rules1c=new RContainer<DutchPorterRule,true,false>(2,2);
	Rules1c->InsertPtr(new DutchPorterRule("se","",1,-1,0,DutchPorterRule::CondEndsWithCons));
	Rules1c->InsertPtr(new DutchPorterRule("s","",0,-1,0,DutchPorterRule::CondEndsWithCons));

	//Rules 2a
	Rules2a=new RContainer<DutchPorterRule,true,false>(1,2);
	Rules2a->InsertPtr(new DutchPorterRule("e","",0,-1,0,DutchPorterRule::CondEndsWithCons,true));

	//Rules 2aa
	Rules2aa=new RContainer<DutchPorterRule,true,false>(20,5);
	Rules2aa->InsertPtr(new DutchPorterRule("bb","b",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("cc","c",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("dd","d",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("ff","f",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("gg","g",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("hh","h",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("jj","j",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("kk","k",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("ll","l",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("mm","m",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("nn","n",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("pp","p",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("qq","q",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("rr","r",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("ss","s",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("tt","t",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("vv","v",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("ww","w",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("xx","x",1,0));
	Rules2aa->InsertPtr(new DutchPorterRule("zz","z",1,0));

	//Rules 3a
	Rules3a=new RContainer<DutchPorterRule,true,false>(1,2);
	Rules3a->InsertPtr(new DutchPorterRule("heid","",3,-1,1,DutchPorterRule::CondNotc,true));

	// Rules 3b
	Rules3b=new RContainer<DutchPorterRule,true,false>(6,5);
	Rules3b->InsertPtr(new DutchPorterRule("end","",2,-1,1));
	Rules3b->InsertPtr(new DutchPorterRule("ing","",2,-1,1,DutchPorterRule::CondNote));
	Rules3b->InsertPtr(new DutchPorterRule("ig","",1,-1,1,DutchPorterRule::CondNote));
	Rules3b->InsertPtr(new DutchPorterRule("lijk","",3,-1,1,DutchPorterRule::CondNothing,true));
	Rules3b->InsertPtr(new DutchPorterRule("baar","",3,-1,1));
	Rules3b->InsertPtr(new DutchPorterRule("bar","",2,-1,1));

	//Rules 4
	Rules4=new RContainer<DutchPorterRule,true,false>(4,2);
	Rules4->InsertPtr(new DutchPorterRule("aa","a",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4->InsertPtr(new DutchPorterRule("oo","o",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4->InsertPtr(new DutchPorterRule("uu","u",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4->InsertPtr(new DutchPorterRule("ee","e",2,1,-1,DutchPorterRule::CondUndouble));

	//Rule 5
	Rules5=new RContainer<DutchPorterRule,true,false>(1,2);
	Rules5->InsertPtr(new DutchPorterRule("v","f",0,0,-1));


	// Skip Words //savoir comment on abr�ie 1er, 2eme, 3e ...
	SkipSequence("th");
	SkipSequence("st");
	SkipSequence("nd");
	SkipSequence("rd");
	SkipSequence("s");
	SkipSequence("ies");
}


//-----------------------------------------------------------------------------
int GLangNL::GetWordSize(char* kwd)
{
	int Result=0;             // WordSize of the word.
	int State=0;              // Current state of the machine.

	while(*kwd)
	{
		switch(State)
		{
			case 0:
				State=(IsVowel(*kwd))?1:2;
				break;

			case 1:
				State=(IsVowel(*kwd))?1:2;
				if(State==2) Result++;
				break;

			case 2:
				State=((IsVowel(*kwd))||((*kwd)=='y'))?1:2;
				break;
		}
		kwd++;
	}
	return(Result);
}


//-----------------------------------------------------------------------------
bool GLangNL::EndsWithCons(const char* newend)
{
	if(!(*newend))
		return(false);
	else
		return(!strchr("a�e����u�y",*newend));
}


//-----------------------------------------------------------------------------
bool GLangNL::EndsWithc(const char* newend)
{
	if(!(*newend))
		return(false);
	else
		return(strchr("c",*newend));
}


//-----------------------------------------------------------------------------
bool GLangNL::EndsWithe(const char* newend)
{
	if(!(*newend))
		return(false);
	else
		return(strchr("e�",*newend));
}


//-----------------------------------------------------------------------------
bool GLangNL::EndsWithCVD(char* tmp,char* newend)
{
	size_t length;
	char* end;
	if((length=strlen(tmp))<3)
		return(false);
	end=tmp+length-1;
	return(  (!strchr("a�e����u�yl",*end--))  &&     // Consonant
	         (strchr("a�e�o�u",*end--))     &&     // Vowel
	         (strchr("a�e�o�u",*end))     &&     // Vowel
	         (!strchr("a�e����u�y",*newend))              // Consonant
	      );
}

//-----------------------------------------------------------------------------
bool GLangNL::ApplyRules(char* kwd,char* &end,RContainer<DutchPorterRule,true,false>* rules)
{
	DutchPorterRule* ptr;
	char* ending;
	int WordSize;
	char tmp;
	char* newend;
	size_t len;

	len=strlen(kwd);
	RCursor<DutchPorterRule> Cur(*rules);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		ptr=Cur();

		// If the word is leng enough, find the potentiel end suffix and put it
		// in ending. If the ending isn't corresponding to the rule's suffix,
		// go to the next rule.
		if(len<=static_cast<size_t>(ptr->OldOffset)) continue;
		ending=end-ptr->OldOffset;
		if(ptr->Condition==DutchPorterRule::CondUndouble)
		{
			if(strncmp(ending,ptr->OldSuffix,2)) continue;
		}
		else
		{
			if(strcmp(ending,ptr->OldSuffix)) continue;
		}
		tmp=*ending;
		newend=ending-1;
		(*ending)=0;
		WordSize=GetWordSize(kwd);

		// Verify if the minimum root size is Ok.
		if(ptr->MinRootSize>=WordSize)
		{
			(*ending)=tmp;
			continue;
		}

		// If there is a condition verify it.
		switch(ptr->Condition)
		{
			case DutchPorterRule::CondEndsWithCons:
				if(!EndsWithCons(newend))
				{
					(*ending)=tmp;
					return(ptr->Next);
				}
				break;
			case DutchPorterRule::CondNotc:
				if(EndsWithc(newend))
				{
					(*ending)=tmp;
					return(ptr->Next);
				}
				break;
			case DutchPorterRule::CondNote:
				if(EndsWithe(newend))
				{
					(*ending)=tmp;
					return(ptr->Next);
				}
				break;
			case DutchPorterRule::CondUndouble:
				{
					(*ending)=tmp;
					if(!EndsWithCVD(ending,newend))
						return(ptr->Next);
					else (*ending)=0;
				}
				break;
			default:
				break;
		}

		// Replace the old suffix by the new one, update len and return 'Next'.
		strcat(kwd,ptr->NewSuffix);
		if(ptr->Condition==DutchPorterRule::CondUndouble)
			strncat(kwd,(end), strlen(end));
		end=ending+ptr->NewOffset;
		return(ptr->Next);
	}
	return(false);
}


//-----------------------------------------------------------------------------
RString GLangNL::GetStemming(const RString& _kwd)
{
	RString res;
	char kwd[51];
	char* end;
	size_t len;

	// Transform _kwd in lowercase and store it in kwd.
	res=_kwd.ToLower();
	if(res.GetLen()>50) return(res);
	strcpy(kwd,res);
	len=_kwd.GetLen()-1;
	end=&kwd[len];

	// Do the different steps of the Porter algorithm.
	ApplyRules(kwd,end,Rules1a);
	ApplyRules(kwd,end,Rules1b);
	ApplyRules(kwd,end,Rules1c);
	if(ApplyRules(kwd,end,Rules2a))
		ApplyRules(kwd,end,Rules2aa);
	if(ApplyRules(kwd,end,Rules3a))
		ApplyRules(kwd,end,Rules1b);
	if(ApplyRules(kwd,end,Rules3b))
		if(ApplyRules(kwd,end,Rules2a))
			ApplyRules(kwd,end,Rules2aa);
	ApplyRules(kwd,end,Rules4);
	ApplyRules(kwd,end,Rules5);


	// Put the result in res and return it.
	end=kwd;
	while((*end)&&(!isspace(*end)))
		end++;
	if(*end)
		(*end)=0;
	res=kwd;
	return(res);
}


//------------------------------------------------------------------------------
void GLangNL::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangNL::~GLangNL(void)
{
	if(Rules1a) delete Rules1a;
	if(Rules1b) delete Rules1b;
	if(Rules1c) delete Rules1c;
	if(Rules2a) delete Rules2a;
	if(Rules2aa) delete Rules2aa;
	if(Rules3a) delete Rules3a;
	if(Rules3b) delete Rules3b;
	if(Rules4) delete Rules4;
	if(Rules5) delete Rules5;
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Dutch",GLangNL,"nl")

