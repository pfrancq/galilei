/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile' Computing Method - Header.

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
#ifndef GProfileCalcH
#define GProfileCalcH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;
class GProfile;


//-----------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile, i.e. its sub-profiles.
* @author Pascal Francq
* @short Generic Profile' Computing Method.
*/
class GProfileCalc
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalc(GSession* session) throw(bad_alloc);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GProfile* profile)=0;

	/**
	* Get the type of the method implemented. This is used to find the
	* correspondance between a profiles description and the compute method
	* associated.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const=0;

	/**
	* Destructor.
	*/
	virtual ~GProfileCalc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

