/*

	GALILEI Research Project

	GProfileCalc.cpp

	Generic Profile Computing Method - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
//include files for GALILEI
#include <profiles/gprofilecalc.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfileCalc::GProfileCalc(const char* name,GSession* session) throw(bad_alloc)
	: ComputingName(name),Session(session)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GProfileCalc::Compare(const GProfileCalc& desc) const
{
	return(ComputingName.Compare(desc.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfileCalc::Compare(const GProfileCalc* desc) const
{
	return(ComputingName.Compare(desc->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfileCalc::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfileCalc::Compare(const tSubProfileDesc t) const
{
	return(GetType()-t);
}


//-----------------------------------------------------------------------------
GALILEI::GProfileCalc::~GProfileCalc(void)
{
}
