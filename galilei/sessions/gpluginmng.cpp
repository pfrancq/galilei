/*

	GALILEI Research Project

	GPluginMng.cpp

	Plugin Manager for GALILEI - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gpluginmng.h>
#include <docs/gdocanalyse.h>
#include <docs/gwordsclustering.h>
#include <docs/glinkcalc.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <profiles/gprofilecalc.h>
#include <filters/gurlmanager.h>
#include <filters/gfilter.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GPluginMng
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPluginMng::GPluginMng(const char*) throw(bad_alloc,GException)
	:  ProfileCalcs(0), ProfileCalc(0), Groupings(0), Grouping(0), Mng(0), DocAnalyse(0)

{
	ProfileCalcs=new RContainer<GProfileCalc,unsigned int,true,true>(3,3);
	Groupings=new RContainer<GGrouping,tId,true,true>(3,3);
	GroupCalcs=new RContainer<GGroupCalc,tId,true,true>(2,3);
	LinkCalcs=new RContainer<GLinkCalc,unsigned int,true,true>(3,2);
//	DocAnalyse=new GDocAnalyse(this,DocOptions);
}


//-----------------------------------------------------------------------------
void GPluginMng::SetCurrentComputingMethod(const char* name) throw(GException)
{
	GProfileCalc* tmp;

	tmp=ProfileCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
	ProfileCalc=tmp;
}


//-----------------------------------------------------------------------------
GProfileCalcCursor& GPluginMng::GetComputingsCursor(void)
{
	GProfileCalcCursor *cur=GProfileCalcCursor::GetTmpCursor();
	cur->Set(ProfileCalcs);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GPluginMng::SetCurrentGroupingMethod(const char* name) throw(GException)
{
	GGrouping* tmp;

	tmp=Groupings->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Grouping method '")+name+"' doesn't exists.");
	Grouping=tmp;
}


//-----------------------------------------------------------------------------
GGroupingCursor& GPluginMng::GetGroupingsCursor(void)
{
	GGroupingCursor *cur=GGroupingCursor::GetTmpCursor();
	cur->Set(Groupings);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GPluginMng::SetCurrentGroupCalcMethod(const char* name) throw(GException)
{
	GGroupCalc* tmp;

	tmp=GroupCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Group Description method '")+name+"' doesn't exists.");
	GroupCalc=tmp;
}


//-----------------------------------------------------------------------------
GGroupCalcCursor& GPluginMng::GetGroupCalcsCursor(void)
{
	GGroupCalcCursor *cur=GGroupCalcCursor::GetTmpCursor();
	cur->Set(GroupCalcs);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GPluginMng::SetCurrentLinkCalcMethod(const char* name) throw(GException)
{
	GLinkCalc* tmp;

	tmp=LinkCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Link Description method '")+name+"' doesn't exists.");
	LinkCalc=tmp;
}


//-----------------------------------------------------------------------------
GLinkCalcCursor& GPluginMng::GetLinkCalcsCursor(void)
{
	GLinkCalcCursor *cur=GLinkCalcCursor::GetTmpCursor();
	cur->Set(LinkCalcs);
	return(*cur);
}


//-----------------------------------------------------------------------------
GPluginMng::~GPluginMng(void) throw(GException)
{
	if(DocAnalyse) delete DocAnalyse;
	if(Groupings) delete Groupings;
	if(GroupCalcs) delete GroupCalcs;
	if(ProfileCalcs) delete ProfileCalcs;
}
