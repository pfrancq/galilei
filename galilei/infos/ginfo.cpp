/*

	GALILEI Research Project

	GInfo.cpp

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/ginfo.h>



//-----------------------------------------------------------------------------
//
// GInfo
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GInfo::GInfo(void)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GInfo::Compare(const GInfo* i) const
{
	return(this!=i);
}


//-----------------------------------------------------------------------------
float GALILEI::GInfo::Similarity(const GInfo* i) const
{
	return(this==i);
}


//-----------------------------------------------------------------------------
float GALILEI::GInfo::DisSimilarity(const GInfo* i) const
{
	return(this!=i);
}


//-----------------------------------------------------------------------------
const RString GALILEI::GInfo::ClassName(void) const
{
	return("GInfo");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GInfo::InfoType(void) const
{
	return(infoNothing);
}


//-----------------------------------------------------------------------------
bool GALILEI::GInfo::SameClass(const GInfo* i) const
{
	return(InfoType()==i->InfoType());
}


//-----------------------------------------------------------------------------
GALILEI::GInfo::~GInfo(void)
{
}
