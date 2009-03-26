/*

	GALILEI Research Project

	GCAGroup.hh

	Group - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofile.h>
#include <gdoc.h>
#include <gsession.h>
#include <guser.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcagroup.h>
#include <gcachromo.h>
#include <gcainst.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class SubProfileLocal
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*class SubProfileLocal
{
public:
	GCAObj* Prof;
	double AvgSim;
};


//-----------------------------------------------------------------------------
int sort_function(const void* a,const void* b)
{
	double af=(static_cast<const SubProfileLocal*>(a))->AvgSim;
	double bf=(static_cast<const SubProfileLocal*>(b))->AvgSim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}
*/


//-----------------------------------------------------------------------------
//
// class GCAGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAGroup::GCAGroup(GCAGroup* grp)
	: RGroupSC<GCAGroup,GCAObj,GCAChromo>(grp)
{
}


//-----------------------------------------------------------------------------
GCAGroup::GCAGroup(GCAChromo* owner,const size_t id)
	: RGroupSC<GCAGroup,GCAObj,GCAChromo>(owner,id)
{
}


//---------------------------------------------------------------------------
GCAGroup::~GCAGroup(void)
{
}
