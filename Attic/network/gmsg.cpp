/*

	GALILEI Research Project

	GMsg.cpp

	GALILEI Message to Exchange Information - Implementation.

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
// include files for current application
#include <network/gmsg.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GMsg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMsg::GMsg(void)
	: DataLen(0), Data(0), Answer(false)
{
}


//-----------------------------------------------------------------------------
GMsg::GMsg(const GMsg* src) throw(bad_alloc)
	: Payload(src->Payload), DataLen(src->DataLen), Data(0), Answer(src->Answer)
{
	memcpy(Descriptor,src->Descriptor,16);
	Data=new char[DataLen];
	memcpy(Data,src->Data,DataLen);
}


//-----------------------------------------------------------------------------
void GMsg::Copy(const GMsg* src) throw(bad_alloc)
{
	memcpy(Descriptor,src->Descriptor,16);
	Payload=src->Payload;
	if(Data)
	{
		delete[] Data;
		Data=0;
	}
	DataLen=src->DataLen;
	Answer=src->Answer;
	Data=new char[DataLen];
	memcpy(Data,src->Data,DataLen);
}


//-----------------------------------------------------------------------------
void GMsg::CreateDescriptor(void)
{
	char* ptr;
	unsigned int i;

	for(i=17,ptr=Descriptor;--i;ptr++)
		(*ptr) = rand()%255;
	Descriptor[8] = 0xff;
	Descriptor[15] = 1;
};


//-----------------------------------------------------------------------------
void GMsg::Run(void)
{
	cout<<"Message: "<<Payload<<endl;
}


//-----------------------------------------------------------------------------
bool GMsg::NeedAnswer(void) const
{
	return(Answer);
}


//-----------------------------------------------------------------------------
unsigned int GMsg::GetPayload(void) const
{
	return(Payload);
}


//-----------------------------------------------------------------------------
void GMsg::uint16_str(uint16 value,char* buf)
{
	#ifdef WORDS_BIGENDIAN
		char str[2] = { (value & 0xff), ((value >> 8) & 0xff) };
		memcpy(buf,str,2);
	#else
		memcpy(buf,(char*)&value,2);
	#endif
}


//-----------------------------------------------------------------------------
void GMsg::uint32_str(uint32 value,char* buf)
{

	#ifdef WORDS_BIGENDIAN
		char str[4] = { (value & 0xff), ((value >> 8) & 0xff),
                  ((value >> 16) & 0xff), ((value >> 24) & 0xff) };
		memcpy(buf,str,3);
	#else
		memcpy(buf,(char*)&value,4);
	#endif
}


//-----------------------------------------------------------------------------
uint16 GMsg::str_uint16(const char *str)
{
	#ifdef WORDS_BIGENDIAN
		return ((((unsigned )(str[1]) & 0xff) << 8) |
           ((unsigned )(str[0]) & 0xff));
	#else
		return *((uint32*)str);
	#endif
}


//-----------------------------------------------------------------------------
uint32 GMsg::str_uint32(const char *str)
{
#ifdef WORDS_BIGENDIAN
  return ( (((unsigned )(str[3]) & 0xff) << 24) |
           (((unsigned )(str[2]) & 0xff) << 16) |
           (((unsigned )(str[1]) & 0xff) << 8) |
            ((unsigned )(str[0]) & 0xff));
#else
  return *((uint32*)str);
#endif
}


//-----------------------------------------------------------------------------
GMsg::~GMsg(void)
{
	if(Data)
		delete[] Data;
}



//-----------------------------------------------------------------------------
//
// GMsgLogin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMsgLogin::GMsgLogin(void)
	: GMsg()
{
	Answer=true;
}


//-----------------------------------------------------------------------------
GMsgLogin::GMsgLogin(const char* login,const char* pwd)
	: GMsg()
{
	CreateDescriptor();
	DataLen=strlen(login)+strlen(pwd)+2;
	Payload=mtLogin;
	Data=new char[DataLen];
	strcpy(Data,login);
	strcpy(&Data[strlen(login)+1],pwd);
}


//-----------------------------------------------------------------------------
const char* GMsgLogin::GetLogin(void) const
{
	return(Data);
}


//-----------------------------------------------------------------------------
const char* GMsgLogin::GetPwd(void) const
{
	return(&Data[strlen(Data)+1]);
}



//-----------------------------------------------------------------------------
//
// GMsgLoginAnswer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMsgLoginAnswer::GMsgLoginAnswer(void)
	: GMsg()
{
}


//-----------------------------------------------------------------------------
GMsgLoginAnswer::GMsgLoginAnswer(bool yes)
	: GMsg()
{
	CreateDescriptor();
	DataLen=1;
	Payload=mtLoginAnswer;
	Data=new char[DataLen];
	Data[0]=yes;
}

//-----------------------------------------------------------------------------
bool GMsgLoginAnswer::IsAccept(void)
{
	return(Data[0]);
}
