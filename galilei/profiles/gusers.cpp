

#include <stdlib.h>

//include files for GALILEI
#include<gusers.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  HGsers
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
HGsers::HGsers(unsigned int nb,GSession *session) throw(bad_alloc)
  : RContainer<GUser,unsigned,true,true>(nb+nb/2,nb/2),Session(session)
{
}


//---------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(unsigned int profileid)
{
	GSubProfile *sub;
	GUser *usr;

	for(Start();!End();Next())
	{
		usr=(*this)();
		for(usr->Start();!usr->End();usr->Next())
		{
			sub=(*usr)()->GetPtr<unsigned int>(profileid);
			if(sub) return(sub);
		}
	}
	return(0);
}



