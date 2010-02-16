/*

	GALILEI Research Project

	GGroup.hh

	Generic Group - Inline Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
//  GGroup<cObj,cGroup,tObjType>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	GALILEI::GGroup<cObj,cGroup,type>::GGroup(size_t id,size_t blockid,const R::RString& name,const R::RDate& u,const R::RDate& c)
	: R::RContainer<cObj,false,true>(20,10), GWeightInfosObj(id,blockid,type,name,osNew),
	  Updated(u), Computed(c), Data(0)
{
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	GSubjectData<cObj>* GALILEI::GGroup<cObj,cGroup,type>::GetData(void) const
{
	if(!Data)
		const_cast<GGroup*>(this)->Data=new GSubjectData<cObj>();
	return(Data);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	int GALILEI::GGroup<cObj,cGroup,type>::Compare(const GGroup& grp) const
{
	return(R::CompareIds(Id,grp.Id));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	int GALILEI::GGroup<cObj,cGroup,type>::Compare(const GGroup* grp) const
{
	if(!grp)
		return(1);
	return(R::CompareIds(Id,grp->Id));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	int GALILEI::GGroup<cObj,cGroup,type>::Compare(const size_t id) const
{
	return(R::CompareIds(Id,id));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	R::RDate GALILEI::GGroup<cObj,cGroup,type>::GetUpdated(void) const
{
	return(Updated);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	R::RDate GALILEI::GGroup<cObj,cGroup,type>::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	bool GALILEI::GGroup<cObj,cGroup,type>::IsIn(const cObj* obj) const
{
	return(R::RContainer<cObj,false,true>::IsIn(*obj));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::DeleteObj(cObj* obj)
{
	obj->SetGroup(0);
	R::RContainer<cObj,false,true>::DeletePtr(*obj);
	if(Data)
		Data->Dirty();
//	State=osUpdated;
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::InsertObj(cObj* obj)
{
	R::RContainer<cObj,false,true>::InsertPtr(obj);
//	State=osUpdated;
	obj->SetGroup(Id);
	if(Data)
		Data->Dirty();
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::InsertPtr(cObj* obj)
{
	InsertProfile(obj);
	if(Data)
		Data->Dirty();
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::DeleteObjs(void)
{
//	State=osUpdated;
	R::RCursor<cObj> Objs(*this);
	for(Objs.Start();!Objs.End();Objs.Next())
		Objs()->SetGroup(0);
	if(Data)
		Data->Dirty();
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	R::RCursor<cObj> GALILEI::GGroup<cObj,cGroup,type>::GetObjs(void) const
{
	return(R::RCursor<cObj>(*this));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	R::RCursor<cObj> GALILEI::GGroup<cObj,cGroup,type>::GetCursor(void) const
{
	return(R::RCursor<cObj>(*this));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	size_t GALILEI::GGroup<cObj,cGroup,type>::GetNbObjs(void) const
{
	return(R::RContainer<cObj,false,true>::GetNb());
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	cObj* GALILEI::GGroup<cObj,cGroup,type>::RelevantObj(double& avgsim) const
{
	cObj* rel;
	double sum;

	// If no objects -> No relevant one.
	if(!R::RContainer<cObj,false,true>::GetNb())
	{
		avgsim=0.0;
		return(0);
	}

	// Suppose the first element is the most relevant.
	R::RCursor<cObj> Prof(*this);
	Prof.Start();
	rel=Prof();
	if(R::RContainer<cObj,false,true>::GetNb()==1)
	{
		avgsim=1.0;
		return(rel);
	}
	// Similarities
	GMeasure* ProfilesSims=GetSimMeasure();
	avgsim=ComputeSumSim(ProfilesSims,rel);

	// Look if in the other objects, there is a better one
	for(;!Prof.End();Prof.Next())
	{
		sum=ComputeSumSim(ProfilesSims,Prof());
		if(sum>=avgsim)
		{
			rel=Prof();
			avgsim=sum;
		}
	}
	avgsim/=static_cast<double>(R::RContainer<cObj,false,true>::GetNb()-1);

	// return most relevant
	return(rel);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	cObj* GALILEI::GGroup<cObj,cGroup,type>::RelevantObj(void) const
{
	double d;
	return(RelevantObj(d));
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	double GALILEI::GGroup<cObj,cGroup,type>::ComputeSumSim(GMeasure* measure,const cObj* obj) const
{
	double sum;

	if(!measure)
		throw GException("No profiles similarities");
	R::RCursor<cObj> Prof(*this);
	for(Prof.Start(),sum=0.0;!Prof.End();Prof.Next())
	{
		if(Prof()==obj) continue;
		double res;
		measure->Measure(0,obj->GetId(),Prof()->GetId(),&res);
		sum+=(res);
	}
	return(sum);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::Clear(void)
{
	GWeightInfosObj::Clear();
	if(Data)
		Data->Dirty();
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::Update(GSession* session,GWeightInfos& infos)
{
	// Remove its references
	DelRefs(type);
	session->UpdateRefs(infos,type,Id,false);

	// Assign information
	GWeightInfosObj::Clear();
	State=osUpdated;
	Computed.SetToday();
	GWeightInfosObj::Transfer(infos);

	// Clear infos
	infos.Clear();

	// Update its references
	AddRefs(type);
	session->UpdateRefs(infos,type,Id,true);

	// Emit an event that it was modified
	if(Data)
		Data->Dirty();
	Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	size_t GALILEI::GGroup<cObj,cGroup,type>::GetNbObjs(const GSubject* subject) const
{
	size_t tot;
	R::RCursor<cObj> sub(*this);
	for(sub.Start(),tot=0;!sub.End();sub.Next())
		if(subject->IsIn(sub()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	void GALILEI::GGroup<cObj,cGroup,type>::HasUpdate(cObj* obj)
{
	if(R::RContainer<cObj,false,true>::GetPtr(*obj))
		Updated.SetToday();
}


//------------------------------------------------------------------------------
template<class cObj,class cGroup,GALILEI::tObjType type>
	GALILEI::GGroup<cObj,cGroup,type>::~GGroup(void)
{
	R::RCursor<cObj> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
		Prof()->SetGroup(0);
	if(Data)
		delete Data;
}
