/*

	GALILEI Research Project

	GStatsCalc.cpp

	Generic Statistics Method - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R
#include <rstd/rxmlfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gstatscalc.h>
#include <rstd/rxmlfile.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GStatsCalc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStatsCalc::GStatsCalc(GFactoryStatsCalc* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryStatsCalc>(fac), Session(0)
{
}


//------------------------------------------------------------------------------
void GStatsCalc::Connect(GSession* session) throw(GException)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GStatsCalc::Disconnect(GSession*) throw(GException)
{
	Session=0;
}


//------------------------------------------------------------------------------
void GStatsCalc::AddTag(RXMLStruct* xml,RXMLTag* parent,RString element,double val)
{
	RXMLTag* tag=new RXMLTag(element);
	tag->InsertAttr("Value",dtou(val));
	xml->AddTag(parent,tag);
}


//------------------------------------------------------------------------------
void GStatsCalc::AddTag(RXMLStruct* xml,RXMLTag* parent,RString element,RString val)
{
	RXMLTag* tag=new RXMLTag(element);
	tag->InsertAttr("Value",val);
	xml->AddTag(parent,tag);
}


//------------------------------------------------------------------------------
GStatsCalc::~GStatsCalc(void)
{
}
