
#ifdef GProfileH
#define GProfileH
//---------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>

using namespace GALILEI ;
//---------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace Rstd;
//---------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>

//---------------------------------------------------------------------------
// class GProfile
class GProfile : public RStd::RContainer<GSubProfile,unsigned,true,true>
{
public:

	/**
	* The owner of the profile.
	*/
	GUser *Owner;

	unsigned Id;       		// Identifier
	RString Name;           // Name
	bool Modified;          // Profile modified since last analyse?
	RContainer<GProfDoc,unsigned,true,true> FdbkDocs; // Documents juged by profile
	time_t	Updated;			// Date of updated

	GProfile(GUser *owner) throw(bad_alloc);
	GProfile(GUser *owner,unsigned int id,RString &name) throw(bad_alloc);
	int Compare(const unsigned int &id);
	int Compare(const GProfile &profile);
	int Compare(const GProfile *profile);
	void Calc(void);
	virtual void Load(void) throw(bad_alloc,GException)=0;
	virtual void Save(void) throw(bad_alloc,GException)=0;
};
