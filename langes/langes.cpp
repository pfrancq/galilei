/*

	GALILEI Research Project

	LangES.cpp

	Spanish Language - Implementation.

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
#include <langes.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangES
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangES::GLangES(GFactoryLang* fac)
	: GLang(fac,"Spanish","es"), Stemmer(sb_stemmer_new("spanish",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangES : Spanish is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangES::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("al"));
	stop.InsertPtr(new RString("algo"));
	stop.InsertPtr(new RString("algunas"));
	stop.InsertPtr(new RString("algunos"));
	stop.InsertPtr(new RString("ante"));
	stop.InsertPtr(new RString("antes"));
	stop.InsertPtr(new RString("él"));
	stop.InsertPtr(new RString("éramos"));
	stop.InsertPtr(new RString("como"));
	stop.InsertPtr(new RString("con"));
	stop.InsertPtr(new RString("contra"));
	stop.InsertPtr(new RString("cual"));
	stop.InsertPtr(new RString("cuando"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("del"));
	stop.InsertPtr(new RString("desde"));
	stop.InsertPtr(new RString("donde"));
	stop.InsertPtr(new RString("durante"));
	stop.InsertPtr(new RString("e"));
	stop.InsertPtr(new RString("el"));
	stop.InsertPtr(new RString("ella"));
	stop.InsertPtr(new RString("ellas"));
	stop.InsertPtr(new RString("ellos"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("entre"));
	stop.InsertPtr(new RString("era"));
	stop.InsertPtr(new RString("erais"));
	stop.InsertPtr(new RString("eran"));
	stop.InsertPtr(new RString("eras"));
	stop.InsertPtr(new RString("eres"));
	stop.InsertPtr(new RString("es"));
	stop.InsertPtr(new RString("esa"));
	stop.InsertPtr(new RString("esas"));
	stop.InsertPtr(new RString("ese"));
	stop.InsertPtr(new RString("eso"));
	stop.InsertPtr(new RString("esos"));
	stop.InsertPtr(new RString("esta"));
	stop.InsertPtr(new RString("estaba"));
	stop.InsertPtr(new RString("estabais"));
	stop.InsertPtr(new RString("estaban"));
	stop.InsertPtr(new RString("estabas"));
	stop.InsertPtr(new RString("estad"));
	stop.InsertPtr(new RString("estada"));
	stop.InsertPtr(new RString("estadas"));
	stop.InsertPtr(new RString("estado"));
	stop.InsertPtr(new RString("estados"));
	stop.InsertPtr(new RString("estamos"));
	stop.InsertPtr(new RString("estando"));
	stop.InsertPtr(new RString("estar"));
	stop.InsertPtr(new RString("estará"));
	stop.InsertPtr(new RString("estarán"));
	stop.InsertPtr(new RString("estarás"));
	stop.InsertPtr(new RString("estaré"));
	stop.InsertPtr(new RString("estaréis"));
	stop.InsertPtr(new RString("estaría"));
	stop.InsertPtr(new RString("estaríais"));
	stop.InsertPtr(new RString("estaríamos"));
	stop.InsertPtr(new RString("estarían"));
	stop.InsertPtr(new RString("estarías"));
	stop.InsertPtr(new RString("estaremos"));
	stop.InsertPtr(new RString("estas"));
	stop.InsertPtr(new RString("está"));
	stop.InsertPtr(new RString("estábamos"));
	stop.InsertPtr(new RString("estáis"));
	stop.InsertPtr(new RString("están"));
	stop.InsertPtr(new RString("estás"));
	stop.InsertPtr(new RString("esté"));
	stop.InsertPtr(new RString("estéis"));
	stop.InsertPtr(new RString("estén"));
	stop.InsertPtr(new RString("estés"));
	stop.InsertPtr(new RString("este"));
	stop.InsertPtr(new RString("estemos"));
	stop.InsertPtr(new RString("esto"));
	stop.InsertPtr(new RString("estos"));
	stop.InsertPtr(new RString("estoy"));
	stop.InsertPtr(new RString("estuve"));
	stop.InsertPtr(new RString("estuviéramos"));
	stop.InsertPtr(new RString("estuviésemos"));
	stop.InsertPtr(new RString("estuviera"));
	stop.InsertPtr(new RString("estuvierais"));
	stop.InsertPtr(new RString("estuvieran"));
	stop.InsertPtr(new RString("estuvieras"));
	stop.InsertPtr(new RString("estuvieron"));
	stop.InsertPtr(new RString("estuviese"));
	stop.InsertPtr(new RString("estuvieseis"));
	stop.InsertPtr(new RString("estuviesen"));
	stop.InsertPtr(new RString("estuvieses"));
	stop.InsertPtr(new RString("estuvimos"));
	stop.InsertPtr(new RString("estuviste"));
	stop.InsertPtr(new RString("estuvisteis"));
	stop.InsertPtr(new RString("estuvo"));
	stop.InsertPtr(new RString("fuéramos"));
	stop.InsertPtr(new RString("fuésemos"));
	stop.InsertPtr(new RString("fue"));
	stop.InsertPtr(new RString("fuera"));
	stop.InsertPtr(new RString("fuerais"));
	stop.InsertPtr(new RString("fueran"));
	stop.InsertPtr(new RString("fueras"));
	stop.InsertPtr(new RString("fueron"));
	stop.InsertPtr(new RString("fuese"));
	stop.InsertPtr(new RString("fueseis"));
	stop.InsertPtr(new RString("fuesen"));
	stop.InsertPtr(new RString("fueses"));
	stop.InsertPtr(new RString("fui"));
	stop.InsertPtr(new RString("fuimos"));
	stop.InsertPtr(new RString("fuiste"));
	stop.InsertPtr(new RString("fuisteis"));
	stop.InsertPtr(new RString("ha"));
	stop.InsertPtr(new RString("habéis"));
	stop.InsertPtr(new RString("había"));
	stop.InsertPtr(new RString("habíais"));
	stop.InsertPtr(new RString("habíamos"));
	stop.InsertPtr(new RString("habían"));
	stop.InsertPtr(new RString("habías"));
	stop.InsertPtr(new RString("habida"));
	stop.InsertPtr(new RString("habidas"));
	stop.InsertPtr(new RString("habido"));
	stop.InsertPtr(new RString("habidos"));
	stop.InsertPtr(new RString("habiendo"));
	stop.InsertPtr(new RString("habrá"));
	stop.InsertPtr(new RString("habrán"));
	stop.InsertPtr(new RString("habrás"));
	stop.InsertPtr(new RString("habré"));
	stop.InsertPtr(new RString("habréis"));
	stop.InsertPtr(new RString("habría"));
	stop.InsertPtr(new RString("habríais"));
	stop.InsertPtr(new RString("habríamos"));
	stop.InsertPtr(new RString("habrían"));
	stop.InsertPtr(new RString("habrías"));
	stop.InsertPtr(new RString("habremos"));
	stop.InsertPtr(new RString("han"));
	stop.InsertPtr(new RString("has"));
	stop.InsertPtr(new RString("hasta"));
	stop.InsertPtr(new RString("hay"));
	stop.InsertPtr(new RString("haya"));
	stop.InsertPtr(new RString("hayamos"));
	stop.InsertPtr(new RString("hayan"));
	stop.InsertPtr(new RString("hayas"));
	stop.InsertPtr(new RString("hayáis"));
	stop.InsertPtr(new RString("he"));
	stop.InsertPtr(new RString("hemos"));
	stop.InsertPtr(new RString("hube"));
	stop.InsertPtr(new RString("hubiéramos"));
	stop.InsertPtr(new RString("hubiésemos"));
	stop.InsertPtr(new RString("hubiera"));
	stop.InsertPtr(new RString("hubierais"));
	stop.InsertPtr(new RString("hubieran"));
	stop.InsertPtr(new RString("hubieras"));
	stop.InsertPtr(new RString("hubieron"));
	stop.InsertPtr(new RString("hubiese"));
	stop.InsertPtr(new RString("hubieseis"));
	stop.InsertPtr(new RString("hubiesen"));
	stop.InsertPtr(new RString("hubieses"));
	stop.InsertPtr(new RString("hubimos"));
	stop.InsertPtr(new RString("hubiste"));
	stop.InsertPtr(new RString("hubisteis"));
	stop.InsertPtr(new RString("hubo"));
	stop.InsertPtr(new RString("la"));
	stop.InsertPtr(new RString("las"));
	stop.InsertPtr(new RString("le"));
	stop.InsertPtr(new RString("les"));
	stop.InsertPtr(new RString("lo"));
	stop.InsertPtr(new RString("los"));
	stop.InsertPtr(new RString("más"));
	stop.InsertPtr(new RString("mí"));
	stop.InsertPtr(new RString("mía"));
	stop.InsertPtr(new RString("mías"));
	stop.InsertPtr(new RString("mío"));
	stop.InsertPtr(new RString("míos"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("mi"));
	stop.InsertPtr(new RString("mis"));
	stop.InsertPtr(new RString("mucho"));
	stop.InsertPtr(new RString("muchos"));
	stop.InsertPtr(new RString("muy"));
	stop.InsertPtr(new RString("nada"));
	stop.InsertPtr(new RString("ni"));
	stop.InsertPtr(new RString("no"));
	stop.InsertPtr(new RString("nos"));
	stop.InsertPtr(new RString("nosotras"));
	stop.InsertPtr(new RString("nosotros"));
	stop.InsertPtr(new RString("nuestra"));
	stop.InsertPtr(new RString("nuestras"));
	stop.InsertPtr(new RString("nuestro"));
	stop.InsertPtr(new RString("nuestros"));
	stop.InsertPtr(new RString("o"));
	stop.InsertPtr(new RString("os"));
	stop.InsertPtr(new RString("otra"));
	stop.InsertPtr(new RString("otras"));
	stop.InsertPtr(new RString("otro"));
	stop.InsertPtr(new RString("otros"));
	stop.InsertPtr(new RString("para"));
	stop.InsertPtr(new RString("pero"));
	stop.InsertPtr(new RString("poco"));
	stop.InsertPtr(new RString("por"));
	stop.InsertPtr(new RString("porque"));
	stop.InsertPtr(new RString("qué"));
	stop.InsertPtr(new RString("que"));
	stop.InsertPtr(new RString("quien"));
	stop.InsertPtr(new RString("quienes"));
	stop.InsertPtr(new RString("sí"));
	stop.InsertPtr(new RString("se"));
	stop.InsertPtr(new RString("sea"));
	stop.InsertPtr(new RString("seamos"));
	stop.InsertPtr(new RString("sean"));
	stop.InsertPtr(new RString("seas"));
	stop.InsertPtr(new RString("seáis"));
	stop.InsertPtr(new RString("sentid"));
	stop.InsertPtr(new RString("sentida"));
	stop.InsertPtr(new RString("sentidas"));
	stop.InsertPtr(new RString("sentido"));
	stop.InsertPtr(new RString("sentidos"));
	stop.InsertPtr(new RString("será"));
	stop.InsertPtr(new RString("serán"));
	stop.InsertPtr(new RString("serás"));
	stop.InsertPtr(new RString("seré"));
	stop.InsertPtr(new RString("seréis"));
	stop.InsertPtr(new RString("sería"));
	stop.InsertPtr(new RString("seríais"));
	stop.InsertPtr(new RString("seríamos"));
	stop.InsertPtr(new RString("serían"));
	stop.InsertPtr(new RString("serías"));
	stop.InsertPtr(new RString("seremos"));
	stop.InsertPtr(new RString("siente"));
	stop.InsertPtr(new RString("sin"));
	stop.InsertPtr(new RString("sintiendo"));
	stop.InsertPtr(new RString("sobre"));
	stop.InsertPtr(new RString("sois"));
	stop.InsertPtr(new RString("somos"));
	stop.InsertPtr(new RString("son"));
	stop.InsertPtr(new RString("soy"));
	stop.InsertPtr(new RString("su"));
	stop.InsertPtr(new RString("sus"));
	stop.InsertPtr(new RString("suya"));
	stop.InsertPtr(new RString("suyas"));
	stop.InsertPtr(new RString("suyo"));
	stop.InsertPtr(new RString("suyos"));
	stop.InsertPtr(new RString("también"));
	stop.InsertPtr(new RString("tanto"));
	stop.InsertPtr(new RString("tú"));
	stop.InsertPtr(new RString("te"));
	stop.InsertPtr(new RString("tenéis"));
	stop.InsertPtr(new RString("tenía"));
	stop.InsertPtr(new RString("teníais"));
	stop.InsertPtr(new RString("teníamos"));
	stop.InsertPtr(new RString("tenían"));
	stop.InsertPtr(new RString("tenías"));
	stop.InsertPtr(new RString("tendrá"));
	stop.InsertPtr(new RString("tendrán"));
	stop.InsertPtr(new RString("tendrás"));
	stop.InsertPtr(new RString("tendré"));
	stop.InsertPtr(new RString("tendréis"));
	stop.InsertPtr(new RString("tendría"));
	stop.InsertPtr(new RString("tendríais"));
	stop.InsertPtr(new RString("tendríamos"));
	stop.InsertPtr(new RString("tendrían"));
	stop.InsertPtr(new RString("tendrías"));
	stop.InsertPtr(new RString("tendremos"));
	stop.InsertPtr(new RString("tenemos"));
	stop.InsertPtr(new RString("tenga"));
	stop.InsertPtr(new RString("tengamos"));
	stop.InsertPtr(new RString("tengan"));
	stop.InsertPtr(new RString("tengas"));
	stop.InsertPtr(new RString("tengáis"));
	stop.InsertPtr(new RString("tengo"));
	stop.InsertPtr(new RString("tenida"));
	stop.InsertPtr(new RString("tenidas"));
	stop.InsertPtr(new RString("tenido"));
	stop.InsertPtr(new RString("tenidos"));
	stop.InsertPtr(new RString("teniendo"));
	stop.InsertPtr(new RString("ti"));
	stop.InsertPtr(new RString("tiene"));
	stop.InsertPtr(new RString("tienen"));
	stop.InsertPtr(new RString("tienes"));
	stop.InsertPtr(new RString("todo"));
	stop.InsertPtr(new RString("todos"));
	stop.InsertPtr(new RString("tu"));
	stop.InsertPtr(new RString("tus"));
	stop.InsertPtr(new RString("tuve"));
	stop.InsertPtr(new RString("tuviéramos"));
	stop.InsertPtr(new RString("tuviésemos"));
	stop.InsertPtr(new RString("tuviera"));
	stop.InsertPtr(new RString("tuvierais"));
	stop.InsertPtr(new RString("tuvieran"));
	stop.InsertPtr(new RString("tuvieras"));
	stop.InsertPtr(new RString("tuvieron"));
	stop.InsertPtr(new RString("tuviese"));
	stop.InsertPtr(new RString("tuvieseis"));
	stop.InsertPtr(new RString("tuviesen"));
	stop.InsertPtr(new RString("tuvieses"));
	stop.InsertPtr(new RString("tuvimos"));
	stop.InsertPtr(new RString("tuviste"));
	stop.InsertPtr(new RString("tuvisteis"));
	stop.InsertPtr(new RString("tuvo"));
	stop.InsertPtr(new RString("tuya"));
	stop.InsertPtr(new RString("tuyas"));
	stop.InsertPtr(new RString("tuyo"));
	stop.InsertPtr(new RString("tuyos"));
	stop.InsertPtr(new RString("un"));
	stop.InsertPtr(new RString("una"));
	stop.InsertPtr(new RString("uno"));
	stop.InsertPtr(new RString("unos"));
	stop.InsertPtr(new RString("vosostras"));
	stop.InsertPtr(new RString("vosostros"));
	stop.InsertPtr(new RString("vuestra"));
	stop.InsertPtr(new RString("vuestras"));
	stop.InsertPtr(new RString("vuestro"));
	stop.InsertPtr(new RString("vuestros"));
	stop.InsertPtr(new RString("y"));
	stop.InsertPtr(new RString("ya"));
	stop.InsertPtr(new RString("yo"));
}


//-----------------------------------------------------------------------------
RString GLangES::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangES::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangES::~GLangES(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("Spanish",GLangES,"es")
