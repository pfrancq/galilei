/*

	GALILEI Research Project

	GLangFR.cpp

	French Language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glangfr.h>
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
	
	FrenchPorterRule(const char* os,const char* ns,int oo,int no,int mr=-1);
	int Compare(const FrenchPorterRule*) {return(-1);}
};


//-----------------------------------------------------------------------------
GLangFR::FrenchPorterRule::FrenchPorterRule(const char* os,const char* ns,int oo,int no,int mr)
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
	: GLang("French","fr"), Rules1(0), Rules2(0), Rules3(0)
{

	Rules1=new RContainer<FrenchPorterRule,unsigned int,true,false>(130,10);
	Rules1->InsertPtr(new FrenchPorterRule("issaient","", 7,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ellement","el", 7,1,0));
	Rules1->InsertPtr(new FrenchPorterRule("issement","", 7,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("alement","al",6 ,1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eraient","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iraient","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eassent","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ussent","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("amment","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("emment","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issant","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issent","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("assent","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eaient","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issait","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("èrent","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erent","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irent","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erait","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irait","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iront","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eront","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ement","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aient","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("îrent","",4 , -1,0));
	Rules1->InsertPtr(new FrenchPorterRule("eont","", 3,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eant","", 3,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eait","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ient","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ent","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ont","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ant","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eât","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ait","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("at","",1,-1,0));
	Rules1->InsertPtr(new FrenchPorterRule("ât","",1,-1,0));
	Rules1->InsertPtr(new FrenchPorterRule("it","",1 , -1,0));
	Rules1->InsertPtr(new FrenchPorterRule("ît","",1 , -1,0));
	Rules1->InsertPtr(new FrenchPorterRule("t","", 0, 0,0));

	Rules1->InsertPtr(new FrenchPorterRule("uction","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ication","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("iation","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ation","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ition","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("tion","",3 ,0 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("ateur","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("teur","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eur","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ier","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("er","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ir","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("r","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("eassiez","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issiez","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("assiez","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ussiez","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issez","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("assez","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eriez","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iriez","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erez","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irez","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iez","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ez","",1 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("erai","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irai","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eai","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ai","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("i","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("ira","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("era","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ea","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("a","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("f","v",0 ,0 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("yeux","oeil",3 ,3 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eux","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aux","al",2 ,1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("x","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("issante","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eresse","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eante","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("easse","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eure","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("esse","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("asse","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ance","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ence","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aise","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("euse","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("oise","o",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("isse","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ante","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ouse","ou",3 ,1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("ière","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ete","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ète","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iere","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aire","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ure","",2 ,-1 ,1));

	Rules1->InsertPtr(new FrenchPorterRule("erie","",3 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("étude","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("etude","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("itude","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ade","",2 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("isme","",3 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("age","",2 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("trice","",4 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("cque","c",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("que","c",2 ,0 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("eille","eil",4 ,2 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("elle","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("able","",3 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("iste","", 3,-1 ,0));


	Rules1->InsertPtr(new FrenchPorterRule("ulle","ul",3 ,1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("gue","g",2 ,0 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("ette","",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("nne","n",2 ,0 ,0));	

	Rules1->InsertPtr(new FrenchPorterRule("itée","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ité","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("té","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ée","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("é","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("usse","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aise","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ate","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ite","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ee","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("e","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("issements","",8 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issantes","",7 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ications","",7 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eassions","",7 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eresses","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issions","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("assions","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iations","",6 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("issants","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ussions","",6 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ements","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eantes","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issons","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("assons","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("easses","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("études","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("etudes","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("itudes","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("issais","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("trices","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eilles","eil",5 ,2 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irions","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erions","",5 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ateurs","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ations","",5 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("usses","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("tions","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ances","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("entes","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("teurs","",4 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("eants","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irons","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("irais","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ences","",4,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ients","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ieres","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eures","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aires","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erons","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("esses","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("euses","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ulles","ul",4 ,1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("cques","c",4 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("elles","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("istes","", 4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("aises","", 4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("asses","", 4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("isses","", 4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("oises","o",4 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("tions","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ouses","ou",4 ,1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ières","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eries","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("antes","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ismes","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("erais","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eâtes","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eâmes","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("itées","",4 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ettes","",4 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ages","", 3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ents","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ètes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("etes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ions","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ités","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ites","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ates","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("âtes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("îtes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iers","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("iras","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eras","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ures","",3 ,-1 ,1));
	Rules1->InsertPtr(new FrenchPorterRule("ants","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("îmes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ûmes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("âmes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ades","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eais","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eons","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ques","c",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("gues","g",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("nnes","n",3 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ttes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("îtes","",3 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("tés","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ons","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ais","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ées","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ees","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ats","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("eas","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("ts","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("rs","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("as","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("es","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("fs","v",1 ,0 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("és","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("is","",1 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("s","",0 ,-1 ,0));

	Rules1->InsertPtr(new FrenchPorterRule("eau","",2 ,-1 ,0));
	Rules1->InsertPtr(new FrenchPorterRule("au","",2 ,-1 ,0));


	Rules2=new RContainer<FrenchPorterRule,unsigned int,true,false>(5,10);

	Rules2->InsertPtr(new FrenchPorterRule("ent","", 2,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("ation","",4 ,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("ition","",4 ,0 ,1));
	Rules2->InsertPtr(new FrenchPorterRule("tion","",3 ,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("el","", 1,-1,1));
	Rules2->InsertPtr(new FrenchPorterRule("i","", 0,-1,0));

	Rules3=new RContainer<FrenchPorterRule,unsigned int,true,false>(5,10);

	Rules3->InsertPtr(new FrenchPorterRule("nn","n",1 ,0,0));
	Rules3->InsertPtr(new FrenchPorterRule("ll","l",1 ,0,0));
	Rules3->InsertPtr(new FrenchPorterRule("tt","t",1 ,0,0));
	Rules3->InsertPtr(new FrenchPorterRule("y","",0 ,-1,0));
	Rules3->InsertPtr(new FrenchPorterRule("t","",0 ,-1,0));
	Rules3->InsertPtr(new FrenchPorterRule("qu","c",1 ,0,0));


	SkipWords.InsertPtr(new SkipWord("ier"));
	SkipWords.InsertPtr(new SkipWord("iers"));
	SkipWords.InsertPtr(new SkipWord("èm"));
	SkipWords.InsertPtr(new SkipWord("ème"));
	SkipWords.InsertPtr(new SkipWord("èmes"));
	SkipWords.InsertPtr(new SkipWord("er"));
	SkipWords.InsertPtr(new SkipWord("ers"));
	SkipWords.InsertPtr(new SkipWord("re"));
	SkipWords.InsertPtr(new SkipWord("res"));
	SkipWords.InsertPtr(new SkipWord("e"));
	SkipWords.InsertPtr(new SkipWord("aine"));
	SkipWords.InsertPtr(new SkipWord("aines"));
	SkipWords.InsertPtr(new SkipWord("ième"));
	SkipWords.InsertPtr(new SkipWord("ièmes"));
}


//-----------------------------------------------------------------------------
int GALILEI::GLangFR::GetWordSize(char* kwd)
{
	int Result=0;             // WordSize of the word.
	int State=0;             // Current state of the machine.
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
		return((IsVowel(*kwd))||(strpbrk(kwd+1,"âaeiouyéàèîöê")));
}


//-----------------------------------------------------------------------------
bool GALILEI::GLangFR::ApplyRules(char* kwd,char* &end,RContainer<FrenchPorterRule,unsigned int,true,false>* rules,int /*ruleslevel*/)
{
	FrenchPorterRule* ptr;
	char* ending;
	int WordSize;
	char tmp;
	int len;

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
        WordSize=GetWordSize(kwd);
		// Verify if the minimum root size is Ok.
		if(ptr->MinRootSize>=WordSize)
		{
			(*ending)=tmp;
			continue;
		}

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
	char kwd[51];
	char* end;
	unsigned int len;

	// Transform _kwd in lowercase and store it in kwd.
	(*res)=_kwd;
	res->StrLwr();
	if(res->GetLen()>50) return(*res);
	strcpy(kwd,(*res)());
	len=_kwd.GetLen()-1;
	end=&kwd[len];

	// Do the unique step(for the moment) of the french adapation of Porter algorithm.
	ApplyRules(kwd,end,Rules1,1);
	ApplyRules(kwd,end,Rules2,2);
	ApplyRules(kwd,end,Rules3,3);
	
	// Put the result in res and return it.
	while((*end)&&(!isspace(*end)))
		end++;
	if(*end)
		(*end)=0;
	(*res)=kwd;
	return(*res);
}


//-----------------------------------------------------------------------------
GALILEI::GLangFR::~GLangFR(void)
{
	if(Rules1) delete Rules1;
	if(Rules2) delete Rules2;
	if(Rules3) delete Rules3;
	
}
