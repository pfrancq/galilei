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
		: Subjects(0), GroupsHistoryMng(0)/*, ProfilesSims(0), ProfilesDocsSims(0),
	  GroupsDocsSims(0)*/, Random(0), Storage(str),  SaveDocs(true),
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
// GSession::Constructor
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

	str->Connect(this);
}


//------------------------------------------------------------------------------
void GSession::Connect(void)
{
	GPluginManagers::PluginManagers.Connect(this);
}



//------------------------------------------------------------------------------
//
// GSession::General
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	if(Data->Subjects)
		Data->Subjects->ReInit();

	// Clear groups
	RCursor<GGroupsLang> Groups(Data->GroupsLang);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		ClearGroups(Groups()->Lang);
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
void GSession::RunPrg(GSlot* rec,const char* filename)
{
	GSessionPrg Prg(filename,this,rec);
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


//-------------------------------------------------------------------------------
unsigned int GSession::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = Data->DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->GetNb());
}


//-----------------------------------------------------------------------------
unsigned int GSession::FillDocs(GDoc** docs)
{
	return(Data->Docs.GetTab(docs));
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
GDoc* GSession::GetDoc(const char* url,bool load) const
{
	GDocRefURL* ref;

	ref=Data->DocsRefUrl.GetPtr(url);
	if(!ref) return(0);
	return(ref->Doc);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GData* data,const GDict* dict)
{
	Data->Storage->AssignId(data,dict);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GDoc* doc)
{
	if(!Data->Docs.GetNb())
	{
		doc->SetId(1);
		return;
	}
	doc->SetId(Data->Docs[Data->Docs.GetNb()-1]->GetId()+1);
}


//-----------------------------------------------------------------------------
void GSession::InsertDoc(GDoc* d)
{
	GDocsLang* docsLang;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
		AssignId(d);

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
void GSession::AnalyseDocs(GSlot* rec)
{
	GDocXML* xml;
	RContainer<GDoc,false,true> tmpDocs(5,2);
	bool Cont;               // Continue the analysuis

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Data->Storage->GetName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyse documents");
	}

	// Analyse the documents
	do
	{
		// Go through the existing documents
		R::RCursor<GDoc> Docs=GetDocs();
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			try
			{
				xml=0;       // DocXML to create
				AnalyseDoc(xml,Docs(),&tmpDocs,rec);
			}
			catch(GException& e)
			{
				// Write error message to the log file handled by the GSlot.
				if(rec)
					rec->WriteStr(e.GetMsg());
				else
					throw GException(e.GetMsg());
			}
			delete xml;
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
	R::RCursor<GPostDoc> PostDocs=GPluginManagers::GetManager<GPostDocManager>("PostDoc")->GetPlugIns();
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
void GSession::AnalyseDoc(GDocXML* &xml,GDoc* doc,RContainer<GDoc,false,true>* newdocs,GSlot* rec)
{
	bool undefLang;
//	GDocAnalyse* Analyse;
//	RString err;

	// Verify that the textanalyse method is selected
	GDocAnalyse* Analyse=GPluginManagers::GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	if(Data->ComputeModifiedDocs&&(!doc->MustCompute())) return;
	if(rec)
	{
		rec->Interact();
		rec->NextDoc(doc);
	}
	if(Intern::ExternBreak) return;
	undefLang=false;
	if((!Data->ComputeModifiedDocs)||(doc->MustCompute()))
	{
		if(!doc->GetLang()) undefLang=true;
		if(!xml)
			xml=GPluginManagers::GetManager<GFilterManager>("Filter")->CreateDocXML(doc);
		if(xml)
		{
			doc->InitFailed();
			Analyse->Analyze(xml,doc,newdocs);
			if((undefLang)&&(doc->GetLang()))
			{
				MoveDoc(doc);
			}
		}
		else
			doc->IncFailed();
	}
	if(Data->SaveDocs)
	{
		Data->Storage->SaveDoc(doc);
		doc->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
void GSession::QueryMetaEngine(RContainer<RString,true,false> &keyWords)
{
	GMetaEngine* metaEngine;
	// Verify that a meta engine is selected
	metaEngine=GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod();
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
void GSession::AssignId(GUser* user)
{
	if(Data->Users.GetNb())
		user->SetId(Data->Users[Data->Users.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first user has an identificator of 1
	else
		user->SetId(1);
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
void GSession::InsertUser(GUser* usr)
{
	Data->Users.InsertPtrAt(usr,usr->GetId(),true);
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
void GSession::AssignId(GProfile* p)
{
	if(Data->Profiles.GetNb())
		p->SetId(Data->Profiles[Data->Profiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first profile has an identificator of 1
	else
		p->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertProfile(GProfile* p)
{
	Data->Profiles.InsertPtrAt(p,p->GetId());
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
size_t GSession::GetSubProfilesNb(const GLang* lang) const
{
	if(lang)
	{
		GSubProfiles* ptr=Data->SubProfiles.GetPtr(lang);
		if(ptr)
			return(ptr->GetNb());
		else
			return(0);
	}
	size_t nb=0;
	RCursor<GSubProfiles> Sub(Data->SubProfiles);
	for(Sub.Start();!Sub.End();Sub.Next())
		nb+=Sub()->GetNb();
	return(nb);
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
void GSession::AssignId(GSubProfile* sub)
{
	unsigned int id,i;
	RCursor<GSubProfiles> Cur;

	Cur.Set(Data->SubProfiles);
	for(Cur.Start(),id=0;!Cur.End();Cur.Next())
	{
		if(!Cur()->GetNb()) continue;
		i=(*Cur())[Cur()->GetMaxPos()]->GetId()+1;
		if(id<i)
			id=i;
	}
	sub->SetId(id);
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
void GSession::ClearSubprofiles(GLang* lang)
{
	GSubProfiles* ptr=Data->SubProfiles.GetPtr(lang);
	if(ptr)
		ptr->Clear();
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfiles();
	GLinkCalc* LinkCalc=GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod();

	// Run all pre-profile methods that are enabled
	R::RCursor<GPreProfile> PreProfile=GPluginManagers::GetManager<GPreProfileManager>("PreProfile")->GetPlugIns();
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
		try
		{
			CalcProfile(Prof(),rec);
		}
		catch(GException& e)
		{
		}
	}

	// Save the best computed Links (As Hub and Authority)
	if((Data->SaveSubProfiles)&&(LinkCalc))
		Data->Storage->SaveLinks(this);

	// Run all post-profiles methods that are enabled
	R::RCursor<GPostProfile> PostProfile=GPluginManagers::GetManager<GPostProfileManager>("PostProfile")->GetPlugIns();
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
void GSession::CalcProfile(GProfile* profile,GSlot* rec)
{
	GProfileCalc* Profiling=GPluginManagers::GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod();
	GLinkCalc* LinkCalc=GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod();

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
		if(Data->ComputeModifiedSubProfiles&&(!Subs()->MustCompute())) continue;
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
//
// GSession::Groups
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
R::RCursor<GGroup> GSession::GetGroups(void) const
{
	return(R::RCursor<GGroup>(Data->Groups));
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNbGroups(void) const
{
	return(Data->Groups.GetNb());
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
unsigned int GSession::GetNbGroups(GLang* lang) const
{
	GGroupsLang* grps;

	grps=Data->GroupsLang.GetPtr<const GLang*>(lang);
	if(!grps)
		return(0);
	return(grps->GetNb());
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
void GSession::ClearGroups(GLang* lang)
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
void GSession::GroupingProfiles(GSlot* rec)
{
	GGrouping* Grouping=GPluginManagers::GetManager<GGroupingManager>("Grouping")->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

    // Group the subprofiles
	Grouping->Grouping(rec,Data->UseExistingGroups,Data->SaveGroups);

	// Run all post-group methods that are enabled
	R::RCursor<GPostGroup> PostGroups=GPluginManagers::GetManager<GPostGroupManager>("PostGroup")->GetPlugIns();
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
	CalcDesc=GPluginManagers::GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod();

	// Clear current clustering
	RCursor<GGroupsLang> Groups(Data->GroupsLang);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		ClearGroups(Groups()->Lang);
	}

	// Get the active languages
	RCursor<GLang> Langs=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIns();

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
	if(Data->Storage)
		Data->Storage->Disconnect(this);

	delete Data;
}
