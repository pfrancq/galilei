/*

	GALILEI Research Project

	GIWordList.cpp

	Represents a list of words' references - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
// include files for GALILEI
#include <infos/giwordlist.h>
#include <infos/giword.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GIWordList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordList::GIWordList(void) throw(bad_alloc)
	: RContainer<GIWord,unsigned,true,true>(30,10), Id(cNoRef)
{
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordList::ClassName(void) const
{
	return("GWordList");
}


//-----------------------------------------------------------------------------
const GInfo::GInfoType GALILEI::GIWordList::InfoType(void) const
{
	return(infoWordList);
}
