/*

	GALILEI Research Project

	GProfOptions.h

	Options for the profiles description - Header.

	(C) 2002 by P. Francq.

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
#ifndef GProfOptionsH
#define GProfOptionsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfOptions provides a representation for the options concerning the
* profiles computing and the subprofile description.
* @author Pascal Francq
* @short Profile Options.
*/
class GProfOptions
{
public:

	/**
	* Type of the description to use.
	*/
	tSubProfileDesc Model;

	/**
	* Sizes of the List in the Vector Space.
	*/
	unsigned int ListSize;

	/**
	* Type of the method to use to compute the vector space dfescription.
	*/
	tVectorMethod VectorMethod;

	/**
	* Default Constructor.
	*/
	GProfOptions(void);

	/**
	* Copy Constructor.
	*/
	GProfOptions(const GProfOptions& opt);

	/**
	* Copy Constructor.
	*/
	GProfOptions(const GProfOptions* opt);

	/**
	* Assignment operator using another options.
	*/
	GProfOptions& operator=(const GProfOptions& opt);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
