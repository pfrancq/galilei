/*

	GALILEI Research Project

	LangPT.cpp

	Portuguese Language - Implementation.

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
#include <langpt.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangPT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangPT::GLangPT(GPlugInFactory* fac)
	: GLang(fac,"Portuguese","pt"), Stemmer(sb_stemmer_new("portuguese",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangPT : Portuguese is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangPT::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("a"));
	stop.InsertPtr(new RString("ao"));
	stop.InsertPtr(new RString("aos"));
	stop.InsertPtr(new RString("aquela"));
	stop.InsertPtr(new RString("aquelas"));
	stop.InsertPtr(new RString("aquele"));
	stop.InsertPtr(new RString("aqueles"));
	stop.InsertPtr(new RString("aquilo"));
	stop.InsertPtr(new RString("as"));
	stop.InsertPtr(new RString("até"));
	stop.InsertPtr(new RString("à"));
	stop.InsertPtr(new RString("às"));
	stop.InsertPtr(new RString("éramos"));
	stop.InsertPtr(new RString("com"));
	stop.InsertPtr(new RString("como"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("das"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("dela"));
	stop.InsertPtr(new RString("delas"));
	stop.InsertPtr(new RString("dele"));
	stop.InsertPtr(new RString("deles"));
	stop.InsertPtr(new RString("depois"));
	stop.InsertPtr(new RString("do"));
	stop.InsertPtr(new RString("dos"));
	stop.InsertPtr(new RString("e"));
	stop.InsertPtr(new RString("ela"));
	stop.InsertPtr(new RString("elas"));
	stop.InsertPtr(new RString("ele"));
	stop.InsertPtr(new RString("eles"));
	stop.InsertPtr(new RString("em"));
	stop.InsertPtr(new RString("entre"));
	stop.InsertPtr(new RString("era"));
	stop.InsertPtr(new RString("eram"));
	stop.InsertPtr(new RString("essa"));
	stop.InsertPtr(new RString("essas"));
	stop.InsertPtr(new RString("esse"));
	stop.InsertPtr(new RString("esses"));
	stop.InsertPtr(new RString("esta"));
	stop.InsertPtr(new RString("estamos"));
	stop.InsertPtr(new RString("estas"));
	stop.InsertPtr(new RString("estava"));
	stop.InsertPtr(new RString("estavam"));
	stop.InsertPtr(new RString("está"));
	stop.InsertPtr(new RString("estávamos"));
	stop.InsertPtr(new RString("estão"));
	stop.InsertPtr(new RString("este"));
	stop.InsertPtr(new RString("esteja"));
	stop.InsertPtr(new RString("estejam"));
	stop.InsertPtr(new RString("estejamos"));
	stop.InsertPtr(new RString("estes"));
	stop.InsertPtr(new RString("esteve"));
	stop.InsertPtr(new RString("estivéramos"));
	stop.InsertPtr(new RString("estivéssemos"));
	stop.InsertPtr(new RString("estive"));
	stop.InsertPtr(new RString("estivemos"));
	stop.InsertPtr(new RString("estiver"));
	stop.InsertPtr(new RString("estivera"));
	stop.InsertPtr(new RString("estiveram"));
	stop.InsertPtr(new RString("estiverem"));
	stop.InsertPtr(new RString("estivermos"));
	stop.InsertPtr(new RString("estivesse"));
	stop.InsertPtr(new RString("estivessem"));
	stop.InsertPtr(new RString("estou"));
	stop.InsertPtr(new RString("eu"));
	stop.InsertPtr(new RString("fôramos"));
	stop.InsertPtr(new RString("fôssemos"));
	stop.InsertPtr(new RString("foi"));
	stop.InsertPtr(new RString("fomos"));
	stop.InsertPtr(new RString("for"));
	stop.InsertPtr(new RString("fora"));
	stop.InsertPtr(new RString("foram"));
	stop.InsertPtr(new RString("forem"));
	stop.InsertPtr(new RString("formos"));
	stop.InsertPtr(new RString("fosse"));
	stop.InsertPtr(new RString("fossem"));
	stop.InsertPtr(new RString("fui"));
	stop.InsertPtr(new RString("haja"));
	stop.InsertPtr(new RString("hajam"));
	stop.InsertPtr(new RString("hajamos"));
	stop.InsertPtr(new RString("havemos"));
	stop.InsertPtr(new RString("há"));
	stop.InsertPtr(new RString("hão"));
	stop.InsertPtr(new RString("hei"));
	stop.InsertPtr(new RString("houvéramos"));
	stop.InsertPtr(new RString("houvéssemos"));
	stop.InsertPtr(new RString("houve"));
	stop.InsertPtr(new RString("houvemos"));
	stop.InsertPtr(new RString("houver"));
	stop.InsertPtr(new RString("houvera"));
	stop.InsertPtr(new RString("houveram"));
	stop.InsertPtr(new RString("houverá"));
	stop.InsertPtr(new RString("houverão"));
	stop.InsertPtr(new RString("houveríamos"));
	stop.InsertPtr(new RString("houverei"));
	stop.InsertPtr(new RString("houverem"));
	stop.InsertPtr(new RString("houveremos"));
	stop.InsertPtr(new RString("houveria"));
	stop.InsertPtr(new RString("houveriam"));
	stop.InsertPtr(new RString("houvermos"));
	stop.InsertPtr(new RString("houvesse"));
	stop.InsertPtr(new RString("houvessem"));
	stop.InsertPtr(new RString("isso"));
	stop.InsertPtr(new RString("isto"));
	stop.InsertPtr(new RString("já"));
	stop.InsertPtr(new RString("lhe"));
	stop.InsertPtr(new RString("lhes"));
	stop.InsertPtr(new RString("mais"));
	stop.InsertPtr(new RString("mas"));
	stop.InsertPtr(new RString("me"));
	stop.InsertPtr(new RString("mesmo"));
	stop.InsertPtr(new RString("meu"));
	stop.InsertPtr(new RString("meus"));
	stop.InsertPtr(new RString("minha"));
	stop.InsertPtr(new RString("minhas"));
	stop.InsertPtr(new RString("muito"));
	stop.InsertPtr(new RString("na"));
	stop.InsertPtr(new RString("nas"));
	stop.InsertPtr(new RString("não"));
	stop.InsertPtr(new RString("nós"));
	stop.InsertPtr(new RString("nem"));
	stop.InsertPtr(new RString("no"));
	stop.InsertPtr(new RString("nos"));
	stop.InsertPtr(new RString("nossa"));
	stop.InsertPtr(new RString("nossas"));
	stop.InsertPtr(new RString("nosso"));
	stop.InsertPtr(new RString("nossos"));
	stop.InsertPtr(new RString("num"));
	stop.InsertPtr(new RString("numa"));
	stop.InsertPtr(new RString("o"));
	stop.InsertPtr(new RString("os"));
	stop.InsertPtr(new RString("ou"));
	stop.InsertPtr(new RString("para"));
	stop.InsertPtr(new RString("pela"));
	stop.InsertPtr(new RString("pelas"));
	stop.InsertPtr(new RString("pelo"));
	stop.InsertPtr(new RString("pelos"));
	stop.InsertPtr(new RString("por"));
	stop.InsertPtr(new RString("qual"));
	stop.InsertPtr(new RString("quando"));
	stop.InsertPtr(new RString("que"));
	stop.InsertPtr(new RString("quem"));
	stop.InsertPtr(new RString("são"));
	stop.InsertPtr(new RString("só"));
	stop.InsertPtr(new RString("se"));
	stop.InsertPtr(new RString("seja"));
	stop.InsertPtr(new RString("sejam"));
	stop.InsertPtr(new RString("sejamos"));
	stop.InsertPtr(new RString("sem"));
	stop.InsertPtr(new RString("será"));
	stop.InsertPtr(new RString("serão"));
	stop.InsertPtr(new RString("seríamos"));
	stop.InsertPtr(new RString("serei"));
	stop.InsertPtr(new RString("seremos"));
	stop.InsertPtr(new RString("seria"));
	stop.InsertPtr(new RString("seriam"));
	stop.InsertPtr(new RString("seu"));
	stop.InsertPtr(new RString("seus"));
	stop.InsertPtr(new RString("somos"));
	stop.InsertPtr(new RString("sou"));
	stop.InsertPtr(new RString("sua"));
	stop.InsertPtr(new RString("suas"));
	stop.InsertPtr(new RString("também"));
	stop.InsertPtr(new RString("tém"));
	stop.InsertPtr(new RString("tínhamos"));
	stop.InsertPtr(new RString("te"));
	stop.InsertPtr(new RString("tem"));
	stop.InsertPtr(new RString("temos"));
	stop.InsertPtr(new RString("tenha"));
	stop.InsertPtr(new RString("tenham"));
	stop.InsertPtr(new RString("tenhamos"));
	stop.InsertPtr(new RString("tenho"));
	stop.InsertPtr(new RString("terá"));
	stop.InsertPtr(new RString("terão"));
	stop.InsertPtr(new RString("teríamos"));
	stop.InsertPtr(new RString("terei"));
	stop.InsertPtr(new RString("teremos"));
	stop.InsertPtr(new RString("teria"));
	stop.InsertPtr(new RString("teu"));
	stop.InsertPtr(new RString("teus"));
	stop.InsertPtr(new RString("teve"));
	stop.InsertPtr(new RString("tinha"));
	stop.InsertPtr(new RString("tinham"));
	stop.InsertPtr(new RString("tivéramos"));
	stop.InsertPtr(new RString("tivéssemos"));
	stop.InsertPtr(new RString("tive"));
	stop.InsertPtr(new RString("tivemos"));
	stop.InsertPtr(new RString("tiver"));
	stop.InsertPtr(new RString("tivera"));
	stop.InsertPtr(new RString("tiveram"));
	stop.InsertPtr(new RString("tiverem"));
	stop.InsertPtr(new RString("tivermos"));
	stop.InsertPtr(new RString("tivesse"));
	stop.InsertPtr(new RString("tivessem"));
	stop.InsertPtr(new RString("tu"));
	stop.InsertPtr(new RString("tua"));
	stop.InsertPtr(new RString("tuas"));
	stop.InsertPtr(new RString("um"));
	stop.InsertPtr(new RString("uma"));
	stop.InsertPtr(new RString("você"));
	stop.InsertPtr(new RString("vocês"));
	stop.InsertPtr(new RString("vos"));
}


//-----------------------------------------------------------------------------
RString GLangPT::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangPT::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangPT::~GLangPT(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("pt","Portuguese",GLangPT)
