/*

	GALILEI Research Project

	GGroupEvaluate.cpp

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <tests/ggroupevaluate.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupEvaluate
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluate::GGroupEvaluate(GLang* lang)
	:  Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluate::Compare(const GGroupEvaluate& ) const
{
	return(-1);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluate::Compare(const GGroupEvaluate* ) const
{
	return(-1);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluate::~GGroupEvaluate(void)
{

}
