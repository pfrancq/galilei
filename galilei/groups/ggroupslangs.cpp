/*

	GALILEI Research Project

	GGroupsLangs.cpp

	All Groups - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroupslangs.h>
#include<gsessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GGroupLangs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupsLangs::GGroupsLangs(GSession* session) throw(bad_alloc)
	: RContainer<GGroups,unsigned int,true,true>(session->Langs->NbPtr,2)
{
}
