/*

	GALILEI Research Project

	GMIMEType.h

	MIME Type - Implementation.

	(C) 2001 by P. Francq.

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
#ifndef GMIMETypeH
#define GMIMETypeH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GMIMEType class provides a representation for a given mime-type.
* @author Pascal Francq
* @short MIME-Type.
*/
class GMIMEType
{
	/**
	* The word.
	*/
	RString Name;

public:

	/**
	* Construct a mime type.
	* @param mime           Name of the mimetype.
	*/
	GMIMEType(const RString& name);

	/**
	* Compare method needed by RContainer.
	*/
	int Compare(const GMIMEType* m) const;

	/**
	* Compare method needed by RContainer.
	*/
	int Compare(const GMIMEType& m) const;

	/**
	* Compare method needed by RContainer.
	*/
	int Compare(const RString& m) const;

	/**
	* Compare method needed by RContainer.
	*/
	int Compare(const char* m) const;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
