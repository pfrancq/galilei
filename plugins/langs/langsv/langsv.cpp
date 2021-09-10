/*

	GALILEI Research Project

	LangSV.cpp

	Swedish Language - Implementation.

	Copyright 2001-2011 by the Snowball Project.
	Copyright 2001-2014 by Pascal Francq.
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
#include <langsv.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangSV
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangSV::GLangSV(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Swedish","sv"), Stemmer(sb_stemmer_new("swedish",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
	if(!Stemmer)
	    throw GException("GLangSV : Swedish is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangSV::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("alla"));
	stop.InsertPtr(new RString("allt"));
	stop.InsertPtr(new RString("att"));
	stop.InsertPtr(new RString("av"));
	stop.InsertPtr(new RString("än"));
	stop.InsertPtr(new RString("är"));
	stop.InsertPtr(new RString("åt"));
	stop.InsertPtr(new RString("över"));
	stop.InsertPtr(new RString("blev"));
	stop.InsertPtr(new RString("bli"));
	stop.InsertPtr(new RString("blir"));
	stop.InsertPtr(new RString("blivit"));
	stop.InsertPtr(new RString("där"));
	stop.InsertPtr(new RString("då"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("dem"));
	stop.InsertPtr(new RString("den"));
	stop.InsertPtr(new RString("denna"));
	stop.InsertPtr(new RString("deras"));
	stop.InsertPtr(new RString("dess"));
	stop.InsertPtr(new RString("dessa"));
	stop.InsertPtr(new RString("det"));
	stop.InsertPtr(new RString("detta"));
	stop.InsertPtr(new RString("dig"));
	stop.InsertPtr(new RString("din"));
	stop.InsertPtr(new RString("dina"));
	stop.InsertPtr(new RString("ditt"));
	stop.InsertPtr(new RString("du"));
	stop.InsertPtr(new RString("efter"));
	stop.InsertPtr(new RString("ej"));
	stop.InsertPtr(new RString("eller"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("era"));
	stop.InsertPtr(new RString("ert"));
	stop.InsertPtr(new RString("ett"));
	stop.InsertPtr(new RString("för"));
	stop.InsertPtr(new RString("från"));
	stop.InsertPtr(new RString("ha"));
	stop.InsertPtr(new RString("hade"));
	stop.InsertPtr(new RString("han"));
	stop.InsertPtr(new RString("hans"));
	stop.InsertPtr(new RString("har"));
	stop.InsertPtr(new RString("här"));
	stop.InsertPtr(new RString("henne"));
	stop.InsertPtr(new RString("hennes"));
	stop.InsertPtr(new RString("hon"));
	stop.InsertPtr(new RString("honom"));
	stop.InsertPtr(new RString("hur"));
	stop.InsertPtr(new RString("i"));
	stop.InsertPtr(new RString("icke"));
	stop.InsertPtr(new RString("ingen"));
	stop.InsertPtr(new RString("inom"));
	stop.InsertPtr(new RString("inte"));
	stop.InsertPtr(new RString("jag"));
	stop.InsertPtr(new RString("ju"));
	stop.InsertPtr(new RString("kan"));
	stop.InsertPtr(new RString("kunde"));
	stop.InsertPtr(new RString("man"));
	stop.InsertPtr(new RString("med"));
	stop.InsertPtr(new RString("mellan"));
	stop.InsertPtr(new RString("men"));
	stop.InsertPtr(new RString("mig"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("mina"));
	stop.InsertPtr(new RString("mitt"));
	stop.InsertPtr(new RString("mot"));
	stop.InsertPtr(new RString("mycket"));
	stop.InsertPtr(new RString("när"));
	stop.InsertPtr(new RString("någon"));
	stop.InsertPtr(new RString("något"));
	stop.InsertPtr(new RString("några"));
	stop.InsertPtr(new RString("ni"));
	stop.InsertPtr(new RString("nu"));
	stop.InsertPtr(new RString("och"));
	stop.InsertPtr(new RString("om"));
	stop.InsertPtr(new RString("oss"));
	stop.InsertPtr(new RString("på"));
	stop.InsertPtr(new RString("samma"));
	stop.InsertPtr(new RString("så"));
	stop.InsertPtr(new RString("sådan"));
	stop.InsertPtr(new RString("sådana"));
	stop.InsertPtr(new RString("sådant"));
	stop.InsertPtr(new RString("sedan"));
	stop.InsertPtr(new RString("sig"));
	stop.InsertPtr(new RString("sin"));
	stop.InsertPtr(new RString("sina"));
	stop.InsertPtr(new RString("sitta"));
	stop.InsertPtr(new RString("själv"));
	stop.InsertPtr(new RString("skulle"));
	stop.InsertPtr(new RString("som"));
	stop.InsertPtr(new RString("till"));
	stop.InsertPtr(new RString("under"));
	stop.InsertPtr(new RString("upp"));
	stop.InsertPtr(new RString("ut"));
	stop.InsertPtr(new RString("utan"));
	stop.InsertPtr(new RString("vad"));
	stop.InsertPtr(new RString("var"));
	stop.InsertPtr(new RString("vara"));
	stop.InsertPtr(new RString("varför"));
	stop.InsertPtr(new RString("varit"));
	stop.InsertPtr(new RString("varje"));
	stop.InsertPtr(new RString("vars"));
	stop.InsertPtr(new RString("vart"));
	stop.InsertPtr(new RString("vår"));
	stop.InsertPtr(new RString("våra"));
	stop.InsertPtr(new RString("vårt"));
	stop.InsertPtr(new RString("vem"));
	stop.InsertPtr(new RString("vi"));
	stop.InsertPtr(new RString("vid"));
	stop.InsertPtr(new RString("vilka"));
	stop.InsertPtr(new RString("vilken"));
	stop.InsertPtr(new RString("vilket"));
}


//-----------------------------------------------------------------------------
RString GLangSV::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd,false));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangSV::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangSV::~GLangSV(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("sv","Swedish",GLangSV)
