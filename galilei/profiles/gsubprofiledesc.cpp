/*

	GALILEI Research Project

	GSubProfileDesc.cpp

	Generic Sub-Profile Description - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

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
