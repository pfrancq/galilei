/*

	GALILEI Research Project

	GSubProfileRef.cpp

	Reference of a SubProfile - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ggroups/gsubprofileref.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileRef
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfileRef::GSubProfileRef(GSubProfile* sub,const char* a)
	: SubProfile(sub), Attached(a)
{
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
