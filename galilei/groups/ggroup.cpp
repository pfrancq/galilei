/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#include <groups/ggroup.h>
#include <docs/gdocproxy.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
#include <infos/gweightinfo.h>
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
		return(Doc->GetDoc()->GetId()-d->GetDoc()->GetId());
	}
};



//------------------------------------------------------------------------------
//
//  GGroup
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroup::GGroup(unsigned int id,GLang* lang,bool com) throw(std::bad_alloc)
	: RContainer<GSubProfile,false,true>(20,10), GWeightInfos(60), Id(id),
	  State(osUpToDate), Lang(lang), Community(com)
{
}


//------------------------------------------------------------------------------
GGroup::GGroup(GLang* lang,bool com) throw(std::bad_alloc)
	: RContainer<GSubProfile,false,true>(20,10), GWeightInfos(60), Id(cNoRef),
	  State(osCreated), Lang(lang), Community(com)
{
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
	return(Id-grp->Id);
}


//------------------------------------------------------------------------------
int GGroup::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
bool GGroup::IsDefined(void) const
{
	return(!GWeightInfos::IsEmpty());
}


//------------------------------------------------------------------------------
bool GGroup::IsEmpty(void) const
{
	return(GWeightInfos::IsEmpty());
}


//------------------------------------------------------------------------------
void GGroup::SetId(unsigned int id) throw(GException)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a group");
	Id=id;
}


//------------------------------------------------------------------------------
void GGroup::SetState(tObjState state)
{
	State=state;
}


//------------------------------------------------------------------------------
bool GGroup::IsIn(const GSubProfile* sp) const
{
	return(R::RContainer<GSubProfile,false,true>::IsIn(sp));
}


//------------------------------------------------------------------------------
void GGroup::DeleteSubProfile(GSubProfile* sp) throw(std::bad_alloc)
{
	if(Community)
		sp->SetGroup(0);
	R::RContainer<GSubProfile,false,true>::DeletePtr(sp);
	State=osUpdated;
}


//------------------------------------------------------------------------------
void GGroup::InsertSubProfile(GSubProfile* sp) throw(std::bad_alloc)
{
	R::RContainer<GSubProfile,false,true>::InsertPtr(sp);
	State=osUpdated;
	if(Community)
		sp->SetGroup(this);
}


//------------------------------------------------------------------------------
void GGroup::InsertPtr(GSubProfile* sp) throw(std::bad_alloc)
{
	InsertSubProfile(sp);
}


//------------------------------------------------------------------------------
void GGroup::DeleteSubProfiles(void) throw(std::bad_alloc)
{
	GSubProfileCursor Sub;

	State=osUpdated;
	if(Community)
	{
		Sub.Set(this);
		for(Sub.Start();!Sub.End();Sub.Next())
			Sub()->SetGroup(0);
	}
	Clear();
}


//------------------------------------------------------------------------------
GSubProfileCursor GGroup::GetSubProfilesCursor(void)
{
	GSubProfileCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GSubProfileCursor GGroup::GetCursor(void)
{
	return(GetSubProfilesCursor());
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbSubProfiles(const GGroup* grp) const
{
	unsigned int tot;
	GSubProfileCursor sub(this);

	for(sub.Start(),tot=0;!sub.End();sub.Next())
		if(grp->IsIn(sub()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbSubProfiles(void) const
{
	return(R::RContainer<GSubProfile,false,true>::GetNb());
}


//------------------------------------------------------------------------------
void GGroup::NotJudgedDocsList(RContainer<GFdbk,false,true>* docs, GSubProfile* s) const throw(std::bad_alloc)
{
	GSubProfileCursor sub(this);
	RCursor<GFdbk> Fdbks;
	GFdbk* ptr;
	tDocAssessment j;

	// Clear container.
	docs->Clear();

	for(sub.Start();!sub.End();sub.Next())
	{
		if(sub()==s) continue;

		// Go through the judgments
		Fdbks=sub()->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify that it was not judged by s
			if(s->GetProfile()->GetFdbk(Fdbks()->GetDoc()->GetId())) continue;

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
void GGroup::NotJudgedDocsRelList(RContainer<GFdbk,false,false>* docs, GSubProfile* s,GSession* session) const throw(std::bad_alloc)
{
	GSubProfileCursor sub(this);
	RCursor<GFdbk> Fdbks;
	tDocAssessment j;
	RContainer<GFdbkRef,true,false> Docs(50,25);

	// Clear container.
	docs->Clear();

	// Go through the subprofiles
	for(sub.Start();!sub.End();sub.Next())
	{
		//If current treated subprofile is the subprofile "s" ->Then only add links docs
		if(sub()==s)
		{
			// Go through the judgments
			Fdbks=sub()->GetFdbks();
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				// Verify if the document is a relevant hub or authority.
				j=Fdbks()->GetFdbk();
				if(!( (j & (djOK & djHub)) || (j & (djOK & djAutority)))) continue;

				// Verify if already inserted in Docs.
				if(Docs.GetPtr<const GFdbk*>(Fdbks())) continue;
				// Insert it.
				Docs.InsertPtr(new GFdbkRef(Fdbks(),session->GetSimDocProf(Fdbks()->GetDoc()->GetId(),s->GetId())));
			}
			continue;
		}

		// Go through the judgments
		Fdbks=sub()->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify if the document is relevant.
			j=Fdbks()->GetFdbk();
			if(!(j & djOK)) continue;

			// Verify if already inserted in Docs or if it was not judged by the
			// subprofile s.
			if((Docs.GetPtr<const GFdbk*>(Fdbks()))||(s->GetProfile()->GetFdbk(Fdbks()->GetDoc()->GetId()))) continue;

			// Insert it.
			Docs.InsertPtr(new GFdbkRef(Fdbks(),session->GetSimDocProf(Fdbks()->GetDoc()->GetId(),s->GetId())));
		}
	}

	// Sort the container by similarity
	if(Docs.NbPtr)
		qsort(static_cast<void*>(Docs.Tab),Docs.NbPtr,sizeof(GFdbkRef*),sortOrder);

	// Copy the result in docs
	for(Docs.Start();!Docs.End();Docs.Next())
		docs->InsertPtr(Docs()->Doc);
}


//------------------------------------------------------------------------------
GSubProfile* GGroup::RelevantSubProfile(bool iff) const
{
	GSubProfile* rel;
	GSubProfileCursor sub(this);
	double refsum,sum;

	// If no objects -> No relevant one.
	if(!R::RContainer<GSubProfile,false,true>::GetNb())
		return(0);

	// Suppose the first element is the most relevant.
	rel=RContainer<GSubProfile,false,true>::GetPtrAt(0);
	refsum=ComputeSumSim(rel,iff);

	// Look if in the other objects, there is a better one
	for(sub.Start();!sub.End();sub.Next())
	{
		sum=ComputeSumSim(sub(),iff);
		if (sum>=refsum)
		{
			rel=sub();
			refsum=sum;
		}
	}

	// return most relevant
	return(rel);
}


//------------------------------------------------------------------------------
double GGroup::ComputeSumSim(const GSubProfile* s,bool iff) const
{
	double sum;
	GSubProfileCursor sub(this);

	for(sub.Start(),sum=0.0;!sub.End();sub.Next())
	{
		if(sub()==s) continue;
		if(iff)
			sum=sum+s->SimilarityIFF(sub());
		else
			sum=sum+s->Similarity(sub());
	}
	return(sum);
}


//------------------------------------------------------------------------------
GWeightInfoCursor GGroup::GetWeightInfoCursor(void)
{
	GWeightInfoCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GGroup* desc) const
{
	return(GWeightInfos::Similarity(desc));
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GGroup* desc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(desc,otGroup,Lang));
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(doc));
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(doc,otDocGroup,Lang));
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GSubProfile* doc) const
{
	return(GWeightInfos::Similarity(doc));
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GSubProfile* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(doc,otSubProfileGroup,Lang));
}


//------------------------------------------------------------------------------
void GGroup::UpdateRefs(void) const throw(GException)
{
	if(Community)
		AddRefs(otGroup,Lang);
}


//------------------------------------------------------------------------------
void GGroup::RemoveRefs(void) const throw(GException)
{
	if(Community)
		DelRefs(otGroup,Lang);
}


//------------------------------------------------------------------------------
void GGroup::Clear(void)
{
	GWeightInfos::Clear();
}


//------------------------------------------------------------------------------
GGroup::~GGroup(void)
{
	try
	{
		if(Community)
		{
			GSubProfileCursor Sub;
			Sub.Set(this);
			for(Sub.Start();!Sub.End();Sub.Next())
				Sub()->SetGroup(0);
		}
		RemoveRefs();
	}
	catch(...)
	{
	}
}
