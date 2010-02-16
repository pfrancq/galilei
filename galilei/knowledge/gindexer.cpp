/*

	GALILEI Research Project

	GIndexer.cpp

	Indexer - Implementation.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
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
// class GIndexer::Type
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GIndexer::IndexType
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
// class GIndexer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GIndexer::GIndexer(void)
	: StructDoc(0), tmpRefs(5000), Types(5), TypesNames(5)
{
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
void GIndexer::CreateConfig(RConfig* config)
{
	// Parse all types
	RCursor<IndexType> Type(Types);
	for(Type.Start();!Type.End();Type.Next())
	{
		// Automatic indexing
		config->InsertParam(new RParamValue("Index"+Type()->Name+"Inc",false),"Indexer");

		// Descriptions
		config->InsertParam(new RParamValue(Type()->Name+"DescBlock",1024),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"DescTolerance",10),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"DescCache",20),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"DescType",RBlockFile::WriteBack),"Indexer");

		// Inverted file
		config->InsertParam(new RParamValue(Type()->Name+"IndexBlock",1024),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"IndexTolerance",10),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"IndexCache",20),"Indexer");
		config->InsertParam(new RParamValue(Type()->Name+"IndexType",RBlockFile::WriteBack),"Indexer");

	}

	// Documents structures
	config->InsertParam(new RParamValue("DocumentsStructBlock",4096),"Indexer");
	config->InsertParam(new RParamValue("DocumentsStructTolerance",40),"Indexer");
	config->InsertParam(new RParamValue("DocumentsStructCache",20),"Indexer");
	config->InsertParam(new RParamValue("DocumentsStructType",RBlockFile::WriteBack),"Indexer");
}


//------------------------------------------------------------------------------
void GIndexer::Apply(RConfig* config)
{
	// Create (if necessary) the directory corresponding to the name of the session
	// Create all the index files
	RString Dir(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Storage->GetWorld()+RFile::GetDirSeparator());
	try
	{
		RDir::CreateDirIfNecessary(Dir,true);

		// Parse all types
		RCursor<IndexType> Type(Types);
		for(Type.Start();!Type.End();Type.Next())
		{
			Type()->IndexInc=config->GetBool("Index"+Type()->Name+"Inc","Indexer");
			Type()->Desc=new RIndexFile(Dir+Type()->Name+".desc",
					config->GetUInt(Type()->Name+"DescBlock","Indexer"),
					config->GetUInt(Type()->Name+"DescCache","Indexer"),
					config->GetUInt(Type()->Name+"DescTolerance","Indexer"));
			Type()->Desc->Open();
			Type()->Desc->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt(Type()->Name+"DescType","Indexer")));
			Type()->Index=new RIndexFile(Dir+Type()->Name+".index",
					config->GetUInt(Type()->Name+"IndexBlock","Indexer"),
					config->GetUInt(Type()->Name+"IndexCache","Indexer"),
					config->GetUInt(Type()->Name+"IndexTolerance","Indexer"));
			Type()->Index->Open();
			Type()->Index->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt(Type()->Name+"IndexType","Indexer")));
		}

		StructDoc=new RIndexFile(Dir+"Documents.struct",
				config->GetUInt("DocsStructBlock","Indexer"),
				config->GetUInt("DocsStructCache","Indexer"),
				config->GetUInt("DocsStructTolerance","Indexer"));
		StructDoc->Open();
		StructDoc->SetCacheType(static_cast<RBlockFile::CacheType>(config->GetInt("DocsStructType","Indexer")));
	}
	catch(...)
	{
		cerr<<"GIndexer::Apply: Problems in creating directories in '"<<Dir<<"'"<<endl;
	}
}

//------------------------------------------------------------------------------
void GIndexer::ClearStruct(void)
{
	StructDoc->Clear();
}


//------------------------------------------------------------------------------
void GIndexer::ClearDesc(tObjType objtype)
{
	IndexType* ptr(Types.GetPtr(objtype));
	if(!ptr)
		ThrowGException(GetObjType(objtype,true,true)+" do not have descriptions");
	ptr->Desc->Clear();
}


//------------------------------------------------------------------------------
void GIndexer::ClearIndex(tObjType objtype)
{
	IndexType* ptr(Types.GetPtr(objtype));
	if(!ptr)
		ThrowGException(GetObjType(objtype,true,true)+" do not have index");
	ptr->Index->Clear();
}


//------------------------------------------------------------------------------
void GIndexer::FlushDesc(tObjType objtype)
{
	IndexType* ptr(Types.GetPtr(objtype));
	if(!ptr)
		ThrowGException(GetObjType(objtype,true,true)+" do not have descriptions");
	ptr->Desc->Flush();
}


//------------------------------------------------------------------------------
void GIndexer::LoadInfos(GWeightInfos* &infos,tObjType type,size_t blockid,size_t id)
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
void GIndexer::SaveInfos(const GWeightInfos& infos,tObjType type,size_t& blockid,size_t id)
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
void GIndexer::LoadRefs(GConcept* concept,RNumContainer<size_t,true>& refs,tObjType type)
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
void GIndexer::BuildRefs(tObjType type,GSlot* slot)
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
GIndexer::~GIndexer(void)
{
	delete StructDoc;
}
