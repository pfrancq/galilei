/*

	GALILEI Research Project

	GMsgList.h

	Manager of Information Classes - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GMsgListh
#define GMsgListh


//-----------------------------------------------------------------------------
// include files for Common C++
#include <cc++/socket.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gmsg.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GMsgList class provides a basic manager to manipulate types of
* information. Eacht type a message has to be created, this class has to be
* called.
* @short Information Type Manager
* @author Pascal Francq
*/
class GMsgList : public ost::Buffer
{
	GMsg* Msgs;
	unsigned int Size;
	GMsg* head;
	GMsg* tail;

public:

	GMsgList(unsigned int size=100) throw(std::bad_alloc);

protected:

	/**
	 * Return the first object in the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to copy contents of head of buffer to.
	 */
	int onPeek(void *buf);

	/**
	 * Wait for and return a fixed object in the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to hold object returned from the buffer.
	 */
	int onWait(void *buf);

	/**
	 * Post an object of the appropriate size into the buffer.
	 * @return predefined size of this buffers objects.
	 * @param buf pointer to data to copy into the buffer.
	 */
	int onPost(void *buf);

public:

	/**
	 * Test if buffer is a valid object.
	 * @return true if object is valid.
	 */
	virtual bool isValid(void);

	/**
	*/
	virtual ~GMsgList(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
