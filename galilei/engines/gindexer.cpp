/*

	GALILEI Research Project

	GIndexer.cpp

	Indexer - Implementation.

	Copyright 2004-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 Université Libre de Bruxelles (ULB).

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
#include <math.h>


//------------------------------------------------------------------------------
// include file for GALILEI
#include <rdir.h>
#include <rrecfile.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include file for GALILEI
#include <gindexer.h>
#include <glang.h>
#include <gconcept.h>
#include <gdocstruct.h>
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <ggalileiapp.h>
#include <gdoc.h>
#include <gdocstruct.h>
#include <gprofile.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <gconcept.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  Records to store description and structure files
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Record representing an occurrence of concept of a given type with a given weight
class DescRec
{
public:
	char TypeId;       // Identifier of the type.
	size_t ConceptId;  // Identifier of the concept.
	double Weight;     // Weight of the concepts.

	DescRec(void) : TypeId(0), ConceptId(0), Weight(NAN) {}
	void Set(char tid,size_t cid,double w) {TypeId=tid;ConceptId=cid;Weight=w;}
	void Read(RRecFile<DescRec,false>& f)  {f>>TypeId>>ConceptId>>Weight;}
	void Write(RRecFile<DescRec,false>& f) {f<<TypeId<<ConceptId<<Weight;}
};



//------------------------------------------------------------------------------
//
// class GIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIndexer::GIndexer(GSession* session)
	: Session(session)
{
}


//------------------------------------------------------------------------------
void GIndexer::Clear(tObjType objtype)
{
	switch(objtype)
	{
		case otDoc:
		{
			RDir DelDir(DirDocs);
			DelDir.Clear();
			break;
		}
		case otProfile:
		{
			RDir DelDir(DirProfiles);
			DelDir.Clear();
			break;
		}
		case otCommunity:
		{
			RDir DelDir(DirCommunities);
			DelDir.Clear();
			break;
		}
		case otTopic:
		{
			RDir DelDir(DirTopics);
			DelDir.Clear();
			break;
		}
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GIndexer::Apply(void)
{
	RString Dir(GALILEIApp->GetIndexDir());
	RString Name(Session->GetStorage()->GetWorld());
	try
	{
		RDir::CreateDirIfNecessary(Dir+RFile::GetDirSeparator()+Name,true);
		DirDocs=Dir+RFile::GetDirSeparator()+Name+RFile::GetDirSeparator()+"Documents"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirDocs,false);
		DirProfiles=Dir+RFile::GetDirSeparator()+Name+RFile::GetDirSeparator()+"Profiles"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirProfiles,false);
		DirCommunities=Dir+RFile::GetDirSeparator()+Name+RFile::GetDirSeparator()+"Communities"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirCommunities,false);
		DirTopics=Dir+RFile::GetDirSeparator()+Name+RFile::GetDirSeparator()+"Topics"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirTopics,false);
	}
	catch(...)
	{
		cerr<<"GIndexer::Apply: Problems in creating directories in '"<<Dir<<"'"<<endl;
	}
}


//------------------------------------------------------------------------------
void GIndexer::LoadInfos(GWeightInfos& infos,tObjType type,size_t id)
{
	try
	{
		// Find the file to open
		RString File;
		switch(type)
		{
			case otDoc:
				File=DirDocs;
				break;
			case otProfile:
				File=DirProfiles;
				break;
			case otCommunity:
				File=DirCommunities;
				break;
			case otTopic:
				File=DirTopics;
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		File+=RString::Number(id)+".desc";
		if(!RFile::Exists(File)) return;

		// Read the file
		DescRec Rec;
		GConceptType* type(0);
		char tid=0;
		size_t i;
		RRecFile<DescRec,false> Obj(File,sizeof(DescRec));
		Obj.Open(RIO::Read);
		for(i=0;!Obj.End();i++)
		{
			Obj.ReadRec(Rec);
			if(tid!=Rec.TypeId)
			{
				tid=Rec.TypeId;
				type=Session->GetConceptType(tid,false);
			}
			infos.InsertPtrAt(new GWeightInfo(type->GetConcept(Rec.ConceptId),Rec.Weight),i,false);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(RString("GIndexer::LoadInfos: ")+e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GIndexer::SaveInfos(GWeightInfos& infos,tObjType type,size_t id)
{
	try
	{
		// Find the file to open
		RString File;
		switch(type)
		{
			case otDoc:
				File=DirDocs;
				break;
			case otProfile:
				File=DirProfiles;
				break;
			case otCommunity:
				File=DirCommunities;
				break;
			case otTopic:
				File=DirTopics;
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		File+=RString::Number(id)+".desc";

		// Create the file
		DescRec Rec;
		RRecFile<DescRec,false> Obj(File,sizeof(DescRec));
		Obj.Open(RIO::Create);
		RCursor<GWeightInfo> Words(infos);
		for(Words.Start();!Words.End();Words.Next())
		{
			Rec.Set(Words()->GetType()->GetId(),Words()->GetId(),Words()->GetWeight());
			Obj.WriteRec(Rec);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(RString("GIndexer::SaveInfos: ")+e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GIndexer::LoadStruct(GDocStruct& docstruct,GDoc* doc)
{
	try
	{
		// Read the file
		DescRec Rec;
		GConceptType* ctype(0);
		size_t tId,pos,nbchild;
		char tTypeId(0),type_id;
		char depth;
		char type;

		RBinaryFile Obj(DirDocs+RString::Number(doc->GetId())+".struct");
		Obj.Open(RIO::Read);

		// Load the size of the LCs
		size_t max=doc->GetNbLCs();
		for(size_t i=0;i<max;i++)
		{
			Obj>>tId;
			docstruct.SetNbLCEntries(i,tId);
		}

		// Load the records
		while(!Obj.End())
		{

			Obj>>tId>>type_id>>type>>pos>>depth>>nbchild;
			if(type_id!=tTypeId)
			{
				tTypeId=type_id;
				ctype=Session->GetConceptType(type_id,false);
			}
			docstruct.AddRecord(ctype->GetConcept(tId),static_cast<GVTDRec::RecType>(type),pos,depth,nbchild);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(RString("GIndexer::LoadStruct: ")+e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GIndexer::SaveStruct(GDocStruct& docstruct,GDoc* doc)
{
	try
	{
	    if(!docstruct.GetNbRecs())
			return;
		// Create the file
		RBinaryFile Obj(DirDocs+RString::Number(doc->GetId())+".struct");
		Obj.Open(RIO::Create);

		// Save the size of the LCs
		size_t max=docstruct.GetNbLCs();
		for(size_t i=0;i<max;i++)
			Obj<<docstruct.GetNbLCEntries(i);

		// Save the records
		R::RCursor<GVTDRec> Recs(docstruct.GetRecs());
		for(Recs.Start();!Recs.End();Recs.Next())
			Obj<<Recs()->GetConcept()->GetId()<<Recs()->GetConcept()->GetType()->GetId()<<static_cast<char>(Recs()->GetType())<<Recs()->GetPos()<<Recs()->GetDepth()<<docstruct.GetFirstChild(Recs());
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(RString("GIndexer::SaveStruct: ")+e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GIndexer::BuildRefs(tObjType type)
{
/*	const size_t MaxBuffer(1024*1024);
	off_t* Buffer[MaxBuffer/sizeof(off_t)];*/

	// Open the indexer
	RString IndexName(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetStorage()->GetWorld()+RFile::GetDirSeparator()+"Index");
	RDir::CreateDirIfNecessary(IndexName,false);
	IndexName+=RFile::GetDirSeparator();
	switch(type)
	{
		case otDoc:
			IndexName+="Documents.index";
			break;
		default:
			throw GException("GIndexer::BuildRefs: Cannot build an index for "+GetObjType(type));
	}
	RBinaryFile Index(IndexName);
	Index.Open(RIO::Create);

	// Go trough all the concepts
/*	GConcept* MinConcept(0);
	GConcept* MaxConcept(0);
	RCursor<GConceptType> Types(Session->GetConceptTypes());
	Types.Start();
	GConcept** Concepts(Types()->Direct);
	size_t Nb(Types()->GetNb());
	MinConcept=(*Concepts);
	i=0;
	size_t NbMem(0);*/
}


//------------------------------------------------------------------------------
void GIndexer::FindRefs(GConcept* /*concept*/,R::RVectorInt<size_t,true>& /*refs*/,tObjType /*type*/) const
{
}


//------------------------------------------------------------------------------
GIndexer::~GIndexer(void)
{
}
