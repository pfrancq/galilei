/*

	GALILEI Research Project

	GIWordOccur.cpp

	Word Occurences Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giwordoccur.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GIWordOccur
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::GIWordOccur(const unsigned int id)
	: GIWord(id), Occurs(0)
{
}


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::GIWordOccur(const unsigned int id,const unsigned int nb)
	: GIWord(id), Occurs(nb)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordOccur::Compare(const GInfo* i) const
{
	if(SameClass(i))
		return(Id-(static_cast<const GIWordOccur*>(i))->Id);
	else
		return(GALILEI::GInfo::Compare(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccur::Similarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccur*>(i))->Id)==0);
	else
		return(GALILEI::GInfo::Similarity(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWordOccur::DisSimilarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWordOccur*>(i))->Id)!=0);
	else
		return(GALILEI::GInfo::DisSimilarity(i));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordOccur::ClassName(void) const
{
	return("GIWordOccur");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GIWordOccur::InfoType(void) const
{
	return(infoWordOccur);
}


//-----------------------------------------------------------------------------
GALILEI::GIWordOccur::~GIWordOccur(void)
{
}
