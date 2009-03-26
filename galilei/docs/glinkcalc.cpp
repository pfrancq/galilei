/*

	GALILEI Research Project

	GLinkCalc.cpp

	Generic Link Method - Implementation.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
//include file for GALILEI
#include <glinkcalc.h>
#include <glink.h>
#include <glinks.h>
#include <gdoc.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GLinkCalc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RContainer<GLinks,true,true> GLinkCalc::Links_Out(100,50);
bool GLinkCalc::Inited=false;

//------------------------------------------------------------------------------
GLinkCalc::GLinkCalc(GFactoryLinkCalc* fac)
	: GPlugin<GFactoryLinkCalc>(fac)
{
}


//------------------------------------------------------------------------------
void GLinkCalc::InitGraph(void)
{
	R::RCursor<GDoc> cur = Session->GetDocs();
	R::RCursor<GLink> lcur;
	GLinks* links_out=0;
	size_t size(0);

	if(!Inited)
	{
		// Create a container of  outgoing links for all document present in the system
		size=cur.GetNb()*sizeof(GLinks);
		for(cur.Start();!cur.End();cur.Next())
		{
			Links_Out.InsertPtr( new GLinks( cur() ));
		}

			// For each page p set all the pages qi pointed by p
		for(cur.Start();!cur.End();cur.Next())
		{
			if (!cur()->GetNbLinks()) continue;
			links_out=Links_Out.GetPtr(cur()->GetId());
			lcur=cur()->GetLinks();
			size+=lcur.GetNb()*sizeof(GLink);

			for (lcur.Start();!lcur.End();lcur.Next())
			{
				//  out_going links
				links_out->InsertPtr(lcur());
			}
		}
		Inited =true;
	}
}


//------------------------------------------------------------------------------
void GLinkCalc::AddDoc(GDoc* doc)
{
	R::RCursor<GLink> lcur;
	GLinks* links_out=0;

	if (!doc->GetNbLinks()) return;

	links_out=Links_Out.GetPtr(doc->GetId());
	if (! links_out)
	{
		Links_Out.InsertPtr(new GLinks(doc) );
		links_out =Links_Out.GetPtr(doc->GetId());
		lcur= doc->GetLinks();

		for (lcur.Start();!lcur.End();lcur.Next())
		{
			//  out_going links
			links_out->InsertPtr(lcur());
		}
	}
	else return;
}


//------------------------------------------------------------------------------
void GLinkCalc::Disconnect(GSession*)
{
	Session=0;
	Inited=false;
}


//------------------------------------------------------------------------------
GLinkCalc::~GLinkCalc(void)
{
}



//------------------------------------------------------------------------------
//
// class GLinkCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLinkCalcManager::GLinkCalcManager(void)
	: GPluginManager<GLinkCalcManager,GFactoryLinkCalc,GLinkCalc>("LinkCalc",API_LINKCALC_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
GLinkCalcManager::~GLinkCalcManager(void)
{
}
