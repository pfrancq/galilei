/*

	GALILEI Research Project

	GDescriptionObject.hh

	Object represented by a list of weighted information entities - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GDescriptionObject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C>
	GDescriptionObject<C>::GDescriptionObject(GSession* session,size_t id,size_t blockid,tObjType objtype,const R::RString& name)
	: GObject(session,id,name,objtype), GDescription(), State(osLatest), BlockId(blockid)
{
}


//------------------------------------------------------------------------------
template<class C>
	bool GDescriptionObject<C>::LoadVectors(void)
{
	if(BlockId)
	{
		tObjState old(State);
		State=osOnDemand;                        // The object is on-demand of loading
		Session->LoadDesc(static_cast<C*>(this),static_cast<C*>(this)->Vectors,BlockId,Id);  // Load the object
		State=old;                               // Put the old state back
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
template<class C>
	void GDescriptionObject<C>::Clear(bool disk)
{
	GDescription::Clear();
	if(disk)
		BlockId=0;
}


//------------------------------------------------------------------------------
template<class C>
	GDescriptionObject<C>::~GDescriptionObject(void)
{
}
