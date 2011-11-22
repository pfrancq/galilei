/*

	GALILEI Research Project

	GTokenizer.cpp

	Text Tokenizer - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gtokenizer.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GTokenizer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTokenizer::GTokenizer(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac)
{
}


//------------------------------------------------------------------------------
void GTokenizer::AddChar(const RChar& car)
{
	Buffer+=car;
	CurPos++;
}


//------------------------------------------------------------------------------
RString GTokenizer::Extract(size_t begin,size_t end)
{
	return(Buffer.Mid(begin,end-begin));
}


//------------------------------------------------------------------------------
void GTokenizer::Start(void)
{
	Buffer.Clear();
	CurPos=0;
}
