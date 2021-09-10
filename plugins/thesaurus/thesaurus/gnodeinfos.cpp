/*

	GALILEI Project

	GNodeInfos.cpp

	Node representing a set of words - Implementation.

	Copyright 1998-2014 by Pascal Francq (pfrancq@ulb.ac.be).
	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
// include files for current application
#include <gnodeinfos.h>
#include <gchromoh.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class GNodeInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GNodeInfos::GNodeInfos(size_t id,size_t max)
	: RNodeGA<GNodeInfos,RObjH,GChromoH>(id,max)
{
}


//------------------------------------------------------------------------------
int GNodeInfos::Compare(const GNodeInfos* n) const
{
	return(Id-n->Id);
}


//------------------------------------------------------------------------------
int GNodeInfos::Compare(const GNodeInfos& n) const
{
	return(Id-n.Id);
}


//------------------------------------------------------------------------------
void GNodeInfos::Evaluate(double& val, double nbchoices)
{
	// Update count for this level
	nbchoices+=NbSubNodes+NbSubObjects;

	// For each objects add number of choices to get there
	val+=NbSubObjects*nbchoices;

	// Go through each child nodes in order to continue computing the value
	RNodeCursor<GChromoH,GNodeInfos> Cur(this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Evaluate(val,nbchoices);
}


//------------------------------------------------------------------------------
void GNodeInfos::CopyInfos(const GNodeInfos& w)
{
	RNodeGA<GNodeInfos,RObjH,GChromoH>::CopyInfos(w);
}
