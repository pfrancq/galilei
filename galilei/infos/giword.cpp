/*

	GALILEI Research Project

	GIWord.cpp

	Word Information - Implementation.

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
#include <ginfos/giword.h>



//-----------------------------------------------------------------------------
//
// GIWord
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIWord::GIWord(unsigned int id)
	: GALILEI::GInfo(), Id(id)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GIWord::Compare(const GInfo* i) const
{
	if(SameClass(i))
		return(Id-(static_cast<const GIWord*>(i))->Id);
	else
		return(GALILEI::GInfo::Compare(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWord::Similarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWord*>(i))->Id)==0);
	else
		return(GALILEI::GInfo::Similarity(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWord::DisSimilarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWord*>(i))->Id)!=0);
	else
		return(GALILEI::GInfo::DisSimilarity(i));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWord::ClassName(void) const
{
	return("GIWord");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GIWord::InfoType(void) const
{
	return(infoWord);
}


//-----------------------------------------------------------------------------
GALILEI::GIWord::~GIWord(void)
{
}
