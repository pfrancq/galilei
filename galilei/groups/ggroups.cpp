/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroups.h>
#include<ggroups/ggroupslangs.h>
#include<gprofiles/gusers.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//  GGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroups::GGroups(GGroupsLangs *owner,GLang *lang) throw(bad_alloc)
	: RContainer<GGroup,unsigned int,true,false>(20,10), Owner(owner), Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GGroups::Compare(const GGroups &groups)
{
	return(Lang->Compare(groups.Lang));
}


//-----------------------------------------------------------------------------
int GGroups::Compare(const GGroups *groups)
{
	return(Lang->Compare(groups->Lang));
}


//-----------------------------------------------------------------------------
void GGroups::Calc(void)
{
	GGroup *grp;
	GSubProfile *sub;
	GUsers *users;
	GUser *user;
	RContainer<GSubProfile,unsigned int,false,false> ToAttach(20,10);
	RContainer<GGroup,unsigned int,false,false> ToDel(20,10);

	// Search all sub profiles to attach for this language
	users=Owner->Session->Users;
	for(users->Start();!users->End();users->Next())	
	{
		user=(*users)();
		for(user->Start();!user->End();user->Next())
		{
			sub=(*user)()->GetPtr<GLang*>(Lang);
			if(sub->bToAttach) ToAttach.InsertPtr(sub);
		}
	}

	// For all subprofiles to attach
	for(ToAttach.Start();!ToAttach.End();ToAttach.Next())
	{
		sub=ToAttach();		
		Start();
		while((!End())&&(!((*this)()->CanAttach(sub))))
			Next();
		if(End())
		{
			grp=new GGroup(this);
			InsertPtr(grp);
		}
		else
			grp=(*this)();
		grp->InsertPtr(sub);
		sub->Attached=time((time_t *)0);		
		sub->bToAttach=false;	
	}

	// Verify that no groups left with no subprofile
	for(Start();!End();Next())
		if((*this)()->NbPtr==0) ToDel.InsertPtr((*this)());
	for(ToDel.Start();!ToDel.End();ToDel.Next())
		DeletePtr(ToDel());
}
