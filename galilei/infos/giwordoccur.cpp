/*

	GALILEI Research Project

	GIWordOccur.cpp

	Word Occurences Information - Implementation.

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
#include <infos/giwordoccur.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GIWordOccur
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::GIWordOccur(const unsigned int id)
	: GIWord(id), Occurs(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::GIWordOccur(const unsigned int id,const unsigned int nb)
	: GIWord(id), Occurs(nb)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordOccur::Compare(const GInfo* i) const
{
	if(SameClass(i))
		return(Id-(static_cast<const GIWordOccur*>(i))->Id);
	else
		return(GALILEI::GInfo::Compare(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccur::Similarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccur*>(i))->Id)==0);
	else
		return(GALILEI::GInfo::Similarity(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccur::DisSimilarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccur*>(i))->Id)!=0);
	else
		return(GALILEI::GInfo::DisSimilarity(i));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordOccur::ClassName(void) const
{
	return("GIWordOccur");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GIWordOccur::InfoType(void) const
{
	return(infoWordOccur);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::~GIWordOccur(void)
{
}
