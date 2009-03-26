/*

	GALILEI Research Project

	GLinks.cpp

	List of Links - Implementation.

	Copyright 2003-2004 by Valery Vandaele.
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
// include files for GALILEI
#include <glinks.h>
#include <glink.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GLinks
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLinks::GLinks(GDoc* d)
	: RContainer<GLink,false,true>(3,2), Doc(d)
{
	if(!d)
		throw GException("Document cannot be null for a list of links");
}


//------------------------------------------------------------------------------
R::RCursor<GLink> GLinks::GetLinks(void)
{
	return(R::RCursor<GLink>(*this));
}


//------------------------------------------------------------------------------
int GLinks::Compare(const GLinks* lnk) const
{
	return(CompareIds(Doc->GetId(),lnk->GetDoc()->GetId()));
}


//------------------------------------------------------------------------------
int GLinks::Compare(const GLinks& lnk) const
{
	return(CompareIds(Doc->GetId(),lnk.GetDoc()->GetId()));
}


//------------------------------------------------------------------------------
int GLinks::Compare(const size_t id) const
{
	return(CompareIds(Doc->GetId(),id));
}


//------------------------------------------------------------------------------
GLinks::~GLinks(void)
{
}
