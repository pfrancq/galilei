/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/

#include <stdlib.h>


//include files for GALILEI
#include<gprofiles/gsubprofile.h>
#include <gprofiles/gprofile.h>
#include <gprofiles/gusers.h>
using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  GSubProfile
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GSubProfile::GSubProfile(void) throw(bad_alloc)
  : Id(cNoRef),Lang(0),MOK(0),MKO(0),OK(0),KO(0),Common(0),bToAttach(true)
{
	OK=new GWordList();
	KO=new GWordList();
	Common=new GWordList();
}


//---------------------------------------------------------------------------
bool GSubProfile::NeedOK(void)
{
	return((NbOK<30)&&(NbOK<MOK->NbPtr));
}


//---------------------------------------------------------------------------
bool GSubProfile::NeedKO(void)
{
	return((NbKO<30)&&(NbOK<MKO->NbPtr));
}

//---------------------------------------------------------------------------
GSubProfile::GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc)
  : Owner(owner),Id(id),Lang(lang),MOK(0),MKO(0),OK(0),KO(0),Common(0),bToAttach(true)
{
	OK=new GWordList();
	KO=new GWordList();
	Common=new GWordList();
	MOK=new GWordCalcs(Lang,Owner->Owner->Owner->Session);
	MKO=new GWordCalcs(Lang,Owner->Owner->Owner->Session);
}


//---------------------------------------------------------------------------
int GSubProfile::Compare(const unsigned int id)
{
	return(Id-id);
}


//---------------------------------------------------------------------------
int GSubProfile::Compare(const GLang* lang)
{
	return(Lang->Compare(lang));
}


//---------------------------------------------------------------------------
int GSubProfile::Compare(const GSubProfile &subprofile)
{
  return(Id-subprofile.Id);
}


//---------------------------------------------------------------------------
int GSubProfile::Compare(const GSubProfile *subprofile)
{
  return(Id-subprofile->Id);
}


//---------------------------------------------------------------------------
void GSubProfile::Analyse(GProfDoc *profdoc)
{
	switch(profdoc->Fdbk)
	{
		case 'O':
		case 'N':
			MOK->Analyse(profdoc->Doc);
			break;

		case 'K':
			MKO->Analyse(profdoc->Doc);
			break;
	}
}


//---------------------------------------------------------------------------
void GSubProfile::Analyse(void)
{
	unsigned int kwdid;
	GIWord *ref;

	MOK->EndCalc();
	MKO->EndCalc();

	NbOK=0;
	NbKO=0;
	while(NeedOK()||NeedKO())
	{
		if(NeedOK())
		{
			kwdid=MOK->NextWord();
			ref=KO->GetPtr<unsigned int>(kwdid);
			if(ref)
			{
				KO->DeletePtr(ref);
				NbKO--;
				Common->InsertPtr(new GIWord(kwdid));
			}
			else
			{
				OK->InsertPtr(new GIWord(kwdid));
				NbOK++;
			}
		}
		if(NeedKO())
		{
			kwdid=MKO->NextWord();
			ref=OK->GetPtr<unsigned int>(kwdid);
			if(ref)
			{
				OK->DeletePtr(ref);
				NbOK--;
				Common->InsertPtr(new GIWord(kwdid));
			}
			else
			{
				KO->InsertPtr(new GIWord(kwdid));
				NbKO++;
			}
		}
	}
}


//---------------------------------------------------------------------------
bool GSubProfile::Similar(GSubProfile *sub)
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
	return(Sim>=0.5);
}


//---------------------------------------------------------------------------
GSubProfile::~GSubProfile(void)
{
	if(MOK) delete MOK;
	if(MKO) delete MKO;
	if(OK) delete OK;
	if(KO) delete KO;
	if(Common) delete Common;
}


