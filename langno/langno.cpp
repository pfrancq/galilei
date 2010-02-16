/*

	GALILEI Research Project

	LangNO.cpp

	Norwegian Language - Implementation.

	Copyright 2001-2010 by the Snowball Project.
	Copyright 2001-2010 by Pascal Francq.
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
#include <langno.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangNO
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangNO::GLangNO(GPlugInFactory* fac)
	: GLang(fac,"Norwegian","no"), Stemmer(sb_stemmer_new("norwegian",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangNO : Norwegian is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangNO::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("at"));
	stop.InsertPtr(new RString("av"));
	stop.InsertPtr(new RString("å"));
	stop.InsertPtr(new RString("både"));
	stop.InsertPtr(new RString("båe"));
	stop.InsertPtr(new RString("begge"));
	stop.InsertPtr(new RString("ble"));
	stop.InsertPtr(new RString("blei"));
	stop.InsertPtr(new RString("bli"));
	stop.InsertPtr(new RString("blir"));
	stop.InsertPtr(new RString("blitt"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("då"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("deg"));
	stop.InsertPtr(new RString("dei"));
	stop.InsertPtr(new RString("deim"));
	stop.InsertPtr(new RString("deira"));
	stop.InsertPtr(new RString("deires"));
	stop.InsertPtr(new RString("dem"));
	stop.InsertPtr(new RString("den"));
	stop.InsertPtr(new RString("denne"));
	stop.InsertPtr(new RString("der"));
	stop.InsertPtr(new RString("dere"));
	stop.InsertPtr(new RString("dere"));
	stop.InsertPtr(new RString("deres"));
	stop.InsertPtr(new RString("deres"));
	stop.InsertPtr(new RString("desse"));
	stop.InsertPtr(new RString("det"));
	stop.InsertPtr(new RString("dette"));
	stop.InsertPtr(new RString("di"));
	stop.InsertPtr(new RString("din"));
	stop.InsertPtr(new RString("disse"));
	stop.InsertPtr(new RString("ditt"));
	stop.InsertPtr(new RString("du"));
	stop.InsertPtr(new RString("dykk"));
	stop.InsertPtr(new RString("dykkar"));
	stop.InsertPtr(new RString("eg"));
	stop.InsertPtr(new RString("ei"));
	stop.InsertPtr(new RString("ein"));
	stop.InsertPtr(new RString("eit"));
	stop.InsertPtr(new RString("eitt"));
	stop.InsertPtr(new RString("eller"));
	stop.InsertPtr(new RString("elles"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("ett"));
	stop.InsertPtr(new RString("etter"));
	stop.InsertPtr(new RString("for"));
	stop.InsertPtr(new RString("fra"));
	stop.InsertPtr(new RString("ha"));
	stop.InsertPtr(new RString("hade"));
	stop.InsertPtr(new RString("han"));
	stop.InsertPtr(new RString("hans"));
	stop.InsertPtr(new RString("har"));
	stop.InsertPtr(new RString("hennar"));
	stop.InsertPtr(new RString("henne"));
	stop.InsertPtr(new RString("hennes"));
	stop.InsertPtr(new RString("her"));
	stop.InsertPtr(new RString("hjå"));
	stop.InsertPtr(new RString("ho"));
	stop.InsertPtr(new RString("hoe"));
	stop.InsertPtr(new RString("honom"));
	stop.InsertPtr(new RString("hoss"));
	stop.InsertPtr(new RString("hossen"));
	stop.InsertPtr(new RString("hu"));
	stop.InsertPtr(new RString("hun"));
	stop.InsertPtr(new RString("hva"));
	stop.InsertPtr(new RString("hvem"));
	stop.InsertPtr(new RString("hver"));
	stop.InsertPtr(new RString("hvilke"));
	stop.InsertPtr(new RString("hvilken"));
	stop.InsertPtr(new RString("hvis"));
	stop.InsertPtr(new RString("hvor"));
	stop.InsertPtr(new RString("hvordan"));
	stop.InsertPtr(new RString("hvorfor"));
	stop.InsertPtr(new RString("i"));
	stop.InsertPtr(new RString("ikke"));
	stop.InsertPtr(new RString("ikkje"));
	stop.InsertPtr(new RString("inga"));
	stop.InsertPtr(new RString("ingen"));
	stop.InsertPtr(new RString("ingi"));
	stop.InsertPtr(new RString("inkje"));
	stop.InsertPtr(new RString("inn"));
	stop.InsertPtr(new RString("inni"));
	stop.InsertPtr(new RString("inte"));
	stop.InsertPtr(new RString("ja"));
	stop.InsertPtr(new RString("jeg"));
	stop.InsertPtr(new RString("kan"));
	stop.InsertPtr(new RString("kom"));
	stop.InsertPtr(new RString("korleis"));
	stop.InsertPtr(new RString("korso"));
	stop.InsertPtr(new RString("kun"));
	stop.InsertPtr(new RString("kunne"));
	stop.InsertPtr(new RString("kva"));
	stop.InsertPtr(new RString("kvar"));
	stop.InsertPtr(new RString("kvarhelst"));
	stop.InsertPtr(new RString("kven"));
	stop.InsertPtr(new RString("kvi"));
	stop.InsertPtr(new RString("kvifor"));
	stop.InsertPtr(new RString("man"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("med"));
	stop.InsertPtr(new RString("medan"));
	stop.InsertPtr(new RString("meg"));
	stop.InsertPtr(new RString("meget"));
	stop.InsertPtr(new RString("mellom"));
	stop.InsertPtr(new RString("men"));
	stop.InsertPtr(new RString("mi"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("mine"));
	stop.InsertPtr(new RString("mitt"));
	stop.InsertPtr(new RString("mot"));
	stop.InsertPtr(new RString("mykje"));
	stop.InsertPtr(new RString("nå"));
	stop.InsertPtr(new RString("når"));
	stop.InsertPtr(new RString("ned"));
	stop.InsertPtr(new RString("no"));
	stop.InsertPtr(new RString("noe"));
	stop.InsertPtr(new RString("noen"));
	stop.InsertPtr(new RString("noka"));
	stop.InsertPtr(new RString("noko"));
	stop.InsertPtr(new RString("nokon"));
	stop.InsertPtr(new RString("nokor"));
	stop.InsertPtr(new RString("nokre"));
	stop.InsertPtr(new RString("og"));
	stop.InsertPtr(new RString("om"));
	stop.InsertPtr(new RString("opp"));
	stop.InsertPtr(new RString("oss"));
	stop.InsertPtr(new RString("over"));
	stop.InsertPtr(new RString("på"));
	stop.InsertPtr(new RString("samme"));
	stop.InsertPtr(new RString("så"));
	stop.InsertPtr(new RString("sånn"));
	stop.InsertPtr(new RString("seg"));
	stop.InsertPtr(new RString("selv"));
	stop.InsertPtr(new RString("si"));
	stop.InsertPtr(new RString("si"));
	stop.InsertPtr(new RString("sia"));
	stop.InsertPtr(new RString("sia"));
	stop.InsertPtr(new RString("sidan"));
	stop.InsertPtr(new RString("sidan"));
	stop.InsertPtr(new RString("siden"));
	stop.InsertPtr(new RString("sin"));
	stop.InsertPtr(new RString("sine"));
	stop.InsertPtr(new RString("sitt"));
	stop.InsertPtr(new RString("sjøl"));
	stop.InsertPtr(new RString("skal"));
	stop.InsertPtr(new RString("skulle"));
	stop.InsertPtr(new RString("so"));
	stop.InsertPtr(new RString("som"));
	stop.InsertPtr(new RString("som"));
	stop.InsertPtr(new RString("somme"));
	stop.InsertPtr(new RString("somt"));
	stop.InsertPtr(new RString("til"));
	stop.InsertPtr(new RString("uden"));
	stop.InsertPtr(new RString("um"));
	stop.InsertPtr(new RString("upp"));
	stop.InsertPtr(new RString("ut"));
	stop.InsertPtr(new RString("var"));
	stop.InsertPtr(new RString("var"));
	stop.InsertPtr(new RString("var"));
	stop.InsertPtr(new RString("vart"));
	stop.InsertPtr(new RString("varte"));
	stop.InsertPtr(new RString("vår"));
	stop.InsertPtr(new RString("være"));
	stop.InsertPtr(new RString("være"));
	stop.InsertPtr(new RString("ved"));
	stop.InsertPtr(new RString("vere"));
	stop.InsertPtr(new RString("verte"));
	stop.InsertPtr(new RString("vi"));
	stop.InsertPtr(new RString("vil"));
	stop.InsertPtr(new RString("ville"));
	stop.InsertPtr(new RString("vore"));
	stop.InsertPtr(new RString("vors"));
	stop.InsertPtr(new RString("vort"));
}


//-----------------------------------------------------------------------------
RString GLangNO::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangNO::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangNO::~GLangNO(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("no","Norwegian",GLangNO)
