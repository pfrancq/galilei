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



//-----------------------------------------------------------------------------
//
//  GProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfile::GProfile(GUser* owner,const unsigned int nb,const unsigned int nbf) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(nb,nb/2),Owner(owner),Id(cNoRef),Name(),
    Modified(false),FdbkDocs(nbf+nbf/2,nbf/2)
{
}


//-----------------------------------------------------------------------------
GALILEI::GProfile::GProfile(GUser *owner,const unsigned int id,const char* name,const unsigned int nb,const unsigned int nbf=100) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(nb,nb/2),Owner(owner),Id(id),Name(name),
    Modified(false),FdbkDocs(nbf+nbf/2,nbf/2)
{
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
void GALILEI::GProfile::AddDocJudged(GDoc* doc,char j) throw(bad_alloc)
{
	if(doc)
		FdbkDocs.InsertPtr(new GProfDoc(doc,j));
}
