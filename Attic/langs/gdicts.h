/*

	GALILEI Research Project

	GDicts.h

	List of dictionnaries - Header.

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
#ifndef GDictsH
#define GDictsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/gdict.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDicts class provides a representation for a list of dictionnaries. These
* can be either stoplists or "real" dictionnaries.
* @author Pascal Francq
* @short List of dictionnaries.
*/
class GDicts : public RStd::RContainer<GDict,unsigned,true,true>
{
public:

	/**
	* Constructor.
	* @param nb             Number of dictinnaries contained.
	*/
	GDicts(unsigned nb) throw(bad_alloc);

	/**
	* Clear all the dictionnaries.
	*/
	void Clear(void);

	/**
	* Destrcutor.
	*/
	virtual ~GDicts(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
