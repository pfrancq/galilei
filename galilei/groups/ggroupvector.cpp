/*

	GALILEI Research Project

	GGroupVector.cpp

	Group in the Vector Model- Implementation.

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroupvector.h>
#include <profiles/gsubprofile.h>
#include <langs/glang.h>
#include <langs/gdict.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupVector::GGroupVector(const unsigned int id,GLang* lang) throw(bad_alloc)
	: GGroup(id,lang), GIWordsWeights(60)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupVector::UpdateRefs(void) const
{
	GDict* d=Lang->GetDict();
	if(d)
		AddRefs(otGroup,d);

}


//-----------------------------------------------------------------------------
void GALILEI::GGroupVector::RemoveRefs(void) const
{
	GDict* d=Lang->GetDict();
	if(d)
		DelRefs(otGroup,d);
}
