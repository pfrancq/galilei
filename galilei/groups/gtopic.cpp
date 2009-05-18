/*

	GALILEI Research Project

	GTopic.cpp

	Topic - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gtopic.h>
#include <gdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GTopic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTopic::GTopic(const RString& name)
	: GGroup<GDoc,GTopic,otTopic>(cNoRef,0,name,RDate::GetToday(),RDate::Null)
{
}


//------------------------------------------------------------------------------
GTopic::GTopic(size_t id,size_t blockid,const RString& name,const RDate& u,const RDate& c)
	: GGroup<GDoc,GTopic,otTopic>(id,blockid,name,u,c)
{
}


//------------------------------------------------------------------------------
GTopic::~GTopic(void)
{
}
