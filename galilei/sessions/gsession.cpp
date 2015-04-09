/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <rnodecursor.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <gdoc.h>
#include <gconcepttree.h>
#include <glinkcalc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gdocfragment.h>
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
#include <ggalileiapp.h>
#include <gsimulator.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
#include <gtool.h>
#include <gpredicate.h>
#include <gstatement.h>
#include <gclass.h>
#include <gsearchquery.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
// define
#define HANDLEALLEXCEPTIONS(rec,msg) 	                                        \
catch(GException& e)                                                           \
{                                                                              \
	if(rec)                                                                     \
	{                                                                           \
		cerr<<msg<<e.GetMsg()<<endl;                                             \
		rec->WriteStr(msg+e.GetMsg());                                           \
	}                                                                           \
	else                                                                        \
		throw GException(msg+e.GetMsg());                                        \
}                                                                              \
catch(RIOException& e)                                                         \
{                                                                              \
	if(rec)                                                                     \
	{                                                                           \
		cerr<<e.GetMsg()<<endl;                                                  \
		rec->WriteStr(e.GetMsg());                                               \
	}                                                                           \
	else                                                                        \
		throw GException(e.GetMsg());                                            \
}                                                                              \
catch(RException& e)                                                           \
{                                                                              \
	if(rec)                                                                     \
	{                                                                           \
		cerr<<msg<<e.GetMsg()<<endl;                                             \
		rec->WriteStr(msg+e.GetMsg());                                           \
	}                                                                           \
	else                                                                        \
		throw GException(msg+e.GetMsg());                                        \
}                                                                              \
catch(exception& e)                                                            \
{                                                                              \
	if(rec)                                                                     \
	{                                                                           \
		cerr<<msg<<e.what()<<endl;                                               \
		rec->WriteStr(msg+e.what());                                             \
	}                                                                           \
	else                                                                        \
		throw GException(msg+e.what());                                          \
}                                                                              \
catch(...)                                                                     \
{                                                                              \
	if(rec)                                                                     \
		rec->WriteStr(msg+"Undefined error");                                    \
	else                                                                        \
		throw GException(msg+"Undefined error");                                 \
}



//------------------------------------------------------------------------------
/*const size_t SizeRecDesc=sizeof(size_t)+sizeof(double);
const size_t SizeRecStruct=sizeof(size_t)+sizeof(char)+sizeof(size_t)+sizeof(char)+sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);*/



//------------------------------------------------------------------------------
//
// GSessionMsg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSessionMsg::GSessionMsg(GSession* session,tObjType type)
	: Session(session), Type(type), DeleteObjs(false)
{

}


//------------------------------------------------------------------------------
GSessionMsg::GSessionMsg(GSession* session,tObjType type,bool del)
	: Session(session), Type(type), DeleteObjs(del)
{

}



//------------------------------------------------------------------------------
//
// GSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSession::GSession(size_t id,const RString& name)
	:
	  RObject(name),
	  RConfig("lib/galilei/sessions",name),
	  GObjects<GDoc,hDocs>(20000,"Documents",otDoc),
	  GObjects<GTopic,hTopics>(200,"Topics",otTopic),
	  GObjects<GUser,hUsers>(1000,"Users",otUser),
	  GObjects<GProfile,hProfiles>(5000,"Profiles",otProfile),
	  GObjects<GCommunity,hCommunities>(100,"Communities",otCommunity),
	  GClasses(300),
	  Id(id), Name(name),
	  ValidConfigFile(false), Log("/var/log/galilei/"+name+".log"),
	  CurrentRandom(1), Random(RRandom::Good,1),
	  ExternBreak(false),
	  Simulator(0), DocAnalyze(this)

{
	// Log files
	Log.Open(RIO::Append);

	//reinterpret_cast<RString*>(-1)
}


//------------------------------------------------------------------------------
void GSession::HandlerNotFound(const R::RNotification& /*notification*/)
{
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
bool GSession::MustBreak(void)
{
	return(ExternBreak);
}



//------------------------------------------------------------------------------
void GSession::ResetBreak(void)
{
	ExternBreak=false;
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
	Random.Reset(CurrentRandom);
}


//------------------------------------------------------------------------------
void GSession::Init(void)
{
	// Create the configuration parameters
	GSimulator::CreateConfig(this);
	GObjects<GDoc,hDocs>::Init(this);
	GObjects<GTopic,hTopics>::Init(this);
	GObjects<GUser,hUsers>::Init(this);
	GObjects<GProfile,hProfiles>::Init(this);
	GObjects<GCommunity,hCommunities>::Init(this);
	GClasses::Init(this);

	// Create the configuration parameters for the plug-ins
	R::RCursor<GPlugInManager> Managers(GALILEIApp->GetManagers());
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->CreateConfig(this);

	// Load the configuration now
	Load(false,true);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig(this);
	ApplyConfig();

	// This is a valid configuration file
	ValidConfigFile=true;
}


//------------------------------------------------------------------------------
void GSession::EmitReInit(tObjType type,bool del)
{
	GSessionMsg Msg(this,type,del);
	PostNotification<GSessionMsg&>(hReInit,Msg);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GSubject*,bool cleardocs)
{

	// Clear the users
	Clear(pUser,true);

	//Clear the topics and classes
	Clear(pTopic,true);
	EmitReInit(otTopic,true);
	Clear(pClass,true);
	EmitReInit(otClass,true);

	//	 Verify if the document descriptions must be recomputed
	if(cleardocs)
	{
		Clear(pDoc,false);
		EmitReInit(otDoc,false);
	}

	// Re-initialize the subjects
	GSubjects::ReInit();
	EmitReInit(otSubject,false);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GDoc*,bool del)
{
	// Treat the feedbacks
	if(del)
	{
		// Delete the feedbacks from the profiles
		RCursor<GProfile> Profiles(GetObjs(pProfile));
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Profiles()->ClearFdbks();
		if(SaveResults)
			Storage->Clear(otFdbk);
		EmitReInit(otFdbk,true);
	}

	// Clear the other dependencies
	Clear(pTopic,true);
	EmitReInit(otTopic,true);
	Clear(pClass,true);
	EmitReInit(otClass,true);

	// Clear the documents
	Clear(pDoc,del);
	EmitReInit(otDoc,del);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GUser*)
{
	// Delete the feedbacks from the documents
	RCursor<GDoc> Docs(GetObjs(pDoc));
	for(Docs.Start();!Docs.End();Docs.Next())
		Docs()->ClearFdbks();
	if(SaveResults)
		Storage->Clear(otFdbk);
	EmitReInit(otFdbk,true);

	// Clear the other dependencies
	Clear(pCommunity,true);
	EmitReInit(otCommunity,true);
	Clear(pProfile,true);
	EmitReInit(otProfile,true);

	// Clear the users
	Clear(pUser,true);
	EmitReInit(otUser,true);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GProfile*,bool del)
{
	// Treat the feedbacks
	if(del)
	{
		// Delete the feedbacks from the documents
		RCursor<GDoc> Docs(GetObjs(pDoc));
		for(Docs.Start();!Docs.End();Docs.Next())
			Docs()->ClearFdbks();
		if(SaveResults)
			Storage->Clear(otFdbk);
		EmitReInit(otFdbk,true);
	}

	// Clear the other dependencies
	Clear(pCommunity,true);
	EmitReInit(otCommunity,true);

	// Clear the profiles
	Clear(pProfile,del);
	EmitReInit(otProfile,del);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GFdbk*)
{
	RCursor<GDoc> Docs(GetObjs(pDoc));
	for(Docs.Start();!Docs.End();Docs.Next())
		Docs()->ClearFdbks();
	RCursor<GProfile> Profiles(GetObjs(pProfile));
	for(Profiles.Start();!Profiles.End();Profiles.Next())
		Profiles()->ClearFdbks();
	if(SaveResults)
		Storage->Clear(otFdbk);
	EmitReInit(otFdbk,true);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GTopic*)
{
	Clear(pTopic,true);
	EmitReInit(otTopic,true);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GClass*)
{
	Clear(pClass,true);
	EmitReInit(otClass,true);
}


//------------------------------------------------------------------------------
void GSession::ReInit(const GCommunity*)
{
	Clear(pCommunity,true);
	EmitReInit(otCommunity,true);
}


//------------------------------------------------------------------------------
void GSession::AnalyzeDoc(GDoc* doc,GSlot* rec,bool force)
{
	if(rec)
	{
		rec->Interact();
		rec->NextDoc(doc);
	}
	if(ExternBreak) return;

	DocAnalyze.Analyze(doc,force);
}


//------------------------------------------------------------------------------
void GSession::AnalyzeDocs(GSlot* rec)
{
	// Opens and appends the Log File for all errors occurring in the filter or analyze phase.
	if(rec)
	{
		RString err("Documents Filtering and Analysis on Data Set : "+Storage->GetFactory()->GetName()+ " on : " +RString::Number(RDate::GetToday().GetDay())+"/"+ RString::Number(RDate::GetToday().GetMonth())+"/"+RString::Number(RDate::GetToday().GetYear()));
		rec->WriteStr("Analyze documents");
	}

	// Assign the current plug-ins
	DocAnalyze.AssignPlugIns();

	// Analyze the documents - Go through the existing documents
	R::RCursor<GDoc> Docs(GetObjs(pDoc));
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		try
		{
			AnalyzeDoc(Docs(),rec);
		}
		// If a log file specified -> write to it and it is OK
		// If no log file specified -> Propagate error
		HANDLEALLEXCEPTIONS(rec,Docs()->GetURI()()+"("+RString::Number(Docs()->GetId())+"): ")
	}

	 // Force to save all document descriptions and structures
	FlushDesc(pDoc);
	FlushTree(pDoc);
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
	GObjects<GDoc,hDocs>::OpenFiles(this,Name);
	GObjects<GTopic,hTopics>::OpenFiles(this,Name);
	GObjects<GUser,hUsers>::OpenFiles(this,Name);
	GObjects<GProfile,hProfiles>::OpenFiles(this,Name);
	GObjects<GCommunity,hCommunities>::OpenFiles(this,Name);
	GClasses::OpenFiles(this,Name);
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
	method->Compute(profile);
	profile->Update(method->Description);
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec)
{
	GProfileCalc* Profiling(GALILEIApp->GetCurrentPlugIn<GProfileCalc>("ProfileCalc"));
	GLinkCalc* LinkCalc(GALILEIApp->GetCurrentPlugIn<GLinkCalc>("LinkCalc",RString::Null,false));

	R::RCursor<GProfile> Prof(GetObjs(pProfile));
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
	FlushDesc(pProfile);   // Force to save all profiles description
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
size_t GSession::GetNbObjs(tObjType type) const
{
	switch(type)
	{
		case otUser:
			return(GObjects<GUser,hUsers>::Objects.GetNb());
		case otConcept:
			return(Concepts.GetNb());
		case otConceptType:
			return(ConceptTypes.GetNb());
		case otDoc:
			return(GObjects<GDoc,hDocs>::Objects.GetNb());
		case otProfile:
			return(GObjects<GProfile,hProfiles>::Objects.GetNb());
		case otCommunity:
			return(GObjects<GCommunity,hCommunities>::Objects.GetNb());
		case otTopic:
			return(GObjects<GTopic,hTopics>::Objects.GetNb());
		case otClass:
			return(GetNbObjs(pClass));
		case otSubject:
			return(GetNbObjs(pSubject));
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//------------------------------------------------------------------------------
size_t GSession::GetMaxObjId(tObjType type) const
{
	switch(type)
	{
		case otConcept:
			if(!Concepts.GetNb())
				return(0);
			return(Concepts[Concepts.GetMaxPos()]->GetId());
		case otDoc:
			if(!GObjects<GDoc,hDocs>::Objects.GetNb())
				return(0);
			return(GObjects<GDoc,hDocs>::Objects[GObjects<GDoc,hDocs>::Objects.GetMaxPos()]->GetId());
		case otProfile:
			if(!GObjects<GProfile,hProfiles>::Objects.GetNb())
				return(0);
			return(GObjects<GProfile,hProfiles>::Objects[GObjects<GProfile,hProfiles>::Objects.GetMaxPos()]->GetId());
		case otCommunity:
			if(!GObjects<GCommunity,hCommunities>::Objects.GetNb())
				return(0);
			return(GObjects<GCommunity,hCommunities>::Objects[GObjects<GCommunity,hCommunities>::Objects.GetMaxPos()]->GetId());
		case otTopic:
			if(!GObjects<GTopic,hTopics>::Objects.GetNb())
				return(0);
			return(GObjects<GTopic,hTopics>::Objects[GObjects<GTopic,hTopics>::Objects.GetMaxPos()]->GetId());
		case otClass:
			 return(GetMaxObjId(pClass));
		case otSubject:
			 const_cast<GSession*>(this)->LoadObjs(pSubject);
			 if(!Subjects.GetNb())
				  return(0);
			 return(Subjects[Subjects.GetMaxPos()]->GetId());
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxObjPos(tObjType type) const
{
	switch(type)
	{
		case otConcept:
			return(Concepts.GetMaxPos());
		case otDoc:
			return(GetMaxObjPos(pDoc));
		case otTopic:
			return(GetMaxObjPos(pTopic));
		case otClass:
			return(GetMaxObjPos(pClass));
		case otProfile:
			return(GetMaxObjPos(pProfile));
		case otUser:
			return(GetMaxObjPos(pUser));
		case otCommunity:
			return(GetMaxObjPos(pCommunity));
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
GObject* GSession::GetObj(tObjType type,size_t id,bool null)
{
	switch(type)
	{
		case otConcept:
			return(Concepts[id]);
		case otConceptType:
			return(GetObj(pConceptType,id,null));
		case otDoc:
			return(GetObj(pDoc,id,null));
		case otUser:
			return(GetObj(pUser,id,null));
		case otProfile:
			return(GetObj(pProfile,id,null));
		case otCommunity:
			return(GetObj(pCommunity,id,null));
		case otTopic:
			return(GetObj(pTopic,id,null));
		case otClass:
			return(GetObj(pClass,id,null));
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//------------------------------------------------------------------------------
size_t GSession::GetObjs(tObjType type,GObject** &tab,bool alloc)
{
	switch(type)
	{
		case otDoc:
			if(alloc)
				tab=new GObject*[GObjects<GDoc,hDocs>::Objects.GetMaxPos()+1];
			return(GObjects<GDoc,hDocs>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otProfile:
			if(alloc)
				tab=new GObject*[GObjects<GProfile,hProfiles>::Objects.GetMaxPos()+1];
			return(GObjects<GProfile,hProfiles>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otCommunity:
			if(alloc)
				tab=new GObject*[GObjects<GCommunity,hCommunities>::Objects.GetMaxPos()+1];
			return(GObjects<GCommunity,hCommunities>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otTopic:
			if(alloc)
				tab=new GObject*[GObjects<GTopic,hTopics>::Objects.GetMaxPos()+1];
			return(GObjects<GTopic,hTopics>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//------------------------------------------------------------------------------
GSimulator* GSession::GetSimulator(void) const
{
	if(!Simulator)
	{
		const_cast<GSession*>(this)->Simulator=new GSimulator(const_cast<GSession*>(this));
		const_cast<GSession*>(this)->Simulator->Session=const_cast<GSession*>(this);
		Simulator->ApplyParams();
	}
	return(Simulator);
}


//------------------------------------------------------------------------------
void GSession::GroupDocs(GSlot* rec)
{
	// Verify that there is a method to cluster the documents
	GGroupDocs* Grouping(GALILEIApp->GetCurrentPlugIn<GGroupDocs>("GroupDocs"));

	// How to compute the topic descriptions
	GTopicCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GTopicCalc>("TopicCalc"));
	if(!CalcDesc)
		mThrowGException("No current method to compute topic descriptions");

    // Group the documents
	Grouping->Grouping(rec,ClusterSelectedDocs);

	// Update the topics
	R::RCursor<GTopic> Topic(GetObjs(pTopic));
	for(Topic.Start();!Topic.End();Topic.Next())
	{
		// Compute the topic description
		CalcDesc->Compute(Topic());
		Topic()->Update(CalcDesc->Description);
	}
	FlushDesc(pTopic);   // Force to save all topics description
}


//------------------------------------------------------------------------------
void GSession::GroupProfiles(GSlot* rec)
{
	// Verify that there is a method to cluster the profiles
	GGroupProfiles* Grouping(GALILEIApp->GetCurrentPlugIn<GGroupProfiles>("GroupProfiles"));

	// How to compute the community descriptions
	GCommunityCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GCommunityCalc>("CommunityCalc"));

    // Group the profiles
	Grouping->Grouping(rec);

	// Update the communities
	R::RCursor<GCommunity> Community(GetObjs(pCommunity));
	for(Community.Start();!Community.End();Community.Next())
	{
		// Compute the community description
		CalcDesc->Compute(Community());
		Community()->Update(CalcDesc->Description);
	}
	FlushDesc(pCommunity);   // Force to save all communities description
}


//------------------------------------------------------------------------------
bool GSession::InsertFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate done,bool load)
{
	if(fdbk==ftUnknown)
		mThrowGException("Cannot add an unknown feedback to profile +'"+RString::Number(Id)+"' for document '"+RString::Number(docid)+"'");

	bool NewFdbk(false);
	GProfile* prof(GetObj(pProfile,profid,false));
	if(prof)
		NewFdbk=prof->InsertFdbk(docid,fdbk,done);
	GDoc* doc(GetObj(pDoc,docid,false));
	if(doc)
		NewFdbk=doc->InsertFdbk(profid);

	if((!load)&&((!Storage->IsAllInMemory())||(SaveResults)))
		Storage->UpdateFdbk(profid,docid,fdbk,done);

	return((!load)&&NewFdbk);
}


//------------------------------------------------------------------------------
void GSession::RequestMetaEngine(const R::RString query)
{
	GMetaEngine* MetaEngine(GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine"));
	if(!MetaEngine)
		mThrowGException("No meta-engine selected");
	GSearchQuery* Query(MetaEngine->BuildQuery(query));
	MetaEngine->Request(Query);
	delete Query;
}


//------------------------------------------------------------------------------
void GSession::RunTool(const RString& name,const RString& list,GSlot* slot,bool need)
{
	GTool* Tool(GALILEIApp->GetPlugIn<GTool>("Tools",name,list));
	if((!Tool)&&need)
		mThrowGException("Tool '"+name+"' does not exist in '"+list+"'");
	Tool->Run(slot);
}


//------------------------------------------------------------------------------
void GSession::UpdateCommunity(GProfile* prof)
{
	if(prof&&(prof->GetGroupId()!=cNoRef)&&(prof->GetGroupId()))
	{
		GCommunity* grp(GetObj(pCommunity,prof->GetGroupId(),false));
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
	GProfile* prof(GetObj(pProfile,profid,false,false));
	UpdateCommunity(prof);
}


//------------------------------------------------------------------------------
void GSession::SetName(GUser* user,const RString& name,const RString& fullname)
{
	user->Name=name;
	if(fullname.IsEmpty())
		user->FullName=name;
	else
		user->FullName=fullname;
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->SaveObj(user);
}


//-----------------------------------------------------------------------------
void GSession::SetName(GProfile* prof,const RString& name)
{
	prof->Name=name;
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->SaveObj(prof);
}


//------------------------------------------------------------------------------
void GSession::SetSocial(GProfile* prof,bool social)
{
	prof->Social=social;
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->SaveObj(prof);
}


//------------------------------------------------------------------------------
void GSession::SetConfidence(GProfile* prof,double score,char level)
{
	prof->Score=score;
	prof->Level=level;
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->SaveObj(prof);
}


//------------------------------------------------------------------------------
void GSession::UpdateProfiles(size_t docid)
{
	// If there are some profile -> propagate in memory
	GDoc* doc(GetObj(pDoc,docid));
	if(doc)
	{
		RNumCursor<size_t> Cur(doc->GetFdbks());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			GProfile* prof=GetObj(pProfile,Cur());
			if(!prof)
				continue;
			prof->WasUpdated(docid);
		}
	}

	// Use database
	if((!Storage->IsAllInMemory())||(SaveResults))
		Storage->UpdateProfiles(docid);
}


//------------------------------------------------------------------------------
void GSession::UpdateTopic(GDoc* doc)
{
	if(doc&&(doc->GetGroupId()!=cNoRef))
	{
		GTopic* top(GetObj(pTopic,doc->GetGroupId(),false));
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
	GDoc* doc(GetObj(pDoc,docid,false,false));
	UpdateTopic(doc);
}


//------------------------------------------------------------------------------
bool GSession::IsDefined(const RContainer<GVector,true,true>& vectors)
{
	RCursor<GVector> Vector(vectors);
	for(Vector.Start();!Vector.End();Vector.Next())
		if(Vector()->IsDefined())
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
GSession::~GSession(void)
{
	// Save the configuration if there is a valid file
	if(ValidConfigFile)
	{
		RCursor<GPlugInManager> Cur(GALILEIApp->GetManagers());
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->SaveConfig(this);
		Save();
	}

	delete Simulator;
	if(Storage)
		Storage->Session=0;
}
