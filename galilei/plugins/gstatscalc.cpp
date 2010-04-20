/*

	GALILEI Research Project

	GStatsCalc.cpp

	Generic Statistics Method - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rxmlfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstatscalc.h>
#include <rxmlfile.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GStatsCalc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStatsCalc::GStatsCalc(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac)
{
}


//------------------------------------------------------------------------------
RXMLTag* GStatsCalc::AddTag(RXMLStruct* xml,RXMLTag* parent,RString element,double val,const char* fmt)
{
	RXMLTag* tag(new RXMLTag(element));
	xml->AddTag(parent,tag);
	tag->InsertAttr("Value",RString::Number(val,fmt));
	return(tag);
}


//------------------------------------------------------------------------------
RXMLTag* GStatsCalc::AddTag(RXMLStruct* xml,RXMLTag* parent,RString element,RString val)
{
	RXMLTag* tag(new RXMLTag(element));
	xml->AddTag(parent,tag);
	tag->InsertAttr("Value",val);
	return(tag);
}


//------------------------------------------------------------------------------
GStatsCalc::~GStatsCalc(void)
{
}
