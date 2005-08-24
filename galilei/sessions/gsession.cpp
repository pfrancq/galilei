/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2005 by the Université libre de Bruxelles.

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
#include <gdict.h>
#include <gwordlist.h>
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
#include <gprofilessims.h>
#include <gprofilesdocssims.h>
#include <ggroupsdocssims.h>
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
#include <gpluginmanagers.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
//
// class GFreeId
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFreeId
{
public:
	unsigned int Id;          // Identificator.

	// Constructor and Compare methods.
	GFreeId(unsigned int id) : Id(id) {}
	int Compare(const GFreeId& f) const {return(Id-f.Id);}
	int Compare(unsigned int id) const {return(Id-id);}

	// Destructor
	~GFreeId(void) {}
};


//------------------------------------------------------------------------------
//
// class GDocsLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsLang : public R::RContainer<GDoc,false,true>
{
public:
	GLang* Lang;

	GDocsLang(GLang* lang) throw(std::bad_alloc)
		: RContainer<GDoc,false,true>(10000,5000), Lang(lang) {}
	int Compare(const GDocsLang& docLang) const;
	int Compare(const GLang* lang) const;
	void Clear(void);
	virtual ~GDocsLang(void) {}
};


//-----------------------------------------------------------------------------
int GDocsLang::Compare(const GDocsLang& docLang) const
{
	if(!Lang)
	{
		if(!docLang.Lang) return(0);
		return(-1);
	}
	if(!docLang.Lang) return(1);
	return(Lang->Compare(*docLang.Lang));
}


//-----------------------------------------------------------------------------
int GDocsLang::Compare(const GLang* lang) const
{
	if(!Lang)
	{
		if(!lang) return(0);
		return(-1);
	}
	if(!lang) return(1);
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GDocsLang::Clear(void)
{
	if(Lang&&Lang->GetDict())
		Lang->GetDict()->Clear(otDoc);
	R::RContainer<GDoc,false,true>::Clear();
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
	int Compare(const char* url) const  {return(Doc->GetURL().Compare(url));}
	~GDocRefURL(void) {}
};



//------------------------------------------------------------------------------
//
// class GGroupsLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGroupsLang : public R::RContainer<GGroup,false,true>
{
public:

	GLang* Lang;              // Language

	// Constructor and Compare methods.
	GGroupsLang(GLang* lang)
		: RContainer<GGroup,false,true>(20,10), Lang(lang) {}
	int Compare(const GGroupsLang& groups) const {return(Lang->Compare(*groups.Lang));}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}

	// Get the group of a given subprofile
	GGroup* GetGroup(const GSubProfile* sub);

	// Destructor
	~GGroupsLang(void) {}
};


//------------------------------------------------------------------------------
GGroup* GGroupsLang::GetGroup(const GSubProfile* sub)
{
	R::RCursor<GGroup> Groups;

	Groups.Set(*this);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		if(Groups()->IsIn(sub))
			return(Groups());
	}
	return(0);
}



//------------------------------------------------------------------------------
//
//  GSubProfiles
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set of subprofiles of a given language.
class GSubProfiles : public R::RContainer<GSubProfile,true,true>
{
public:

	GLang* Lang;                 // Language

	// Constructor and Compare methods.
	GSubProfiles(GLang* lang,unsigned int s)
		: RContainer<GSubProfile,true,true>(s,s/2), Lang(lang) {}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
	int Compare(const GSubProfiles& s) const {return(Lang->Compare(s.Lang));}
	int Compare(const GSubProfiles* s) const {return(Lang->Compare(s->Lang));}

	// Get a cursor over the subprofiles of the system.
	RCursor<GSubProfile> GetSubProfiles(void) const {return(RCursor<GSubProfile>(*this));}

	void Clear(void);

	// Destructor.
	virtual ~GSubProfiles(void) {}
};


//------------------------------------------------------------------------------
void GSubProfiles::Clear(void)
{
/*	if(Lang&&Lang->GetDict())
		Lang->GetDict()->Clear(otSubProfile);*/
	R::RContainer<GSubProfile,true,true>::Clear();
}



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
	GProfilesSimsManager* ProfilesSims;                               // Subprofiles/Subprofiles similarities
	GProfilesDocsSimsManager* ProfilesDocsSims;                       // Subprofiles/Documents similarities
	GGroupsDocsSimsManager* GroupsDocsSims;                           // Groups/Documents similarities
	GProfilesGroupsSimsManager* ProfilesGroupsSims;                   // Subprofiles/Groups similarities
	int CurrentRandom;                                                // Current seek for this session.
	R::RRandom* Random;                                               // Random number generator
	GStorage* Storage;                                                // Storage manager
	static GSession* Session;                                         // Static pointer to the session
	static bool ExternBreak;                                          // Should the session stop as soon as possible?
	bool SaveDocs;                                                    // Must the documents be saved after computed?
	bool SaveSubProfiles;                                             // Must the subprofiles be saved after computed?
	bool SaveGroups;                                                  // Must the groups be saved after computed?
	bool ComputeModifiedDocs;                                         // Compute only the modified documents.
	bool ComputeModifiedSubProfiles;                                  // Compute only the modified subprofiles.
	bool UseExistingGroups;                                           // Use the existing groups.
	GSlot* Slot;                                                      // Slot for the session

	R::RContainer<GDoc,true,true> Docs;
	R::RContainer<GDocsLang,true,true> DocsLang; // Documents ordered by language and identificator.
	R::RContainer<GDocRefURL,true,true> DocsRefUrl; // Documents ordered by URL.
	R::RContainer<GUser,true,true> Users;
	R::RContainer<GProfile,true,true> Profiles; // Profiles handled by the system.
	R::RContainer<GSubProfiles,true,true> SubProfiles; // SubProfiles handled by the system.
	R::RContainer<GGroup,true,true> Groups;
	R::RContainer<GGroupsLang,true,true> GroupsLang;
	R::RContainer<GFreeId,true,true> FreeIds;
	unsigned int MaxDocs;
	unsigned int MaxSubProfiles;
	unsigned int MaxGroups;

	Intern(GStorage* str,unsigned int mdocs,unsigned int maxsub,unsigned int maxgroups,unsigned int d,unsigned int u,unsigned int p,unsigned int g,unsigned int nblangs)
		: Subjects(0), GroupsHistoryMng(0), ProfilesSims(0), ProfilesDocsSims(0),
	  GroupsDocsSims(0), Random(0), Storage(str),  SaveDocs(true),
	  SaveSubProfiles(true), SaveGroups(true), ComputeModifiedDocs(true),
	  ComputeModifiedSubProfiles(true), UseExistingGroups(true), Slot(0),
		Docs(d+(d/2),d/2), DocsLang(nblangs), DocsRefUrl(d+(d/2),d/2),
		Users(u,u/2), Profiles(p,p/2), SubProfiles(nblangs),
		Groups(g+(g/2),g/2),  GroupsLang(nblangs), FreeIds(50,25),
		MaxDocs(mdocs), MaxSubProfiles(maxsub), MaxGroups(maxgroups)
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
// GSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::GSession(GStorage* str,unsigned int maxdocs,unsigned int maxsubprofiles,unsigned int maxgroups)
	: Data(0)
{
	if(!str)
		throw GException("No Storage");

	// Init Part
	Data=new Intern(str,maxdocs,maxsubprofiles,maxgroups,str->GetNbSaved(otDoc),str->GetNbSaved(otUser),str->GetNbSaved(otProfile),str->GetNbSaved(otGroup),str->GetNbSaved(otLang));
	if(!Intern::Session)
		Intern::Session=this;
}


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
bool GSession::MustSave(tObjType objtype) const
{
	switch(objtype)
	{
		case otDoc:
			return(Data->SaveDocs);
			break;
		case otSubProfile:
			return(Data->SaveSubProfiles);
			break;
		case otGroup:
			return(Data->SaveGroups);
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			return(false);
	}
}


//------------------------------------------------------------------------------
bool GSession::ComputeModified(tObjType objtype) const
{
	switch(objtype)
	{
		case otDoc:
			return(Data->ComputeModifiedDocs);
			break;
		case otSubProfile:
			return(Data->ComputeModifiedSubProfiles);
			break;
		case otGroup:
			return(Data->UseExistingGroups);
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			return(false);
	}
}


//------------------------------------------------------------------------------
void GSession::SetSave(tObjType objtype,bool save)
{
	switch(objtype)
	{
		case otDoc:
			Data->SaveDocs=save;
			break;
		case otSubProfile:
			Data->SaveSubProfiles=save;
			break;
		case otGroup:
			Data->SaveGroups=save;
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			break;
	}
}


//------------------------------------------------------------------------------
void GSession::SetComputeModified(tObjType objtype,bool modified)
{
	switch(objtype)
	{
		case otDoc:
			Data->ComputeModifiedDocs=modified;
			break;
		case otSubProfile:
			Data->ComputeModifiedSubProfiles=modified;
			break;
		case otGroup:
			Data->UseExistingGroups=modified;
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			break;
	}
}


//------------------------------------------------------------------------------
void GSession::Connect(void)
{
	GPluginManagers::PluginManagers.Connect(this);
}

//-----------------------------------------------------------------------------
R::RCursor<GDoc> GSession::GetDocs(void) const
{
	return(R::RCursor<GDoc>(Data->Docs));
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GSession::GetDocs(GLang* lang) const
{
	GDocsLang* ptr;
	R::RCursor<GDoc> cur;

	ptr=Data->DocsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(*ptr);
	return(cur);
}


//-----------------------------------------------------------------------------
unsigned int GSession::GetNbDocs(void) const
{
	return(Data->Docs.GetNb());
}


//-----------------------------------------------------------------------------
unsigned int GSession::GetMaxPosDoc(void) const
{
	return(Data->Docs.GetMaxPos());
}


//-----------------------------------------------------------------------------
unsigned int GSession::FillDocs(GDoc** docs)
{
	return(Data->Docs.GetTab(docs));
}


//-------------------------------------------------------------------------------
unsigned int GSession::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = Data->DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->GetNb());
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNewId(void) const
{
	if(!Data->Docs.GetNb()) return(1);
	return(Data->Docs[Data->Docs.GetNb()-1]->GetId()+1);
}


//-----------------------------------------------------------------------------
void GSession::InsertDoc(GDoc* d)
{
	GDocsLang* docsLang;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
		d->SetId(GetNewId());

	// Insert the document
	Data->Docs.InsertPtrAt(d,d->GetId());
	docsLang = Data->DocsLang.GetInsertPtr<GLang*>(d->GetLang());
	docsLang->InsertPtr(d);

	//insert the doc in the DocsRefUrl container.
	Data->DocsRefUrl.InsertPtr(new GDocRefURL(d));
}


//-----------------------------------------------------------------------------
void GSession::MoveDoc(GDoc* d)
{
	GDocsLang* docsLang;

	// Remove doc from container of docs with no language
	docsLang=Data->DocsLang.GetPtr<GLang*>(0);
	docsLang->DeletePtr(d);

	// Move doc to container of appropriated language.
	docsLang=Data->DocsLang.GetInsertPtr(d->GetLang());
	docsLang->InsertPtr(d);
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(unsigned int id,bool load) const
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
		throw GException("Unknown document");
	d=Data->Storage->LoadDoc(const_cast<GSession*>(this),id);
	if(!d)
		throw GException("Unknown document");
	const_cast<GSession*>(this)->InsertDoc(d);
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(const char* url)
{
	GDocRefURL* ref;

	ref=Data->DocsRefUrl.GetPtr(url);
	if(!ref) return(0);
	return(ref->Doc);
}


//-----------------------------------------------------------------------------
void GSession::ClearDocs(void)
{
	RCursor<GDocsLang> Cur(Data->DocsLang);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Data->DocsRefUrl.Clear();
	Data->Docs.Clear();
}


//------------------------------------------------------------------------------
GDocXML* GSession::CreateDocXML(GDoc* doc)
{
	return((dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(doc));
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
			const_cast<GSession*>(this)->Data->Storage->LoadSubjects(const_cast<GSession*>(this));
	}
	return(Data->Subjects);
}


//------------------------------------------------------------------------------
void GSession::SetSims(GProfilesSimsManager* sims)
{
	Data->ProfilesSims=sims;
}


//------------------------------------------------------------------------------
GProfilesSimsManager* GSession::GetProfilesSims(void) const
{
	return(Data->ProfilesSims);
}


//------------------------------------------------------------------------------
void GSession::SetSims(GProfilesDocsSimsManager* sims)
{
	Data->ProfilesDocsSims=sims;
}


//------------------------------------------------------------------------------
GProfilesDocsSimsManager* GSession::GetProfilesDocsSims(void) const
{
	return(Data->ProfilesDocsSims);
}


//------------------------------------------------------------------------------
void GSession::SetSims(GGroupsDocsSimsManager* sims)
{
	Data->GroupsDocsSims=sims;
}


//------------------------------------------------------------------------------
GGroupsDocsSimsManager* GSession::GetGroupsDocsSims(void) const
{
	return(Data->GroupsDocsSims);
}


//------------------------------------------------------------------------------
void GSession::SetSims(GProfilesGroupsSimsManager* sims)
{
	Data->ProfilesGroupsSims=sims;
}


//------------------------------------------------------------------------------
GProfilesGroupsSimsManager* GSession::GetProfilesGroupsSims(void) const
{
	return(Data->ProfilesGroupsSims);
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
void GSession::AssignId(GData* data,const GDict* dict)
{
	Data->Storage->AssignId(data,dict);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GGroup* grp)
{
	// Is there a free identificator
	// -> Take the first one.
	if(Data->FreeIds.GetNb())
	{
		unsigned int id=Data->FreeIds[0]->Id;
		delete Data->FreeIds[0];
		grp->SetId(id);
		return;
	}
	if(!Data->Groups.GetNb())
	{
		grp->SetId(1);
		return;
	}
	grp->SetId(Data->Groups[Data->Groups.GetNb()-1]->GetId()+1);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GDoc* doc)
{
	doc->SetId(GetNewId());
}


//------------------------------------------------------------------------------
void GSession::AssignId(GSubProfile* sub)
{
	sub->SetId(GetNewId(otSubProfile));
}


//------------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocs();
	RContainer<GDoc,false,true> tmpDocs(5,2);
	GDocAnalyse* Analyse;
	RString err;
	bool Cont;               // Continue the analysuis

	// Verify that the textanalyse method is selected
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManagers::PluginManagers.GetManager("DocAnalyse")))->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		err= "Documents Filtering and Analysis on Data Set : "+Data->Storage->GetName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear());
		rec->WriteStr(err.Latin1());
	}

	// Analyse the documents
	do
	{
		// Go through the existing documents
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			if(Data->ComputeModifiedDocs&&(!Docs()->MustCompute())) continue;
			if(rec)
			{
				rec->Interact();
				rec->NextDoc(Docs());
			}
			if(Intern::ExternBreak) return;
			undefLang=false;
			try
			{
				if((!Data->ComputeModifiedDocs)||(Docs()->MustCompute()))
				{
					if (!Docs()->GetLang()) undefLang=true;
					xml=(dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(Docs());
					if(xml)
					{
						Docs()->InitFailed();
						Analyse->Analyze(xml,Docs(),&tmpDocs);
						delete xml;
						xml=0;
						if((undefLang)&&(Docs()->GetLang()))
						{
							MoveDoc(Docs());
						}
					}
					else
						Docs()->IncFailed();
				}
				if(Data->SaveDocs)
				{
					Data->Storage->SaveDoc(Docs());
					Docs()->SetState(osSaved);
				}
			}
			catch(GException& e)
			{
				if(xml)
					delete xml;

				// Write error message to the log file handled by the GSlot.
				if(rec)
					rec->WriteStr(e.GetMsg());
				else
					throw GException(e.GetMsg());
			}
		}

		// Add the new documents.
		// Continue the analysis if documents were added.
		RCursor<GDoc> Cur(tmpDocs);
		Cont=tmpDocs.GetNb();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			InsertDoc(Cur());
			//#warning add the document with the Cur()->GetId()
		}
		tmpDocs.Clear();
	}
	while(Cont);

	// Run all post-doc methods that are enabled
	ComputePostDoc(rec);
}


//------------------------------------------------------------------------------
void GSession::AnalyseNewDocs(GSlot* rec)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocs();
	RContainer<GDoc,false,true> tmpDocs(5,2);
	GDocAnalyse* Analyse;
	RString err;
	bool Cont;               // Continue the analysuis

	// Verify that the textanalyse method is selected
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManagers::PluginManagers.GetManager("DocAnalyse")))->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		err= "Documents Filtering and Analysis on Data Set : "+Data->Storage->GetName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear());
		rec->WriteStr(err.Latin1());
	}

	// Analyse the documents
	do
	{
		// Go through the existing documents
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			if(Data->ComputeModifiedDocs&&(!Docs()->MustCompute())) continue;
			if(rec)
			{
				rec->Interact();
				rec->NextDoc(Docs());
			}
			if(Intern::ExternBreak) return;
			undefLang=false;
			try
			{
				if((!Data->ComputeModifiedDocs)||(Docs()->MustCompute()))
				{
					if (!Docs()->GetLang()) undefLang=true;
					xml=(dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(Docs());
					if(xml)
					{
						Docs()->InitFailed();
						Analyse->Analyze(xml,Docs(),&tmpDocs);
						delete xml;
						xml=0;
						if((undefLang)&&(Docs()->GetLang()))
						{
							MoveDoc(Docs());
						}
					}
					else
						Docs()->IncFailed();
				}
				if(Data->SaveDocs)
				{
					Data->Storage->SaveDoc(Docs());
					Docs()->SetState(osSaved);
				}
			}
			catch(GException& e)
			{
				if(xml)
					delete xml;

				// Write error message to the log file handled by the GSlot.
				if(rec)
					rec->WriteStr(e.GetMsg());
				else
					throw GException(e.GetMsg());
			}
		}

		// Add the new documents.
		// Continue the analysis if documents were added.
		RCursor<GDoc> Cur(tmpDocs);
		Cont=tmpDocs.GetNb();
		for(Cur.Start();!Cur.End();Cur.Next())
			InsertDoc(Cur());
		tmpDocs.Clear();
	}
	while(Cont);

	// Run all post-doc methods that are enabled
	//ComputePostDoc(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePostDoc(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostDoc> PostDocs=(dynamic_cast<GPostDocManager*>(GPluginManagers::PluginManagers.GetManager("PostDoc")))->GetFactories();

	if(rec)
		rec->Interact();
	if(Intern::ExternBreak) return;

	for(PostDocs.Start();!PostDocs.End();PostDocs.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(PostDocs()->GetPlugin())
		{
			if(rec)
			{
				sprintf(tmp, "PostDoc : Running %s",PostDocs()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PostDocs()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::QueryMetaEngine(RContainer<RString,true,false> &keyWords)
{
	GMetaEngine* metaEngine;
	// Verify that a meta engine is selected
	metaEngine=(dynamic_cast<GMetaEngineManager*>(GPluginManagers::PluginManagers.GetManager("MetaEngine")))->GetCurrentMethod();
	if(!metaEngine)
		throw GException("No meta engine method chosen.");
	metaEngine->Query(keyWords,true); //true ->Use all keywords passed to the meta engine
	metaEngine->Process();
}


//------------------------------------------------------------------------------
R::RCursor<GUser> GSession::GetUsers(void) const
{
	return(R::RCursor<GUser>(Data->Users));
}


//------------------------------------------------------------------------------
void GSession::InsertUser(GUser* usr)
{
	Data->Users.InsertPtrAt(usr,usr->GetId(),true);
}


//------------------------------------------------------------------------------
GUser* GSession::GetUser(unsigned int id,bool load) const
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
		throw GException("Unknown user");
	u=Data->Storage->LoadUser(const_cast<GSession*>(this),id);
	if(!u)
		throw GException("Unknown user");
	const_cast<GSession*>(this)->InsertUser(u);
	return(u);
}


//------------------------------------------------------------------------------
size_t GSession::GetNbUsers(void) const
{
	return(Data->Users.GetNb());
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNewId(tObjType obj)
{
	unsigned int id,i;
	RCursor<GSubProfiles> Cur;

	switch(obj)
	{
		case otProfile:
			if(Data->Profiles.GetNb())
				id=Data->Profiles[Data->Profiles.GetMaxPos()]->GetId()+1;  // Not [GetNb()-1] because first profile has an identificator of 1
			else
				id=1;
			break;

		case otSubProfile:
			Cur.Set(Data->SubProfiles);
			for(Cur.Start(),id=0;!Cur.End();Cur.Next())
			{
				if(!Cur()->GetNb()) continue;
				i=(*Cur())[Cur()->GetMaxPos()]->GetId()+1;
				if(id<i)
					id=i;
			}
			break;

		case otUser:
			if(Data->Users.GetNb())
				id=Data->Users[Data->Users.GetMaxPos()]->GetId()+1; // Not [GetNb()-1] because first user has an identificator of 1
			else
				id=1;
			break;

		default:
			throw GException("No a valid type");
			break;
	}
	return(id);
}


//------------------------------------------------------------------------------
void GSession::InsertProfile(GProfile* p)
{
	Data->Profiles.InsertPtrAt(p,p->GetId());
}


//------------------------------------------------------------------------------
GProfile* GSession::GetProfile(unsigned int id,bool load) const
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
		throw GException("Unknown profile");
	p=Data->Storage->LoadProfile(const_cast<GSession*>(this),id);
	if(!p)
		throw GException("Unknown profile");
	const_cast<GSession*>(this)->InsertProfile(p);
	return(p);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GSession::GetProfiles(void) const
{
	return(R::RCursor<GProfile>(Data->Profiles));
}


//------------------------------------------------------------------------------
unsigned int GSession::GetProfilesNb(void) const
{
	return(Data->Profiles.GetNb());
}


//------------------------------------------------------------------------------
void GSession::InsertSubProfile(GSubProfile* s)
{
	GLang* l;
	GSubProfiles* list;

	l=s->GetLang();
	list=Data->SubProfiles.GetPtr<const GLang*>(l);
	if(!list)
		Data->SubProfiles.InsertPtr(list=new GSubProfiles(l,Data->Profiles.GetMaxNb()));
//	list->InsertPtrAt(s,s->GetId());
	list->InsertPtr(s);
}


//------------------------------------------------------------------------------
GSubProfile* GSession::GetSubProfile(unsigned int id,const GLang* lang,bool load) const
{
	GSubProfile* s;

	if(lang)
	{
		GSubProfiles* subs=Data->SubProfiles.GetPtr(lang);
		if(subs)
		{
			s=subs->GetPtr(id);
			if(s)
				return(s);
		}
	}
	else
	{
		RCursor<GSubProfiles> Cur(Data->SubProfiles);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			s=Cur()->GetPtr(id);
			if(s)
				return(s);
		}
	}
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
		throw GException("Unknown subprofile");
	s=Data->Storage->LoadSubProfile(const_cast<GSession*>(this),id);
	if(!s)
		throw GException("Unknown subprofile");
	const_cast<GSession*>(this)->InsertSubProfile(s);
	return(s);
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GSession::GetSubProfiles(const GLang* lang) const
{
	GSubProfiles* ptr=Data->SubProfiles.GetPtr(lang);
	if(ptr)
		return(ptr->GetSubProfiles());
	return(RCursor<GSubProfile>());
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfiles();
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");

	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(rec)
			rec->NextProfile(Prof());
		Prof()->DispatchFdbks();
		//Calc Links on the profile description
		if(LinkCalc)
			LinkCalc->Compute(Prof());
		Subs=Prof()->GetSubProfiles();
		for(Subs.Start();!Subs.End();Subs.Next())
		{
			if(rec)
				rec->Interact();

			if(Intern::ExternBreak) return;
			if(Data->ComputeModifiedSubProfiles&&(!Subs()->MustCompute())) continue;
			try
			{
				if((!Data->ComputeModifiedSubProfiles)||(Subs()->MustCompute()))
				{
					Profiling->Compute(Subs());

					if(Data->SaveSubProfiles)
					{
						Data->Storage->SaveSubProfile(Subs());
						Subs()->SetState(osSaved);
					}
				}
			}
			catch(GException& e)
			{
			}
		}
	}

	// Save the best computed Links (As Hub and Authority)
	if((Data->SaveSubProfiles)&&(LinkCalc))
		Data->Storage->SaveLinks(this);

	//runs the post profiling methds;
	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::CalcProfile(GSlot* rec,GProfile* profile)
{
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
//	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");
	if(rec)
		rec->NextProfile(profile);
	profile->DispatchFdbks();
	//Calc Links on the profile description
	if(LinkCalc)
		LinkCalc->Compute(profile);
	RCursor<GSubProfile> Subs(profile->GetSubProfiles());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(rec)
			rec->Interact();

		if(Intern::ExternBreak) return;
		if(Data->ComputeModifiedSubProfiles&&(!Subs()->MustCompute())) continue;
		try
		{
			if((!Data->ComputeModifiedSubProfiles)||(Subs()->MustCompute()))
			{
				Profiling->Compute(Subs());

				if(Data->SaveSubProfiles)
				{
					Data->Storage->SaveSubProfile(Subs());
					Subs()->SetState(osSaved);
				}
			}
		}
		catch(GException& e)
		{
		}
	}

	// Save the best computed Links (As Hub and Authority)
	if((Data->SaveSubProfiles)&&(LinkCalc))
		Data->Storage->SaveLinks(this);

	//runs the post profiling methds;
//	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePreProfile(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPreProfile> PreProfile=(dynamic_cast<GPreProfileManager*>(GPluginManagers::PluginManagers.GetManager("PreProfile")))->GetFactories();

	if(rec)
		rec->Interact();
	if(Intern::ExternBreak) return;

	for(PreProfile.Start();!PreProfile.End();PreProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(PreProfile()->GetPlugin())
		{
			if(rec)
			{
				sprintf(tmp, "PostProfile : Running %s",PreProfile()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PreProfile()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::ComputePostProfile(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostProfile> PostProfile=(dynamic_cast<GPostProfileManager*>(GPluginManagers::PluginManagers.GetManager("PostProfile")))->GetFactories();

	if(rec)
		rec->Interact();
	if(Intern::ExternBreak) return;

	for(PostProfile.Start();!PostProfile.End();PostProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(PostProfile()->GetPlugin())
		{
			if(rec)
			{
				sprintf(tmp, "PostProfile : Running %s",PostProfile()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PostProfile()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::GroupingProfiles(GSlot* rec)
{
	GGrouping* Grouping=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

    // Group the subprofiles
	Grouping->Grouping(rec,Data->UseExistingGroups,Data->SaveGroups);

	// Run all post-group methods that are enabled
	ComputePostGroup(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePostGroup(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostGroup> PostGroups=(dynamic_cast<GPostGroupManager*>(GPluginManagers::PluginManagers.GetManager("PostGroup")))->GetFactories();

	if(rec)
		rec->Interact();
	if(Intern::ExternBreak) return;

	for(PostGroups.Start();!PostGroups.End();PostGroups.Next())
	{
		if(rec)
			rec->Interact();
		if(Intern::ExternBreak) return;
		if(PostGroups()->GetPlugin())
		{
			if(rec)
			{
				sprintf(tmp, "PostGroup : Running %s",PostGroups()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PostGroups()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date,R::RDate update)
{
	GProfile* prof=GetProfile(p,false);
	if(prof)
		prof->InsertFdbk(d,assess,date,update);
	GDoc* doc=GetDoc(d,false);
	if(doc)
		doc->InsertFdbk(p);
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
R::RCursor<GGroup> GSession::GetGroups(void) const
{
	return(R::RCursor<GGroup>(Data->Groups));
}


//------------------------------------------------------------------------------
R::RCursor<GGroup> GSession::GetGroups(GLang* lang)
{
	GGroupsLang* ptr;
	R::RCursor<GGroup> cur;

	ptr=Data->GroupsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(*ptr);
	return(cur);
}


//------------------------------------------------------------------------------
void GSession::InsertGroup(GGroup* grp)
{
	GGroupsLang* groupsLang;

	Data->Groups.InsertPtr(grp);
	groupsLang = Data->GroupsLang.GetInsertPtr<GLang*>(grp->GetLang());
	groupsLang->InsertPtr(grp);
}


//------------------------------------------------------------------------------
void GSession::DeleteGroup(GGroup* grp)
{
	GGroupsLang* groupsLang=Data->GroupsLang.GetInsertPtr<GLang*>(grp->GetLang());
	if(groupsLang)
		groupsLang->DeletePtr(grp);
	Data->FreeIds.InsertPtr(new GFreeId(grp->GetId()));
	Data->Groups.DeletePtr(grp);
}


//------------------------------------------------------------------------------
GGroup* GSession::GetGroup(const GSubProfile* sub,bool load) const
{
	GGroupsLang* groupsLang=Data->GroupsLang.GetInsertPtr<GLang*>(sub->GetLang());
	if(!groupsLang)
		throw GException("No language defined");
	GGroup* grp=groupsLang->GetGroup(sub);
	if(grp)
		return(grp);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
		throw GException("Unknown group "+itou(sub->GetGroupId()));
	grp=Data->Storage->LoadGroup(const_cast<GSession*>(this),sub->GetGroupId());
	if(!grp)
		throw GException("Unknown group "+itou(sub->GetGroupId()));
	const_cast<GSession*>(this)->InsertGroup(grp);
	return(grp);
}


//------------------------------------------------------------------------------
GGroup*GSession::GetGroup(unsigned int id,bool load) const
{
	GGroup* grp=Data->Groups.GetPtr<unsigned int>(id);
	if(grp)
		return(grp);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
		throw GException("Unknown group "+itou(id));
	grp=Data->Storage->LoadGroup(const_cast<GSession*>(this),id);
	if(!grp)
		throw GException("Unknown group "+itou(id));
	const_cast<GSession*>(this)->InsertGroup(grp);
	return(grp);
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNbGroups(void) const
{
	return(Data->Groups.GetNb());
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNbGroups(GLang* lang) const
{
	GGroupsLang* grps;

	grps=Data->GroupsLang.GetPtr<const GLang*>(lang);
	if(!grps)
		return(0);
	return(grps->GetNb());
}


//------------------------------------------------------------------------------
void GSession::Clear(GLang* lang)
{
	size_t i,nb;
	GGroupsLang* grps=Data->GroupsLang.GetPtr<const GLang*>(lang);
	GGroup* grp;

	// Go through the groups and delete all invalid groups.
	if(!grps) return;

	if(lang&&lang->GetDict())
		lang->GetDict()->Clear(otGroup);
	for(nb=grps->GetNb(),i=0;i<nb;i++)
	{
		grp=(*grps)[i];
		Data->Groups.DeletePtr(grp);
	}
	grps->Clear();
}


//------------------------------------------------------------------------------
void GSession::CopyIdealGroups(void)
{
	RCursor<GSubProfile> Sub;
	GGroup* grp;
	GGroupCalc* CalcDesc;

	// Get current grouping description method
	CalcDesc=(dynamic_cast<GGroupCalcManager*>(GPluginManagers::PluginManagers.GetManager("GroupCalc")))->GetCurrentMethod();

	// Clear current clustering
	RCursor<GGroupsLang> Groups(Data->GroupsLang);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		Clear(Groups()->Lang);
	}

	// Get the active languages
	RCursor<GLang> Langs=(dynamic_cast<GLangManager*>(GPluginManagers::PluginManagers.GetManager("Lang")))->GetPlugIns();

	// Go through each subjects
	R::RCursor<GSubject> Grps(*Data->Subjects);
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go trough each lang
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			// If the subject has no subprofiles -> next one.
			if(!Grps()->GetNbSubProfiles(Langs()))
				continue;

			// Create a new group in groups
			grp=new GGroup(cNoRef,Langs(),true,RDate(""),RDate(""));
			AssignId(grp);
			InsertGroup(grp);

			// Go through each subprofile
			Sub=Grps()->GetSubProfiles(Langs());
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				grp->InsertSubProfile(Sub());
			}

			// Compute Description
			if(CalcDesc)
				CalcDesc->Compute(grp);
		}
	}

	if(Data->SaveGroups)
	{
		Data->Storage->SaveGroups(this);
		RCursor<GGroup> Groups(GetGroups());
		for(Groups.Start();!Groups.End();Groups.Next())
			Groups()->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
void GSession::RunPrg(GSlot* rec,const char* filename)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Exec();
}


//------------------------------------------------------------------------------
void GSession::DocsFilter(int nbdocs,int nboccurs)
{
	//The number of word in current lang.
	R::RCursor<GDoc> DocCursorTemp =GetDocs();
	DocCursorTemp.Start();

	int NbKwd;
	GDict* CurDic=DocCursorTemp()->GetLang()->GetDict();
	NbKwd=CurDic->GetDataMaxId();

	int* j;
	int* k;
	bool* test;
	j=new int [NbKwd];
	k=new int [NbKwd];
	test=new bool[NbKwd];
	for(int i=0;i<NbKwd;i++)
	{
		j[i]=0;
		k[i]=0;
	}
	R::RCursor<GDoc> DocCursor=GetDocs();
	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDoc* Doc=DocCursor();
		RCursor<GWeightInfo> Cur(*Doc);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			GWeightInfo* WW=Cur();
			if(WW->GetWeight()>j[WW->GetId()])
			{
				j[WW->GetId()]=int(WW->GetWeight());
			}
			k[WW->GetId()]+=1;
		}
	}
	int compt=0;
	for(int i=0;i<NbKwd;i++)
	{
		if((j[i]>=nboccurs)&&k[i]>=nbdocs)
			{
				test[i]=true;
				compt++;
			}
		else
			{
				test[i]=false;
			}
	}

	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDoc* Doc=DocCursor();
		for(int i=0;i<NbKwd;i++)
		{
			if(!test[i])
			{
				GWeightInfo* WW=Doc->GetPtr<const unsigned int>(i);
				if(WW) Doc->DeletePtr(*WW);
			}
		}
	}
	delete[] j;
	delete[] k;
	delete[] test;
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
void GSession::LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen)
{
	unsigned int i;

	// fill the container
	for (i=mingen; i<maxgen+1; i++)
		Data->GroupsHistoryMng->InsertGroupsHistory(Data->Storage->LoadAnHistoricGroups(this, i));
}


//------------------------------------------------------------------------------
void GSession::LoadHistoricGroupsByDate(RString mindate,RString maxdate)
{
	Data->Storage->LoadHistoricGroupsByDate(this,mindate,maxdate);
}


//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	if(Data->Subjects)
		Data->Subjects->ReInit();

	// Clear groups
	RCursor<GGroupsLang> Groups(Data->GroupsLang);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		Clear(Groups()->Lang);
	}
	Data->Groups.Clear();

	// Clear Fdbks
	ClearFdbks();

	// Clear Users
	RCursor<GSubProfiles> Cur(Data->SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Data->Profiles.Clear();
	Data->Users.Clear();
}


//------------------------------------------------------------------------------
void GSession::ExportMatrix(GSlot* rec, const char* type, const char* filename, GLang* lang, bool label)
{
	Data->Storage->ExportMatrix(this, rec, type, filename, lang, label);
}


//------------------------------------------------------------------------------
void GSession::UpdateProfiles(unsigned int docid)
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
				prof->HasUpdate(docid);
			}
		}
	}

	// Use database
	if((!Data->Storage->IsAllInMemory())||(Data->SaveSubProfiles))
		Data->Storage->UpdateProfiles(docid);
}



//------------------------------------------------------------------------------
void GSession::UpdateGroups(unsigned int subid)
{
	// If there is a group -> propagate in memory
	GSubProfile* sub=GetSubProfile(subid,0,false);
	if(sub)
	{
		GGroup* grp=GetGroup(sub->GetGroupId(),false);
		if(grp)
			grp->HasUpdate(sub);
	}

	// Use database
	if((!Data->Storage->IsAllInMemory())||(Data->SaveGroups))
		Data->Storage->UpdateGroups(subid);
}


//------------------------------------------------------------------------------
GSession::~GSession(void)
{
	delete Data;
}
