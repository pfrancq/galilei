/*
	GALILEI Research Project

	GLink.cpp

	Link - Implementation.

	Copyright 2003-2008 by the Université Libre de Bruxelles.

	Authors
		 Vandaele valery(vavdaele@ulb.ac.be)

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
#include <glink.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// GLink
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLink::GLink(const GDoc* doc)
	: DocTO(doc), Occurs(0)
{
}


//------------------------------------------------------------------------------
int GLink::Compare(const GLink* link) const
{
	return(DocTO->GetId() - link->GetDoc()->GetId());
}


//------------------------------------------------------------------------------
int GLink::Compare(const GLink& link) const
{
	return(DocTO->GetId()-link.GetDoc()->GetId());
}


//------------------------------------------------------------------------------
int GLink::Compare( const size_t id) const
{
	return (DocTO->GetId()- id);
}


//------------------------------------------------------------------------------
int GLink::Compare(const GDoc* doc) const
{
	return(DocTO->GetId()- doc->GetId());
}


//------------------------------------------------------------------------------
int GLink::Compare(const GDoc& doc) const
{
	return (DocTO->GetId() - doc.GetId());
}


//------------------------------------------------------------------------------
GLink::~GLink(void)
{
}
