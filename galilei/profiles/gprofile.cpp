

#include <stdlib.h>

//include files for GALILEI
#include<gprofile.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  GProfile
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GProfile::GProfile(GUser *owner) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(10,5),Owner(owner),Id(cNoRef),Name(),
    Modified(false),FdbkDocs(100,50)
{
}


//---------------------------------------------------------------------------
GProfile::GProfile(GUser *owner,unsigned int id,RString &name) throw(bad_alloc)
  : RContainer<HSubProfile,unsigned,true,true>(10,5),Owner(owner),Id(id),Name(name),
    Modified(false),FdbkDocs(100,50)
{
}


//---------------------------------------------------------------------------
int GProfile::Compare(const unsigned int &id)
{
	return(Id-id);
}


//---------------------------------------------------------------------------
int GProfile::Compare(const GProfile &profile)
{
  return(Id-profile.Id);
}


//---------------------------------------------------------------------------
int GProfile::Compare(const GProfile *profile)
{
  return(Id-profile->Id);
}


//---------------------------------------------------------------------------
void GProfile::Calc(void)
{
	GSubProfile *sub,**subs;
	unsigned int i;

	for(FdbkDocs.Start();!FdbkDocs.End();FdbkDocs.Next())
		if(!FdbkDocs()->Doc->Calc)
		{
      	sub=GetPtr<GLang*>(FdbkDocs()->Doc->Lang,false);
			if(sub)
				sub->Analyse(FdbkDocs());
		}
	for(i=NbPtr+1,subs=Tab;--i;subs++)
		(*subs)->Analyse();
	Modified=true;
}


