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
#include <gslot.h>
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
	: Session(session), DocsInvertedFile(0)
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
void GIndexer::BuildBuffer(size_t* buffer,GConcept* min,GConcept* max)
{
/*	cout<<"Index concepts between ("<<(int)min->GetType()->GetId()<<","<<min->GetId();
	cout<<") and ("<<(int)max->GetType()->GetId()<<","<<max->GetId()<<")"<<endl;*/

	if(!max)
		max=min;
	GWeightInfos Doc(10000);

	// Go through the documents
	RCursor<GDoc> Docs(Session->GetDocs());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Load the description in Infos
		Doc.Clear();
		LoadInfos(Doc,otDoc,Docs()->GetId());

		// Go thought the information entities
		RCursor<GWeightInfo> Infos(Doc.GetInfos());
		for(Infos.Start();!Infos.End();Infos.Next())
		{
			if(((*Infos())<min)||((*Infos())>max)) continue;

			// Set that Infos() is referenced in Docs()
			GConcept* ptr(Infos()->GetConcept());
			buffer[static_cast<size_t>(/*Infos()->GetConcept()*/ptr->IndexDocs++)]=Docs()->GetId();
		}
	}
}


//------------------------------------------------------------------------------
void GIndexer::BuildRefs(tObjType type,GSlot* slot)
{
	const size_t MaxBuffer(GALILEIApp->GetIndexRamSize()*1024/sizeof(size_t));
	cout<<MaxBuffer<<endl;
	size_t* Buffer(new size_t[MaxBuffer*sizeof(size_t)]);

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

	// Go trough all the concepts a first time
	GConcept* MinConcept(0);
	GConcept* MaxConcept(0);
	RCursor<GConceptType> Types(Session->GetConceptTypes());
	Types.Start();
	if(slot)
		slot->NextConceptType(Types());
	GConcept** Concepts(Types()->Direct);
	size_t Nb(Types()->MaxId);
	size_t i(0);
	size_t PosMem(0);
	for(;;Concepts++,i++)
	{
		// Test if not end of the current concept type
		if(i==Nb)
		{
			do
			{
				Types.Next();
				if(Types.End()) // End of the concepts (-> End of do-while)
					break;
				if(slot)
					slot->NextConceptType(Types());
				Concepts=Types()->Direct;
				Nb=Types()->MaxId;
				i=0;
			}
			while(!Nb);
			if(Types.End()) // End of the concepts (-> End of for)
				break;
		}

		// If no concept -> skip it
		if(!(*Concepts)) continue;

		// Verify if the maximum buffer is obtained if this concept is treated
		if(PosMem+(*Concepts)->NbRefDocs>=MaxBuffer)
		{
			BuildBuffer(Buffer,MinConcept,MaxConcept);
			Index.Write(reinterpret_cast<char*>(Buffer),PosMem*sizeof(size_t));
			MinConcept=MaxConcept=0;
			PosMem=0;
		}

		// Treat the concept
		if(!MinConcept)
			MinConcept=(*Concepts);
		(*Concepts)->IndexDocs=PosMem;
		PosMem+=(*Concepts)->NbRefDocs;
		MaxConcept=(*Concepts); // Suppose this concept is the last one to index
	}
	if(PosMem) // Something left to write
	{
		BuildBuffer(Buffer,MinConcept,MaxConcept);
		Index.Write(reinterpret_cast<char*>(Buffer),PosMem*sizeof(size_t));
	}

	delete[] Buffer;


	// Go trough all the concepts a second time
	Types.Start();
	if(slot)
		slot->NextConceptType(Types());
	Concepts=Types()->Direct;
	Nb=Types()->MaxId;
	i=0;
	off_t PosFile(0);
	for(;;Concepts++,i++)
	{
		// Test if not end of the current concept type
		if(i==Nb)
		{
			do
			{
				Types.Next();
				if(Types.End()) // End of the concepts (-> End of do-while)
					break;
				if(slot)
					slot->NextConceptType(Types());
				Concepts=Types()->Direct;
				Nb=Types()->MaxId;
				i=0;
			}
			while(!Nb);
			if(Types.End()) // End of the concepts (-> End of for)
				break;
		}

		// Add the concepts to the current index
		if(!(*Concepts)) continue;
		(*Concepts)->IndexDocs=PosFile;
		PosFile+=(*Concepts)->NbRefDocs*sizeof(size_t);
		Session->GetStorage()->SaveIndex(*Concepts,otDoc,(*Concepts)->IndexDocs);
	}
}


//------------------------------------------------------------------------------
void GIndexer::FindRefs(GConcept* concept,R::RVectorInt<size_t,false>& refs,tObjType type)
{
	if(type!=otDoc)
		throw GException("GIndexer::FindRefs: Cannot find an index for "+GetObjType(type));

	// If the inverted file is not open -> open it
	if(!DocsInvertedFile)
	{
		RString IndexName(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetStorage()->GetWorld()+RFile::GetDirSeparator()+"Index"+RFile::GetDirSeparator());
		switch(type)
		{
			case otDoc:
				IndexName+="Documents.index";
				break;
			default:
				throw GException("GIndexer::FindRefs: Cannot find an index for "+GetObjType(type));
		}

		DocsInvertedFile=new RBinaryFile(IndexName);
		DocsInvertedFile->Open(RIO::Read);
	}

	// Fill refs
	refs.Clear();
	off_t Pos(concept->IndexDocs);
	if(Pos==MaxOffT)
		return;
	DocsInvertedFile->Seek(Pos);
	for(size_t nb(concept->NbRefDocs+1);--nb;)
	{
		size_t DocId;
		DocsInvertedFile->Read(reinterpret_cast<char*>(&DocId),sizeof(size_t));
		refs.Insert(DocId);
	}
}


//------------------------------------------------------------------------------
GIndexer::~GIndexer(void)
{
	delete DocsInvertedFile;
}
