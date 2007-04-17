/**
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
#include <gdict.h>
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
// class PerLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class PerLang
{
public:
	GLang* Lang;
	RContainer<GDoc,false,true> Docs;
	RContainer<GSubProfile,false,true> SubProfiles;
	RContainer<GGroup,false,true> Groups;

	PerLang(GLang* lang) : Lang(lang), Docs(10000), SubProfiles(1000), Groups(500)
	{}

	int Compare(const PerLang& lang) const;
	int Compare(const GLang* lang) const;
	GGroup* GetGroup(const GSubProfile* sub);
};


//-----------------------------------------------------------------------------
int PerLang::Compare(const PerLang& lang) const
{
	if(!Lang)
	{
		if(!lang.Lang) return(0);
		return(-1);
	}
	if(!lang.Lang) return(1);
	return(Lang->Compare(*lang.Lang));
}


//-----------------------------------------------------------------------------
int PerLang::Compare(const GLang* lang) const
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
GGroup* PerLang::GetGroup(const GSubProfile* sub)
{
	R::RCursor<GGroup> Groups(Groups);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		if(Groups()->IsIn(sub))
			return(Groups());
	}
	return(0);
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
	R::RContainer<GSubProfile,true,true> SubProfiles;                 // SubProfiles handled by the system.
	R::RContainer<GGroup,true,true> Groups;                           // Groups handled by the system.
	R::RContainer<PerLang,true,true> Langs;                           // Documents, Subprofiles and Groups divided by language.
	R::RContainer<GFreeId,true,true> FreeIds;                         // Free identificators for the groups.
	R::RContainer<GConceptType,true,true> ConceptTypes;               // Types of Concepts
	R::RContainer<GRelationType,true,true> RelationTypes;             // Types of Concepts
	unsigned int MaxDocs;                                             // Maximum number of documents to handle in memory.
	unsigned int MaxSubProfiles;                                      // Maximum number of subprofiles to handle in memory.
	unsigned int MaxGroups;                                           // Maximum number of groups to handle in memory.
	GFilterManager* FilterManager;                                    // Pointer to the filter manager
	
	Intern(GStorage* str,unsigned int mdocs,unsigned int maxsub,unsigned int maxgroups,unsigned int d,unsigned int u,unsigned int p,unsigned int g,unsigned int nblangs)
		: Subjects(0), GroupsHistoryMng(0), Random(0), Storage(str),  SaveResults(true),
		  Slot(0), Docs(d+(d/2),d/2), DocsRefUrl(d+(d/2),d/2),
		  Users(u,u/2), Profiles(p,p/2), SubProfiles(p*5,(p*5)/2), Groups(g+(g/2),g/2), Langs(nblangs+1), FreeIds(50,25),
		  ConceptTypes(10,5), RelationTypes(10,5),
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
			fac->GetPlugin()->GetNbSaved(otGroup),
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
void GSession::Init(void)
{
	GFactoryStorage* fac=GALILEIApp->GetManager<GStorageManager>("Storage")->GetCurrentFactory();
	fac->GetPlugin()->LoadConceptTypes();
	fac->GetPlugin()->LoadRelationTypes();
	fac->GetPlugin()->LoadRelations();
}


//------------------------------------------------------------------------------
void GSession::ForceReCompute(tObjType type)
{
	if(type==otDocs)
	{
		// Clear the information of the documents
		RCursor<GDoc> Docs(Data->Docs);
		for(Docs.Start();!Docs.End();Docs.Next())
			Docs()->ClearInfos();
	}
	else if(type==otUsers)
	{
		// Delete the subprofiles
		RCursor<PerLang> Cur(Data->Langs);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->Lang)
				Cur()->Lang->Clear(otSubProfile);
			Cur()->SubProfiles.Clear();
		}
		Data->SubProfiles.Clear();
	}
	else if(type==otGroups)
	{
		// Delete the groups
		RCursor<PerLang> Groups(Data->Langs);
		for(Groups.Start();!Groups.End();Groups.Next())
		{
			ClearGroups(Groups()->Lang);
		}
		Data->Groups.Clear();
	}
	else
		throw GException("Only 'otDocs', 'otUsers' and 'otGroups' are allowed.");
}


//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	if(Data->Subjects)
		Data->Subjects->ReInit();

	// Clear groups
	RCursor<PerLang> Groups(Data->Langs);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		ClearGroups(Groups()->Lang);
	}
	Data->Groups.Clear();

	// Clear Fdbks
	ClearFdbks();

	// Clear Users
	RCursor<PerLang> Cur(Data->Langs);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Lang)
			Cur()->Lang->Clear(otSubProfile);
		Cur()->SubProfiles.Clear();
	}
	Data->SubProfiles.Clear();
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
	GConceptType* type=Data->ConceptTypes.GetPtr(id);
	if(!type)
	{
		if(!null)
			throw GException("Unknow concept type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(const RString& name,bool null) const
{
	GConceptType* type=Data->ConceptTypes.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			throw GException("Unknow concept type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
void GSession::InsertConceptType(unsigned int id,const R::RString& name,const R::RString& desc)
{
	Data->ConceptTypes.InsertPtr(new GConceptType(id,name,desc));
}


//------------------------------------------------------------------------------
void GSession::AssignId(GConcept* data,const GDict* dict)
{
	Data->Storage->AssignId(data,dict);
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
void GSession::InsertRelation(unsigned int id,const R::RString& name,unsigned int subjectid,GLang* subjectlang,unsigned int subjecttypeid,unsigned int type,unsigned int objectid,GLang* objectlang,unsigned int objecttypeid,double weight)
{
	GDict* dict;

	if((!objectlang)||(!subjectlang))
		throw GException("Language not defined");

	// Get the concept related to the subject
	dict=subjectlang->GetDict(subjecttypeid);
	if(!dict)
		throw GException("Object "+RString::Number(subjectid)+" of type "+RString::Number(subjecttypeid)+" does not exist");
	GConcept* subject=dict->GetConcept(subjectid);
	if(!subject)
		throw GException("Object "+RString::Number(subjectid)+" of type "+RString::Number(subjecttypeid)+" does not exist");

	// Get the concept related to the object
	dict=objectlang->GetDict(objecttypeid);
	if(!dict)
		throw GException("Object "+RString::Number(objectid)+" of type "+RString::Number(objecttypeid)+" does not exist");
	GConcept* object=dict->GetConcept(objectid);
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
	PerLang* ptr;
	R::RCursor<GDoc> cur;

	ptr=Data->Langs.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(ptr->Docs);
	return(cur);
}


//-------------------------------------------------------------------------------
unsigned int GSession::GetNbDocs(GLang* lang) const
{
	PerLang* docL = Data->Langs.GetPtr<GLang*>(lang);
	if(!docL)
		return(0);
	return(docL->Docs.GetNb());
}


//-----------------------------------------------------------------------------
unsigned int GSession::FillDocs(GDoc** docs)
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
	PerLang* Lang;
	bool NewOne=false;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
	{
		AssignId(d);
		NewOne=true;
	}

	// Insert the document
	Data->Docs.InsertPtrAt(d,d->GetId());
	Lang = Data->Langs.GetInsertPtr<GLang*>(d->GetLang());
	if(!Lang)
		Data->Langs.InsertPtr(Lang=new PerLang(d->GetLang()));
	Lang->Docs.InsertPtr(d);

	//insert the doc in the DocsRefUrl container.
	Data->DocsRefUrl.InsertPtr(new GDocRefURL(d));

	if(NewOne)
	{
		// If new one and all documents are in memory -> store it in the database
		if(Data->Storage->IsAllInMemory())
			Data->Storage->SaveDoc(d);
		Event(d,eObjCreate);
	}
}


//-----------------------------------------------------------------------------
void GSession::MoveDoc(GDoc* d)
{
	PerLang* Lang;

	// Remove doc from container of docs with no language
	Lang=Data->Langs.GetPtr<GLang*>(0);
	if(!Lang)
		Data->Langs.InsertPtr(Lang=new PerLang(0));
	Lang->Docs.DeletePtr(d);

	// Move doc to container of appropriated language.
	Lang=Data->Langs.GetInsertPtr(d->GetLang());
	if(!Lang)
		Data->Langs.InsertPtr(Lang=new PerLang(d->GetLang()));
	Lang->Docs.InsertPtr(d);
}


//-----------------------------------------------------------------------------
void GSession::ClearDocs(void)
{
	RCursor<PerLang> Cur(Data->Langs);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Docs.Clear();
	Data->DocsRefUrl.Clear();
	Data->Docs.Clear();
}


//------------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec)
{
	GDocXML* xml;
	RContainer<GDoc,false,true> tmpDocs(5,2);
	bool Cont;               // Continue the analysis

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Data->Storage->GetFactory()->GetName()+ " on : " +RString::Number(RDate::GetToday().GetDay())+"/"+ RString::Number(RDate::GetToday().GetMonth())+"/"+RString::Number(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyse documents");
	}

	// Analyse the documents
	do
	{
		// Get the method
		GDocAnalyse* Analyse=GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod();
		if(!Analyse)
			throw GException("No document analysis method chosen.");
		
		// Go through the existing documents
		R::RCursor<GDoc> Docs=GetDocs();
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			try
			{
				xml=0;       // DocXML to create
				tmpDocs.Clear();
				AnalyseDoc(xml,Docs(),Analyse,&tmpDocs,rec);

				// Add the new documents.
				// Continue the analysis if documents were added.
				RCursor<GDoc> Cur(tmpDocs);
				Cont=tmpDocs.GetNb();
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					InsertDoc(Cur());
					//#warning add the document with the Cur()->GetId()
				}
				delete xml;
			}
			HANDLEALLEXCEPTIONS(rec,Docs()->GetURL()+"("+RString::Number(Docs()->GetId())+"):")
		}
	}
	while(Cont);

	DoPostDocs(rec);
}


//------------------------------------------------------------------------------
void GSession::AnalyseDoc(GDocXML* &xml,GDoc* doc,GDocAnalyse* method,RContainer<GDoc,false,true>* newdocs,GSlot* rec)
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
	try
	{
		doc->SetStatus(dsCannotAccess); // Suppose it cannot be access
		if(!xml)
			xml=Data->FilterManager->CreateDocXML(doc);
		if(xml)
		{
			// Analyse document -> Is something goes wrong -> It failed
			// If a log file specified -> write to it and it is OK
			// If no log file specified -> Propagate error
			doc->SetStatus(dsCannotAnalyse); // Suppose it cannot be analysed
			method->Analyze(xml,doc,newdocs);
			if((undefLang)&&(doc->GetLang()))
			{
				MoveDoc(doc);
			}
			doc->SetStatus(dsOK); // OK, everything fine
		}
	}
	HANDLEALLEXCEPTIONS(rec,doc->GetURL()+"("+RString::Number(doc->GetId())+"):")

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
		Event(user,eObjCreate);
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
		Event(p,eObjCreate);
	}
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GSession::GetSubProfiles(const GLang* lang) const
{
	PerLang* ptr=const_cast<GSession*>(this)->Data->Langs.GetInsertPtr(const_cast<GLang*>(lang));
	if(ptr)
		return(RCursor<GSubProfile>(ptr->SubProfiles));
	return(RCursor<GSubProfile>());
}


//------------------------------------------------------------------------------
size_t GSession::GetSubProfilesNb(const GLang* lang) const
{
	if(lang)
	{
		PerLang* ptr=Data->Langs.GetPtr(lang);
		if(ptr)
			return(ptr->SubProfiles.GetNb());
		else
			return(0);
	}
	size_t nb=0;
	RCursor<PerLang> Sub(Data->Langs);
	for(Sub.Start();!Sub.End();Sub.Next())
		nb+=Sub()->SubProfiles.GetNb();
	return(nb);
}


//------------------------------------------------------------------------------
GSubProfile* GSession::GetSubProfile(unsigned int id,bool load,bool null) const
{
	GSubProfile* s;

	if(id>Data->SubProfiles.GetMaxPos())
		return(0);
	s=Data->SubProfiles[id];
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

	// The first subprofile has the identificator 1
	if(Data->SubProfiles.GetNb())
		sub->SetId(Data->SubProfiles[Data->SubProfiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first subprofile has an identificator of 1
	else
		sub->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::InsertSubProfile(GSubProfile* s)
{
	GLang* l;
	PerLang* list;
	bool NewOne=false;

	if(s->GetId()==cNoRef)
	{
		AssignId(s);
		NewOne=true;
	}
	Data->SubProfiles.InsertPtrAt(s,s->GetId());
	l=s->GetLang();
	list=Data->Langs.GetPtr<const GLang*>(l);
	if(!list)
		Data->Langs.InsertPtr(list=new PerLang(l));
	list->SubProfiles.InsertPtr(s);
}


//------------------------------------------------------------------------------
/*void GSession::ClearSubprofiles(GLang* lang)
{
	cout<<"Problem"<<endl;
	if(lang)
		lang->Clear(otSubProfile);
	PerLang* ptr=Data->Langs.GetPtr(lang);
	if(ptr)
		ptr->SubProfiles.Clear();
}*/


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
		try
		{
			CalcProfile(Prof(),rec);
		}
		catch(GException& e)
		{
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
	PerLang* ptr;
	R::RCursor<GGroup> cur;

	ptr=Data->Langs.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(ptr->Groups);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GSession::GetNbGroups(GLang* lang) const
{
	PerLang* grps;

	grps=Data->Langs.GetPtr<const GLang*>(lang);
	if(!grps)
		return(0);
	return(grps->Groups.GetNb());
}


//------------------------------------------------------------------------------
GGroup* GSession::GetGroup(const GSubProfile* sub,bool load,bool null) const
{
	PerLang* groupsLang=Data->Langs.GetInsertPtr<GLang*>(sub->GetLang());
	if(!groupsLang)
		throw GException("No language defined");
	GGroup* grp=groupsLang->GetGroup(sub);
	if(grp)
		return(grp);
	if(!load)
		return(0);
	if(Data->Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			throw GException("Unknown group "+RString::Number(sub->GetGroupId()));
	}
	grp=Data->Storage->LoadGroup(sub->GetGroupId());
	if(!grp)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown group "+RString::Number(sub->GetGroupId()));
	}
	const_cast<GSession*>(this)->InsertGroup(grp);
	return(grp);
}


//------------------------------------------------------------------------------
GGroup*GSession::GetGroup(unsigned int id,bool load,bool null) const
{
	GGroup* grp=Data->Groups.GetPtr<unsigned int>(id);
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
	grp=Data->Storage->LoadGroup(id);
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

	// The first group has the identificator 1
	// Is there a free identificator -> Take the first one.
	if(Data->FreeIds.GetNb())
	{
		unsigned int id=Data->FreeIds[0]->Id;
		Data->FreeIds.DeletePtrAt(0);
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
	PerLang* groupsLang;

	Data->Groups.InsertPtr(grp);
	groupsLang = Data->Langs.GetInsertPtr<GLang*>(grp->GetLang());
	if(!groupsLang)
		Data->Langs.InsertPtr(groupsLang=new PerLang(grp->GetLang()));
	groupsLang->Groups.InsertPtr(grp);
}


//------------------------------------------------------------------------------
void GSession::DeleteGroup(GGroup* grp)
{
	PerLang* groupsLang=Data->Langs.GetInsertPtr<GLang*>(grp->GetLang());
	if(groupsLang)
		groupsLang->Groups.DeletePtr(grp);
	Data->FreeIds.InsertPtr(new GFreeId(grp->GetId()));
	Data->Groups.DeletePtr(grp);
}


//------------------------------------------------------------------------------
void GSession::ClearGroups(GLang* lang)
{
	size_t i,nb;
	PerLang* grps=Data->Langs.GetPtr<const GLang*>(lang);
	GGroup* grp;

	// Go through the groups and delete all invalid groups.
	if(!grps) return;

	if(lang)
		lang->Clear(otGroup);
	for(nb=grps->Groups.GetNb(),i=0;i<nb;i++)
	{
		grp=grps->Groups[i];
		Data->FreeIds.InsertPtr(new GFreeId(grp->GetId()));
		Data->Groups.DeletePtr(grp);
	}
	grps->Groups.Clear();
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
	RCursor<PerLang> Groups(Data->Langs);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		ClearGroups(Groups()->Lang);
	}

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
	GSubProfile* sub=GetSubProfile(subid,0,false);
	if(sub)
	{
		GGroup* grp=GetGroup(sub->GetGroupId(),false);
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
