/*

	GALILEI Research Project

	GInfo.cpp

	Basic Information - Implementation.

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
#include <infos/giwordoccurs.h>



//-----------------------------------------------------------------------------
//
// GIWordOccurs
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIWordOccurs::GIWordOccurs(unsigned int id)
	: GALILEI::GIWord(id), Occurs(0)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordOccurs::Compare(const GInfo* i) const
{
	if(SameClass(i))
		return(Id-(static_cast<const GIWordOccurs*>(i))->Id);
	else
		return(GALILEI::GInfo::Compare(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccurs::Similarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccurs*>(i))->Id)==0);
	else
		return(GALILEI::GInfo::Similarity(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccurs::DisSimilarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccurs*>(i))->Id)!=0);
	else
		return(GALILEI::GInfo::DisSimilarity(i));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordOccurs::ClassName(void) const
{
	return("GIWordOccurs");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GIWordOccurs::InfoType(void) const
{
	return(IWordOccurs);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordOccurs::~GIWordOccurs(void)
{
}
