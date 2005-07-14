/*

	GALILEI Research Project

	GConfig.h

	XML Structure representing a configuration - Header.

	Copyright 2003 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GConfigH
#define GConfigH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlstruct.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConfig class provides a representation for a XML structure representing
* all the parameters of the plugins.
* @author Pascal Francq
* @short XML Config Structure.
*/
class GConfig : R::RXMLStruct
{

	/**
	* File containg the document (must be local).
	*/
	R::RString FileName;

	/**
	* Tag holding the main categories.
	*/
	R::RXMLTag* Root;
	
	/**
	* Document Analysis Methods.
	*/
	R::RXMLTag* SessionParams;

public:

	/**
	* Constructor.
	* @param f               Name of the file.
	*/
	GConfig(const char* f) throw(std::bad_alloc);

	/**
	* Load a file in the structure.
	*/
	void Load(void) throw(GException);

	/**
	* Save a file in the structure.
	*/
	void Save(void) throw(GException);

	/**
	* Read the Session params in the structure.
	* @param p             Session parameters.
	*/
	void Read(GSessionParams& p);

	/**
	* Store the Session params in the structure.
	* @param p             Session parameters.
	*/
	void Store(GSessionParams& p);

	/**
	* Destructor.
	*/
	~GConfig(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
