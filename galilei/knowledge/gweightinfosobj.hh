/*

	GALILEI Research Project

	GWeightInfosObj.hh

	Object represented by a list of weighted information entities - Inline Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
//
//  GWeightInfosObj<cObj,tObjType>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	GWeightInfosObj<cObj,type>::GWeightInfosObj(size_t id,size_t size,tObjState state)
		: Id(id), State(state), Vector(0), Size(size)
{
	if(Id!=R::cNoRef)
		GSession::Event(static_cast<cObj*>(this),eObjNew);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	const GWeightInfos* GWeightInfosObj<cObj,type>::GetVector(void) const
{
	if(!Vector)
			const_cast<GWeightInfosObj*>(this)->Vector=new GWeightInfos(Size);
 	if((State==osNeedLoad)&&Size)
	{
		const_cast<GWeightInfosObj*>(this)->State=osOnDemand;      // The object is on-demand of loading
		GSession* session=GSession::Get();
		if(session)
			session->LoadInfos(*Vector,type,Id);                   // Load the object
		const_cast<GWeightInfosObj*>(this)->State=osUpToDate;      // It is updated !
	}
	return(Vector);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::SetState(tObjState state)
{
	if((State==osNeedLoad)&&(state==osDelete))
	{
		if(!Vector)
			const_cast<GWeightInfosObj*>(this)->Vector=new GWeightInfos(Size);

		// Force to load the description since the references must be decreased
		// in the destructor.
		const_cast<GWeightInfosObj*>(this)->State=osOnDemand;      // The object is on-demand of loading
		GSession* session=GSession::Get();
		if(session)
			session->LoadInfos(*Vector,type,Id);                   // Load the object
		const_cast<GWeightInfosObj*>(this)->State=osUpToDate;      // It is updated !
	}
	State=state;
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::SetId(size_t id)
{
	if(id==R::cNoRef)
		throw GException("Cannot assign cNoRef to a "+GetObjType(type));
	Id=id;
	GSession::Event(static_cast<cObj*>(this),eObjNew);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::CopyInfos(const R::RContainer<GWeightInfo,false,true>& infos)
{
	Size=infos.GetNb();
	State=osUpToDate;
	if(Size)
	{
		if(!Vector)
			Vector=new GWeightInfos(Size);
		Vector->CopyInfos(infos);
	}
	else
	{
		if(Vector)
			Vector->Clear();
	}
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::Clear(void)
{
	Size=0;
	if(Vector)
		Vector->Clear();
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::SetSize(size_t size)
{
	Size=size;
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::AddRefs(tObjType ObjType) const
{
	GetVector()->AddRefs(ObjType);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::DelRefs(tObjType ObjType) const
{
	GetVector()->DelRefs(ObjType);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	void GWeightInfosObj<cObj,type>::Transfer(GWeightInfos& info)
{
	Size=info.GetNb();
	GetVector();
	Vector->Transfer(info);
}


//------------------------------------------------------------------------------
template<class cObj,tObjType type>
	GWeightInfosObj<cObj,type>::~GWeightInfosObj(void)
{
	GSession::Event(static_cast<cObj*>(this),eObjDelete);

	try
	{
		if(State==osDelete)  // The object has modified the references count but was not saved
			Vector->DelRefs(type);
	}
	catch(...)
	{
	}
	delete Vector;
}

