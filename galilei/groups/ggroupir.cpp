/*

	R Project Library

	RNodeGA.hh

	GA Node - Header.

	(C) 2001 by P. Francq.

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
#include <groups/ggroupir.h>
#include <groups/gchromoir.h>
#include <groups/gobjir.h>
using namespace GALILEI;
using namespace RGGA;
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class GGroupIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupIR::GGroupIR(GGroupIR* grp)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR>(grp), AvgSim(0.0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupIR::GGroupIR(RGroups<GGroupIR,GObjIR,GGroupDataIR>* owner,const unsigned int id,const GGroupDataIR* data)
	: RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR>(owner,id,data)
{
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::Verify(void)
{
	if(!RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR>::Verify())
		return(false);
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::Clear(void)
{
	RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR>::Clear();
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::CanInsert(const GObjIR* /*obj*/)
{
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostInsert(const GObjIR* /*obj*/)
{
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostDelete(const GObjIR* /*obj*/)
{
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::DoOptimisation(GObjIR** /*objs*/,unsigned int& /*nbobjs*/)
{
	return(true);
}


//---------------------------------------------------------------------------
GGroupIR& GALILEI::GGroupIR::operator=(const GGroupIR& grp)

{
	RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR>::operator=(grp);
	AvgSim=grp.AvgSim;
	return(*this);
}


//---------------------------------------------------------------------------
GALILEI::GGroupIR::~GGroupIR(void)
{
}
