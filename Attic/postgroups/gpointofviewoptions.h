/*

	GALILEI Research Project

	GDocOptions.h

	Options for the document Analysis - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Stphane rideau (srideau@ulb.ac.be).

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
#ifndef GPtOfViewOptionH
#define GPtOfViewOptionH


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GPtOfViewOptions provides a representation for the options concerning the
* point of view.
*/
class GPtOfViewOptions
{
public:

	/**
	* Define if the links towards other groups are shown or not 
	*/
	bool ShowLinks;

	/**
	*  Define the precision of the points of view
	*/
	bool lowPOV;

	/**
	*
	*/
	bool mediumPOV;

	/**
	*
	*/
	bool highPOV;

	/*
	* Default Constructor.
	*/
	GPtOfViewOptions(void);

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
