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
#include<gprofiles/gusers.h>
#include<gsessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//  GGroups
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
GGroup* GALILEI::GGroups::GetGroup(const GSubProfile* sub) const
{
	RContainerCursor<GGroup,unsigned int,true,false> cur(this);

	for(cur.Start();!cur.End();cur.Next())
	{
		if(cur()->GetPtr(sub))
			return(cur());
	}
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroups::Calc(void)
{
//	GGroup *grp;
//	GSubProfile *sub;
//	GUsers *users;
//	GUser *user;
//	RContainer<GSubProfile,unsigned int,false,false> ToAttach(20,10);
//	RContainer<GGroup,unsigned int,false,false> ToDel(20,10);
//
//	// Search all sub profiles to attach for this language
//	users=Owner->Session->Users;
//	for(users->Start();!users->End();users->Next())	
//	{
//		user=(*users)();
//		for(user->Start();!user->End();user->Next())
//		{
//			sub=(*user)()->GetPtr<GLang*>(Lang);
//			if(sub->bToAttach) ToAttach.InsertPtr(sub);
//		}
//	}
//
//	// For all subprofiles to attach
//	for(ToAttach.Start();!ToAttach.End();ToAttach.Next())
//	{
//		sub=ToAttach();		
//		Start();
//		while((!End())&&(!((*this)()->CanAttach(sub))))
//			Next();
//		if(End())
//		{
//			grp=new GGroup(this);
//			InsertPtr(grp);
//		}
//		else
//			grp=(*this)();
//		grp->InsertPtr(sub);
//		sub->Attached=time((time_t *)0);		
//		sub->bToAttach=false;	
//	}
//
//	// Verify that no groups left with no subprofile
//	for(Start();!End();Next())
//		if((*this)()->NbPtr==0) ToDel.InsertPtr((*this)());
//	for(ToDel.Start();!ToDel.End();ToDel.Next())
//		DeletePtr(ToDel());
}
