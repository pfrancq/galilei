/*

	GALILEI Research Project

	GDescriptionSet.cpp

	Description Set - Implementation.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for C/C++ ANSI
#include <cmath>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescriptionset.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GDescriptionSetData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDescriptionSetData::GDescriptionSetData(GSession*)
	: Dirty(true)
{

}


//-----------------------------------------------------------------------------
GDescriptionSetData::~GDescriptionSetData(void)
{

}



//-----------------------------------------------------------------------------
//
// GDescriptionSet
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDescriptionSet::GDescriptionSet(GSession* session)
	: Session(session), Set(200),
	  Data(0)
{
}


//-----------------------------------------------------------------------------
void GDescriptionSet::Clear(void)
{
	Set.Clear();
	if(Data)
		Data->Dirty=true;
}


//-----------------------------------------------------------------------------
void GDescriptionSet::InsertDescription(const GDescription* desc)
{
	Set.InsertPtr(desc);
	if(Data)
		Data->Dirty=true;
}


//-----------------------------------------------------------------------------
void GDescriptionSet::SetData(GDescriptionSetData* data)
{
	Data=data;
}


//-----------------------------------------------------------------------------
RCursor<const GDescription> GDescriptionSet::GetDescriptions(void) const
{
	return(RCursor<const GDescription>(Set));
}


//-----------------------------------------------------------------------------
GDescriptionSet::~GDescriptionSet(void)
{
	if(Data)
	{
		delete Data;
		Data=0;
	}
}