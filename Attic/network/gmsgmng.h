/*

	GALILEI Research Project

	GMsgMng.h

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
#ifndef GMsgMngh
#define GMsgMngh


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gmsg.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GSocketPort;


//-----------------------------------------------------------------------------
/**
* The GMsgMng class provides a basic manager to maniuplate types of
* information. Eacht type a message has to be created, this class has to be
* called.
* @short Information Type Manager
* @author Pascal Francq
*/
class GMsgMng
{
public:

	/**
	* Constructor.
	*/
	GMsgMng(void) throw(std::bad_alloc);

	/**
	* Create an information message based on a given header.
	* @param head            Header of the message.
	* @return Pointer to the message created.
	*/
	GMsg* CreateInfo(const char* head) throw(std::bad_alloc);

	/**
	* Create an information message based on a given header.
	* @param head            Header of the message.
	* @param port            Port receiving the message.
	* @return Pointer to the message created.
	*/
	GMsg* CreateInfo(const char* head,GSocketPort* port) throw(std::bad_alloc);

protected:

	/**
	* Create a given message.
	* @param t               Type of the message to create.
	* @return Pointer to the message created.
	*/
	virtual GMsg* MsgCreator(MsgType t);

	/**
	* Create a given message.
	* @param t               Type of the message to create.
	* @param port            Port receiving the message.
	* @return Pointer to the message created.
	*/
	virtual GMsg* MsgCreator(MsgType t,GSocketPort* port);

public:

	/**
	*/
	virtual ~GMsgMng(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
