/*

	R Project Library

	GGroupCalcManager.cpp

	Manager to handle group computing method - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupcalc.h>
using namespace GALILEI;
using namespace R;
using namespace ltmm;



//-----------------------------------------------------------------------------
//
// class GGroupCalcManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroupCalcManager::GGroupCalcManager(const char* path,bool dlg) throw(GException)
	: RContainer<GFactoryGroupCalc,unsigned int,true,true>(10,5), Current(0)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;
	RString Name;
	char DlgLib[100];
	int len;

	loader<>& l=loader<>::instance();
	Path+="/groups";
	dp=opendir(Path);
	Path+="/";
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		// Create the factory and insert it
		len=strlen(ep->d_name);
		if(strcmp(&ep->d_name[len-3],".la")) continue;
		if(!strcmp(&ep->d_name[len-7],"_dlg.la")) continue;
		try
		{
			Name=Path+ep->d_name;
			handle<>& myhandle = l.load(Name);
			symbol* myinit   = myhandle.find_symbol("FactoryCreate");
			GFactoryGroupCalc* myfactory = ((GFactoryGroupCalcInit)(*(*myinit)))(this,ep->d_name);
			if(strcmp(API_GROUPCALC_VERSION,myfactory->GetAPIVersion()))
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
				if(!dlg) continue;
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
		throw(GException(Msg));
}


//-----------------------------------------------------------------------------
void GGroupCalcManager::Connect(GSession* session)
{
	GFactoryGroupCalcCursor Cur;
	GGroupCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
void GGroupCalcManager::Disconnect(GSession* session)
{
	GFactoryGroupCalcCursor Cur;
	GGroupCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
GGroupCalc* GGroupCalcManager::GetCurrentMethod(void)
{
	return(Current);
}


//-----------------------------------------------------------------------------
GFactoryGroupCalcCursor& GGroupCalcManager::GetGroupCalcsCursor(void)
{
	GFactoryGroupCalcCursor *cur=GFactoryGroupCalcCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroupCalcManager::~GGroupCalcManager(void)
{
}
