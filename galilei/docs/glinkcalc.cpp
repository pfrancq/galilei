/*
	GALILEI Research Project

	GLinkCalc.cpp

	Generic Link Method - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

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
//include file for GALILEI
#include <docs/glinkcalc.h>
#include <docs/glink.h>
#include <docs/glinks.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
//
// GLinkCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GLinkCalc::GLinkCalc(GFactoryLinkCalc* fac) throw(bad_alloc)
	: GPlugin<GFactoryLinkCalc>(fac), Session(0)
{
	Links_Out = new RContainer<GLinks,unsigned int,true,true> (100,50);
	Inited=false;
}




//-----------------------------------------------------------------------------
void GLinkCalc::InitGraph(void)
{
	GDocCursor cur = Session->GetDocsCursor();
	GLinkCursor lcur;
	GLinks* links_out=0;
	int size=0;

	if(( !Inited) && (Session->GetCount("htmlsbylinks") ))
	{
		// Create a container of  outgoing links for all document present in the system
		size = cur.GetNb() * sizeof(GLinks);
		for (cur.Start();!cur.End();cur.Next())
		{
			Links_Out->InsertPtr( new GLinks( cur() ));
		}

			// For each page p set all the pages qi pointed by p
		for (cur.Start();!cur.End();cur.Next())
		{
			if (!cur()->GetNbLinks()) continue;
			links_out=Links_Out->GetPtr(cur()->GetId());
			lcur = cur()->GetLinkCursor();
			size += lcur.GetNb() * sizeof(GLink);

			for (lcur.Start();!lcur.End();lcur.Next())
			{
				//  out_going links
				links_out->InsertPtr(lcur());
			}
		}

		Inited =true;

		cout << "la taille d'un entier" << sizeof(int)<<endl;
		cout << "la taille du graph: "<<size<<endl;
	}
}


//-----------------------------------------------------------------------------
void GLinkCalc::AddDoc(GDoc* doc)
{
	GLinkCursor lcur;
	GLinks* links_out=0;

	if (!doc->GetNbLinks()) return;

	links_out=Links_Out->GetPtr(doc->GetId());
	if (! links_out)
	{
		Links_Out->InsertPtr(new GLinks(doc) );
		links_out =Links_Out->GetPtr(doc->GetId());
		lcur= doc->GetLinkCursor();

		for (lcur.Start();!lcur.End();lcur.Next())
		{
			//  out_going links
			links_out->InsertPtr(lcur());
		}
	}
	else return;
}


//-----------------------------------------------------------------------------
void GLinkCalc::Connect(GSession* session)
{
	Session=session;
}


//-----------------------------------------------------------------------------
void GLinkCalc::Disconnect(GSession*)
{
	Session=0;
}


//-----------------------------------------------------------------------------
GALILEI::GLinkCalc::~GLinkCalc(void)
{
	delete Links_Out;
}


