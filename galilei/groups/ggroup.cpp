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
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(void) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(cNoRef)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroup::GGroup(const unsigned int id) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(id)
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
bool GALILEI::GGroup::CanAttach(GSubProfile *sub)
{
	for(Start();!End();Next())
		if((*this)()->Similar(sub))
			return(true);
	return(false);
}
