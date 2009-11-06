/*

	GALILEI Research Project

	LangIT.cpp

	Italian Language - Implementation.

	Copyright 2001-2009 by the Snowball Project.
	Copyright 2001-2009 by Pascal Francq.
	Copyright 2001 by David Wartel.
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
// include files for R
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langit.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangIT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangIT::GLangIT(GPluginFactory* fac)
	: GLang(fac,"Italian","it"), Stemmer(sb_stemmer_new("italian",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangIT : Italian is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangIT::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("abbia"));
	stop.InsertPtr(new RString("abbiamo"));
	stop.InsertPtr(new RString("abbiano"));
	stop.InsertPtr(new RString("abbiate"));
	stop.InsertPtr(new RString("ad"));
	stop.InsertPtr(new RString("agl"));
	stop.InsertPtr(new RString("agli"));
	stop.InsertPtr(new RString("ai"));
	stop.InsertPtr(new RString("al"));
	stop.InsertPtr(new RString("all"));
	stop.InsertPtr(new RString("alla"));
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("allo"));
	stop.InsertPtr(new RString("anche"));
	stop.InsertPtr(new RString("avemmo"));
	stop.InsertPtr(new RString("avendo"));
	stop.InsertPtr(new RString("avesse"));
	stop.InsertPtr(new RString("avessero"));
	stop.InsertPtr(new RString("avessi"));
	stop.InsertPtr(new RString("avessimo"));
	stop.InsertPtr(new RString("aveste"));
	stop.InsertPtr(new RString("avesti"));
	stop.InsertPtr(new RString("avete"));
	stop.InsertPtr(new RString("aveva"));
	stop.InsertPtr(new RString("avevamo"));
	stop.InsertPtr(new RString("avevano"));
	stop.InsertPtr(new RString("avevate"));
	stop.InsertPtr(new RString("avevi"));
	stop.InsertPtr(new RString("avevo"));
	stop.InsertPtr(new RString("avrai"));
	stop.InsertPtr(new RString("avranno"));
	stop.InsertPtr(new RString("avrà"));
	stop.InsertPtr(new RString("avrò"));
	stop.InsertPtr(new RString("avrebbe"));
	stop.InsertPtr(new RString("avrebbero"));
	stop.InsertPtr(new RString("avrei"));
	stop.InsertPtr(new RString("avremmo"));
	stop.InsertPtr(new RString("avremo"));
	stop.InsertPtr(new RString("avreste"));
	stop.InsertPtr(new RString("avresti"));
	stop.InsertPtr(new RString("avrete"));
	stop.InsertPtr(new RString("avuta"));
	stop.InsertPtr(new RString("avute"));
	stop.InsertPtr(new RString("avuti"));
	stop.InsertPtr(new RString("avuto"));
	stop.InsertPtr(new RString("è"));
	stop.InsertPtr(new RString("c"));
	stop.InsertPtr(new RString("che"));
	stop.InsertPtr(new RString("chi"));
	stop.InsertPtr(new RString("ci"));
	stop.InsertPtr(new RString("coi"));
	stop.InsertPtr(new RString("col"));
	stop.InsertPtr(new RString("come"));
	stop.InsertPtr(new RString("con"));
	stop.InsertPtr(new RString("contro"));
	stop.InsertPtr(new RString("cui"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("dagl"));
	stop.InsertPtr(new RString("dagli"));
	stop.InsertPtr(new RString("dai"));
	stop.InsertPtr(new RString("dal"));
	stop.InsertPtr(new RString("dall"));
	stop.InsertPtr(new RString("dalla"));
	stop.InsertPtr(new RString("dalle"));
	stop.InsertPtr(new RString("dallo"));
	stop.InsertPtr(new RString("degl"));
	stop.InsertPtr(new RString("degli"));
	stop.InsertPtr(new RString("dei"));
	stop.InsertPtr(new RString("del"));
	stop.InsertPtr(new RString("dell"));
	stop.InsertPtr(new RString("della"));
	stop.InsertPtr(new RString("delle"));
	stop.InsertPtr(new RString("dello"));
	stop.InsertPtr(new RString("di"));
	stop.InsertPtr(new RString("dov"));
	stop.InsertPtr(new RString("dove"));
	stop.InsertPtr(new RString("e"));
	stop.InsertPtr(new RString("ebbe"));
	stop.InsertPtr(new RString("ebbero"));
	stop.InsertPtr(new RString("ebbi"));
	stop.InsertPtr(new RString("ed"));
	stop.InsertPtr(new RString("era"));
	stop.InsertPtr(new RString("erano"));
	stop.InsertPtr(new RString("eravamo"));
	stop.InsertPtr(new RString("eravate"));
	stop.InsertPtr(new RString("eri"));
	stop.InsertPtr(new RString("ero"));
	stop.InsertPtr(new RString("essendo"));
	stop.InsertPtr(new RString("faccia"));
	stop.InsertPtr(new RString("facciamo"));
	stop.InsertPtr(new RString("facciano"));
	stop.InsertPtr(new RString("facciate"));
	stop.InsertPtr(new RString("faccio"));
	stop.InsertPtr(new RString("facemmo"));
	stop.InsertPtr(new RString("facendo"));
	stop.InsertPtr(new RString("facesse"));
	stop.InsertPtr(new RString("facessero"));
	stop.InsertPtr(new RString("facessi"));
	stop.InsertPtr(new RString("facessimo"));
	stop.InsertPtr(new RString("faceste"));
	stop.InsertPtr(new RString("facesti"));
	stop.InsertPtr(new RString("faceva"));
	stop.InsertPtr(new RString("facevamo"));
	stop.InsertPtr(new RString("facevano"));
	stop.InsertPtr(new RString("facevate"));
	stop.InsertPtr(new RString("facevi"));
	stop.InsertPtr(new RString("facevo"));
	stop.InsertPtr(new RString("fai"));
	stop.InsertPtr(new RString("fanno"));
	stop.InsertPtr(new RString("farai"));
	stop.InsertPtr(new RString("faranno"));
	stop.InsertPtr(new RString("farà"));
	stop.InsertPtr(new RString("farò"));
	stop.InsertPtr(new RString("farebbe"));
	stop.InsertPtr(new RString("farebbero"));
	stop.InsertPtr(new RString("farei"));
	stop.InsertPtr(new RString("faremmo"));
	stop.InsertPtr(new RString("faremo"));
	stop.InsertPtr(new RString("fareste"));
	stop.InsertPtr(new RString("faresti"));
	stop.InsertPtr(new RString("farete"));
	stop.InsertPtr(new RString("fece"));
	stop.InsertPtr(new RString("fecero"));
	stop.InsertPtr(new RString("feci"));
	stop.InsertPtr(new RString("fosse"));
	stop.InsertPtr(new RString("fossero"));
	stop.InsertPtr(new RString("fossi"));
	stop.InsertPtr(new RString("fossimo"));
	stop.InsertPtr(new RString("foste"));
	stop.InsertPtr(new RString("fosti"));
	stop.InsertPtr(new RString("fu"));
	stop.InsertPtr(new RString("fui"));
	stop.InsertPtr(new RString("fummo"));
	stop.InsertPtr(new RString("furono"));
	stop.InsertPtr(new RString("gli"));
	stop.InsertPtr(new RString("ha"));
	stop.InsertPtr(new RString("hai"));
	stop.InsertPtr(new RString("hanno"));
	stop.InsertPtr(new RString("ho"));
	stop.InsertPtr(new RString("i"));
	stop.InsertPtr(new RString("il"));
	stop.InsertPtr(new RString("in"));
	stop.InsertPtr(new RString("io"));
	stop.InsertPtr(new RString("l"));
	stop.InsertPtr(new RString("la"));
	stop.InsertPtr(new RString("le"));
	stop.InsertPtr(new RString("lei"));
	stop.InsertPtr(new RString("li"));
	stop.InsertPtr(new RString("lo"));
	stop.InsertPtr(new RString("loro"));
	stop.InsertPtr(new RString("lui"));
	stop.InsertPtr(new RString("ma"));
	stop.InsertPtr(new RString("mi"));
	stop.InsertPtr(new RString("mia"));
	stop.InsertPtr(new RString("mie"));
	stop.InsertPtr(new RString("miei"));
	stop.InsertPtr(new RString("mio"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("negl"));
	stop.InsertPtr(new RString("negli"));
	stop.InsertPtr(new RString("nei"));
	stop.InsertPtr(new RString("nel"));
	stop.InsertPtr(new RString("nell"));
	stop.InsertPtr(new RString("nella"));
	stop.InsertPtr(new RString("nelle"));
	stop.InsertPtr(new RString("nello"));
	stop.InsertPtr(new RString("noi"));
	stop.InsertPtr(new RString("non"));
	stop.InsertPtr(new RString("nostra"));
	stop.InsertPtr(new RString("nostre"));
	stop.InsertPtr(new RString("nostri"));
	stop.InsertPtr(new RString("nostro"));
	stop.InsertPtr(new RString("o"));
	stop.InsertPtr(new RString("per"));
	stop.InsertPtr(new RString("perché"));
	stop.InsertPtr(new RString("più"));
	stop.InsertPtr(new RString("quale"));
	stop.InsertPtr(new RString("quanta"));
	stop.InsertPtr(new RString("quante"));
	stop.InsertPtr(new RString("quanti"));
	stop.InsertPtr(new RString("quanto"));
	stop.InsertPtr(new RString("quella"));
	stop.InsertPtr(new RString("quelle"));
	stop.InsertPtr(new RString("quelli"));
	stop.InsertPtr(new RString("quello"));
	stop.InsertPtr(new RString("questa"));
	stop.InsertPtr(new RString("queste"));
	stop.InsertPtr(new RString("questi"));
	stop.InsertPtr(new RString("questo"));
	stop.InsertPtr(new RString("sarai"));
	stop.InsertPtr(new RString("saranno"));
	stop.InsertPtr(new RString("sarà"));
	stop.InsertPtr(new RString("sarò"));
	stop.InsertPtr(new RString("sarebbe"));
	stop.InsertPtr(new RString("sarebbero"));
	stop.InsertPtr(new RString("sarei"));
	stop.InsertPtr(new RString("saremmo"));
	stop.InsertPtr(new RString("saremo"));
	stop.InsertPtr(new RString("sareste"));
	stop.InsertPtr(new RString("saresti"));
	stop.InsertPtr(new RString("sarete"));
	stop.InsertPtr(new RString("se"));
	stop.InsertPtr(new RString("sei"));
	stop.InsertPtr(new RString("si"));
	stop.InsertPtr(new RString("sia"));
	stop.InsertPtr(new RString("siamo"));
	stop.InsertPtr(new RString("siano"));
	stop.InsertPtr(new RString("siate"));
	stop.InsertPtr(new RString("siete"));
	stop.InsertPtr(new RString("sono"));
	stop.InsertPtr(new RString("sta"));
	stop.InsertPtr(new RString("stai"));
	stop.InsertPtr(new RString("stanno"));
	stop.InsertPtr(new RString("starai"));
	stop.InsertPtr(new RString("staranno"));
	stop.InsertPtr(new RString("starà"));
	stop.InsertPtr(new RString("starò"));
	stop.InsertPtr(new RString("starebbe"));
	stop.InsertPtr(new RString("starebbero"));
	stop.InsertPtr(new RString("starei"));
	stop.InsertPtr(new RString("staremmo"));
	stop.InsertPtr(new RString("staremo"));
	stop.InsertPtr(new RString("stareste"));
	stop.InsertPtr(new RString("staresti"));
	stop.InsertPtr(new RString("starete"));
	stop.InsertPtr(new RString("stava"));
	stop.InsertPtr(new RString("stavamo"));
	stop.InsertPtr(new RString("stavano"));
	stop.InsertPtr(new RString("stavate"));
	stop.InsertPtr(new RString("stavi"));
	stop.InsertPtr(new RString("stavo"));
	stop.InsertPtr(new RString("stemmo"));
	stop.InsertPtr(new RString("stesse"));
	stop.InsertPtr(new RString("stessero"));
	stop.InsertPtr(new RString("stessi"));
	stop.InsertPtr(new RString("stessimo"));
	stop.InsertPtr(new RString("steste"));
	stop.InsertPtr(new RString("stesti"));
	stop.InsertPtr(new RString("stette"));
	stop.InsertPtr(new RString("stettero"));
	stop.InsertPtr(new RString("stetti"));
	stop.InsertPtr(new RString("stia"));
	stop.InsertPtr(new RString("stiamo"));
	stop.InsertPtr(new RString("stiano"));
	stop.InsertPtr(new RString("stiate"));
	stop.InsertPtr(new RString("sto"));
	stop.InsertPtr(new RString("su"));
	stop.InsertPtr(new RString("sua"));
	stop.InsertPtr(new RString("sue"));
	stop.InsertPtr(new RString("sugl"));
	stop.InsertPtr(new RString("sugli"));
	stop.InsertPtr(new RString("sui"));
	stop.InsertPtr(new RString("sul"));
	stop.InsertPtr(new RString("sull"));
	stop.InsertPtr(new RString("sulla"));
	stop.InsertPtr(new RString("sulle"));
	stop.InsertPtr(new RString("sullo"));
	stop.InsertPtr(new RString("suo"));
	stop.InsertPtr(new RString("suoi"));
	stop.InsertPtr(new RString("ti"));
	stop.InsertPtr(new RString("tra"));
	stop.InsertPtr(new RString("tu"));
	stop.InsertPtr(new RString("tua"));
	stop.InsertPtr(new RString("tue"));
	stop.InsertPtr(new RString("tuo"));
	stop.InsertPtr(new RString("tuoi"));
	stop.InsertPtr(new RString("tutti"));
	stop.InsertPtr(new RString("tutto"));
	stop.InsertPtr(new RString("un"));
	stop.InsertPtr(new RString("una"));
	stop.InsertPtr(new RString("uno"));
	stop.InsertPtr(new RString("vi"));
	stop.InsertPtr(new RString("voi"));
	stop.InsertPtr(new RString("vostra"));
	stop.InsertPtr(new RString("vostre"));
	stop.InsertPtr(new RString("vostri"));
	stop.InsertPtr(new RString("vostro"));
}


//-----------------------------------------------------------------------------
RString GLangIT::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangIT::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangIT::~GLangIT(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("it","Italian",GLangIT)
