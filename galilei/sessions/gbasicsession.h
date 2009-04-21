/*

	GALILEI Research Project

	GBasicSession.h

	Basic Session - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
#ifndef GBasicSession_H
#define GBasicSession_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GBasicSession provides the base class for every class that should given
* GSession. Each intermediate class should inherit it virtually.
* @author Pascal Francq
* @short Basic Session.
*/
class GBasicSession
{
protected:

	/**
	 * Storage manager.
	 */
	GStorage* Storage;

	/**
	 *  Must the results be saved after computed ?
	 */
	bool SaveResults;

public:

	/**
	 * Construct the basic session. In practice, the storage is created.
	 */
	GBasicSession(void);

	/**
	* @return Pointer to storage manager.
	*/
	GStorage* GetStorage(void) const;

	/**
	* Verify if the results of the computations must be saved.
	*/
	bool MustSaveResults(void) const {return(SaveResults);}

	/**
	* Set if the results of the computation must be saved or not.
	* @param save            Save the objects after computation?
	*/
	void SetSaveResults(bool save=true) {SaveResults=save;}

	/**
	 * Destruct the basic session. In practice, the storage is deleted.
	 */
	virtual ~GBasicSession(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
