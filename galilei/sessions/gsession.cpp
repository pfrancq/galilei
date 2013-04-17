/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
const size_t SizeRecDesc=sizeof(size_t)+sizeof(double);
const size_t SizeRecStruct=sizeof(size_t)+sizeof(char)+sizeof(size_t)+sizeof(char)+sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);



//------------------------------------------------------------------------------
//
// GSessionMsg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSessionMsg::GSessionMsg(GSession* session,tObjType type)
	: Session(session), Type(type), Meta(otSession)
{

}


//------------------------------------------------------------------------------
GSessionMsg::GSessionMsg(GSession* session,tObjType type,tObjType meta)
	: Session(session), Type(type), Meta(meta)
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
	  GObjects<GDoc,eCreateDoc>(20000,"Documents",otDoc),
	  GObjects<GTopic,eCreateTopic>(200,"Topics",otTopic),
	  GObjects<GUser,eCreateUser>(1000,"Users",otUser),
	  GObjects<GProfile,eCreateProfile>(5000,"Profiles",otProfile),
	  GObjects<GCommunity,eCreateCommunity>(100,"Communities",otCommunity),
	  GClasses(300),
	  Id(id), Name(name),
	  ValidConfigFile(false), Log("/var/log/galilei/"+name+".log"),
	  CurrentRandom(1), Random(RRandom::Good,1),
	  ExternBreak(false),
	  Simulator(0), Subjects(200), DocAnalyze(this)

{
	// Log files
	Log.Open(RIO::Append);
}


//------------------------------------------------------------------------------
void GSession::HandlerNotFound(const R::RNotification& /*notification*/)
{
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
void GSession::AnalyzeDoc(GDoc* doc,bool ram,GSlot* rec)
{
	if(rec)
	{
		rec->Interact();
		rec->NextDoc(doc);
	}
	if(ExternBreak) return;

	DocAnalyze.Analyze(doc,ram);
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

	// Assign the current plug-ins
	DocAnalyze.AssignPlugIns();

	// Analyze the documents - Go through the existing documents
	R::RCursor<GDoc> Docs(GetObjs(pDoc));
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		try
		{
			AnalyzeDoc(Docs(),ram,rec);
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
	GObjects<GDoc,eCreateDoc>::OpenFiles(this,Name);
	GObjects<GTopic,eCreateTopic>::OpenFiles(this,Name);
	GObjects<GUser,eCreateUser>::OpenFiles(this,Name);
	GObjects<GProfile,eCreateProfile>::OpenFiles(this,Name);
	GObjects<GCommunity,eCreateCommunity>::OpenFiles(this,Name);
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
	bool Save(SaveResults&&(profile->GetId()!=cNoRef));
	bool DelRefs(profile->IsDefined());
	method->Compute(profile);
	profile->Update(this,method->Description,DelRefs);
	if(Save)
	{
		if(profile->IsDefined())
			profile->SaveDesc();
		Storage->SaveObj(profile);
		profile->SetState(osSaved);
	}
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
int GSession::Compare(const GSession& session) const
{
	return(CompareIds(Id,session.Id));
}


//------------------------------------------------------------------------------
int GSession::Compare(const GSession* session) const
{
	return(CompareIds(Id,session->Id));
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
void GSession::Reset(tObjType type)
{
	switch(type)
	{
		case otDoc:
			ClearRefs(this,otDoc);
			ClearIndex(this,otDoc);
			GObjects<GDoc,eCreateDoc>::Clear(pDoc);
			if(SaveResults)
				Storage->Clear(otDoc);
			break;
		case otFdbk:
		{
			RCursor<GDoc> Docs(GetObjs(pDoc));
			for(Docs.Start();!Docs.End();Docs.Next())
				Docs()->ClearFdbks();
			RCursor<GProfile> Profiles(GetObjs(pProfile));
			for(Profiles.Start();!Profiles.End();Profiles.Next())
				Profiles()->ClearFdbks();
			if(SaveResults)
				Storage->Clear(otFdbk);
			break;
		}
		case otUser:
		{
			GObjects<GUser,eCreateUser>::Clear(pUser);
			if(SaveResults)
				Storage->Clear(otUser);
			break;
		}
		case otProfile:
		{
			ClearRefs(this,otProfile);
			ClearIndex(this,otProfile);
			GObjects<GProfile,eCreateProfile>::Clear(pProfile);
			if(SaveResults)
			{
				Storage->Clear(otProfile);
				ResetFile(otProfile,otDescFile);
				ResetFile(otProfile,otIndexFile);
			}
			break;
		}
		case otCommunity:
			ClearRefs(this,otCommunity);
			ClearIndex(this,otCommunity);
			GObjects<GCommunity,eCreateCommunity>::Clear(pCommunity);
			if(SaveResults)
			{
				Storage->Clear(otCommunity);
				ResetFile(otCommunity,otDescFile);
				ResetFile(otCommunity,otIndexFile);
			}
			break;
		case otTopic:
			ClearRefs(this,otTopic);
			ClearIndex(this,otTopic);
			GObjects<GTopic,eCreateTopic>::Clear(pTopic);
			if(SaveResults)
			{
				Storage->Clear(otTopic);
				ResetFile(otTopic,otDescFile);
				ResetFile(otTopic,otIndexFile);
			}
			break;
		case otClass:
			ClearRefs(this,otClass);
			ClearIndex(this,otClass);
			GClasses::Clear();
			if(SaveResults)
			{
				Storage->Clear(otClass);
				ResetFile(otClass,otDescFile);
				ResetFile(otClass,otIndexFile);
			}
			break;
		case otSubject:
			Subjects.Clear();
			break;
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}

	// Send a notification
	GSessionMsg Msg(this,type);
	PostNotification<GSessionMsg&>("Reset",Msg);
}


//------------------------------------------------------------------------------
void GSession::ResetFile(tObjType type,tObjType meta)
{
	switch(type)
	{
		case otDoc:
			switch(meta)
			{
				case otDescFile:
					if(GObjects<GDoc,eCreateDoc>::Desc)
						GObjects<GDoc,eCreateDoc>::Desc->Clear();
					break;
				case otTreeFile:
					if(GObjects<GDoc,eCreateDoc>::Tree)
						GObjects<GDoc,eCreateDoc>::Tree->Clear();
					break;
				case otIndexFile:
					if(GObjects<GDoc,eCreateDoc>::Index)
						GObjects<GDoc,eCreateDoc>::Index->Clear();
					break;
				default:
					mThrowGException(GetObjType(meta,true,true)+" is not a valid file type for documents");
			}
			break;

		case otProfile:
			switch(meta)
			{
				case otDescFile:
					if(GObjects<GProfile,eCreateProfile>::Desc)
						GObjects<GProfile,eCreateProfile>::Desc->Clear();
					break;
				case otIndexFile:
					if(GObjects<GProfile,eCreateProfile>::Index)
						GObjects<GProfile,eCreateProfile>::Index->Clear();
					break;
				default:
					mThrowGException(GetObjType(meta,true,true)+" is not a valid file type for profiles");
			}
			break;

		case otCommunity:
			switch(meta)
			{
				case otDescFile:
					if(GObjects<GCommunity,eCreateCommunity>::Desc)
						GObjects<GCommunity,eCreateCommunity>::Desc->Clear();
					break;
				case otIndexFile:
					if(GObjects<GCommunity,eCreateCommunity>::Index)
						GObjects<GCommunity,eCreateCommunity>::Index->Clear();
					break;
				default:
					mThrowGException(GetObjType(meta,true,true)+" is not a valid file type for communities");
			}
			break;

		case otTopic:
			switch(meta)
			{
				case otDescFile:
					if(GObjects<GTopic,eCreateTopic>::Desc)
						GObjects<GTopic,eCreateTopic>::Desc->Clear();
					break;
				case otIndexFile:
					if(GObjects<GTopic,eCreateTopic>::Index)
						GObjects<GTopic,eCreateTopic>::Index->Clear();
					break;
				default:
					mThrowGException(GetObjType(meta,true,true)+" is not a valid file type for topics");
			}
			break;

		case otClass:
			switch(meta)
			{
				case otDescFile:
					if(GObjects<GClass,eCreateClass>::Desc)
						GObjects<GClass,eCreateClass>::Desc->Clear();
					break;
				case otIndexFile:
					if(GObjects<GClass,eCreateClass>::Index)
						GObjects<GClass,eCreateClass>::Index->Clear();
					break;
				default:
					mThrowGException(GetObjType(meta,true,true)+" is not a valid file type for classes");
			}
			break;

		default:
			mThrowGException(GetObjType(type,true,true)+" have no files associated");
	}

		// Send a notification
	GSessionMsg Msg(this,type,meta);
	PostNotification<GSessionMsg&>("ResetFile",Msg);
}


//------------------------------------------------------------------------------
void GSession::ForceReCompute(tObjType type)
{
	switch(type)
	{
		case otDoc:
		{
			// Clear the description of the documents -> Also profiles and topics
			RCursor<GDoc> Doc(GetObjs(pDoc));
			for(Doc.Start();!Doc.End();Doc.Next())
			{
				Doc()->ClearInfos(SaveResults);
				Doc()->ClearTree(SaveResults);
				if(SaveResults)
					Storage->SaveObj(Doc());
			}
			if(SaveResults)
			{
				ResetFile(otDoc,otDescFile);
				ResetFile(otDoc,otIndexFile);
				ResetFile(otDoc,otTreeFile);
			}
			ClearRefs(this,otDoc);
			ClearIndex(this,otDoc);
			ForceReCompute(otTopic);
			ForceReCompute(otProfile);
			ForceReCompute(otClass);
			break;
		}
		case otTopic:
		{
			// Delete the topics
			Reset(otTopic);
			break;
		}
		case otProfile:
		{
			// Clear the description of the documents -> Also communities
			RCursor<GProfile> Profile(GetObjs(pProfile));
			for(Profile.Start();!Profile.End();Profile.Next())
			{
				Profile()->ClearInfos(SaveResults);
				if(SaveResults)
					Storage->SaveObj(Profile());
			}
			if(SaveResults)
			{
				ResetFile(otProfile,otDescFile);
				ResetFile(otProfile,otIndexFile);
			}
			ClearRefs(this,otProfile);
			ClearIndex(this,otProfile);
			ForceReCompute(otCommunity);
			break;
		}
		case otCommunity:
		{
			// Delete the communities
			Reset(otCommunity);
			break;
		}
		case otClass:
		{
			// Delete the classes
			Reset(otClass);
			break;
		}
		default:
			mThrowGException(GetObjType(type,true,true)+" are not allowed");
	}

	// Send a notification
	GSessionMsg Msg(this,type);
	PostNotification<GSessionMsg&>("ForceReCompute",Msg);
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
size_t GSession::GetNbObjs(tObjType type) const
{
	switch(type)
	{
		case otUser:
			return(GObjects<GUser,eCreateUser>::Objects.GetNb());
		case otConcept:
			return(Concepts.GetNb());
		case otConceptType:
			return(ConceptTypes.GetNb());
		case otDoc:
			return(GObjects<GDoc,eCreateDoc>::Objects.GetNb());
		case otProfile:
			return(GObjects<GProfile,eCreateProfile>::Objects.GetNb());
		case otCommunity:
			return(GObjects<GCommunity,eCreateCommunity>::Objects.GetNb());
		case otTopic:
			return(GObjects<GTopic,eCreateTopic>::Objects.GetNb());
		case otSubject:
			LoadSubjects();
			return(Subjects.GetNbNodes());
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
			if(!GObjects<GDoc,eCreateDoc>::Objects.GetNb())
				return(0);
			return(GObjects<GDoc,eCreateDoc>::Objects[GObjects<GDoc,eCreateDoc>::Objects.GetMaxPos()]->GetId());
		case otProfile:
			if(!GObjects<GProfile,eCreateProfile>::Objects.GetNb())
				return(0);
			return(GObjects<GProfile,eCreateProfile>::Objects[GObjects<GProfile,eCreateProfile>::Objects.GetMaxPos()]->GetId());
		case otCommunity:
			if(!GObjects<GCommunity,eCreateCommunity>::Objects.GetNb())
				return(0);
			return(GObjects<GCommunity,eCreateCommunity>::Objects[GObjects<GCommunity,eCreateCommunity>::Objects.GetMaxPos()]->GetId());
		case otTopic:
			if(!GObjects<GTopic,eCreateTopic>::Objects.GetNb())
				return(0);
			return(GObjects<GTopic,eCreateTopic>::Objects[GObjects<GTopic,eCreateTopic>::Objects.GetMaxPos()]->GetId());
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
			return(GObjects<GDoc,eCreateDoc>::Objects.GetMaxPos());
		case otTopic:
			return(GObjects<GTopic,eCreateTopic>::Objects.GetMaxPos());
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
			return(GetConceptType(id,null));
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
				tab=new GObject*[GObjects<GDoc,eCreateDoc>::Objects.GetMaxPos()+1];
			return(GObjects<GDoc,eCreateDoc>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otProfile:
			if(alloc)
				tab=new GObject*[GObjects<GProfile,eCreateProfile>::Objects.GetMaxPos()+1];
			return(GObjects<GProfile,eCreateProfile>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otCommunity:
			if(alloc)
				tab=new GObject*[GObjects<GCommunity,eCreateCommunity>::Objects.GetMaxPos()+1];
			return(GObjects<GCommunity,eCreateCommunity>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		case otTopic:
			if(alloc)
				tab=new GObject*[GObjects<GTopic,eCreateTopic>::Objects.GetMaxPos()+1];
			return(GObjects<GTopic,eCreateTopic>::Objects.GetTab(reinterpret_cast<void**>(tab)));
		default:
			mThrowGException(GetObjType(type,true,true)+" are not managed");
	}
}


//-----------------------------------------------------------------------------
size_t GSession::GetMaxDepth(void) const
{
	LoadSubjects();
	return(Subjects.MaxDepth);
}


//-----------------------------------------------------------------------------
size_t GSession::GetNbIdealGroups(tObjType type) const
{
	LoadSubjects();
	size_t nb(0);
	RNodeCursor<GSubjects,GSubject> Cur(Subjects);
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
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
	RNodeCursor<GSubjects,GSubject> Cur(Subjects);
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
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
		mThrowGException("Document has multiple subjects");
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
	return(Subjects.GetSubjects());
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
RNodeCursor<GSubjects,GSubject> GSession::GetSubjects(const GSubject* subject) const
{
	LoadSubjects();
	if(subject)
		return(RNodeCursor<GSubjects,GSubject>(subject));
	return(RNodeCursor<GSubjects,GSubject>(Subjects));
}


//------------------------------------------------------------------------------
double GSession::GetUpOperationsCost(const GSubject* u,const GSubject* v) const
{
	return(Subjects.GetUpOperationsCost(u,v));
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
		bool DelRefs(Topic()->IsDefined());
		CalcDesc->Compute(Topic());
		Topic()->Update(this,CalcDesc->Description,DelRefs);

		if(SaveResults)
		{
			if(Topic()->IsDefined())
				Topic()->SaveDesc();
			Storage->SaveObj(Topic());
			Topic()->SetState(osSaved);
		}
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
		bool DelRefs(Community()->IsDefined());
		CalcDesc->Compute(Community());
		Community()->Update(this,CalcDesc->Description,DelRefs);

		if(SaveResults)
		{
			if(Community()->IsDefined())
				Community()->SaveDesc();
			Storage->SaveObj(Community());
			Community()->SetState(osSaved);
		}
	}
	FlushDesc(pCommunity);   // Force to save all communities description
}


//------------------------------------------------------------------------------
void GSession::Init(void)
{
	// Create the configuration parameters
	GSimulator::CreateConfig(this);
	GObjects<GDoc,eCreateDoc>::Init(this);
	GObjects<GTopic,eCreateTopic>::Init(this);
	GObjects<GUser,eCreateUser>::Init(this);
	GObjects<GProfile,eCreateProfile>::Init(this);
	GObjects<GCommunity,eCreateCommunity>::Init(this);
	GObjects<GClass,eCreateClass>::Init(this);

	// Create the configuration parameters for the plug-ins
	R::RCursor<GPlugInManager> Managers(GALILEIApp->GetManagers());
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->CreateConfig(this);

	// Load the configuration now
	Load(false,true);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig(this);
	ApplyConfig();

	// This is valid configuration file
	ValidConfigFile=true;
}


//------------------------------------------------------------------------------
void GSession::Insert(GDoc* doc,size_t subjectid,bool selected)
{
	LoadSubjects();

	// Find the subject
	GSubject* Subject(Subjects.GetNode(subjectid));
	if(!Subject)
		mThrowGException("No subject with identifier '"+RString::Number(subjectid)+"'");

	// Look if the document must be insert in CategorizedDocs
	bool Find;
	size_t Pos(Subject->CategorizedDocs.GetIndex(doc,Find));
	if(!Find)
		Subject->CategorizedDocs.InsertPtrAt(doc,Pos,false);

	// If it is not selected -> nothing to do
	if(!selected)
		return;

	// Add the document in Subjects
	R::RContainer<GSubject,false,false>* line(Subjects.DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		Subjects.DocsSubjects.InsertPtrAt(line=new R::RContainer<GSubject,false,false>(10,5),doc->GetId(),true);
	Pos=line->GetIndex(*Subject,Find);
	if(!Find)
		line->InsertPtrAt(Subject,Pos,false);

	// Add the document in Subject
	Pos=Subject->Docs.GetIndex(doc,Find);
	if(!Find)
		Subject->Docs.InsertPtrAt(doc,Pos,false);

	// Emit a selection signal
	doc->PostNotification(eSelectDoc);
}


//------------------------------------------------------------------------------
void GSession::Insert(GProfile* profile,size_t subjectid)
{
	LoadSubjects();
	GSubject* subject(Subjects.Subjects.GetPtr(subjectid));
	if(!subject)
		mThrowGException("No subject with identifier '"+RString::Number(subjectid)+"'");
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
void GSession::LoadSubjects(void) const
{
	if(Subjects.SubjectsLoaded)
		return;
	const_cast<GSession*>(this)->Subjects.SubjectsLoaded=true;
	const_cast<GSession*>(this)->Storage->LoadSubjects();
}


//------------------------------------------------------------------------------
void GSession::RequestMetaEngine(const R::RString query)
{
	GMetaEngine* MetaEngine(GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine"));
	MetaEngine->Request(query);
}


//------------------------------------------------------------------------------
void GSession::ReInit(void)
{
	// Clear feedbacks
	Reset(otFdbk);

	Reset(otCommunity);
	Reset(otProfile);
	Reset(otUser);

	// Re-initialize the subjects
	LoadSubjects();
	RNodeCursor<GSubjects,GSubject> Subject(Subjects);
	for(Subject.Start();!Subject.End();Subject.Next())
		Subject()->ReInit();

	// Clear the documents and profiles assignment
	Subjects.ProfilesSubject.Clear();
	Subjects.DocsSubjects.Clear();
	Subjects.SelectedDocs.Clear();
}


//------------------------------------------------------------------------------
void GSession::RunTool(const RString& name,const RString& list,GSlot* slot,bool need)
{
	GTool* Tool(GALILEIApp->GetPlugIn<GTool>("Tools",name,list));
	if((!Tool)&&need)
		mThrowGException("Tool '"+name+"' does not exist in '"+list+"'");
	try
	{
		Tool->Run(slot);
	}
	HANDLEALLEXCEPTIONS(slot,"Error while executing the tool '"+name+"' in '"+list+"': ")
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
			if(Cur()->Vectors)
			{
				delete Cur()->Vectors;
				Cur()->Vectors=0;
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
			RNodeCursor<GSubjects,GSubject> Cur2(Subject);
			for(Cur2.Start();(!Cur2.End())&&OK;Cur2.Next())
				if(Cur2()->GetNbSubjects()||Cur2()->CategorizedDocs.GetNb())
					OK=false;
			if(!OK)
			{
				NbNoLeaf++;

				// Create a new subject
				GSubject* NewSubject(new GSubject(this,Subjects.GetNbNodes()+NbNoLeaf,Subject->Name+" general",true));
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

	cout<<"There are "<<GetNbObjs(otSubject)<<" subjects:"<<endl;
	if(NbNoLeaf)
		cout<<"  "<<NbNoLeaf<<" non-leaf subjects have some documents attached"<<endl;
	if(NbLeaf)
		cout<<"  "<<NbLeaf<<" leaf subjects have no documents attached"<<endl;
	cout<<"  "<<static_cast<double>(NbNoLeaf+NbLeaf)*100.0/static_cast<double>(GetNbObjs(otSubject))<<"% of invalid subjects"<<endl;
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
void GSession::Assign(GClass* theclass,GDescription& desc)
{
	theclass->Update(desc);
	if(SaveResults)
	{
		theclass->SaveDesc();
		Storage->SaveObj(theclass);
	}
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
