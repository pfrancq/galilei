/*

	GALILEI Research Project

	GALILEI.cpp

	GALILEI Header - Implementation.

	Copyright 2005-2013 by Pascal Francq (pascal@francq.info).
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
// Events pointers
//
//------------------------------------------------------------------------------
const hNotification GALILEI::eCreateDoc(NotificationCenter.GetNotificationHandle("CreateDoc"));
const hNotification GALILEI::eNewDoc(NotificationCenter.GetNotificationHandle("NewDoc"));
const hNotification GALILEI::eUpdateDoc(NotificationCenter.GetNotificationHandle("UpdateDoc"));
const hNotification GALILEI::eDocModified(NotificationCenter.GetNotificationHandle("DocModified"));
const hNotification GALILEI::eDelDoc(NotificationCenter.GetNotificationHandle("DelDoc"));
const hNotification GALILEI::eDestroyDoc(NotificationCenter.GetNotificationHandle("DestroyDoc"));
const hNotification GALILEI::eSelectDoc(NotificationCenter.GetNotificationHandle("SelectDoc"));
const hNotification GALILEI::eUnselectDoc(NotificationCenter.GetNotificationHandle("UnselectDoc"));
const hNotification GALILEI::eCreateTopic(NotificationCenter.GetNotificationHandle("CreateTopic"));
const hNotification GALILEI::eNewTopic(NotificationCenter.GetNotificationHandle("NewTopic"));
const hNotification GALILEI::eUpdateTopic(NotificationCenter.GetNotificationHandle("UpdateTopic"));
const hNotification GALILEI::eTopicModified(NotificationCenter.GetNotificationHandle("TopicModified"));
const hNotification GALILEI::eDelTopic(NotificationCenter.GetNotificationHandle("DelTopic"));
const hNotification GALILEI::eDestroyTopic(NotificationCenter.GetNotificationHandle("DestroyTopic"));
const hNotification GALILEI::eCreateUser(NotificationCenter.GetNotificationHandle("CreateUser"));
const hNotification GALILEI::eNewUser(NotificationCenter.GetNotificationHandle("NewUser"));
const hNotification GALILEI::eUpdateUser(NotificationCenter.GetNotificationHandle("UpdateUser"));
const hNotification GALILEI::eUserModified(NotificationCenter.GetNotificationHandle("UserModified"));
const hNotification GALILEI::eDelUser(NotificationCenter.GetNotificationHandle("DelUser"));
const hNotification GALILEI::eDestroyUser(NotificationCenter.GetNotificationHandle("DestroyUser"));
const hNotification GALILEI::eCreateProfile(NotificationCenter.GetNotificationHandle("CreateProfile"));
const hNotification GALILEI::eNewProfile(NotificationCenter.GetNotificationHandle("NewProfile"));
const hNotification GALILEI::eUpdateProfile(NotificationCenter.GetNotificationHandle("UpdateProfile"));
const hNotification GALILEI::eProfileModified(NotificationCenter.GetNotificationHandle("ProfileModified"));
const hNotification GALILEI::eDelProfile(NotificationCenter.GetNotificationHandle("DelProfile"));
const hNotification GALILEI::eDestroyProfile(NotificationCenter.GetNotificationHandle("DestroyProfile"));
const hNotification GALILEI::eCreateCommunity(NotificationCenter.GetNotificationHandle("CreateCommunity"));
const hNotification GALILEI::eNewCommunity(NotificationCenter.GetNotificationHandle("NewCommunity"));
const hNotification GALILEI::eUpdateCommunity(NotificationCenter.GetNotificationHandle("UpdateCommunity"));
const hNotification GALILEI::eCommunityModified(NotificationCenter.GetNotificationHandle("CommunityModified"));
const hNotification GALILEI::eDelCommunity(NotificationCenter.GetNotificationHandle("DelCommunity"));
const hNotification GALILEI::eDestroyCommunity(NotificationCenter.GetNotificationHandle("DestroyCommunity"));
const hNotification GALILEI::eCreateClass(NotificationCenter.GetNotificationHandle("CreateClass"));
const hNotification GALILEI::eNewClass(NotificationCenter.GetNotificationHandle("NewClass"));
const hNotification GALILEI::eUpdateClass(NotificationCenter.GetNotificationHandle("UpdateClass"));
const hNotification GALILEI::eClassModified(NotificationCenter.GetNotificationHandle("ClassModified"));
const hNotification GALILEI::eDelClass(NotificationCenter.GetNotificationHandle("DelClass"));
const hNotification GALILEI::eDestroyClass(NotificationCenter.GetNotificationHandle("DestroyClass"));



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
	mThrowGException("'"+RString::Number(type)+"' is not a valid object type value");
}


//------------------------------------------------------------------------------
tObjType GALILEI::ObjType_cast(size_t objtype)
{
	if(objtype>=otReference)
		mThrowGException("'"+RString::Number(objtype)+"' is not a valid object type value");
	return(static_cast<tObjType>(objtype));
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
	mThrowGException("'"+RString::Number(state)+"' is not a valid state value");
}


//------------------------------------------------------------------------------
RString GALILEI::GetEvent(hNotification event,bool upper)
{
	if(event==eNewDoc)
		return(Str("document created in memory",upper));
	if(event==eCreateDoc)
		return(Str("document created in system",upper));
	if(event==eDocModified)
		return(Str("document modified",upper));
	if(event==eDelDoc)
		return(Str("document deleted from memory",upper));
	if(event==eDestroyDoc)
		return(Str("document deleted from system",upper));
	if(event==eSelectDoc)
		return(Str("document selected from subjects",upper));
	if(event==eUnselectDoc)
		return(Str("document unselected from subject",upper));
	return("'"+RString(NotificationCenter.GetNotificationName(event)())+"' is not a valid GALILEI event");
}


//------------------------------------------------------------------------------
RString GALILEI::GetConceptCat(tConceptCat cat,bool upper,bool plural)
{
	switch(cat)
	{
		case ccText:
			return(Str("text",upper));
		case ccMetadata:
			return(Str("metadata",upper));
		case ccSemantic:
			if(plural)
				return(Str("semantic rules",upper));
			return(Str("semantic rule",upper));
		case ccLink:
			if(plural)
				return(Str("links",upper));
			return(Str("link",upper));
	}
	mThrowGException("'"+RString::Number(cat)+"' is not a valid concept category value");
}


//------------------------------------------------------------------------------
tConceptCat GALILEI::ConceptCat_cast(size_t cat)
{
	if(cat>=4)
		mThrowGException("'"+RString::Number(cat)+"' is not a valid concept category value");
	return(static_cast<tConceptCat>(cat));
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
	mThrowGException("'"+RString::Number(type)+"' is not a valid feedback type value");
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
		default: mThrowGException("'"+RString::Number(type)+"' is not a valid feedback type value");
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
	mThrowGException("'"+RString::Number(type)+"' is not a valid profile type value");
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
