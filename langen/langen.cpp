/*

	GALILEI Research Project

	LangEN.cpp

	English Language - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#include <langen.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class PorterRule;
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GLangEN::PorterRule
{
public:
	enum tCondition{CondNothing,CondContainsVowel,CondAddAnE,CondRemoveAnE};

	RString OldSuffix;
	RString NewSuffix;
	int MinRootSize;
	tCondition Condition;
	bool Next;

	PorterRule(const RString& os,const RString& ns,int mr=-1,tCondition c=CondNothing,bool n=false)
		: OldSuffix(os), NewSuffix(ns), MinRootSize(mr), Condition(c), Next(n) {}
	int Compare(const PorterRule&) const {return(-1);}
};



//-----------------------------------------------------------------------------
//
// class GLangEN
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangEN::GLangEN(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"English","en"), Rules1a(4,2), Rules1b(3,2), Rules1bb(15,5), Rules1c(1,2),
	  Rules2(20,5), Rules3(7,5), Rules4(20,5), Rules5a(2,5), Rules5b(1,5)
{
	// Rules 1a
	Rules1a.InsertPtr(new PorterRule("sses","ss"));
	Rules1a.InsertPtr(new PorterRule("ies","i"));
	Rules1a.InsertPtr(new PorterRule("ss","ss"));
	Rules1a.InsertPtr(new PorterRule("s",""));

	// Rules 1b
	Rules1b.InsertPtr(new PorterRule("eed","ee",0,PorterRule::CondNothing,true));
	Rules1b.InsertPtr(new PorterRule("ed","",-1,PorterRule::CondContainsVowel,true));
	Rules1b.InsertPtr(new PorterRule("ing","",-1,PorterRule::CondContainsVowel,true));

	// Rules 1bb
	Rules1bb.InsertPtr(new PorterRule("at","ate"));
	Rules1bb.InsertPtr(new PorterRule("bl","ble"));
	Rules1bb.InsertPtr(new PorterRule("iz","ize"));
	Rules1bb.InsertPtr(new PorterRule("bb","b"));
	Rules1bb.InsertPtr(new PorterRule("dd","d"));
	Rules1bb.InsertPtr(new PorterRule("ff","f"));
	Rules1bb.InsertPtr(new PorterRule("gg","g"));
	Rules1bb.InsertPtr(new PorterRule("mm","m"));
	Rules1bb.InsertPtr(new PorterRule("nn","n"));
	Rules1bb.InsertPtr(new PorterRule("pp","p"));
	Rules1bb.InsertPtr(new PorterRule("rr","r"));
	Rules1bb.InsertPtr(new PorterRule("tt","t"));
	Rules1bb.InsertPtr(new PorterRule("ww","w"));
	Rules1bb.InsertPtr(new PorterRule("xx","x"));
	Rules1bb.InsertPtr(new PorterRule("","e"));

	// Rules 1c
	Rules1c.InsertPtr(new PorterRule("y","i",-1,PorterRule::CondContainsVowel));

	// Rules 2
	Rules2.InsertPtr(new PorterRule("ational","ate",0));
	Rules2.InsertPtr(new PorterRule("tional","tion",0));
	Rules2.InsertPtr(new PorterRule("enci","ence",0));
	Rules2.InsertPtr(new PorterRule("anci","ance",0));
	Rules2.InsertPtr(new PorterRule("izer","ize",0));
	Rules2.InsertPtr(new PorterRule("abli","able",0));
	Rules2.InsertPtr(new PorterRule("alli","al",0));
	Rules2.InsertPtr(new PorterRule("entli","ent",0));
	Rules2.InsertPtr(new PorterRule("eli","e",0));
	Rules2.InsertPtr(new PorterRule("ousli","ous",0));
	Rules2.InsertPtr(new PorterRule("ization","ize",0));
	Rules2.InsertPtr(new PorterRule("ation","ate",0));
	Rules2.InsertPtr(new PorterRule("ator","ate",0));
	Rules2.InsertPtr(new PorterRule("alism","al",0));
	Rules2.InsertPtr(new PorterRule("iveness","ive",0));
	Rules2.InsertPtr(new PorterRule("fulnes","ful",0));
	Rules2.InsertPtr(new PorterRule("ousness","ous",0));
	Rules2.InsertPtr(new PorterRule("aliti","al",0));
	Rules2.InsertPtr(new PorterRule("iviti","ive",0));
	Rules2.InsertPtr(new PorterRule("biliti","ble",0));

	// Rules 3
	Rules3.InsertPtr(new PorterRule("icate","ic",0));
	Rules3.InsertPtr(new PorterRule("ative","",0));
	Rules3.InsertPtr(new PorterRule("alize","al",0));
	Rules3.InsertPtr(new PorterRule("iciti","ic",0));
	Rules3.InsertPtr(new PorterRule("ical","ic",0));
	Rules3.InsertPtr(new PorterRule("ful","",0));
	Rules3.InsertPtr(new PorterRule("ness","",0));

	// Rules 4
	Rules4.InsertPtr(new PorterRule("al","",1));
	Rules4.InsertPtr(new PorterRule("ance","",1));
	Rules4.InsertPtr(new PorterRule("ence","",1));
	Rules4.InsertPtr(new PorterRule("er","",1));
	Rules4.InsertPtr(new PorterRule("ic","",1));
	Rules4.InsertPtr(new PorterRule("able","",1));
	Rules4.InsertPtr(new PorterRule("ible","",1));
	Rules4.InsertPtr(new PorterRule("ant","",1));
	Rules4.InsertPtr(new PorterRule("ement","",1));
	Rules4.InsertPtr(new PorterRule("ment","",1));
	Rules4.InsertPtr(new PorterRule("ent","",1));
	Rules4.InsertPtr(new PorterRule("sion","s",1));
	Rules4.InsertPtr(new PorterRule("tion","t",1));
	Rules4.InsertPtr(new PorterRule("ou","",1));
	Rules4.InsertPtr(new PorterRule("ism","",1));
	Rules4.InsertPtr(new PorterRule("ate","",1));
	Rules4.InsertPtr(new PorterRule("iti","",1));
	Rules4.InsertPtr(new PorterRule("ous","",1));
	Rules4.InsertPtr(new PorterRule("ive","",1));
	Rules4.InsertPtr(new PorterRule("ize","",1));

	// Rules 5a
	Rules5a.InsertPtr(new PorterRule("e","",1));
	Rules5a.InsertPtr(new PorterRule("e","",-1,PorterRule::CondRemoveAnE));

	// Rules 5b
	Rules5b.InsertPtr(new PorterRule("ll","l",1));

	// Skip Words
	SkipSequence("th");
	SkipSequence("st");
	SkipSequence("nd");
	SkipSequence("rd");
	SkipSequence("s");
	SkipSequence("ies");
}


//-----------------------------------------------------------------------------
void GLangEN::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("able"));
	stop.InsertPtr(new RString("about"));
	stop.InsertPtr(new RString("above"));
	stop.InsertPtr(new RString("according"));
	stop.InsertPtr(new RString("accordingly"));
	stop.InsertPtr(new RString("across"));
	stop.InsertPtr(new RString("actually"));
	stop.InsertPtr(new RString("after"));
	stop.InsertPtr(new RString("afterwards"));
	stop.InsertPtr(new RString("again"));
	stop.InsertPtr(new RString("against"));
	stop.InsertPtr(new RString("ain"));
	stop.InsertPtr(new RString("all"));
	stop.InsertPtr(new RString("allow"));
	stop.InsertPtr(new RString("allows"));
	stop.InsertPtr(new RString("almost"));
	stop.InsertPtr(new RString("alone"));
	stop.InsertPtr(new RString("along"));
	stop.InsertPtr(new RString("already"));
	stop.InsertPtr(new RString("also"));
	stop.InsertPtr(new RString("although"));
	stop.InsertPtr(new RString("always"));
	stop.InsertPtr(new RString("am"));
	stop.InsertPtr(new RString("among"));
	stop.InsertPtr(new RString("amongst"));
	stop.InsertPtr(new RString("an"));
	stop.InsertPtr(new RString("and"));
	stop.InsertPtr(new RString("another"));
	stop.InsertPtr(new RString("any"));
	stop.InsertPtr(new RString("anybody"));
	stop.InsertPtr(new RString("anyhow"));
	stop.InsertPtr(new RString("anyone"));
	stop.InsertPtr(new RString("anything"));
	stop.InsertPtr(new RString("anyway"));
	stop.InsertPtr(new RString("anyways"));
	stop.InsertPtr(new RString("anywhere"));
	stop.InsertPtr(new RString("apart"));
	stop.InsertPtr(new RString("appear"));
	stop.InsertPtr(new RString("appreciate"));
	stop.InsertPtr(new RString("appropriate"));
	stop.InsertPtr(new RString("are"));
	stop.InsertPtr(new RString("aren"));
	stop.InsertPtr(new RString("around"));
	stop.InsertPtr(new RString("as"));
	stop.InsertPtr(new RString("aside"));
	stop.InsertPtr(new RString("ask"));
	stop.InsertPtr(new RString("asking"));
	stop.InsertPtr(new RString("associated"));
	stop.InsertPtr(new RString("at"));
	stop.InsertPtr(new RString("available"));
	stop.InsertPtr(new RString("away"));
	stop.InsertPtr(new RString("awfully"));
	stop.InsertPtr(new RString("be"));
	stop.InsertPtr(new RString("became"));
	stop.InsertPtr(new RString("because"));
	stop.InsertPtr(new RString("become"));
	stop.InsertPtr(new RString("becomes"));
	stop.InsertPtr(new RString("becoming"));
	stop.InsertPtr(new RString("been"));
	stop.InsertPtr(new RString("before"));
	stop.InsertPtr(new RString("beforehand"));
	stop.InsertPtr(new RString("behind"));
	stop.InsertPtr(new RString("being"));
	stop.InsertPtr(new RString("believe"));
	stop.InsertPtr(new RString("below"));
	stop.InsertPtr(new RString("beside"));
	stop.InsertPtr(new RString("besides"));
	stop.InsertPtr(new RString("best"));
	stop.InsertPtr(new RString("better"));
	stop.InsertPtr(new RString("between"));
	stop.InsertPtr(new RString("beyond"));
	stop.InsertPtr(new RString("bill"));
	stop.InsertPtr(new RString("both"));
	stop.InsertPtr(new RString("bottom"));
	stop.InsertPtr(new RString("brief"));
	stop.InsertPtr(new RString("but"));
	stop.InsertPtr(new RString("by"));
	stop.InsertPtr(new RString("c"));
	stop.InsertPtr(new RString("call"));
	stop.InsertPtr(new RString("came"));
	stop.InsertPtr(new RString("can"));
	stop.InsertPtr(new RString("cannot"));
	stop.InsertPtr(new RString("cause"));
	stop.InsertPtr(new RString("causes"));
	stop.InsertPtr(new RString("certain"));
	stop.InsertPtr(new RString("certainly"));
	stop.InsertPtr(new RString("changes"));
	stop.InsertPtr(new RString("clearly"));
	stop.InsertPtr(new RString("cm"));
	stop.InsertPtr(new RString("co"));
	stop.InsertPtr(new RString("com"));
	stop.InsertPtr(new RString("come"));
	stop.InsertPtr(new RString("comes"));
	stop.InsertPtr(new RString("computer"));
	stop.InsertPtr(new RString("con"));
	stop.InsertPtr(new RString("concerning"));
	stop.InsertPtr(new RString("consequently"));
	stop.InsertPtr(new RString("consider"));
	stop.InsertPtr(new RString("considering"));
	stop.InsertPtr(new RString("contain"));
	stop.InsertPtr(new RString("containing"));
	stop.InsertPtr(new RString("contains"));
	stop.InsertPtr(new RString("corresponding"));
	stop.InsertPtr(new RString("could"));
	stop.InsertPtr(new RString("couldn"));
	stop.InsertPtr(new RString("course"));
	stop.InsertPtr(new RString("cry"));
	stop.InsertPtr(new RString("currently"));
	stop.InsertPtr(new RString("d"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("definitely"));
	stop.InsertPtr(new RString("describe"));
	stop.InsertPtr(new RString("described"));
	stop.InsertPtr(new RString("despite"));
	stop.InsertPtr(new RString("detail"));
	stop.InsertPtr(new RString("did"));
	stop.InsertPtr(new RString("didn"));
	stop.InsertPtr(new RString("different"));
	stop.InsertPtr(new RString("do"));
	stop.InsertPtr(new RString("does"));
	stop.InsertPtr(new RString("doesn"));
	stop.InsertPtr(new RString("doing"));
	stop.InsertPtr(new RString("don"));
	stop.InsertPtr(new RString("done"));
	stop.InsertPtr(new RString("down"));
	stop.InsertPtr(new RString("downwards"));
	stop.InsertPtr(new RString("due"));
	stop.InsertPtr(new RString("during"));
	stop.InsertPtr(new RString("each"));
	stop.InsertPtr(new RString("edu"));
	stop.InsertPtr(new RString("eg"));
	stop.InsertPtr(new RString("eight"));
	stop.InsertPtr(new RString("either"));
	stop.InsertPtr(new RString("eleven"));
	stop.InsertPtr(new RString("else"));
	stop.InsertPtr(new RString("elsewhere"));
	stop.InsertPtr(new RString("empty"));
	stop.InsertPtr(new RString("enough"));
	stop.InsertPtr(new RString("entirely"));
	stop.InsertPtr(new RString("especially"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("etc"));
	stop.InsertPtr(new RString("even"));
	stop.InsertPtr(new RString("ever"));
	stop.InsertPtr(new RString("every"));
	stop.InsertPtr(new RString("everybody"));
	stop.InsertPtr(new RString("everyone"));
	stop.InsertPtr(new RString("everything"));
	stop.InsertPtr(new RString("everywhere"));
	stop.InsertPtr(new RString("ex"));
	stop.InsertPtr(new RString("exactly"));
	stop.InsertPtr(new RString("example"));
	stop.InsertPtr(new RString("except"));
	stop.InsertPtr(new RString("far"));
	stop.InsertPtr(new RString("few"));
	stop.InsertPtr(new RString("fifteen"));
	stop.InsertPtr(new RString("fifth"));
	stop.InsertPtr(new RString("fill"));
	stop.InsertPtr(new RString("find"));
	stop.InsertPtr(new RString("fire"));
	stop.InsertPtr(new RString("first"));
	stop.InsertPtr(new RString("five"));
	stop.InsertPtr(new RString("followed"));
	stop.InsertPtr(new RString("following"));
	stop.InsertPtr(new RString("follows"));
	stop.InsertPtr(new RString("for"));
	stop.InsertPtr(new RString("former"));
	stop.InsertPtr(new RString("formerly"));
	stop.InsertPtr(new RString("forth"));
	stop.InsertPtr(new RString("forty"));
	stop.InsertPtr(new RString("four"));
	stop.InsertPtr(new RString("from"));
	stop.InsertPtr(new RString("front"));
	stop.InsertPtr(new RString("ft"));
	stop.InsertPtr(new RString("full"));
	stop.InsertPtr(new RString("further"));
	stop.InsertPtr(new RString("furthermore"));
	stop.InsertPtr(new RString("get"));
	stop.InsertPtr(new RString("gets"));
	stop.InsertPtr(new RString("getting"));
	stop.InsertPtr(new RString("give"));
	stop.InsertPtr(new RString("given"));
	stop.InsertPtr(new RString("gives"));
	stop.InsertPtr(new RString("go"));
	stop.InsertPtr(new RString("goes"));
	stop.InsertPtr(new RString("going"));
	stop.InsertPtr(new RString("gone"));
	stop.InsertPtr(new RString("got"));
	stop.InsertPtr(new RString("gotten"));
	stop.InsertPtr(new RString("gr"));
	stop.InsertPtr(new RString("greetings"));
	stop.InsertPtr(new RString("had"));
	stop.InsertPtr(new RString("hadn"));
	stop.InsertPtr(new RString("happens"));
	stop.InsertPtr(new RString("hardly"));
	stop.InsertPtr(new RString("has"));
	stop.InsertPtr(new RString("hasn"));
	stop.InsertPtr(new RString("have"));
	stop.InsertPtr(new RString("haven"));
	stop.InsertPtr(new RString("having"));
	stop.InsertPtr(new RString("he"));
	stop.InsertPtr(new RString("hello"));
	stop.InsertPtr(new RString("help"));
	stop.InsertPtr(new RString("hence"));
	stop.InsertPtr(new RString("her"));
	stop.InsertPtr(new RString("here"));
	stop.InsertPtr(new RString("hereafter"));
	stop.InsertPtr(new RString("hereby"));
	stop.InsertPtr(new RString("herein"));
	stop.InsertPtr(new RString("hereupon"));
	stop.InsertPtr(new RString("hers"));
	stop.InsertPtr(new RString("herself"));
	stop.InsertPtr(new RString("hi"));
	stop.InsertPtr(new RString("him"));
	stop.InsertPtr(new RString("himself"));
	stop.InsertPtr(new RString("his"));
	stop.InsertPtr(new RString("hither"));
	stop.InsertPtr(new RString("hopefully"));
	stop.InsertPtr(new RString("how"));
	stop.InsertPtr(new RString("howbeit"));
	stop.InsertPtr(new RString("however"));
	stop.InsertPtr(new RString("hundred"));
	stop.InsertPtr(new RString("i"));
	stop.InsertPtr(new RString("ie"));
	stop.InsertPtr(new RString("if"));
	stop.InsertPtr(new RString("ignored"));
	stop.InsertPtr(new RString("immediate"));
	stop.InsertPtr(new RString("in"));
	stop.InsertPtr(new RString("in"));
	stop.InsertPtr(new RString("inasmuch"));
	stop.InsertPtr(new RString("inc"));
	stop.InsertPtr(new RString("indeed"));
	stop.InsertPtr(new RString("indicate"));
	stop.InsertPtr(new RString("indicated"));
	stop.InsertPtr(new RString("indicates"));
	stop.InsertPtr(new RString("inner"));
	stop.InsertPtr(new RString("insofar"));
	stop.InsertPtr(new RString("instead"));
	stop.InsertPtr(new RString("into"));
	stop.InsertPtr(new RString("inward"));
	stop.InsertPtr(new RString("is"));
	stop.InsertPtr(new RString("isn"));
	stop.InsertPtr(new RString("it"));
	stop.InsertPtr(new RString("its"));
	stop.InsertPtr(new RString("itself"));
	stop.InsertPtr(new RString("just"));
	stop.InsertPtr(new RString("keep"));
	stop.InsertPtr(new RString("keeps"));
	stop.InsertPtr(new RString("kept"));
	stop.InsertPtr(new RString("km"));
	stop.InsertPtr(new RString("know"));
	stop.InsertPtr(new RString("known"));
	stop.InsertPtr(new RString("knows"));
	stop.InsertPtr(new RString("ll"));
	stop.InsertPtr(new RString("last"));
	stop.InsertPtr(new RString("lately"));
	stop.InsertPtr(new RString("later"));
	stop.InsertPtr(new RString("latter"));
	stop.InsertPtr(new RString("latterly"));
	stop.InsertPtr(new RString("least"));
	stop.InsertPtr(new RString("less"));
	stop.InsertPtr(new RString("lest"));
	stop.InsertPtr(new RString("let"));
	stop.InsertPtr(new RString("like"));
	stop.InsertPtr(new RString("liked"));
	stop.InsertPtr(new RString("likely"));
	stop.InsertPtr(new RString("little"));
	stop.InsertPtr(new RString("look"));
	stop.InsertPtr(new RString("looking"));
	stop.InsertPtr(new RString("looks"));
	stop.InsertPtr(new RString("lot"));
	stop.InsertPtr(new RString("ltd"));
	stop.InsertPtr(new RString("m"));
	stop.InsertPtr(new RString("made"));
	stop.InsertPtr(new RString("mainly"));
	stop.InsertPtr(new RString("many"));
	stop.InsertPtr(new RString("may"));
	stop.InsertPtr(new RString("maybe"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("mean"));
	stop.InsertPtr(new RString("meanwhile"));
	stop.InsertPtr(new RString("merely"));
	stop.InsertPtr(new RString("might"));
	stop.InsertPtr(new RString("mill"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("ml"));
	stop.InsertPtr(new RString("mm"));
	stop.InsertPtr(new RString("mon"));
	stop.InsertPtr(new RString("more"));
	stop.InsertPtr(new RString("moreover"));
	stop.InsertPtr(new RString("most"));
	stop.InsertPtr(new RString("mostly"));
	stop.InsertPtr(new RString("move"));
	stop.InsertPtr(new RString("much"));
	stop.InsertPtr(new RString("must"));
	stop.InsertPtr(new RString("my"));
	stop.InsertPtr(new RString("myself"));
	stop.InsertPtr(new RString("name"));
	stop.InsertPtr(new RString("namely"));
	stop.InsertPtr(new RString("nd"));
	stop.InsertPtr(new RString("near"));
	stop.InsertPtr(new RString("nearly"));
	stop.InsertPtr(new RString("necessary"));
	stop.InsertPtr(new RString("need"));
	stop.InsertPtr(new RString("needs"));
	stop.InsertPtr(new RString("neither"));
	stop.InsertPtr(new RString("never"));
	stop.InsertPtr(new RString("nevertheless"));
	stop.InsertPtr(new RString("new"));
	stop.InsertPtr(new RString("next"));
	stop.InsertPtr(new RString("nine"));
	stop.InsertPtr(new RString("no"));
	stop.InsertPtr(new RString("nobody"));
	stop.InsertPtr(new RString("non"));
	stop.InsertPtr(new RString("none"));
	stop.InsertPtr(new RString("noone"));
	stop.InsertPtr(new RString("nor"));
	stop.InsertPtr(new RString("normally"));
	stop.InsertPtr(new RString("not"));
	stop.InsertPtr(new RString("nothing"));
	stop.InsertPtr(new RString("novel"));
	stop.InsertPtr(new RString("now"));
	stop.InsertPtr(new RString("nowhere"));
	stop.InsertPtr(new RString("obviously"));
	stop.InsertPtr(new RString("of"));
	stop.InsertPtr(new RString("off"));
	stop.InsertPtr(new RString("often"));
	stop.InsertPtr(new RString("oh"));
	stop.InsertPtr(new RString("ok"));
	stop.InsertPtr(new RString("okay"));
	stop.InsertPtr(new RString("old"));
	stop.InsertPtr(new RString("on"));
	stop.InsertPtr(new RString("once"));
	stop.InsertPtr(new RString("one"));
	stop.InsertPtr(new RString("ones"));
	stop.InsertPtr(new RString("only"));
	stop.InsertPtr(new RString("onto"));
	stop.InsertPtr(new RString("or"));
	stop.InsertPtr(new RString("other"));
	stop.InsertPtr(new RString("others"));
	stop.InsertPtr(new RString("otherwise"));
	stop.InsertPtr(new RString("ought"));
	stop.InsertPtr(new RString("our"));
	stop.InsertPtr(new RString("ours"));
	stop.InsertPtr(new RString("ourselves"));
	stop.InsertPtr(new RString("out"));
	stop.InsertPtr(new RString("outside"));
	stop.InsertPtr(new RString("over"));
	stop.InsertPtr(new RString("overall"));
	stop.InsertPtr(new RString("own"));
	stop.InsertPtr(new RString("particular"));
	stop.InsertPtr(new RString("particularly"));
	stop.InsertPtr(new RString("per"));
	stop.InsertPtr(new RString("perhaps"));
	stop.InsertPtr(new RString("placed"));
	stop.InsertPtr(new RString("please"));
	stop.InsertPtr(new RString("plus"));
	stop.InsertPtr(new RString("possible"));
	stop.InsertPtr(new RString("presumably"));
	stop.InsertPtr(new RString("probably"));
	stop.InsertPtr(new RString("provides"));
	stop.InsertPtr(new RString("put"));
	stop.InsertPtr(new RString("que"));
	stop.InsertPtr(new RString("quite"));
	stop.InsertPtr(new RString("qv"));
	stop.InsertPtr(new RString("rather"));
	stop.InsertPtr(new RString("rd"));
	stop.InsertPtr(new RString("re"));
	stop.InsertPtr(new RString("really"));
	stop.InsertPtr(new RString("reasonably"));
	stop.InsertPtr(new RString("regarding"));
	stop.InsertPtr(new RString("regardless"));
	stop.InsertPtr(new RString("regards"));
	stop.InsertPtr(new RString("relatively"));
	stop.InsertPtr(new RString("respectively"));
	stop.InsertPtr(new RString("right"));
	stop.InsertPtr(new RString("s"));
	stop.InsertPtr(new RString("said"));
	stop.InsertPtr(new RString("same"));
	stop.InsertPtr(new RString("saw"));
	stop.InsertPtr(new RString("say"));
	stop.InsertPtr(new RString("saying"));
	stop.InsertPtr(new RString("says"));
	stop.InsertPtr(new RString("sec"));
	stop.InsertPtr(new RString("second"));
	stop.InsertPtr(new RString("secondly"));
	stop.InsertPtr(new RString("see"));
	stop.InsertPtr(new RString("seeing"));
	stop.InsertPtr(new RString("seem"));
	stop.InsertPtr(new RString("seemed"));
	stop.InsertPtr(new RString("seeming"));
	stop.InsertPtr(new RString("seems"));
	stop.InsertPtr(new RString("seen"));
	stop.InsertPtr(new RString("self"));
	stop.InsertPtr(new RString("selves"));
	stop.InsertPtr(new RString("sensible"));
	stop.InsertPtr(new RString("sent"));
	stop.InsertPtr(new RString("serious"));
	stop.InsertPtr(new RString("seriously"));
	stop.InsertPtr(new RString("seven"));
	stop.InsertPtr(new RString("several"));
	stop.InsertPtr(new RString("shall"));
	stop.InsertPtr(new RString("she"));
	stop.InsertPtr(new RString("should"));
	stop.InsertPtr(new RString("shouldn"));
	stop.InsertPtr(new RString("show"));
	stop.InsertPtr(new RString("since"));
	stop.InsertPtr(new RString("sincere"));
	stop.InsertPtr(new RString("six"));
	stop.InsertPtr(new RString("so"));
	stop.InsertPtr(new RString("some"));
	stop.InsertPtr(new RString("somebody"));
	stop.InsertPtr(new RString("somehow"));
	stop.InsertPtr(new RString("someone"));
	stop.InsertPtr(new RString("something"));
	stop.InsertPtr(new RString("sometime"));
	stop.InsertPtr(new RString("sometimes"));
	stop.InsertPtr(new RString("somewhat"));
	stop.InsertPtr(new RString("somewhere"));
	stop.InsertPtr(new RString("soon"));
	stop.InsertPtr(new RString("sorry"));
	stop.InsertPtr(new RString("specified"));
	stop.InsertPtr(new RString("specify"));
	stop.InsertPtr(new RString("specifying"));
	stop.InsertPtr(new RString("still"));
	stop.InsertPtr(new RString("sub"));
	stop.InsertPtr(new RString("such"));
	stop.InsertPtr(new RString("sup"));
	stop.InsertPtr(new RString("sure"));
	stop.InsertPtr(new RString("t"));
	stop.InsertPtr(new RString("take"));
	stop.InsertPtr(new RString("taken"));
	stop.InsertPtr(new RString("tell"));
	stop.InsertPtr(new RString("tends"));
	stop.InsertPtr(new RString("th"));
	stop.InsertPtr(new RString("than"));
	stop.InsertPtr(new RString("thank"));
	stop.InsertPtr(new RString("thanks"));
	stop.InsertPtr(new RString("thanx"));
	stop.InsertPtr(new RString("that"));
	stop.InsertPtr(new RString("thats"));
	stop.InsertPtr(new RString("the"));
	stop.InsertPtr(new RString("their"));
	stop.InsertPtr(new RString("theirs"));
	stop.InsertPtr(new RString("them"));
	stop.InsertPtr(new RString("themselves"));
	stop.InsertPtr(new RString("then"));
	stop.InsertPtr(new RString("thence"));
	stop.InsertPtr(new RString("there"));
	stop.InsertPtr(new RString("thereafter"));
	stop.InsertPtr(new RString("thereby"));
	stop.InsertPtr(new RString("therefore"));
	stop.InsertPtr(new RString("therein"));
	stop.InsertPtr(new RString("thereupon"));
	stop.InsertPtr(new RString("these"));
	stop.InsertPtr(new RString("they"));
	stop.InsertPtr(new RString("thick"));
	stop.InsertPtr(new RString("thin"));
	stop.InsertPtr(new RString("think"));
	stop.InsertPtr(new RString("third"));
	stop.InsertPtr(new RString("this"));
	stop.InsertPtr(new RString("thorough"));
	stop.InsertPtr(new RString("thoroughly"));
	stop.InsertPtr(new RString("those"));
	stop.InsertPtr(new RString("though"));
	stop.InsertPtr(new RString("three"));
	stop.InsertPtr(new RString("through"));
	stop.InsertPtr(new RString("throughout"));
	stop.InsertPtr(new RString("thru"));
	stop.InsertPtr(new RString("thus"));
	stop.InsertPtr(new RString("to"));
	stop.InsertPtr(new RString("together"));
	stop.InsertPtr(new RString("too"));
	stop.InsertPtr(new RString("took"));
	stop.InsertPtr(new RString("top"));
	stop.InsertPtr(new RString("toward"));
	stop.InsertPtr(new RString("towards"));
	stop.InsertPtr(new RString("tried"));
	stop.InsertPtr(new RString("tries"));
	stop.InsertPtr(new RString("truly"));
	stop.InsertPtr(new RString("try"));
	stop.InsertPtr(new RString("trying"));
	stop.InsertPtr(new RString("twelve"));
	stop.InsertPtr(new RString("twenty"));
	stop.InsertPtr(new RString("twice"));
	stop.InsertPtr(new RString("two"));
	stop.InsertPtr(new RString("u"));
	stop.InsertPtr(new RString("un"));
	stop.InsertPtr(new RString("under"));
	stop.InsertPtr(new RString("unfortunately"));
	stop.InsertPtr(new RString("unless"));
	stop.InsertPtr(new RString("unlikely"));
	stop.InsertPtr(new RString("until"));
	stop.InsertPtr(new RString("unto"));
	stop.InsertPtr(new RString("up"));
	stop.InsertPtr(new RString("upon"));
	stop.InsertPtr(new RString("us"));
	stop.InsertPtr(new RString("use"));
	stop.InsertPtr(new RString("used"));
	stop.InsertPtr(new RString("useful"));
	stop.InsertPtr(new RString("uses"));
	stop.InsertPtr(new RString("using"));
	stop.InsertPtr(new RString("usually"));
	stop.InsertPtr(new RString("uucp"));
	stop.InsertPtr(new RString("ve"));
	stop.InsertPtr(new RString("value"));
	stop.InsertPtr(new RString("various"));
	stop.InsertPtr(new RString("very"));
	stop.InsertPtr(new RString("via"));
	stop.InsertPtr(new RString("viz"));
	stop.InsertPtr(new RString("vs"));
	stop.InsertPtr(new RString("w"));
	stop.InsertPtr(new RString("want"));
	stop.InsertPtr(new RString("wants"));
	stop.InsertPtr(new RString("was"));
	stop.InsertPtr(new RString("wasn"));
	stop.InsertPtr(new RString("way"));
	stop.InsertPtr(new RString("we"));
	stop.InsertPtr(new RString("welcome"));
	stop.InsertPtr(new RString("well"));
	stop.InsertPtr(new RString("went"));
	stop.InsertPtr(new RString("were"));
	stop.InsertPtr(new RString("weren"));
	stop.InsertPtr(new RString("what"));
	stop.InsertPtr(new RString("whatever"));
	stop.InsertPtr(new RString("when"));
	stop.InsertPtr(new RString("whence"));
	stop.InsertPtr(new RString("whenever"));
	stop.InsertPtr(new RString("where"));
	stop.InsertPtr(new RString("whereas"));
	stop.InsertPtr(new RString("whereby"));
	stop.InsertPtr(new RString("wherein"));
	stop.InsertPtr(new RString("whereupon"));
	stop.InsertPtr(new RString("wherever"));
	stop.InsertPtr(new RString("whether"));
	stop.InsertPtr(new RString("which"));
	stop.InsertPtr(new RString("while"));
	stop.InsertPtr(new RString("with"));
	stop.InsertPtr(new RString("wither"));
	stop.InsertPtr(new RString("who"));
	stop.InsertPtr(new RString("whoever"));
	stop.InsertPtr(new RString("whole"));
	stop.InsertPtr(new RString("whom"));
	stop.InsertPtr(new RString("whose"));
	stop.InsertPtr(new RString("why"));
	stop.InsertPtr(new RString("will"));
	stop.InsertPtr(new RString("willing"));
	stop.InsertPtr(new RString("wish"));
	stop.InsertPtr(new RString("with"));
	stop.InsertPtr(new RString("within"));
	stop.InsertPtr(new RString("without"));
	stop.InsertPtr(new RString("won"));
	stop.InsertPtr(new RString("wonder"));
	stop.InsertPtr(new RString("would"));
	stop.InsertPtr(new RString("wouldn"));
	stop.InsertPtr(new RString("www"));
	stop.InsertPtr(new RString("yd"));
	stop.InsertPtr(new RString("yes"));
	stop.InsertPtr(new RString("yet"));
	stop.InsertPtr(new RString("you"));
	stop.InsertPtr(new RString("your"));
	stop.InsertPtr(new RString("yours"));
	stop.InsertPtr(new RString("yourself"));
	stop.InsertPtr(new RString("yourselves"));
	stop.InsertPtr(new RString("zero"));
}


//-----------------------------------------------------------------------------
int GLangEN::GetWordSize(const RString& kwd)
{
	size_t Result(0);             // WordSize of the word.
	size_t State(0);              // Current state of the machine.
	const RChar* ptr(kwd());

	while(!ptr->IsNull())
	{
		switch(State)
		{
			case 0:
				State=(IsVowel(*ptr))?1:2;
				break;

			case 1:
				State=(IsVowel(*ptr))?1:2;
				if(State==2) Result++;
				break;

			case 2:
				State=((IsVowel(*ptr))||((*ptr)=='y'))?1:2;
				break;
		}
		ptr++;
	}
	return(Result);
}


//-----------------------------------------------------------------------------
bool GLangEN::ContainsVowel(const RString& kwd)
{
	if(!kwd.GetLen())
		return(false);
	else
		return((IsVowel(kwd[static_cast<size_t>(0)]))||(kwd.FindAnyStr("aeiouy",1)!=-1));
}


//-----------------------------------------------------------------------------
bool GLangEN::EndsWithCVC(const R::RString& kwd)
{
	if(kwd.GetLen()<2)
		return(false);
	RChar End(kwd[kwd.GetLen()-1]);
	RChar End2(kwd[kwd.GetLen()-2]);
	RChar End3(kwd[kwd.GetLen()-3]);
	return(  (RString("aeiouwxy").Find(End)==-1)  &&     // Consonant
	         (RString("aeiouy").Find(End2)!=-1)     &&     // Vowel
	         (RString("aeiou").Find(End3)==-1)              // Consonant
	      );
}


//-----------------------------------------------------------------------------
bool GLangEN::ApplyRules(RString& kwd,RContainer<PorterRule,true,false>& rules)
{
	RCursor<PorterRule> Cur(rules);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Verify if the old suffix correspond to the end of the word
		if(!kwd.IsAt(Cur()->OldSuffix,-Cur()->OldSuffix.GetLen()))
			continue;

		// Verify if the minimum root size is Ok.
		int WordSize(GetWordSize(kwd));
		if(Cur()->MinRootSize>=WordSize)
			continue;

		// If there is a condition verify it.
		switch(Cur()->Condition)
		{
			case PorterRule::CondContainsVowel:
				if(!ContainsVowel(kwd))
					return(Cur()->Next);
				break;
			case PorterRule::CondAddAnE:
				if((WordSize!=1)||(EndsWithCVC(kwd)))
					return(Cur()->Next);
				break;
			case PorterRule::CondRemoveAnE:
				if((WordSize!=1)||(EndsWithCVC(kwd)))
					return(Cur()->Next);
				break;
			default:
				break;
		}

		// Replace the old suffix by the new one, and return 'Next'.
		kwd.Insert(Cur()->NewSuffix,-Cur()->OldSuffix.GetLen(),(size_t)-1);
		return(Cur()->Next);
	}
	return(false);
}


//-----------------------------------------------------------------------------
RString GLangEN::GetStemming(const RString& kwd)
{
	// Transform kwd in lower case and store it in res.
	RString res(kwd.ToLower());
	if(res.GetLen()>50)
		return(res);

	// Do the different steps of the Porter algorithm.
	ApplyRules(res,Rules1a);
	if(ApplyRules(res,Rules1b))
		ApplyRules(res,Rules1bb);
	ApplyRules(res,Rules1c);
	ApplyRules(res,Rules2);
	ApplyRules(res,Rules3);
	ApplyRules(res,Rules4);
	ApplyRules(res,Rules5a);
	ApplyRules(res,Rules5b);

	return(res);
}


//------------------------------------------------------------------------------
void GLangEN::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangEN::~GLangEN(void)
{
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("en","English",GLangEN)
