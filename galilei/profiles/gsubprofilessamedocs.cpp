/*

	GALILEI Research Project

	GSubProfileSamesDocs.cpp

	SubProfiles with common documents judged - Implementation

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
// include files for GALILEI
//#include <groups/ginstir.h>
//#include <groups/gchromoir.h>
//#include <groups/ggroupir.h>
//#include <groups/gobjir.h>
//#include <groups/girheuristic.h>
//#include <groups/gcomparegrouping.h>
#include <profiles/gsubprofilessamedocs.h>
//#include <profiles/gprofilessim.h>
//#include <profiles/gsubprofile.h>
//#include <profiles/gprofile.h>
//#include <groups/ggroupvector.h>
//#include <groups/ggroups.h>
//#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GSubProfilesSameDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfilesSameDocs::GSubProfilesSameDocs(unsigned int i1,unsigned int i2,double r)
	: Ratio(r)
{
	if(i1>i2)
	{
		Id1=i1;
		Id2=i2;
	}
	else
	{
		Id1=i2;
		Id2=i1;
	}
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfilesSameDocs::Compare(const GSubProfilesSameDocs* g) const
{
	unsigned int comp;

	comp=Id1-g->Id1;
	if(comp) return(comp);
	return(Id2-g->Id2);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfilesSameDocs::Compare(const GSubProfilesSameDocs& g) const
{
	unsigned int comp;

	comp=Id1-g.Id1;
	if(comp) return(comp);
	return(Id2-g.Id2);
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfilesSameDocs::IsIn(const tId obj) const
{
	return((obj==Id1)||(obj==Id2));
}
