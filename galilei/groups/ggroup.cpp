/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	(C) 2001 by P. Francq.

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
//  GGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(const unsigned int id,GLang* lang) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10), Id(id),
	  State(osUpToDate), Lang(lang)
{
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
void GALILEI::GGroup::NotJudgedDocsRelList(RStd::RContainer<GProfDoc,unsigned,false,false>* docs, GSubProfile* s)
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
			if(ptr) continue;
			j=Fdbks()->GetFdbk();
			if((j==djNav)||(j==djOK))
				docs->InsertPtr(Fdbks());
		}
	}
}
