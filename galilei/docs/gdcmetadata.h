/*

	GALILEI Research Project

	GDCMetaData.h

	Dubin Core Metadata Element - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GDCMetaDataH
#define GDCMetaDataH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDCMetaData class provides a representation for a Metadata element
* comming from Dublin Core Metadata Elements.
* @author Pascal Francq
* @short Dublin Core Metadata Element.
*/
class GDCMetaData
{
public:
	enum MetaDataType {Title,Creator,Subject,Description,Publisher,Contributor,Date,
	           Type,Format,Identifier,Source,Language,Relation,Coverage,Rights};
protected:

	/**
	* Type of the metadata.
	*/
	MetaDataType MetaData;

	/**
	* Value of the metadata.
	*/
	RStd::RString Value;

public:

	/**
	* Constructor.
	* @param type           Type of the metadata.
	* @param val            Value of the metadata.
	*/
	GDCMetaData(MetaDataType type,const char val) throw(bad_alloc);

	/**
	* Compare function used by RStd::RContainer.
	*/
	int Compare(const GDCMetaData* mt) const;

	/**
	* Compare function used by RStd::RContainer.
	*/
	int Compare(const GDCMetaData& mt) const;

	/**
	* Compare function used by RStd::RContainer.
	*/
	int Compare(const MetaDataType t) const;

	/**
	* Get the type of the metadata.
	* @returns Pointer to a C string.
	*/
	const char* GetType(void);

	/**
	* Get the value of the metadata.
	* @returns Pointer to a C string.
	*/
	const char* GetValue(void) {return(Value());}

	/**
	* Destructor.
	*/
	virtual ~GDCMetaData(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
