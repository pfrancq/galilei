/*
	GALILEI Research Project

	GBalancedLinks.cpp

	List of Links with an associated weight - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be))

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
//include file for GALILEI
#include <docs/gbalancedlinks.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GBalancedLinks
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GBalancedLinks::GBalancedLinks(GDoc* d,double w) throw(bad_alloc, GException)
	: GLinks(d) , Weight(w)
{
}


//------------------------------------------------------------------------------
GBalancedLinks::GBalancedLinks(GDoc* d) throw(bad_alloc, GException)
	: GLinks(d) , Weight(1)
{
}


//------------------------------------------------------------------------------
void GBalancedLinks::SetWeight(double w)
{
	Weight=w;
}


//------------------------------------------------------------------------------
int GBalancedLinks::Compare(const GBalancedLinks* lnk) const
{
	return(Doc->GetId()- lnk->GetDoc()->GetId());
}


//------------------------------------------------------------------------------
int GBalancedLinks::Compare(GBalancedLinks& lnk) const
{
	return(Doc->GetId()- lnk.GetDoc()->GetId());
}


//------------------------------------------------------------------------------
int GBalancedLinks::Compare(const unsigned int id) const
{
	return(Doc->GetId()-id);
}


//------------------------------------------------------------------------------
GBalancedLinks::~GBalancedLinks(void)
{
}
