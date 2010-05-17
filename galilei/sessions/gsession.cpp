/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
#include <rxmlfile.h>
#include <rdir.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <gdoc.h>
#include <gdocstruct.h>
#include <gdocanalyze.h>
#include <glinkcalc.h>
#include <glink.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <genginedoc.h>
#include <guser.h>
#include <gprofile.h>
#include <gprofilecalc.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <gsubject.h>
#include <gfilter.h>
#include <gweightinfo.h>
#include <ggalileiapp.h>
#include <gsimulator.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
#include <gtool.h>
#include <gpredicate.h>
#include <gstatement.h>
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
const size_t SizeRecDesc=sizeof(size_t)+sizeof(double);
const size_t SizeRecStruct=sizeof(size_t)+sizeof(char)+sizeof(size_t)+sizeof(char)+sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);



//------------------------------------------------------------------------------
//
// class GSession::Type
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSession::IndexType
{
public:
	tObjType Type;
	RString Name;
	bool IndexInc;
	RIndexFile* Desc;
	RIndexFile* Index;

	IndexType(tObjType type,const RString& name) : Type(type), Name(name), IndexInc(false), Desc(0), Index(0) {}
	int Compare(const IndexType& type) const {return(Type-type.Type);}
	int Compare(tObjType type) const {return(Type-type);}
};




//------------------------------------------------------------------------------
//
//  GSession::GDocRefURL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSession::GDocRefURL
{
public:
	GDoc* Doc;

	GDocRefURL(GDoc* d) : Doc(d) {}
	int Compare(const GDocRefURL& doc) const {return(Doc->GetURL().Compare(doc.Doc->GetURL()));}
	int Compare(const RString& url) const {return(Doc->GetURL().Compare(url));}
};



//------------------------------------------------------------------------------
//
// GSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::GSession(size_t id,const RString& name)
	: RConfig("lib/galilei/sessions",name), Id(id), Name(name), Storage(0), SaveResults(true), Log("/var/log/galilei/"+name+".log"), Simulator(0), Random(0),
	  ConceptTypes(20), ConceptTypesByIds(20), Concepts(50000,10000),
	  Predicates(30), PredicatesByIds(30), Statements(5000,5000),
	  StructDoc(0), tmpRefs(5000), Types(5), TypesNames(5),
	  Docs(20000), DocsLoaded(false), DocsRefUrl(20000),
	  Users(1000), UsersLoaded(false), Profiles(5000),
	  Communities(100), CommunitiesLoaded(false),
	  Topics(200), TopicsLoaded(false), Classes(300,100), ClassesLoaded(false),
	  MaxDocs(0), MaxProfiles(0), MaxGroups(0),
	  Subjects(200), ExternBreak(false)

{
	// Log files
	Log.Open(RIO::Append);

	// Random
	CurrentRandom=0;
	Random=RRandom::Create(RRandom::Good,CurrentRandom);

	// Indexer
	Types.InsertPtr(new IndexType(otDoc,"Documents"));
	TypesNames.InsertPtr(new RString("Documents"));
	Types.InsertPtr(new IndexType(otProfile,"Profiles"));
	TypesNames.InsertPtr(new RString("Profiles"));
	Types.InsertPtr(new IndexType(otCommunity,"Communities"));
	TypesNames.InsertPtr(new RString("Communities"));
	Types.InsertPtr(new IndexType(otTopic,"Topics"));
	TypesNames.InsertPtr(new RString("Topics"));
	Types.InsertPtr(new IndexType(otClass,"Classes"));
	TypesNames.InsertPtr(new RString("Classes"));
}


//------------------------------------------------------------------------------
void GSession::AnalyzeDoc(GDoc* doc,bool ram,GDocAnalyze* method,GSlot* rec)
{
	// Verify that the document analysis method is selected
	if(!method)
		method=GALILEIApp->GetCurrentPlugIn<GDocAnalyze>("DocAnalyze");

	if(!doc->MustCompute()) return;
	if(rec)
	{
		rec->Interact();
		rec->NextDoc(doc);
	}
	if(ExternBreak) return;

	//cout<<"Analyze "<<file.GetPath()<<endl;
	R::RIO::RSmartTempFile TmpFile;
	RURI File;
	bool Save=(SaveResults&&(doc->GetId()!=cNoRef));
	GFilter* Filter(GALILEIApp->FindMIMEType(doc,File,TmpFile));
   	bool DelRef(doc->IsDefined());
   	method->SetHTMLMode(false);
	method->PrepareAnalyze(doc,Filter==0);
	if(Filter)
	{
		Filter->Clear(method);
		Filter->Analyze(doc,File,method,rec);
	}
	else
	{
		method->Open(File,RIO::Read,"UTF-8");
		method->Close();
	}
	method->TerminateAnalyze();

	// Save the description and the structure
	if(Save)
	{
		if(method->Infos.IsDefined())
			SaveInfos(method->Infos,otDoc,doc->BlockId,doc->Id);
		if(method->Struct.GetNbRecs())
			SaveStruct(&method->Struct,doc->StructId,doc->Id);
	}

	// Set the information to the document
	doc->Update(this,method->Lang,method->Infos,method->Struct,ram||(!Save),DelRef);

	// Save the information related to the document
	if(Save)
	{
		Storage->SaveDoc(doc);
		if(ram)
			doc->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
void GSession::AnalyzeDocs(bool ram,GSlot* rec)
{
	// Opens and appends the Log File for all errors occurring in the filter or analyze phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Storage->GetFactory()->GetName()+ " on : " +RString::Number(RDate::GetToday().GetDay())+"/"+ RString::Number(RDate::GetToday().GetMonth())+"/"+RString::Number(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyze documents");
	}

	// Get the method
	GDocAnalyze* Analyze(GALILEIApp->GetCurrentPlugIn<GDocAnalyze>("DocAnalyze"));

	// Analyze the documents - Go through the existing documents
	R::RCursor<GDoc> Docs=GetDocs();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		try
		{
			AnalyzeDoc(Docs(),ram,Analyze,rec);
		}
		// If a log file specified -> write to it and it is OK
		// If no log file specified -> Propagate error
		HANDLEALLEXCEPTIONS(rec,Docs()->GetURL()()+"("+RString::Number(Docs()->GetId())+"): ")
	}
	FlushDesc(otDoc);   // Force to save all documents description
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
						Res+=Name;
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
void GSession::ApplyConfig(void)
{
	// Load the configuration of the  simulator and the indexer
	if(Simulator)
		Simulator->ApplyParams();
	ClusterSelectedDocs=GetBool("ClusterSelectedDocs","Simulator","Documents");

	// Create (if necessary) the directory corresponding to the name of the session
	// Create all the index files
	RString Dir(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Name+RFile::GetDirSeparator());
	try
	{
		RDir::CreateDirIfNecessary(Dir,true);

		// Parse all types
		RCursor<IndexType> Type(Types);
		for(Type.Start();!Type.End();Type.Next())
		{
			Type()->Desc=new RIndexFile(Dir+Type()->Name+".desc",
					GetUInt("BlockSize","Indexer",Type()->Name,"Description"),
					GetUInt("CacheSize","Indexer",Type()->Name,"Description"),
					GetUInt("Tolerance","Indexer",Type()->Name,"Description"));
			Type()->Desc->Open();
			Type()->Desc->SetCacheType(static_cast<RBlockFile::CacheType>(GetInt("Type","Indexer",Type()->Name,"Description")));
			Type()->IndexInc=GetBool("Increment","Indexer",Type()->Name,"Index");
			Type()->Index=new RIndexFile(Dir+Type()->Name+".index",
					GetUInt("BlockSize","Indexer",Type()->Name,"Index"),
					GetUInt("CacheSize","Indexer",Type()->Name,"Index"),
					GetUInt("Tolerance","Indexer",Type()->Name,"Index"));
			Type()->Index->Open();
			Type()->Index->SetCacheType(static_cast<RBlockFile::CacheType>(GetInt("Type","Indexer",Type()->Name,"Index")));
		}

		StructDoc=new RIndexFile(Dir+"Documents.struct",
				GetUInt("BlockSize","Indexer","Documents","Structure"),
				GetUInt("CacheSize","Indexer","Documents","Structure"),
				GetUInt("Tolerance","Indexer","Documents","Structure"));
		StructDoc->Open();
		StructDoc->SetCacheType(static_cast<RBlockFile::CacheType>(GetInt("Type","Indexer","Documents","Structure")));
	}
	catch(...)
	{
		cerr<<"GSession::Apply: Problems in creating directories in '"<<Dir<<"'"<<endl;
	}
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
	if(Communities.GetNb())
		com->SetId(Communities[Communities.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first community has an identifier of 1
	else
		com->SetId(1);
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
	if(Docs.GetNb())
		doc->SetId(Docs[Docs.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first doc has an identifier of 1
	else
		doc->SetId(1);
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
	if(Profiles.GetNb())
		p->SetId(Profiles[Profiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first profile has an identifier of 1
	else
		p->SetId(1);
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
	if(Topics.GetNb())
		top->SetId(Topics[Topics.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first topic has an identifier of 1
	else
		top->SetId(1);
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
	if(Users.GetNb())
		user->SetId(Users[Users.GetMaxPos()]->GetId()+1); // Not [GetNb()-1] because first user has an identifier of 1
	else
		user->SetId(1);
}


//------------------------------------------------------------------------------
void GSession::AssignInfos(GClass* theclass,GWeightInfos& infos)
{
	theclass->Update(this,infos);
	if(SaveResults)
	{
		if(theclass->Vector)
			SaveInfos(*theclass->Vector,otClass,theclass->BlockId,theclass->Id);
		Storage->SaveClass(theclass);
	}
}


//------------------------------------------------------------------------------
bool GSession::MustBreak(void)
{
	return(ExternBreak);
}


//------------------------------------------------------------------------------
void GSession::BuildRefs(tObjType type,GSlot* slot)
{
	IndexType* ptr(Types.GetPtr(type));
	if(!ptr)
		ThrowGException(GetObjType(type,true,true)+" do not have index");

	GSession* Session(dynamic_cast<GSession*>(this));

	// Clear the file and put all block identifier of concepts to 0.
	if(slot)
		slot->WriteStr("Clear the index");
	ptr->Index->Clear();
	RCursor<GConcept> Concepts(Session->GetConcepts());
	for(Concepts.Start();!Concepts.End();Concepts.Next())
	{
		Concepts()->SetIndex(type,0);

		// If no cache is asked -> Save each time
		if(ptr->Index->GetCacheType()==RBlockFile::WriteThrough)
		{
			size_t idx(0);
			Storage->SaveIndex(Concepts(),otDoc,idx);
			Concepts()->SetIndex(type,idx);
		}
	}

	// Go trough each document
	R::RCursor<GDoc> Cur(dynamic_cast<GSession*>(this)->GetDocs());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(slot)
			slot->NextDoc(Cur());

		// Position the block file to the correct position and read the size
		ptr->Desc->Seek(Cur()->BlockId,Cur()->Id);
		size_t size,concept;
		double weight;
		ptr->Desc->Read((char*)&size,sizeof(size_t));
		for(size_t i=0;i<size;i++)
		{
			// Read concept identifier and weight
			ptr->Desc->Read((char*)&concept,sizeof(size_t));
			ptr->Desc->Read((char*)&weight,sizeof(double));

			// Read the vector representing the current index
			GConcept* Concept(Session->GetConcept(concept));
			if(Concept->GetIndex(type))
				ptr->Index->Read(Concept->GetIndex(type),Concept->Id,tmpRefs);
			else
				tmpRefs.Clear();

			// Add the document to the index
			size_t oldsize(tmpRefs.GetNb());
			tmpRefs.Insert(Cur()->Id);

			// If the size of vector has changed -> Save it back
			if(oldsize!=tmpRefs.GetNb())
			{
				size_t idx(Concept->GetIndex(type));
				ptr->Index->Write(idx,Concept->Id,tmpRefs);
				Concept->SetIndex(type,idx);

				// If no cache is asked -> Save each time
				if(ptr->Index->GetCacheType()==RBlockFile::WriteThrough)
					Storage->SaveIndex(Concept,otDoc,Concept->GetIndex(type));
			}
		}
	}

	// If cache was asked, flush the cache and save the block identifier of the concepts
	if(ptr->Index->GetCacheType()==RBlockFile::WriteBack)
	{
		if(slot)
			slot->WriteStr("Flush the index");
		ptr->Index->Flush();
		RCursor<GConcept> Concepts(Session->GetConcepts());
		for(Concepts.Start();!Concepts.End();Concepts.Next())
			Storage->SaveIndex(Concepts(),otDoc,Concepts()->GetIndex(type));
	}
}


//------------------------------------------------------------------------------
void GSession::CalcProfile(GProfile* profile,GProfileCalc* method,GLinkCalc* linkcalc,GSlot* rec)
{
	if(!method)
		method=GALILEIApp->GetCurrentPlugIn<GProfileCalc>("ProfileCalc");

	if(rec)
		rec->NextProfile(profile);

	// If necessary, compute Links on the profile description
	if(linkcalc)
		linkcalc->Compute(profile);

	if(rec)
		rec->Interact();

	if(ExternBreak) return;
	if(!profile->MustCompute()) return;
	bool Save(SaveResults&&(profile->GetId()!=cNoRef));
	method->Compute(profile);

	// Set the Variable of the profile
	profile->Update(this,method->Infos);

	if(Save)
	{
		if(profile->GetVector().GetNb())
			SaveInfos(profile->GetVector(),otProfile,profile->BlockId,profile->Id);
		Storage->SaveProfile(profile);
	}
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	GProfileCalc* Profiling(GALILEIApp->GetCurrentPlugIn<GProfileCalc>("ProfileCalc"));
	GLinkCalc* LinkCalc(GALILEIApp->GetCurrentPlugIn<GLinkCalc>("LinkCalc",RString::Null,false));

	R::RCursor<GProfile> Prof=GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(ExternBreak) return;
		try
		{
			CalcProfile(Prof(),Profiling,LinkCalc,rec);
		}
		catch(GException& e)
		{
			cerr<<e.GetMsg()<<endl;
		}
	}
	FlushDesc(otProfile);   // Force to save all profiles description
}


//------------------------------------------------------------------------------
void GSession::Clear(tObjType type,tObjType meta)
{
	switch(meta)
	{
		case otNoClass:
		switch(type)
		{
			case otDoc:
				DocsRefUrl.Clear();
				Docs.Clear();
				break;
			case otFdbk:
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
				break;
			}
			case otCommunity:
				Communities.Clear();
				if(SaveResults)
				{
					Storage->Clear(otCommunity);
					Clear(otCommunity,otDescFile);
					Clear(otCommunity,otIndexFile);
				}
				break;
			case otTopic:
				Topics.Clear();
				if(SaveResults)
				{
					Storage->Clear(otTopic);
					Clear(otTopic,otDescFile);
					Clear(otTopic,otIndexFile);
				}
				break;
			case otSubject:
				Subjects.Clear();
				Subjects.ProfilesSubject.Clear();
				Subjects.SelectedDocs.Clear();
				Subjects.DocsSubjects.Clear();
				Subjects.Clear();
				Subjects.MaxDepth=0;
				break;
			default:
				ThrowGException(GetObjType(type,true,true)+" are not managed");
		}
		break;
		case otDescFile:
		{
			IndexType* ptr(Types.GetPtr(type));
			if(!ptr)
				ThrowGException(GetObjType(type,true,true)+" do not have descriptions");
			ptr->Desc->Clear();
			break;
		}
		case otStructFile:
			if(type!=otDoc)
				ThrowGException("Only documents have structure file");
			StructDoc->Clear();
			break;
		case otIndexFile:
		{
			IndexType* ptr(Types.GetPtr(type));
			if(!ptr)
				ThrowGException(GetObjType(type,true,true)+" do not have index");
			ptr->Index->Clear();
			break;
		}
		case otReference:
		{
			RCursor<GConceptType> Types(ConceptTypes);
			for(Types.Start();!Types.End();Types.Next())
				Types()->ClearRef(type);
			break;
		}
		default:
			ThrowGException(GetObjType(meta,true,true)+" is not a valid file type");
	}
}


//------------------------------------------------------------------------------
int GSession::Compare(const GSession& session) const
{
	return(CompareIds(Id,session.Id));
}


//------------------------------------------------------------------------------
int GSession::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int GSession::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSession::ComputeSugs(GSlot* rec)
{
	// Run all suggestions methods that are enabled
	R::RCastCursor<GPlugIn,GComputeSugs> ComputeSugs(GALILEIApp->GetPlugIns<GComputeSugs>("ComputeSugs"));
	for(ComputeSugs.Start();!ComputeSugs.End();ComputeSugs.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(rec)
			rec->WriteStr("Suggestions computing method : "+ComputeSugs()->GetFactory()->GetName());
		ComputeSugs()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::ComputeTrust(GSlot* rec)
{
	// Run all trust methods that are enabled
	R::RCastCursor<GPlugIn,GComputeTrust> ComputeTrust(GALILEIApp->GetPlugIns<GComputeTrust>("ComputeTrust"));
	for(ComputeTrust.Start();!ComputeTrust.End();ComputeTrust.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(rec)
			rec->WriteStr("Trust computing method : "+ComputeTrust()->GetFactory()->GetName());
		ComputeTrust()->Run();
	}
}


//------------------------------------------------------------------------------
void GSession::Delete(GCommunity* com)
{
	Communities.DeletePtr(com);
}


//------------------------------------------------------------------------------
void GSession::Delete(GConcept* concept)
{
	if((!concept)||(!concept->GetType()))
		ThrowGException("Cannot delete concept");
	Storage->DeleteConcept(concept);
	concept->GetType()->DeletePtr(*concept);
	Concepts.DeletePtrAt(concept->GetId(),false);
}


//------------------------------------------------------------------------------
void GSession::Delete(GTopic* top)
{
	Topics.DeletePtrAt(top->GetId(),false);
}


//-----------------------------------------------------------------------------
size_t GSession::FillTopics(GTopic** topics)
{
	return(Topics.GetTab(topics,1,Topics.GetMaxPos()));
}


//-----------------------------------------------------------------------------
size_t GSession::FillSelectedDocs(GDoc** docs)
{
	return(Subjects.SelectedDocs.GetTab(docs,1,Subjects.SelectedDocs.GetMaxPos()));
}


//-----------------------------------------------------------------------------
size_t GSession::FillSubjects(GSubject** subjects)
{
	return(Subjects.GetTab(subjects));
}


//------------------------------------------------------------------------------
void GSession::FlushDesc(tObjType type)
{
	IndexType* ptr(Types.GetPtr(type));
	if(!ptr)
		ThrowGException(GetObjType(type,true,true)+" do not have descriptions");
	ptr->Desc->Flush();
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
			RCursor<GDoc> Docs(Docs);
			for(Docs.Start();!Docs.End();Docs.Next())
			{
				Docs()->ClearInfos(SaveResults);
				Docs()->ClearStruct(SaveResults);
				if(SaveResults)
					Storage->SaveDoc(Docs());
			}
			if(SaveResults)
			{
				Clear(otDoc,otDescFile);
				Clear(otDoc,otIndexFile);
				Clear(otDoc,otStructFile);
			}
			Break=false;
		}
		case otTopic:
		{
			// Delete the topics
			Clear(otTopic,otReference);
			Topics.Clear();
			if(SaveResults)
			{
				Clear(otTopic,otDescFile);
				Clear(otTopic,otIndexFile);
				Storage->Clear(otTopic);
			}
			if(Break)
				break;
		}
		case otProfile:
		{
			// Delete the profiles -> Also groups
			Clear(otProfile,otReference);
			Profiles.Clear();
			if(SaveResults)
			{
				Clear(otProfile,otDescFile);
				Clear(otProfile,otIndexFile);
				Storage->Clear(otProfile);
			}
		}
		case otCommunity:
		{
			// Delete the communities
			Clear(otCommunity,otReference);
			Communities.Clear();
			if(SaveResults)
			{
				Clear(otCommunity,otDescFile);
				Clear(otCommunity,otIndexFile);
				Storage->Clear(otCommunity);
			}
			break;
		}
		case otClass:
		{
			// Delete the classes
			Clear(otClass,otReference);
			Classes.Clear();
			if(SaveResults)
			{
				Clear(otClass,otDescFile);
				Clear(otClass,otIndexFile);
				Storage->Clear(otClass);
			}
			break;
		}
		default:
			ThrowGException(GetObjType(type,true,true)+" are not allowed");
	}
}


//------------------------------------------------------------------------------
GClass* GSession::GetClass(size_t id,bool null)
{
	RCursor<GClass> Cur(Classes.GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetId()==id)
			return(Cur());
	if(!null)
		ThrowGException("GSession::GetClass(size_t,bool): Class '"+RString::Number(id)+"' not found");
	return(0);
}


//------------------------------------------------------------------------------
RCursor<GCommunity> GSession::GetCommunities(void)
{
	return(RCursor<GCommunity>(Communities));
}


//------------------------------------------------------------------------------
GCommunity* GSession::GetCommunity(size_t id,bool load,bool null)
{
	if(id==cNoRef)
	{
		if(null)
			return(0);
		ThrowGException("Unknown community "+RString::Number(id));
	}
	GCommunity* grp=Communities.GetPtr(id);
	if(grp)
		return(grp);

	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown community "+RString::Number(id));
	}
	grp=Storage->LoadCommunity(id);
	if(!grp)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown community "+RString::Number(id));
	}
	Insert(grp);
	return(grp);
}


//------------------------------------------------------------------------------
GConcept* GSession::GetConcept(size_t id)
{
	if(id>Concepts.GetMaxPos())
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	GConcept* concept(Concepts[id]);
	if(!concept)
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(concept);
}



//------------------------------------------------------------------------------
R::RCursor<GConcept> GSession::GetConcepts(void) const
{
	return(R::RCursor<GConcept>(Concepts));
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(char id,bool null)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypesByIds[id];
	}
	catch(...)
	{
	}
	if((!type)&&(!null))
		ThrowGException("Unknown concept type "+RString::Number(id));
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(const RString& name,const RString& desc)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypes.GetPtr(name);
	}
	catch(...)
	{
	}
	if(!type)
	{
		ConceptTypes.InsertPtr(type=new GConceptType(this,0,name,desc,5000));
		Storage->AssignId(type);
		ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GConceptType* GSession::GetConceptType(const RString& name,bool null)
{
	GConceptType* type(0);
	try
	{
		type=ConceptTypes.GetPtr(name);
	}
	catch(...)
	{
	}
	if((!type)&&(!null))
		ThrowGException("Unknown concept type '"+name+"'");
	return(type);
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(size_t id,bool load,bool null)
{
	GDoc* d;

	if(id>Docs.GetMaxPos())
		return(0);
	d=Docs[id];
	if(d)
		return(d);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown document "+RString::Number(id));
	}
	d=Storage->LoadDoc(id);
	if(!d)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown document "+RString::Number(id));
	}
	Insert(d);
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GSession::GetDoc(const RString& url,bool,bool null) const
{
	GDocRefURL* ref;

	ref=DocsRefUrl.GetPtr(url);
	if(!ref)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown document '"+RString(url)+"'");
	}
	return(ref->Doc);
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GSession::GetDocs(void) const
{
	return(R::RCursor<GDoc>(Docs));
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxDepth(void) const
{
	LoadSubjects();
	return(Subjects.MaxDepth);
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxObjectId(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			if(!Docs.GetNb())
				return(0);
			return(Docs[Docs.GetMaxPos()]->GetId());
		case otProfile:
			if(!Profiles.GetNb())
				return(0);
			return(Profiles[Profiles.GetMaxPos()]->GetId());
		case otCommunity:
			if(!Communities.GetNb())
				return(0);
			return(Communities[Communities.GetMaxPos()]->GetId());
		case otTopic:
			if(!Topics.GetNb())
				return(0);
			return(Topics[Topics.GetMaxPos()]->GetId());
		default:
			ThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxObjectPos(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			return(Docs.GetMaxPos());
		case otTopic:
			return(Topics.GetMaxPos());
		default:
			ThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
size_t GSession::GetNbIdealGroups(tObjType type) const
{
	LoadSubjects();
	size_t nb(0);
	RCursor<GSubject> Cur(Subjects.GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSession::GetNbObjects(tObjType type) const
{
	switch(type)
	{
		case otUser:
			return(Users.GetNb());
		case otConceptType:
			return(ConceptTypes.GetNb());
		case otDoc:
			return(Docs.GetNb());
		case otProfile:
			return(Profiles.GetNb());
		case otCommunity:
			return(Communities.GetNb());
		case otTopic:
			return(Topics.GetNb());
		case otSubject:
			LoadSubjects();
			return(Subjects.GetNb());
		default:
			ThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//------------------------------------------------------------------------------
size_t GSession::GetNbSubjects(GDoc* doc) const
{
	LoadSubjects();
	const R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	return(line->GetNb());
}


//-----------------------------------------------------------------------------
size_t GSession::GetNbTopicsDocs(void) const
{
	LoadSubjects();
	size_t nb(0);
	RCursor<GSubject> Cur(Subjects.GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//-----------------------------------------------------------------------------
GObject* GSession::GetObject(tObjType type,size_t id,bool null)
{
	switch(type)
	{
		case otConcept:
			return(Concepts[id]);
		case otConceptType:
			return(GetConceptType(id,null));
		case otDoc:
			return(GetDoc(id,null));
		case otUser:
			return(GetUser(id,null));
		case otProfile:
			return(GetProfile(id,null));
		case otCommunity:
			return(GetCommunity(id,null));
		case otTopic:
			return(GetTopic(id,null));
		default:
			ThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//------------------------------------------------------------------------------
size_t GSession::GetObjects(tObjType type,GObject** &tab,bool alloc)
{
	switch(type)
	{
		case otDoc:
			if(alloc)
				tab=new GObject*[Docs.GetMaxPos()+1];
			return(Docs.GetTab(reinterpret_cast<void**>(tab)));
		case otProfile:
			if(alloc)
				tab=new GObject*[Profiles.GetMaxPos()+1];
			return(Profiles.GetTab(reinterpret_cast<void**>(tab)));
		case otCommunity:
			if(alloc)
				tab=new GObject*[Communities.GetMaxPos()+1];
			return(Communities.GetTab(reinterpret_cast<void**>(tab)));
		case otTopic:
			if(alloc)
				tab=new GObject*[Topics.GetMaxPos()+1];
			return(Topics.GetTab(reinterpret_cast<void**>(tab)));
		default:
			ThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
GPredicate* GSession::GetPredicate(size_t id,bool null)
{
	GPredicate* type=PredicatesByIds[id];
	if(!type)
	{
		if(!null)
			ThrowGException("Unknown relation type "+RString::Number(id));
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
GPredicate* GSession::GetPredicate(const RString& name,bool null)
{
	GPredicate* type=Predicates.GetPtr(name,false);
	if(!type)
	{
		if(!null)
			ThrowGException("Unknown relation type "+name);
		return(0);
	}
	return(type);
}


//-----------------------------------------------------------------------------
RCursor<GPredicate> GSession::GetPredicates(void) const
{
	return(RCursor<GPredicate>(Predicates));
}


//------------------------------------------------------------------------------
GProfile* GSession::GetProfile(size_t id,bool load,bool null)
{
	GProfile* p;

	if(id>Profiles.GetMaxPos())
		return(0);
	p=Profiles[id];
	if(p)
		return(p);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown profile "+RString::Number(id)+" in memory");
	}
	p=Storage->LoadProfile(id);
	if(!p)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown profile "+RString::Number(id)+" in storage");
	}
	Insert(p);
	return(p);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GSession::GetProfiles(void) const
{
	return(R::RCursor<GProfile>(Profiles));
}


//------------------------------------------------------------------------------
GSimulator* GSession::GetSimulator(void) const
{
	if(!Simulator)
	{
		const_cast<GSession*>(this)->Simulator=new GSimulator(const_cast<GSession*>(this));
		Simulator->ApplyParams();
	}
	return(Simulator);
}


//-----------------------------------------------------------------------------
GStatement* GSession::GetStatement(size_t id)
{
	GStatement* Statement(Statements[id]);
	if(!Statement)
		ThrowGException("'"+RString::Number(id)+"' is not a valid concept identifier");
	return(Statement);
}


//------------------------------------------------------------------------------
GSubject* GSession::GetSubject(size_t id) const
{
	LoadSubjects();
	return(Subjects.Subjects.GetPtr(id));
}


//------------------------------------------------------------------------------
const GSubject* GSession::GetSubject(GCommunity* com) const
{
	LoadSubjects();
	RCursor<GSubject> Cur(GetSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Community==com)
			return(Cur());
	}
	return(0);
}


//------------------------------------------------------------------------------
const GSubject* GSession::GetSubject(GDoc* doc) const
{
	const R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	if(line->GetNb()>1)
		ThrowGException("Document has multiple subjects");
	return((*line)[0]);
}


//------------------------------------------------------------------------------
GSubject* GSession::GetSubject(const RString& name) const
{
	LoadSubjects();
	return(Subjects.GetNode(name));
}


//------------------------------------------------------------------------------
const GSubject* GSession::GetSubject(GProfile* prof) const
{
	LoadSubjects();
	return(Subjects.ProfilesSubject.GetPtrAt(prof->GetId()));
}


//------------------------------------------------------------------------------
RCursor<GSubject> GSession::GetSubjects(void) const
{
	LoadSubjects();
	return(Subjects.GetNodes());
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSession::GetSubjects(GDoc* doc) const
{
	LoadSubjects();
	const R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
	if(line)
		return(R::RCursor<GSubject>(*line));
	return(R::RCursor<GSubject>());
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSession::GetSubjects(const GSubject* subject) const
{
	LoadSubjects();
	return(Subjects.GetNodes(subject));
}


//------------------------------------------------------------------------------
RCursor<GSubject> GSession::GetTopSubjects(void) const
{
	LoadSubjects();
	return(Subjects.GetTopNodes());
}


//------------------------------------------------------------------------------
GTopic* GSession::GetTopic(size_t id,bool load,bool null)
{
	if(id==cNoRef)
	{
		if(null)
			return(0);
		ThrowGException("Unknown topic "+RString::Number(id));
	}
	GTopic* top=Topics[id];
	if(top)
		return(top);

	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown topic "+RString::Number(id));
	}
	top=Storage->LoadTopic(id);
	if(!top)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown topic "+RString::Number(id));
	}
	Insert(top);
	return(top);
}


//------------------------------------------------------------------------------
RCursor<GTopic> GSession::GetTopics(void)
{
	return(RCursor<GTopic>(Topics));
}


//------------------------------------------------------------------------------
double GSession::GetUpOperationsCost(const GSubject* u,const GSubject* v) const
{
	return(Subjects.GetUpOperationsCost(u,v));
}


//------------------------------------------------------------------------------
GUser* GSession::GetUser(size_t id,bool load,bool null)
{
	GUser* u;

	if(id>Users.GetMaxPos())
		return(0);
	u=Users[id];
	if(u)
		return(u);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown user "+RString::Number(id));
	}
	u=Storage->LoadUser(id);
	if(!u)
	{
		if(null)
			return(0);
		else
			throw GException("Unknown user "+RString::Number(id));
	}
	Insert(u);
	return(u);
}


//------------------------------------------------------------------------------
GUser* GSession::GetUser(const RString name,bool load,bool null)
{
	GUser* u;

	u=Users.GetPtr(name,false);
	if(u)
		return(u);
	if(!load)
		return(0);
	if(Storage->IsAllInMemory())
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown user "+name);
	}
	u=Storage->LoadUser(name);
	if(!u)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown user "+name);
	}
	Insert(u);
	return(u);
}


//------------------------------------------------------------------------------
R::RCursor<GUser> GSession::GetUsers(void) const
{
	return(R::RCursor<GUser>(Users));
}


//------------------------------------------------------------------------------
void GSession::GetXMLStruct(GDoc* doc,R::RXMLStruct* xml,bool& native,GSlot* rec)
{
	xml->Clear();
	R::RIO::RSmartTempFile TmpFile;
	RURI File;
	GFilter* Filter(GALILEIApp->FindMIMEType(doc,File,TmpFile));
	RXMLFile XML(File,xml);
	XML.SetInvalidXMLCodes(true);
	native=(Filter==0);
	if(Filter)
	{
		Filter->Clear(&XML);
		Filter->Analyze(doc,File,&XML,rec);
	}
	else
		XML.Open(RIO::Read);
}


//------------------------------------------------------------------------------
void GSession::GroupDocs(GSlot* rec)
{
	// Verify that there is a method to cluster the documents
	GGroupDocs* Grouping(GALILEIApp->GetCurrentPlugIn<GGroupDocs>("GroupDocs"));

	// How to compute the groups
	GTopicCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GTopicCalc>("TopicCalc"));

    // Group the documents
	Grouping->Grouping(rec,ClusterSelectedDocs);

	// If something to save to to compute -> Pass through the topics
	if(SaveResults||CalcDesc)
	{
		if(SaveResults)
			Storage->Clear(otTopic);

		// Compute the description of the groups and Save the information.
		R::RCursor<GTopic> Groups(GetTopics());
		for(Groups.Start();!Groups.End();Groups.Next())
		{
			if(CalcDesc)
			{
				CalcDesc->Compute(Groups());
				Groups()->Update(this,CalcDesc->Infos);
			}
			if(SaveResults)
			{
				if(CalcDesc&&CalcDesc->Infos.IsDefined())
					SaveInfos(CalcDesc->Infos,otTopic,Groups()->BlockId,Groups()->Id);
				Storage->SaveTopic(Groups());
				Groups()->SetState(osSaved);
			}
		}
	}
	FlushDesc(otTopic);   // Force to save all topics description
}


//------------------------------------------------------------------------------
void GSession::GroupProfiles(GSlot* rec)
{
	// Verify that there is a method to cluster the profiles
	GGroupProfiles* Grouping(GALILEIApp->GetCurrentPlugIn<GGroupProfiles>("GroupProfiles"));

	// How to compute the groups
	GCommunityCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GCommunityCalc>("CommunityCalc"));

    // Group the profiles
	Grouping->Grouping(rec);

	// If something to save to to compute -> Pass through the communities
	if(SaveResults||CalcDesc)
	{
		if(SaveResults)
			Storage->Clear(otCommunity);

		// Compute the description of the groups and Save the information.
		R::RCursor<GCommunity> Groups(GetCommunities());
		for(Groups.Start();!Groups.End();Groups.Next())
		{
			if(CalcDesc)
			{
				CalcDesc->Compute(Groups());
				Groups()->Update(this,CalcDesc->Infos);
			}
			if(SaveResults)
			{
				if(CalcDesc&&CalcDesc->Infos.IsDefined())
					SaveInfos(CalcDesc->Infos,otCommunity,Groups()->BlockId,Groups()->Id);
				Storage->SaveCommunity(Groups());
				Groups()->SetState(osSaved);
			}
		}
	}
	FlushDesc(otCommunity);   // Force to save all communities description
}


//------------------------------------------------------------------------------
void GSession::Init(void)
{
	// Create the configuration parameters
	GSimulator::CreateConfig(this);
	RCursor<RString> Type(TypesNames);
	for(Type.Start();!Type.End();Type.Next())
	{
		// Descriptions
		InsertParam(new RParamValue("BlockSize",1024),"Indexer",(*Type()),"Description");
		InsertParam(new RParamValue("Tolerance",10),"Indexer",(*Type()),"Description");
		InsertParam(new RParamValue("CacheSize",20),"Indexer",(*Type()),"Description");
		InsertParam(new RParamValue("Type",RBlockFile::WriteBack),"Indexer",(*Type()),"Description");

		// Inverted file
		InsertParam(new RParamValue("Increment",false),"Indexer",(*Type()),"Index");
		InsertParam(new RParamValue("BlockSize",1024),"Indexer",(*Type()),"Index");
		InsertParam(new RParamValue("Tolerance",10),"Indexer",(*Type()),"Index");
		InsertParam(new RParamValue("CacheSize",20),"Indexer",(*Type()),"Index");
		InsertParam(new RParamValue("Type",RBlockFile::WriteBack),"Indexer",(*Type()),"Index");
	}

	// Documents structures
	InsertParam(new RParamValue("BlockSize",4096),"Indexer","Documents","Structure");
	InsertParam(new RParamValue("Tolerance",40),"Indexer","Documents","Structure");
	InsertParam(new RParamValue("CacheSize",20),"Indexer","Documents","Structure");
	InsertParam(new RParamValue("Type",RBlockFile::WriteBack),"Indexer","Documents","Structure");

	// Create the configuration parameters for the plug-ins
	R::RCursor<GPlugInManager> Managers(GALILEIApp->GetManagers());
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->CreateConfig(this);

	// Load the configuration now
	Load(false);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig(this);
	ApplyConfig();
}


//------------------------------------------------------------------------------
void GSession::Insert(GClass* parent,GClass* tclass)
{
	// Insert it in the tree
	Classes.InsertNode(parent,tclass);

	// Look if data has an identifier. If not, assign one.
	if(tclass->GetId()==cNoRef)
		Storage->AssignId(tclass);
}


//------------------------------------------------------------------------------
void GSession::Insert(GCommunity* com)
{
	Communities.InsertPtr(com);
}


//------------------------------------------------------------------------------
GConcept* GSession::Insert(const GConcept* concept)
{
	GConceptType* Type;
	if((!concept)||(!(Type=concept->GetType())))
		ThrowGException("Cannot insert the concept");

	bool InDirect(false);

	// Invalid concept are not inserted
	if(concept->IsEmpty())
		ThrowGException("Empty concept cannot be inserted into a dictionary - id="+RString::Number(concept->GetId()));

	// Look if the data exists in the dictionary. If not, create and insert it.
	GConcept* ptr(Type->GetPtr(*concept));
	if(!ptr)
	{
		ptr=concept->DeepCopy();
		Type->InsertPtr(ptr);
		InDirect=true;
	}

	// Look if data has an identifier. If not, assign one.
	if(ptr->GetId()==cNoRef)
	{
		Storage->AssignId(ptr);
		InDirect=true;
	}

	// Finally, if an identifier has been assigned and/or a new one -> Direct
	if(InDirect)
		Concepts.InsertPtrAt(ptr,ptr->GetId(),true);

	return(ptr);
}


//-----------------------------------------------------------------------------
void GSession::Insert(GDoc* d)
{
	bool NewOne=false;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
	{
		AssignId(d);
		NewOne=true;
	}

	// Insert the document
	Docs.InsertPtrAt(d,d->GetId());

	// Insert the doc in the DocsRefUrl container.
	DocsRefUrl.InsertPtr(new GDocRefURL(d));

	if(NewOne)
	{
		// If new one and all documents are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveDoc(d);
		d->Emit(GEvent::eObjCreated);
	}
}


//------------------------------------------------------------------------------
void GSession::Insert(GDoc* doc,size_t subjectid,size_t usedid)
{
	LoadSubjects();
	GSubject* subject(Subjects.GetNode(subjectid));
	if(subject)
		subject->CategorizedDocs.InsertPtr(doc);
	GSubject* used(Subjects.GetNode(usedid));
	if(used)
	{
		R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
		if(!line)
			Subjects.DocsSubjects.InsertPtrAt(line=new R::RContainer<GSubject,false,false>(10,5),doc->GetId(),true);
		bool Find;
		size_t Index(line->GetIndex(*used,Find));
		if(!Find)
			line->InsertPtrAt(used,Index,false);
		Index=used->Docs.GetIndex(*doc,Find);
		if(!Find)
			used->Docs.InsertPtrAt(doc,Index,false);
		Index=Subjects.SelectedDocs.GetIndex(*doc,Find);
		if(!Find)
			Subjects.SelectedDocs.InsertPtrAt(doc,Index,false);
	}
}


//------------------------------------------------------------------------------
void GSession::Insert(GProfile* p)
{
	bool NewOne=false;

	if(p->GetId()==cNoRef)
	{
		AssignId(p);
		NewOne=true;
	}
	Profiles.InsertPtrAt(p,p->GetId());
	if(NewOne)
	{
		// If new one and all profiles are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveProfile(p);
		p->Emit(GEvent::eObjCreated);
	}
}


//------------------------------------------------------------------------------
void GSession::Insert(GProfile* profile,size_t subjectid)
{
	LoadSubjects();
	GSubject* subject(Subjects.Subjects.GetPtr(subjectid));
	if(!subject) return;
	Subjects.ProfilesSubject.InsertPtrAt(subject,profile->GetId(),true);
	subject->Profiles.InsertPtr(profile);
}


//------------------------------------------------------------------------------
void GSession::Insert(GSubject* to,GSubject* subject)
{
	LoadSubjects();
	Subjects.InsertNode(to,subject);
	Subjects.Subjects.InsertPtr(subject);
}


//------------------------------------------------------------------------------
void GSession::Insert(GTopic* top)
{
	Topics.InsertPtrAt(top,top->GetId());
}


//------------------------------------------------------------------------------
void GSession::Insert(GUser* user)
{
	bool NewOne=false;

	if(user->GetId()==cNoRef)
	{
		AssignId(user);
		NewOne=true;
	}
	Users.InsertPtrAt(user,user->GetId(),true);
	if(NewOne)
	{
		// If new one and all users are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveUser(user);
	}
}


//-----------------------------------------------------------------------------
void GSession::InsertConceptType(char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics,size_t refclasses)
{
	GConceptType* type=new GConceptType(this,id,name,desc,5000);
	if(!id)
		Storage->AssignId(type);

	type->SetReferences(refdocs,refprofiles,refgroups,reftopics,refclasses);
	ConceptTypes.InsertPtr(type);
	ConceptTypesByIds.InsertPtrAt(type,type->GetId(),true);
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate date,R::RDate computed,bool newone)
{
	GProfile* prof=GetProfile(profid,false);
	if(prof)
		prof->AddFdbk(docid,fdbk,date,computed);
	GDoc* doc=GetDoc(docid,false);
	if(doc)
		doc->InsertFdbk(profid);
	if(newone&&((!Storage->IsAllInMemory())||(SaveResults)))
		Storage->AddFdbk(profid,docid,fdbk,date,computed);
}


//-----------------------------------------------------------------------------
GPredicate* GSession::InsertPredicate(size_t id,const R::RString& name,const R::RString& desc)
{
	bool InDirect(false);

	GPredicate* ptr(Predicates.GetPtr(name));
	if(!ptr)
	{
		ptr=new GPredicate(id,name,desc);
		InDirect=true;
		Predicates.InsertPtr(ptr);
	}

	if(ptr->GetId()==cNoRef)
	{
		Storage->AssignId(ptr);
		InDirect=true;
	}

	if(InDirect)
		PredicatesByIds.InsertPtrAt(ptr,ptr->GetId(),true);

	return(ptr);
}


//-----------------------------------------------------------------------------
void GSession::InsertStatement(size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight)
{
	// Get the concept related to Xi
	GObject* Xi(GetObject(xitype,xi));
	if(!Xi)
		ThrowGException("Object "+RString::Number(xi)+" is not a "+GetObjType(xitype,false,false));

	// Get the concept related to the object
	GObject* Xj(GetObject(xjtype,xj));
	if(!Xj)
		ThrowGException("Object "+RString::Number(xj)+" is not a "+GetObjType(xjtype,false,false));

	// Find the predicate
	GPredicate* Predicate(PredicatesByIds[predicate]);
	if(!Predicate)
		ThrowGException("Predicate "+RString::Number(predicate)+" does not exist");

	// Insert the statement
	bool InDirect(true);
	GStatement* Statement(new GStatement(id,Predicate,Xi,Xj,weight));
	Predicate->InsertStatement(Statement);

	// Look if data has an identifier. If not, assign one.
	if(Statement->GetId()==cNoRef)
	{
		Storage->AssignId(Statement);
		InDirect=true;
	}

	if(InDirect)
		Statements.InsertPtrAt(Statement,Statement->GetId(),true);
}


//------------------------------------------------------------------------------
bool GSession::IsFromParentSubject(GDoc* doc,const GSubject* s) const
{
	LoadSubjects();

	// Verify that a parent exist and that it is not the root node
	if(!s->Parent)
		return(false);

	for(s=s->Parent;!s;s=s->Parent)
		if(s->Docs.IsIn(doc))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool GSession::IsFromSubject(GDoc* doc,const GSubject* s) const
{
	LoadSubjects();
	const R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(false);
	return(line->IsIn(*s));
}


//------------------------------------------------------------------------------
void GSession::LoadClasses(void)
{
	if(ClassesLoaded)
		return;
	Storage->LoadClasses();
	ClassesLoaded=true;
}


//------------------------------------------------------------------------------
void GSession::LoadCommunities(void)
{
	if(CommunitiesLoaded)
		return;
	Storage->LoadCommunities();
	CommunitiesLoaded=true;
}


//------------------------------------------------------------------------------
void GSession::LoadDocs(void)
{
	if(DocsLoaded)
		return;
	Storage->LoadDocs();
	DocsLoaded=true;
}


//------------------------------------------------------------------------------
void GSession::LoadInfos(GWeightInfos* &infos,tObjType type,size_t blockid,size_t id)
{
	try
	{
		IndexType* ptr(Types.GetPtr(type));
		if(!ptr)
			ThrowGException(GetObjType(type,true,true)+" do not have descriptions");

		// Position the block file to the correct position and read the size
		ptr->Desc->Seek(blockid,id);
		size_t size,concept;
		double weight;
		ptr->Desc->Read((char*)&size,sizeof(size_t));
		if(!infos)
			infos=new GWeightInfos(size);
		else
			infos->Clear(size);
		for(size_t i=0;i<size;i++)
		{
			ptr->Desc->Read((char*)&concept,sizeof(size_t));
			ptr->Desc->Read((char*)&weight,sizeof(double));
			infos->InsertPtrAt(new GWeightInfo(dynamic_cast<GSession*>(this)->GetConcept(concept),weight),i,false);
		}
	}
	catch(RIOException e)
	{
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GSession::LoadRefs(GConcept* concept,RNumContainer<size_t,true>& refs,tObjType type)
{
	IndexType* ptr(Types.GetPtr(type));
	if(!ptr)
		ThrowGException(GetObjType(type,true,true)+" do not have index");

	size_t Refs(concept->GetIndex(type));
	if(!Refs)
	{
		refs.Clear();
		return;
	}
	ptr->Index->Read(Refs,concept->Id,refs);
}


//------------------------------------------------------------------------------
void GSession::LoadStruct(GDocStruct* &docstruct,size_t blockid,size_t id)
{
	try
	{
		// Position the file to correct block
		StructDoc->Seek(blockid,id);

		// Read the number of records and number of local caches
		size_t nbrecs,nblcs;
		StructDoc->Seek(blockid,id);
		StructDoc->Read((char*)&nbrecs,sizeof(size_t));
		StructDoc->Read((char*)&nblcs,sizeof(size_t));
		if(!docstruct)
			docstruct=new GDocStruct(nbrecs,nblcs);
		else
			docstruct->SetSizes(nbrecs,nblcs);

		// Read the LCs
		for(size_t i=0;i<nblcs;i++)
		{
			size_t nb;
			StructDoc->Read((char*)&nb,sizeof(size_t));
			docstruct->SetNbLCEntries(i,nb);
		}

		// Read the records
		for(size_t i=0;i<nbrecs;i++)
		{
			size_t concept,pos,nbchild;
			char type,depth;
			StructDoc->Read((char*)&concept,sizeof(size_t));
			StructDoc->Read((char*)&type,sizeof(char));
			StructDoc->Read((char*)&pos,sizeof(size_t));
			StructDoc->Read((char*)&depth,sizeof(char));
			StructDoc->Read((char*)&nbchild,sizeof(size_t));
			docstruct->AddRecord(dynamic_cast<GSession*>(this)->GetConcept(concept),static_cast<GVTDRec::RecType>(type),pos,depth,nbchild);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GSession::LoadSubjects(void) const
{
	if(Subjects.SubjectsLoaded)
		return;
	const_cast<GSession*>(this)->Subjects.SubjectsLoaded=true;
	const_cast<GSession*>(this)->Storage->LoadSubjects();
}


//------------------------------------------------------------------------------
void GSession::LoadTopics(void)
{
	if(TopicsLoaded)
		return;
	Storage->LoadTopics();
	TopicsLoaded=true;
}


//------------------------------------------------------------------------------
void GSession::LoadUsers(void)
{
	if(UsersLoaded)
		return;
	Storage->LoadUsers();
	UsersLoaded=true;
}


//------------------------------------------------------------------------------
void GSession::QueryMetaEngine(RContainer<RString,true,false> &keyWords)
{
	// Verify that a meta engine is selected
	GMetaEngine* metaEngine(GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine"));
	metaEngine->Query(keyWords,true); //true ->Use all keywords passed to the meta engine
	metaEngine->Process();
}


//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	// Clear feedbacks
	Clear(otFdbk);

	Clear(otCommunity,otReference);
	Clear(otProfile,otReference);

	// Clear groups, profiles and users
	Communities.Clear();
	Profiles.Clear();
	Users.Clear();

	// Re-initialize the subjects
	// Re-initialize the subjects
	LoadSubjects();
	RCursor<GSubject> Subject(Subjects.GetNodes());
	for(Subject.Start();!Subject.End();Subject.Next())
		Subject()->ReInit();

	// Clear the documents and profiles assignment
	Subjects.ProfilesSubject.Clear();
	Subjects.DocsSubjects.Clear();
	Subjects.SelectedDocs.Clear();
}


//------------------------------------------------------------------------------
GConcept* GSession::RenameConcept(GConcept* concept,const R::RString& name)
{
	if((!concept)||(!concept->GetType()))
		ThrowGException("Cannot rename concept");

	// Look if the new name is not  already in the dictionary
	GConcept* ptr=concept->GetType()->GetPtr(name);
	if(ptr==concept)
		return(concept);
	if(ptr)
	{
		// Both concept must be merge and the old one deleted
		ptr->NbRefDocs+=concept->NbRefDocs;
		ptr->NbRefProfiles+=concept->NbRefProfiles;
		ptr->NbRefCommunities+=concept->NbRefCommunities;
		ptr->NbRefTopics+=concept->NbRefTopics;
		Delete(concept);
		return(ptr);
	}
	else
	{
		// Rename really the concept
		concept->GetType()->DeletePtr(*concept);
		concept->Name=name;
		concept->GetType()->InsertPtr(concept);
		Storage->SaveConcept(concept);
		return(concept);
	}
}


//------------------------------------------------------------------------------
void GSession::ResetBreak(void)
{
	ExternBreak=false;
}


//------------------------------------------------------------------------------
void GSession::RunTool(const R::RString& name,GSlot* slot,bool need)
{
	GTool* Tool(GALILEIApp->GetPlugIn<GTool>("Tool",name));
	if((!Tool)&&need)
		ThrowGException("Tool '"+name+"' does not exist");
	Tool->Run(slot);
}


//------------------------------------------------------------------------------
void GSession::SaveInfos(const GWeightInfos& infos,tObjType type,size_t& blockid,size_t id)
{
	try
	{
		size_t size(infos.GetNb()),concept;
		double weight;
		if(!size)
			return;

		IndexType* ptr(Types.GetPtr(type));
		if(!ptr)
			ThrowGException(GetObjType(type,true,true)+" do not have descriptions");

		// Position the block file to the correct position and write the size
		ptr->Desc->Seek(blockid,id,sizeof(size_t)+size*SizeRecDesc);
		ptr->Desc->Write((char*)&size,sizeof(size_t));
		RCursor<GWeightInfo> Cur(infos);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			concept=Cur()->GetId();
			weight=Cur()->GetWeight();
			ptr->Desc->Write((char*)&concept,sizeof(size_t));
			ptr->Desc->Write((char*)&weight,sizeof(double));
		}
	}
	catch(RIOException e)
	{
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GSession::SaveStruct(GDocStruct* docstruct,size_t& blockid,size_t id)
{
	try
	{
	    if(!docstruct->GetNbRecs())
			return;

		// Position the file to correct block and announce that a given number of bytes will be written
	    size_t nbrecs(docstruct->GetNbRecs()),nblcs(docstruct->GetNbLCs());
	    size_t size(SizeT2+(nblcs*sizeof(size_t))+(nbrecs*SizeRecStruct));
		StructDoc->Seek(blockid,id,size);

		// Save the size of the number of records and the number of LCs
		StructDoc->Write((char*)&nbrecs,sizeof(size_t));
		StructDoc->Write((char*)&nblcs,sizeof(size_t));

		// Save LCs
		for(size_t i=0;i<nblcs;i++)
		{
			size_t nb(docstruct->GetNbLCEntries(i));
			StructDoc->Write((char*)&nb,sizeof(size_t));
		}

		// Save the records
		R::RCursor<GVTDRec> Recs(docstruct->GetRecs());
		for(Recs.Start();!Recs.End();Recs.Next())
		{
			size_t nb;
			char car;
			nb=Recs()->GetConcept()->GetId();
			StructDoc->Write((char*)&nb,sizeof(size_t));
			car=static_cast<char>(Recs()->GetType());
			StructDoc->Write((char*)&car,sizeof(char));
			nb=Recs()->GetPos();
			StructDoc->Write((char*)&nb,sizeof(size_t));
			car=Recs()->GetDepth();
			StructDoc->Write((char*)&car,sizeof(char));
			nb=docstruct->GetFirstChild(Recs());
			StructDoc->Write((char*)&nb,sizeof(size_t));
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GSession::SetBreak(void)
{
	ExternBreak=true;
}


//------------------------------------------------------------------------------
void GSession::SetCurrentRandom(int rand)
{
	CurrentRandom=rand;
	Random->Reset(CurrentRandom);
}


//------------------------------------------------------------------------------
void GSession::SetDescType(tSubjectDesc type)
{
	if(type!=Subjects.DescType)
	{
		// All descriptions of the subjects must be deleted.
		RCursor<GSubject> Cur(GetSubjects());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->Vector)
			{
				delete Cur()->Vector;
				Cur()->Vector=0;
			}
		}
	}
	Subjects.DescType=type;
}


//------------------------------------------------------------------------------
struct NewGenericSubject
{
	GSubject* Subject;
	GSubject* Parent;

	NewGenericSubject(GSubject* subject,GSubject* parent) : Subject(subject), Parent(parent) {}
	int Compare(const NewGenericSubject&) const {return(-1);}
};
void GSession::TestSubjects(void)
{
	RContainer<GSubject,false,false> ToDel(500); // Subjects to delete
	RContainer<NewGenericSubject,true,false> ToIns(500);   // Subjects to insert

	size_t NbNoLeaf(0);
	size_t NbLeaf(0);
	RCursor<GSubject> Cur(GetSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GSubject* Subject(Cur());
		if(Subject->GetNbSubjects()&&Subject->Docs.GetNb())
		{
			// If no child subjects have documents or children -> it is OK.
			bool OK(true);
			RCursor<GSubject> Cur2(Subject->GetSubjects());
			for(Cur2.Start();(!Cur2.End())&&OK;Cur2.Next())
				if(Cur2()->GetNbSubjects()||Cur2()->CategorizedDocs.GetNb())
					OK=false;
			if(!OK)
			{
				NbNoLeaf++;

				// Create a new subject
				GSubject* NewSubject(new GSubject(this,Subjects.GetNb()+NbNoLeaf,Subject->Name+" general",true));
				ToIns.InsertPtr(new NewGenericSubject(NewSubject,Cur()));

				// Transfer all the document from Cur() to subject
				RCursor<GDoc> Docs(Subject->CategorizedDocs);
				for(Docs.Start();!Docs.End();Docs.Next())
					NewSubject->CategorizedDocs.InsertPtr(Docs());
				Docs.Set(Subject->Docs);
				for(Docs.Start();!Docs.End();Docs.Next())
					NewSubject->Docs.InsertPtr(Docs());
				Subject->CategorizedDocs.Clear();
				Subject->Docs.Clear();
			}
		}
		else if(!Subject->GetNbSubjects()&&!Subject->CategorizedDocs.GetNb())
		{
			ToDel.InsertPtr(Subject);
			NbLeaf++;
		}
	}

	// Delete the nodes
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		Subjects.DeleteNode(Cur(),true);

	// Insert the nodes
	RCursor<NewGenericSubject> New(ToIns);
	for(New.Start();!New.End();New.Next())
		Insert(New()->Parent,New()->Subject);

	// Save if necessary
	if(MustSaveResults())
	{
		Storage->Clear(otSubject);
		Cur=GetSubjects();
		for(Cur.Start();!Cur.End();Cur.Next())
			Storage->SaveSubject(Cur());
	}

	cout<<"There are "<<GetNbObjects(otSubject)<<" subjects:"<<endl;
	if(NbNoLeaf)
		cout<<"  "<<NbNoLeaf<<" non-leaf subjects have some documents attached"<<endl;
	if(NbLeaf)
		cout<<"  "<<NbLeaf<<" leaf subjects have no documents attached"<<endl;
	cout<<"  "<<static_cast<double>(NbNoLeaf+NbLeaf)*100.0/static_cast<double>(GetNbObjects(otSubject))<<"% of invalid subjects"<<endl;
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
void GSession::UpdateProfiles(size_t docid)
{
	// If there are some profile -> propagate in memory
	GDoc* doc=GetDoc(docid);
	if(doc)
	{
		RNumContainer<size_t,true>* fdbks=doc->GetFdbks();
		if(fdbks)
		{
			RNumCursor<size_t> Cur(*fdbks);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GProfile* prof=GetProfile(Cur());
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
void GSession::UpdateRefs(const GWeightInfos& infos,tObjType type,size_t id,bool add)
{
	IndexType* ptr(Types.GetPtr(type));
	if(!ptr)
		ThrowGException(GetObjType(type,true,true)+" do not have index");
	if(!ptr->IndexInc)
		return;

	// Update the index for all concepts
	RCursor<GWeightInfo> Cur(infos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Read the vector representing the current index
		GConcept* Concept(Cur()->Concept);
		if(Concept->GetIndex(type))
			ptr->Index->Read(Concept->GetIndex(type),Concept->Id,tmpRefs);
		else
			tmpRefs.Clear();

		// Add or remove the document from the index
		size_t oldsize(tmpRefs.GetNb());
		if(add)
			tmpRefs.Insert(id);
		else
			tmpRefs.Delete(id);

		// If the size of vector has changed -> Save it back
		if(oldsize!=tmpRefs.GetNb())
		{
			size_t idx(Concept->GetIndex(type));
			ptr->Index->Write(idx,Concept->Id,tmpRefs);
			Concept->SetIndex(type,idx);
		}
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
GSession::~GSession(void)
{
	// Save the configuration
	RCursor<GPlugInManager> Cur(GALILEIApp->GetManagers());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->SaveConfig(this);
	Save();

	delete Random;
	delete Simulator;
	delete StructDoc;
	Storage->Session=0;
}
