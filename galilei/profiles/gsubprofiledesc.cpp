/*

	GALILEI Research Project

	GSubProfileDesc.cpp

	Generic Sub-Profile Description - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<profiles/gsubprofiledesc.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<groups/ggroup.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileDesc
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSubProfileDesc::GSubProfileDesc(GSubProfile* sub,GGroup* grp,const char* a) throw(bad_alloc)
  : Owner(sub), Group(grp), Attached(a)
{
//	Profile->InsertPtr(this);
//	if(grp)
//		grp->InsertPtr(this);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileDesc::Compare(const GSubProfileDesc& desc) const
{
	return(-1);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileDesc::Compare(const GSubProfileDesc* desc) const
{
	return(-1);
}


//-----------------------------------------------------------------------------
unsigned int  GALILEI::GSubProfileDesc::GetId(void) const
{
	return(Owner->GetId());
}


//-----------------------------------------------------------------------------
GLang*  GALILEI::GSubProfileDesc::GetLang(void) const
{
	return(Owner->GetLang());
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileDesc::IsDefined(void) const
{
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfileDesc::SetGroup(GGroup* grp)
{
	Group=grp;
	if(grp)
		Attached.SetToday();
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfileDesc::IsUpdated(void) const
{
	return(Attached<(*Owner->GetProfile()->GetUpdated()));
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileDesc::~GSubProfileDesc(void)
{
}
