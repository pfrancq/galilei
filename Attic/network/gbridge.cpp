/*

	GBridge.cpp

	Main Entry - Implementation.

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



//------------------------------------------------------------------------------
// include files for R
#include <rxml/rxmltag.h>
using namespace RXML;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <network/gbridge.h>
#include <network/gsocketport.h>
using namespace GALILEI;
using namespace ost;
using namespace RIO;



//---------------------------------------------------------------------------
//
// GMsgLoginServer
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GMsgLoginServer : public GMsgLogin
{
	GBridge* Main;
	GSocketPort* Port;
public:
	GMsgLoginServer(GBridge* m,GSocketPort* p) : GMsgLogin(), Main(m), Port(p) {}
	virtual void Run(void);
};


//---------------------------------------------------------------------------
void GMsgLoginServer::Run(void)
{
	bool Ans;

	if((!strcmp(GetLogin(),"pfrancq"))&&(!strcmp(GetPwd(),"pfrancq")))
		Ans=true;
	else
		Ans=false;
	GMsgLoginAnswer Answer(Ans);
	Port->Send(&Answer);
	if(Ans)
	{
		Main->WriteLog(GetLogin()+RString(" connected"));
	}
	Port->SetUserId(1);
}



//------------------------------------------------------------------------------
//
// class GBridge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GBridge::GBridge(RXMLStruct* s,InetHostAddress& machine)
	: GServer(machine,atoi(s->GetTag("Entry")->GetAttrValue("Port"))),
	  TCPSocket(machine,atoi(s->GetTag("Entry")->GetAttrValue("Port"))), Thread(),
	  Config(s), Log(0)
{
	Log=new RIO::RTextFile(s->GetTag("Log")->GetAttrValue("File"),Append);
	Log->WriteLog("GALILEI Server launch up");
}


//---------------------------------------------------------------------------
void GBridge::WriteLog(const char* entry)
{
	Log->WriteLog(entry);
}


//---------------------------------------------------------------------------
GMsg* GBridge::MsgCreator(MsgType t,GSocketPort* port)
{
	switch(t)
	{
		case mtLogin:
			return(new GMsgLoginServer(this,port));

		default:
			return(0);
	}
}


//---------------------------------------------------------------------------
void GBridge::CloseSocketPort(GSocketPort* /*port*/)
{
	Log->WriteLog("pfrancq disconnected");
}


//------------------------------------------------------------------------------
GBridge::~GBridge(void)
{
	if(Config)
		delete Config;
	if(Log)
	{
		Log->WriteLog("GALILEI server shutdown");
		delete Log;
	}
}


