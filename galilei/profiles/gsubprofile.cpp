

#include <stdlib.h>


//include files for GALILEI
#include<gsubprofile.h>

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
GSubProfile::GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc)
  : Owner(owner),Id(id),Lang(lang),MOK(0),MKO(0),OK(0),KO(0),Common(0),bToAttach(true)
{
	OK=new GWordList();
	KO=new GWordList();
	Common=new HWordList();
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
	GWordRef *ref;

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
				Common->InsertPtr(new GWordRef(kwdid));
			}
			else
			{
				OK->InsertPtr(new GWordRef(kwdid));
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
				Common->InsertPtr(new GWordRef(kwdid));
			}
			else
			{
				KO->InsertPtr(new GWordRef(kwdid));
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
			if((*OK)()->Id==(*sub->OK)()->Id) Sim++;
		for(sub->Common->Start();!sub->Common->End();sub->Common->Next())
			if((*OK)()->Id==(*sub->Common)()->Id) Sim++;
	}
	for(Common->Start();!Common->End();Common->Next())
	{
		for(sub->OK->Start();!sub->OK->End();sub->OK->Next())
			if((*Common)()->Id==(*sub->OK)()->Id) Sim++;
		for(sub->Common->Start();!sub->Common->End();sub->Common->Next())
			if((*Common)()->Id==(*sub->Common)()->Id) Sim++;
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


