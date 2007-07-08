/*

	Genetic Community Algorithm

	GGCAGroup.hh

	Group - Header.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsubprofile.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gsession.h>
#include <guser.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggcagroup.h>
#include <ggcachromo.h>
#include <ggcainst.h>
#include <ggcaobj.h>



//-----------------------------------------------------------------------------
//
// class SubProfileLocal
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class SubProfileLocal
{
public:
	GGCAObj* Prof;
	double AvgSim;
};


//-----------------------------------------------------------------------------
int sort_function(const void* a,const void* b)
{
	double af=(static_cast<const SubProfileLocal*>(a))->AvgSim;
	double bf=(static_cast<const SubProfileLocal*>(b))->AvgSim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//-----------------------------------------------------------------------------
//
// class GGCAGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAGroup::GGCAGroup(GGCAGroup* grp)
	: RGroup<GGCAGroup,GGCAObj,GGCAChromo>(grp), BestSumDist(0.0),
	  Relevant(0), Dirty(true), ToEval(true)
{
}


//-----------------------------------------------------------------------------
GGCAGroup::GGCAGroup(GGCAChromo* owner,const unsigned int id)
	: RGroup<GGCAGroup,GGCAObj,GGCAChromo>(owner,id), BestSumDist(0.0),
	  Relevant(0), Dirty(true), ToEval(true)
{
}


//---------------------------------------------------------------------------
void GGCAGroup::Clear(void)
{
	RGroup<GGCAGroup,GGCAObj,GGCAChromo>::Clear();
	BestSumDist=0.0;
	Relevant=0;
	ToEval=Dirty=true;
}


//---------------------------------------------------------------------------
bool GGCAGroup::HasSameUser(const GGCAObj* obj) const
{
	GUser* usr=obj->GetSubProfile()->GetProfile()->GetUser();
	RCursor<GGCAObj> ptr=Owner->GetObjs(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
		if(usr==ptr()->GetSubProfile()->GetProfile()->GetUser())
			return(true);
	return(false);
}


//---------------------------------------------------------------------------
bool GGCAGroup::CanInsert(const GGCAObj* obj)
{
	GSubProfile* sub1;
	GSubProfile* sub2;

	if(!NbSubObjects)
		return(true);
	sub1=obj->GetSubProfile();
	GUser* usr1=sub1->GetProfile()->GetUser();
	RCursor<GGCAObj> ptr=Owner->GetObjs(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		sub2=ptr()->GetSubProfile();
		// Do not put two profiles of the same user in the same group
		if(usr1==sub2->GetProfile()->GetUser())
			return(false);
		// Min sim and min disagreement must be respected
		if((Owner->Instance->GetSim(sub1,sub2)<=Owner->Instance->Params->MinSimLevel) ||
		  (Owner->Instance->GetDisagreementRatio(sub1,sub2)>=Owner->Instance->Params->MinDisagreement))
		  	return(false);
	}
	return(true);
}


//---------------------------------------------------------------------------
void GGCAGroup::PostInsert(const GGCAObj* /*obj*/)
{
	ToEval=Dirty=true;
}


//---------------------------------------------------------------------------
void GGCAGroup::PostDelete(const GGCAObj* /*obj*/)
{
	ToEval=Dirty=true;
}


//---------------------------------------------------------------------------
double GGCAGroup::ComputeSumDist(GGCAObj* obj)
{
	double Sum;
	GSubProfile* sub;
	double tmp;

	if(!NbSubObjects)
		return(0.0);
	sub=obj->GetSubProfile();
	RCursor<GGCAObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start(),Sum=0.0;!ptr.End();ptr.Next())
	{
		if(ptr()==obj) continue;
		tmp=1-Owner->Instance->GetSim(sub,ptr()->GetSubProfile());
		Sum+=tmp*tmp;
	}
	return(Sum);
}


//---------------------------------------------------------------------------
void GGCAGroup::ComputeRelevant(void)
{
	double SumDist;

	if(!Dirty)
		return;

	// If no objects -> No relevant one.
	if(!NbSubObjects)
	{
		Relevant=0;
		BestSumDist=0.0;
		Dirty=false;
		return;
	}

	// Suppose the first element is the most relevant.
	RCursor<GGCAObj> ptr(Owner->GetObjs(*this));
	ptr.Start();
	Relevant=(ptr());
	BestSumDist=ComputeSumDist(ptr());

	// Look if in the other objects, there is a better one
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		SumDist=ComputeSumDist(ptr());
		if(SumDist<BestSumDist)
		{
			Relevant=(ptr());
			BestSumDist=SumDist;
		}
	}
	Dirty=false;
}


//---------------------------------------------------------------------------
void GGCAGroup::Evaluate(double& dist,double& agree,double& disagree)
{
	ComputeRelevant();
	if(ToEval)
	{
		ToEval=false;
		RCursor<GGCAObj> CurObj(Owner->GetObjs(*this));
		RCursor<GGCAObj> CurObj2(Owner->GetObjs(*this));
		size_t i;
		for(CurObj.Start(),i=0,AgreementSum=DisagreementSum=0.0;i<GetNbObjs()-1;CurObj.Next(),i++)
		{
			for(CurObj2.GoTo(i+1);!CurObj2.End();CurObj2.Next())
			{
				AgreementSum+=Owner->Instance->GetAgreementRatio(CurObj()->GetSubProfile(),CurObj2()->GetSubProfile());
				DisagreementSum+=Owner->Instance->GetDisagreementRatio(CurObj()->GetSubProfile(),CurObj2()->GetSubProfile());
			}
		}
	}
	dist+=BestSumDist;
	agree+=AgreementSum;
	disagree+=DisagreementSum;
}


//---------------------------------------------------------------------------
void GGCAGroup::SetRelevant(GGCAObj* obj)
{
	Relevant=obj;
	ToEval=Dirty=true;
}


//---------------------------------------------------------------------------
GGCAGroup& GGCAGroup::operator=(const GGCAGroup& grp)
{
	RGroup<GGCAGroup,GGCAObj,GGCAChromo>::operator=(grp);
	BestSumDist=grp.BestSumDist;
	Relevant=grp.Relevant;
	Dirty=grp.Dirty;
	ToEval=grp.ToEval;
	AgreementSum=grp.AgreementSum;
	DisagreementSum=grp.AgreementSum;
	return(*this);
}


//---------------------------------------------------------------------------
double GGCAGroup::GetMaxRatioSame(GGCAObj* obj)
{
	double max,tmp;
	GSubProfile* sub=obj->GetSubProfile();

	// Look if in the other objects, there is a better one
	RCursor<GGCAObj> ptr=Owner->GetObjs(*this);
	for(ptr.Start(),max=0.0;!ptr.End();ptr.Next())
	{
		tmp=Owner->Instance->GetAgreementRatio(sub,ptr()->GetSubProfile());
		if(tmp>max)
			max=tmp;
	}
	return(max);
}


//---------------------------------------------------------------------------
GGCAGroup::~GGCAGroup(void)
{
}
