/*

	GALILEI Research Project

	GLinkCalcManager.cpp

	Link Methods Manager - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/glinkcalcmanager.h>
#include <docs/glinkcalc.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GLinkCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLinkCalcManager::GLinkCalcManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryLinkCalc,true,true>(10,5),GPluginManager("LinkCalc",paths), Current(0)
{
	for(paths->Start(); !paths->End(); paths->Next())
	{
		RString Path((*paths)());
		Path+="/linking";
		LoadPlugins<GFactoryLinkCalc,GFactoryLinkCalcInit,GLinkCalcManager>(this,Path.Latin1(),API_LINKCALC_VERSION, dlg);
	}
}


//------------------------------------------------------------------------------
void GLinkCalcManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryLinkCalc> Cur;
	GLinkCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GLinkCalcManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryLinkCalc> Cur;
	GLinkCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GLinkCalcManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryLinkCalc* fac;
	GLinkCalc* tmp;

	fac=GetPtr<const char*>(name);
	if(fac)
		tmp=fac->GetPlugin();
	else
		tmp=0;
	Current=tmp;
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
}


//------------------------------------------------------------------------------
GLinkCalc* GLinkCalcManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryLinkCalc> GLinkCalcManager::GetLinkCalcsCursor(void)
{
	R::RCursor<GFactoryLinkCalc> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GLinkCalcManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryLinkCalc> Cur;

	if(!t) return;
	Cur=GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
	try
	{
		SetCurrentMethod(t->GetAttrValue("current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GLinkCalcManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryLinkCalc> Cur;
	Cur=GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
	if(Current)
		t->InsertAttr("current",Current->GetFactory()->GetName());
	else
		t->InsertAttr("current","None");
}


//------------------------------------------------------------------------------
GLinkCalcManager::~GLinkCalcManager(void)
{
}
