/*

	GALILEI Research Project

	GDocAnalyseManager.cpp

	Document Analysis Methods Manager - Implementation.

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
#include <docs/gdocanalysemanager.h>
#include <docs/gdocanalyse.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDocAnalyseManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocAnalyseManager::GDocAnalyseManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryDocAnalyse,true,true>(10,5), GPluginManager("DocAnalyse",path),Current(0)
{

	RString Path(path);
	Path+="/docs";

	LoadPlugins<GFactoryDocAnalyse,GFactoryDocAnalyseInit,GDocAnalyseManager>(this,Path.Latin1(),API_DOCANALYSE_VERSION, dlg);

}


//------------------------------------------------------------------------------
void GDocAnalyseManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryDocAnalyse> Cur;
	GDocAnalyse* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GDocAnalyseManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryDocAnalyse> Cur;
	GDocAnalyse* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GDocAnalyseManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryDocAnalyse* fac;
	GDocAnalyse* tmp;

	fac=GetPtr<const char*>(name);
	if(fac)
		tmp=fac->GetPlugin();
	else
		tmp=0;
	Current=tmp;
	if(!tmp)
		throw GException(RString("Document Analysing method '")+name+"' doesn't exists.");
}


//------------------------------------------------------------------------------
GDocAnalyse* GDocAnalyseManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryDocAnalyse> GDocAnalyseManager::GetDocAnalysesCursor(void)
{
	R::RCursor<GFactoryDocAnalyse> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GDocAnalyseManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryDocAnalyse> Cur;

	if(!t) return;
	Cur=GetDocAnalysesCursor();
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
void GDocAnalyseManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryDocAnalyse> Cur;
	Cur=GetDocAnalysesCursor();
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
GDocAnalyseManager::~GDocAnalyseManager(void)
{
}
