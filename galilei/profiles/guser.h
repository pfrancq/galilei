


#ifdef GUsersH
#defineGUSersH
//---------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gusers.h>
#include <gprofiles/gprofile.h>
using namespace GALILEI;


//---------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace Rstd;
//---------------------------------------------------------------------------
// class GUser
class GUser : public RStd::RContainer<GProfile,unsigned,true,true>
{
public:

	GUsers *Owner;
	unsigned Id;         // Identifier
	RString Name;        // Name
	unsigned SessNum;    // Number of session

	GUser(GUsers *owner) throw(bad_alloc);
	int Compare(const unsigned int id);
	int Compare(const GUser &user);
	int Compare(const GUser *user);
	virtual void Load(void) throw(bad_alloc,GException)=0;
};



