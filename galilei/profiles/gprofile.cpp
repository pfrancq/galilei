/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

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
// include filess for GALILEI
#include <profiles/gprofile.h>
#include <profiles/guser.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroup.h>
using namespace GALILEI;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
//  GProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfile::GProfile(GUser *usr,const unsigned int id,const char* name,const char* u,const char* a,unsigned int nb,unsigned int nbf) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,false,true>(nb,nb/2), User(usr),Id(id),Name(name),
    Fdbks(nbf+nbf/2,nbf/2), Updated(u), Computed(a)
{
	if(Updated>Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
	User->InsertPtr(this);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const unsigned int &id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const GProfile &profile) const
{
  return(Id-profile.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const GProfile *profile) const
{
  return(Id-profile->Id);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GProfile::GetSubProfile(const GLang* lang) const
{
	return(GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::SetState(tObjState state)
{
	GGroup* grp;

	State=state;

	// If the profile was updated -> the groups containing the subprofiles
	// are modified.
	if(State==osUpdated)
	{
		for(Start();!End();Next())
		{
			grp=(*this)()->GetGroup();
			if(grp)
				grp->SetState(osModified);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::UpdateFinished(void)
{
	SetState(osUpdated);
	Computed.SetToday();
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::DocsStart(void)
{
	Fdbks.Start();
}


//-----------------------------------------------------------------------------
bool GALILEI::GProfile::DocsEnd(void) const
{
	return(Fdbks.End());
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::DocsNext(void)
{
	Fdbks.Next();
}


//-----------------------------------------------------------------------------
GProfDoc* GALILEI::GProfile::GetCurDocs(void)
{
	return(Fdbks());
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::AddJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.InsertPtr(j);
}
