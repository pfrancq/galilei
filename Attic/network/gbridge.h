/*

	GBridge.h

	Main Entry - Header.

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
#ifndef GBridgeH
#define GBridgeH


//------------------------------------------------------------------------------
// include files for R
#include <rstd/rtextfile.h>
#include <rstd/rxmlstruct.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gserver.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
*/
class GBridge : public GServer
{
	R::RXMLStruct* Config;
	R::RTextFile* Log;
public:

	GBridge(R::RXMLStruct* s,ost::InetHostAddress& machine);

	virtual void WriteLog(const char* entry);

protected:

	/**
	* Create a given message.
	* @param t               Type of the message to create.
	* @param port            Port receiving the message.
	* @return Pointer to the message created.
	*/
	virtual GMsg* MsgCreator(MsgType t,GSocketPort* port);

public:

	virtual void CloseSocketPort(GSocketPort* port);

	virtual ~GBridge(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

