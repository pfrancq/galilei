/*

	R Project Library

	GProfileCalcManager.cpp

	Profile Computing Methods Manager - Implementation.

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
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GProfileCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfileCalcManager::GProfileCalcManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryProfileCalc,true,true>(10,5), Current(0)
{
	RString Path(path);
	Path+="/profiling";

	LoadPlugins<GFactoryProfileCalc,GFactoryProfileCalcInit,GProfileCalcManager>(this,Path.Latin1(),API_PROFILECALC_VERSION, dlg);

}


//------------------------------------------------------------------------------
void GProfileCalcManager::Connect(GSession* session) throw(GException)
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


//------------------------------------------------------------------------------
void GProfileCalcManager::Disconnect(GSession* session) throw(GException)
{
	GFactoryProfileCalcCursor Cur;
	GProfileCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GProfileCalcManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryProfileCalc* fac;
	GProfileCalc* tmp;

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
GProfileCalc* GProfileCalcManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
GFactoryProfileCalcCursor GProfileCalcManager::GetProfileCalcsCursor(void)
{
	GFactoryProfileCalcCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GProfileCalcManager::~GProfileCalcManager(void)
{
}
