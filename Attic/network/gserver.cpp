/*

	GALILEI Research Project

	GServer.cpp

	GALILEI Server - Implementation.

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


//---------------------------------------------------------------------------
// include files for current application
#include <network/gserver.h>
#include <network/gsocketport.h>
#include <network/gmsg.h>
using namespace ost;
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// GServer
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GServer::GServer(InetHostAddress& machine,int port)
	: TCPSocket(machine, port), Thread(), m_bQuitServer(true)
{
	m_pSocketService = new SocketService(0);

	//IMPORTANT SOCKET SERVICE MUST NOW BE EXPLICITLY STARTED
	m_pSocketService->start();
}


//---------------------------------------------------------------------------
void GServer::WriteLog(const char*)
{
}


//---------------------------------------------------------------------------
void GServer::run(void)
{
	waitMutex.enterMutex();
	m_bQuitServer = false;
	while(!m_bQuitServer)
	{
		try
		{
			// new does all the work to accept a new connection
			// and attach itself to the SocketService.
			CreateSocketPort(m_pSocketService, *((TCPSocket *)this));
		}
		catch ( ... )
		{
			// Bummer - there was an error.
			cerr << "GSocketPort create failed\n";
			exit();
		}
	}
	waitMutex.leaveMutex();
}


//---------------------------------------------------------------------------
SocketPort* GServer::CreateSocketPort(SocketService *pService, TCPSocket & Socket)
{
	return(new GSocketPort(this,pService,Socket));
}


//---------------------------------------------------------------------------
void GServer::StartServer()
{
	m_bQuitServer = true;
	start();
	while(m_bQuitServer)
	{
		Thread::yield();
	}
}


//---------------------------------------------------------------------------
void GServer::CloseSocketPort(GSocketPort*)
{
}


//---------------------------------------------------------------------------
void GServer::StopServer(void)
{
	m_bQuitServer = true;

	InetHostAddress host;
	tpport_t port;
	host = getLocal(&port);

	//This is required so that CreateSocketPort can return.
	TCPStream strm(host, port);

//	waitMutex.enterMutex();
//	waitMutex.leaveMutex();
}



//---------------------------------------------------------------------------
GServer::~GServer(void)
{
	terminate();
	if(m_pSocketService)
		delete m_pSocketService;
}
