/*

	R Project Library

	GProfileCalcManager.cpp

	Generic Manager to handle URL file - Implementation.

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
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;
using namespace ltmm;



//-----------------------------------------------------------------------------
//
// class GProfileCalcManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfileCalcManager::GProfileCalcManager(const char* path) throw(GException)
	: RContainer<GFactoryProfileCalc,unsigned int,true,true>(10,5), Current(0)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;

	loader<>& l=loader<>::instance();
	Path+="/profiling";
	l.addto_search_path(Path());
	dp=opendir(Path);
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		if(strcmp(&ep->d_name[strlen(ep->d_name)-3],".la")) continue;
		try
		{
			handle<>& myhandle = l.load(ep->d_name);
			symbol* myinit   = myhandle.find_symbol("FactoryCreate");
			GFactoryProfileCalc* myfactory = ((GFactoryProfileCalcInit)(*(*myinit)))(this,ep->d_name);
			if(strcmp(API_PROFILECALC_VERSION,myfactory->GetAPIVersion()))
			{
				Msg+=ep->d_name;
				Msg+=" - Plugin not compatible with API Version\n";
				continue;
			}
			InsertPtr(myfactory);
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
void GProfileCalcManager::Connect(GSession* session)
{
	GFactoryProfileCalcCursor Cur;
	GProfileCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcManager::Disconnect(GSession* session)
{
	GFactoryProfileCalcCursor Cur;
	GProfileCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
void GProfileCalcManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryProfileCalc* fac;
	GProfileCalc* tmp;

	fac=GetPtr<const char*>(name);
	if(fac)
		tmp=fac->GetPlugin();
	else
		tmp=0;
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
	Current=tmp;
}


//-----------------------------------------------------------------------------
GProfileCalc* GProfileCalcManager::GetCurrentMethod(void)
{
	return(Current);
}


//-----------------------------------------------------------------------------
GFactoryProfileCalcCursor& GProfileCalcManager::GetProfileCalcsCursor(void)
{
	GFactoryProfileCalcCursor *cur=GFactoryProfileCalcCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GProfileCalcManager::~GProfileCalcManager(void)
{
}
