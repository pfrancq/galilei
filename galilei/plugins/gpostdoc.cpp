/*

	GALILEI Research Project

	GPostDoc.cpp

	Generic Documents Post-Analysis Method - Implementation.

	Copyright 2003-2004 by Nicolas Kumps.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
// include file for GALILEI
#include <gpostdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GPostDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostDoc::GPostDoc(GPluginFactory* fac)
	: GPlugin(fac)
{
}


//------------------------------------------------------------------------------
GPostDoc::~GPostDoc(void)
{
}
