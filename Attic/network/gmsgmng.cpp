/*

	GALILEI Research Project

	GMsgMng.cpp

	Manager of Information Classes - Implementation.

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
// include files for GALILEI
#include <network/gmsgmng.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GMsgMng
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMsgMng::GMsgMng(void) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
GMsg* GMsgMng::CreateInfo(const char* head) throw(bad_alloc)
{
	MsgType payload;
	GMsg* msg;

	payload=static_cast<MsgType>(GMsg::str_uint16(&head[16]));
	msg=MsgCreator(payload);
	if(msg)
	{
		memcpy(msg->Descriptor,head,16);
		msg->Payload=GMsg::str_uint16(&head[16]);
		msg->DataLen=GMsg::str_uint32(&head[18]);
		msg->Data=new char[msg->DataLen];
	}
	return(msg);
}


//-----------------------------------------------------------------------------
GMsg* GMsgMng::CreateInfo(const char* head,GSocketPort* port) throw(bad_alloc)
{
	MsgType payload;
	GMsg* msg;

	payload=static_cast<MsgType>(GMsg::str_uint16(&head[16]));
	msg=MsgCreator(payload,port);
	if(msg)
	{
		memcpy(msg->Descriptor,head,16);
		msg->Payload=GMsg::str_uint16(&head[16]);
		msg->DataLen=GMsg::str_uint32(&head[18]);
		msg->Data=new char[msg->DataLen];
	}
	return(msg);
}


//-----------------------------------------------------------------------------
GMsg* GMsgMng::MsgCreator(MsgType)
{
	return(0);
}


//-----------------------------------------------------------------------------
GMsg* GMsgMng::MsgCreator(MsgType,GSocketPort*)
{
	return(0);
}


//-----------------------------------------------------------------------------
GMsgMng::~GMsgMng(void)
{
}
