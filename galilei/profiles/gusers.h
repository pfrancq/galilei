


#ifdef GUsersH
#define GUsersH
//---------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gusers.h>
#include <gsessions/gsession.h>
#include <gprofiles/gprofile.h>
#include <gprofiles/gsubprofile.h>

using namespace GALILEI;

//--------------------------------------------------------------------------

// include files for Rainbow
#include <rstd/rcontainer.h>

using namespace Rstd;

//---------------------------------------------------------------------------
// class GUsers
class GUsers : public RStd::RContainer<GUser,unsigned,true,true>
{
public:
	GSession *Session;

	GUsers(unsigned int nb,GSession *session) throw(bad_alloc);
	template<class hprofile> GProfile* GetProfile(unsigned int userid,unsigned int profileid,const char* name);
	GSubProfile* GetSubProfile(unsigned int profileid);
	virtual void Load(void) throw(bad_alloc,GException)=0;
};
