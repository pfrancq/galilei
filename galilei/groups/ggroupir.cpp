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
#include <profiles/gsubprofiledesc.h>
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
	: RGGA::RGroup<GGroupIR,GSubProfileDesc,GGroupDataIR>(grp)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupIR::GGroupIR(RGroups<GGroupIR,GSubProfileDesc,GGroupDataIR>* owner,const unsigned int id,const GGroupDataIR* data)
	: RGGA::RGroup<GGroupIR,GSubProfileDesc,GGroupDataIR>(owner,id,data)
{
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::Verify(void)
{
	if(!RGGA::RGroup<GGroupIR,GSubProfileDesc,GGroupDataIR>::Verify())
		return(false);
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::Clear(void)
{
	RGGA::RGroup<GGroupIR,GSubProfileDesc,GGroupDataIR>::Clear();
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::CanInsert(const GSubProfileDesc* /*obj*/)
{
	return(true);
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostInsert(const GSubProfileDesc* /*obj*/)
{
}


//---------------------------------------------------------------------------
void GALILEI::GGroupIR::PostDelete(const GSubProfileDesc* /*obj*/)
{
}


//---------------------------------------------------------------------------
bool GALILEI::GGroupIR::DoOptimisation(GSubProfileDesc** /*objs*/,unsigned int& /*nbobjs*/)
{
	return(true);
}


//---------------------------------------------------------------------------
GGroupIR& GALILEI::GGroupIR::operator=(const GGroupIR& grp)

{
	RGGA::RGroup<GGroupIR,GSubProfileDesc,GGroupDataIR>::operator=(grp);
	return(*this);
}


//---------------------------------------------------------------------------
GALILEI::GGroupIR::~GGroupIR(void)
{
}
