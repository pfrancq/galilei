/*

	GALILEI Research Project

	LangDK.cpp

	Danish Language - Implementation.

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
#include <langdk.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangDK
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangDK::GLangDK(GPlugInFactory* fac)
	: GLang(fac,"Danish","dk"), Stemmer(sb_stemmer_new("danish",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangDK : Danish is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangDK::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("ad"));
	stop.InsertPtr(new RString("af"));
	stop.InsertPtr(new RString("alle"));
	stop.InsertPtr(new RString("alt"));
	stop.InsertPtr(new RString("anden"));
	stop.InsertPtr(new RString("at"));
	stop.InsertPtr(new RString("blev"));
	stop.InsertPtr(new RString("blive"));
	stop.InsertPtr(new RString("bliver"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("dem"));
	stop.InsertPtr(new RString("den"));
	stop.InsertPtr(new RString("denne"));
	stop.InsertPtr(new RString("der"));
	stop.InsertPtr(new RString("deres"));
	stop.InsertPtr(new RString("det"));
	stop.InsertPtr(new RString("dette"));
	stop.InsertPtr(new RString("dig"));
	stop.InsertPtr(new RString("din"));
	stop.InsertPtr(new RString("disse"));
	stop.InsertPtr(new RString("dog"));
	stop.InsertPtr(new RString("du"));
	stop.InsertPtr(new RString("efter"));
	stop.InsertPtr(new RString("eller"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("end"));
	stop.InsertPtr(new RString("er"));
	stop.InsertPtr(new RString("et"));
	stop.InsertPtr(new RString("for"));
	stop.InsertPtr(new RString("fra"));
	stop.InsertPtr(new RString("ham"));
	stop.InsertPtr(new RString("han"));
	stop.InsertPtr(new RString("hans"));
	stop.InsertPtr(new RString("har"));
	stop.InsertPtr(new RString("havde"));
	stop.InsertPtr(new RString("have"));
	stop.InsertPtr(new RString("hende"));
	stop.InsertPtr(new RString("hendes"));
	stop.InsertPtr(new RString("her"));
	stop.InsertPtr(new RString("hos"));
	stop.InsertPtr(new RString("hun"));
	stop.InsertPtr(new RString("hvad"));
	stop.InsertPtr(new RString("hvis"));
	stop.InsertPtr(new RString("hvor"));
	stop.InsertPtr(new RString("i"));
	stop.InsertPtr(new RString("ikke"));
	stop.InsertPtr(new RString("ind"));
	stop.InsertPtr(new RString("jeg"));
	stop.InsertPtr(new RString("jer"));
	stop.InsertPtr(new RString("jo"));
	stop.InsertPtr(new RString("kunne"));
	stop.InsertPtr(new RString("man"));
	stop.InsertPtr(new RString("mange"));
	stop.InsertPtr(new RString("med"));
	stop.InsertPtr(new RString("meget"));
	stop.InsertPtr(new RString("men"));
	stop.InsertPtr(new RString("mig"));
	stop.InsertPtr(new RString("min"));
	stop.InsertPtr(new RString("mine"));
	stop.InsertPtr(new RString("mit"));
	stop.InsertPtr(new RString("mod"));
	stop.InsertPtr(new RString("når"));
	stop.InsertPtr(new RString("ned"));
	stop.InsertPtr(new RString("noget"));
	stop.InsertPtr(new RString("nogle"));
	stop.InsertPtr(new RString("nu"));
	stop.InsertPtr(new RString("og"));
	stop.InsertPtr(new RString("også"));
	stop.InsertPtr(new RString("om"));
	stop.InsertPtr(new RString("op"));
	stop.InsertPtr(new RString("os"));
	stop.InsertPtr(new RString("over"));
	stop.InsertPtr(new RString("på"));
	stop.InsertPtr(new RString("selv"));
	stop.InsertPtr(new RString("sig"));
	stop.InsertPtr(new RString("sin"));
	stop.InsertPtr(new RString("sine"));
	stop.InsertPtr(new RString("sit"));
	stop.InsertPtr(new RString("skal"));
	stop.InsertPtr(new RString("skulle"));
	stop.InsertPtr(new RString("som"));
	stop.InsertPtr(new RString("thi"));
	stop.InsertPtr(new RString("til"));
	stop.InsertPtr(new RString("ud"));
	stop.InsertPtr(new RString("under"));
	stop.InsertPtr(new RString("var"));
	stop.InsertPtr(new RString("være"));
	stop.InsertPtr(new RString("været"));
	stop.InsertPtr(new RString("vi"));
	stop.InsertPtr(new RString("vil"));
	stop.InsertPtr(new RString("ville"));
	stop.InsertPtr(new RString("vor"));
}


//-----------------------------------------------------------------------------
RString GLangDK::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangDK::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GLangDK::~GLangDK(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("dk","Danish",GLangDK)
