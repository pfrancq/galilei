/*

	GALILEI Research Project

	GClient.h

	Generic GALILEI Client - Header.

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

*/


//-----------------------------------------------------------------------------
#ifndef GClientH
#define GClientH


//-----------------------------------------------------------------------------
// include files for Common C++
#include <cc++/socket.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gmsgmng.h>
#include <network/gmsglist.h>


//-----------------------------------------------------------------------------
namespace GALILEI{


//-----------------------------------------------------------------------------
// forward class declaration
class GMsg;
class GMsgMng;


//-----------------------------------------------------------------------------
/*
* The GClient class provides a generic client using the GALILEI information
* exchange protocol.
*
* This class can be used whenever an application (client or processes) needs
* some information from another process.
* @short Generic GALILEI Client
* @author Pascal Francq
*/
class GClient : public ost::TCPSession, public GMsgList, public GMsgMng
{
	/**
	* User Name.
	*/
	char* User;

	/**
	* Password
	*/
	char* Pwd;

	/**
	* Boolean value that determines if the thread should run.
	*/
	bool bRun;

public:

	/**
	* Constructor.
	* @param ia              Address of the server.
	* @param size            Maximal size of message hold by the client.
	*/
	GClient(const ost::InetHostAddress& ia,const char* usr,const char* pwd,unsigned int size=100);

protected:

	/**
	* Login to the server.
	* @return true if the user and password are valid.
	*/
	bool Login(void);

	/**
	* Method to send a message over the network.
	*/
	void Send(GMsg* msg);

	/**
	* Method to receive a message from the network. The message has to be
	* deleted.
	* @return Pointer to the message.
	*/
	GMsg* Receive(void);

	/**
	* Method running the thread.
	*/
	virtual void run(void);

	/**
	* Method called when the thread is finish. Actually, this method only
	* deletes the 'this' pinter.
	*/
	virtual void final(void);

	/**
	* Create a given message.
	* @param t               Type of the message to create.
	* @return Pointer to the message created.
	*/
	virtual GMsg* MsgCreator(MsgType t);

public:

	/**
	* Stop the thread.
	*/
	void Stop(void);

	/**
	* Destructor.
	*/
	virtual ~GClient(void);
};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
