/*

	GALILEI Research Project

	GClient.cpp

	Generic GALILEI Client - Implementation.

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
// include files for ANSI C++
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gclient.h>
#include <network/gmsg.h>
#include <network/gmsgmng.h>
using namespace ost;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GClient
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GClient::GClient(const InetHostAddress& ia,const char* usr,const char* pwd,unsigned int size)
	: TCPSession(ia, 3999), GMsgList(size), GMsgMng(), User(0), Pwd(0), bRun(true)
{
	User=new char[strlen(usr)+1];
	strcpy(User,usr);
	Pwd=new char[strlen(pwd)+1];
	strcpy(Pwd,pwd);
}


//-----------------------------------------------------------------------------
bool GClient::Login(void)
{
	GMsg* ret;
	bool a;

	GMsgLogin Login(User,Pwd);
	Send(&Login);
	ret=Receive();
	if(!ret) return(false);
	ret->Run();
	a=dynamic_cast<GMsgLoginAnswer*>(ret)->IsAccept();
	delete ret;
	return(a);
}


//-----------------------------------------------------------------------------
void GClient::Send(GMsg* msg)
{
	char Pay[2];
	char Len[4];

	GMsg::uint16_str(msg->Payload,Pay);
	GMsg::uint32_str(msg->DataLen,Len);
	tcp()->write(msg->Descriptor,16);
	tcp()->write(Pay,2);
	tcp()->write(Len,4);
	tcp()->write(msg->Data,msg->DataLen);
	(*tcp())<<endl;
}


//-----------------------------------------------------------------------------
GMsg* GClient::Receive(void)
{
	char Buf[HeaderLen];
	GMsg* msg;

	tcp()->read(Buf,HeaderLen);
	msg=CreateInfo(Buf);
	if(!msg) return(0);
	tcp()->read(msg->Data,msg->DataLen);
	return(msg);
}


//-----------------------------------------------------------------------------
void GClient::run(void)
{
	GMsg* ret;
	GMsg msg;

	// User must first login
	if(!Login()) return;

	// Send Messages
	for(;bRun;)
	{
		// Wait for a message
		if(isValid())
		{
			wait(&msg);

			// Send it through network
			Send(&msg);

			// If a answer is needed,
			// - Receive it.
			// - Run it.=
			if(msg.NeedAnswer())
			{
				ret=Receive();
				if(ret)
				{
					ret->Run();
					delete ret;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GClient::final(void)
{
	delete this;
}


//---------------------------------------------------------------------------
GMsg* GClient::MsgCreator(MsgType t)
{
	switch(t)
	{
		case mtLoginAnswer:
			return(new GMsgLoginAnswer());

		default:
			return(0);
	}
}


//-----------------------------------------------------------------------------
void GClient::Stop(void)
{
	bRun=false;
}


//-----------------------------------------------------------------------------
GClient::~GClient(void)
{
	if(User) delete[] User;
	if(Pwd) delete[] Pwd;
}
