/*

	GALILEI Research Project

	GDocsProfilesSimsCosinus.cpp

	Similarities between documents and profiles - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <ggenericsims.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GDocsProfilesSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsProfilesSims : public GGenericSims
{
public:
	GDocsProfilesSims(GFactoryMeasure* fac) : GGenericSims(fac,false,otDocProfile) {}
};


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Documents/Profiles Similarities","Multi-vector",GDocsProfilesSims)
