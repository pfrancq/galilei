/*

	GALILEI Research Project

	GLinkCalcManager.cpp

	Link Methods Manager - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/glinkcalcmanager.h>
#include <docs/glinkcalc.h>
using namespace GALILEI;
using namespace R;
using namespace ltmm;



//------------------------------------------------------------------------------
//
// class GLinkCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLinkCalcManager::GLinkCalcManager(const char* path,bool dlg) throw(bad_alloc,GException)
	: RContainer<GFactoryLinkCalc,unsigned int,true,true>(10,5), Current(0)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;
	RString Name;
	char DlgLib[100];
	int len;

	loader<>& l=loader<>::instance();
	Path+="/linking";
	dp=opendir(Path);
	Path+="/";
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		len=strlen(ep->d_name);
		if(strcmp(&ep->d_name[len-3],".la")) continue;
		if(!strcmp(&ep->d_name[len-7],"_dlg.la")) continue;
		try
		{
			// Create the factory and insert it
			Name=Path+ep->d_name;
			handle<>& myhandle = l.load(Name);
			symbol* myinit   = myhandle.find_symbol("FactoryCreate");
			GFactoryLinkCalc* myfactory = ((GFactoryLinkCalcInit)(*(*myinit)))(this,ep->d_name);
			if(strcmp(API_LINKCALC_VERSION,myfactory->GetAPIVersion()))
			{
				Msg+=ep->d_name;
				Msg+=" - Plugin not compatible with API Version\n";
				continue;
			}
			InsertPtr(myfactory);

			// Look if dialog boxes are available
			if(!dlg) continue;
			try
			{
				strcpy(DlgLib,Name);
				DlgLib[Name.GetLen()-3]=0;
				strcat(DlgLib,"_dlg.la");
				handle<>& myhandle2 = l.load(DlgLib);
				myfactory->SetAbout(myhandle2.find_symbol("About"));
				myfactory->SetConfig(myhandle2.find_symbol("Configure"));
			}
			catch(...)
			{
			}
		}
		catch(std::exception& e)
		{
			Msg+=ep->d_name;
			Msg+=" - ";
			Msg+=e.what();
			Msg+="\n";
		}
	}
	closedir(dp);

	// If something in Msg -> error
	if(Msg.GetLen())
	{
		cout<<Msg<<endl;
		throw(GException(Msg));
	}
}


//------------------------------------------------------------------------------
void GLinkCalcManager::Connect(GSession* session) throw(GException)
{
	GFactoryLinkCalcCursor Cur;
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
	GFactoryLinkCalcCursor Cur;
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
GFactoryLinkCalcCursor& GLinkCalcManager::GetLinkCalcsCursor(void)
{
	GFactoryLinkCalcCursor *cur=GFactoryLinkCalcCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
GLinkCalcManager::~GLinkCalcManager(void)
{
}
