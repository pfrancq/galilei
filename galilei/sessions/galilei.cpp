/*

	GALILEI Research Project

	GALILEI.cpp

	GALILEI Header - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gsession.h>
#include <gsignalhandler.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// Static Variables
//
//------------------------------------------------------------------------------
R::RContainer<GSignalHandler,false,false> GSession::Handlers(30,20);



//------------------------------------------------------------------------------
//
// General functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
R::RString GALILEI::GetObjType(tObjType objtype)
{
	switch(objtype)
	{
		case otNoClass:
			return(RString("unknown"));
		case otSession:
			return(RString("session"));
		case otDoc:
			return(RString("document"));
		case otUser:
			return(RString("user"));
		case otProfile:
			return(RString("profile"));
		case otHistoricalProfile:
			return(RString("historical profile"));
		case otCommunity:
			return(RString("community"));
		case otHistoricalCommunity:
			return(RString("historical community"));
		case otFdbk:
			return(RString("assessment"));
		case otLang:
			return(RString("language"));
		case otHistory:
			return(RString("history"));
		case otTopic:
			return(RString("topic"));
		case otSubject:
			return(RString("subject"));
		case otEngine:
			return(RString("search engine"));
		case otEngineDoc:
			return(RString("document proposed by a search engine"));
		case otConcept:
			return(RString("concept"));
		case otConceptType:
			return(RString("concept type"));
		case otRelation:
			return(RString("relation"));
		case otRelationType:
			return(RString("relation type"));
	}
	return(RString("unknown"));
}


//------------------------------------------------------------------------------
R::RString GALILEI::GetState(tObjState state)
{
	switch(state)
	{
		case osUnknow:
			return(RString("unknown"));
		case osCreated:
			return(RString("created"));
		case osNew:
			return(RString("allocated"));
		case osUpToDate:
			return(RString("up to date"));
		case osModified:
			return(RString("modified"));
		case osUpdated:
			return(RString("updated"));
		case osSaved:
			return(RString("saved"));
		case osDelete:
			return(RString("deleted"));
		case osDestroyed:
			return(RString("destroyed"));
		case osOnDemand:
			return(RString("demands information"));
		case osLoaded:
			return(RString("information loaded"));
		case osNeedLoad:
			return(RString("must load information"));
	}
	return(RString("unknown"));
}


//------------------------------------------------------------------------------
R::RString GALILEI::GetEvent(tEvent event)
{
	switch(event)
	{
		case eUnknow:
			return(RString("Unknown"));
		case eObjNew:
			return(RString("object created in memory"));
		case eObjCreated:
			return(RString("object created in system"));
		case eObjModified:
			return(RString("object modified"));
		case eObjDelete:
			return(RString("object deleted from memory"));
		case eObjDestroyed:
			return(RString("object deleted from system"));
	}
	return(RString("unknown"));
}


//------------------------------------------------------------------------------
R::RString GALILEI::GetAssessment(tDocAssessment assessment)
{
	RString str;

	if(assessment&djUnknow)
		str="unknown";
	if(assessment&djOK)
	str="relevant";
	if(assessment&djKO)
		str="fuzzy relevant";
	if(assessment&djOutScope)
		str="irrelevant";
	if(assessment&djHub)
		str+="/hub";
	if(assessment&djAuthority)
		str+="/authority";
	return(str);
}


//------------------------------------------------------------------------------
R::RString GALILEI::GetAssessmentCode(tDocAssessment assessment)
{
	RString code;

	if(assessment&djOK)
		code="O";
	if(assessment&djKO)
		code="K";
	if(assessment&djOutScope)
		code="H";
	if(assessment&djHub)
		code+="H";
	if(assessment&djAuthority)
		code+="A";
	return(code);
}


//-----------------------------------------------------------------------------
tDocAssessment GALILEI::GetAssessmentType(const R::RString& assessment)
{
	tDocAssessment jug;

	// Normal assessment
	switch(assessment[static_cast<size_t>(0)].Unicode())
	{
		case 'O':
			jug=djOK;
			break;
		case 'K':
			jug=djKO;
			break;
		case 'H':
			jug= djOutScope;
			break;
		default:
			jug=djUnknow;
			break;
	}
	if(assessment.GetLen()==1)
		return(jug);

	// Eventually hub or authority
	switch(assessment[static_cast<size_t>(1)].Unicode())
	{
		case 'H':
			jug = tDocAssessment(jug|djHub);
			break;
		case 'A':
			jug = tDocAssessment(jug|djAuthority);
			break;
		default:
			break;
	}
	return(jug);
}


//-----------------------------------------------------------------------------
RString GALILEI::GetMeasureType(tTypeMeasure type)
{
	switch(type)
	{
		case tmUnknow :
			return(RString("Unknown measure."));
		case tmDoc :
			return(RString("Measure(document,document)"));
		case tmProfile :
			return(RString("Measure(profile,profile)"));
		case tmDocProfile :
			return(RString("Measure(document,profile)"));
		case tmCommunity :
			return(RString("Measure(community,community)"));
		case tmDocCommunity :
			return(RString("Measure(document,community)"));
		case tmProfileCommunity :
			return(RString("Measure(profile,community)"));
		case tmTopic :
			return(RString("Measure(topic,topic)"));
		case tmDocTopic :
			return(RString("Measure(document,topic)"));
	}
	return(RString::Null);
}
