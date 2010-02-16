/*

	GALILEI Project

	GNodeInfos.h

	Node representing a set of words. - Header.

	Copyright 1998-2010 by Pascal Francq (pfrancq@ulb.ac.be).
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


//-----------------------------------------------------------------------------
#ifndef GNodeInfosH
#define GNodeInfosH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <robjh.h>
#include <rnodesga.h>
#include <rnodega.h>
#include <ghga.h>
using namespace R;


//-----------------------------------------------------------------------------
class GNodeInfos : public RNodeGA<GNodeInfos,RObjH,GChromoH>
{
public:

	using RNode<GChromoH,GNodeInfos,false>::GetNodes;

	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identifier of the node.
	*/
	GNodeInfos(GChromoH* owner,size_t id,size_t max);

	int Compare(const GNodeInfos* n) const;

	int Compare(const GNodeInfos& n) const;

	void Evaluate(double& val, double nbchoices);
	virtual void CopyInfos(const GNodeInfos& w);
};


//-----------------------------------------------------------------------------
#endif
