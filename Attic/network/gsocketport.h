/*

	GALILEI Research Project

	GSocketPort.h

	GALILEI Socket Service - Header.

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
#ifndef GSocketPortH
#define GSocketPortH


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
// forward class declaration
class GServer;


//-----------------------------------------------------------------------------
class GSocketPort : public ost::SocketPort
{
protected:

	/**
	* Flag set to true while Socket is open.
	*/
	bool m_bOpen;

	/**
	* Flag set to true when disconnection event has occurred.
	*/
	bool m_bDoDisconnect;

	/**
	* Flag set to true when reception has timed out.
	*/
	bool m_bTimedOut;

	/**
	* Flag set to true when the first bytes of a transmission have arrived.
	*/
	bool m_bReceptionStarted;

	/**
	* Count of last number of bytes received in pending().
	*/
	unsigned int m_nLastBytesAvail;

	/**
	* Buffer used to store received data for parsing.
	*/
	char *m_pBuf;

	/**
	* Point to the GServer that manages the port
	*/
	GServer* Server;

	/**
	* Id of the user connected.
	*/
	unsigned int UserId;

public:

	/**
	* Constructor.
	* @param server          Server.
	*/
	GSocketPort(GServer* server,ost::SocketService *pService, ost::TCPSocket& tcpSocket);

	/**
	* Overridden from class SocketPort.
	* Called when data is available in the receive buffer.
	*/
	virtual void pending(void);

	/**
	* Overridden from class SocketPort.
	* Called when the socket has been disconnected from the client-side.
	* Under some conditions this function is NOT called, which is why we have
	* some additional disconnection functionality within pending().
	*/
	virtual void disconnect(void);

	/**
	* Overridden from class SocketPort.
	* This function is called by the system when our timer expires.
	* We use the timer for 2 things:
	* 1) To determine whether reception has timed out. (Timer started in pending())
	* 2) To call CloseInterface to safely destroy the port.
	*/
	virtual void expired(void);

	/**
	* Our function to provide uniform closure of the Socket.
	* Can be called from the outside!
	*/
	bool CloseSocket(void);

	/**
	* This function should be called from pending() when the first bytes of our 
	* data has been received. If the complete data has not been received by the time
	* this expires we consider this an error.
	*/
	void ResetReadTimeout(timeout_t timeout);

	/**
	* This function should be use in the event of a reception error, to flush out
	* the receive buffer.
	*/
	void FlushRxData(void);

	/**
	* This function is called just before the port is closed.
	* Do not send any data from this function!
	*/
	virtual void OnConnectionClosed(void);

	/**
	* Called when the receive timeout occurs
	*/
	virtual void OnRxTimeout(void);

	/**
	* Little utility function for sending data to the client.
	*/
	void Send(GMsg* msg);

	/**
	* Set the identificator of the user connected through this socket. If the
	* identificator is "cNoRef", the socket is closed.
	*/
	void SetUserId(unsigned int id);

	/**
	* Destructor.
	*/
	virtual ~GSocketPort(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
