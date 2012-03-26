/*

	GALILEI Research Project

	GALILEI.cpp

	GALILEI Header - Implementation.

	Copyright 2005-2012 by Pascal Francq (pascal@francq.info).
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
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// Some null pointers
//
//------------------------------------------------------------------------------
const GConcept* GALILEI::pConcept(0);
const GConceptType* GALILEI::pConceptType(0);
const GDoc* GALILEI::pDoc(0);
const GClass* GALILEI::pClass(0);
const GTopic* GALILEI::pTopic(0);
const GUser* GALILEI::pUser(0);
const GProfile* GALILEI::pProfile(0);
const GCommunity* GALILEI::pCommunity(0);
const GSubject* GALILEI::pSubject(0);



//------------------------------------------------------------------------------
//
// General functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RString Str(const char* str,bool upper)
{
	RString string(*str);
	if(upper)
		string=string.ToUpper();
	string+=(str+1);
	return(string);
}


//------------------------------------------------------------------------------
RString GALILEI::GetObjType(tObjType type,bool upper,bool plural)
{
	switch(type)
	{
		case otNoClass:
			return(Str("unknown",upper));
		case otAnyClass:
			return(Str("any class",upper));
		case otSession:
			if(plural)
				return(Str("sessions",upper));
			return(Str("session",upper));
		case otDoc:
			if(plural)
				return(Str("documents",upper));
			return(Str("document",upper));
		case otUser:
			if(plural)
				return(Str("users",upper));
			return(Str("user",upper));
		case otProfile:
			if(plural)
				return(Str("profiles",upper));
			return(Str("profile",upper));
		case otCommunity:
			if(plural)
				return(Str("communities",upper));
			return(Str("community",upper));
		case otFdbk:
			if(plural)
				return(Str("feedbacks",upper));
			return(Str("feedback",upper));
		case otLang:
			if(plural)
				return(Str("languages",upper));
			return(Str("language",upper));
		case otTopic:
			if(plural)
				return(Str("topics",upper));
			return(Str("topic",upper));
		case otSubject:
			if(plural)
				return(Str("subjects",upper));
			return(Str("subject",upper));
		case otEngine:
			if(plural)
				return(Str("search engines",upper));
			return(Str("search engine",upper));
		case otEngineDoc:
			if(plural)
				return(Str("documents proposed by a search engine",upper));
			return(Str("document proposed by a search engine",upper));
		case otConcept:
			if(plural)
				return(Str("concepts",upper));
			return(Str("concept",upper));
		case otConceptType:
			if(plural)
				return(Str("concept types",upper));
			return(Str("concept type",upper));
		case otConceptCat:
			if(plural)
				return(Str("concept categories",upper));
			return(Str("concept category",upper));
		case otPredicate:
			if(plural)
				return(Str("predicates",upper));
			return(Str("predicate",upper));
		case otStatement:
			if(plural)
				return(Str("statements",upper));
			return(Str("statement",upper));
		case otClass:
			if(plural)
				return(Str("classes",upper));
			return(Str("class",upper));
		case otTree:
			if(plural)
				return(Str("concept trees",upper));
			return(Str("concept tree",upper));
		case otSuggestion:
			if(plural)
				return(Str("suggestions",upper));
			return(Str("suggestion",upper));
		case otDocRanking:
			if(plural)
				return(Str("document rankings",upper));
			return(Str("document ranking",upper));
		case otDescFile:
			if(plural)
				return(Str("description files",upper));
			return(Str("description file",upper));
		case otTreeFile:
			if(plural)
				return(Str("concept tree files",upper));
			return(Str("concept tree file",upper));
		case otIndexFile:
			if(plural)
				return(Str("index files",upper));
			return(Str("index file",upper));
		case otReference:
			if(plural)
				return(Str("references",upper));
			return(Str("reference",upper));

	}
	ThrowGException("'"+RString::Number(type)+"' is not a valid object type value");
}


//------------------------------------------------------------------------------
RString GALILEI::GetState(tObjState state,bool upper)
{
	switch(state)
	{
		case osUnknow:
			return(Str("unknown",upper));
		case osCreated:
			return(Str("created",upper));
		case osNew:
			return(Str("allocated",upper));
		case osUpToDate:
			return(Str("up to date",upper));
		case osModified:
			return(Str("modified",upper));
		case osUpdated:
			return(Str("updated",upper));
		case osSaved:
			return(Str("saved",upper));
		case osDelete:
			return(Str("deleted",upper));
		case osDestroyed:
			return(Str("destroyed",upper));
		case osOnDemand:
			return(Str("demands information",upper));
		case osLoaded:
			return(Str("information loaded",upper));
		case osNeedLoad:
			return(Str("must load information",upper));
	}
	ThrowGException("'"+RString::Number(state)+"' is not a valid state value");
}


//------------------------------------------------------------------------------
RString GEvent::GetEvent(tEvent event,bool upper)
{
	switch(event)
	{
		case eUnknow:
			return(Str("unknown",upper));
		case eObjNew:
			return(Str("object created in memory",upper));
		case eObjCreated:
			return(Str("object created in system",upper));
		case eObjModified:
			return(Str("object modified",upper));
		case eObjDelete:
			return(Str("object deleted from memory",upper));
		case eObjDestroyed:
			return(Str("object deleted from system",upper));
	}
	ThrowGException("'"+RString::Number(event)+"' is not a valid event type value");
}


//------------------------------------------------------------------------------
RString GALILEI::GetFdbkType(tFdbkType type,bool upper)
{
	switch(type)
	{
		case ftUnknown:
			return(Str("unknown",upper));
		case ftRelevant:
			return(Str("relevant",upper));
		case ftFuzzyRelevant:
			return(Str("fuzzy relevant",upper));
		case ftIrrelevant:
			return(Str("irrelevant",upper));
	}
	ThrowGException("'"+RString::Number(type)+"' is not a valid feedback type value");
}


//------------------------------------------------------------------------------
tFdbkType GALILEI::GetFdbkType(unsigned int type)
{
	switch(type)
	{
		case 0 : return(ftUnknown);
		case 1 : return(ftRelevant);
		case 2 : return(ftFuzzyRelevant);
		case 3 : return(ftIrrelevant);
		default: ThrowGException("'"+RString::Number(type)+"' is not a valid feedback type value");
	}
}


//------------------------------------------------------------------------------
RString GALILEI::GetProfileType(tProfileType type,bool upper,bool plural)
{
	switch(type)
	{
		case ptUnknown:
			return(Str("unknown",upper));
		case ptInterest:
			if(plural)
				return(Str("interests",upper));
			return(Str("interest",upper));
		case ptExpertise:
			if(plural)
				return(Str("expertise's",upper));
			return(Str("expertise",upper));
	}
	ThrowGException("'"+RString::Number(type)+"' is not a valid profile type value");
}


//------------------------------------------------------------------------------
tProfileType GALILEI::GetProfileType(unsigned int type)
{
	switch(type)
	{
		case 0 : return(ptUnknown);
		case 1 : return(ptInterest);
		case 2 : return(ptExpertise);
		default: return(ptUnknown);
	}
}
