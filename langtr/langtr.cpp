/*

	GALILEI Research Project

	LangTR.cpp

	Turkish Language - Implementation.

	Copyright 2001-2010 by the Snowball Project.
	Copyright 2008-2010 by Pascal Francq.

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
#include <langtr.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangSV
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangTR::GLangTR(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Turkish","tr"), Stemmer(sb_stemmer_new("turkish",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangTR : Turkish is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangTR::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("acaba"));
	stop.InsertPtr(new RString("altmýþ"));
	stop.InsertPtr(new RString("altý"));
	stop.InsertPtr(new RString("ama"));
	stop.InsertPtr(new RString("bana"));
	stop.InsertPtr(new RString("bazý"));
	stop.InsertPtr(new RString("belki"));
	stop.InsertPtr(new RString("ben"));
	stop.InsertPtr(new RString("benden"));
	stop.InsertPtr(new RString("beni"));
	stop.InsertPtr(new RString("benim"));
	stop.InsertPtr(new RString("beþ"));
	stop.InsertPtr(new RString("bin"));
	stop.InsertPtr(new RString("bir"));
	stop.InsertPtr(new RString("biri"));
	stop.InsertPtr(new RString("birkaç"));
	stop.InsertPtr(new RString("birkez"));
	stop.InsertPtr(new RString("birþey"));
	stop.InsertPtr(new RString("birþeyi"));
	stop.InsertPtr(new RString("biz"));
	stop.InsertPtr(new RString("bizden"));
	stop.InsertPtr(new RString("bizi"));
	stop.InsertPtr(new RString("bizim"));
	stop.InsertPtr(new RString("bu"));
	stop.InsertPtr(new RString("buna"));
	stop.InsertPtr(new RString("bunda"));
	stop.InsertPtr(new RString("bundan"));
	stop.InsertPtr(new RString("bunu"));
	stop.InsertPtr(new RString("bunun"));
	stop.InsertPtr(new RString("da"));
	stop.InsertPtr(new RString("daha"));
	stop.InsertPtr(new RString("dahi"));
	stop.InsertPtr(new RString("de"));
	stop.InsertPtr(new RString("defa"));
	stop.InsertPtr(new RString("diye"));
	stop.InsertPtr(new RString("doksan"));
	stop.InsertPtr(new RString("dokuz"));
	stop.InsertPtr(new RString("dört"));
	stop.InsertPtr(new RString("elli"));
	stop.InsertPtr(new RString("en"));
	stop.InsertPtr(new RString("gibi"));
	stop.InsertPtr(new RString("hem"));
	stop.InsertPtr(new RString("hep"));
	stop.InsertPtr(new RString("hepsi"));
	stop.InsertPtr(new RString("her"));
	stop.InsertPtr(new RString("hiç"));
	stop.InsertPtr(new RString("iki"));
	stop.InsertPtr(new RString("ile"));
	stop.InsertPtr(new RString("insermi"));
	stop.InsertPtr(new RString("ise"));
	stop.InsertPtr(new RString("için"));
	stop.InsertPtr(new RString("katrilyon"));
	stop.InsertPtr(new RString("kez"));
	stop.InsertPtr(new RString("ki"));
	stop.InsertPtr(new RString("kim"));
	stop.InsertPtr(new RString("kimden"));
	stop.InsertPtr(new RString("kime"));
	stop.InsertPtr(new RString("kimi"));
	stop.InsertPtr(new RString("kýrk"));
	stop.InsertPtr(new RString("milyar"));
	stop.InsertPtr(new RString("milyon"));
	stop.InsertPtr(new RString("mu"));
	stop.InsertPtr(new RString("mü"));
	stop.InsertPtr(new RString("mý"));
	stop.InsertPtr(new RString("nasýl"));
	stop.InsertPtr(new RString("ne"));
	stop.InsertPtr(new RString("neden"));
	stop.InsertPtr(new RString("nerde"));
	stop.InsertPtr(new RString("nerede"));
	stop.InsertPtr(new RString("nereye"));
	stop.InsertPtr(new RString("niye"));
	stop.InsertPtr(new RString("niçin"));
	stop.InsertPtr(new RString("on"));
	stop.InsertPtr(new RString("ona"));
	stop.InsertPtr(new RString("ondan"));
	stop.InsertPtr(new RString("onlar"));
	stop.InsertPtr(new RString("onlardan"));
	stop.InsertPtr(new RString("onlari"));
	stop.InsertPtr(new RString("onlarýn"));
	stop.InsertPtr(new RString("onu"));
	stop.InsertPtr(new RString("otuz"));
	stop.InsertPtr(new RString("sanki"));
	stop.InsertPtr(new RString("sekiz"));
	stop.InsertPtr(new RString("seksen"));
	stop.InsertPtr(new RString("sen"));
	stop.InsertPtr(new RString("senden"));
	stop.InsertPtr(new RString("seni"));
	stop.InsertPtr(new RString("senin"));
	stop.InsertPtr(new RString("siz"));
	stop.InsertPtr(new RString("sizden"));
	stop.InsertPtr(new RString("sizi"));
	stop.InsertPtr(new RString("sizin"));
	stop.InsertPtr(new RString("trilyon"));
	stop.InsertPtr(new RString("tüm"));
	stop.InsertPtr(new RString("ve"));
	stop.InsertPtr(new RString("veya"));
	stop.InsertPtr(new RString("ya"));
	stop.InsertPtr(new RString("yani"));
	stop.InsertPtr(new RString("yedi"));
	stop.InsertPtr(new RString("yetmiþ"));
	stop.InsertPtr(new RString("yirmi"));
	stop.InsertPtr(new RString("yüz"));
	stop.InsertPtr(new RString("çok"));
	stop.InsertPtr(new RString("çünkü"));
	stop.InsertPtr(new RString("üç"));
	stop.InsertPtr(new RString("þey"));
	stop.InsertPtr(new RString("þeyden"));
	stop.InsertPtr(new RString("þeyi"));
	stop.InsertPtr(new RString("þeyler"));
	stop.InsertPtr(new RString("þu"));
	stop.InsertPtr(new RString("þuna"));
	stop.InsertPtr(new RString("þunda"));
	stop.InsertPtr(new RString("þundan"));
	stop.InsertPtr(new RString("þunu"));
}


//-----------------------------------------------------------------------------
RString GLangTR::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangTR::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangTR::~GLangTR(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("tr","Turkish",GLangTR)
