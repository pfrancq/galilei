/*

	GALILEI Research Project

	GSocketPort.cpp

	GALILEI Socket Service - Implementation.

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
// include files for ANSI C++
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R
#include <rstd/rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gsocketport.h>
#include <network/gmsg.h>
#include <network/gserver.h>
using namespace ost;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Dirty global to allow us to quit simply
int g_nOpenPorts=0;


//-----------------------------------------------------------------------------
// Defines
/**
* Specifies the maximum number of bytes in a 'packet'.
*/
#define MAX_RXBUF 32768

/**
* Specifies how long we will wait for a complete packet.
*/
#define MAX_RXTIMEOUT 10000

/**
* Specifies the timeout for the diconnect timer.
*/
#define DISCONNECT_MS 500



//-----------------------------------------------------------------------------
//
// GSocketPort
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSocketPort::GSocketPort(GServer* server,SocketService *pService, TCPSocket & tcpSocket)
	: SocketPort(pService, tcpSocket), Server(server)
{
	tpport_t port;
	InetHostAddress ia = getPeer( & port );
	Server->WriteLog(RString("Connecting from ")+ia.getHostname()+":"+itou(port));

	// Set up non-blocking reads
	setCompletion( false );

	//1.9.3 THIS LINE DOES NOT SEEM TO BE REQUIRED ANYMORE!
	//This sorts out a bug which prevents connections after a disconnect
	//setDetectOutput(true);

	m_bOpen = true;
	m_bDoDisconnect = false;
	m_bTimedOut = false;
	m_bReceptionStarted = false;
	m_nLastBytesAvail = 0;
	m_pBuf = new char[MAX_RXBUF];
	g_nOpenPorts++;
}


//-----------------------------------------------------------------------------
void GSocketPort::pending(void)
{
	if(!m_bOpen)
		return;

	// Read all available bytes into our buffer
	unsigned int nBytesAvail = peek(m_pBuf,MAX_RXBUF);

	if(!m_bReceptionStarted)
	{	//Start the receive timer
		ResetReadTimeout(MAX_RXTIMEOUT);	//Got 'n' seconds to get all the data else we timeout
		m_bReceptionStarted = true;
	}
	else
	{
		if(m_bTimedOut)	//The receive timer has expired...this is a timeout condition
		{
			ResetReadTimeout(MAX_RXTIMEOUT); //Clear the timeout flag
			m_nLastBytesAvail = 0;		//Reset the flags
			m_bReceptionStarted = false;
			OnRxTimeout();	//Do whatever 'we' do for a timeout (probably a flush or disconnect)...
			return;
		}
	}

	if(m_nLastBytesAvail == nBytesAvail)	//Check if any more data has been received since last time
	{										//No point in parsing unless this has changed!
		//Maybe yield in here!
		//Thread::yield();
		if(nBytesAvail == 0)		//If we have been called with 0 bytes available (twice now)
		{							//a disconnection has occurred
			if(!m_bDoDisconnect)
			{
				CloseSocket();	//Force the close
			}
		}
		return;
	}

	//Depending on your application you may want to attempt to process the extra data
	//(or change your MAX_RXBUF).
	//
	//Here I just flush the whole lot, because I assume a 'legal' client wont send more than
	//we can receive....maybe someone is trying to flood / overrun us!
	if(nBytesAvail > MAX_RXBUF)
	{
		cerr << "TCP/IP overflow..." << endl;
		FlushRxData();
		m_nLastBytesAvail = 0;
		m_bReceptionStarted = false;
		return;
	}
	m_nLastBytesAvail = nBytesAvail;
	GMsg* msg=0;
	char Buf[HeaderLen];

	for(;;)
	{

		if((nBytesAvail>=HeaderLen)&&(!msg))
		{
			receive(Buf,HeaderLen);
			msg=Server->CreateInfo(Buf,this);
		}

		if((msg)&&(nBytesAvail>=msg->DataLen+HeaderLen))
		{
			m_nLastBytesAvail = 0;
			m_bReceptionStarted = false;
			receive(msg->Data,msg->DataLen);
			receive(Buf,1);
			msg->Run();
			delete msg;
			msg=0;
			return;
		}
	}
}


//-----------------------------------------------------------------------------
void GSocketPort::disconnect(void)
{
	if(m_bOpen)
	{
		m_bDoDisconnect = true;
		CloseSocket();
	}
}


//-----------------------------------------------------------------------------
void GSocketPort::expired(void)
{
	if(m_bDoDisconnect && m_bOpen)
	{
		CloseSocket();
	}
	else if(m_bOpen && m_bReceptionStarted)
	{
		//Timer must have expired because the rx data has not all been received
		m_bTimedOut = true;
	}
}


//-----------------------------------------------------------------------------
bool GSocketPort::CloseSocket(void)
{
	if(m_bOpen && m_bDoDisconnect)
	{									//This is where the disconnection really occurs
		m_bOpen = false;				//If m_bDoDisconnect == true we know this has been called
		OnConnectionClosed();			//through the timer, so 'delete this' is safe!
		delete this;
	}
	else if(m_bOpen)
	{
		m_bDoDisconnect = true;			//Just set the timer and the flag so we can
		setTimer(DISCONNECT_MS);		//disconnect safely, in DISCONNECT_MS
	}
	return(true);
}


//-----------------------------------------------------------------------------
void GSocketPort::ResetReadTimeout(timeout_t timeout)
{
	m_bTimedOut = false;
	setTimer(timeout);
}


//-----------------------------------------------------------------------------
void GSocketPort::FlushRxData(void)
{
	while(receive(m_pBuf, MAX_RXBUF) > 0);
	cerr << "FLUSHED" << endl;
}


//-----------------------------------------------------------------------------
void GSocketPort::OnConnectionClosed(void)
{
	// If User was connected, the process must do some clean-up.
	if(UserId!=0xFFFF)
		Server->CloseSocketPort(this);
}

//-----------------------------------------------------------------------------
void GSocketPort::OnRxTimeout(void)
{
	cerr << "Receive timeout occurred" << endl;
	FlushRxData();
}


//-----------------------------------------------------------------------------
void GSocketPort::Send(GMsg* msg)
{
	char Pay[2];
	char Len[4];

	// If null pointer, 0 bytes were sent
	if(!msg||m_bDoDisconnect)
		return;

	// Send Header and Data
	GMsg::uint16_str(msg->Payload,Pay);
	GMsg::uint32_str(msg->DataLen,Len);
	send(msg->Descriptor,16);
	send(Pay,2);
	send(Len,4);
	send(msg->Data,msg->DataLen);
	while(!isPending(Socket::pendingOutput, 0))		//Wait for output to complete
	{
		if(m_bDoDisconnect || !m_bOpen)
		{
			//If we are disconnecting, just pretend all the bytes were sent
			return;
		}
		//I like to yield whenever waiting for things...
		//this is optional and may not suit your implementation!
		Thread::yield();
	}
	return;
}


//-----------------------------------------------------------------------------
void GSocketPort::SetUserId(unsigned int id)
{
	if(id==0xFFFF)
	{
		CloseSocket();
		return;
	}
	UserId=id;
}


//-----------------------------------------------------------------------------
GSocketPort::~GSocketPort(void)
{
	g_nOpenPorts--;
	endSocket();
	delete [] m_pBuf;
}
