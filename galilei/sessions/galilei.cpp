/*

	GALILEI Research Project

	GALILEI.cpp

	GALILEI Header  - Implementation.

	Copyright 2005 by the Université libre de Bruxelles.

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
// include files for GALILEI
#include <galilei.h>
#include <gpluginmanagers.h>
#include <gsession.h>
#include <gsignalhandler.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// Static Variables
//
//------------------------------------------------------------------------------
R::RContainer<GSignalHandler,false,false> GSession::Handlers(30,20);
GPluginManagers GPluginManagers::PluginManagers;



//------------------------------------------------------------------------------
//
// General functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
R::RString GetObjType(tObjType objtype)
{
	switch(objtype)
	{
		case otNoClass:
			return(RString("unknow"));
			break;
		case otSession:
			return(RString("session"));
			break;
		case otDoc:
			return(RString("document"));
			break;
		case otDocs:
			return(RString("documents"));
			break;
		case otUsers:
			return(RString("users"));
			break;
		case otUser:
			return(RString("user"));
			break;
		case otProfile:
			return(RString("profile"));
			break;
		case otSubProfile:
			return(RString("subProfile"));
			break;
		case otGroups:
			return(RString("groups"));
			break;
		case otGroup:
			return(RString("group"));
			break;
		case otDocSubProfile:
			return(RString("document or subprofile"));
			break;
		case otDocGroup:
			return(RString("document or group"));
			break;
		case otSubProfileGroup:
			return(RString("subprofile or group"));
			break;
		case otFdbk:
			return(RString("assessment"));
			break;
		case otLang:
			return(RString("language"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetState(tObjState state)
{
	switch(state)
	{
		case osUnknow:
			return(RString("unknow"));
			break;
		case osCreated:
			return(RString("created"));
			break;
		case osUpToDate:
			return(RString("up to date"));
			break;
		case osModified:
			return(RString("modified"));
			break;
		case osUpdated:
			return(RString("updated"));
			break;
		case osSaved:
			return(RString("saved"));
			break;
		case osDelete:
			return(RString("deleted"));
			break;
		case osNotNeeded:
			return(RString("not needed"));
			break;
		case osOnDemand:
			return(RString("demands information"));
			break;
		case osNeedLoad:
			return(RString("must load information"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetEvent(tEvent event)
{
	switch(event)
	{
		case eUnknow:
			return(RString("Unknow"));
			break;
		case eObjCreated:
			return(RString("object created"));
			break;
		case eObjModified:
			return(RString("object modified"));
			break;
		case eObjDeleted:
			return(RString("object deleted"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetAssessment(tDocAssessment assessment)
{
	switch(assessment)
	{
		case djUnknow:
			return(RString("unknow"));
			break;
		case djOK:
			return(RString("relevant"));
			break;
		case djKO:
			return(RString("fuzzy relevant"));
			break;
		case djOutScope:
			return(RString("irrelevant"));
			break;
		case djHub:
			return(RString("hub"));
			break;
		case djAutority:
			return(RString("autority"));
			break;
		case djMaskJudg:
			return(RString("mask for assessments"));
			break;
		case djMaskHubAuto:
			return(RString("mask for hub/autority"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetInfoType(tInfoType infotype)
{
	switch(infotype)
	{
		case infoNothing:
			return(RString("unknow"));
			break;
		case infoWord:
			return(RString("word"));
			break;
		case infoWordList:
			return(RString("list of words"));
			break;
		case infoWordOccurs:
			return(RString("list of occurences of a word"));
			break;
		case infoDoc:
			return(RString("document"));
			break;
	}
	return(RString("unknow"));
}
