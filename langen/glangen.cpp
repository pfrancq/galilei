/*

	GALILEI Research Project

	GLangEN.cpp

	English Language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <string.h>
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glangen.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class PorterRule;
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GLangEN::PorterRule
{
public:
	enum tCondition{CondNothing,CondContainsVowel,CondAddAnE,CondRemoveAnE};

	char* OldSuffix;
	char* NewSuffix;
	int OldOffset;
	int NewOffset;
	int MinRootSize;
	tCondition Condition;
	bool Next;

	PorterRule(char* os,char* ns,int oo,int no,int mr=-1,tCondition c=CondNothing,bool n=false);
	int Compare(const PorterRule*) {return(-1);}
};


//-----------------------------------------------------------------------------
GLangEN::PorterRule::PorterRule(char* os,char* ns,int oo,int no,int mr,tCondition c,bool n)
	: OldSuffix(0), NewSuffix(0), OldOffset(oo), NewOffset(no), MinRootSize(mr),
	  Condition(c), Next(n)
{
	OldSuffix=strdup(os);
	NewSuffix=strdup(ns);
}



//-----------------------------------------------------------------------------
//
// class GLangEN
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangEN::GLangEN(void) throw(bad_alloc)
	: GLang("English","en"), Rules1a(0), Rules1b(0), Rules1bb(0), Rules1c(0),
	  Rules2(0), Rules3(0), Rules4(0), Rules5a(0), Rules5b(0)
{
	// Rules 1a
	Rules1a=new RContainer<PorterRule,unsigned int,true,false>(4,2);
	Rules1a->InsertPtr(new PorterRule("sses","ss",3,1));
	Rules1a->InsertPtr(new PorterRule("ies","i",2,0));
	Rules1a->InsertPtr(new PorterRule("ss","ss",1,1));
	Rules1a->InsertPtr(new PorterRule("s","",0,-1));

	// Rules 1b
	Rules1b=new RContainer<PorterRule,unsigned int,true,false>(3,2);
	Rules1b->InsertPtr(new PorterRule("eed","ee",2,1,0,PorterRule::CondNothing,true));
	Rules1b->InsertPtr(new PorterRule("ed"," ",1,-1,-1,PorterRule::CondContainsVowel,true));
	Rules1b->InsertPtr(new PorterRule("ing"," ",2,-1,-1,PorterRule::CondContainsVowel,true));

	// Rules 1bb
	Rules1bb=new RContainer<PorterRule,unsigned int,true,false>(15,5);
	Rules1bb->InsertPtr(new PorterRule("at","ate",1,2));
	Rules1bb->InsertPtr(new PorterRule("bl","ble",1,2));
	Rules1bb->InsertPtr(new PorterRule("iz","ize",1,2));
	Rules1bb->InsertPtr(new PorterRule("bb","b",1,0));
	Rules1bb->InsertPtr(new PorterRule("dd","d",1,0));
	Rules1bb->InsertPtr(new PorterRule("ff","f",1,0));
	Rules1bb->InsertPtr(new PorterRule("gg","g",1,0));
	Rules1bb->InsertPtr(new PorterRule("mm","m",1,0));
	Rules1bb->InsertPtr(new PorterRule("nn","n",1,0));
	Rules1bb->InsertPtr(new PorterRule("pp","p",1,0));
	Rules1bb->InsertPtr(new PorterRule("rr","r",1,0));
	Rules1bb->InsertPtr(new PorterRule("tt","t",1,0));
	Rules1bb->InsertPtr(new PorterRule("ww","w",1,0));
	Rules1bb->InsertPtr(new PorterRule("xx","x",1,0));
	Rules1bb->InsertPtr(new PorterRule(" ","e",0,0));

	// Rules 1c
	Rules1c=new RContainer<PorterRule,unsigned int,true,false>(1,2);
	Rules1c->InsertPtr(new PorterRule("y","i",0,0,-1,PorterRule::CondContainsVowel));

	// Rules 2
	Rules2=new RContainer<PorterRule,unsigned int,true,false>(20,5);
	Rules2->InsertPtr(new PorterRule("ational","ate",6,2,0));
	Rules2->InsertPtr(new PorterRule("tional","tion",5,3,0));
	Rules2->InsertPtr(new PorterRule("enci","ence",3,3,0));
	Rules2->InsertPtr(new PorterRule("anci","ance",3,3,0));
	Rules2->InsertPtr(new PorterRule("izer","ize",3,2,0));
	Rules2->InsertPtr(new PorterRule("abli","able",3,3,0));
	Rules2->InsertPtr(new PorterRule("alli","al",3,1,0));
	Rules2->InsertPtr(new PorterRule("entli","ent",4,2,0));
	Rules2->InsertPtr(new PorterRule("eli","e",2,0,0));
	Rules2->InsertPtr(new PorterRule("ousli","ous",4,2,0));
	Rules2->InsertPtr(new PorterRule("ization","ize",6,2,0));
	Rules2->InsertPtr(new PorterRule("ation","ate",4,2,0));
	Rules2->InsertPtr(new PorterRule("ator","ate",3,2,0));
	Rules2->InsertPtr(new PorterRule("alism","al",4,1,0));
	Rules2->InsertPtr(new PorterRule("iveness","ive",6,2,0));
	Rules2->InsertPtr(new PorterRule("fulnes","ful",5,2,0));
	Rules2->InsertPtr(new PorterRule("ousness","ous",6,2,0));
	Rules2->InsertPtr(new PorterRule("aliti","al",4,1,0));
	Rules2->InsertPtr(new PorterRule("iviti","ive",4,2,0));
	Rules2->InsertPtr(new PorterRule("biliti","ble",5,2,0));

	// Rules 3
	Rules3=new RContainer<PorterRule,unsigned int,true,false>(7,5);
	Rules3->InsertPtr(new PorterRule("icate","ic",4,1,0));
	Rules3->InsertPtr(new PorterRule("ative"," ",4,-1,0));
	Rules3->InsertPtr(new PorterRule("alize","al",4,1,0));
	Rules3->InsertPtr(new PorterRule("iciti","ic",4,1,0));
	Rules3->InsertPtr(new PorterRule("ical","ic",3,1,0));
	Rules3->InsertPtr(new PorterRule("ful"," ",2,-1,0));
	Rules3->InsertPtr(new PorterRule("ness"," ",3,-1,0));

	// Rules 4
	Rules4=new RContainer<PorterRule,unsigned int,true,false>(20,5);
	Rules4->InsertPtr(new PorterRule("al"," ",1,-1,1));
	Rules4->InsertPtr(new PorterRule("ance"," ",3,-1,1));
	Rules4->InsertPtr(new PorterRule("ence"," ",3,-1,1));
	Rules4->InsertPtr(new PorterRule("er"," ",1,-1,1));
	Rules4->InsertPtr(new PorterRule("ic"," ",1,-1,1));
	Rules4->InsertPtr(new PorterRule("able"," ",3,-1,1));
	Rules4->InsertPtr(new PorterRule("ible"," ",3,-1,1));
	Rules4->InsertPtr(new PorterRule("ant"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("ement"," ",4,-1,1));
	Rules4->InsertPtr(new PorterRule("ment"," ",3,-1,1));
	Rules4->InsertPtr(new PorterRule("ent"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("sion","s",3,0,1));
	Rules4->InsertPtr(new PorterRule("tion","t",3,0,1));
	Rules4->InsertPtr(new PorterRule("ou"," ",1,-1,1));
	Rules4->InsertPtr(new PorterRule("ism"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("ate"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("iti"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("ous"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("ive"," ",2,-1,1));
	Rules4->InsertPtr(new PorterRule("ize"," ",2,-1,1));

	// Rules 5a
	Rules5a=new RContainer<PorterRule,unsigned int,true,false>(2,5);
	Rules5a->InsertPtr(new PorterRule("e"," ",0,-1,1));
	Rules5a->InsertPtr(new PorterRule("e"," ",0,-1,-1));

	// Rules 5b
	Rules5b=new RContainer<PorterRule,unsigned int,true,false>(1,5);
	Rules5b->InsertPtr(new PorterRule("ll","l",1,0,1));
}


//-----------------------------------------------------------------------------
int GALILEI::GLangEN::GetWordSize(char* kwd)
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
bool GALILEI::GLangEN::ContainsVowel(const char* kwd)
{
	if(!(*kwd))
		return(false);
	else
		return((IsVowel(*kwd))||(!strpbrk(kwd+1,"aeiouy")));
}


//-----------------------------------------------------------------------------
bool GALILEI::GLangEN::EndsWithCVC(char* kwd,char* &end)
{
	int length;

	if((length=strlen(kwd))<2)
		return(false);
	else
	{
		end=kwd+length-1;
		return((!strchr("aeiouwxy",(*end)--))&&    // Consonant
			(!strchr("aeiouy",(*end)--))&&         // Vowel
			(strchr("aeiou",(*end))));             // Consonant
	}
}

//-----------------------------------------------------------------------------
bool GALILEI::GLangEN::ApplyRules(char* kwd,char* &end,RContainer<PorterRule,unsigned int,true,false>* rules)
{
	PorterRule* ptr;
	char* ending;
	int WordSize;
	char tmp;
	int len;

	WordSize=GetWordSize(kwd);
	len=strlen(kwd);
	for(rules->Start();!rules->End();rules->Next())
	{
		ptr=(*rules)();

		// If the word is leng enough, find the potentiel end suffix and put it
		// in ending. If the ending isn't corresponding to the rule's suffix,
		// go to the next rule.
		if(len<=ptr->OldOffset) continue;
		ending=end-ptr->OldOffset;
		if(strcmp(ending,ptr->OldSuffix)) continue;
		tmp=*ending;
		(*ending)=0;

		// Verify if the minimum root size is Ok.
		if(ptr->MinRootSize>WordSize) continue;

		// If there is a condition verify it.
		switch(ptr->Condition)
		{
			case PorterRule::CondContainsVowel:
				if(!ContainsVowel(kwd)) continue;
				break;
			case PorterRule::CondAddAnE:
				if(!((WordSize==1)&&(EndsWithCVC(kwd,end)))) continue;
				break;
			case PorterRule::CondRemoveAnE:
				if(!((WordSize==1)&&(!EndsWithCVC(kwd,end)))) continue;
				break;
			default:
				break;
		}

		// Replace the old suffix by the new one, update len and return 'Next'.
		strcat(kwd,ptr->NewSuffix);
		end=ending+ptr->NewOffset;
		return(ptr->Next);
	}
	return(false);
}


//-----------------------------------------------------------------------------
RString& GALILEI::GLangEN::GetStemming(const RString& _kwd)
{
	RString *res=RString::GetString();
	char kwd[50];
	char* end;
	unsigned int len;

	// Transform _kwd in lowercase and store it in kwd.
	(*res)=_kwd;
	res->StrLwr();
	strcpy(kwd,(*res)());
	len=_kwd.GetLen()-1;
	end=&kwd[len];

	// Do the different steps of the Porter algorithm.
	ApplyRules(kwd,end,Rules1a);
	if(ApplyRules(kwd,end,Rules1b))
		ApplyRules(kwd,end,Rules1bb);
	ApplyRules(kwd,end,Rules1c);
	ApplyRules(kwd,end,Rules2);
	ApplyRules(kwd,end,Rules3);
	ApplyRules(kwd,end,Rules4);
	ApplyRules(kwd,end,Rules5a);
	ApplyRules(kwd,end,Rules5b);

	// Put the result in res and return it.
	end=kwd;
	while((*end)&&(!isspace(*end)))
		end++;
	if(*end)
		(*end)=0;
	(*res)=kwd;
	return((*res));
}


//-----------------------------------------------------------------------------
GALILEI::GLangEN::~GLangEN(void)
{
	if(Rules1a) delete Rules1a;
	if(Rules1b) delete Rules1b;
	if(Rules1bb) delete Rules1bb;
	if(Rules1c) delete Rules1c;
	if(Rules2) delete Rules2;
	if(Rules3) delete Rules3;
	if(Rules4) delete Rules4;
	if(Rules5a) delete Rules5a;
	if(Rules5b) delete Rules5b;
}
