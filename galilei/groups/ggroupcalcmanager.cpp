/*

	GALILEI Research Project

	GGroupCalcManager.cpp

	Manager to handle group computing method - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupcalc.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GGroupCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupCalcManager::GGroupCalcManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryGroupCalc,true,true>(10,5),GPluginManager("GroupCalc",paths), Current(0)
{
	RCursor<RString> Cur(*paths);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RString Path(Cur());
		Path+="/groups";
		LoadPlugins<GFactoryGroupCalc,GFactoryGroupCalcInit,GGroupCalcManager>(this,Path.Latin1(),API_GROUPCALC_VERSION, dlg);
	}

}


//------------------------------------------------------------------------------
void GGroupCalcManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryGroupCalc> Cur;
	GGroupCalc* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GGroupCalcManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryGroupCalc> Cur;
	GGroupCalc* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GGroupCalcManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryGroupCalc* fac;
	GGroupCalc* tmp;

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
GGroupCalc* GGroupCalcManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryGroupCalc> GGroupCalcManager::GetGroupCalcsCursor(void)
{
	R::RCursor<GFactoryGroupCalc> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
void GGroupCalcManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryGroupCalc> Cur;

	if(!t) return;
	Cur=GetGroupCalcsCursor();
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
void GGroupCalcManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryGroupCalc> Cur;
	Cur=GetGroupCalcsCursor();
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
GGroupCalcManager::~GGroupCalcManager(void)
{
}
