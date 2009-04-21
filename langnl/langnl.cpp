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
	memcpy(OldSuffix,os,sizeof(char)*(l+1));
	l=strlen(ns);
	NewSuffix=new char[l+1];
	memcpy(NewSuffix,ns,sizeof(char)*(l+1));
}


//-----------------------------------------------------------------------------
GLangNL::DutchPorterRule::~DutchPorterRule(void)
{
	delete[] OldSuffix;
	delete[] NewSuffix;
}



//-----------------------------------------------------------------------------
//
// class GLangNL
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangNL::GLangNL(GFactoryLang* fac)
	: GLang(fac,"Dutch","nl"), Rules1a(1), Rules1b(2), Rules1c(2), Rules2a(1),
	  Rules2aa(20), Rules3a(1), Rules3b(6), Rules4(4), Rules5(1)
{
	// Rules 1a
	Rules1a.InsertPtr(new DutchPorterRule("heden","heid",4,3,0));


	// Rules 1b
	Rules1b.InsertPtr(new DutchPorterRule("ene","",2,-1,0,DutchPorterRule::CondEndsWithCons));
	Rules1b.InsertPtr(new DutchPorterRule("en","",1,-1,0,DutchPorterRule::CondEndsWithCons));

	// Rules 1c
	Rules1c.InsertPtr(new DutchPorterRule("se","",1,-1,0,DutchPorterRule::CondEndsWithCons));
	Rules1c.InsertPtr(new DutchPorterRule("s","",0,-1,0,DutchPorterRule::CondEndsWithCons));

	// Rules 2a
	Rules2a.InsertPtr(new DutchPorterRule("e","",0,-1,0,DutchPorterRule::CondEndsWithCons,true));

	// Rules 2aa
	Rules2aa.InsertPtr(new DutchPorterRule("bb","b",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("cc","c",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("dd","d",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("ff","f",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("gg","g",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("hh","h",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("jj","j",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("kk","k",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("ll","l",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("mm","m",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("nn","n",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("pp","p",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("qq","q",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("rr","r",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("ss","s",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("tt","t",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("vv","v",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("ww","w",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("xx","x",1,0));
	Rules2aa.InsertPtr(new DutchPorterRule("zz","z",1,0));

	// Rules 3a
	Rules3a.InsertPtr(new DutchPorterRule("heid","",3,-1,1,DutchPorterRule::CondNotc,true));

	// Rules 3b
	Rules3b.InsertPtr(new DutchPorterRule("end","",2,-1,1));
	Rules3b.InsertPtr(new DutchPorterRule("ing","",2,-1,1,DutchPorterRule::CondNote));
	Rules3b.InsertPtr(new DutchPorterRule("ig","",1,-1,1,DutchPorterRule::CondNote));
	Rules3b.InsertPtr(new DutchPorterRule("lijk","",3,-1,1,DutchPorterRule::CondNothing,true));
	Rules3b.InsertPtr(new DutchPorterRule("baar","",3,-1,1));
	Rules3b.InsertPtr(new DutchPorterRule("bar","",2,-1,1));

	// Rules 4
	Rules4.InsertPtr(new DutchPorterRule("aa","a",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4.InsertPtr(new DutchPorterRule("oo","o",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4.InsertPtr(new DutchPorterRule("uu","u",2,1,-1,DutchPorterRule::CondUndouble));
	Rules4.InsertPtr(new DutchPorterRule("ee","e",2,1,-1,DutchPorterRule::CondUndouble));

	// Rule 5
	Rules5.InsertPtr(new DutchPorterRule("v","f",0,0,-1));

	// Skip Words
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
		return(!strchr("aáäeéëèiíoóöuóöy",*newend));
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
		return(strchr("eéë",*newend));
}


//-----------------------------------------------------------------------------
bool GLangNL::EndsWithCVD(char* tmp,char* newend)
{
	size_t length;
	char* end;
	if((length=strlen(tmp))<3)
		return(false);
	end=tmp+length-1;
	return(  (!strchr("aáäeéëèiíoóöuóöyl",*end--))  &&    // Consonant
	         (strchr("aáäeéëoóöuúü",*end--))     &&       // Vowel
	         (strchr("aáäeéëoóöuúü",*end))     &&         // Vowel
	         (!strchr("aáäeéëèiíoóöuóöy",*newend))        // Consonant
	      );
}


//-----------------------------------------------------------------------------
bool GLangNL::ApplyRules(char* kwd,char* &end,RContainer<DutchPorterRule,true,false>& rules)
{
	DutchPorterRule* ptr;
	char* ending;
	int WordSize;
	char tmp;
	char* newend;
	size_t len;

	len=strlen(kwd);
	RCursor<DutchPorterRule> Cur(rules);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		ptr=Cur();

		// If the word length is enough, find the potential end suffix and put it
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
void GLangNL::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("aan"));
	stop.InsertPtr(new RString("aandacht"));
	stop.InsertPtr(new RString("aandoen"));
	stop.InsertPtr(new RString("aandoet"));
	stop.InsertPtr(new RString("aandringen"));
	stop.InsertPtr(new RString("aangeeft"));
	stop.InsertPtr(new RString("aangegaan"));
	stop.InsertPtr(new RString("aangegeven"));
	stop.InsertPtr(new RString("aangehouden"));
	stop.InsertPtr(new RString("aangewend"));
	stop.InsertPtr(new RString("aangezien"));
	stop.InsertPtr(new RString("aanhangend"));
	stop.InsertPtr(new RString("aanhouden"));
	stop.InsertPtr(new RString("aannemende"));
	stop.InsertPtr(new RString("aantal"));
	stop.InsertPtr(new RString("aantrekkelijk"));
	stop.InsertPtr(new RString("aantrekkelijke"));
	stop.InsertPtr(new RString("aantrekkelijker"));
	stop.InsertPtr(new RString("aantrekkelijkheid"));
	stop.InsertPtr(new RString("aanvang"));
	stop.InsertPtr(new RString("aanvankelijk"));
	stop.InsertPtr(new RString("aanvankelijke"));
	stop.InsertPtr(new RString("aanverwante"));
	stop.InsertPtr(new RString("aanwezig"));
	stop.InsertPtr(new RString("aanwezige"));
	stop.InsertPtr(new RString("aanwezigen"));
	stop.InsertPtr(new RString("aanwezigheid"));
	stop.InsertPtr(new RString("aanzien"));
	stop.InsertPtr(new RString("aanzienlijk"));
	stop.InsertPtr(new RString("aanzienlijke"));
	stop.InsertPtr(new RString("absoluut"));
	stop.InsertPtr(new RString("acht"));
	stop.InsertPtr(new RString("achter"));
	stop.InsertPtr(new RString("achtereenvolgens"));
	stop.InsertPtr(new RString("achterelkaar"));
	stop.InsertPtr(new RString("achterna"));
	stop.InsertPtr(new RString("achterwege"));
	stop.InsertPtr(new RString("achtig"));
	stop.InsertPtr(new RString("activiteit"));
	stop.InsertPtr(new RString("activiteiter"));
	stop.InsertPtr(new RString("actueel"));
	stop.InsertPtr(new RString("actuele"));
	stop.InsertPtr(new RString("adequaat"));
	stop.InsertPtr(new RString("adequate"));
	stop.InsertPtr(new RString("af"));
	stop.InsertPtr(new RString("afdoen"));
	stop.InsertPtr(new RString("afdoet"));
	stop.InsertPtr(new RString("afgaande"));
	stop.InsertPtr(new RString("afgedaan"));
	stop.InsertPtr(new RString("afgelopen"));
	stop.InsertPtr(new RString("afgenomen"));
	stop.InsertPtr(new RString("afgezien"));
	stop.InsertPtr(new RString("afhankelijk"));
	stop.InsertPtr(new RString("afhankelijke"));
	stop.InsertPtr(new RString("afkomstig"));
	stop.InsertPtr(new RString("afkomstige"));
	stop.InsertPtr(new RString("afspelen"));
	stop.InsertPtr(new RString("afwezigheid"));
	stop.InsertPtr(new RString("afwisselen"));
	stop.InsertPtr(new RString("afwisselend"));
	stop.InsertPtr(new RString("afzonderlijk"));
	stop.InsertPtr(new RString("afzonderlijke"));
	stop.InsertPtr(new RString("aj"));
	stop.InsertPtr(new RString("al"));
	stop.InsertPtr(new RString("aldaar"));
	stop.InsertPtr(new RString("aldus"));
	stop.InsertPtr(new RString("algemeen"));
	stop.InsertPtr(new RString("alhoewel"));
	stop.InsertPtr(new RString("alias"));
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("allebei"));
	stop.InsertPtr(new RString("alleen"));
	stop.InsertPtr(new RString("allemaal"));
	stop.InsertPtr(new RString("allen"));
	stop.InsertPtr(new RString("allereerst"));
	stop.InsertPtr(new RString("allerhande"));
	stop.InsertPtr(new RString("allerlei"));
	stop.InsertPtr(new RString("allerminst"));
	stop.InsertPtr(new RString("alles"));
	stop.InsertPtr(new RString("als"));
	stop.InsertPtr(new RString("alsmede"));
	stop.InsertPtr(new RString("alsnog"));
	stop.InsertPtr(new RString("alsook"));
	stop.InsertPtr(new RString("altijd"));
	stop.InsertPtr(new RString("altoos"));
	stop.InsertPtr(new RString("alvorens"));
	stop.InsertPtr(new RString("alwaar"));
	stop.InsertPtr(new RString("and"));
	stop.InsertPtr(new RString("ander"));
	stop.InsertPtr(new RString("andere"));
	stop.InsertPtr(new RString("anderen"));
	stop.InsertPtr(new RString("anders"));
	stop.InsertPtr(new RString("anderszins"));
	stop.InsertPtr(new RString("anderzijds"));
	stop.InsertPtr(new RString("apart"));
	stop.InsertPtr(new RString("aparte"));
	stop.InsertPtr(new RString("avonds"));
	stop.InsertPtr(new RString("bedacht"));
	stop.InsertPtr(new RString("bedoeld"));
	stop.InsertPtr(new RString("bedoeling"));
	stop.InsertPtr(new RString("behaald"));
	stop.InsertPtr(new RString("behalve"));
	stop.InsertPtr(new RString("behoeft"));
	stop.InsertPtr(new RString("behoefte"));
	stop.InsertPtr(new RString("behoeften"));
	stop.InsertPtr(new RString("behoeve"));
	stop.InsertPtr(new RString("behoeven"));
	stop.InsertPtr(new RString("behoorlijk"));
	stop.InsertPtr(new RString("behoort"));
	stop.InsertPtr(new RString("behoren"));
	stop.InsertPtr(new RString("behorend"));
	stop.InsertPtr(new RString("behorende"));
	stop.InsertPtr(new RString("behouden"));
	stop.InsertPtr(new RString("behoudens"));
	stop.InsertPtr(new RString("behulp"));
	stop.InsertPtr(new RString("beide"));
	stop.InsertPtr(new RString("beiden"));
	stop.InsertPtr(new RString("bekeken"));
	stop.InsertPtr(new RString("bekend"));
	stop.InsertPtr(new RString("bekende"));
	stop.InsertPtr(new RString("belangrijk"));
	stop.InsertPtr(new RString("belangrijke"));
	stop.InsertPtr(new RString("belangrijkste"));
	stop.InsertPtr(new RString("ben"));
	stop.InsertPtr(new RString("benaderd"));
	stop.InsertPtr(new RString("benadering"));
	stop.InsertPtr(new RString("beneden"));
	stop.InsertPtr(new RString("benodigd"));
	stop.InsertPtr(new RString("benodigde"));
	stop.InsertPtr(new RString("bent"));
	stop.InsertPtr(new RString("benut"));
	stop.InsertPtr(new RString("benutten"));
	stop.InsertPtr(new RString("benutting"));
	stop.InsertPtr(new RString("beoogde"));
	stop.InsertPtr(new RString("beoogt"));
	stop.InsertPtr(new RString("beoordeeld"));
	stop.InsertPtr(new RString("beoordelen"));
	stop.InsertPtr(new RString("bepaald"));
	stop.InsertPtr(new RString("bepaalde"));
	stop.InsertPtr(new RString("bepaalt"));
	stop.InsertPtr(new RString("bepalen"));
	stop.InsertPtr(new RString("bepalend"));
	stop.InsertPtr(new RString("bepalende"));
	stop.InsertPtr(new RString("bereid"));
	stop.InsertPtr(new RString("bereikbare"));
	stop.InsertPtr(new RString("bereiken"));
	stop.InsertPtr(new RString("bereikt"));
	stop.InsertPtr(new RString("bereikte"));
	stop.InsertPtr(new RString("berust"));
	stop.InsertPtr(new RString("beschouwd"));
	stop.InsertPtr(new RString("beschouwen"));
	stop.InsertPtr(new RString("beschreven"));
	stop.InsertPtr(new RString("beschrijven"));
	stop.InsertPtr(new RString("beschrijving"));
	stop.InsertPtr(new RString("beste"));
	stop.InsertPtr(new RString("bestond"));
	stop.InsertPtr(new RString("beter"));
	stop.InsertPtr(new RString("betere"));
	stop.InsertPtr(new RString("betreffen"));
	stop.InsertPtr(new RString("betreffende"));
	stop.InsertPtr(new RString("betreft"));
	stop.InsertPtr(new RString("betrekkelijk"));
	stop.InsertPtr(new RString("betrekt"));
	stop.InsertPtr(new RString("betrof"));
	stop.InsertPtr(new RString("betroffen"));
	stop.InsertPtr(new RString("betrokken"));
	stop.InsertPtr(new RString("betrokkenen"));
	stop.InsertPtr(new RString("betrokkenheid"));
	stop.InsertPtr(new RString("beurtelings"));
	stop.InsertPtr(new RString("bevat"));
	stop.InsertPtr(new RString("bevatte"));
	stop.InsertPtr(new RString("bevatten"));
	stop.InsertPtr(new RString("bevattende"));
	stop.InsertPtr(new RString("bevestigd"));
	stop.InsertPtr(new RString("bevinden"));
	stop.InsertPtr(new RString("bevindende"));
	stop.InsertPtr(new RString("bevindt"));
	stop.InsertPtr(new RString("bevonden"));
	stop.InsertPtr(new RString("bevredigende"));
	stop.InsertPtr(new RString("bewaard"));
	stop.InsertPtr(new RString("bewaarjeod"));
	stop.InsertPtr(new RString("bewerkstelligd"));
	stop.InsertPtr(new RString("bewerkstelligen"));
	stop.InsertPtr(new RString("bewust"));
	stop.InsertPtr(new RString("bezig"));
	stop.InsertPtr(new RString("bezigheid"));
	stop.InsertPtr(new RString("bezighouden"));
	stop.InsertPtr(new RString("bieden"));
	stop.InsertPtr(new RString("biedt"));
	stop.InsertPtr(new RString("bij"));
	stop.InsertPtr(new RString("bijbehorende"));
	stop.InsertPtr(new RString("bijkomstig"));
	stop.InsertPtr(new RString("bijna"));
	stop.InsertPtr(new RString("bijv"));
	stop.InsertPtr(new RString("bijvoorbeeld"));
	stop.InsertPtr(new RString("bijzonder"));
	stop.InsertPtr(new RString("bijzondere"));
	stop.InsertPtr(new RString("binnen"));
	stop.InsertPtr(new RString("binnenin"));
	stop.InsertPtr(new RString("binnenkort"));
	stop.InsertPtr(new RString("blijft"));
	stop.InsertPtr(new RString("blijken"));
	stop.InsertPtr(new RString("blijkens"));
	stop.InsertPtr(new RString("blijkt"));
	stop.InsertPtr(new RString("blijven"));
	stop.InsertPtr(new RString("blijvend"));
	stop.InsertPtr(new RString("blijvende"));
	stop.InsertPtr(new RString("boven"));
	stop.InsertPtr(new RString("bovenal"));
	stop.InsertPtr(new RString("bovendien"));
	stop.InsertPtr(new RString("bovengenoemd"));
	stop.InsertPtr(new RString("bovenstaande"));
	stop.InsertPtr(new RString("bovenste"));
	stop.InsertPtr(new RString("bovenvermeld"));
	stop.InsertPtr(new RString("brede"));
	stop.InsertPtr(new RString("breder"));
	stop.InsertPtr(new RString("breed"));
	stop.InsertPtr(new RString("brengen"));
	stop.InsertPtr(new RString("brengt"));
	stop.InsertPtr(new RString("buiten"));
	stop.InsertPtr(new RString("cetera"));
	stop.InsertPtr(new RString("circa"));
	stop.InsertPtr(new RString("concrete"));
	stop.InsertPtr(new RString("conform"));
	stop.InsertPtr(new RString("consequent"));
	stop.InsertPtr(new RString("consequenties"));
	stop.InsertPtr(new RString("creeren"));
	stop.InsertPtr(new RString("cruciaal"));
	stop.InsertPtr(new RString("daadwerkelijk"));
	stop.InsertPtr(new RString("daadwerkelijke"));
	stop.InsertPtr(new RString("daar"));
	stop.InsertPtr(new RString("daaraan"));
	stop.InsertPtr(new RString("daarbij"));
	stop.InsertPtr(new RString("daarbuiten"));
	stop.InsertPtr(new RString("daardoor"));
	stop.InsertPtr(new RString("daarentegen"));
	stop.InsertPtr(new RString("daarheen"));
	stop.InsertPtr(new RString("daarin"));
	stop.InsertPtr(new RString("daarmede"));
	stop.InsertPtr(new RString("daarmee"));
	stop.InsertPtr(new RString("daarna"));
	stop.InsertPtr(new RString("daarnaar"));
	stop.InsertPtr(new RString("daarnet"));
	stop.InsertPtr(new RString("daarom"));
	stop.InsertPtr(new RString("daarop"));
	stop.InsertPtr(new RString("daarover"));
	stop.InsertPtr(new RString("daartegen"));
	stop.InsertPtr(new RString("daartegenover"));
	stop.InsertPtr(new RString("daartoe"));
	stop.InsertPtr(new RString("daaruit"));
	stop.InsertPtr(new RString("daarvan"));
	stop.InsertPtr(new RString("daarvanlangs"));
	stop.InsertPtr(new RString("daarvoor"));
	stop.InsertPtr(new RString("dagelijks"));
	stop.InsertPtr(new RString("dagelijkse"));
	stop.InsertPtr(new RString("dan"));
	stop.InsertPtr(new RString("danwel"));
	stop.InsertPtr(new RString("dat"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("deel"));
	stop.InsertPtr(new RString("degene"));
	stop.InsertPtr(new RString("delen"));
	stop.InsertPtr(new RString("den"));
	stop.InsertPtr(new RString("denkbaar"));
	stop.InsertPtr(new RString("der"));
	stop.InsertPtr(new RString("dergelijk"));
	stop.InsertPtr(new RString("dergelijke"));
	stop.InsertPtr(new RString("dergelijks"));
	stop.InsertPtr(new RString("derhalve"));
	stop.InsertPtr(new RString("dermate"));
	stop.InsertPtr(new RString("des"));
	stop.InsertPtr(new RString("desbetreffende"));
	stop.InsertPtr(new RString("desnoods"));
	stop.InsertPtr(new RString("desondanks"));
	stop.InsertPtr(new RString("destijds"));
	stop.InsertPtr(new RString("deze"));
	stop.InsertPtr(new RString("dezelfde"));
	stop.InsertPtr(new RString("dezer"));
	stop.InsertPtr(new RString("die"));
	stop.InsertPtr(new RString("diens"));
	stop.InsertPtr(new RString("dient"));
	stop.InsertPtr(new RString("dientengevolge"));
	stop.InsertPtr(new RString("diezelfde"));
	stop.InsertPtr(new RString("dikwijls"));
	stop.InsertPtr(new RString("ding"));
	stop.InsertPtr(new RString("dingen"));
	stop.InsertPtr(new RString("direct"));
	stop.InsertPtr(new RString("directe"));
	stop.InsertPtr(new RString("dit"));
	stop.InsertPtr(new RString("ditmaal"));
	stop.InsertPtr(new RString("divers"));
	stop.InsertPtr(new RString("diverse"));
	stop.InsertPtr(new RString("diversen"));
	stop.InsertPtr(new RString("doen"));
	stop.InsertPtr(new RString("doet"));
	stop.InsertPtr(new RString("door"));
	stop.InsertPtr(new RString("doordat"));
	stop.InsertPtr(new RString("doorgaan"));
	stop.InsertPtr(new RString("doorgaand"));
	stop.InsertPtr(new RString("doorgaans"));
	stop.InsertPtr(new RString("doorheen"));
	stop.InsertPtr(new RString("drie"));
	stop.InsertPtr(new RString("duidelijk"));
	stop.InsertPtr(new RString("duidelijke"));
	stop.InsertPtr(new RString("durven"));
	stop.InsertPtr(new RString("dus"));
	stop.InsertPtr(new RString("dusdanig"));
	stop.InsertPtr(new RString("dusver"));
	stop.InsertPtr(new RString("duurt"));
	stop.InsertPtr(new RString("echter"));
	stop.InsertPtr(new RString("een"));
	stop.InsertPtr(new RString("eenduidige"));
	stop.InsertPtr(new RString("eenmaal"));
	stop.InsertPtr(new RString("eenmalig"));
	stop.InsertPtr(new RString("eenmalige"));
	stop.InsertPtr(new RString("eens"));
	stop.InsertPtr(new RString("eenvoudig"));
	stop.InsertPtr(new RString("eenvoudige"));
	stop.InsertPtr(new RString("eenzelfde"));
	stop.InsertPtr(new RString("eer"));
	stop.InsertPtr(new RString("eerdat"));
	stop.InsertPtr(new RString("eerder"));
	stop.InsertPtr(new RString("eerdere"));
	stop.InsertPtr(new RString("eerlang"));
	stop.InsertPtr(new RString("eerst"));
	stop.InsertPtr(new RString("eerste"));
	stop.InsertPtr(new RString("eerstgenoemde"));
	stop.InsertPtr(new RString("eerstvolgende"));
	stop.InsertPtr(new RString("eigen"));
	stop.InsertPtr(new RString("eigenlijk"));
	stop.InsertPtr(new RString("eigenlijke"));
	stop.InsertPtr(new RString("elders"));
	stop.InsertPtr(new RString("elk"));
	stop.InsertPtr(new RString("elkaar"));
	stop.InsertPtr(new RString("elke"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("ene"));
	stop.InsertPtr(new RString("enerzijds"));
	stop.InsertPtr(new RString("enige"));
	stop.InsertPtr(new RString("enigszins"));
	stop.InsertPtr(new RString("enkele"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("eraan"));
	stop.InsertPtr(new RString("erachter"));
	stop.InsertPtr(new RString("eraf"));
	stop.InsertPtr(new RString("erboven"));
	stop.InsertPtr(new RString("erdoor"));
	stop.InsertPtr(new RString("erg"));
	stop.InsertPtr(new RString("ergens"));
	stop.InsertPtr(new RString("erin"));
	stop.InsertPtr(new RString("ermee"));
	stop.InsertPtr(new RString("erna"));
	stop.InsertPtr(new RString("ernaar"));
	stop.InsertPtr(new RString("ernstige"));
	stop.InsertPtr(new RString("erom"));
	stop.InsertPtr(new RString("eronder"));
	stop.InsertPtr(new RString("erop"));
	stop.InsertPtr(new RString("ertoe"));
	stop.InsertPtr(new RString("eruit"));
	stop.InsertPtr(new RString("ervan"));
	stop.InsertPtr(new RString("ervoor"));
	stop.InsertPtr(new RString("essentieel"));
	stop.InsertPtr(new RString("essentiele"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("etc"));
	stop.InsertPtr(new RString("etcetera"));
	stop.InsertPtr(new RString("even"));
	stop.InsertPtr(new RString("evenals"));
	stop.InsertPtr(new RString("eveneens"));
	stop.InsertPtr(new RString("evenmin"));
	stop.InsertPtr(new RString("eventueel"));
	stop.InsertPtr(new RString("eventuele"));
	stop.InsertPtr(new RString("evenwel"));
	stop.InsertPtr(new RString("extra"));
	stop.InsertPtr(new RString("feit"));
	stop.InsertPtr(new RString("feitelijk"));
	stop.InsertPtr(new RString("feitelijke"));
	stop.InsertPtr(new RString("gaan"));
	stop.InsertPtr(new RString("gaande"));
	stop.InsertPtr(new RString("gaat"));
	stop.InsertPtr(new RString("gaf"));
	stop.InsertPtr(new RString("geacht"));
	stop.InsertPtr(new RString("geachte"));
	stop.InsertPtr(new RString("gebeurd"));
	stop.InsertPtr(new RString("gebeurde"));
	stop.InsertPtr(new RString("gebeurt"));
	stop.InsertPtr(new RString("gebracht"));
	stop.InsertPtr(new RString("gebruikelijk"));
	stop.InsertPtr(new RString("gebruikelijke"));
	stop.InsertPtr(new RString("gebruikende"));
	stop.InsertPtr(new RString("gebruikmakend"));
	stop.InsertPtr(new RString("gebruikmaking"));
	stop.InsertPtr(new RString("gebruikt"));
	stop.InsertPtr(new RString("gebruikte"));
	stop.InsertPtr(new RString("gedaan"));
	stop.InsertPtr(new RString("gedacht"));
	stop.InsertPtr(new RString("gedeelte"));
	stop.InsertPtr(new RString("gedeeltelijk"));
	stop.InsertPtr(new RString("gedeeltelijke"));
	stop.InsertPtr(new RString("gedurende"));
	stop.InsertPtr(new RString("geeft"));
	stop.InsertPtr(new RString("geen"));
	stop.InsertPtr(new RString("gegaan"));
	stop.InsertPtr(new RString("gegeven"));
	stop.InsertPtr(new RString("gehaald"));
	stop.InsertPtr(new RString("gehad"));
	stop.InsertPtr(new RString("geheel"));
	stop.InsertPtr(new RString("gehele"));
	stop.InsertPtr(new RString("gehouden"));
	stop.InsertPtr(new RString("gekeken"));
	stop.InsertPtr(new RString("gekomen"));
	stop.InsertPtr(new RString("gekregen"));
	stop.InsertPtr(new RString("gekund"));
	stop.InsertPtr(new RString("geldt"));
	stop.InsertPtr(new RString("geleden"));
	stop.InsertPtr(new RString("gelegd"));
	stop.InsertPtr(new RString("gelet"));
	stop.InsertPtr(new RString("gelijk"));
	stop.InsertPtr(new RString("gelijkende"));
	stop.InsertPtr(new RString("gelijktijdig"));
	stop.InsertPtr(new RString("gemaakt"));
	stop.InsertPtr(new RString("gemaakte"));
	stop.InsertPtr(new RString("gemakkelijk"));
	stop.InsertPtr(new RString("gemakkelijke"));
	stop.InsertPtr(new RString("gemakkelijker"));
	stop.InsertPtr(new RString("gemakkelijkst"));
	stop.InsertPtr(new RString("gemakkelijkste"));
	stop.InsertPtr(new RString("gemiddeld"));
	stop.InsertPtr(new RString("gemiddelde"));
	stop.InsertPtr(new RString("gemoeten"));
	stop.InsertPtr(new RString("gemogen"));
	stop.InsertPtr(new RString("genoeg"));
	stop.InsertPtr(new RString("genoemd"));
	stop.InsertPtr(new RString("genoemde"));
	stop.InsertPtr(new RString("getroffen"));
	stop.InsertPtr(new RString("geven"));
	stop.InsertPtr(new RString("geweest"));
	stop.InsertPtr(new RString("gewone"));
	stop.InsertPtr(new RString("gewoon"));
	stop.InsertPtr(new RString("gewoonlijk"));
	stop.InsertPtr(new RString("gewoonweg"));
	stop.InsertPtr(new RString("geworden"));
	stop.InsertPtr(new RString("gezien"));
	stop.InsertPtr(new RString("ging"));
	stop.InsertPtr(new RString("goed"));
	stop.InsertPtr(new RString("goeddeels"));
	stop.InsertPtr(new RString("goede"));
	stop.InsertPtr(new RString("graag"));
	stop.InsertPtr(new RString("groot"));
	stop.InsertPtr(new RString("grootst"));
	stop.InsertPtr(new RString("grootste"));
	stop.InsertPtr(new RString("grootte"));
	stop.InsertPtr(new RString("grote"));
	stop.InsertPtr(new RString("groter"));
	stop.InsertPtr(new RString("grotere"));
	stop.InsertPtr(new RString("haalt"));
	stop.InsertPtr(new RString("haar"));
	stop.InsertPtr(new RString("had"));
	stop.InsertPtr(new RString("hadden"));
	stop.InsertPtr(new RString("halen"));
	stop.InsertPtr(new RString("half"));
	stop.InsertPtr(new RString("hangt"));
	stop.InsertPtr(new RString("hare"));
	stop.InsertPtr(new RString("heb"));
	stop.InsertPtr(new RString("hebben"));
	stop.InsertPtr(new RString("hebt"));
	stop.InsertPtr(new RString("heden"));
	stop.InsertPtr(new RString("heeft"));
	stop.InsertPtr(new RString("heel"));
	stop.InsertPtr(new RString("heen"));
	stop.InsertPtr(new RString("heet"));
	stop.InsertPtr(new RString("heftig"));
	stop.InsertPtr(new RString("hele"));
	stop.InsertPtr(new RString("helemaal"));
	stop.InsertPtr(new RString("hem"));
	stop.InsertPtr(new RString("hen"));
	stop.InsertPtr(new RString("herbergt"));
	stop.InsertPtr(new RString("het"));
	stop.InsertPtr(new RString("hetgeen"));
	stop.InsertPtr(new RString("hetzelfde"));
	stop.InsertPtr(new RString("hetzij"));
	stop.InsertPtr(new RString("hier"));
	stop.InsertPtr(new RString("hieraan"));
	stop.InsertPtr(new RString("hierbeneden"));
	stop.InsertPtr(new RString("hierbij"));
	stop.InsertPtr(new RString("hierboven"));
	stop.InsertPtr(new RString("hierdoor"));
	stop.InsertPtr(new RString("hierin"));
	stop.InsertPtr(new RString("hiermede"));
	stop.InsertPtr(new RString("hiermee"));
	stop.InsertPtr(new RString("hierna"));
	stop.InsertPtr(new RString("hierom"));
	stop.InsertPtr(new RString("hieronder"));
	stop.InsertPtr(new RString("hierop"));
	stop.InsertPtr(new RString("hierover"));
	stop.InsertPtr(new RString("hiertoe"));
	stop.InsertPtr(new RString("hieruit"));
	stop.InsertPtr(new RString("hiervan"));
	stop.InsertPtr(new RString("hiervoor"));
	stop.InsertPtr(new RString("hij"));
	stop.InsertPtr(new RString("hoe"));
	stop.InsertPtr(new RString("hoefde"));
	stop.InsertPtr(new RString("hoefden"));
	stop.InsertPtr(new RString("hoeft"));
	stop.InsertPtr(new RString("hoelang"));
	stop.InsertPtr(new RString("hoeveel"));
	stop.InsertPtr(new RString("hoeven"));
	stop.InsertPtr(new RString("hoeverre"));
	stop.InsertPtr(new RString("hoewel"));
	stop.InsertPtr(new RString("hoge"));
	stop.InsertPtr(new RString("hoger"));
	stop.InsertPtr(new RString("hogere"));
	stop.InsertPtr(new RString("honderd"));
	stop.InsertPtr(new RString("hoofde"));
	stop.InsertPtr(new RString("hoofdzakelijk"));
	stop.InsertPtr(new RString("hoog"));
	stop.InsertPtr(new RString("hoogst"));
	stop.InsertPtr(new RString("hoogste"));
	stop.InsertPtr(new RString("hoogte"));
	stop.InsertPtr(new RString("hooguit"));
	stop.InsertPtr(new RString("hoort"));
	stop.InsertPtr(new RString("horen"));
	stop.InsertPtr(new RString("houden"));
	stop.InsertPtr(new RString("houdend"));
	stop.InsertPtr(new RString("houder"));
	stop.InsertPtr(new RString("houders"));
	stop.InsertPtr(new RString("houdt"));
	stop.InsertPtr(new RString("huidig"));
	stop.InsertPtr(new RString("huidige"));
	stop.InsertPtr(new RString("hun"));
	stop.InsertPtr(new RString("hunne"));
	stop.InsertPtr(new RString("ieder"));
	stop.InsertPtr(new RString("iedere"));
	stop.InsertPtr(new RString("iedereen"));
	stop.InsertPtr(new RString("iemand"));
	stop.InsertPtr(new RString("iets"));
	stop.InsertPtr(new RString("ik"));
	stop.InsertPtr(new RString("ikzelf"));
	stop.InsertPtr(new RString("immers"));
	stop.InsertPtr(new RString("in"));
	stop.InsertPtr(new RString("inclusief"));
	stop.InsertPtr(new RString("inderdaad"));
	stop.InsertPtr(new RString("indien"));
	stop.InsertPtr(new RString("indienen"));
	stop.InsertPtr(new RString("ineens"));
	stop.InsertPtr(new RString("ingaan"));
	stop.InsertPtr(new RString("ingang"));
	stop.InsertPtr(new RString("inmiddels"));
	stop.InsertPtr(new RString("interessant"));
	stop.InsertPtr(new RString("inzage"));
	stop.InsertPtr(new RString("inzake"));
	stop.InsertPtr(new RString("inzicht"));
	stop.InsertPtr(new RString("inzichtelijk"));
	stop.InsertPtr(new RString("is"));
	stop.InsertPtr(new RString("ja"));
	stop.InsertPtr(new RString("jaar"));
	stop.InsertPtr(new RString("jaren"));
	stop.InsertPtr(new RString("jarenlang"));
	stop.InsertPtr(new RString("jarenlange"));
	stop.InsertPtr(new RString("jezelf"));
	stop.InsertPtr(new RString("jij"));
	stop.InsertPtr(new RString("jijzelf"));
	stop.InsertPtr(new RString("jonger"));
	stop.InsertPtr(new RString("jongste"));
	stop.InsertPtr(new RString("jou"));
	stop.InsertPtr(new RString("jouw"));
	stop.InsertPtr(new RString("jouwe"));
	stop.InsertPtr(new RString("juist"));
	stop.InsertPtr(new RString("juiste"));
	stop.InsertPtr(new RString("jullie"));
	stop.InsertPtr(new RString("kan"));
	stop.InsertPtr(new RString("keer"));
	stop.InsertPtr(new RString("kenbaar"));
	stop.InsertPtr(new RString("kennen"));
	stop.InsertPtr(new RString("kent"));
	stop.InsertPtr(new RString("keren"));
	stop.InsertPtr(new RString("keus"));
	stop.InsertPtr(new RString("keuze"));
	stop.InsertPtr(new RString("keuzen"));
	stop.InsertPtr(new RString("keuzes"));
	stop.InsertPtr(new RString("kiest"));
	stop.InsertPtr(new RString("kijk"));
	stop.InsertPtr(new RString("kijken"));
	stop.InsertPtr(new RString("kijkt"));
	stop.InsertPtr(new RString("kilo"));
	stop.InsertPtr(new RString("kilogram"));
	stop.InsertPtr(new RString("kilometers"));
	stop.InsertPtr(new RString("kiometer"));
	stop.InsertPtr(new RString("klaar"));
	stop.InsertPtr(new RString("klein"));
	stop.InsertPtr(new RString("kleine"));
	stop.InsertPtr(new RString("kleiner"));
	stop.InsertPtr(new RString("kleinere"));
	stop.InsertPtr(new RString("kleinste"));
	stop.InsertPtr(new RString("komen"));
	stop.InsertPtr(new RString("komend"));
	stop.InsertPtr(new RString("komende"));
	stop.InsertPtr(new RString("komst"));
	stop.InsertPtr(new RString("komt"));
	stop.InsertPtr(new RString("kon"));
	stop.InsertPtr(new RString("konden"));
	stop.InsertPtr(new RString("kort"));
	stop.InsertPtr(new RString("korte"));
	stop.InsertPtr(new RString("korter"));
	stop.InsertPtr(new RString("kortgeleden"));
	stop.InsertPtr(new RString("kortom"));
	stop.InsertPtr(new RString("kortst"));
	stop.InsertPtr(new RString("kortste"));
	stop.InsertPtr(new RString("kortweg"));
	stop.InsertPtr(new RString("krachtens"));
	stop.InsertPtr(new RString("kreeg"));
	stop.InsertPtr(new RString("kregen"));
	stop.InsertPtr(new RString("krijgen"));
	stop.InsertPtr(new RString("krijgt"));
	stop.InsertPtr(new RString("kunnen"));
	stop.InsertPtr(new RString("kunt"));
	stop.InsertPtr(new RString("kwam"));
	stop.InsertPtr(new RString("kwamen"));
	stop.InsertPtr(new RString("laagste"));
	stop.InsertPtr(new RString("laat"));
	stop.InsertPtr(new RString("laatst"));
	stop.InsertPtr(new RString("laatste"));
	stop.InsertPtr(new RString("lage"));
	stop.InsertPtr(new RString("lagere"));
	stop.InsertPtr(new RString("lang"));
	stop.InsertPtr(new RString("langdurig"));
	stop.InsertPtr(new RString("langdurige"));
	stop.InsertPtr(new RString("lange"));
	stop.InsertPtr(new RString("langer"));
	stop.InsertPtr(new RString("langere"));
	stop.InsertPtr(new RString("langs"));
	stop.InsertPtr(new RString("langst"));
	stop.InsertPtr(new RString("langste"));
	stop.InsertPtr(new RString("laten"));
	stop.InsertPtr(new RString("later"));
	stop.InsertPtr(new RString("latere"));
	stop.InsertPtr(new RString("leggen"));
	stop.InsertPtr(new RString("legt"));
	stop.InsertPtr(new RString("liep"));
	stop.InsertPtr(new RString("liever"));
	stop.InsertPtr(new RString("lijkt"));
	stop.InsertPtr(new RString("loopt"));
	stop.InsertPtr(new RString("lopen"));
	stop.InsertPtr(new RString("lopend"));
	stop.InsertPtr(new RString("lopende"));
	stop.InsertPtr(new RString("maak"));
	stop.InsertPtr(new RString("maakt"));
	stop.InsertPtr(new RString("maakte"));
	stop.InsertPtr(new RString("maakten"));
	stop.InsertPtr(new RString("maar"));
	stop.InsertPtr(new RString("mag"));
	stop.InsertPtr(new RString("maken"));
	stop.InsertPtr(new RString("makkelijk"));
	stop.InsertPtr(new RString("mate"));
	stop.InsertPtr(new RString("mede"));
	stop.InsertPtr(new RString("mee"));
	stop.InsertPtr(new RString("meegegeven"));
	stop.InsertPtr(new RString("meegenomen"));
	stop.InsertPtr(new RString("meende"));
	stop.InsertPtr(new RString("meenden"));
	stop.InsertPtr(new RString("meer"));
	stop.InsertPtr(new RString("meerdere"));
	stop.InsertPtr(new RString("meest"));
	stop.InsertPtr(new RString("meestal"));
	stop.InsertPtr(new RString("meeste"));
	stop.InsertPtr(new RString("melden"));
	stop.InsertPtr(new RString("meldt"));
	stop.InsertPtr(new RString("men"));
	stop.InsertPtr(new RString("menen"));
	stop.InsertPtr(new RString("met"));
	stop.InsertPtr(new RString("meter"));
	stop.InsertPtr(new RString("mezelf"));
	stop.InsertPtr(new RString("middel"));
	stop.InsertPtr(new RString("middels"));
	stop.InsertPtr(new RString("mij"));
	stop.InsertPtr(new RString("mijn"));
	stop.InsertPtr(new RString("mijnent"));
	stop.InsertPtr(new RString("mijner"));
	stop.InsertPtr(new RString("mijzelf"));
	stop.InsertPtr(new RString("miljard"));
	stop.InsertPtr(new RString("miljoen"));
	stop.InsertPtr(new RString("miljoenen"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("minder"));
	stop.InsertPtr(new RString("mindere"));
	stop.InsertPtr(new RString("minste"));
	stop.InsertPtr(new RString("minstens"));
	stop.InsertPtr(new RString("misschien"));
	stop.InsertPtr(new RString("mits"));
	stop.InsertPtr(new RString("mocht"));
	stop.InsertPtr(new RString("mochten"));
	stop.InsertPtr(new RString("moeilijk"));
	stop.InsertPtr(new RString("moeilijker"));
	stop.InsertPtr(new RString("moeilijkst"));
	stop.InsertPtr(new RString("moeilijkste"));
	stop.InsertPtr(new RString("moeite"));
	stop.InsertPtr(new RString("moeizaam"));
	stop.InsertPtr(new RString("moeizamer"));
	stop.InsertPtr(new RString("moest"));
	stop.InsertPtr(new RString("moesten"));
	stop.InsertPtr(new RString("moet"));
	stop.InsertPtr(new RString("moeten"));
	stop.InsertPtr(new RString("mogelijk"));
	stop.InsertPtr(new RString("mogelijke"));
	stop.InsertPtr(new RString("mogelijkerwijs"));
	stop.InsertPtr(new RString("mogelijkheden"));
	stop.InsertPtr(new RString("mogelijkheid"));
	stop.InsertPtr(new RString("mogen"));
	stop.InsertPtr(new RString("moment"));
	stop.InsertPtr(new RString("momenteel"));
	stop.InsertPtr(new RString("na"));
	stop.InsertPtr(new RString("naar"));
	stop.InsertPtr(new RString("naarmate"));
	stop.InsertPtr(new RString("naartoe"));
	stop.InsertPtr(new RString("naast"));
	stop.InsertPtr(new RString("naaste"));
	stop.InsertPtr(new RString("nabij"));
	stop.InsertPtr(new RString("nabije"));
	stop.InsertPtr(new RString("nabijheid"));
	stop.InsertPtr(new RString("nadat"));
	stop.InsertPtr(new RString("nader"));
	stop.InsertPtr(new RString("naderbij"));
	stop.InsertPtr(new RString("nadere"));
	stop.InsertPtr(new RString("nadruk"));
	stop.InsertPtr(new RString("nagenoeg"));
	stop.InsertPtr(new RString("name"));
	stop.InsertPtr(new RString("namelijk"));
	stop.InsertPtr(new RString("namens"));
	stop.InsertPtr(new RString("natuurlijk"));
	stop.InsertPtr(new RString("nauw"));
	stop.InsertPtr(new RString("nauwelijks"));
	stop.InsertPtr(new RString("nauwlettend"));
	stop.InsertPtr(new RString("navenant"));
	stop.InsertPtr(new RString("nb"));
	stop.InsertPtr(new RString("nee"));
	stop.InsertPtr(new RString("neemt"));
	stop.InsertPtr(new RString("neer"));
	stop.InsertPtr(new RString("negen"));
	stop.InsertPtr(new RString("nemen"));
	stop.InsertPtr(new RString("nergens"));
	stop.InsertPtr(new RString("net"));
	stop.InsertPtr(new RString("ngaande"));
	stop.InsertPtr(new RString("niet"));
	stop.InsertPtr(new RString("niets"));
	stop.InsertPtr(new RString("niettemin"));
	stop.InsertPtr(new RString("nieuwe"));
	stop.InsertPtr(new RString("nieuwste"));
	stop.InsertPtr(new RString("noch"));
	stop.InsertPtr(new RString("nodig"));
	stop.InsertPtr(new RString("nodige"));
	stop.InsertPtr(new RString("noemde"));
	stop.InsertPtr(new RString("noemen"));
	stop.InsertPtr(new RString("noemenswaardig"));
	stop.InsertPtr(new RString("noemt"));
	stop.InsertPtr(new RString("nog"));
	stop.InsertPtr(new RString("nogal"));
	stop.InsertPtr(new RString("nogmaals"));
	stop.InsertPtr(new RString("noodgedwongen"));
	stop.InsertPtr(new RString("noodzaak"));
	stop.InsertPtr(new RString("noodzakelijk"));
	stop.InsertPtr(new RString("noodzakelijke"));
	stop.InsertPtr(new RString("nooit"));
	stop.InsertPtr(new RString("normaal"));
	stop.InsertPtr(new RString("normale"));
	stop.InsertPtr(new RString("normaliter"));
	stop.InsertPtr(new RString("not"));
	stop.InsertPtr(new RString("nu"));
	stop.InsertPtr(new RString("nuttig"));
	stop.InsertPtr(new RString("nuttige"));
	stop.InsertPtr(new RString("nuttiger"));
	stop.InsertPtr(new RString("ochtends"));
	stop.InsertPtr(new RString("of"));
	stop.InsertPtr(new RString("ofschoon"));
	stop.InsertPtr(new RString("oftewel"));
	stop.InsertPtr(new RString("ofwel"));
	stop.InsertPtr(new RString("om"));
	stop.InsertPtr(new RString("omdat"));
	stop.InsertPtr(new RString("omgaan"));
	stop.InsertPtr(new RString("omgang"));
	stop.InsertPtr(new RString("omgedeerde"));
	stop.InsertPtr(new RString("omgegaan"));
	stop.InsertPtr(new RString("omgekeerd"));
	stop.InsertPtr(new RString("omgeving"));
	stop.InsertPtr(new RString("omhoog"));
	stop.InsertPtr(new RString("omlaag"));
	stop.InsertPtr(new RString("omschreven"));
	stop.InsertPtr(new RString("omstandigheden"));
	stop.InsertPtr(new RString("omstreeks"));
	stop.InsertPtr(new RString("omtrent"));
	stop.InsertPtr(new RString("omvat"));
	stop.InsertPtr(new RString("omver"));
	stop.InsertPtr(new RString("ondanks"));
	stop.InsertPtr(new RString("onder"));
	stop.InsertPtr(new RString("onderbouwde"));
	stop.InsertPtr(new RString("onderbouwing"));
	stop.InsertPtr(new RString("onderbrengen"));
	stop.InsertPtr(new RString("ondergebracht"));
	stop.InsertPtr(new RString("onderhavige"));
	stop.InsertPtr(new RString("onderhevig"));
	stop.InsertPtr(new RString("onderkennen"));
	stop.InsertPtr(new RString("onderling"));
	stop.InsertPtr(new RString("onderlinge"));
	stop.InsertPtr(new RString("ondermeer"));
	stop.InsertPtr(new RString("ondernomen"));
	stop.InsertPtr(new RString("onderschreven"));
	stop.InsertPtr(new RString("ondertussen"));
	stop.InsertPtr(new RString("ondervind"));
	stop.InsertPtr(new RString("ondervindt"));
	stop.InsertPtr(new RString("ongeacht"));
	stop.InsertPtr(new RString("ongehinderd"));
	stop.InsertPtr(new RString("ongeveer"));
	stop.InsertPtr(new RString("onlangs"));
	stop.InsertPtr(new RString("onmiddellijk"));
	stop.InsertPtr(new RString("onmiddellijke"));
	stop.InsertPtr(new RString("onmisbaar"));
	stop.InsertPtr(new RString("onmisbare"));
	stop.InsertPtr(new RString("onmogelijk"));
	stop.InsertPtr(new RString("onmogelijke"));
	stop.InsertPtr(new RString("onnodig"));
	stop.InsertPtr(new RString("onnodige"));
	stop.InsertPtr(new RString("onontkoombaar"));
	stop.InsertPtr(new RString("onrealistische"));
	stop.InsertPtr(new RString("ons"));
	stop.InsertPtr(new RString("onstond"));
	stop.InsertPtr(new RString("onszelf"));
	stop.InsertPtr(new RString("ontdaan"));
	stop.InsertPtr(new RString("ontstaan"));
	stop.InsertPtr(new RString("ontstaat"));
	stop.InsertPtr(new RString("ontstane"));
	stop.InsertPtr(new RString("onvoldoende"));
	stop.InsertPtr(new RString("onze"));
	stop.InsertPtr(new RString("ook"));
	stop.InsertPtr(new RString("op"));
	stop.InsertPtr(new RString("opdat"));
	stop.InsertPtr(new RString("opeenvolgende"));
	stop.InsertPtr(new RString("open"));
	stop.InsertPtr(new RString("opgebracht"));
	stop.InsertPtr(new RString("opgedaan"));
	stop.InsertPtr(new RString("opgedane"));
	stop.InsertPtr(new RString("opgevraagd"));
	stop.InsertPtr(new RString("opnieuw"));
	stop.InsertPtr(new RString("optimaal"));
	stop.InsertPtr(new RString("optimale"));
	stop.InsertPtr(new RString("opzicht"));
	stop.InsertPtr(new RString("opzichte"));
	stop.InsertPtr(new RString("opzichten"));
	stop.InsertPtr(new RString("opzij"));
	stop.InsertPtr(new RString("or"));
	stop.InsertPtr(new RString("oud"));
	stop.InsertPtr(new RString("oude"));
	stop.InsertPtr(new RString("ouder"));
	stop.InsertPtr(new RString("over"));
	stop.InsertPtr(new RString("overal"));
	stop.InsertPtr(new RString("overblijft"));
	stop.InsertPtr(new RString("overblijvende"));
	stop.InsertPtr(new RString("overeen"));
	stop.InsertPtr(new RString("overeind"));
	stop.InsertPtr(new RString("overig"));
	stop.InsertPtr(new RString("overige"));
	stop.InsertPtr(new RString("overigen"));
	stop.InsertPtr(new RString("overigens"));
	stop.InsertPtr(new RString("overwegen"));
	stop.InsertPtr(new RString("overwegingen"));
	stop.InsertPtr(new RString("overzien"));
	stop.InsertPtr(new RString("paar"));
	stop.InsertPtr(new RString("pakt"));
	stop.InsertPtr(new RString("pas"));
	stop.InsertPtr(new RString("passend"));
	stop.InsertPtr(new RString("passende"));
	stop.InsertPtr(new RString("per"));
	stop.InsertPtr(new RString("persoon"));
	stop.InsertPtr(new RString("persoonlijk"));
	stop.InsertPtr(new RString("persoonlijke"));
	stop.InsertPtr(new RString("plaats"));
	stop.InsertPtr(new RString("plaatse"));
	stop.InsertPtr(new RString("plaatsgevonden"));
	stop.InsertPtr(new RString("plaatsvinden"));
	stop.InsertPtr(new RString("plaatsvindt"));
	stop.InsertPtr(new RString("plaatsvond"));
	stop.InsertPtr(new RString("plaatsvonden"));
	stop.InsertPtr(new RString("pleegt"));
	stop.InsertPtr(new RString("postadres"));
	stop.InsertPtr(new RString("postbus"));
	stop.InsertPtr(new RString("precies"));
	stop.InsertPtr(new RString("precieze"));
	stop.InsertPtr(new RString("probeert"));
	stop.InsertPtr(new RString("proberen"));
	stop.InsertPtr(new RString("procent"));
	stop.InsertPtr(new RString("ps"));
	stop.InsertPtr(new RString("qua"));
	stop.InsertPtr(new RString("raakt"));
	stop.InsertPtr(new RString("rechtstreeks"));
	stop.InsertPtr(new RString("redelijk"));
	stop.InsertPtr(new RString("reden"));
	stop.InsertPtr(new RString("reeds"));
	stop.InsertPtr(new RString("reeel"));
	stop.InsertPtr(new RString("reeele"));
	stop.InsertPtr(new RString("respectievelijk"));
	stop.InsertPtr(new RString("rest"));
	stop.InsertPtr(new RString("roepen"));
	stop.InsertPtr(new RString("roept"));
	stop.InsertPtr(new RString("rond"));
	stop.InsertPtr(new RString("rondom"));
	stop.InsertPtr(new RString("ruim"));
	stop.InsertPtr(new RString("ruime"));
	stop.InsertPtr(new RString("ruimschoots"));
	stop.InsertPtr(new RString("rustig"));
	stop.InsertPtr(new RString("samen"));
	stop.InsertPtr(new RString("scheelt"));
	stop.InsertPtr(new RString("sedert"));
	stop.InsertPtr(new RString("sinds"));
	stop.InsertPtr(new RString("sindsdien"));
	stop.InsertPtr(new RString("slecht"));
	stop.InsertPtr(new RString("slechte"));
	stop.InsertPtr(new RString("slechter"));
	stop.InsertPtr(new RString("slechtere"));
	stop.InsertPtr(new RString("slechts"));
	stop.InsertPtr(new RString("smalle"));
	stop.InsertPtr(new RString("snel"));
	stop.InsertPtr(new RString("sommige"));
	stop.InsertPtr(new RString("soms"));
	stop.InsertPtr(new RString("soort"));
	stop.InsertPtr(new RString("soorten"));
	stop.InsertPtr(new RString("soortgelijke"));
	stop.InsertPtr(new RString("speciaal"));
	stop.InsertPtr(new RString("speciale"));
	stop.InsertPtr(new RString("speelt"));
	stop.InsertPtr(new RString("spelen"));
	stop.InsertPtr(new RString("spoedig"));
	stop.InsertPtr(new RString("sprak"));
	stop.InsertPtr(new RString("sprake"));
	stop.InsertPtr(new RString("spraken"));
	stop.InsertPtr(new RString("spreekt"));
	stop.InsertPtr(new RString("spreken"));
	stop.InsertPtr(new RString("sprekend"));
	stop.InsertPtr(new RString("sprekender"));
	stop.InsertPtr(new RString("staan"));
	stop.InsertPtr(new RString("staat"));
	stop.InsertPtr(new RString("steeds"));
	stop.InsertPtr(new RString("stel"));
	stop.InsertPtr(new RString("stellen"));
	stop.InsertPtr(new RString("stelt"));
	stop.InsertPtr(new RString("sterk"));
	stop.InsertPtr(new RString("strikt"));
	stop.InsertPtr(new RString("strikte"));
	stop.InsertPtr(new RString("stringente"));
	stop.InsertPtr(new RString("talloze"));
	stop.InsertPtr(new RString("tamelijk"));
	stop.InsertPtr(new RString("te"));
	stop.InsertPtr(new RString("tegelijk"));
	stop.InsertPtr(new RString("tegemoet"));
	stop.InsertPtr(new RString("tegen"));
	stop.InsertPtr(new RString("tegengaat"));
	stop.InsertPtr(new RString("tegengegaan"));
	stop.InsertPtr(new RString("tegenover"));
	stop.InsertPtr(new RString("tegenwoordig"));
	stop.InsertPtr(new RString("telkens"));
	stop.InsertPtr(new RString("telt"));
	stop.InsertPtr(new RString("ten"));
	stop.InsertPtr(new RString("teneinde"));
	stop.InsertPtr(new RString("tenminste"));
	stop.InsertPtr(new RString("tenslotte"));
	stop.InsertPtr(new RString("tenzij"));
	stop.InsertPtr(new RString("ter"));
	stop.InsertPtr(new RString("terecht"));
	stop.InsertPtr(new RString("terug"));
	stop.InsertPtr(new RString("terveel"));
	stop.InsertPtr(new RString("terwijl"));
	stop.InsertPtr(new RString("tevens"));
	stop.InsertPtr(new RString("tezamen"));
	stop.InsertPtr(new RString("thans"));
	stop.InsertPtr(new RString("tien"));
	stop.InsertPtr(new RString("tijdens"));
	stop.InsertPtr(new RString("toch"));
	stop.InsertPtr(new RString("toe"));
	stop.InsertPtr(new RString("toen"));
	stop.InsertPtr(new RString("toenmaals"));
	stop.InsertPtr(new RString("toenmalig"));
	stop.InsertPtr(new RString("ton"));
	stop.InsertPtr(new RString("tot"));
	stop.InsertPtr(new RString("totaal"));
	stop.InsertPtr(new RString("totale"));
	stop.InsertPtr(new RString("totdat"));
	stop.InsertPtr(new RString("tussen"));
	stop.InsertPtr(new RString("twee"));
	stop.InsertPtr(new RString("uit"));
	stop.InsertPtr(new RString("uiteen"));
	stop.InsertPtr(new RString("uiteenlopend"));
	stop.InsertPtr(new RString("uiteenlopende"));
	stop.InsertPtr(new RString("uiteindelijk"));
	stop.InsertPtr(new RString("uiteindelijke"));
	stop.InsertPtr(new RString("uiteraard"));
	stop.InsertPtr(new RString("uitermate"));
	stop.InsertPtr(new RString("uiterste"));
	stop.InsertPtr(new RString("uitgezonderd"));
	stop.InsertPtr(new RString("uiting"));
	stop.InsertPtr(new RString("uitkomen"));
	stop.InsertPtr(new RString("uitsluitend"));
	stop.InsertPtr(new RString("vaak"));
	stop.InsertPtr(new RString("vage"));
	stop.InsertPtr(new RString("vallen"));
	stop.InsertPtr(new RString("valt"));
	stop.InsertPtr(new RString("van"));
	stop.InsertPtr(new RString("vanaf"));
	stop.InsertPtr(new RString("vandaan"));
	stop.InsertPtr(new RString("vandaar"));
	stop.InsertPtr(new RString("vangen"));
	stop.InsertPtr(new RString("vanuit"));
	stop.InsertPtr(new RString("vanwege"));
	stop.InsertPtr(new RString("vanzelf"));
	stop.InsertPtr(new RString("vanzelfsprekend"));
	stop.InsertPtr(new RString("vast"));
	stop.InsertPtr(new RString("vaste"));
	stop.InsertPtr(new RString("vatten"));
	stop.InsertPtr(new RString("veel"));
	stop.InsertPtr(new RString("veelal"));
	stop.InsertPtr(new RString("veeleer"));
	stop.InsertPtr(new RString("vele"));
	stop.InsertPtr(new RString("velen"));
	stop.InsertPtr(new RString("veler"));
	stop.InsertPtr(new RString("ver"));
	stop.InsertPtr(new RString("verder"));
	stop.InsertPtr(new RString("verdere"));
	stop.InsertPtr(new RString("verdergaand"));
	stop.InsertPtr(new RString("verdergaande"));
	stop.InsertPtr(new RString("vergt"));
	stop.InsertPtr(new RString("verkregen"));
	stop.InsertPtr(new RString("verkrijgbaar"));
	stop.InsertPtr(new RString("verkrijgen"));
	stop.InsertPtr(new RString("verkrijgt"));
	stop.InsertPtr(new RString("verlaten"));
	stop.InsertPtr(new RString("verleden"));
	stop.InsertPtr(new RString("verleend"));
	stop.InsertPtr(new RString("vermoedelijk"));
	stop.InsertPtr(new RString("verschil"));
	stop.InsertPtr(new RString("verschillen"));
	stop.InsertPtr(new RString("verschillend"));
	stop.InsertPtr(new RString("verschillende"));
	stop.InsertPtr(new RString("versus"));
	stop.InsertPtr(new RString("vertoont"));
	stop.InsertPtr(new RString("vervolgens"));
	stop.InsertPtr(new RString("via"));
	stop.InsertPtr(new RString("vier"));
	stop.InsertPtr(new RString("vijf"));
	stop.InsertPtr(new RString("vind"));
	stop.InsertPtr(new RString("vinden"));
	stop.InsertPtr(new RString("vindt"));
	stop.InsertPtr(new RString("vol"));
	stop.InsertPtr(new RString("volgend"));
	stop.InsertPtr(new RString("volgende"));
	stop.InsertPtr(new RString("volgens"));
	stop.InsertPtr(new RString("volle"));
	stop.InsertPtr(new RString("volledig"));
	stop.InsertPtr(new RString("volledige"));
	stop.InsertPtr(new RString("vond"));
	stop.InsertPtr(new RString("vonden"));
	stop.InsertPtr(new RString("voor"));
	stop.InsertPtr(new RString("vooraanstaand"));
	stop.InsertPtr(new RString("vooraanstaande"));
	stop.InsertPtr(new RString("vooraf"));
	stop.InsertPtr(new RString("voorafgaand"));
	stop.InsertPtr(new RString("voorafgaande"));
	stop.InsertPtr(new RString("vooral"));
	stop.InsertPtr(new RString("vooralsnog"));
	stop.InsertPtr(new RString("voorbij"));
	stop.InsertPtr(new RString("voordat"));
	stop.InsertPtr(new RString("voordezen"));
	stop.InsertPtr(new RString("voordien"));
	stop.InsertPtr(new RString("voordoet"));
	stop.InsertPtr(new RString("voorgaande"));
	stop.InsertPtr(new RString("voorhand"));
	stop.InsertPtr(new RString("voorheen"));
	stop.InsertPtr(new RString("voornamelijk"));
	stop.InsertPtr(new RString("voornemens"));
	stop.InsertPtr(new RString("voornoemde"));
	stop.InsertPtr(new RString("voorop"));
	stop.InsertPtr(new RString("voort"));
	stop.InsertPtr(new RString("voorts"));
	stop.InsertPtr(new RString("voortvloeiend"));
	stop.InsertPtr(new RString("voortvloeiende"));
	stop.InsertPtr(new RString("vooruit"));
	stop.InsertPtr(new RString("vorig"));
	stop.InsertPtr(new RString("vorige"));
	stop.InsertPtr(new RString("vrij"));
	stop.InsertPtr(new RString("vrijwel"));
	stop.InsertPtr(new RString("vroeg"));
	stop.InsertPtr(new RString("waar"));
	stop.InsertPtr(new RString("waaraan"));
	stop.InsertPtr(new RString("waarachter"));
	stop.InsertPtr(new RString("waarbij"));
	stop.InsertPtr(new RString("waarbinnen"));
	stop.InsertPtr(new RString("waarboven"));
	stop.InsertPtr(new RString("waarde"));
	stop.InsertPtr(new RString("waarden"));
	stop.InsertPtr(new RString("waardoor"));
	stop.InsertPtr(new RString("waarin"));
	stop.InsertPtr(new RString("waarlangs"));
	stop.InsertPtr(new RString("waarmee"));
	stop.InsertPtr(new RString("waarna"));
	stop.InsertPtr(new RString("waarom"));
	stop.InsertPtr(new RString("waaronder"));
	stop.InsertPtr(new RString("waarop"));
	stop.InsertPtr(new RString("waarover"));
	stop.InsertPtr(new RString("waarschijnlijk"));
	stop.InsertPtr(new RString("waarschijnlijker"));
	stop.InsertPtr(new RString("waartegen"));
	stop.InsertPtr(new RString("waartoe"));
	stop.InsertPtr(new RString("waaruit"));
	stop.InsertPtr(new RString("waarvan"));
	stop.InsertPtr(new RString("wanneer"));
	stop.InsertPtr(new RString("want"));
	stop.InsertPtr(new RString("waren"));
	stop.InsertPtr(new RString("was"));
	stop.InsertPtr(new RString("wat"));
	stop.InsertPtr(new RString("wederom"));
	stop.InsertPtr(new RString("weer"));
	stop.InsertPtr(new RString("wees"));
	stop.InsertPtr(new RString("weet"));
	stop.InsertPtr(new RString("weg"));
	stop.InsertPtr(new RString("wegens"));
	stop.InsertPtr(new RString("weinig"));
	stop.InsertPtr(new RString("weinige"));
	stop.InsertPtr(new RString("wel"));
	stop.InsertPtr(new RString("weldra"));
	stop.InsertPtr(new RString("weliswaar"));
	stop.InsertPtr(new RString("welk"));
	stop.InsertPtr(new RString("welke"));
	stop.InsertPtr(new RString("wellicht"));
	stop.InsertPtr(new RString("werd"));
	stop.InsertPtr(new RString("werden"));
	stop.InsertPtr(new RString("werkt"));
	stop.InsertPtr(new RString("wie"));
	stop.InsertPtr(new RString("wiens"));
	stop.InsertPtr(new RString("wier"));
	stop.InsertPtr(new RString("wij"));
	stop.InsertPtr(new RString("wijze"));
	stop.InsertPtr(new RString("wijzelf"));
	stop.InsertPtr(new RString("wijzen"));
	stop.InsertPtr(new RString("wil"));
	stop.InsertPtr(new RString("wilde"));
	stop.InsertPtr(new RString("willekeurig"));
	stop.InsertPtr(new RString("willen"));
	stop.InsertPtr(new RString("wist"));
	stop.InsertPtr(new RString("wisten"));
	stop.InsertPtr(new RString("worden"));
	stop.InsertPtr(new RString("wordende"));
	stop.InsertPtr(new RString("wordt"));
	stop.InsertPtr(new RString("zag"));
	stop.InsertPtr(new RString("zal"));
	stop.InsertPtr(new RString("ze"));
	stop.InsertPtr(new RString("zeer"));
	stop.InsertPtr(new RString("zeggen"));
	stop.InsertPtr(new RString("zeker"));
	stop.InsertPtr(new RString("zekere"));
	stop.InsertPtr(new RString("zelf"));
	stop.InsertPtr(new RString("zelfde"));
	stop.InsertPtr(new RString("zelfs"));
	stop.InsertPtr(new RString("zelfstandig"));
	stop.InsertPtr(new RString("zes"));
	stop.InsertPtr(new RString("zet"));
	stop.InsertPtr(new RString("zetten"));
	stop.InsertPtr(new RString("zeven"));
	stop.InsertPtr(new RString("zich"));
	stop.InsertPtr(new RString("zichzelf"));
	stop.InsertPtr(new RString("zie"));
	stop.InsertPtr(new RString("zien"));
	stop.InsertPtr(new RString("ziet"));
	stop.InsertPtr(new RString("zij"));
	stop.InsertPtr(new RString("zijn"));
	stop.InsertPtr(new RString("zijnde"));
	stop.InsertPtr(new RString("zo"));
	stop.InsertPtr(new RString("zoals"));
	stop.InsertPtr(new RString("zodanig"));
	stop.InsertPtr(new RString("zodanige"));
	stop.InsertPtr(new RString("zodat"));
	stop.InsertPtr(new RString("zodoende"));
	stop.InsertPtr(new RString("zodra"));
	stop.InsertPtr(new RString("zogeheten"));
	stop.InsertPtr(new RString("zogenaamde"));
	stop.InsertPtr(new RString("zogenoemde"));
	stop.InsertPtr(new RString("zognaamd"));
	stop.InsertPtr(new RString("zojuist"));
	stop.InsertPtr(new RString("zolang"));
	stop.InsertPtr(new RString("zomaar"));
	stop.InsertPtr(new RString("zonder"));
	stop.InsertPtr(new RString("zonodig"));
	stop.InsertPtr(new RString("zorgde"));
	stop.InsertPtr(new RString("zorgden"));
	stop.InsertPtr(new RString("zorgt"));
	stop.InsertPtr(new RString("zou"));
	stop.InsertPtr(new RString("zouden"));
	stop.InsertPtr(new RString("zoveel"));
	stop.InsertPtr(new RString("zover"));
	stop.InsertPtr(new RString("zowat"));
	stop.InsertPtr(new RString("zowel"));
	stop.InsertPtr(new RString("zozeer"));
	stop.InsertPtr(new RString("zulke"));
	stop.InsertPtr(new RString("zulks"));
	stop.InsertPtr(new RString("zullen"));
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
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Dutch",GLangNL,"nl")

