/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include filess for GALILEI
#include <gprofiles/gprofile.h>
using namespace GALILEI;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
//  GProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfile::GProfile(GUser *owner,const unsigned int id,const char* name,const char* u,const char* a,unsigned int nb,unsigned int nbf) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(nb,nb/2),Owner(owner),Id(id),Name(name),
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
const GSubProfile* GALILEI::GProfile::GetSubProfile(const GLang* lang) const
{
	return(GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::UpdateFinished(void)
{
	State=osUpdated;
	Computed.SetToday();
}
