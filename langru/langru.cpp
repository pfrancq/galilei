/*

	GALILEI Research Project

	LangRU.cpp

	Russian Language - Implementation.

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
#include <langru.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GLangSV
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLangRU::GLangRU(GSession* session,GPlugInFactory* fac)
	: GLang(session,fac,"Russian","ru"), Stemmer(sb_stemmer_new("russian",0)), StdCodec(RTextEncoding::GetTextEncoding("utf-8"))
{
    if(!Stemmer)
    	throw GException("GLangRU : Russian is not available for stemming");
}


//-----------------------------------------------------------------------------
void GLangRU::GetStopWords(RContainer<RString,true,false>& stop)
{
	stop.InsertPtr(new RString("и"));
	stop.InsertPtr(new RString("в"));
	stop.InsertPtr(new RString("во"));
	stop.InsertPtr(new RString("не"));
	stop.InsertPtr(new RString("что"));
	stop.InsertPtr(new RString("он"));
	stop.InsertPtr(new RString("на"));
	stop.InsertPtr(new RString("я"));
	stop.InsertPtr(new RString("с"));
	stop.InsertPtr(new RString("со"));
	stop.InsertPtr(new RString("как"));
	stop.InsertPtr(new RString("а"));
	stop.InsertPtr(new RString("то"));
	stop.InsertPtr(new RString("все"));
	stop.InsertPtr(new RString("она"));
	stop.InsertPtr(new RString("так"));
	stop.InsertPtr(new RString("его"));
	stop.InsertPtr(new RString("но"));
	stop.InsertPtr(new RString("да"));
	stop.InsertPtr(new RString("ты"));
	stop.InsertPtr(new RString("к"));
	stop.InsertPtr(new RString("у"));
	stop.InsertPtr(new RString("же"));
	stop.InsertPtr(new RString("вы"));
	stop.InsertPtr(new RString("за"));
	stop.InsertPtr(new RString("бы"));
	stop.InsertPtr(new RString("по"));
	stop.InsertPtr(new RString("только"));
	stop.InsertPtr(new RString("ее"));
	stop.InsertPtr(new RString("мне"));
	stop.InsertPtr(new RString("было"));
	stop.InsertPtr(new RString("вот"));
	stop.InsertPtr(new RString("от"));
	stop.InsertPtr(new RString("меня"));
	stop.InsertPtr(new RString("еще"));
	stop.InsertPtr(new RString("нет"));
	stop.InsertPtr(new RString("о"));
	stop.InsertPtr(new RString("из"));
	stop.InsertPtr(new RString("ему"));
	stop.InsertPtr(new RString("теперь"));
	stop.InsertPtr(new RString("когда"));
	stop.InsertPtr(new RString("даже"));
	stop.InsertPtr(new RString("ну"));
	stop.InsertPtr(new RString("вдруг"));
	stop.InsertPtr(new RString("ли"));
	stop.InsertPtr(new RString("если"));
	stop.InsertPtr(new RString("уже"));
	stop.InsertPtr(new RString("или"));
	stop.InsertPtr(new RString("ни"));
	stop.InsertPtr(new RString("быть"));
	stop.InsertPtr(new RString("был"));
	stop.InsertPtr(new RString("него"));
	stop.InsertPtr(new RString("до"));
	stop.InsertPtr(new RString("вас"));
	stop.InsertPtr(new RString("нибудь"));
	stop.InsertPtr(new RString("опять"));
	stop.InsertPtr(new RString("уж"));
	stop.InsertPtr(new RString("вам"));
	stop.InsertPtr(new RString("сказал"));
	stop.InsertPtr(new RString("ведь"));
	stop.InsertPtr(new RString("там"));
	stop.InsertPtr(new RString("потом"));
	stop.InsertPtr(new RString("себя"));
	stop.InsertPtr(new RString("ничего"));
	stop.InsertPtr(new RString("ей"));
	stop.InsertPtr(new RString("может"));
	stop.InsertPtr(new RString("они"));
	stop.InsertPtr(new RString("тут"));
	stop.InsertPtr(new RString("где"));
	stop.InsertPtr(new RString("есть"));
	stop.InsertPtr(new RString("надо"));
	stop.InsertPtr(new RString("ней"));
	stop.InsertPtr(new RString("для"));
	stop.InsertPtr(new RString("мы"));
	stop.InsertPtr(new RString("тебя"));
	stop.InsertPtr(new RString("их"));
	stop.InsertPtr(new RString("чем"));
	stop.InsertPtr(new RString("была"));
	stop.InsertPtr(new RString("сам"));
	stop.InsertPtr(new RString("чтоб"));
	stop.InsertPtr(new RString("безv"));
	stop.InsertPtr(new RString("будто"));
	stop.InsertPtr(new RString("человек"));
	stop.InsertPtr(new RString("чего"));
	stop.InsertPtr(new RString("раз"));
	stop.InsertPtr(new RString("тоже"));
	stop.InsertPtr(new RString("себе"));
	stop.InsertPtr(new RString("под"));
	stop.InsertPtr(new RString("жизнь"));
	stop.InsertPtr(new RString("будет"));
	stop.InsertPtr(new RString("ж"));
	stop.InsertPtr(new RString("тогда"));
	stop.InsertPtr(new RString("кто"));
	stop.InsertPtr(new RString("этот"));
	stop.InsertPtr(new RString("говорил"));
	stop.InsertPtr(new RString("того"));
	stop.InsertPtr(new RString("потому"));
	stop.InsertPtr(new RString("этого"));
	stop.InsertPtr(new RString("какой"));
	stop.InsertPtr(new RString("совсем"));
	stop.InsertPtr(new RString("ним"));
	stop.InsertPtr(new RString("здесь"));
	stop.InsertPtr(new RString("этом"));
	stop.InsertPtr(new RString("один"));
	stop.InsertPtr(new RString("почти"));
	stop.InsertPtr(new RString("мой"));
	stop.InsertPtr(new RString("тем"));
	stop.InsertPtr(new RString("чтобы"));
	stop.InsertPtr(new RString("нее"));
	stop.InsertPtr(new RString("кажется"));
	stop.InsertPtr(new RString("сейчас"));
	stop.InsertPtr(new RString("были"));
	stop.InsertPtr(new RString("куда"));
	stop.InsertPtr(new RString("зачем"));
	stop.InsertPtr(new RString("сказать"));
	stop.InsertPtr(new RString("всех"));
	stop.InsertPtr(new RString("никогда"));
	stop.InsertPtr(new RString("сегодня"));
	stop.InsertPtr(new RString("можно"));
	stop.InsertPtr(new RString("при"));
	stop.InsertPtr(new RString("наконец"));
	stop.InsertPtr(new RString("два"));
	stop.InsertPtr(new RString("об"));
	stop.InsertPtr(new RString("другой"));
	stop.InsertPtr(new RString("хоть"));
	stop.InsertPtr(new RString("после"));
	stop.InsertPtr(new RString("над"));
	stop.InsertPtr(new RString("больше"));
	stop.InsertPtr(new RString("тот"));
	stop.InsertPtr(new RString("через"));
	stop.InsertPtr(new RString("эти"));
	stop.InsertPtr(new RString("нас"));
	stop.InsertPtr(new RString("про"));
	stop.InsertPtr(new RString("всего"));
	stop.InsertPtr(new RString("них"));
	stop.InsertPtr(new RString("какая"));
	stop.InsertPtr(new RString("много"));
	stop.InsertPtr(new RString("разве"));
	stop.InsertPtr(new RString("сказала"));
	stop.InsertPtr(new RString("три"));
	stop.InsertPtr(new RString("эту"));
	stop.InsertPtr(new RString("моя"));
	stop.InsertPtr(new RString("впрочем"));
	stop.InsertPtr(new RString("хорошо"));
	stop.InsertPtr(new RString("свою"));
	stop.InsertPtr(new RString("этой"));
	stop.InsertPtr(new RString("перед"));
	stop.InsertPtr(new RString("иногда"));
	stop.InsertPtr(new RString("лучше"));
	stop.InsertPtr(new RString("чуть"));
	stop.InsertPtr(new RString("том"));
	stop.InsertPtr(new RString("нельзя"));
	stop.InsertPtr(new RString("такой"));
	stop.InsertPtr(new RString("им"));
	stop.InsertPtr(new RString("более"));
	stop.InsertPtr(new RString("всегда"));
	stop.InsertPtr(new RString("конечно"));
	stop.InsertPtr(new RString("всю"));
	stop.InsertPtr(new RString("между"));
}


//-----------------------------------------------------------------------------
RString GLangRU::GetStemming(const RString& kwd)
{
	RCString Str(StdCodec->FromUnicode(kwd));
	const sb_symbol * stemmed = sb_stemmer_stem(Stemmer,(const sb_symbol*)Str(),Str.GetLen());
	return RString((char *)stemmed);
}


//------------------------------------------------------------------------------
void GLangRU::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GLangRU::~GLangRU(void)
{
	if(Stemmer)
		sb_stemmer_delete(Stemmer);
}


//------------------------------------------------------------------------------
CREATE_LANG_FACTORY("ru","Russian",GLangRU)
