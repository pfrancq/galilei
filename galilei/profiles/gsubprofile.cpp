/*

	GALILEI Research Project

	GSubProfile.cpp

	Sub-Profile - Implementation.

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
#include<profiles/gsubprofile.h>
#include<profiles/gsubprofiledescvector.h>
#include<profiles/gprofile.h>
#include<langs/glang.h>
#include<infos/giword.h>
#include<infos/giwordlist.h>
#include<profiles/gprofdoc.h>
#include<groups/ggroup.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GSubProfile
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSubProfile::GSubProfile(GProfile *prof,unsigned int id,GLang *lang,GGroup* grp,const char* a) throw(bad_alloc)
  :  RContainer<GSubProfileDesc,unsigned,true,true>(2,1),Id(id), Profile(prof), Lang(lang), Group(grp), Attached(a), OK(0), KO(0), Common(0)
{
	OK=new GIWordList();
	KO=new GIWordList();
	Common=new GIWordList();
	InsertPtr(new GSubProfileDescVector(this,grp,a));
	Profile->InsertPtr(this);
	if(grp)
		grp->InsertPtr(this);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GSubProfile& subprofile) const
{
	return(Id-subprofile.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GSubProfile* subprofile) const
{
	return(Id-subprofile->Id);
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfile::IsDefined(void) const
{
	// The OK and Common lists can't be empty.
	return((OK->NbPtr)||(Common->NbPtr));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::Similarity(const GSubProfile *sub) const
{
	double Sim=0;
	unsigned int NbComp;

	for(OK->Start();!OK->End();OK->Next())
	{
		for(sub->OK->Start();!sub->OK->End();sub->OK->Next())
			if((*OK)()->GetId()==(*sub->OK)()->GetId()) Sim++;
		for(sub->Common->Start();!sub->Common->End();sub->Common->Next())
			if((*OK)()->GetId()==(*sub->Common)()->GetId()) Sim++;
	}
	for(Common->Start();!Common->End();Common->Next())
	{
		for(sub->OK->Start();!sub->OK->End();sub->OK->Next())
			if((*Common)()->GetId()==(*sub->OK)()->GetId()) Sim++;
		for(sub->Common->Start();!sub->Common->End();sub->Common->Next())
			if((*Common)()->GetId()==(*sub->Common)()->GetId()) Sim++;
	}
	NbComp=OK->NbPtr+Common->NbPtr+sub->OK->NbPtr+sub->Common->NbPtr;
	Sim=(2.0*Sim)/static_cast<double>(NbComp);
	return(Sim);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::SetGroup(GGroup* grp)
{
	Group=grp;
	if(grp)
		Attached.SetToday();
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfile::IsUpdated(void) const
{
	return(Attached<(*Profile->GetUpdated()));
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfile::~GSubProfile(void)
{
	if(OK) delete OK;
	if(KO) delete KO;
	if(Common) delete Common;
}
