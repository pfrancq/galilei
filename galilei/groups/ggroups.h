

#ifdef GGroupsH
#define GGroupsH
//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroup.h>
#include<ggroups/ggroupslangs.h>
using namespace GALILEI;

//-----------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer>

using namespace Rstd;

//-----------------------------------------------------------------------------
/**
*/
class GGroups : public RStd::RContainer<GGroup,unsigned int,true,false>
{
public:
	GGroupsLangs *Owner;	
	GLang *Lang;

	GGroups(GGroupsLangs *owner,GLang *lang) throw(bad_alloc);
	int Compare(const GGroups &groups);
	int Compare(const GGroups *groups);	
	virtual void Load(void)=0;
	virtual void Save(void)=0;
	virtual GGroup* NewGroup(void)=0;
	void Calc(void);
};
