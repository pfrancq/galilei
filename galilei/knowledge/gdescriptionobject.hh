/*

	GALILEI Research Project

	GDescription.hh

	Object represented by a list of weighted information entities - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	GDescriptionObject<C,hCreate,hNew,hDel>::GDescriptionObject(GSession* session,size_t id,size_t blockid,tObjType objtype,const R::RString& name,tObjState state)
	: GObject(session,id,name,objtype), GDescription(), State(state), BlockId(blockid)
{
	if(Id!=R::cNoRef)
		PostNotification(hNew);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	bool GDescriptionObject<C,hCreate,hNew,hDel>::LoadVectors(void)
{
	if(BlockId)
	{
		static_cast<C*>(this)->State=osOnDemand;                        // The object is on-demand of loading
		Session->LoadDesc(static_cast<C*>(this),static_cast<C*>(this)->Vectors,BlockId,Id);  // Load the object
		static_cast<C*>(this)->State=osUpToDate;                        // It is updated !
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	void GDescriptionObject<C,hCreate,hNew,hDel>::SetState(tObjState state)
{
	if((!Vectors)&&(state==osDelete))
	{
		GetVectors();
	}
	State=state;
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	void GDescriptionObject<C,hCreate,hNew,hDel>::SetId(size_t id)
{
	if(id==R::cNoRef)
		ThrowGException("Cannot assign cNoRef to a "+GALILEI::GetObjType(ObjType,false,false));
	Id=id;
	PostNotification(hNew);
}



//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	void GDescriptionObject<C,hCreate,hNew,hDel>::Copy(GConcept* concept,const R::RContainer<GConceptRef,false,true>& list)
{
	State=osUpToDate;
	GDescription::Copy(concept,list);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	void GDescriptionObject<C,hCreate,hNew,hDel>::Clear(bool disk)
{
	GDescription::Clear();
	if(disk)
		BlockId=0;
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	void GDescriptionObject<C,hCreate,hNew,hDel>::SaveDesc(void)
{
	if(!Vectors)
		return;
	Session->SaveDesc(static_cast<C*>(0),*Vectors,BlockId,Id);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification& hCreate,const R::hNotification& hNew,const R::hNotification& hDel>
	GDescriptionObject<C,hCreate,hNew,hDel>::~GDescriptionObject(void)
{
	PostNotification(hDel);

	try
	{
		if(State==osDelete)  // The object has modified the references count but was not saved
			DelRefs(Session,ObjType);
	}
	catch(...)
	{
	}
}
