/*

	GALILEI Research Project

	GGroup.cpp

	Group - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ggroups/ggroup.h>
#include <gprofiles/gsubprofile.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(GLang* lang) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,false>(20,10), Id(cNoRef),
	  State(osCreated), Lang(lang)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(const unsigned int id,GLang* lang) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,false>(20,10), Id(id),
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
}
