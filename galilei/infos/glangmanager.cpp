/*

	GALILEI Research Project

	GLangManager.h

	Manager for language plugins - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
// include file for ANSI C/C++
#include <string.h>
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include file for Galilei
#include <infos/glangmanager.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GLangManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLangManager::GLangManager(RContainer<RString, true, false>* paths,bool load,bool dlg) throw(std::bad_alloc,GException)
  : RContainer<GFactoryLang,true,true>(10,5),GPluginManager("Lang",paths), Load(load)
{
	for(paths->Start(); !paths->End(); paths->Next())
	{
		RString Path((*paths)());
		Path+="/langs";
		LoadPlugins<GFactoryLang,GFactoryLangInit,GLangManager>(this,Path.Latin1(),API_LANG_VERSION, dlg);
	}
}


//------------------------------------------------------------------------------
void GLangManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryLang> Cur;
	GLang* lang;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		lang=Cur()->GetPlugin();
		if(lang)
			lang->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GLangManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryLang> Cur;
	GLang* lang;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		lang=Cur()->GetPlugin();
		if(lang)
			lang->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
GLang* GLangManager::GetLang(const char* code) const
{
	GFactoryLang* fac;

	if(!code)
		return(0);
	fac=GetPtr<const char*>(code);
	if(!fac)
		return(0);
	return(fac->GetPlugin());
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryLang> GLangManager::GetLangsCursor(void)
{
	R::RCursor<GFactoryLang> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GLangManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryLang> Cur;

	if(!t) return;
	Cur=GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GLangManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryLang> Cur;
	Cur=GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GLangManager::~GLangManager(void)
{
}
