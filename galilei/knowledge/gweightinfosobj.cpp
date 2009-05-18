/*

	GALILEI Research Project

	GWeightInfosObj.hh

	Object represented by a list of weighted information entities - Implementation.

	Copyright 2009 by Pascal Francq (pascal@francq.info).

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
#include <gweightinfosobj.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GWeightInfosObj
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfosObj::GWeightInfosObj(size_t id,size_t blockid,tObjType objtype,const R::RString& name,tObjState state)
	: GObject(id,name,objtype), State(state), Vector(0), BlockId(blockid)
{
	if(Id!=R::cNoRef)
		Emit(GEvent::eObjNew);
}


//------------------------------------------------------------------------------
const GWeightInfos* GWeightInfosObj::GetVector(void) const
{
	if(!Vector)
	{
		if(BlockId)
		{
			const_cast<GWeightInfosObj*>(this)->State=osOnDemand;      // The object is on-demand of loading
			GSession* session=GSession::Get();
			if(session)
				session->LoadInfos(const_cast<GWeightInfosObj*>(this)->Vector,ObjType,BlockId,Id); // Load the object
			const_cast<GWeightInfosObj*>(this)->State=osUpToDate;         // It is updated !
		}
		else
			const_cast<GWeightInfosObj*>(this)->Vector=new GWeightInfos(1);
	}
	return(Vector);
}


//------------------------------------------------------------------------------
void GWeightInfosObj::SetState(tObjState state)
{
	if((!Vector)&&(state==osDelete))
	{
		GetVector();
	}
	State=state;
}


//------------------------------------------------------------------------------
void GWeightInfosObj::SetId(size_t id)
{
	if(id==R::cNoRef)
		throw GException("Cannot assign cNoRef to a "+GALILEI::GetObjType(ObjType));
	Id=id;
	Emit(GEvent::eObjNew);
}


//------------------------------------------------------------------------------
void GWeightInfosObj::CopyInfos(const R::RContainer<GWeightInfo,false,true>& infos)
{
	State=osUpToDate;
	if(infos.GetNb())
	{
		if(!Vector)
			Vector=new GWeightInfos(infos.GetNb());
		Vector->CopyInfos(infos);
	}
	else
	{
		if(Vector)
			Vector->Clear();
	}
}


//------------------------------------------------------------------------------
void GWeightInfosObj::Clear(void)
{
	if(Vector)
		Vector->Clear();
}


//------------------------------------------------------------------------------
void GWeightInfosObj::Transfer(GWeightInfos& info)
{
	GetVector();
	Vector->Transfer(info);
}


//------------------------------------------------------------------------------
GWeightInfosObj::~GWeightInfosObj(void)
{
	Emit(GEvent::eObjDelete);

	try
	{
		if(State==osDelete)  // The object has modified the references count but was not saved
			GetVector()->DelRefs(ObjType);
	}
	catch(...)
	{
	}
	delete Vector;
}

