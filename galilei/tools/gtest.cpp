/*

	GALILEI Research Project

	GTest.cpp

	Internal Testing - Implementation.

	Copyright 2008 by the Université libre de Bruxelles.

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
// include files for GALILEI Project
#include <gtest.h>
#include <gsession.h>
#include <gconcepttype.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GTest
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTest::GTest(const RString& file)
	: Results(file)
{
	Results.Open(RIO::Create);
}


//------------------------------------------------------------------------------
void GTest::Run(void)
{
	GSession* Session(GSession::Get());

	// Compute idf, ipf, icf and itf factors
	RCursor<GConceptType> Types(Session->GetConceptTypes());
	for(Types.Start();!Types.End();Types.Next())
	{
		GLang* Lang(Types()->GetLang());
		if(Lang&&(Lang->GetStop()==Types()))
			continue;
	}
}


//------------------------------------------------------------------------------
GTest::~GTest(void)
{
}
