/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <groups/ggroup.h>
#include <docs/gdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GProfDocRef
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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



//-----------------------------------------------------------------------------
//
//  GGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(const unsigned int id,GLang* lang) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10), Id(id),
	  State(osUpToDate), Lang(lang)
{
}


//--------------------------------------------------------------------------
int GALILEI::GGroup::sortOrder(const void *a,const void *b)
{
	double af=(*((GProfDocRef**)(a)))->Sim;
	double bf=(*((GProfDocRef**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroup::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroup::Compare(const GGroup& group) const
{
	return(Id-group.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroup::Compare(const GGroup* group) const
{
	return(Id-group->Id);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroup::DeleteSubProfile(GSubProfile* sp)
{
	DeletePtr(sp);
	State=osUpdated;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroup::InsertSubProfile(GSubProfile* sp)
{
	InsertPtr(sp);
	State=osUpdated;
	sp->SetGroup(this);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroup::DeleteSubProfiles(void)
{
	State=osUpdated;
	for(Start();!End();Next())
		(*this)()->SetGroup(0);
	Clear();
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GGroup::GetSubProfileCursor(void)
{
	GSubProfileCursor* cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroup::GetNbSubProfiles(GGroup* grp)
{
	unsigned int tot=0;
	GSubProfileCursor Sub;

	Sub.Set(this);
	for(Sub.Start();!Sub.End();Sub.Next())
		if(grp->IsIn<const GSubProfile*>(Sub()))
			tot++;
	return(tot);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroup::NotJudgedDocsList(RStd::RContainer<GProfDoc,unsigned,false,true>* docs, GSubProfile* s)
{
	GSubProfile** tab;
	unsigned int i;
	GProfDocCursor Fdbks;
	GProfDoc* ptr;
	tDocJudgement j;

	// Clear container.
	docs->Clear();

	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)==s) continue;

		// Go through the judgments
		Fdbks=(*tab)->GetProfile()->GetProfDocCursor();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Must be the same language than the group.
			if(Fdbks()->GetDoc()->GetLang()!=Lang) continue;

			// Verify that it was not judged by s
			if(s->GetProfile()->GetFeedback(Fdbks()->GetDoc())) continue;

			// Verify if already inserted:
			// If not -> insert it in docs.
			// If yes -> Verify judgement
			ptr=docs->GetPtr<const GProfDoc*>(Fdbks());
			if(ptr)
			{
				j=ptr->GetFdbk();
				switch(j)
				{
					case djNav:
						if(Fdbks()->GetFdbk()==djOK)
						{
							docs->DeletePtr(ptr);
							docs->InsertPtr(Fdbks());
						}
						break;
					case djKO:
						if((Fdbks()->GetFdbk()==djOK)&&(Fdbks()->GetFdbk()==djNav))
						{
							docs->DeletePtr(ptr);
							docs->InsertPtr(Fdbks());
						}
						break;
					case djOutScope:
						if((Fdbks()->GetFdbk()==djOK)&&(Fdbks()->GetFdbk()==djNav)&&(Fdbks()->GetFdbk()==djKO))
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


//-----------------------------------------------------------------------------
void GALILEI::GGroup::NotJudgedDocsRelList(RStd::RContainer<GProfDoc,unsigned,false,false>* docs, GSubProfile* s,bool global)
{
	GSubProfile** tab;
	unsigned int i;
	GProfDocCursor Fdbks;
	tDocJudgement j;
	RContainer<GProfDocRef,unsigned int,true,false> Docs(50,25);

	// Clear container.
	docs->Clear();

	// Go through the subprofiles
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)==s) continue;

		// Go through the judgments
		Fdbks=(*tab)->GetProfile()->GetProfDocCursor();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Must be the same language than the group.
			if(Fdbks()->GetDoc()->GetLang()!=Lang) continue;

			// Verify if already inserted in Docs or if it was not judged by the
			// subprofile s
			if((Docs.GetPtr<const GProfDoc*>(Fdbks()))||(s->GetProfile()->GetFeedback(Fdbks()->GetDoc()))) continue;

			// If not -> insert it in docs if relevant.
			j=Fdbks()->GetFdbk();
			if((j==djNav)||(j==djOK))
			{
				if(global)
					Docs.InsertPtr(new GProfDocRef(Fdbks(),s->GlobalSimilarity(Fdbks()->GetDoc())));
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


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GGroup::RelevantSubProfile(bool g) const
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
	refsum=ComputeSumSim(rel,g);

	// Look if in the other objects, there is a better one
	for(i=NbPtr,sub++;--i;sub++)
	{
		sum=ComputeSumSim(*sub,g);
		if (sum>=refsum)
		{
			rel=(*sub);
			refsum=sum;
		}
	}

	// return most relevant
	return(rel);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::ComputeSumSim(const GSubProfile* s,bool g) const
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned int i;

	for(sub1=Tab,i=NbPtr+1;--i;sub1++)
	{
		if((*sub1)==s) continue;
		if(g)
			sum=sum+s->GlobalSimilarity((*sub1));
		else
			sum=sum+s->Similarity((*sub1));
	}
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::Similarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::GlobalSimilarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::GlobalSimilarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::Similarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroup::GlobalSimilarity(const GGroup*) const
{
	return(0.0);
}
