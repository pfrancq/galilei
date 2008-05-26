/*

	GALILEI Research Project

	GTopic.cpp

	Group - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gtopic.h>
#include <gdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GTopic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTopic::GTopic(unsigned int id,const R::RDate& u,const R::RDate& c)
	: GGroup<GDoc,GTopic,otTopic>(id,u,c)
{
}


//------------------------------------------------------------------------------
GTopic::~GTopic(void)
{
}
