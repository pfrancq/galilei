

#ifdef GGroupH
#define GGroupH
//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gsubprofiles.h>
#include <ggroups/ggroups.h>
using namespace GALILEI

//-----------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer>
using namespace Rstd;

//-----------------------------------------------------------------------------
/**
* This class represent a group of sub-profiles. In fact, it is implemented as a
* container of GSubProfile.
* @author Pascal Francq
* @short Group class.
*/
class GGroup : public RStd::RContainer<GSubProfile,unsigned int,false,true>
{
public:
	
	unsigned int Id;
	GGroups *Owner;	

	/**
	* Construct a group.
	*/
	GGroup(GGroups *owner) throw(bad_alloc);

	/**
	* Construct a group with a specific identificator.
	* @param id		The identificator.
	*/
	GGroup(GGroups *owner,unsigned int id) throw(bad_alloc);

	int Compare(const GGroup &group);
	int Compare(const GGroup *group);
	virtual void Load(void)=0;
	virtual void Save(void)=0;
	bool CanAttach(GSubProfile *sub);
};


