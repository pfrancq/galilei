/*

	GALILEI Research Project

	GDocSim.cpp

	Similarity of a documentumentcument - Implementation.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocsim.h>
#include <docs/gdoc.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GDocSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GALILEI::GDocSim::GDocSim(GDoc* d,double s)
	: Doc(d), Sim(s)
{
}


//--------------------------------------------------------------------------
int GALILEI::GDocSim::sortOrder(const void *a,const void *b)
{
	double af=(*((GDocSim**)(a)))->Sim;
	double bf=(*((GDocSim**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
int GALILEI::GDocSim::Compare(const GDocSim* d) const
{
	return(Doc->GetId()-d->Doc->GetId());
}


//------------------------------------------------------------------------------
int GALILEI::GDocSim::Compare(const GDocSim& d) const
{
	return(Doc->GetId()-d.Doc->GetId());
}


//------------------------------------------------------------------------------
int GALILEI::GDocSim::Compare(const GDoc* d) const
{
	return(Doc->GetId()-d->GetId());
}
