/*

	GALILEI Research Project

	GProfileCalc.cpp

	Generic Profile Computing Method - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#include <profiles/gprofilecalc.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GProfileCalc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfileCalc::GProfileCalc(GFactoryProfileCalc* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryProfileCalc>(fac), Session(0), SubProfile(0)
{
}


//------------------------------------------------------------------------------
void GProfileCalc::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GProfileCalc::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
GProfileCalc::~GProfileCalc(void)
{
}
