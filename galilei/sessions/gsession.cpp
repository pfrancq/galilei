/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <fstream>
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <random.h>
#include <rcursor.h>
#include <rxmlfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <grelationtype.h>
#include <grelation.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <gstatscalc.h>
#include <gdoc.h>
#include <gdocstruct.h>
#include <gdocanalyse.h>
#include <gdocxml.h>
#include <glinkcalc.h>
#include <glink.h>
#include <gpostdoc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <genginedoc.h>
#include <guser.h>
#include <gprofile.h>
#include <gprofilecalc.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gfilter.h>
#include <gpostdoc.h>
#include <gweightinfo.h>
#include <gpostcommunity.h>
#include <gposttopic.h>
#include <ggalileiapp.h>
#include <gdebugobject.h>
#include <gindexer.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
// define
#define HANDLEALLEXCEPTIONS(rec,msg) 	                                       \
catch(GException& e)                                                           \
{                                                                              \
	if(rec)                                                                    \
	{                                                                          \
		cerr<<msg<<e.GetMsg()<<endl;                                           \
		rec->WriteStr(msg+e.GetMsg());                                         \
	}                                                                          \
	else                                                                       \
		throw GException(msg+e.GetMsg());                                      \
}                                                                              \
catch(RIOException& e)                                                         \
{                                                                              \
	if(rec)                                                                    \
	{                                                                          \
		cerr<<e.GetMsg()<<endl;                                                \
		rec->WriteStr(e.GetMsg());                                             \
	}                                                                          \
	else                                                                       \
		throw GException(e.GetMsg());                                          \
}                                                                              \
catch(RException& e)                                                           \
{                                                                              \
	if(rec)                                                                    \
	{                                                                          \
		cerr<<msg<<e.GetMsg()<<endl;                                           \
		rec->WriteStr(msg+e.GetMsg());                                         \
	}                                                                          \
	else                                                                       \
		throw GException(msg+e.GetMsg());                                      \
}                                                                              \
catch(exception& e)                                                            \
{                                                                              \
	if(rec)                                                                    \
	{                                                                          \
		cerr<<msg<<e.what()<<endl;                                             \
		rec->WriteStr(msg+e.what());                                           \
	}                                                                          \
	else                                                                       \
		throw GException(msg+e.what());                                        \
}                                                                              \
catch(...)                                                                     \
{                                                                              \
	if(rec)                                                                    \
		rec->WriteStr(msg+"Undefined error");                                  \
	else                                                                       \
		throw GException(msg+"Undefined error");                               \
}



//------------------------------------------------------------------------------
//
// class GDocRefURL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocRefURL
{
public:
	GDoc* Doc;

	GDocRefURL(GDoc* d) : Doc(d) {}
	int Compare(const GDocRefURL& doc) const {return(Doc->GetURL().Compare(doc.Doc->GetURL()));}
	int Compare(const char* url) const {return(Doc->GetURL().Compare(url));}
	~GDocRefURL(void) {}
};



//------------------------------------------------------------------------------
//
// GSession::Intern
//
//------------------------------------------------------------------------------

class GSession::Intern
{
public:
	GSubjects* Subjects;                                              // Subjects.
	int CurrentRandom;                                                // Current seek for this session.
	R::RRandom* Random;                                               // Random number generator
	static GSession* Session;                                         // Static pointer to the session
	static bool ExternBreak;                                          // Should the session stop as soon as possible?
	GSlot* Slot;                                                      // Slot for the session
	RDebug* Debug;                                                    // Debug output for the session
	RContainer<GDoc,true,true> Docs;                                  // Documents handled by the system.
	bool DocsLoaded;                                                  // Are the documents loaded?
	RContainer<GDocRefURL,true,true> DocsRefUrl;                      // Documents ordered by URL.
	RContainer<GUser,true,true> Users;                                // Users handled by the system.
	bool UsersLoaded;                                                 // Are the users and the profiles loaded?
	RContainer<GProfile,true,true> Profiles;                          // Profiles handled by the system.
	RContainer<GCommunity,true,true> Communities;	                  // Communities handled by the system.
	bool CommunitiesLoaded;                                           // Are the communities loaded?
	RContainer<GTopic,true,true> Topics;	                          // Topics handled by the system.
	bool TopicsLoaded;                                                // Are the topics loaded?
	RContainer<GConceptType,true,true> ConceptTypes;                  // Types of Concepts
	RContainer<GRelationType,true,true> RelationTypes;                // Types of Relations
	size_t MaxDocs;                                                   // Maximum number of documents to handle in memory.
	size_t MaxProfiles;                                               // Maximum number of profiles to handle in memory.
	size_t MaxGroups;                                                 // Maximum number of groups to handle in memory.
	GFilterManager* FilterManager;                                    // Pointer to the filter manager.
	bool ClusterSelectedDocs;                                         // Limit the clustering of the documents to the selected ones.

	Intern(size_t mdocs,size_t maxprof,size_t maxgroups,size_t d,size_t u,size_t p,size_t t,size_t c)
		: Subjects(0), Random(0),
		  Slot(0), Docs(d+(d/2),d/2), DocsLoaded(false), DocsRefUrl(d+(d/2),d/2),
		  Users(u,u/2), UsersLoaded(false), Profiles(p,p/2),
		  Communities(c+(c/2),c/2), CommunitiesLoaded(false),
		  Topics(t+(t/2),t/2), TopicsLoaded(false),
		  ConceptTypes(50,10), RelationTypes(10,5),
		  MaxDocs(mdocs), MaxProfiles(maxprof), MaxGroups(maxgroups), FilterManager(0)
	{
		CurrentRandom=0;
		Random=RRandom::Create(RRandom::Good,CurrentRandom);
	}

	~Intern(void)
	{
		delete Random;
		delete Subjects;
		Session=0;
		ExternBreak=false;
	}
};



//------------------------------------------------------------------------------
// Global variables
GSession* GSession::Intern::Session=0;
bool GSession::Intern::ExternBreak=false;
RContainer<GDebugObject,false,true> DebugObjs(100,100);                    // Objects given debugging information.



//------------------------------------------------------------------------------
//
// GSession::Constructor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::GSession(GSlot* slot,R::RDebug* debug,size_t maxdocs,size_t maxprofiles,size_t maxgroups)
	: GIndexer(), GOntology(Storage->GetNbSaved(otConcept)), Data(0)
{
	// Init Part
	Data=new Intern(maxdocs,maxprofiles,maxgroups,
			Storage->GetNbSaved(otDoc),
			Storage->GetNbSaved(otUser),
			Storage->GetNbSaved(otProfile),
			Storage->GetNbSaved(otTopic),
			Storage->GetNbSaved(otCommunity));

	Data->Slot=slot;
	Data->Debug=debug;
	Data->FilterManager=GALILEIApp->GetManager<GFilterManager>("Filter");

	if(!Intern::Session)
		Intern::Session=this;

	// Create the configurations
	SetConfigInfos("lib/galilei/sessions",Storage->GetWorld());
	InsertParam(new RParamValue("NbOK",10.0),"Subjects");
	InsertParam(new RParamValue("RelOK",true),"Subjects");
	InsertParam(new RParamValue("NbKO",10.0),"Subjects");
	InsertParam(new RParamValue("RelKO",true),"Subjects");
	InsertParam(new RParamValue("NbH",50.0),"Subjects");
	InsertParam(new RParamValue("RelH",true),"Subjects");
	InsertParam(new RParamValue("PercErr",0.0),"Subjects");
	InsertParam(new RParamValue("NbProfMin",2),"Subjects");
	InsertParam(new RParamValue("NbProfMax",10),"Subjects");
	InsertParam(new RParamValue("PercSocial",100.0),"Subjects");
	InsertParam(new RParamValue("NbSubjects",100.0),"Subjects");
	InsertParam(new RParamValue("RelSubjects",true),"Subjects");
	InsertParam(new RParamValue("NbMinDocsSubject",50),"Subjects");
	InsertParam(new RParamValue("NbDocsAssess",30),"Subjects");
	InsertParam(new RParamValue("SwitchPerc",5.0),"Subjects");
	InsertParam(new RParamValue("ManualSubjects",false),"Subjects");
	InsertParam(new RParamValue("NbDocsPerSubject",100.0),"Subjects");
	InsertParam(new RParamValue("PercNbDocsPerSubject",true),"Subjects");
	InsertParam(new RParamValue("ClusterSelectedDocs",false),"Subjects");
	InsertParam(new RParamList("SelectedSubjects"),"Subjects");
	InsertParam(new RParamValue("IndexDir","/var/galilei"),"Indexer");
	Load(false);
}



//------------------------------------------------------------------------------
//
// GSession::General
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::Apply(void)
{
	Data->ClusterSelectedDocs=GetBool("ClusterSelectedDocs","Subjects");
	if(Data->Subjects)
		Data->Subjects->Apply();
	GIndexer::Apply();
}


//------------------------------------------------------------------------------
void GSession::ForceReCompute(tObjType type)
{
	bool Break(true);

	switch(type)
	{
		case otDoc:
		{
			// Clear the information of the documents -> Also profiles and groups and topics
			RCursor<GDoc> Docs(Data->Docs);
			for(Docs.Start();!Docs.End();Docs.Next())
				Docs()->ClearInfos();
			if(SaveResults)
				GIndexer::Clear(otDoc);
			Break=false;
		}
		case otTopic:
		{
			// Delete the topics
			RCursor<GConceptType> Types(Data->ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->ClearRef(otTopic);
			Data->Topics.Clear();
			if(SaveResults)
			{
				GIndexer::Clear(otTopic);
				Storage->Clear(otTopic);
			}
			if(Break)
				break;
		}
		case otProfile:
		{
			// Delete the profiles -> Also groups
			RCursor<GConceptType> Types(Data->ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->ClearRef(otProfile);
			Data->Profiles.Clear();
			if(SaveResults)
			{
				GIndexer::Clear(otProfile);
				Storage->Clear(otProfile);
			}
		}
		case otCommunity:
		{
			// Delete the groups
			RCursor<GConceptType> Types(Data->ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->ClearRef(otCommunity);
			Data->Communities.Clear();
			if(SaveResults)
			{
				GIndexer::Clear(otCommunity);
				Storage->Clear(otCommunity);
			}
			break;
		}
		default:
			throw GException("Only 'otDoc', 'otUser' and 'otGroup' are allowed.");
	}
}


//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	if(Data->Subjects)
		Data->Subjects->ReInit();

	// Clear feedbacks
	ClearFdbks();

	RCursor<GConceptType> Types(Data->ConceptTypes);
	for(Types.Start();!Types.End();Types.Next())
	{
		Types()->ClearRef(otCommunity);
		Types()->ClearRef(otProfile);
	}

	// Clear groups, profiles and users
	Data->Communities.Clear();
	Data->Profiles.Clear();
	Data->Users.Clear();
}


//------------------------------------------------------------------------------
RString GSession::AnalyzeString(const RString& str)
{
	RString Res;

	for(size_t i=0;i<str.GetLen();i++)
	{
		if(str[i]=='%')
		{
			// Look if another %
			RString Rem;
			for(i++;(i<str.GetLen()&&str[i]!='%');i++)
				Rem+=str[i];

			// Is there another %?
			if(i==str.GetLen())
			{
				// No -> accident
				Res+='%'+Rem;
			}
			else
			{
				if(Rem=="world")
				{
					if(Storage)
						Res+=Storage->GetWorld();
					else
						Res+='%'+Rem+'%';
				}
				else
				{
					Res+='%'+Rem+'%';
				}
			}
		}
		else
			Res+=str[i];
	}
	return(Res);
}


//------------------------------------------------------------------------------
GSubjects* GSession::GetSubjects(bool load) const
{
	if((!Data->Subjects)&&(load))
	{
		Data->Subjects=new GSubjects(const_cast<GSession*>(this));
		if(Storage)
			const_cast<GSession*>(this)->Storage->LoadSubjects();
		Data->Subjects->Apply();
	}
	return(Data->Subjects);
}


//------------------------------------------------------------------------------
void GSession::SetSlot(GSlot* slot)
{
	Data->Slot=slot;
}


//------------------------------------------------------------------------------
GSlot* GSession::GetSlot(void) const
{
	return(Data->Slot);
}


//------------------------------------------------------------------------------
void GSession::SetDebug(R::RDebug* debug)
{
	Data->Debug=debug;
}


//------------------------------------------------------------------------------
R::RDebug* GSession::GetDebug(void) const
{
	return(Data->Debug);
}


//------------------------------------------------------------------------------
void GSession::AddDebugObject(const GDebugObject* debug)
{
	DebugObjs.InsertPtr(debug);
}


//------------------------------------------------------------------------------
void GSession::RemoveDebugObject(const GDebugObject* debug)
{
	DebugObjs.DeletePtr(*debug);
}


//------------------------------------------------------------------------------
RCursor<GDebugObject> GSession::GetDebugObjects(void) const
{
	return(RCursor<GDebugObject>(DebugObjs));
}


//------------------------------------------------------------------------------
void GSession::SetActiveDebugObject(const R::RString& name,bool active)
{
	GDebugObject* obj=DebugObjs.GetPtr(name);
	if(!obj)
		throw GException("No debugging object called '"+name+"'");
	if(active)
		obj->Debug=Data->Debug;
	else
		obj->Debug=0;
}


//------------------------------------------------------------------------------
void GSession::DebugInfo(const RString& name,const RString& info)
{
	GDebugObject* obj=DebugObjs.GetPtr(name);
	if(!obj)
		throw GException("No debugging object called '"+name+"'");
	RDebug* old(obj->Debug);
	obj->Debug=Data->Debug;
	obj->DebugInfo(info);
	obj->Debug=old;
}


//------------------------------------------------------------------------------
void GSession::SetCurrentRandom(int rand)
{
	Data->CurrentRandom=rand;
	Data->Random->Reset(Data->CurrentRandom);
}


//------------------------------------------------------------------------------
int GSession::GetCurrentRandom(void) const
{
	return(Data->CurrentRandom);
}


//------------------------------------------------------------------------------
int GSession::GetCurrentRandomValue(size_t max)
{
	return(int(Data->Random->GetValue(max)));
}


//------------------------------------------------------------------------------
R::RRandom* GSession::GetRandom(void) const
{
	return(Data->Random);
}



//------------------------------------------------------------------------------
//
// GSession::Generic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
size_t GSession::GetNbElements(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			return(Data->Docs.GetNb());
		case otProfile:
			return(Data->Profiles.GetNb());
		case otCommunity:
			return(Data->Communities.GetNb());
		case otTopic:
			return(Data->Topics.GetNb());
		default:
			throw GException("GSession::GetNbElements : Type "+GetObjType(type)+" is not handled");
	}
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxElementId(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			if(!Data->Docs.GetNb())
				return(0);
			return(Data->Docs[Data->Docs.GetMaxPos()]->GetId());
		case otProfile:
			if(!Data->Profiles.GetNb())
				return(0);
			return(Data->Profiles[Data->Profiles.GetMaxPos()]->GetId());
		case otCommunity:
			if(!Data->Communities.GetNb())
				return(0);
			return(Data->Communities[Data->Communities.GetMaxPos()]->GetId());
		case otTopic:
			if(!Data->Topics.GetNb())
				return(0);
			return(Data->Topics[Data->Topics.GetMaxPos()]->GetId());
		default:
			throw GException("GSession::GetMaxElementId : Type "+GetObjType(type)+" is not handled");
	}
}


//------------------------------------------------------------------------------
void* GSession::GetElement(tObjType type,size_t id,bool null) const
{
	switch(type)
	{
		case otDoc:
			return(GetDoc(id,null));
		case otProfile:
			return(GetProfile(id,null));
		case otCommunity:
			return(GetCommunity(id,null));
		case otTopic:
			return(GetTopic(id,null));
		default:
			throw GException("GSession::GetMaxElementId : Type "+GetObjType(type)+" is not handled");
	}
}


//------------------------------------------------------------------------------
void GSession::ClearGroups(tObjType type)
{
	switch(type)
	{
		case otTopic:
			ClearTopics();
			break;
		case otCommunity:
			ClearCommunities();
			break;
		default:
			throw GException("GSession::ClearGroups : Type "+GetObjType(type)+" is not handled");
	}
}


//------------------------------------------------------------------------------
void* GSession::NewGroup(tObjType type,const RString& name)
{
	switch(type)
	{
		case otTopic:
			return(new GTopic(cNoRef,name,RDate(""),RDate("")));
		case otCommunity:
			return(new GCommunity(cNoRef,name,RDate(""),RDate("")));
		default:
			throw GException("GSession::NewGroup : Type "+GetObjType(type)+" is not handled");
	}
}


//------------------------------------------------------------------------------
void GSession::InsertGroup(void* ptr,tObjType type)
{
	switch(type)
	{
		case otTopic:
			InsertTopic(static_cast<GTopic*>(ptr));
			break;
		case otCommunity:
			InsertCommunity(static_cast<GCommunity*>(ptr));
			break;
		default:
			throw GException("GSession::InsertGroup : Type "+GetObjType(type)+" is not handled");
	}
}



//------------------------------------------------------------------------------
//
// GSession::Knowledge
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RCursor<GRelationType> GSession::GetRelationTypes(void) const
{
	return(RCursor<GRelationType>(Data->RelationTypes));
}


//-----------------------------------------------------------------------------
GRelationType* GSession::GetRelationType(size_t id,bool null) const
{
	GRelationType* type=Data->RelationTypes.GetPtr(id);
	if(!type)
	{
		if(!null)
			throw GException("Unknown relation type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GRelationType* GSession::GetRelationType(const RString& name,bool null) const
{
	GRelationType* type=Data->RelationTypes.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			throw GException("Unknown relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
void GSession::InsertRelationType(size_t id,const R::RString& name,const R::RString& desc)
{
	Data->RelationTypes.InsertPtr(new GRelationType(id,name,desc));
}


//-----------------------------------------------------------------------------
void GSession::InsertRelation(size_t id,const R::RString& name,size_t subjectid,size_t type,size_t objectid,double weight)
{
	// Get the concept related to the subject
	GConcept* subject=GetConcept(subjectid);
	if(!subject)
		throw GException("Object "+RString::Number(subjectid)+" does not exist");

	// Get the concept related to the object
	GConcept* object=GetConcept(objectid);
	if(!object)
		throw GException("Object "+RString::Number(objectid)+" does not exist");

	GRelationType* relationttype=Data->RelationTypes.GetPtr(type);
	if(!relationttype)
		throw GException("Relation type "+RString::Number(type)+" does not exist");
	relationttype->InsertRelation(new GRelation(id,name,subject,type,object,weight));
}


//-----------------------------------------------------------------------------
GRelation* GSession::GetRelation(size_t id,size_t type,bool null)
{
	GRelationType* relationttype=Data->RelationTypes.GetPtr(type);
	if(!relationttype)
	{
		if(!null)
			throw GException("Relation "+RString::Number(id)+" of type "+RString::Number(type)+" does not exist");
		return(0);
	}
	GRelation* rel=relationttype->GetRelation(id);
	if((!rel)&&(!null))
		throw GException("Relation "+RString::Number(id)+" of type "+RString::Number(type)+" does not exist");
	return(rel);
}


//------------------------------------------------------------------------------
void GSession::GetRelations(R::RContainer<GRelation,false,false>& rel,GConcept* /*subject*/,size_t type,GConcept* /*object*/,bool /*sym*/)
{
	RCursor<GRelationType> Types(Data->RelationTypes);
	for(Types.Start();!Types.End();Types.Next())
	{
		if((type!=cNoRef)&&(type!=Types()->GetId()))
			continue;
		RCursor<GRelation> Rel(Types()->GetRelations());
		for(Rel.Start();!Rel.End();Rel.Next())
		{
/*			if((subject)&&(((*subject)!=(*Rel()->GetSubject()))||(sym&&((*subject)!=(*Rel()->GetObject())))))
				continue;
			if((object)&&(((*object)!=(*Rel()->GetObject()))||(sym&&((*object)!=(*Rel()->GetSubject())))))
				continue;*/
			rel.InsertPtr(Rel());
		}
	}
}



//------------------------------------------------------------------------------
//
// GSession::Documents
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::LoadDocs(void)
{
	if(Data->DocsLoaded)
		return;
	Storage->LoadDocs();
	Data->DocsLoaded=true;
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GSession::GetDocs(void) const
{
	return(R::RCursor<GDoc>(Data->Docs));
}


//-----------------------------------------------------------------------------
size_t GSession::GetNbDocs(void) const
{
	return(Data->Docs.GetNb());
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxDocId(void) const
{
	if(!Data->Docs.GetNb())
		return(0);
	return(Data->Docs[Data->Docs.GetMaxPos()]->GetId());
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxPosDoc(void) const
{
	return(Data->Docs.GetMaxPos());
}


//-----------------------------------------------------------------------------
size_t GSession::FillDocs(GDoc** docs)
{
	return(Data->Docs.GetTab(docs,1,Data->Docs.GetMaxPos()));
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(size_t id,bool load,bool null) const
{
	GDoc* d;

	if(id>Data->Docs.GetMaxPos())
		return(0);
	d=Data->Docs[id];
	if(d)
		return(d);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown document "+RString::Number(id));
	}
	d=Storage->LoadDoc(id);
	if(!d)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown document "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertDoc(d);
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(const char* url,bool,bool null) const
{
	GDocRefURL* ref;

	ref=Data->DocsRefUrl.GetPtr(url);
	if(!ref)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown document '"+RString(url)+"'");
	}
	return(ref->Doc);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GDoc* doc)
{
	// If all documents are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(doc);
		return;
	}

	// The first document has the identifier 1
	if(Data->Docs.GetNb())
		doc->SetId(Data->Docs[Data->Docs.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first doc has an identifier of 1
	else
		doc->SetId(1);
}


//-----------------------------------------------------------------------------
void GSession::InsertDoc(GDoc* d)
{
	bool NewOne=false;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
	{
		AssignId(d);
		NewOne=true;
	}

	// Insert the document
	Data->Docs.InsertPtrAt(d,d->GetId());

	// Insert the doc in the DocsRefUrl container.
	Data->DocsRefUrl.InsertPtr(new GDocRefURL(d));

	if(NewOne)
	{
		// If new one and all documents are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveDoc(d);
		Event(d,eObjCreated);
	}
}


//-----------------------------------------------------------------------------
void GSession::ClearDocs(void)
{
	Data->DocsRefUrl.Clear();
	Data->Docs.Clear();
}


//------------------------------------------------------------------------------
void GSession::AnalyseDocs(bool ram,GSlot* rec)
{
	// Opens and appends the Log File for all errors occurring in the filter or analyze phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Storage->GetFactory()->GetName()+ " on : " +RString::Number(RDate::GetToday().GetDay())+"/"+ RString::Number(RDate::GetToday().GetMonth())+"/"+RString::Number(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyze documents");
	}

	// Get the method
	GDocAnalyse* Analyse=GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	// Analyze the documents - Go through the existing documents
	R::RCursor<GDoc> Docs=GetDocs();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		try
		{
			AnalyseDoc(Docs(),ram,Analyse,rec);
		}
		// If a log file specified -> write to it and it is OK
		// If no log file specified -> Propagate error
		HANDLEALLEXCEPTIONS(rec,Docs()->GetURL()()+"("+RString::Number(Docs()->GetId())+"): ")
	}

	// Launch post doc methods
	DoPostDocs(rec);
}


//------------------------------------------------------------------------------
void GSession::AnalyseDoc(GDoc* doc,bool ram,GDocAnalyse* method,GSlot* rec)
{
	// Verify that the document analysis method is selected
	if(!method)
	{
		method=GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod();
		if(!method)
			throw GException("No document analysis method chosen.");
	}

	if(!doc->MustCompute()) return;
	if(rec)
	{
		rec->Interact();
		rec->NextDoc(doc);
	}
	if(Intern::ExternBreak) return;

	RIO::RSmartTempFile docxml;
	bool Native;
	bool Save=(SaveResults&&(doc->GetId()!=cNoRef));
	RURI uri=Data->FilterManager->WhatAnalyze(doc,docxml,Native);
	if(uri().IsEmpty())
		return;

     // Analyze document -> Is something goes wrong -> It failed
	bool DelRef(doc->IsDefined());
	method->Analyze(doc,uri,Native);

	// Save the binary files
	if(Save)
	{
		SaveInfos(method->Infos,otDoc,doc->GetId());
		SaveStruct(method->Struct,doc);
	}

	// Set the information to the document
	doc->Update(method->Lang,method->Infos,method->Struct,ram||(!Save),DelRef);

	// Save the description and the structure
	if(Save)
	{
		Storage->SaveDoc(doc);
		if(ram)
			doc->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
bool GSession::GetDocXML(GDoc* doc,R::RXMLStruct* xml,bool& native)
{
	RIO::RSmartTempFile docxml;
	xml->Clear();
	RURI uri=Data->FilterManager->WhatAnalyze(doc,docxml,native);
	if(uri().IsEmpty())
		return(false);
	RXMLFile XML(uri,xml);
	XML.SetInvalidXMLCodes(true);
	XML.Open(RIO::Read);
	return(true);
}

//------------------------------------------------------------------------------
void GSession::DoPostDocs(GSlot* rec)
{
	// Run all post-doc methods that are enabled
	R::RCursor<GPostDoc> PostDocs=GALILEIApp->GetManager<GPostDocManager>("PostDoc")->GetPlugIns();
	for(PostDocs.Start();!PostDocs.End();PostDocs.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Running Post-document method: "+PostDocs()->GetFactory()->GetName());
		PostDocs()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::QueryMetaEngine(RContainer<RString,true,false> &keyWords)
{
	GMetaEngine* metaEngine;
	// Verify that a meta engine is selected
	metaEngine=GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod();
	if(!metaEngine)
		throw GException("No meta engine method chosen.");
	metaEngine->Query(keyWords,true); //true ->Use all keywords passed to the meta engine
	metaEngine->Process();
}



//------------------------------------------------------------------------------
//
// GSession::Users
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::LoadUsers(void)
{
	if(Data->UsersLoaded)
		return;
	Storage->LoadUsers();
	Data->UsersLoaded=true;
}


//------------------------------------------------------------------------------
R::RCursor<GUser> GSession::GetUsers(void) const
{
	return(R::RCursor<GUser>(Data->Users));
}


//------------------------------------------------------------------------------
size_t GSession::GetNbUsers(void) const
{
	return(Data->Users.GetNb());
}


//------------------------------------------------------------------------------
GUser* GSession::GetUser(size_t id,bool load,bool null) const
{
	GUser* u;

	if(id>Data->Users.GetMaxPos())
		return(0);
	u=Data->Users[id];
	if(u)
		return(u);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+RString::Number(id));
	}
	u=Storage->LoadUser(id);
	if(!u)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertUser(u);
	return(u);
}


//------------------------------------------------------------------------------
GUser* GSession::GetUser(const RString name,bool load,bool null) const
{
	GUser* u;

	u=Data->Users.GetPtr(name,false);
	if(u)
		return(u);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+name);
	}
	u=Storage->LoadUser(name);
	if(!u)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+name);
	}
	const_cast<GSession*>(this)->InsertUser(u);
	return(u);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GUser* user)
{
	// If all users are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(user);
		return;
	}

	// The first user has the identifier 1
	if(Data->Users.GetNb())
		user->SetId(Data->Users[Data->Users.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first user has an identifier of 1
	else
		user->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertUser(GUser* user)
{
	bool NewOne=false;

	if(user->GetId()==cNoRef)
	{
		AssignId(user);
		NewOne=true;
	}
	Data->Users.InsertPtrAt(user,user->GetId(),true);
	if(NewOne)
	{
		// If new one and all users are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveUser(user);
		if(Data->Slot)
			Data->Slot->Alert("User "+user->GetName()+" created ("+RString::Number(user->GetId())+")");
		Event(user,eObjCreated);
	}
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GSession::GetProfiles(void) const
{
	return(R::RCursor<GProfile>(Data->Profiles));
}


//------------------------------------------------------------------------------
size_t GSession::GetNbProfiles(void) const
{
	return(Data->Profiles.GetNb());
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxProfileId(void) const
{
	if(!Data->Profiles.GetNb())
		return(0);
	return(Data->Profiles[Data->Profiles.GetMaxPos()]->GetId());
}


//------------------------------------------------------------------------------
GProfile* GSession::GetProfile(size_t id,bool load,bool null) const
{
	GProfile* p;

	if(id>Data->Profiles.GetMaxPos())
		return(0);
	p=Data->Profiles[id];
	if(p)
		return(p);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown profile "+RString::Number(id)+" in memory");
	}
	p=Storage->LoadProfile(id);
	if(!p)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown profile "+RString::Number(id)+" in storage");
	}
	const_cast<GSession*>(this)->InsertProfile(p);
	return(p);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GProfile* p)
{
	// If all profiles are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(p);
		return;
	}

	// The first profile has the identifier 1
	if(Data->Profiles.GetNb())
		p->SetId(Data->Profiles[Data->Profiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first profile has an identifier of 1
	else
		p->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertProfile(GProfile* p)
{
	bool NewOne=false;

	if(p->GetId()==cNoRef)
	{
		AssignId(p);
		NewOne=true;
	}
	Data->Profiles.InsertPtrAt(p,p->GetId());
	if(NewOne)
	{
		// If new one and all profiles are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveProfile(p);
		if(Data->Slot)
			Data->Slot->Alert("Profile "+p->GetName()+" for user "+p->GetUser()->GetName()+" created ("+RString::Number(p->GetId())+")");
		Event(p,eObjCreated);
	}
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	// Run all pre-profile methods that are enabled
	R::RCursor<GPreProfile> PreProfile=GALILEIApp->GetManager<GPreProfileManager>("PreProfile")->GetPlugIns();
	for(PreProfile.Start();!PreProfile.End();PreProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Pre-profile method : "+PreProfile()->GetFactory()->GetName());
		PreProfile()->Run();
	}

	R::RCursor<GProfile> Prof=GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(Intern::ExternBreak) return;
		try
		{
			CalcProfile(Prof(),rec);
		}
		catch(GException& e)
		{
			cerr<<e.GetMsg()<<endl;
		}
	}
	DoPostProfiles(rec);
}


//------------------------------------------------------------------------------
void GSession::CalcProfile(GProfile* profile,GSlot* rec)
{
	GProfileCalc* Profiling=GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod();
	GLinkCalc* LinkCalc=GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false);

	if(!Profiling)
		throw GException("No computing method chosen.");

	if(rec)
		rec->NextProfile(profile);

	// If necessary, compute Links on the profile description
	if(LinkCalc)
		LinkCalc->Compute(profile);

	if(rec)
		rec->Interact();

	if(Intern::ExternBreak) return;
	if(!profile->MustCompute()) return;
	Profiling->Compute(profile);
	if(Intern::ExternBreak) return;
	if(SaveResults&&(profile->GetId()!=cNoRef))
		Storage->SaveProfile(profile);
}


//------------------------------------------------------------------------------
void GSession::DoPostProfiles(GSlot* rec)
{
	// Run all post-profiles methods that are enabled
	R::RCursor<GPostProfile> PostProfile=GALILEIApp->GetManager<GPostProfileManager>("PostProfile")->GetPlugIns();
	for(PostProfile.Start();!PostProfile.End();PostProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Post-profile method : "+PostProfile()->GetFactory()->GetName());
		PostProfile()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::UpdateProfiles(size_t docid)
{
	// If there are some profile -> propagate in memory
	GDoc* doc=GetDoc(docid);
	if(doc)
	{
		RVectorInt<size_t,true>* fdbks=doc->GetFdbks();
		if(fdbks)
		{
			for(fdbks->Start();!fdbks->End();fdbks->Next())
			{
				GProfile* prof=GetProfile((*fdbks)());
				if(!prof)
					continue;
				prof->HasUpdate(docid);
			}
		}
	}

	// Use database
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->UpdateProfiles(docid);
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(size_t profid,size_t docid,tDocAssessment assess,R::RDate date,R::RDate computed,bool newone)
{
	GProfile* prof=GetProfile(profid,false);
	if(prof)
		prof->InsertFdbk(docid,assess,date,computed);
	GDoc* doc=GetDoc(docid,false);
	if(doc)
		doc->InsertFdbk(profid);
	if(newone&&((!Storage->IsAllInMemory())||(SaveResults)))
		Storage->AddFdbk(profid,docid,assess,date,computed);
}


//------------------------------------------------------------------------------
void GSession::ClearFdbks(void)
{
	RCursor<GDoc> Docs(GetDocs());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		Docs()->ClearFdbks();
	}
	RCursor<GProfile> Profiles=GetProfiles();
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		Profiles()->ClearFdbks();
	}
}



//------------------------------------------------------------------------------
//
// GSession::Groups
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::LoadCommunities(void)
{
	if(Data->CommunitiesLoaded)
		return;
	Storage->LoadCommunities();
	Data->CommunitiesLoaded=true;
}


//------------------------------------------------------------------------------
RCursor<GCommunity> GSession::GetCommunities(void)
{
	return(RCursor<GCommunity>(Data->Communities));
}


//------------------------------------------------------------------------------
size_t GSession::GetNbCommunities(void) const
{
	return(Data->Communities.GetNb());
}



//------------------------------------------------------------------------------
GCommunity* GSession::GetCommunity(size_t id,bool load,bool null) const
{
	if(id==cNoRef)
	{
		if(null)
			return(0);
		throw GException("Unknown community "+RString::Number(id));
	}
	GCommunity* grp=Data->Communities.GetPtr(id);
	if(grp)
		return(grp);

	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown community "+RString::Number(id));
	}
	grp=Storage->LoadCommunity(id);
	if(!grp)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown community "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertCommunity(grp);
	return(grp);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GCommunity* com)
{
	// If all groups are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(com);
		return;
	}

	// The first group has the identifier 1
	if(Data->Communities.GetNb())
		com->SetId(Data->Communities[Data->Communities.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first community has an identifier of 1
	else
		com->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertCommunity(GCommunity* com)
{
	Data->Communities.InsertPtr(com);
}


//------------------------------------------------------------------------------
void GSession::DeleteCommunity(GCommunity* com)
{
	Data->Communities.DeletePtr(com);
}


//------------------------------------------------------------------------------
void GSession::ClearCommunities(void)
{
	Data->Communities.Clear();
}


//------------------------------------------------------------------------------
void GSession::GroupProfiles(GSlot* rec)
{
	GGroupProfiles* Grouping=GALILEIApp->GetManager<GGroupProfilesManager>("GroupProfiles")->GetCurrentMethod();

	// Verify that there is a method to cluster the profile
	if(!Grouping)
		throw GException("No profiles grouping method chosen.");

    // Group the profiles
	Grouping->Grouping(rec,SaveResults);
	DoPostCommunity(rec);
}


//------------------------------------------------------------------------------
void GSession::DoPostCommunity(GSlot* rec)
{
	// Run all post-community methods that are enabled
	R::RCursor<GPostCommunity> PostCommunity=GALILEIApp->GetManager<GPostCommunityManager>("PostCommunity")->GetPlugIns();
	for(PostCommunity.Start();!PostCommunity.End();PostCommunity.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Post-community method : "+PostCommunity()->GetFactory()->GetName());
		PostCommunity()->Run();
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cCalc>
	void GSession::CopyIdealGroups(tObjType objtype,tObjType grouptype,cCalc* calc)
{
	cGroup* grp;

	// Clear current clustering
	ClearGroups(grouptype);

	// Go through each subjects
	R::RCursor<GSubject> Grps(GetSubjects()->GetNodes());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Clear the groups associated to the subject
		Grps()->ClearIdealGroup(grouptype);

		// If the subject has no objects or is not selected -> next one.
		if((!Grps()->IsUsed())&&(!Grps()->GetNbObjs(objtype)))
			continue;

		// Create a new group in groups and associated with the current groups
		grp=static_cast<cGroup*>(NewGroup(grouptype,Grps()->GetName()));
		AssignId(grp);
		InsertGroup(grp,grouptype);
		Grps()->AssignIdealGroup(grp);

		// Go through each object
		RCursor<cObj> Objs=Grps()->GetObjs(static_cast<cObj*>(0));
		for(Objs.Start();!Objs.End();Objs.Next())
			grp->InsertObj(Objs());

		// Compute Description
		if(calc)
			calc->Compute(grp);
	}
}


//------------------------------------------------------------------------------
void GSession::BuildGroupsFromIdeal(tObjType type)
{
	switch(type)
	{
		case otCommunity:
		{
			CopyIdealGroups<GCommunity,GProfile,GCommunityCalc>(otProfile,otCommunity,GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc")->GetCurrentMethod());
			if(SaveResults)
			{
				Storage->SaveCommunities();
				RCursor<GCommunity> Groups(Data->Communities);
				for(Groups.Start();!Groups.End();Groups.Next())
					Groups()->SetState(osSaved);
			}
			break;
		}
		case otTopic:
		{
			CopyIdealGroups<GTopic,GDoc,GTopicCalc>(otDoc,otTopic,GALILEIApp->GetManager<GTopicCalcManager>("TopicCalc")->GetCurrentMethod());
			if(SaveResults)
			{
				Storage->SaveTopics();
				RCursor<GTopic> Topics(Data->Topics);
				for(Topics.Start();!Topics.End();Topics.Next())
					Topics()->SetState(osSaved);
			}
			break;
		}
		default:
			throw GException("GSession::BuildGroupsFromIdeal: '"+GetObjType(type)+"' is not a valid type");
			break;
	}
}


//------------------------------------------------------------------------------
void GSession::UpdateCommunity(GProfile* prof)
{
	if(prof&&(prof->GetGroupId()!=cNoRef))
	{
		GCommunity* grp=GetCommunity(prof->GetGroupId(),false);
		if(grp)
			grp->HasUpdate(prof);
	}

	// Use database
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->UpdateCommunities(prof->GetId());
}


//------------------------------------------------------------------------------
void GSession::UpdateCommunity(size_t profid)
{
	GProfile* prof=GetProfile(profid,false,false);
	UpdateCommunity(prof);
}



//------------------------------------------------------------------------------
//
// GSession::Topics
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::LoadTopics(void)
{
	if(Data->TopicsLoaded)
		return;
	Storage->LoadTopics();
	Data->TopicsLoaded=true;
}


//------------------------------------------------------------------------------
RCursor<GTopic> GSession::GetTopics(void)
{
	return(RCursor<GTopic>(Data->Topics));
}


//-----------------------------------------------------------------------------
size_t GSession::FillTopics(GTopic** topics)
{
	return(Data->Topics.GetTab(topics,1,Data->Topics.GetMaxPos()));
}


//------------------------------------------------------------------------------
size_t GSession::GetNbTopics(void) const
{
	return(Data->Topics.GetNb());
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxTopicId(void) const
{
	if(!Data->Topics.GetNb())
		return(0);
	return(Data->Topics[Data->Topics.GetMaxPos()]->GetId());
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxPosTopic(void) const
{
	return(Data->Topics.GetMaxPos());
}


//------------------------------------------------------------------------------
GTopic* GSession::GetTopic(size_t id,bool load,bool null) const
{
	if(id==cNoRef)
	{
		if(null)
			return(0);
		throw GException("Unknown topic "+RString::Number(id));
	}
	GTopic* top=Data->Topics[id];
	if(top)
		return(top);

	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown topic "+RString::Number(id));
	}
	top=Storage->LoadTopic(id);
	if(!top)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown topic "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertTopic(top);
	return(top);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GTopic* top)
{
	// If all groups are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(top);
		return;
	}

	// The first group has the identifier 1
	if(Data->Topics.GetNb())
		top->SetId(Data->Topics[Data->Topics.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first topic has an identifier of 1
	else
		top->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertTopic(GTopic* top)
{
	Data->Topics.InsertPtrAt(top,top->GetId());
}


//------------------------------------------------------------------------------
void GSession::DeleteTopic(GTopic* top)
{
	Data->Topics.DeletePtrAt(top->GetId(),false);
}


//------------------------------------------------------------------------------
void GSession::ClearTopics(void)
{
	Data->Topics.Clear();
}


//------------------------------------------------------------------------------
void GSession::GroupDocs(GSlot* rec)
{
	GGroupDocs* Grouping=GALILEIApp->GetManager<GGroupDocsManager>("GroupDocs")->GetCurrentMethod();

	// Verify that there is a method to cluster the profile
	if(!Grouping)
		throw GException("No documents grouping method chosen.");

    // Group the profiles
	Grouping->Grouping(rec,SaveResults,Data->ClusterSelectedDocs);
	DoPostTopic(rec);
}


//------------------------------------------------------------------------------
void GSession::DoPostTopic(GSlot* rec)
{
	// Run all post-topic methods that are enabled
	R::RCursor<GPostTopic> Post=GALILEIApp->GetManager<GPostTopicManager>("PostTopic")->GetPlugIns();
	for(Post.Start();!Post.End();Post.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Post-topic method : "+Post()->GetFactory()->GetName());
		Post()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::UpdateTopic(GDoc* doc)
{
	if(doc&&(doc->GetGroupId()!=cNoRef))
	{
		GTopic* top=GetTopic(doc->GetGroupId(),false);
		if(top)
			top->HasUpdate(doc);
	}

	// Use database
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->UpdateTopics(doc->GetId());
}


//------------------------------------------------------------------------------
void GSession::UpdateTopic(size_t docid)
{
	GDoc* doc=GetDoc(docid,false,false);
	UpdateTopic(doc);
}



//------------------------------------------------------------------------------
//
// GSession::Static
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession* GSession::Get(void)
{
	if(!Intern::Session)
		throw GException("No session");
	return(Intern::Session);
}


//------------------------------------------------------------------------------
bool GSession::Break(void)
{
	return(Intern::ExternBreak);
}


//------------------------------------------------------------------------------
void GSession::SetBreak(void)
{
	Intern::ExternBreak=true;
}


//------------------------------------------------------------------------------
void GSession::ResetBreak(void)
{
	Intern::ExternBreak=false;
}



//------------------------------------------------------------------------------
//
// GSession::Destruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::~GSession(void)
{
	// Save the configurations
	Save();

	delete Data;
}
