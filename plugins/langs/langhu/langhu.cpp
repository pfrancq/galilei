/*

	GALILEI Research Project

	LangHU.cpp

	Hungarian Language - Implementation.

	Copyright 2001-2011 by the Snowball Project.
	Copyright 2008-2014 by Pascal Francq.

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
#include <langhu.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangSV
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangHU::GLangHU(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Hungarian","hu"), Stemmer(sb_stemmer_new("hungarian",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangHU : Hungarian is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangHU::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("ahogy"));
	stop.InsertPtr(new RString("ahol"));
	stop.InsertPtr(new RString("aki"));
	stop.InsertPtr(new RString("akik"));
	stop.InsertPtr(new RString("akkor"));
	stop.InsertPtr(new RString("alatt"));
	stop.InsertPtr(new RString("??ltal"));
	stop.InsertPtr(new RString("??ltal??ban"));
	stop.InsertPtr(new RString("amely"));
	stop.InsertPtr(new RString("amelyek"));
	stop.InsertPtr(new RString("amelyekben"));
	stop.InsertPtr(new RString("amelyeket"));
	stop.InsertPtr(new RString("amelyet"));
	stop.InsertPtr(new RString("amelynek"));
	stop.InsertPtr(new RString("ami"));
	stop.InsertPtr(new RString("amit"));
	stop.InsertPtr(new RString("amolyan"));
	stop.InsertPtr(new RString("am??g"));
	stop.InsertPtr(new RString("amikor"));
	stop.InsertPtr(new RString("??t"));
	stop.InsertPtr(new RString("abban"));
	stop.InsertPtr(new RString("ahhoz"));
	stop.InsertPtr(new RString("annak"));
	stop.InsertPtr(new RString("arra"));
	stop.InsertPtr(new RString("arr??l"));
	stop.InsertPtr(new RString("az"));
	stop.InsertPtr(new RString("azok"));
	stop.InsertPtr(new RString("azon"));
	stop.InsertPtr(new RString("azt"));
	stop.InsertPtr(new RString("azzal"));
	stop.InsertPtr(new RString("az??rt"));
	stop.InsertPtr(new RString("azt??n"));
	stop.InsertPtr(new RString("azut??n"));
	stop.InsertPtr(new RString("azonban"));
	stop.InsertPtr(new RString("b??r"));
	stop.InsertPtr(new RString("be"));
	stop.InsertPtr(new RString("bel??l"));
	stop.InsertPtr(new RString("benne"));
	stop.InsertPtr(new RString("cikk"));
	stop.InsertPtr(new RString("cikkek"));
	stop.InsertPtr(new RString("cikkeket"));
	stop.InsertPtr(new RString("csak"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("e"));
	stop.InsertPtr(new RString("eddig"));
	stop.InsertPtr(new RString("eg??sz"));
	stop.InsertPtr(new RString("egy"));
	stop.InsertPtr(new RString("egyes"));
	stop.InsertPtr(new RString("egyetlen"));
	stop.InsertPtr(new RString("egy??b"));
	stop.InsertPtr(new RString("egyik"));
	stop.InsertPtr(new RString("egyre"));
	stop.InsertPtr(new RString("ekkor"));
	stop.InsertPtr(new RString("el"));
	stop.InsertPtr(new RString("el??g"));
	stop.InsertPtr(new RString("ellen"));
	stop.InsertPtr(new RString("el??"));
	stop.InsertPtr(new RString("el??sz??r"));
	stop.InsertPtr(new RString("el??tt"));
	stop.InsertPtr(new RString("els??"));
	stop.InsertPtr(new RString("??n"));
	stop.InsertPtr(new RString("??ppen"));
	stop.InsertPtr(new RString("ebben"));
	stop.InsertPtr(new RString("ehhez"));
	stop.InsertPtr(new RString("emilyen"));
	stop.InsertPtr(new RString("ennek"));
	stop.InsertPtr(new RString("erre"));
	stop.InsertPtr(new RString("ez"));
	stop.InsertPtr(new RString("ezt"));
	stop.InsertPtr(new RString("ezek"));
	stop.InsertPtr(new RString("ezen"));
	stop.InsertPtr(new RString("ezzel"));
	stop.InsertPtr(new RString("ez??rt"));
	stop.InsertPtr(new RString("??s"));
	stop.InsertPtr(new RString("fel"));
	stop.InsertPtr(new RString("fel??"));
	stop.InsertPtr(new RString("hanem"));
	stop.InsertPtr(new RString("hiszen"));
	stop.InsertPtr(new RString("hogy"));
	stop.InsertPtr(new RString("hogyan"));
	stop.InsertPtr(new RString("igen"));
	stop.InsertPtr(new RString("??gy"));
	stop.InsertPtr(new RString("illetve"));
	stop.InsertPtr(new RString("ill"));
	stop.InsertPtr(new RString("ilyen"));
	stop.InsertPtr(new RString("ilyenkor"));
	stop.InsertPtr(new RString("ison"));
	stop.InsertPtr(new RString("ism??t"));
	stop.InsertPtr(new RString("itt"));
	stop.InsertPtr(new RString("j??"));
	stop.InsertPtr(new RString("j??l"));
	stop.InsertPtr(new RString("jobban"));
	stop.InsertPtr(new RString("kell"));
	stop.InsertPtr(new RString("kellett"));
	stop.InsertPtr(new RString("kereszt??l"));
	stop.InsertPtr(new RString("keress??nk"));
	stop.InsertPtr(new RString("ki"));
	stop.InsertPtr(new RString("k??v??l"));
	stop.InsertPtr(new RString("k??z??tt"));
	stop.InsertPtr(new RString("k??z??l"));
	stop.InsertPtr(new RString("legal??bb"));
	stop.InsertPtr(new RString("lehet"));
	stop.InsertPtr(new RString("lehetett"));
	stop.InsertPtr(new RString("legyen"));
	stop.InsertPtr(new RString("lenne"));
	stop.InsertPtr(new RString("lenni"));
	stop.InsertPtr(new RString("lesz"));
	stop.InsertPtr(new RString("lett"));
	stop.InsertPtr(new RString("maga"));
	stop.InsertPtr(new RString("mag??t"));
	stop.InsertPtr(new RString("majd"));
	stop.InsertPtr(new RString("m??r"));
	stop.InsertPtr(new RString("m??s"));
	stop.InsertPtr(new RString("m??sik"));
	stop.InsertPtr(new RString("meg"));
	stop.InsertPtr(new RString("m??g"));
	stop.InsertPtr(new RString("mellett"));
	stop.InsertPtr(new RString("mert"));
	stop.InsertPtr(new RString("mely"));
	stop.InsertPtr(new RString("melyek"));
	stop.InsertPtr(new RString("mi"));
	stop.InsertPtr(new RString("mit"));
	stop.InsertPtr(new RString("m??g"));
	stop.InsertPtr(new RString("mi??rt"));
	stop.InsertPtr(new RString("milyen"));
	stop.InsertPtr(new RString("mikor"));
	stop.InsertPtr(new RString("minden"));
	stop.InsertPtr(new RString("mindent"));
	stop.InsertPtr(new RString("mindenki"));
	stop.InsertPtr(new RString("mindig"));
	stop.InsertPtr(new RString("mint"));
	stop.InsertPtr(new RString("mintha"));
	stop.InsertPtr(new RString("mivel"));
	stop.InsertPtr(new RString("most"));
	stop.InsertPtr(new RString("nagy"));
	stop.InsertPtr(new RString("nagyobb"));
	stop.InsertPtr(new RString("nagyon"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("n??ha"));
	stop.InsertPtr(new RString("nekem"));
	stop.InsertPtr(new RString("neki"));
	stop.InsertPtr(new RString("nem"));
	stop.InsertPtr(new RString("n??h??ny"));
	stop.InsertPtr(new RString("n??lk??l"));
	stop.InsertPtr(new RString("nincs"));
	stop.InsertPtr(new RString("olyan"));
	stop.InsertPtr(new RString("ott"));
	stop.InsertPtr(new RString("??ssze"));
	stop.InsertPtr(new RString("??"));
	stop.InsertPtr(new RString("??k"));
	stop.InsertPtr(new RString("??ket"));
	stop.InsertPtr(new RString("pedig"));
	stop.InsertPtr(new RString("persze"));
	stop.InsertPtr(new RString("r??"));
	stop.InsertPtr(new RString("s"));
	stop.InsertPtr(new RString("saj??t"));
	stop.InsertPtr(new RString("sem"));
	stop.InsertPtr(new RString("semmi"));
	stop.InsertPtr(new RString("sok"));
	stop.InsertPtr(new RString("sokat"));
	stop.InsertPtr(new RString("sokkal"));
	stop.InsertPtr(new RString("sz??m??ra"));
	stop.InsertPtr(new RString("szemben"));
	stop.InsertPtr(new RString("szerint"));
	stop.InsertPtr(new RString("szinte"));
	stop.InsertPtr(new RString("tal??n"));
	stop.InsertPtr(new RString("teh??t"));
	stop.InsertPtr(new RString("teljes"));
	stop.InsertPtr(new RString("tov??bb"));
	stop.InsertPtr(new RString("tov??bb??"));
	stop.InsertPtr(new RString("t??bb"));
	stop.InsertPtr(new RString("??gy"));
	stop.InsertPtr(new RString("ugyanis"));
	stop.InsertPtr(new RString("??j"));
	stop.InsertPtr(new RString("??jabb"));
	stop.InsertPtr(new RString("??jra"));
	stop.InsertPtr(new RString("ut??n"));
	stop.InsertPtr(new RString("ut??na"));
	stop.InsertPtr(new RString("utols??"));
	stop.InsertPtr(new RString("vagy"));
	stop.InsertPtr(new RString("vagyis"));
	stop.InsertPtr(new RString("valaki"));
	stop.InsertPtr(new RString("valami"));
	stop.InsertPtr(new RString("valamint"));
	stop.InsertPtr(new RString("val??"));
	stop.InsertPtr(new RString("vagyok"));
	stop.InsertPtr(new RString("van"));
	stop.InsertPtr(new RString("vannak"));
	stop.InsertPtr(new RString("volt"));
	stop.InsertPtr(new RString("voltam"));
	stop.InsertPtr(new RString("voltak"));
	stop.InsertPtr(new RString("voltunk"));
	stop.InsertPtr(new RString("vissza"));
	stop.InsertPtr(new RString("vele"));
	stop.InsertPtr(new RString("viszont"));
	stop.InsertPtr(new RString("volna"));
}


//-----------------------------------------------------------------------------
RString GLangHU::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd,false));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangHU::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangHU::~GLangHU(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("hu","Hungarian",GLangHU)
