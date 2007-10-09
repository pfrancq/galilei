/*
 
	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2007 by the Université libre de Bruxelles.

	Authors:
		GALILEI Team (pfrancq@ulb.ac.be)

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
#include <rtextfile.h>
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
#include <gsessionprg.h>
#include <gstatscalc.h>
#include <gdoc.h>
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
#include <gsubprofile.h>
#include <gsubprofile.h>
#include <gprofilecalc.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
#include <ggroup.h>
#include <ggrouping.h>
#include <ggroupcalc.h>
#include <gsubjects.h>
#include <gfilter.h>
#include <gpostdoc.h>
#include <gweightinfo.h>
#include <ggroupshistory.h>
#include <gpostgroup.h>
#include <ggalileiapp.h>
#include <glangdata.h>
#include <gdebugobject.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// define
#define HANDLEALLEXCEPTIONS(rec,msg) 	                                       \
catch(GException& e)                                                           \
{                                                                              \
	if(rec)                                                                    \
		rec->WriteStr(e.GetMsg());                                             \
	else                                                                       \
		throw GException(e.GetMsg());                                          \
}                                                                              \
catch(RException& e)                                                           \
{                                                                              \
	if(rec)                                                                    \
		rec->WriteStr(e.GetMsg());                                             \
	else                                                                       \
		throw GException(e.GetMsg());                                          \
}                                                                              \
catch(exception& e)                                                            \
{                                                                              \
	if(rec)                                                                    \
		rec->WriteStr(msg+" "+e.what());                                       \
	else                                                                       \
		throw GException(msg+" "+e.what());                                    \
}                                                                              \
catch(...)                                                                     \
{                                                                              \
	if(rec)                                                                    \
		rec->WriteStr(msg+" Undefined error");                                 \
	else                                                                       \
		throw GException(msg+" Undefined error");                              \
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
	GGroupsHistoryManager* GroupsHistoryMng;                          // Historic group manager
	int CurrentRandom;                                                // Current seek for this session.
	R::RRandom* Random;                                               // Random number generator
	GStorage* Storage;                                                // Storage manager
	static GSession* Session;                                         // Static pointer to the session
	static bool ExternBreak;                                          // Should the session stop as soon as possible?
	bool SaveResults;                                                 // Must the results be saved after computed?
	GSlot* Slot;                                                      // Slot for the session
	R::RDebug* Debug;                                                 // Debug output for the session
	R::RContainer<GDoc,true,true> Docs;                               // Documents handled by the system.
	R::RContainer<GDocRefURL,true,true> DocsRefUrl;                   // Documents ordered by URL.
	R::RContainer<GUser,true,true> Users;                             // Users handled by the system.
	R::RContainer<GProfile,true,true> Profiles;                       // Profiles handled by the system.
	R::RContainer<GLangData,true,true> Langs;                         // Documents, Subprofiles and Groups divided by language.
	R::RContainer<GConceptType,true,true> ConceptTypes;               // Types of Concepts
	R::RContainer<GRelationType,true,true> RelationTypes;             // Types of Relations
	R::RContainer<GDebugObject,false,true> DebugObjs;                 // Objects given debugging information.
	unsigned int MaxDocs;                                             // Maximum number of documents to handle in memory.
	unsigned int MaxSubProfiles;                                      // Maximum number of subprofiles to handle in memory.
	unsigned int MaxGroups;                                           // Maximum number of groups to handle in memory.
	GFilterManager* FilterManager;                                    // Pointer to the filter manager
	
	Intern(GStorage* str,unsigned int mdocs,unsigned int maxsub,unsigned int maxgroups,unsigned int d,unsigned int u,unsigned int p,unsigned int nblangs)
		: Subjects(0), GroupsHistoryMng(0), Random(0), Storage(str),  SaveResults(true),
		  Slot(0), Docs(d+(d/2),d/2), DocsRefUrl(d+(d/2),d/2),
		  Users(u,u/2), Profiles(p,p/2), Langs(nblangs+1),
		  ConceptTypes(10,5), RelationTypes(10,5), DebugObjs(100,100),
		  MaxDocs(mdocs), MaxSubProfiles(maxsub), MaxGroups(maxgroups), FilterManager(0)
	{
		CurrentRandom=0;
		Random = new RRandomGood(CurrentRandom);
	}

	~Intern(void)
	{
		delete Random;
		delete Subjects;
		delete GroupsHistoryMng;
		Session=0;
		ExternBreak=false;
	}
};



//------------------------------------------------------------------------------
// Global variables
GSession* GSession::Intern::Session=0;
bool GSession::Intern::ExternBreak=false;



//------------------------------------------------------------------------------
//
// GSession::Constructor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::GSession(GSlot* slot,R::RDebug* debug,unsigned int maxdocs,unsigned int maxsubprofiles,unsigned int maxgroups)
	: Data(0)
{
	GFactoryStorage* fac=GALILEIApp->GetManager<GStorageManager>("Storage")->GetCurrentFactory();
	if(!fac)
		throw GException("No Storage");

	// Delete the storage if necessary
	if(fac->GetPlugin())
		fac->Delete();

	// Create the storage
	fac->Create();

	// Init Part
	Data=new Intern(fac->GetPlugin(),maxdocs,maxsubprofiles,maxgroups,
			fac->GetPlugin()->GetNbSaved(otDoc),
			fac->GetPlugin()->GetNbSaved(otUser),
			fac->GetPlugin()->GetNbSaved(otProfile),
			fac->GetPlugin()->GetNbSaved(otLang));

	Data->Slot=slot;
	Data->Debug=debug;
	Data->FilterManager=GALILEIApp->GetManager<GFilterManager>("Filter");

	if(!Intern::Session)
		Intern::Session=this;
}



//------------------------------------------------------------------------------
//
// GSession::General
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::ForceReCompute(tObjType type)
{
	switch(type)
	{
		case otDoc:
		{
			// Clear the information of the documents -> Also subprofiles and groups
			RCursor<GDoc> Docs(Data->Docs);
			for(Docs.Start();!Docs.End();Docs.Next())
				Docs()->ClearInfos();
		}
		case otSubProfile:
		{
			// Delete the subprofiles -> Also groups
			RCursor<GConceptType> Types(Data->ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->Clear(otSubProfile);
			RCursor<GLangData> Cur(Data->Langs);
			for(Cur.Start();!Cur.End();Cur.Next())
				Cur()->Clear(otSubProfile);	
			if(Data->SaveResults)
				Data->Storage->Clear(otSubProfile);
		}
		case otGroup:
		{
			// Delete the groups
			RCursor<GConceptType> Types(Data->ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->Clear(otGroup);			
			RCursor<GLangData> Cur(Data->Langs);
			for(Cur.Start();!Cur.End();Cur.Next())
				Cur()->Clear(otGroup);
			if(Data->SaveResults)
				Data->Storage->Clear(otGroup);
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
		
	// Clear Fdbks
	ClearFdbks();

	// Clear groups and subprofiles
	RCursor<GLangData> Cur(Data->Langs);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->Clear(otGroup);
		Cur()->Clear(otSubProfile);
	}
	RCursor<GConceptType> Types(Data->ConceptTypes);
	for(Types.Start();!Types.End();Types.Next())
	{
		Types()->Clear(otGroup);
		Types()->Clear(otSubProfile);
	}

	// Clear Profiles and users
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
					if(Data->Storage)
						Res+=Data->Storage->GetWorld();
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
bool GSession::MustSaveResults(void) const
{
	return(Data->SaveResults);
}


//------------------------------------------------------------------------------
void GSession::SetSaveResults(bool save)
{
	Data->SaveResults=save;
}


//------------------------------------------------------------------------------
GGroupsHistoryManager* GSession::GetGroupsHistoryManager(void) const
{
	if(!Data->GroupsHistoryMng)
		const_cast<GSession*>(this)->Data->GroupsHistoryMng=new GGroupsHistoryManager(const_cast<GSession*>(this),20);
	return(Data->GroupsHistoryMng);
}


//------------------------------------------------------------------------------
GStorage* GSession::GetStorage(void) const
{
	return(Data->Storage);
}


//------------------------------------------------------------------------------
GSubjects* GSession::GetSubjects(bool load) const
{
	if((!Data->Subjects)&&(load))
	{
		Data->Subjects=new GSubjects(const_cast<GSession*>(this));
		if(Data->Storage)
			const_cast<GSession*>(this)->Data->Storage->LoadSubjects();
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
	Data->DebugObjs.InsertPtr(debug);
}


//------------------------------------------------------------------------------
void GSession::RemoveDebugObject(const GDebugObject* debug)
{
	Data->DebugObjs.DeletePtr(debug);
}


//------------------------------------------------------------------------------
RString GSession::GetDebugInfo(const RString& name,const RString& info)
{
	GDebugObject* obj=Data->DebugObjs.GetPtr(name);
	if(!obj)
		throw GException("No debugging object called '"+info+"'");
	return(obj->GetDebugInfo(info));
}

	
//------------------------------------------------------------------------------
void GSession::PutDebugInfo(RTextFile& file,const RString& name,const RString& info)
{
	GDebugObject* obj=Data->DebugObjs.GetPtr(name);
	if(!obj)
		throw GException("No debugging object called '"+info+"'");
	obj->PutDebugInfo(file,info);	
}

	
//------------------------------------------------------------------------------
void GSession::RunPrg(GSlot* rec,const char* filename)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Load();
	Prg.Exec();
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
int GSession::GetCurrentRandomValue(unsigned int max)
{
	return(int(Data->Random->Value(max)));
}


//------------------------------------------------------------------------------
R::RRandom* GSession::GetRandom(void) const
{
	return(Data->Random);
}


//------------------------------------------------------------------------------
R::RCursor<GLangData> GSession::GetLanguageSpecifics(void)
{
	return(R::RCursor<GLangData>(Data->Langs));
}


//------------------------------------------------------------------------------
GLangData* GSession::GetLanguageSpecific(GLang* lang)
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(!ptr)
		Data->Langs.InsertPtr(ptr=new GLangData(lang));
	return(ptr);
}



//------------------------------------------------------------------------------
//
// GSession::Knowledge
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RCursor<GConceptType> GSession::GetConceptTypes(void) const
{
	return(RCursor<GConceptType>(Data->ConceptTypes));
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(unsigned int id,bool null) const
{
	GConceptType* type(0);
	try
	{
		type=Data->ConceptTypes[id];
		if(type)
			type->Load();
	}
	catch(...)
	{
	}	
	if((!type)&&(!null))
		throw GException("Unknow concept type "+RString::Number(id));	
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(const RString& name,bool null) const
{
	GConceptType* type(0);
	try
	{
		type=Data->ConceptTypes.GetPtr(name,false);
		if(type)
			type->Load();
	}
	catch(...)
	{
	}	
	if((!type)&&(!null))
		throw GException("Unknow concept type '"+name+"'");	
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetInsertConceptType(const RString& name,const RString& desc)
{
	GConceptType* type(0);
	try
	{
		type=Data->ConceptTypes.GetPtr(name,false);
		if(type)
			type->Load();
	}
	catch(...)
	{
	}	
	if(!type)
		Data->ConceptTypes.InsertPtr(type=new GConceptType(cNoRef,this,name,desc,0));
	return(type);
}


//-----------------------------------------------------------------------------
void GSession::InsertConceptType(unsigned int id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refsubprofiles,size_t refgroups)
{
	RString code(name.Mid(0,2));
	GLang* Lang;
	GLangData* ptr=Data->Langs.GetPtr(code);
	if(ptr)
		Lang=ptr->GetLang();
	else
		Lang=0;
	size_t s=26;
	size_t s2=5000;
	GConceptType* type=new GConceptType(id,this,name,desc,Lang,s,s2);
	type->SetReferences(refdocs,refsubprofiles,refgroups);
	Data->ConceptTypes.InsertPtrAt(type,id);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GConceptType* type)
{
	Data->Storage->AssignId(type);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GConcept* concept)
{
	Data->Storage->AssignId(concept);
}


//-----------------------------------------------------------------------------
RCursor<GRelationType> GSession::GetRelationTypes(void) const
{
	return(RCursor<GRelationType>(Data->RelationTypes));
}


//-----------------------------------------------------------------------------
GRelationType* GSession::GetRelationType(unsigned int id,bool null) const
{
	GRelationType* type=Data->RelationTypes.GetPtr(id);
	if(!type)
	{
		if(!null)
			throw GException("Unknow relation type "+RString::Number(id));
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
			throw GException("Unknow relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
void GSession::InsertRelationType(unsigned int id,const R::RString& name,const R::RString& desc)
{
	Data->RelationTypes.InsertPtr(new GRelationType(id,name,desc));
}


//-----------------------------------------------------------------------------
void GSession::InsertRelation(unsigned int id,const R::RString& name,unsigned int subjectid,unsigned int subjecttypeid,unsigned int type,unsigned int objectid,unsigned int objecttypeid,double weight)
{
	// Get the concept related to the subject
	GConceptType* ctype=GetConceptType(subjecttypeid,false);
	if(!ctype)
		throw GException("Object "+RString::Number(subjectid)+" of type "+RString::Number(subjecttypeid)+" does not exist");
	GConcept* subject=ctype->GetConcept(subjectid);
	if(!subject)
		throw GException("Object "+RString::Number(subjectid)+" of type "+RString::Number(subjecttypeid)+" does not exist");

	// Get the concept related to the object
	ctype=GetConceptType(objecttypeid,false);
	if(!ctype)
		throw GException("Object "+RString::Number(objectid)+" of type "+RString::Number(objecttypeid)+" does not exist");
	GConcept* object=ctype->GetConcept(objectid);
	if(!object)
		throw GException("Object "+RString::Number(objectid)+" of type "+RString::Number(objecttypeid)+" does not exist");

	GRelationType* relationttype=Data->RelationTypes.GetPtr(type);
	if(!relationttype)
		throw GException("Relation type "+RString::Number(type)+" does not exist");
	relationttype->InsertRelation(new GRelation(id,name,subject,type,object,weight));
}


//-----------------------------------------------------------------------------
GRelation* GSession::GetRelation(unsigned int id,unsigned int type,bool null)
{
	GRelationType* relationttype=Data->RelationTypes.GetPtr(type);
	if(!relationttype)
	{
		if(!null)
			throw GException("Relation "+RString(id)+" of type "+RString::Number(type)+" does not exist");
		return(0);
	}
	GRelation* rel=relationttype->GetRelation(id);
	if((!rel)&&(!null))
		throw GException("Relation "+RString(id)+" of type "+RString::Number(type)+" does not exist");
	return(rel);
}


//------------------------------------------------------------------------------
void GSession::GetRelations(R::RContainer<GRelation,false,false>& rel,GConcept* subject,unsigned int type,GConcept* object,bool sym)
{
	RCursor<GRelationType> Types(Data->RelationTypes);
	for(Types.Start();!Types.End();Types.Next())
	{
		if((type!=cNoRef)&&(type!=Types()->GetId()))
			continue;
		RCursor<GRelation> Rel(Types()->GetRelations());
		for(Rel.Start();!Rel.End();Rel.Next())
		{
			if((subject)&&(((*subject)!=(*Rel()->GetSubject()))||(sym&&((*subject)!=(*Rel()->GetObject())))))
				continue;
			if((object)&&(((*object)!=(*Rel()->GetObject()))||(sym&&((*object)!=(*Rel()->GetSubject())))))
				continue;
			rel.InsertPtr(Rel());
		}
	}
}



//------------------------------------------------------------------------------
//
// GSession::Documents
//
//------------------------------------------------------------------------------

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
R::RCursor<GDoc> GSession::GetDocs(GLang* lang) const
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(R::RCursor<GDoc>(ptr->GetDocs()));
	return(R::RCursor<GDoc>());
}


//-------------------------------------------------------------------------------
size_t GSession::GetNbDocs(GLang* lang) const
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(ptr->GetNbDocs());
	return(0);
}


//-----------------------------------------------------------------------------
size_t GSession::FillDocs(GDoc** docs)
{
	return(Data->Docs.GetTab(docs));
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(unsigned int id,bool load,bool null) const
{
	GDoc* d;

	if(id>Data->Docs.GetMaxPos())
		return(0);
	d=Data->Docs[id];
	if(d)
		return(d);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown document "+RString::Number(id));
	}
	d=Data->Storage->LoadDoc(id);
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
	if(!Data->Storage->IsAllInMemory())
	{
		Data->Storage->AssignId(doc);
		return;
	}

	// The first document has the identificator 1
	if(Data->Docs.GetNb())
		doc->SetId(Data->Docs[Data->Docs.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first doc has an identificator of 1
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
	GLangData* Lang = Data->Langs.GetPtr(d->GetLang());
	if(!Lang)
		Data->Langs.InsertPtr(Lang=new GLangData(d->GetLang())); 
	Lang->InsertDoc(d);

	//insert the doc in the DocsRefUrl container.
	Data->DocsRefUrl.InsertPtr(new GDocRefURL(d));

	if(NewOne)
	{
		// If new one and all documents are in memory -> store it in the database
		if(Data->Storage->IsAllInMemory())
			Data->Storage->SaveDoc(d);
		Event(d,eObjCreated);
	}
}


//-----------------------------------------------------------------------------
void GSession::MoveDoc(GDoc* d)
{
	// Remove doc from container of docs with no language
	GLangData* ptr=Data->Langs.GetPtr<GLang*>(0);
	if(ptr)
		ptr->DeleteDoc(d);

	// Move doc to container of appropriated language.
	ptr=Data->Langs.GetPtr<GLang*>(d->GetLang());
	if(!ptr)
		Data->Langs.InsertPtr(ptr=new GLangData(d->GetLang()));
	ptr->InsertDoc(d);
}


//-----------------------------------------------------------------------------
void GSession::ClearDocs(void)
{
	RCursor<GLangData> Cur(Data->Langs);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear(otDoc);
	Data->DocsRefUrl.Clear();
	Data->Docs.Clear();
}


//------------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec)
{
	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Data->Storage->GetFactory()->GetName()+ " on : " +RString::Number(RDate::GetToday().GetDay())+"/"+ RString::Number(RDate::GetToday().GetMonth())+"/"+RString::Number(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyse documents");
	}

	// Get the method
	GDocAnalyse* Analyse=GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");
	
	// Analyse the documents - Go through the existing documents
	R::RCursor<GDoc> Docs=GetDocs();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		try
		{
			AnalyseDoc(Docs(),Analyse,rec);
		}
		// If a log file specified -> write to it and it is OK
		// If no log file specified -> Propagate error		
		HANDLEALLEXCEPTIONS(rec,Docs()->GetURL()+"("+RString::Number(Docs()->GetId())+"):")
	}

	// Launch post doc methods
	DoPostDocs(rec);
}


//------------------------------------------------------------------------------
void GSession::AnalyseDoc(GDoc* doc,GDocAnalyse* method,GSlot* rec)
{
	bool undefLang;

	// Verify that the textanalyse method is selected
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
	undefLang=false;
	if(!doc->GetLang())
		undefLang=true;
	
	RIO::RSmartTempFile docxml;
	RURI uri=Data->FilterManager->WhatAnalyze(doc,docxml);
	if(!uri.IsEmpty())
	{
		// Analyse document -> Is something goes wrong -> It failed
		method->Analyze(doc,uri);
		if((undefLang)&&(doc->GetLang()))
			MoveDoc(doc);	
	}

	// Save if necessary
	if(Data->SaveResults&&(doc->GetId()!=cNoRef))
	{
		Data->Storage->SaveDoc(doc);
		doc->SetState(osSaved);
	}
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
GUser* GSession::GetUser(unsigned int id,bool load,bool null) const
{
	GUser* u;

	if(id>Data->Users.GetMaxPos())
		return(0);
	u=Data->Users[id];
	if(u)
		return(u);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+RString::Number(id));
	}
	u=Data->Storage->LoadUser(id);
	if(!u)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+RString(id));
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
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+name);
	}
	u=Data->Storage->LoadUser(name);
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
	if(!Data->Storage->IsAllInMemory())
	{
		Data->Storage->AssignId(user);
		return;
	}

	// The first user has the identificator 1
	if(Data->Users.GetNb())
		user->SetId(Data->Users[Data->Users.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first user has an identificator of 1
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
		if(Data->Storage->IsAllInMemory())
			Data->Storage->SaveUser(user);
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
size_t GSession::GetProfilesNb(void) const
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
GProfile* GSession::GetProfile(unsigned int id,bool load,bool null) const
{
	GProfile* p;

	if(id>Data->Profiles.GetMaxPos())
		return(0);
	p=Data->Profiles[id];
	if(p)
		return(p);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown profile "+RString::Number(id)+" in memory");
	}
	p=Data->Storage->LoadProfile(id);
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
	if(!Data->Storage->IsAllInMemory())
	{
		Data->Storage->AssignId(p);
		return;
	}

	// The first profile has the identificator 1
	if(Data->Profiles.GetNb())
		p->SetId(Data->Profiles[Data->Profiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first profile has an identificator of 1
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
		if(Data->Storage->IsAllInMemory())
			Data->Storage->SaveProfile(p);
		if(Data->Slot)
			Data->Slot->Alert("Profile "+p->GetName()+" for user "+p->GetUser()->GetName()+" created ("+RString::Number(p->GetId())+")");
		Event(p,eObjCreated);
	}
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GSession::GetSubProfiles(const GLang* lang) const
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(RCursor<GSubProfile>(ptr->GetSubProfiles()));
	return(RCursor<GSubProfile>());
}


//------------------------------------------------------------------------------
size_t GSession::GetSubProfilesNb(const GLang* lang) const
{
	if(lang)
	{
		GLangData* ptr=Data->Langs.GetPtr(lang);
		if(ptr)
			return(ptr->GetNbSubProfiles());
		return(0);
	}

	size_t nb=0;
	RCursor<GLangData> Sub(Data->Langs);
	for(Sub.Start();!Sub.End();Sub.Next())
		nb+=Sub()->GetNbSubProfiles();
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxSubProfileId(const GLang* lang) const
{
	if(!lang)
		return(0);
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(ptr->GetMaxSubProfileId());
	return(0);	
}


//------------------------------------------------------------------------------
GSubProfile* GSession::GetSubProfile(GLang* lang,unsigned int id,bool load,bool null) const
{
	GSubProfile* s=0;

	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		s=ptr->GetSubProfile(id);
	if(s)
		return(s);
		
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown subprofile "+RString::Number(id));
	}
	s=Data->Storage->LoadSubProfile(id);
	if(!s)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown subprofile "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertSubProfile(s);
	return(s);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GSubProfile* sub)
{
	// If all subprofiles are not in memory -> use the database
	if(!Data->Storage->IsAllInMemory())
	{
		Data->Storage->AssignId(sub);
		return;
	}

	GLangData* ptr=Data->Langs.GetPtr(sub->GetLang());
	if(!ptr)
		Data->Langs.InsertPtr(ptr=new GLangData(sub->GetLang()));
	ptr->AssignId(sub);
}


//------------------------------------------------------------------------------
void GSession::InsertSubProfile(GSubProfile* sub)
{
	GLangData* ptr=Data->Langs.GetPtr(sub->GetLang());
	if(!ptr)
		Data->Langs.InsertPtr(ptr=new GLangData(sub->GetLang()));
	ptr->InsertSubProfile(sub);
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfiles();

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

	// If necessay, calc Links on the profile description
	if(LinkCalc)
		LinkCalc->Compute(profile);

	// Dispacth the feedbacks through the subprofiles
	profile->DispatchFdbks();

	// Go trough the subprofiles
	RCursor<GSubProfile> Subs(profile->GetSubProfiles());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(rec)
			rec->Interact();

		if(Intern::ExternBreak) return;
		if(!Subs()->MustCompute()) continue;
		Profiling->Compute(Subs());
		if(Data->SaveResults&&(Subs()->GetId()!=cNoRef))
		{
			Data->Storage->SaveSubProfile(Subs());
			Subs()->SetState(osSaved);
		}
	}
	if(Data->SaveResults&&(profile->GetId()!=cNoRef))
		Data->Storage->SaveProfile(profile);
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
void GSession::UpdateProfiles(unsigned int docid,GLang* lang)
{
	// If there are some profile -> propagate in memory
	GDoc* doc=GetDoc(docid);
	if(doc)
	{
		RVectorInt<true>* fdbks=doc->GetFdbks();
		if(fdbks)
		{
			for(fdbks->Start();!fdbks->End();fdbks->Next())
			{
				GProfile* prof=GetProfile((*fdbks)());
				if(!prof)
					continue;
				prof->HasUpdate(docid,lang);
			}
		}
	}

	// Use database
	if((!Data->Storage->IsAllInMemory())||(Data->SaveResults))
		Data->Storage->UpdateProfiles(docid,lang);
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(unsigned int p,unsigned int d,GLang* lang,tDocAssessment assess,R::RDate date,R::RDate computed,bool newone)
{
	GProfile* prof=GetProfile(p,false);
	if(prof)
		prof->InsertFdbk(d,lang,assess,date,computed);
	GDoc* doc=GetDoc(d,false);
	if(doc)
		doc->InsertFdbk(p);
	if(newone&&((!Data->Storage->IsAllInMemory())||(Data->SaveResults)))
		Data->Storage->AddFdbk(p,d,lang,assess,date,computed);
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
RCursor<GGroup> GSession::GetGroups(GLang* lang)
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(RCursor<GGroup>(ptr->GetGroups()));
	return(RCursor<GGroup>());
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNbGroups(GLang* lang) const
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		return(ptr->GetNbGroups());
	return(0);

}



//------------------------------------------------------------------------------
GGroup* GSession::GetGroup(GLang* lang,unsigned int id,bool load,bool null) const
{
	GGroup* grp=0;
	
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		grp=ptr->GetGroup(id);
	if(grp)
		return(grp);
		
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown group "+RString::Number(id));
	}
	grp=Data->Storage->LoadGroup(lang,id);
	if(!grp)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown group "+RString::Number(id));
	}
	const_cast<GSession*>(this)->InsertGroup(grp);
	return(grp);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GGroup* grp)
{
	// If all groups are not in memory -> use the database
	if(!Data->Storage->IsAllInMemory())
	{
		Data->Storage->AssignId(grp);
		return;
	}

	GLangData* ptr=Data->Langs.GetPtr(grp->GetLang());
	if(ptr)
		ptr->AssignId(grp);
}


//------------------------------------------------------------------------------
void GSession::InsertGroup(GGroup* grp)
{
	GLangData* ptr=Data->Langs.GetPtr(grp->GetLang());
	if(ptr)
		ptr->InsertGroup(grp);
}


//------------------------------------------------------------------------------
void GSession::DeleteGroup(GGroup* grp)
{
	GLangData* ptr=Data->Langs.GetPtr(grp->GetLang());
	if(ptr)
		ptr->DeleteGroup(grp);

}


//------------------------------------------------------------------------------
void GSession::ClearGroups(GLang* lang)
{
	GLangData* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		ptr->Clear(otGroup);
}


//------------------------------------------------------------------------------
void GSession::GroupingProfiles(GSlot* rec)
{
	GGrouping* Grouping=GALILEIApp->GetManager<GGroupingManager>("Grouping")->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

    // Group the subprofiles
	Grouping->Grouping(rec,Data->SaveResults);
	DoPostGroups(rec);
}


//------------------------------------------------------------------------------
void GSession::DoPostGroups(GSlot* rec)
{
	// Run all post-group methods that are enabled
	R::RCursor<GPostGroup> PostGroups=GALILEIApp->GetManager<GPostGroupManager>("PostGroup")->GetPlugIns();
	for(PostGroups.Start();!PostGroups.End();PostGroups.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(rec)
			rec->WriteStr("Post-group method : "+PostGroups()->GetFactory()->GetName());
		PostGroups()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::CopyIdealGroups(void)
{
	RCursor<GSubProfile> Sub;
	GGroup* grp;
	GGroupCalc* CalcDesc;

	// Get current grouping description method
	CalcDesc=GALILEIApp->GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod();

	// Clear current clustering
	RCursor<GLangData> Groups(Data->Langs);
	for(Groups.Start();!Groups.End();Groups.Next())
		Groups()->Clear(otGroup);
		
	// Get the active languages
	RCursor<GLang> Langs=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();

	// Go through each subjects
	R::RCursor<GSubject> Grps(GetSubjects()->GetNodes());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Clear the groups associated to the subject
		Grps()->ClearGroups();

		// Go trough each lang
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			// If the subject has no subprofiles -> next one.
			if(!Grps()->GetNbSubProfiles(Langs()))
				continue;

			// Create a new group in groups and associated with the current groups
			grp=new GGroup(cNoRef,Langs(),true,RDate(""),RDate(""));
			AssignId(grp);
			InsertGroup(grp);
			Grps()->InsertGroup(grp);

			// Go through each subprofile
			Sub=Grps()->GetSubProfiles(Langs());
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				grp->InsertSubProfile(Sub());
			}

			// Compute Description
			if(CalcDesc)
				CalcDesc->Compute(grp);
			
			if(Data->SaveResults)
			{
				Data->Storage->SaveGroups(Langs());
				RCursor<GGroup> Groups(GetGroups(Langs()));
				for(Groups.Start();!Groups.End();Groups.Next())
					Groups()->SetState(osSaved);
			}		
		}
	}
}


//------------------------------------------------------------------------------
void GSession::LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen)
{
	unsigned int i;

	// fill the container
	for (i=mingen; i<maxgen+1; i++)
		Data->GroupsHistoryMng->InsertGroupsHistory(Data->Storage->LoadAnHistoricGroups(i));
}


//------------------------------------------------------------------------------
void GSession::LoadHistoricGroupsByDate(RString mindate,RString maxdate)
{
	Data->Storage->LoadHistoricGroupsByDate(mindate,maxdate);
}


//------------------------------------------------------------------------------
void GSession::UpdateGroups(unsigned int subid)
{
	// If there is a group -> propagate in memory
	GSubProfile* sub=GetSubProfile(0,subid,false);
	if(sub)
	{
		GGroup* grp=GetGroup(sub->GetLang(),sub->GetGroupId(),false);
		if(grp)
			grp->HasUpdate(sub);
	}

	// Use database
	if((!Data->Storage->IsAllInMemory())||(Data->SaveResults))
		Data->Storage->UpdateGroups(subid);
}



//------------------------------------------------------------------------------
//
// GSession::Static
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession* GSession::Get(void)
{
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
// GSession::Destructors
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::~GSession(void)
{
	// Delete the storage
	Data->Storage->GetFactory()->Delete();

	delete Data;
}
