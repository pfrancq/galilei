/*

	GALILEI Research Project

	LangRO.cpp

	Romanian Language - Implementation.

	Copyright 2001-2009 by the Snowball Project.
	Copyright 2008-2009 by Pascal Francq.

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
#include <langro.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangSV
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangRO::GLangRO(GPluginFactory* fac)
	: GLang(fac,"Romanian","ro"), Stemmer(sb_stemmer_new("romanian",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangRO : Romanian is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangRO::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("aceasta"));
	stop.InsertPtr(new RString("acel"));
	stop.InsertPtr(new RString("acela"));
	stop.InsertPtr(new RString("acesta"));
	stop.InsertPtr(new RString("acolo"));
	stop.InsertPtr(new RString("acum"));
	stop.InsertPtr(new RString("aceştia"));
	stop.InsertPtr(new RString("adevarat"));
	stop.InsertPtr(new RString("afară"));
	stop.InsertPtr(new RString("aici"));
	stop.InsertPtr(new RString("alta"));
	stop.InsertPtr(new RString("altfel"));
	stop.InsertPtr(new RString("altul"));
	stop.InsertPtr(new RString("aproape"));
	stop.InsertPtr(new RString("apropo"));
	stop.InsertPtr(new RString("are"));
	stop.InsertPtr(new RString("asemenea"));
	stop.InsertPtr(new RString("atunci"));
	stop.InsertPtr(new RString("având"));
	stop.InsertPtr(new RString("batrân"));
	stop.InsertPtr(new RString("bun"));
	stop.InsertPtr(new RString("ca"));
	stop.InsertPtr(new RString("cale"));
	stop.InsertPtr(new RString("când"));
	stop.InsertPtr(new RString("cândva"));
	stop.InsertPtr(new RString("care"));
	stop.InsertPtr(new RString("câţiva"));
	stop.InsertPtr(new RString("ce"));
	stop.InsertPtr(new RString("ceva"));
	stop.InsertPtr(new RString("cine"));
	stop.InsertPtr(new RString("citat"));
	stop.InsertPtr(new RString("cu"));
	stop.InsertPtr(new RString("curând"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("dacă"));
	stop.InsertPtr(new RString("deci"));
	stop.InsertPtr(new RString("dar"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("decât"));
	stop.InsertPtr(new RString("departe"));
	stop.InsertPtr(new RString("despre"));
	stop.InsertPtr(new RString("doar"));
	stop.InsertPtr(new RString("dupa"));
	stop.InsertPtr(new RString("ea"));
	stop.InsertPtr(new RString("este"));
	stop.InsertPtr(new RString("ei"));
	stop.InsertPtr(new RString("etc"));
	stop.InsertPtr(new RString("eu"));
	stop.InsertPtr(new RString("fă"));
	stop.InsertPtr(new RString("face"));
	stop.InsertPtr(new RString("făcut"));
	stop.InsertPtr(new RString("fărăv"));
	stop.InsertPtr(new RString("fi"));
	stop.InsertPtr(new RString("fiecare"));
	stop.InsertPtr(new RString("fiind"));
	stop.InsertPtr(new RString("foarte"));
	stop.InsertPtr(new RString("fost"));
	stop.InsertPtr(new RString("frecvent"));
	stop.InsertPtr(new RString("găseşte"));
	stop.InsertPtr(new RString("ia"));
	stop.InsertPtr(new RString("iei"));
	stop.InsertPtr(new RString("în"));
	stop.InsertPtr(new RString("înainte"));
	stop.InsertPtr(new RString("înca"));
	stop.InsertPtr(new RString("între"));
	stop.InsertPtr(new RString("întreabă"));
	stop.InsertPtr(new RString("întrebare"));
	stop.InsertPtr(new RString("întrebări"));
	stop.InsertPtr(new RString("întrebat"));
	stop.InsertPtr(new RString("jos"));
	stop.InsertPtr(new RString("la"));
	stop.InsertPtr(new RString("larg"));
	stop.InsertPtr(new RString("lol"));
	stop.InsertPtr(new RString("lor"));
	stop.InsertPtr(new RString("lui"));
	stop.InsertPtr(new RString("mare"));
	stop.InsertPtr(new RString("mers"));
	stop.InsertPtr(new RString("mic"));
	stop.InsertPtr(new RString("mult"));
	stop.InsertPtr(new RString("multe"));
	stop.InsertPtr(new RString("nevoie"));
	stop.InsertPtr(new RString("niciodată"));
	stop.InsertPtr(new RString("nimic"));
	stop.InsertPtr(new RString("noi"));
	stop.InsertPtr(new RString("nou"));
	stop.InsertPtr(new RString("noutăţi"));
	stop.InsertPtr(new RString("nu"));
	stop.InsertPtr(new RString("not"));
	stop.InsertPtr(new RString("odată"));
	stop.InsertPtr(new RString("oops"));
	stop.InsertPtr(new RString("oprit"));
	stop.InsertPtr(new RString("oricare"));
	stop.InsertPtr(new RString("oricine"));
	stop.InsertPtr(new RString("oricui"));
	stop.InsertPtr(new RString("oriunde"));
	stop.InsertPtr(new RString("pagina"));
	stop.InsertPtr(new RString("pentru"));
	stop.InsertPtr(new RString("poate"));
	stop.InsertPtr(new RString("pornit"));
	stop.InsertPtr(new RString("prin"));
	stop.InsertPtr(new RString("pune"));
	stop.InsertPtr(new RString("răspuns"));
	stop.InsertPtr(new RString("rau"));
	stop.InsertPtr(new RString("recent"));
	stop.InsertPtr(new RString("sau"));
	stop.InsertPtr(new RString("şi"));
	stop.InsertPtr(new RString("spre"));
	stop.InsertPtr(new RString("spune"));
	stop.InsertPtr(new RString("spus"));
	stop.InsertPtr(new RString("sub"));
	stop.InsertPtr(new RString("sunt"));
	stop.InsertPtr(new RString("sus"));
	stop.InsertPtr(new RString("şti"));
	stop.InsertPtr(new RString("ştie"));
	stop.InsertPtr(new RString("timp"));
	stop.InsertPtr(new RString("tot"));
	stop.InsertPtr(new RString("toţi"));
	stop.InsertPtr(new RString("trebui"));
	stop.InsertPtr(new RString("trebuia"));
	stop.InsertPtr(new RString("tu"));
	stop.InsertPtr(new RString("un"));
	stop.InsertPtr(new RString("una"));
	stop.InsertPtr(new RString("unde"));
	stop.InsertPtr(new RString("undeva"));
	stop.InsertPtr(new RString("utilizatori"));
	stop.InsertPtr(new RString("văzut"));
	stop.InsertPtr(new RString("vede"));
	stop.InsertPtr(new RString("versiune"));
	stop.InsertPtr(new RString("via"));
	stop.InsertPtr(new RString("vrea"));
	stop.InsertPtr(new RString("vreau"));
	stop.InsertPtr(new RString("zi"));
	stop.InsertPtr(new RString("zile"));
	stop.InsertPtr(new RString("ziua"));
}


//-----------------------------------------------------------------------------
RString GLangRO::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangRO::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangRO::~GLangRO(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("ro","Romanian",GLangRO)
