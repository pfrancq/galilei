

#include <stdlib.h>
//include files for GALILEI
#include<ggroup.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------
//
//  GGroup
//
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
GGroup::GGroup(GGroups *owner) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(0),Owner(owner)
{
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
GGroup::GGroup(GGroups *owner,unsigned int id) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,false,true>(20,10),Id(id),Owner(owner)
{
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
int GGroup::Compare(const GGroup &group)
{
	return(Id-group.Id);
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
int GGroup::Compare(const GGroup *group)
{
	return(Id-group->Id);
}


/*-----------------------------------------------------------------------------
 *
 * Parameters:
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
bool GGroup::CanAttach(GSubProfile *sub)
{
	for(Start();!End();Next())
		if((*this)()->Similar(sub))
			return(true);
	return(false);
}



