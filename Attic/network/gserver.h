/*

	GALILEI Research Project

	GServer.h

	GALILEI Server - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

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

	Part of this source file comes from the class demonstrates use of the
	CommonC++ SocketPort class.
	Copyright 2001 - Nick Liebmann <nick@ukmail.org>

*/



//-----------------------------------------------------------------------------
#ifndef GSERVER_H
#define GSERVER_H


//-----------------------------------------------------------------------------
// include files for Common C++
#include <cc++/socket.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gmsgmng.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*
* This class implements a Thread that manages a SocketService. Simply
* create an instance of this class with the specified address and port, and
* signal the semaphore when you want it to start.
*
* A new GSocketPort object will be created for every connection that arrives.
*
*/
class GServer :  public virtual ost::TCPSocket, public virtual ost::Thread, public GMsgMng
{
protected:
	ost::SocketService *m_pSocketService;
	bool m_bQuitServer;
	ost::Mutex waitMutex;

public:

	/**
	* Constructor of the server.
	* @param machine         Host of the server.
	* @param port            Port of the server.
	*/
	GServer(ost::InetHostAddress& machine,int port);

	virtual void WriteLog(const char* entry);

	/**
	* Run method used by the thread.
	*/
	virtual void run(void);

	/**
	 * This abstract function is used to create a SocketPort of the desired type.
	 */
	virtual ost::SocketPort* CreateSocketPort(ost::SocketService* pService,ost::TCPSocket& Socket);

	virtual void StartServer(void);

	virtual void CloseSocketPort(GSocketPort* port);

	void StopServer(void);

	virtual ~GServer(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
