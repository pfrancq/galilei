/*

	GALILEI Research Project

	GStorage.cpp

	Generic Storage Manager - Implementation.

	Copyright 2003-2005 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
GStorage::GStorage(RString n,bool all,const R::RDate& filter) throw(std::bad_alloc,GException)
	: Session(0), Name(n), LoadAll(all), Filter(filter), Filtering(filter!=RDate::GetToday()),
	  AllMemory(LoadAll&&(!Filtering)), Cmds(30,15)
{
}


//------------------------------------------------------------------------------
void GStorage::Connect(GSession* session)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GStorage::Disconnect(GSession*)
{
	Session=0;
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
RString GStorage::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
bool GStorage::IsCmdSupported(const R::RString cmd) const
{
	return(Cmds.IsIn(cmd));
}


//------------------------------------------------------------------------------
void GStorage::ExecuteCmd(const R::RXMLTag& inst,void* caller)
{
	GStorageCmd* cmd=Cmds.GetPtr(inst.GetName());
	if(!cmd)
		throw GException("Command '"+inst.GetName()+"' not support on storage "+Name);
	cmd->Run(this,inst,caller);
}


//------------------------------------------------------------------------------
GStorage::~GStorage(void)
{
}
