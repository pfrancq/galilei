/*

	GALILEI Research Project

	GMsg.h

	GALILEI Message to Exchange Information - Header.

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
#ifndef GMsg_H
#define GMsg_H


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <new>


//-----------------------------------------------------------------------------
// include files for Common C++
#include <cc++/config.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// defines
const unsigned int HeaderLen=22;


//-----------------------------------------------------------------------------
/**
* Represent the type of message
*/
enum MsgType
{
	mtLogin                 /** Try to log to the system. */,
	mtLoginAnswer           /** Answer if the user can connect. */
};


//-----------------------------------------------------------------------------
/**
* The GMsg class provides a basic message to exchange information between
* applications.
* @short Information Message
* @author Pascal Francq
*/
class GMsg
{
public:

	/**
	* Descriptor of the message. This has to be unique for the whole network.
	*/
	char Descriptor[16];

	/**
	* Function involved in the message.
	*/
	uint16 Payload;

	/**
	* Length of the data part.
	*/
	uint32 DataLen;

	/**
	* Data.
	*/
	char* Data;

	/**
	* Determine if the message need an answer when it is sends.
	*/
	bool Answer;

public:

	/**
	* Constructor.
	*/
	GMsg(void);

	/**
	* Copy constructor.
	* @param src             Original message.
	*/
	GMsg(const GMsg* src) throw(std::bad_alloc);

	/**
	* Copy a message into the current one.
	* @param src             Original message.
	*/
	void Copy(const GMsg* src) throw(std::bad_alloc);

	/**
	* Create an Descriptor.
	*/
	void CreateDescriptor(void);

	/**
	* Run something when the message is received.
	*/
	virtual void Run(void);

	/**
	* Look if a answer is needed.
	* @return true if needed.
	*/
	bool NeedAnswer(void) const;

	/**
	* Get the payload of the message.
	* @return unsigned int
	*/
	unsigned int GetPayload(void) const;

	/**
	* Static function transforming a 16-bit unsigned int into a set of
	* chacaraters.
	* @param value           Integer value.
	* @param buf             Buffer that will contain the results.
	*/
	static void uint16_str(uint16 value,char* buf);

	/**
	* Static function transforming a 32-bit unsigned int into a set of
	* chacaraters.
	* @param value           Integer value.
	* @param buf             Buffer that will contain the results.
	*/
	static void uint32_str(uint32 value,char* buf);

	/**
	* Static function transforming a set of characters into a 16-bit unsigned
	* int.
	* @param str             Buffer that contain the integer.
	* @returns uint16
	*/
	static uint16 str_uint16(const char* str);

	/**
	* Static function transforming a set of characters into a 32-bit unsigned
	* int.
	* @param str             Buffer that contain the integer.
	* @returns uint32
	*/
	static uint32 str_uint32(const char* str);

	/**
	* Destructor.
	*/
	virtual ~GMsg(void);

	friend class GMsgMng;
};


//-----------------------------------------------------------------------------
class GMsgLogin : public GMsg
{
public:

	/**
	* Constructor.
	*/
	GMsgLogin(void);

	/**
	* Constructor.
	* @param login           Login of the user.
	* @param pwd             Password of the user.
	*/
	GMsgLogin(const char* login,const char* pwd);

	/**
	*/
	const char* GetLogin(void) const;
	const char* GetPwd(void) const;
};


//-----------------------------------------------------------------------------
class GMsgLoginAnswer : public GMsg
{
public:

	/**
	* Constructor.
	*/
	GMsgLoginAnswer(void);
	GMsgLoginAnswer(bool yes);
	bool IsAccept(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
