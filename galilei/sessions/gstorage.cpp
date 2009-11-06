/*

	GALILEI Research Project

	GStorage.cpp

	Generic Storage Manager - Implementation.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GStorage
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorage::GStorage(GPluginFactory* fac)
	: GPlugin(fac), LoadAll(true), Filter(RDate::GetToday()), Filtering(false),
	  AllMemory(true)
{
}


//------------------------------------------------------------------------------
void GStorage::ApplyConfig(void)
{
	AllMemory=(LoadAll&&(!Filtering));
}


//------------------------------------------------------------------------------
void* GStorage::GetInfos(void)
{
	return(0);
}


//------------------------------------------------------------------------------
bool GStorage::IsAllInMemory(void) const
{
	return(AllMemory);
}


//------------------------------------------------------------------------------
bool GStorage::UseFiltering(void) const
{
	return(Filtering);
}


//------------------------------------------------------------------------------
R::RDate GStorage::GetFilter(void) const
{
	return(Filter);
}


//------------------------------------------------------------------------------
GStorage::~GStorage(void)
{
}
