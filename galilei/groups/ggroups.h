/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GGroupsH
#define GGroupsH


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroup.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GGroupsLangs;


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
	//virtual void Load(void)=0;
	//virtual void Save(void)=0;
	//virtual GGroup* NewGroup(void)=0;
	void Calc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
