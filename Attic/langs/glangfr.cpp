/*

	GALILEI Research Project

	GLangFR.cpp

	French Language - Implementation.

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
#include <glangs/glangfr.h>
using namespace GALILEI;
using namespace RStd;


//-----------------------------------------------------------------------------
//
// class FrenchPorterRule
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GLangFR::FrenchPorterRule
{
public:
	char* OldSuffix;
	char* NewSuffix;
	int OldOffset;
	int NewOffset;
	int MinRootSize;
	
	FrenchPorterRule(char* os,char* ns,int oo,int no,int mr=-1);
	int Compare(const FrenchPorterRule*) {return(-1);}
};


//-----------------------------------------------------------------------------
GLangFR::FrenchPorterRule::FrenchPorterRule(char* os,char* ns,int oo,int no,int mr)
	: OldSuffix(0), NewSuffix(0), OldOffset(oo), NewOffset(no), MinRootSize(mr)
{
	OldSuffix=strdup(os);
	NewSuffix=strdup(ns);
}



//-----------------------------------------------------------------------------
//
// class GLangFR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangFR::GLangFR(void) throw(bad_alloc)
	: GLang("French","fr"), Rules1(0)
{
	
	Rules1=new RContainer<FrenchPorterRule,unsigned int,true,false>(130,10);
	Rules1->InsertPtr(new FrenchPorterRule("issaient","", 7,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ellement","el", 7,1,1));
	Rules1->InsertPtr(new FrenchPorterRule("issement","", 7,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("alement","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eraient","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("amment","ant",5 ,2 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("emment","ent",5 ,2 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eaient","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issait","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�rent","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("erait","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iront","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ement","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("aient","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�rent","",4 , -1,1));
	Rules1->InsertPtr(new FrenchPorterRule("eont","", 3,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eant","", 3,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eait","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ient","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ent","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ont","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ant","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ait","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("at","",1,-1,1));
	Rules1->InsertPtr(new FrenchPorterRule("it","",1 , -1,1));
	Rules1->InsertPtr(new FrenchPorterRule("t","", 0, 0,1));

	Rules1->InsertPtr(new FrenchPorterRule("uction","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ication","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iation","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ation","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("tion","t",3 ,0 ,2));

	Rules1->InsertPtr(new FrenchPorterRule("ateur","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eur","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ier","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("er","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ir","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("r","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("issiez","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issez","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iriez","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("erez","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("irez","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iez","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ez","",1 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("erai","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ai","",1 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("ira","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("era","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("a","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("f","v",0 ,0 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("yeux","oeil",3 ,3 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eux","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("aux","al",2 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("x","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("eresse","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("esse","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("euse","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("oise","o",3 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ouse","ou",3 ,1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("i�re","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("aire","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ure","",2 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("erie","",3 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("�tude","et",4 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("itude","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ade","",2 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("isme","",3 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("age","",2 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("trice","",4 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("cque","c",3 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("que","c",2 ,0 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("eille","eil",4 ,2 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("elle","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("able","",3 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("iste","", 3,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�te","et",2 ,1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("ulle","ul",3 ,1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("gue","g",2 ,0 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("nne","n",2 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("tte","",2 ,0 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("it�","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("t�","",1 ,-1 ,1));
    Rules1->InsertPtr(new FrenchPorterRule("�e","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("aise","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ite","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ee","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("e","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("issements","",8 ,-1 ,2));
	Rules1->InsertPtr(new FrenchPorterRule("ications","",7 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eresses","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issions","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iations","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ements","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issons","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�tudes","et",5 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("itudes","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issais","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("trices","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eilles","eil",5 ,2 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("irions","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("erions","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ateurs","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ations","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("tions","",4 ,-1 ,3));
	Rules1->InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("irons","",4 ,-1 ,1));
    Rules1->InsertPtr(new FrenchPorterRule("erons","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("esses","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("euses","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ulles","ul",4 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("cques","c",4 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("elles","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("istes","", 4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("aises","", 4 ,-1 ,2));
	Rules1->InsertPtr(new FrenchPorterRule("oises","o",4 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("tions","t",4 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ouses","ou",4 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("i�res","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eries","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("antes","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ismes","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("erais","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ents","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�tes","et",3 ,1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ions","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("it�s","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ites","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iers","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iras","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eras","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ures","",3 ,-1 ,2));
	Rules1->InsertPtr(new FrenchPorterRule("ants","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�mes","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ades","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ages","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ques","c",3 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("gues","g",3 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("nnes","n",3 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ttes","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�tes","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("t�s","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ons","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ais","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�es","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ees","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("as","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("es","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("fs","v",1 ,0 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("�s","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("is","",1 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("s","",0 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("eau","",2 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("au","",2 ,-1 ,1));


	Rules2=new RContainer<FrenchPorterRule,unsigned int,true,false>(5,10);
	
	Rules2->InsertPtr(new FrenchPorterRule("ent","", 2,-1,2));
	Rules2->InsertPtr(new FrenchPorterRule("ation","",4 ,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("ition","t",4 ,0 ,1));
	Rules2->InsertPtr(new FrenchPorterRule("tion","",3 ,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("el","", 1,-1,2));
	
	Rules3=new RContainer<FrenchPorterRule,unsigned int,true,false>(5,10);
	
	Rules3->InsertPtr(new FrenchPorterRule("nn","n",1 ,0,1));
	Rules3->InsertPtr(new FrenchPorterRule("ll","l",1 ,0,1));
	Rules3->InsertPtr(new FrenchPorterRule("tt","t",1 ,0,1));
	Rules3->InsertPtr(new FrenchPorterRule("y","",0 ,-1,1));
	Rules3->InsertPtr(new FrenchPorterRule("i","",0 ,-1,1));
	Rules3->InsertPtr(new FrenchPorterRule("qu","c",1 ,0,1));

	


	SkipWords.InsertPtr(new SkipWord("ier"));
	SkipWords.InsertPtr(new SkipWord("iers"));
	SkipWords.InsertPtr(new SkipWord("�m"));
	SkipWords.InsertPtr(new SkipWord("�me"));
	SkipWords.InsertPtr(new SkipWord("�mes"));
	SkipWords.InsertPtr(new SkipWord("er"));
	SkipWords.InsertPtr(new SkipWord("ers"));
	SkipWords.InsertPtr(new SkipWord("re"));
	SkipWords.InsertPtr(new SkipWord("res"));
	SkipWords.InsertPtr(new SkipWord("e"));
	SkipWords.InsertPtr(new SkipWord("aine"));
	SkipWords.InsertPtr(new SkipWord("aines"));
	SkipWords.InsertPtr(new SkipWord("i�me"));
	SkipWords.InsertPtr(new SkipWord("i�mes"));
}


//-----------------------------------------------------------------------------
int GALILEI::GLangFR::GetWordSize(char* kwd)
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
bool GALILEI::GLangFR::ContainsVowel(const char* kwd)
{
	if(!(*kwd))
		return(false);
	else
		return((IsVowel(*kwd))||(!strpbrk(kwd+1,"aeiouy")));
}


//-----------------------------------------------------------------------------
bool GALILEI::GLangFR::ApplyRules(char* kwd,char* &end,RContainer<FrenchPorterRule,unsigned int,true,false>* rules,int ruleslevel)
{
	FrenchPorterRule* ptr;
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

		// Replace the old suffix by the new one, update len and return 'Next'.
		strcat(kwd,ptr->NewSuffix);
		end=ending+ptr->NewOffset;
		return(true);
	}
	return(false);
}


//-----------------------------------------------------------------------------
RString& GALILEI::GLangFR::GetStemming(const RString& _kwd)
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

	// Do the unique step(for the moment) of the french adapation ofPorter algorithm.
	cout<<" pas de regle "<<kwd<<"123"<<endl;
	ApplyRules(kwd,end,Rules1,1);
	cout<<"regle1"<<kwd<<"123"<<endl;
	if(!strcmp(kwd,"employ"))
	{
	cout<<"tamere                111111111111111111111111111111111111111111"<<endl;
	}
	ApplyRules(kwd,end,Rules2,2);
	cout<<"regle2"<<kwd<<"123"<<endl;
	ApplyRules(kwd,end,Rules3,3);
	cout<<"regle3"<<kwd<<"123"<<endl;
	
	// Put the result in res and return it.
	while((*end)&&(!isspace(*end)))
		end++;
	if(*end)
		(*end)=0;
	(*res)=kwd;
	return((*res));
}


//-----------------------------------------------------------------------------
GALILEI::GLangFR::~GLangFR(void)
{
	if(Rules1) delete Rules1;
	if(Rules2) delete Rules2;
	if(Rules3) delete Rules3;
	
}
