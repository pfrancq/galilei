/*

	GALILEI Research Project

	GDCMetaData.cpp

	Dubin Core Metadata Element - Implementation.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdcmetadata.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GDCMetaData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDCMetaData::GDCMetaData(MetaDataType type,const char val) throw(bad_alloc)
	: MetaData(type), Value(val)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GDCMetaData::Compare(const GDCMetaData* mt) const
{
	if(MetaData==mt->MetaData)
		return(Value.Compare(mt->Value));
	return(MetaData-mt->MetaData);
}


//-----------------------------------------------------------------------------
int GALILEI::GDCMetaData::Compare(const GDCMetaData& mt) const
{
	if(MetaData==mt.MetaData)
		return(Value.Compare(mt.Value));
	return(MetaData-mt.MetaData);
}


//-----------------------------------------------------------------------------
int GALILEI::GDCMetaData::Compare(const MetaDataType t) const
{
	return(MetaData-t);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GDCMetaData::GetType(void)
{
	switch(MetaData)
	{
		case Title:
			return("Title");
			break;

		case Creator:
			return("Creator");
			break;

		case Subject:
			return("Subject");
			break;

		case Description:
			return("Description");
			break;

		case Publisher:
			return("Publisher");
			break;

		case Contributor:
			return("Contributor");
			break;

		case Date:
			return("Date");
			break;

		case Type:
			return("Type");
			break;

		case Format:
			return("Format");
			break;

		case Identifier:
			return("Identifier");
			break;

		case Source:
			return("Source");
			break;

		case Language:
			return("Language");
			break;

		case Relation:
			return("Relation");
			break;

		case Coverage:
			return("Coverage");
			break;

		case Rights:
			return("Rights");
			break;

		default:
			return(0);
	}
	return(0);
}


//-----------------------------------------------------------------------------
GALILEI::GDCMetaData::~GDCMetaData(void)
{
}
