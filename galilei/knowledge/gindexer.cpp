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
#include <rindexfile.h>
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
const size_t SizeRecDesc=sizeof(size_t)+sizeof(double);
const size_t SizeRecStruct=sizeof(size_t)+sizeof(char)+sizeof(size_t)+sizeof(char)+sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);



//------------------------------------------------------------------------------
//
// class GIndexer::DescFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GIndexer::IndexFile : public RIndexFile
{
public:

	tObjType Type;

	IndexFile(tObjType type,const RURI& uri,size_t blocksize,size_t nbcaches,size_t tolerance)
	: RIndexFile(uri,blocksize,nbcaches,tolerance), Type(type)
	{
		Open();
	}
	int Compare(const IndexFile& file) const {return(Type-file.Type);}
	int Compare(tObjType type) const {return(Type-type);}
};



//------------------------------------------------------------------------------
//
// class GIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIndexer::GIndexer(void)
	: IndexDocs(0), Desc(10), StructDoc(0), tmpRefs(5000), IndexDocsInc(false)
{
}


//------------------------------------------------------------------------------
void GIndexer::Clear(tObjType objtype)
{
	IndexFile* ptr(Desc.GetPtr(objtype));
	if(ptr)
		ptr->Clear();
}


//------------------------------------------------------------------------------
void GIndexer::CreateConfig(RConfig* config)
{
	config->InsertParam(new RParamValue("IndexDocsInc",false),"Indexer");

	// Documents descriptions
	config->InsertParam(new RParamValue("DocsDescBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("DocsDescTolerance",10),"Indexer");
	config->InsertParam(new RParamValue("DocsDescCache",20),"Indexer");
	config->InsertParam(new RParamValue("DocsDescType",RBlockFile::WriteBack),"Indexer");

	// Documents structures
	config->InsertParam(new RParamValue("DocsStructBlock",4096),"Indexer");
	config->InsertParam(new RParamValue("DocsStructTolerance",10),"Indexer");
	config->InsertParam(new RParamValue("DocsStructCache",40),"Indexer");
	config->InsertParam(new RParamValue("DocsStructType",RBlockFile::WriteBack),"Indexer");

	// Documents inverted file
	config->InsertParam(new RParamValue("DocsIndexBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("DocsIndexTolerance",10),"Indexer");
	config->InsertParam(new RParamValue("DocsIndexCache",20),"Indexer");
	config->InsertParam(new RParamValue("DocsIndexType",RBlockFile::WriteBack),"Indexer");

	// Profiles descriptions
	config->InsertParam(new RParamValue("ProfilesDescBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("ProfilesDescTolerance",10),"Indexer");
	config->InsertParam(new RParamValue("ProfilesDescCache",20),"Indexer");
	config->InsertParam(new RParamValue("ProfilesDescType",RBlockFile::WriteBack),"Indexer");

	// Communities descriptions
	config->InsertParam(new RParamValue("CommunitiesDescBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("CommunitiesDescTolerance",10),"Indexer");
	config->InsertParam(new RParamValue("CommunitiesDescCache",20),"Indexer");
	config->InsertParam(new RParamValue("CommunitiesDescType",RBlockFile::WriteBack),"Indexer");

	// Topics descriptions
	config->InsertParam(new RParamValue("TopicsDescBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("TopicsDescTolerance",20),"Indexer");
	config->InsertParam(new RParamValue("TopicsDescCache",20),"Indexer");
	config->InsertParam(new RParamValue("TopicsDescType",RBlockFile::WriteBack),"Indexer");

	// Classes descriptions
	config->InsertParam(new RParamValue("ClassesDescBlock",1024),"Indexer");
	config->InsertParam(new RParamValue("ClassesDescTolerance",20),"Indexer");
	config->InsertParam(new RParamValue("ClassesDescCache",20),"Indexer");
	config->InsertParam(new RParamValue("ClassesDescType",RBlockFile::WriteBack),"Indexer");
}


//------------------------------------------------------------------------------
void GIndexer::Apply(RConfig* config)
{
	// Apply the modifications
	IndexDocsInc=config->GetBool("IndexDocsInc","Indexer");

	// Create (if necessary) the directory corresponding to the name of the session
	// Create all the index files
	RString Dir(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Storage->GetWorld()+RFile::GetDirSeparator());
	try
	{
		IndexFile* ptr;
		RDir::CreateDirIfNecessary(Dir,true);
		Desc.InsertPtr(ptr=new IndexFile(otDoc,Dir+"Documents.desc",config->GetUInt("DocsDescBlock","Indexer"),config->GetUInt("DocsDescCache","Indexer"),config->GetUInt("DocsDescTolerance","Indexer")));
		ptr->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("DocsDescType","Indexer")));
		Desc.InsertPtr(ptr=new IndexFile(otProfile,Dir+"Profiles.desc",config->GetUInt("ProfilesDescBlock","Indexer"),config->GetUInt("ProfilesDescCache","Indexer"),config->GetUInt("ProfilesDescTolerance","Indexer")));
		ptr->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("ProfilesDescType","Indexer")));
		Desc.InsertPtr(ptr=new IndexFile(otCommunity,Dir+"Communities.desc",config->GetUInt("CommunitiesDescBlock","Indexer"),config->GetUInt("CommunitiesDescCache","Indexer"),config->GetUInt("CommunitiesDescTolerance","Indexer")));
		ptr->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("CommunitiesDescType","Indexer")));
		Desc.InsertPtr(ptr=new IndexFile(otTopic,Dir+"Topics.desc",config->GetUInt("TopicsDescBlock","Indexer"),config->GetUInt("TopicsDescCache","Indexer"),config->GetUInt("TopicsDescTolerance","Indexer")));
		ptr->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("TopicsDescType","Indexer")));
		Desc.InsertPtr(ptr=new IndexFile(otClass,Dir+"Classes.desc",config->GetUInt("ClassesDescBlock","Indexer"),config->GetUInt("ClassesDescCache","Indexer"),config->GetUInt("ClassesDescTolerance","Indexer")));
		ptr->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("ClassesDescType","Indexer")));
		StructDoc=new IndexFile(otDoc,Dir+"Documents.struct",config->GetUInt("DocsStructBlock","Indexer"),config->GetUInt("DocsStructCache","Indexer"),config->GetUInt("DocsStructTolerance","Indexer"));
		StructDoc->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("DocsStructType","Indexer")));
		IndexDocs=new IndexFile(otDoc,Dir+"Documents.index",config->GetUInt("DocsIndexBlock","Indexer"),config->GetUInt("DocsIndexCache","Indexer"),config->GetUInt("DocsIndexTolerance","Indexer"));
		IndexDocs->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("DocsIndexType","Indexer")));
	}
	catch(...)
	{
		cerr<<"GIndexer::Apply: Problems in creating directories in '"<<Dir<<"'"<<endl;
	}
}


//------------------------------------------------------------------------------
void GIndexer::LoadInfos(GWeightInfos* &infos,tObjType type,size_t blockid,size_t id)
{
	try
	{
		IndexFile* ptr(Desc.GetPtr(type));
		if(!ptr)
			throw GException("GIndexer::LoadInfos : The objects of type '"+GetObjType(type)+"' do not have descriptions");

		// Position the block file to the correct position and read the size
		ptr->Seek(blockid,id);
		size_t size,concept;
		double weight;
		ptr->Read((char*)&size,sizeof(size_t));
		if(!infos)
			infos=new GWeightInfos(size);
		else
			infos->Clear(size);
		for(size_t i=0;i<size;i++)
		{
			ptr->Read((char*)&concept,sizeof(size_t));
			ptr->Read((char*)&weight,sizeof(double));
			infos->InsertPtrAt(new GWeightInfo(dynamic_cast<GSession*>(this)->GetConcept(concept),weight),i,false);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(RString("GIndexer::LoadInfos: ")+e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GIndexer::SaveInfos(const GWeightInfos* infos,tObjType type,size_t& blockid,size_t id)
{
	try
	{
		size_t size(infos->GetNb()),concept;
		double weight;
		if(!size)
			return;

		IndexFile* ptr(Desc.GetPtr(type));
		if(!ptr)
			throw GException("GIndexer::SaveInfos : The objects of type '"+GetObjType(type)+"' do not have descriptions");

		// Position the block file to the correct position and write the size
		ptr->Seek(blockid,id,sizeof(size_t)+size*SizeRecDesc);
		ptr->Write((char*)&size,sizeof(size_t));
		RCursor<GWeightInfo> Cur(*infos);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			concept=Cur()->GetId();
			weight=Cur()->GetWeight();
			ptr->Write((char*)&concept,sizeof(size_t));
			ptr->Write((char*)&weight,sizeof(double));
		}
	}
	catch(RIOException e)
	{
		throw GException(RString("GIndexer::SaveInfos: ")+e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GIndexer::LoadStruct(GDocStruct* &docstruct,size_t blockid,size_t id)
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
		throw GException(RString("GIndexer::LoadStruct: ")+e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GIndexer::SaveStruct(GDocStruct* docstruct,size_t& blockid,size_t id)
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
		throw GException(RString("GIndexer::SaveStruct: ")+e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GIndexer::UpdateRefs(const GWeightInfos& infos,tObjType type,size_t id,bool add)
{
	if(type!=otDoc)
		throw GException("GIndexer::UpdateRefs: Cannot find an index for "+GetObjType(type));

	// Update the index for all concepts
	RCursor<GWeightInfo> Cur(infos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Read the vector representing the current index
		GConcept* Concept(Cur()->Concept);
		if(Concept->IndexDocs)
			IndexDocs->Read(Concept->IndexDocs,Concept->Id,tmpRefs);
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
			IndexDocs->Write(Concept->IndexDocs,Concept->Id,tmpRefs);
	}
}


//------------------------------------------------------------------------------
void GIndexer::LoadRefs(GConcept* concept,RNumContainer<size_t,true>& refs,tObjType type)
{
	if(type!=otDoc)
		throw GException("GIndexer::LoadRefs: Cannot find an index for "+GetObjType(type));

	if(!concept->IndexDocs)
	{
		refs.Clear();
		return;
	}
	IndexDocs->Read(concept->IndexDocs,concept->Id,refs);
}


//------------------------------------------------------------------------------
void GIndexer::BuildRefs(tObjType type,GSlot* slot)
{
	if(type!=otDoc)
		throw GException("GIndexer::LoadRefs: Cannot find an index for "+GetObjType(type));
	IndexFile* ptr(Desc.GetPtr(type));
	if(!ptr)
		throw GException("GIndexer::BuildRefs : Big problem");
	GSession* Session(dynamic_cast<GSession*>(this));

	// Clear the file and put all block identifier of concepts to 0.
	if(slot)
		slot->WriteStr("Clear the index");
	IndexDocs->Clear();
	RCursor<GConcept> Concepts(Session->GetConcepts());
	for(Concepts.Start();!Concepts.End();Concepts.Next())
	{
		Concepts()->IndexDocs=0;

		// If no cache is asked -> Save each time
		if(IndexDocs->GetCacheType()==RBlockFile::WriteThrough)
			Storage->SaveIndex(Concepts(),otDoc,Concepts()->IndexDocs);
	}

	// Go trough each document
	R::RCursor<GDoc> Cur(dynamic_cast<GSession*>(this)->GetDocs());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(slot)
			slot->NextDoc(Cur());

		// Position the block file to the correct position and read the size
		ptr->Seek(Cur()->BlockId,Cur()->Id);
		size_t size,concept;
		double weight;
		ptr->Read((char*)&size,sizeof(size_t));
		for(size_t i=0;i<size;i++)
		{
			// Read concept identifier and weight
			ptr->Read((char*)&concept,sizeof(size_t));
			ptr->Read((char*)&weight,sizeof(double));

			// Read the vector representing the current index
			GConcept* Concept(Session->GetConcept(concept));
			if(Concept->IndexDocs)
				IndexDocs->Read(Concept->IndexDocs,Concept->Id,tmpRefs);
			else
				tmpRefs.Clear();

			// Add the document to the index
			size_t oldsize(tmpRefs.GetNb());
			tmpRefs.Insert(Cur()->Id);

			// If the size of vector has changed -> Save it back
			if(oldsize!=tmpRefs.GetNb())
			{
				IndexDocs->Write(Concept->IndexDocs,Concept->Id,tmpRefs);

				// If no cache is asked -> Save each time
				if(IndexDocs->GetCacheType()==RBlockFile::WriteThrough)
					Storage->SaveIndex(Concept,otDoc,Concept->IndexDocs);
			}
		}
	}

	// If cache was asked, flush the cache and save the block identifier of the concepts
	if(IndexDocs->GetCacheType()==RBlockFile::WriteBack)
	{
		if(slot)
			slot->WriteStr("Flush the index");
		IndexDocs->Flush();
		RCursor<GConcept> Concepts(Session->GetConcepts());
		for(Concepts.Start();!Concepts.End();Concepts.Next())
			Storage->SaveIndex(Concepts(),otDoc,Concepts()->IndexDocs);
	}
}



//------------------------------------------------------------------------------
GIndexer::~GIndexer(void)
{
	delete IndexDocs;
	delete StructDoc;
}
