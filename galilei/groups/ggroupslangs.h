

#ifdef GGroupsLangsH
#define GGroupsLangsH
//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroups/ggroups.h>
#include <gsessions/gsession/h>

using namespace GALILEI;

//-----------------------------------------------------------------------------
// include files for Rainbw
#include <rstd/rcontainer>

using namespace Rstd;
//-----------------------------------------------------------------------------
/**
*/
class GGroupsLangs : public RStd::RContainer<GGroups,unsigned int,true,true>
{
public:
	
	GSession* Session;

	GGroupsLangs(GSession *session) throw(bad_alloc);
	virtual void Load(void);
	void Calc(void);
	virtual void Save(void);
};
