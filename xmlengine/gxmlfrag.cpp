/*

	GALILEI Research Project

	GXMLFrag.cpp

	Chromosome for an IR Problem - Implementation

	Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)

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
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gxmlfrag.h>
#include <gsession.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GFrag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GXmlFrag::GXmlFrag(unsigned int id)
	: RContainer<RPromCritValue, true, true>(5,3),
	  CritDocSc(0.0), CritTfief(0.0), CritDis(0.0),  CritSpecif(0.0),  ID(id)
{
}


//-----------------------------------------------------------------------------
GALILEI::GXmlFrag::~GXmlFrag(void)
{
}
