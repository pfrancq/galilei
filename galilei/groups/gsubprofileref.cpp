/*

	GALILEI Research Project

	GSubProfileRef.cpp

	Reference of a SubProfile - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroups/gsubprofileref.h>
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofile.h>
#include <ggroups/ggroup.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileRef
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfileRef::GSubProfileRef(GSubProfile* sub,GGroup* grp,const char* a)
	: SubProfile(sub), Group(grp), Attached(a)
{
	grp->InsertPtr(this);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileRef::GSubProfileRef(GSubProfile* sub)
	: SubProfile(sub), Attached()
{
	Attached.SetToday();
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileRef::Compare(const unsigned int id) const
{
	return(SubProfile->GetId()-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileRef::Compare(const GSubProfileRef& gp) const
{
	return(SubProfile->GetId()-gp.SubProfile->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileRef::Compare(const GSubProfileRef* gp) const
{
	return(SubProfile->GetId()-gp->SubProfile->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileRef::Compare(const GSubProfile* s) const
{
	return(SubProfile->GetId()-s->GetId());
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfileRef::GetId(void) const
{
	return(SubProfile->GetId());
}


//-----------------------------------------------------------------------------
GProfile* GALILEI::GSubProfileRef::GetProfile(void) const
{
	return(SubProfile->GetProfile());
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileRef::IsUpdated(void) const
{
	return(Attached<(*SubProfile->GetProfile()->GetUpdated()));
}
