/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <docs/gdoc.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfDocRef
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GProfDocRef
{
public:
	GProfDoc* Doc;
	double Sim;

	GProfDocRef(GProfDoc* d,double s) : Doc(d), Sim(s) {}
	int Compare(const GProfDocRef*) const {return(-1);}
	int Compare(const GProfDocRef&) const {return(-1);}
	int Compare(const GProfDoc* d) const
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
GGroup::GGroup(unsigned int id,GLang* lang,bool com) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10), Id(id),
	  State(osUpToDate), Lang(lang), Community(com)
{
}


//------------------------------------------------------------------------------
GGroup::GGroup(GLang* lang,bool com) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10), Id(cNoRef),
	  State(osCreated), Lang(lang), Community(com)
{
}


//------------------------------------------------------------------------------
int GGroup::sortOrder(const void *a,const void *b)
{
	double af=(*((GProfDocRef**)(a)))->Sim;
	double bf=(*((GProfDocRef**)(b)))->Sim;

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
	return(false);
}


//------------------------------------------------------------------------------
bool GGroup::IsEmpty(void) const
{
	return(NbPtr==0);
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
	return(R::RContainer<GSubProfile,unsigned int,false,true>::IsIn(sp));
}


//------------------------------------------------------------------------------
void GGroup::DeleteSubProfile(GSubProfile* sp) throw(bad_alloc)
{
	if(Community)
		sp->SetGroup(0);
	DeletePtr(sp);
	State=osUpdated;
}


//------------------------------------------------------------------------------
void GGroup::InsertSubProfile(GSubProfile* sp) throw(bad_alloc)
{
	InsertPtr(sp);
	State=osUpdated;
	if(Community)
		sp->SetGroup(this);
}


//------------------------------------------------------------------------------
void GGroup::InsertPtr(GSubProfile* sp) throw(bad_alloc)
{
	InsertSubProfile(sp);
}


//------------------------------------------------------------------------------
void GGroup::DeleteSubProfiles(void) throw(bad_alloc)
{
	GSubProfileCursor Sub;

	State=osUpdated;
	if(Community)
	{
		Sub.Set(this);
		for(Sub();!Sub();Sub())
			Sub()->SetGroup(0);
	}
	Clear();
}


//------------------------------------------------------------------------------
GSubProfileCursor& GGroup::GetSubProfilesCursor(void)
{
	GSubProfileCursor* cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
GSubProfileCursor& GGroup::GetCursor(void)
{
	return(GetSubProfilesCursor());
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbSubProfiles(const GGroup* grp) const
{
	unsigned int tot=0;
	GSubProfile** ptr;
	unsigned int i;

	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		if(grp->IsIn(*ptr))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
unsigned int GGroup::GetNbSubProfiles(void) const
{
	return(NbPtr);
}


//------------------------------------------------------------------------------
void GGroup::NotJudgedDocsList(RContainer<GProfDoc,unsigned,false,true>* docs, GSubProfile* s) const throw(bad_alloc)
{
	GSubProfile** tab;
	unsigned int i;
	GProfDocCursor Fdbks;
	GProfDoc* ptr;
	tDocAssessment j;

	// Clear container.
	docs->Clear();

	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)==s) continue;

		// Go through the judgments
		Fdbks=(*tab)->GetProfDocCursor();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify that it was not judged by s
			if(s->GetFeedback(Fdbks()->GetDoc())) continue;

			// Verify if already inserted:
			// If not -> insert it in docs.
			// If yes -> Verify judgement
			ptr=docs->GetPtr<const GProfDoc*>(Fdbks());
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
void GGroup::NotJudgedDocsRelList(RContainer<GProfDoc,unsigned,false,false>* docs, GSubProfile* s,bool global) const throw(bad_alloc)
{
	GSubProfile** tab;
	unsigned int i;
	GProfDocCursor Fdbks;
	tDocAssessment j;
	RContainer<GProfDocRef,unsigned int,true,false> Docs(50,25);

	// Clear container.
	docs->Clear();

	// Go through the subprofiles
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)==s) continue;

		// Go through the judgments
		Fdbks=(*tab)->GetProfDocCursor();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify if already inserted in Docs or if it was not judged by the
			// subprofile s
			if((Docs.GetPtr<const GProfDoc*>(Fdbks()))||(s->GetFeedback(Fdbks()->GetDoc()))) continue;

			// If not -> insert it in docs if relevant.
			j=Fdbks()->GetFdbk();
			if(j & djOK)
			{
				if(global)
					Docs.InsertPtr(new GProfDocRef(Fdbks(),s->SimilarityIFF(Fdbks()->GetDoc())));
				else
					Docs.InsertPtr(new GProfDocRef(Fdbks(),s->Similarity(Fdbks()->GetDoc())));
			}
		}
	}

	// Sort the container by similarity
	if(NbPtr)
		qsort(static_cast<void*>(Docs.Tab),Docs.NbPtr,sizeof(GProfDocRef*),sortOrder);

	// Copy the result in docs
	for(Docs.Start();!Docs.End();Docs.Next())
		docs->InsertPtr(Docs()->Doc);
}


//------------------------------------------------------------------------------
GSubProfile* GGroup::RelevantSubProfile(bool iff) const
{
	GSubProfile* rel;
	GSubProfile** sub;
	unsigned int i;
	double refsum,sum;

	// If no objects -> No relevant one.
	if(!NbPtr) return(0);

	// Suppose the first element is the most relevant.
	sub=Tab;
	rel=(*Tab);
	refsum=ComputeSumSim(rel,iff);

	// Look if in the other objects, there is a better one
	for(i=NbPtr,sub++;--i;sub++)
	{
		sum=ComputeSumSim(*sub,iff);
		if (sum>=refsum)
		{
			rel=(*sub);
			refsum=sum;
		}
	}

	// return most relevant
	return(rel);
}


//------------------------------------------------------------------------------
double GGroup::ComputeSumSim(const GSubProfile* s,bool iff) const
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned int i;

	for(sub1=Tab,i=NbPtr+1;--i;sub1++)
	{
		if((*sub1)==s) continue;
		if(iff)
			sum=sum+s->SimilarityIFF((*sub1));
		else
			sum=sum+s->Similarity((*sub1));
	}
	return(sum);
}


//------------------------------------------------------------------------------
void GGroup::AddInfo(GWeightInfo*) throw(bad_alloc)
{
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GDoc*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GDoc*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GSubProfile*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GGroup::Similarity(const GGroup*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GGroup::SimilarityIFF(const GGroup*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
GGroup::~GGroup(void) throw(GException)
{
	GSubProfileCursor Sub;

	if(Community)
	{
		Sub.Set(this);
		for(Sub();!Sub();Sub())
			Sub()->SetGroup(0);
	}
}
