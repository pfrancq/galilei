/*

	GALILEI Research Project

	LangFR.cpp

	French Language - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Julien Lamoral.
	Copyright 2001 by Marjorie Paternostre.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).).

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
#include <langfr.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class FrenchPorterRule
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GLangFR::FrenchPorterRule
{
public:
	char* OldSuffix;
	char* NewSuffix;
	int OldOffset;
	int NewOffset;
	int MinRootSize;

	FrenchPorterRule(const char* os,const char* ns,int oo,int no,int mr=-1);
	int Compare(const FrenchPorterRule&) const {return(-1);}
	~FrenchPorterRule(void);
};


//-----------------------------------------------------------------------------
GLangFR::FrenchPorterRule::FrenchPorterRule(const char* os,const char* ns,int oo,int no,int mr)
	: OldSuffix(0), NewSuffix(0), OldOffset(oo), NewOffset(no), MinRootSize(mr)
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
GLangFR::FrenchPorterRule::~FrenchPorterRule(void)
{
	delete[] OldSuffix;
	delete[] NewSuffix;
}



//-----------------------------------------------------------------------------
//
// class GLangFR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangFR::GLangFR(GFactoryLang* fac)
	: GLang(fac,"French","fr"), Rules1(130), Rules2(10), Rules3(10)
{
	// Rules 1
	Rules1.InsertPtr(new FrenchPorterRule("issaient","", 7,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ellement","el", 7,1,0));
	Rules1.InsertPtr(new FrenchPorterRule("issement","", 7,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("alement","al",6 ,1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eraient","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iraient","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eassent","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ussent","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("amment","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("emment","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issant","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issent","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("assent","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eaient","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issait","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("èrent","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erent","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irent","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erait","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irait","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iront","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eront","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ement","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aient","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("îrent","",4 , -1,0));
	Rules1.InsertPtr(new FrenchPorterRule("eont","", 3,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eant","", 3,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eait","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ient","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ent","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ont","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ant","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eât","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ait","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("at","",1,-1,0));
	Rules1.InsertPtr(new FrenchPorterRule("ât","",1,-1,0));
	Rules1.InsertPtr(new FrenchPorterRule("it","",1 , -1,0));
	Rules1.InsertPtr(new FrenchPorterRule("ît","",1 , -1,0));
	Rules1.InsertPtr(new FrenchPorterRule("t","", 0, 0,0));

	Rules1.InsertPtr(new FrenchPorterRule("uction","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ication","",6 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("iation","",5 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("ation","",4 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("ition","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("tion","",3 ,0 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("ateur","",4 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("teur","",3 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("eur","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ier","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("er","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ir","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("r","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("eassiez","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issiez","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("assiez","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ussiez","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issez","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("assez","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eriez","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iriez","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erez","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irez","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iez","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ez","",1 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("erai","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irai","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eai","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ai","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("i","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("ira","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("era","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ea","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("a","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("f","v",0 ,0 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("yeux","oeil",3 ,3 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eux","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aux","al",2 ,1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("x","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("issante","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eresse","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eante","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("easse","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eure","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("esse","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("asse","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ance","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ence","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aise","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("euse","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("oise","o",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("isse","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ante","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ouse","ou",3 ,1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("ière","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ete","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ète","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iere","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aire","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ure","",2 ,-1 ,1));

	Rules1.InsertPtr(new FrenchPorterRule("erie","",3 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("étude","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("etude","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("itude","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ade","",2 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("isme","",3 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("age","",2 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("trice","",4 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("cque","c",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("que","c",2 ,0 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("eille","eil",4 ,2 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("elle","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("able","",3 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("iste","", 3,-1 ,0));


	Rules1.InsertPtr(new FrenchPorterRule("ulle","ul",3 ,1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("gue","g",2 ,0 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("ette","",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("nne","n",2 ,0 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("itée","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ité","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("té","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ée","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("é","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("usse","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aise","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ate","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ite","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ee","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("e","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("issements","",8 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issantes","",7 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ications","",7 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("eassions","",7 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eresses","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issions","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("assions","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iations","",6 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("issants","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ussions","",6 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ements","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eantes","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issons","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("assons","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("easses","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("études","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("etudes","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("itudes","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("issais","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("trices","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eilles","eil",5 ,2 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irions","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erions","",5 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ateurs","",5 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("ations","",5 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("usses","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("tions","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ances","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("entes","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("teurs","",4 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("eants","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irons","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("irais","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ences","",4,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ients","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ieres","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eures","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aires","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erons","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("esses","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("euses","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ulles","ul",4 ,1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("cques","c",4 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("elles","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ables","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("istes","", 4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("aises","", 4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("asses","", 4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("isses","", 4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("oises","o",4 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("tions","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ouses","ou",4 ,1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ières","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eries","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("antes","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ismes","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("erais","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eâtes","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eâmes","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("itées","",4 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ettes","",4 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ages","", 3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ents","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ètes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("etes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ions","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ités","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ites","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ates","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("âtes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("îtes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eurs","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iers","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("iras","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eras","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ures","",3 ,-1 ,1));
	Rules1.InsertPtr(new FrenchPorterRule("ants","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("îmes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ûmes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("âmes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ades","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eais","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eons","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ques","c",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("gues","g",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("nnes","n",3 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ttes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("îtes","",3 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("tés","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ons","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ais","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ées","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ees","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ats","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("eas","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("ts","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("rs","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("as","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("es","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("fs","v",1 ,0 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("és","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("is","",1 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("s","",0 ,-1 ,0));

	Rules1.InsertPtr(new FrenchPorterRule("eau","",2 ,-1 ,0));
	Rules1.InsertPtr(new FrenchPorterRule("au","",2 ,-1 ,0));


	// Rules 2
	Rules2.InsertPtr(new FrenchPorterRule("ent","", 2,-1,1));
	Rules2.InsertPtr(new FrenchPorterRule("ation","",4 ,-1,1));
	Rules2.InsertPtr(new FrenchPorterRule("ition","",4 ,0 ,1));
	Rules2.InsertPtr(new FrenchPorterRule("tion","",3 ,-1,1));
	Rules2.InsertPtr(new FrenchPorterRule("el","", 1,-1,1));
	Rules2.InsertPtr(new FrenchPorterRule("i","", 0,-1,0));

	// Rules 3
	Rules3.InsertPtr(new FrenchPorterRule("nn","n",1 ,0,0));
	Rules3.InsertPtr(new FrenchPorterRule("ll","l",1 ,0,0));
	Rules3.InsertPtr(new FrenchPorterRule("tt","t",1 ,0,0));
	Rules3.InsertPtr(new FrenchPorterRule("y","",0 ,-1,0));
	Rules3.InsertPtr(new FrenchPorterRule("t","",0 ,-1,0));
	Rules3.InsertPtr(new FrenchPorterRule("qu","c",1 ,0,0));


	SkipSequence("ier");
	SkipSequence("iers");
	SkipSequence("èm");
	SkipSequence("ème");
	SkipSequence("èmes");
	SkipSequence("er");
	SkipSequence("ers");
	SkipSequence("re");
	SkipSequence("res");
	SkipSequence("e");
	SkipSequence("aine");
	SkipSequence("aines");
	SkipSequence("ième");
	SkipSequence("ièmes");
}


//-----------------------------------------------------------------------------
int GLangFR::GetWordSize(char* kwd)
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
bool GLangFR::ContainsVowel(const char* kwd)
{
	if(!(*kwd))
		return(false);
	else
		return((IsVowel(*kwd))||(strpbrk(kwd+1,"âaeiouyéàèîöê")));
}


//-----------------------------------------------------------------------------
bool GLangFR::ApplyRules(char* kwd,char* &end,RContainer<FrenchPorterRule,true,false>& rules)
{
	FrenchPorterRule* ptr;
	char* ending;
	int WordSize;
	char tmp;
	size_t len;

	len=strlen(kwd);
	RCursor<FrenchPorterRule> Cur(rules);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		ptr=Cur();

		// If the word length is enough, find the potential end suffix and put it
		// in ending. If the ending isn't corresponding to the rule's suffix,
		// go to the next rule.
		if(len<=static_cast<size_t>(ptr->OldOffset)) continue;
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
void GLangFR::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("à"));
	stop.InsertPtr(new RString("abors"));
	stop.InsertPtr(new RString("accord"));
	stop.InsertPtr(new RString("affilée"));
	stop.InsertPtr(new RString("afin"));
	stop.InsertPtr(new RString("ah"));
	stop.InsertPtr(new RString("ai"));
	stop.InsertPtr(new RString("aie"));
	stop.InsertPtr(new RString("aient"));
	stop.InsertPtr(new RString("aies"));
	stop.InsertPtr(new RString("ailleurs"));
	stop.InsertPtr(new RString("ainsi"));
	stop.InsertPtr(new RString("ait"));
	stop.InsertPtr(new RString("alentour"));
	stop.InsertPtr(new RString("alias"));
	stop.InsertPtr(new RString("alors"));
	stop.InsertPtr(new RString("ap."));
	stop.InsertPtr(new RString("apr."));
	stop.InsertPtr(new RString("après"));
	stop.InsertPtr(new RString("arrière"));
	stop.InsertPtr(new RString("as"));
	stop.InsertPtr(new RString("assez"));
	stop.InsertPtr(new RString("attendu"));
	stop.InsertPtr(new RString("au"));
	stop.InsertPtr(new RString("aucun"));
	stop.InsertPtr(new RString("aucune"));
	stop.InsertPtr(new RString("audit"));
	stop.InsertPtr(new RString("aujourd"));
	stop.InsertPtr(new RString("auparavant"));
	stop.InsertPtr(new RString("auprès"));
	stop.InsertPtr(new RString("auquel"));
	stop.InsertPtr(new RString("aura"));
	stop.InsertPtr(new RString("aurai"));
	stop.InsertPtr(new RString("auraient"));
	stop.InsertPtr(new RString("aurais"));
	stop.InsertPtr(new RString("aurait"));
	stop.InsertPtr(new RString("auras"));
	stop.InsertPtr(new RString("aurez"));
	stop.InsertPtr(new RString("auriez"));
	stop.InsertPtr(new RString("aurions"));
	stop.InsertPtr(new RString("aurons"));
	stop.InsertPtr(new RString("auront"));
	stop.InsertPtr(new RString("aussi"));
	stop.InsertPtr(new RString("aussitôt"));
	stop.InsertPtr(new RString("autant"));
	stop.InsertPtr(new RString("autour"));
	stop.InsertPtr(new RString("autre"));
	stop.InsertPtr(new RString("autrefois"));
	stop.InsertPtr(new RString("autres"));
	stop.InsertPtr(new RString("autrui"));
	stop.InsertPtr(new RString("aux"));
	stop.InsertPtr(new RString("auxdites"));
	stop.InsertPtr(new RString("auxdits"));
	stop.InsertPtr(new RString("auxquelles"));
	stop.InsertPtr(new RString("auxquels"));
	stop.InsertPtr(new RString("avaient"));
	stop.InsertPtr(new RString("avais"));
	stop.InsertPtr(new RString("avait"));
	stop.InsertPtr(new RString("avant"));
	stop.InsertPtr(new RString("avec"));
	stop.InsertPtr(new RString("avez"));
	stop.InsertPtr(new RString("aviez"));
	stop.InsertPtr(new RString("avions"));
	stop.InsertPtr(new RString("avoir"));
	stop.InsertPtr(new RString("avons"));
	stop.InsertPtr(new RString("ayant"));
	stop.InsertPtr(new RString("ayez"));
	stop.InsertPtr(new RString("ayons"));
	stop.InsertPtr(new RString("bah"));
	stop.InsertPtr(new RString("banco"));
	stop.InsertPtr(new RString("bas"));
	stop.InsertPtr(new RString("bé"));
	stop.InsertPtr(new RString("beaucoup"));
	stop.InsertPtr(new RString("ben"));
	stop.InsertPtr(new RString("bien"));
	stop.InsertPtr(new RString("bientôt"));
	stop.InsertPtr(new RString("bis"));
	stop.InsertPtr(new RString("bon"));
	stop.InsertPtr(new RString("c"));
	stop.InsertPtr(new RString("ç"));
	stop.InsertPtr(new RString("ça"));
	stop.InsertPtr(new RString("çà"));
	stop.InsertPtr(new RString("caha"));
	stop.InsertPtr(new RString("cahin"));
	stop.InsertPtr(new RString("car"));
	stop.InsertPtr(new RString("ce"));
	stop.InsertPtr(new RString("céans"));
	stop.InsertPtr(new RString("ceci"));
	stop.InsertPtr(new RString("cela"));
	stop.InsertPtr(new RString("celle"));
	stop.InsertPtr(new RString("celles"));
	stop.InsertPtr(new RString("celui"));
	stop.InsertPtr(new RString("cent"));
	stop.InsertPtr(new RString("cents"));
	stop.InsertPtr(new RString("cependant"));
	stop.InsertPtr(new RString("certain"));
	stop.InsertPtr(new RString("certaine"));
	stop.InsertPtr(new RString("certaines"));
	stop.InsertPtr(new RString("certains"));
	stop.InsertPtr(new RString("certes"));
	stop.InsertPtr(new RString("ces"));
	stop.InsertPtr(new RString("cet"));
	stop.InsertPtr(new RString("cette"));
	stop.InsertPtr(new RString("ceux"));
	stop.InsertPtr(new RString("cf."));
	stop.InsertPtr(new RString("cg"));
	stop.InsertPtr(new RString("cgr"));
	stop.InsertPtr(new RString("chacun"));
	stop.InsertPtr(new RString("chacune"));
	stop.InsertPtr(new RString("chaque"));
	stop.InsertPtr(new RString("cher"));
	stop.InsertPtr(new RString("chez"));
	stop.InsertPtr(new RString("ci"));
	stop.InsertPtr(new RString("cinq"));
	stop.InsertPtr(new RString("cinquante"));
	stop.InsertPtr(new RString("cl"));
	stop.InsertPtr(new RString("cm"));
	stop.InsertPtr(new RString("cm²"));
	stop.InsertPtr(new RString("combien"));
	stop.InsertPtr(new RString("comme"));
	stop.InsertPtr(new RString("comment"));
	stop.InsertPtr(new RString("contrario"));
	stop.InsertPtr(new RString("contre"));
	stop.InsertPtr(new RString("cours"));
	stop.InsertPtr(new RString("crescendo"));
	stop.InsertPtr(new RString("d"));
	stop.InsertPtr(new RString("dans"));
	stop.InsertPtr(new RString("davantage"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("debout"));
	stop.InsertPtr(new RString("decà"));
	stop.InsertPtr(new RString("dedans"));
	stop.InsertPtr(new RString("dehors"));
	stop.InsertPtr(new RString("déjà"));
	stop.InsertPtr(new RString("delà"));
	stop.InsertPtr(new RString("demain"));
	stop.InsertPtr(new RString("dépis"));
	stop.InsertPtr(new RString("depuis"));
	stop.InsertPtr(new RString("derechef"));
	stop.InsertPtr(new RString("derrière"));
	stop.InsertPtr(new RString("des"));
	stop.InsertPtr(new RString("dès"));
	stop.InsertPtr(new RString("desdites"));
	stop.InsertPtr(new RString("desdits"));
	stop.InsertPtr(new RString("désormais"));
	stop.InsertPtr(new RString("desquelles"));
	stop.InsertPtr(new RString("desquels"));
	stop.InsertPtr(new RString("dessous"));
	stop.InsertPtr(new RString("dessus"));
	stop.InsertPtr(new RString("deux"));
	stop.InsertPtr(new RString("devant"));
	stop.InsertPtr(new RString("devers"));
	stop.InsertPtr(new RString("dg"));
	stop.InsertPtr(new RString("die"));
	stop.InsertPtr(new RString("différentes"));
	stop.InsertPtr(new RString("différents"));
	stop.InsertPtr(new RString("dito"));
	stop.InsertPtr(new RString("divers"));
	stop.InsertPtr(new RString("diverses"));
	stop.InsertPtr(new RString("dix"));
	stop.InsertPtr(new RString("dl"));
	stop.InsertPtr(new RString("dm"));
	stop.InsertPtr(new RString("donc"));
	stop.InsertPtr(new RString("dont"));
	stop.InsertPtr(new RString("dorénavant"));
	stop.InsertPtr(new RString("douze"));
	stop.InsertPtr(new RString("du"));
	stop.InsertPtr(new RString("dudit"));
	stop.InsertPtr(new RString("duquel"));
	stop.InsertPtr(new RString("durant"));
	stop.InsertPtr(new RString("égard"));
	stop.InsertPtr(new RString("eh"));
	stop.InsertPtr(new RString("elle"));
	stop.InsertPtr(new RString("elles"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("encontre"));
	stop.InsertPtr(new RString("encore"));
	stop.InsertPtr(new RString("enfin"));
	stop.InsertPtr(new RString("ensemble"));
	stop.InsertPtr(new RString("ensuite"));
	stop.InsertPtr(new RString("entre"));
	stop.InsertPtr(new RString("envers"));
	stop.InsertPtr(new RString("environ"));
	stop.InsertPtr(new RString("emblée"));
	stop.InsertPtr(new RString("ès"));
	stop.InsertPtr(new RString("étaient"));
	stop.InsertPtr(new RString("étais"));
	stop.InsertPtr(new RString("était"));
	stop.InsertPtr(new RString("étant"));
	stop.InsertPtr(new RString("été"));
	stop.InsertPtr(new RString("étiez"));
	stop.InsertPtr(new RString("étions"));
	stop.InsertPtr(new RString("êtes"));
	stop.InsertPtr(new RString("être"));
	stop.InsertPtr(new RString("eûmes"));
	stop.InsertPtr(new RString("eût"));
	stop.InsertPtr(new RString("eûtes"));
	stop.InsertPtr(new RString("es"));
	stop.InsertPtr(new RString("est"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("eu"));
	stop.InsertPtr(new RString("eue"));
	stop.InsertPtr(new RString("eues"));
	stop.InsertPtr(new RString("euh"));
	stop.InsertPtr(new RString("eurent"));
	stop.InsertPtr(new RString("eus"));
	stop.InsertPtr(new RString("eusse"));
	stop.InsertPtr(new RString("eussent"));
	stop.InsertPtr(new RString("eusses"));
	stop.InsertPtr(new RString("eussiez"));
	stop.InsertPtr(new RString("eussions"));
	stop.InsertPtr(new RString("eut"));
	stop.InsertPtr(new RString("eux"));
	stop.InsertPtr(new RString("exprès"));
	stop.InsertPtr(new RString("extenso"));
	stop.InsertPtr(new RString("extremis"));
	stop.InsertPtr(new RString("facto"));
	stop.InsertPtr(new RString("faire"));
	stop.InsertPtr(new RString("fait"));
	stop.InsertPtr(new RString("faveur"));
	stop.InsertPtr(new RString("fûmes"));
	stop.InsertPtr(new RString("fût"));
	stop.InsertPtr(new RString("fûtes"));
	stop.InsertPtr(new RString("fi"));
	stop.InsertPtr(new RString("flac"));
	stop.InsertPtr(new RString("fors"));
	stop.InsertPtr(new RString("fort"));
	stop.InsertPtr(new RString("forte"));
	stop.InsertPtr(new RString("fortiori"));
	stop.InsertPtr(new RString("frais"));
	stop.InsertPtr(new RString("fur"));
	stop.InsertPtr(new RString("furent"));
	stop.InsertPtr(new RString("fus"));
	stop.InsertPtr(new RString("fusse"));
	stop.InsertPtr(new RString("fussent"));
	stop.InsertPtr(new RString("fusses"));
	stop.InsertPtr(new RString("fussiez"));
	stop.InsertPtr(new RString("fussions"));
	stop.InsertPtr(new RString("fut"));
	stop.InsertPtr(new RString("GHz"));
	stop.InsertPtr(new RString("gr"));
	stop.InsertPtr(new RString("grâce"));
	stop.InsertPtr(new RString("grosso"));
	stop.InsertPtr(new RString("guère"));
	stop.InsertPtr(new RString("ha"));
	stop.InsertPtr(new RString("han"));
	stop.InsertPtr(new RString("haut"));
	stop.InsertPtr(new RString("hé"));
	stop.InsertPtr(new RString("hein"));
	stop.InsertPtr(new RString("hem"));
	stop.InsertPtr(new RString("heu"));
	stop.InsertPtr(new RString("hg"));
	stop.InsertPtr(new RString("hier"));
	stop.InsertPtr(new RString("hl"));
	stop.InsertPtr(new RString("hl"));
	stop.InsertPtr(new RString("hm"));
	stop.InsertPtr(new RString("hm³"));
	stop.InsertPtr(new RString("holà"));
	stop.InsertPtr(new RString("hop"));
	stop.InsertPtr(new RString("hormis"));
	stop.InsertPtr(new RString("hors"));
	stop.InsertPtr(new RString("hui"));
	stop.InsertPtr(new RString("huit"));
	stop.InsertPtr(new RString("hum"));
	stop.InsertPtr(new RString("ibidem"));
	stop.InsertPtr(new RString("ici"));
	stop.InsertPtr(new RString("idem"));
	stop.InsertPtr(new RString("il"));
	stop.InsertPtr(new RString("illico"));
	stop.InsertPtr(new RString("ils"));
	stop.InsertPtr(new RString("ipso"));
	stop.InsertPtr(new RString("instar"));
	stop.InsertPtr(new RString("insu"));
	stop.InsertPtr(new RString("issue"));
	stop.InsertPtr(new RString("item"));
	stop.InsertPtr(new RString("j"));
	stop.InsertPtr(new RString("jadis"));
	stop.InsertPtr(new RString("jamais"));
	stop.InsertPtr(new RString("je"));
	stop.InsertPtr(new RString("jusqu"));
	stop.InsertPtr(new RString("jusque"));
	stop.InsertPtr(new RString("juste"));
	stop.InsertPtr(new RString("kg"));
	stop.InsertPtr(new RString("km"));
	stop.InsertPtr(new RString("km²"));
	stop.InsertPtr(new RString("l"));
	stop.InsertPtr(new RString("la"));
	stop.InsertPtr(new RString("ladite"));
	stop.InsertPtr(new RString("laquelle"));
	stop.InsertPtr(new RString("là"));
	stop.InsertPtr(new RString("lès"));
	stop.InsertPtr(new RString("le"));
	stop.InsertPtr(new RString("lequel"));
	stop.InsertPtr(new RString("les"));
	stop.InsertPtr(new RString("les"));
	stop.InsertPtr(new RString("lesquelles"));
	stop.InsertPtr(new RString("lesquels"));
	stop.InsertPtr(new RString("leur"));
	stop.InsertPtr(new RString("leurs"));
	stop.InsertPtr(new RString("lez"));
	stop.InsertPtr(new RString("loin"));
	stop.InsertPtr(new RString("longtemps"));
	stop.InsertPtr(new RString("lors"));
	stop.InsertPtr(new RString("lorsqu'"));
	stop.InsertPtr(new RString("lorsque"));
	stop.InsertPtr(new RString("lui"));
	stop.InsertPtr(new RString("m"));
	stop.InsertPtr(new RString("ma"));
	stop.InsertPtr(new RString("maint"));
	stop.InsertPtr(new RString("mainte"));
	stop.InsertPtr(new RString("maintenant"));
	stop.InsertPtr(new RString("maintes"));
	stop.InsertPtr(new RString("maints"));
	stop.InsertPtr(new RString("mais"));
	stop.InsertPtr(new RString("mal"));
	stop.InsertPtr(new RString("malgré"));
	stop.InsertPtr(new RString("même"));
	stop.InsertPtr(new RString("mêmes"));
	stop.InsertPtr(new RString("m²"));
	stop.InsertPtr(new RString("m³"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("mes"));
	stop.InsertPtr(new RString("mg"));
	stop.InsertPtr(new RString("mgr"));
	stop.InsertPtr(new RString("MHz"));
	stop.InsertPtr(new RString("mien"));
	stop.InsertPtr(new RString("mienne"));
	stop.InsertPtr(new RString("miennes"));
	stop.InsertPtr(new RString("miens"));
	stop.InsertPtr(new RString("mieux"));
	stop.InsertPtr(new RString("mil"));
	stop.InsertPtr(new RString("mille"));
	stop.InsertPtr(new RString("milliards"));
	stop.InsertPtr(new RString("millions"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("minima"));
	stop.InsertPtr(new RString("ml"));
	stop.InsertPtr(new RString("mm"));
	stop.InsertPtr(new RString("mm²"));
	stop.InsertPtr(new RString("modo"));
	stop.InsertPtr(new RString("moi"));
	stop.InsertPtr(new RString("moins"));
	stop.InsertPtr(new RString("mon"));
	stop.InsertPtr(new RString("moult"));
	stop.InsertPtr(new RString("moyennant"));
	stop.InsertPtr(new RString("msec"));
	stop.InsertPtr(new RString("mt"));
	stop.InsertPtr(new RString("n"));
	stop.InsertPtr(new RString("naguère"));
	stop.InsertPtr(new RString("néanmoins"));
	stop.InsertPtr(new RString("nº"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("neuf"));
	stop.InsertPtr(new RString("ni"));
	stop.InsertPtr(new RString("nm"));
	stop.InsertPtr(new RString("non"));
	stop.InsertPtr(new RString("nonante"));
	stop.InsertPtr(new RString("nonobstant"));
	stop.InsertPtr(new RString("nos"));
	stop.InsertPtr(new RString("notre"));
	stop.InsertPtr(new RString("nôtre"));
	stop.InsertPtr(new RString("nous"));
	stop.InsertPtr(new RString("nul"));
	stop.InsertPtr(new RString("nulle"));
	stop.InsertPtr(new RString("ô"));
	stop.InsertPtr(new RString("où"));
	stop.InsertPtr(new RString("occasion"));
	stop.InsertPtr(new RString("octante"));
	stop.InsertPtr(new RString("oh"));
	stop.InsertPtr(new RString("on"));
	stop.InsertPtr(new RString("ont"));
	stop.InsertPtr(new RString("onze"));
	stop.InsertPtr(new RString("or"));
	stop.InsertPtr(new RString("ou"));
	stop.InsertPtr(new RString("ouais"));
	stop.InsertPtr(new RString("oui"));
	stop.InsertPtr(new RString("outre"));
	stop.InsertPtr(new RString("par"));
	stop.InsertPtr(new RString("parbleu"));
	stop.InsertPtr(new RString("parce"));
	stop.InsertPtr(new RString("parce_qu'"));
	stop.InsertPtr(new RString("parce_que"));
	stop.InsertPtr(new RString("parfois"));
	stop.InsertPtr(new RString("parmi"));
	stop.InsertPtr(new RString("part"));
	stop.InsertPtr(new RString("partout"));
	stop.InsertPtr(new RString("pas"));
	stop.InsertPtr(new RString("passé"));
	stop.InsertPtr(new RString("passim"));
	stop.InsertPtr(new RString("pendant"));
	stop.InsertPtr(new RString("personne"));
	stop.InsertPtr(new RString("petto"));
	stop.InsertPtr(new RString("peu"));
	stop.InsertPtr(new RString("peut"));
	stop.InsertPtr(new RString("pis"));
	stop.InsertPtr(new RString("plus"));
	stop.InsertPtr(new RString("plusieurs"));
	stop.InsertPtr(new RString("plutôt"));
	stop.InsertPtr(new RString("point"));
	stop.InsertPtr(new RString("posteriori"));
	stop.InsertPtr(new RString("pour"));
	stop.InsertPtr(new RString("pourquoi"));
	stop.InsertPtr(new RString("pourtant"));
	stop.InsertPtr(new RString("pourvu"));
	stop.InsertPtr(new RString("près"));
	stop.InsertPtr(new RString("préalable"));
	stop.InsertPtr(new RString("presqu'"));
	stop.InsertPtr(new RString("presque"));
	stop.InsertPtr(new RString("primo"));
	stop.InsertPtr(new RString("priori"));
	stop.InsertPtr(new RString("prou"));
	stop.InsertPtr(new RString("puis"));
	stop.InsertPtr(new RString("puisqu"));
	stop.InsertPtr(new RString("puisque"));
	stop.InsertPtr(new RString("qu"));
	stop.InsertPtr(new RString("qua"));
	stop.InsertPtr(new RString("quand"));
	stop.InsertPtr(new RString("quant"));
	stop.InsertPtr(new RString("quarante"));
	stop.InsertPtr(new RString("quasi"));
	stop.InsertPtr(new RString("quatorze"));
	stop.InsertPtr(new RString("quatre"));
	stop.InsertPtr(new RString("que"));
	stop.InsertPtr(new RString("quel"));
	stop.InsertPtr(new RString("quelle"));
	stop.InsertPtr(new RString("quelles"));
	stop.InsertPtr(new RString("quelqu"));
	stop.InsertPtr(new RString("quelque"));
	stop.InsertPtr(new RString("quelquefois"));
	stop.InsertPtr(new RString("quelques"));
	stop.InsertPtr(new RString("quels"));
	stop.InsertPtr(new RString("qui"));
	stop.InsertPtr(new RString("quiconque"));
	stop.InsertPtr(new RString("quinze"));
	stop.InsertPtr(new RString("quoi"));
	stop.InsertPtr(new RString("quoiqu"));
	stop.InsertPtr(new RString("quoique"));
	stop.InsertPtr(new RString("regard"));
	stop.InsertPtr(new RString("revoici"));
	stop.InsertPtr(new RString("revoilà"));
	stop.InsertPtr(new RString("rien"));
	stop.InsertPtr(new RString("s"));
	stop.InsertPtr(new RString("sa"));
	stop.InsertPtr(new RString("sans"));
	stop.InsertPtr(new RString("sauf"));
	stop.InsertPtr(new RString("se"));
	stop.InsertPtr(new RString("sec"));
	stop.InsertPtr(new RString("secundo"));
	stop.InsertPtr(new RString("sein"));
	stop.InsertPtr(new RString("seize"));
	stop.InsertPtr(new RString("selon"));
	stop.InsertPtr(new RString("sensu"));
	stop.InsertPtr(new RString("sept"));
	stop.InsertPtr(new RString("septante"));
	stop.InsertPtr(new RString("sera"));
	stop.InsertPtr(new RString("serai"));
	stop.InsertPtr(new RString("seraient"));
	stop.InsertPtr(new RString("serais"));
	stop.InsertPtr(new RString("serait"));
	stop.InsertPtr(new RString("seras"));
	stop.InsertPtr(new RString("serez"));
	stop.InsertPtr(new RString("seriez"));
	stop.InsertPtr(new RString("serions"));
	stop.InsertPtr(new RString("serons"));
	stop.InsertPtr(new RString("seront"));
	stop.InsertPtr(new RString("ses"));
	stop.InsertPtr(new RString("si"));
	stop.InsertPtr(new RString("sic"));
	stop.InsertPtr(new RString("sien"));
	stop.InsertPtr(new RString("sienne"));
	stop.InsertPtr(new RString("siennes"));
	stop.InsertPtr(new RString("siens"));
	stop.InsertPtr(new RString("sine"));
	stop.InsertPtr(new RString("sinon"));
	stop.InsertPtr(new RString("sitôt"));
	stop.InsertPtr(new RString("situ"));
	stop.InsertPtr(new RString("six"));
	stop.InsertPtr(new RString("soi"));
	stop.InsertPtr(new RString("soient"));
	stop.InsertPtr(new RString("sois"));
	stop.InsertPtr(new RString("soit"));
	stop.InsertPtr(new RString("soixante"));
	stop.InsertPtr(new RString("sommes"));
	stop.InsertPtr(new RString("son"));
	stop.InsertPtr(new RString("sont"));
	stop.InsertPtr(new RString("soudain"));
	stop.InsertPtr(new RString("sous"));
	stop.InsertPtr(new RString("souvent"));
	stop.InsertPtr(new RString("soyez"));
	stop.InsertPtr(new RString("soyons"));
	stop.InsertPtr(new RString("stricto"));
	stop.InsertPtr(new RString("suis"));
	stop.InsertPtr(new RString("suite"));
	stop.InsertPtr(new RString("sujet"));
	stop.InsertPtr(new RString("sur"));
	stop.InsertPtr(new RString("surtout"));
	stop.InsertPtr(new RString("sus"));
	stop.InsertPtr(new RString("t"));
	stop.InsertPtr(new RString("ta"));
	stop.InsertPtr(new RString("tacatac"));
	stop.InsertPtr(new RString("tandis"));
	stop.InsertPtr(new RString("tant"));
	stop.InsertPtr(new RString("tantôt"));
	stop.InsertPtr(new RString("tard"));
	stop.InsertPtr(new RString("tôt"));
	stop.InsertPtr(new RString("te"));
	stop.InsertPtr(new RString("tel"));
	stop.InsertPtr(new RString("telle"));
	stop.InsertPtr(new RString("telles"));
	stop.InsertPtr(new RString("tels"));
	stop.InsertPtr(new RString("temps"));
	stop.InsertPtr(new RString("ter"));
	stop.InsertPtr(new RString("tes"));
	stop.InsertPtr(new RString("tien"));
	stop.InsertPtr(new RString("tienne"));
	stop.InsertPtr(new RString("tiennes"));
	stop.InsertPtr(new RString("tiens"));
	stop.InsertPtr(new RString("toi"));
	stop.InsertPtr(new RString("ton"));
	stop.InsertPtr(new RString("toujours"));
	stop.InsertPtr(new RString("tous"));
	stop.InsertPtr(new RString("tout"));
	stop.InsertPtr(new RString("toute"));
	stop.InsertPtr(new RString("toutefois"));
	stop.InsertPtr(new RString("toutes"));
	stop.InsertPtr(new RString("travers"));
	stop.InsertPtr(new RString("très"));
	stop.InsertPtr(new RString("treize"));
	stop.InsertPtr(new RString("trente"));
	stop.InsertPtr(new RString("trois"));
	stop.InsertPtr(new RString("trop"));
	stop.InsertPtr(new RString("tu"));
	stop.InsertPtr(new RString("un"));
	stop.InsertPtr(new RString("une"));
	stop.InsertPtr(new RString("unes"));
	stop.InsertPtr(new RString("uns"));
	stop.InsertPtr(new RString("vers"));
	stop.InsertPtr(new RString("versa"));
	stop.InsertPtr(new RString("via"));
	stop.InsertPtr(new RString("vingt"));
	stop.InsertPtr(new RString("vis"));
	stop.InsertPtr(new RString("vite"));
	stop.InsertPtr(new RString("vitro"));
	stop.InsertPtr(new RString("vivo"));
	stop.InsertPtr(new RString("voici"));
	stop.InsertPtr(new RString("voilà"));
	stop.InsertPtr(new RString("voire"));
	stop.InsertPtr(new RString("vos"));
	stop.InsertPtr(new RString("votre"));
	stop.InsertPtr(new RString("vôtre"));
	stop.InsertPtr(new RString("vous"));
	stop.InsertPtr(new RString("vue"));
	stop.InsertPtr(new RString("y"));
	stop.InsertPtr(new RString("zéro"));
}


//-----------------------------------------------------------------------------
RString GLangFR::GetStemming(const RString& _kwd)
{
	RString res;
	char kwd[51];
	char* end;
	size_t len;

	// Transform _kwd in lower case and store it in kwd.
	res=_kwd.ToLower();
	if(res.GetLen()>50) return(res);
	strcpy(kwd,res);
	len=_kwd.GetLen()-1;
	end=&kwd[len];

	// Do the unique step(for the moment) of the French adaptation of Porter algorithm.
	ApplyRules(kwd,end,Rules1);
	ApplyRules(kwd,end,Rules2);
	ApplyRules(kwd,end,Rules3);

	// Put the result in res and return it.
	while((*end)&&(!isspace(*end)))
		end++;
	if(*end)
		(*end)=0;
	res=kwd;
	return(res);
}


//------------------------------------------------------------------------------
void GLangFR::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangFR::~GLangFR(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("French",GLangFR,"fr")
