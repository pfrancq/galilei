/*

	GALILEI Research Project

	GIWord.cpp

	Word Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giword.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GIWord
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIWord::GIWord(unsigned int id)
	: GALILEI::GInfo(), Id(id)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GIWord::Compare(const GInfo* i) const
{
	if(SameClass(i))
		return(Id-(static_cast<const GIWord*>(i))->Id);
	else
		return(GALILEI::GInfo::Compare(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWord::Similarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWord*>(i))->Id)==0);
	else
		return(GALILEI::GInfo::Similarity(i));
}


//-----------------------------------------------------------------------------
float GALILEI::GIWord::DisSimilarity(const GInfo* i) const
{
	if(SameClass(i))
		return((Id-(static_cast<const GIWord*>(i))->Id)!=0);
	else
		return(GALILEI::GInfo::DisSimilarity(i));
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWord::ClassName(void) const
{
	return("GIWord");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GIWord::InfoType(void) const
{
	return(infoWord);
}


//-----------------------------------------------------------------------------
GALILEI::GIWord::~GIWord(void)
{
}
