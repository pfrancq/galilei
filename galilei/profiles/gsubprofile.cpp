/*

	GALILEI Research Project

	GSubProfile.cpp

	Sub-Profile - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<gprofiles/gsubprofile.h>
#include <gprofiles/gprofile.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfile::GSubProfile(void) throw(bad_alloc)
  : Id(cNoRef),Lang(0),OK(0),KO(0),Common(0)
{
	OK=new GIWordList();
	KO=new GIWordList();
	Common=new GIWordList();
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfile::GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc)
  : Owner(owner),Id(id),Lang(lang),OK(0),KO(0),Common(0)
{
	OK=new GIWordList();
	KO=new GIWordList();
	Common=new GIWordList();
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
	return(Lang->Compare(subprofile.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GSubProfile* subprofile) const
{
	return(Lang->Compare(subprofile->Lang));
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
	Sim=(4.0*Sim)/static_cast<double>(NbComp);
	return(Sim);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfile::~GSubProfile(void)
{
	if(OK) delete OK;
	if(KO) delete KO;
	if(Common) delete Common;
}


