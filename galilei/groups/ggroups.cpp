/*

	GALILEI Research Project

	GGroups.cpp

	Groups for a given language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroups.h>
#include <glangs/glang.h>
#include <ggroups/ggroup.h>
#include<gprofiles/gsubprofile.h>
#include<gprofiles/gusers.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroups::GGroups(GLang* lang) throw(bad_alloc)
	: RContainer<GGroup,unsigned int,true,false>(20,10), Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GGroups& groups) const
{
	return(Lang->Compare(groups.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GGroups* groups) const
{
	return(Lang->Compare(groups->Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
GGroupCursor& GALILEI::GGroups::GetGroupCursor(void)
{
	GGroupCursor *cur=GGroupCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroups::GetGroup(const GSubProfile* sub) const
{
	RContainerCursor<GGroup,unsigned int,true,false> cur(this);

	for(cur.Start();!cur.End();cur.Next())
	{
		if(cur()->IsIn<const unsigned int>(sub->GetId()))
			return(cur());
	}
	return(0);
}
