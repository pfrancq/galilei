/*

	GALILEI Research Project

	GSubjectTree.cpp

	GSubjectTree - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <iostream>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R-Project
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gsubjecttree.h>
#include <sessions/gsession.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <groups/gsubject.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
using namespace GALILEI;
using namespace RMath;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GSubjectTree
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSubjectTree::GSubjectTree(void)
	: RTree<GSubject,true,false>(100,50)
{
}


//-----------------------------------------------------------------------------
GALILEI::GSubjectTree::~GSubjectTree(void)
{
}
