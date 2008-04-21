/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroup.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gsubject.h>
#include <gstorage.h>
#include <gweightinfo.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GFdbkRef
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFdbkRef
{
public:
	GFdbk* Doc;
	double Sim;

	GFdbkRef(GFdbk* d,double s) : Doc(d), Sim(s) {}
	int Compare(const GFdbkRef*) const {return(-1);}
	int Compare(const GFdbkRef&) const {return(-1);}
	int Compare(const GFdbk* d) const
	{
		return(Doc->GetDocId()-d->GetDocId());
	}
};



//------------------------------------------------------------------------------
//
//  GGroup
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroup::GGroup(unsigned int id,bool com,const R::RDate& u,const R::RDate& c)
	: RContainer<GProfile,false,true>(20,10), GWeightInfos(60), Id(id), 
	  Community(com),  Updated(u), Computed(c)
{
	if(Id!=cNoRef)
		GSession::Event(this,eObjNew);
}


//------------------------------------------------------------------------------
int GGroup::sortOrder(const void *a,const void *b)
{
	double af=(*((GFdbkRef**)(a)))->Sim;
	double bf=(*((GFdbkRef**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
int GGroup::Compare(const GGroup& grp) const
{
	return(Id-grp.Id);
}


//------------------------------------------------------------------------------
int GGroup::Compare(const GGroup* grp) const
{
	if(!grp)
		return(1);
	return(Id-grp->Id);
}


//------------------------------------------------------------------------------
int GGroup::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
void GGroup::LoadInfos(void) const
{
	RContainer<GWeightInfo,false,true> Infos(1000,500);
	GSession* session=GSession::Get();
	if(session&&session->GetStorage())
		session->GetStorage()->LoadInfos(Infos,otGroup,Id);
	if(Infos.GetNb())
		const_cast<GGroup*>(this)->Update(&Infos,false);
}


//------------------------------------------------------------------------------
RDate GGroup::GetUpdated(void) const
{
	return(Updated);
}


//------------------------------------------------------------------------------
RDate GGroup::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
void GGroup::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a group");
	GSession::Event(this,eObjNew);
	Id=id;
}


//------------------------------------------------------------------------------
bool GGroup::IsIn(const GProfile* prof) const
{
	return(RContainer<GProfile,false,true>::IsIn(*prof));
}


//------------------------------------------------------------------------------
void GGroup::DeleteProfile(GProfile* prof)
{
	if(Community)
		prof->SetGroup(0);
	R::RContainer<GProfile,false,true>::DeletePtr(*prof);
//	State=osUpdated;
}


//------------------------------------------------------------------------------
void GGroup::InsertProfile(GProfile* prof)
{
	R::RContainer<GProfile,false,true>::InsertPtr(prof);
//	State=osUpdated;
	if(Community)
		prof->SetGroup(Id);
}


//------------------------------------------------------------------------------
void GGroup::InsertPtr(GProfile* prof)
{
	InsertProfile(prof);
}


//------------------------------------------------------------------------------
void GGroup::DeleteProfiles(void)
{
//	State=osUpdated;
	if(Community)
	{
		RCursor<GProfile> Prof(*this);
		for(Prof.Start();!Prof.End();Prof.Next())
			Prof()->SetGroup(0);
	}
}


//------------------------------------------------------------------------------
RCursor<GProfile> GGroup::GetProfiles(void) const
{
	return(RCursor<GProfile>(*this));
}


//------------------------------------------------------------------------------
RCursor<GProfile> GGroup::GetCursor(void) const
{
	return(RCursor<GProfile>(*this));
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbProfiles(const GSubject* subject) const
{
	unsigned int tot;
	RCursor<GProfile> sub(*this);
	for(sub.Start(),tot=0;!sub.End();sub.Next())
		if(subject->IsIn(sub()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbProfiles(void) const
{
	return(R::RContainer<GProfile,false,true>::GetNb());
}


//------------------------------------------------------------------------------
void GGroup::NotJudgedDocsList(RContainer<GFdbk,false,true>* docs, GProfile* prof) const
{	
	RCursor<GFdbk> Fdbks;
	GFdbk* ptr;
	tDocAssessment j;

	// Clear container.
	docs->Clear();

	RCursor<GProfile> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(Prof()==prof) continue;

		// Go through the assessments of the profile grouped together through this subprofile
		Fdbks=Prof()->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify that it was not assessed by the corresponding profile
			if(prof->GetFdbk(Fdbks()->GetDocId())) continue;

			// Verify if already inserted:
			// If not -> insert it in docs.
			// If yes -> Verify judgement
			ptr=docs->GetPtr<const GFdbk*>(Fdbks());
			if(ptr)
			{
				j=ptr->GetFdbk();
				switch( j & djMaskJudg)
				{
					case djKO:
						if(Fdbks()->GetFdbk() & djOK)
						{
							docs->DeletePtr(ptr);
							docs->InsertPtr(Fdbks());
						}
						break;
					case djOutScope:
						if((Fdbks()->GetFdbk() & djOK)&&(Fdbks()->GetFdbk() & djKO))
						{
							docs->DeletePtr(ptr);
							docs->InsertPtr(Fdbks());
						}
						break;
				}
			}
			else
				docs->InsertPtr(Fdbks());
		}
	}
}


//------------------------------------------------------------------------------
void GGroup::NotJudgedDocsRelList(GMeasure* measure,RContainer<GFdbk,false,false>* docs, GProfile* prof,GSession*) const
{
	RCursor<GFdbk> Fdbks;
	tDocAssessment j;
	RContainer<GFdbkRef,true,false> Docs(50,25);

	if(!measure)
		throw GException("No profiles/documents similarities");

	// Clear container.
	docs->Clear();

	// Go through the subprofiles
	RCursor<GProfile> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		//If current treated profile is the profile "prof" ->Then only add links docs
		if(Prof()==prof)
		{
			// Go through the assessments of the profile grouped together through this profile
			Fdbks=Prof()->GetFdbks();
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				// Verify if the document is a relevant hub or authority.
				j=Fdbks()->GetFdbk();
				if(!( (j & (djOK & djHub)) || (j & (djOK & djAutority)))) continue;

				// Verify if already inserted in Docs.
				if(Docs.GetPtr<const GFdbk*>(Fdbks())) continue;
				// Insert it.
				double res;
				measure->Measure(0,prof->GetId(),Fdbks()->GetDocId(),res);
				Docs.InsertPtr(new GFdbkRef(Fdbks(),res));
			}
			continue;
		}

		// Go through the assessments of the profile grouped together through this subprofile
		Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify if the document is relevant.
			j=Fdbks()->GetFdbk();
			if(!(j & djOK)) continue;

			// Verify if already inserted in Docs or if it was not assessed by the
			// corresponding profile.
			if((Docs.GetPtr<const GFdbk*>(Fdbks()))||(prof->GetFdbk(Fdbks()->GetDocId()))) continue;

			// Insert it.
			double res;
			measure->Measure(0,prof->GetId(),Fdbks()->GetDocId(),&res);
			Docs.InsertPtr(new GFdbkRef(Fdbks(),res));
		}
	}

	// Sort the container by similarity
	if(Docs.GetNb())
		Docs.ReOrder(sortOrder);

	// Copy the result in docs
	RCursor<GFdbkRef> Cur(Docs);
	for(Cur.Start();!Cur.End();Cur.Next())
		docs->InsertPtr(Cur()->Doc);
}


//------------------------------------------------------------------------------
GProfile* GGroup::RelevantProfile(void) const
{
	GProfile* rel;
	RCursor<GProfile> Prof(*this);
	double refsum,sum;

	// Similarities
	GMeasure* ProfilesSims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities");

	// If no objects -> No relevant one.
	if(!R::RContainer<GProfile,false,true>::GetNb())
		return(0);

	// Suppose the first element is the most relevant.
	rel=const_cast<GProfile*>(R::RContainer<GProfile,false,true>::operator[](0));
	refsum=ComputeSumSim(ProfilesSims,rel);

	// Look if in the other objects, there is a better one
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		sum=ComputeSumSim(ProfilesSims,Prof());
		if (sum>=refsum)
		{
			rel=Prof();
			refsum=sum;
		}
	}

	// return most relevant
	return(rel);
}


//------------------------------------------------------------------------------
double GGroup::ComputeSumSim(GMeasure* measure,const GProfile* prof) const
{
	double sum;

	if(!measure)
		throw GException("No profiles similarities");
	RCursor<GProfile> Prof(*this);
	for(Prof.Start(),sum=0.0;!Prof.End();Prof.Next())
	{
		if(Prof()==prof) continue;
		double res;
		measure->Measure(0,prof->GetId(),Prof()->GetId(),&res);
		sum+=(res);
	}
	return(sum);
}


//------------------------------------------------------------------------------
void GGroup::Clear(void)
{
	GWeightInfos::Clear();
}


//------------------------------------------------------------------------------
void GGroup::Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed)
{
	// Remove its references
	if(computed&&Community)
		DelRefs(otGroup);

	// Assign information
	GWeightInfos::Clear();
	if(computed)
	{
		State=osUpdated;
		Computed.SetToday();
	}
	else
		State=osUpToDate;
	CopyInfos(infos);

	// Clear infos
	infos->Clear();

	// Update its references
	if(computed&&Community)
		AddRefs(otGroup);

	// Emit an event that it was modified
	if(computed)
		GSession::Event(this,eObjModified);
}


//------------------------------------------------------------------------------
void GGroup::HasUpdate(GProfile* prof)
{
	if(R::RContainer<GProfile,false,true>::GetPtr(*prof))
		Updated.SetToday();
}


//------------------------------------------------------------------------------
GGroup::~GGroup(void)
{
	GSession::Event(this,eObjDelete);
	try
	{
		if(Community)
		{
			RCursor<GProfile> Prof(*this);
			for(Prof.Start();!Prof.End();Prof.Next())
				Prof()->SetGroup(cNoRef);
			if(State==osDelete)  // The object has modified the references count but was not saved
				DelRefs(otGroup);
		}
	}
	catch(...)
	{
	}
}
