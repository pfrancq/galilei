/*

	GALILEI Research Project

	GObjects.h

	Object List - Inline implementation.

	Copyright 2011 by Pascal Francq (pascal@francq.info).

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
// Const
const size_t SizeRecDesc=sizeof(size_t)+sizeof(double);
const size_t SizeRecStruct=sizeof(size_t)+sizeof(char)+sizeof(size_t)+sizeof(char)+sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);


//-----------------------------------------------------------------------------
//
// class GObjects
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	GObjects<C>::GObjects(size_t size,const R::RString& name,tObjType type)
		: R::RObjectContainer<C,true>(1,size), ObjName(name), Type(type), IndexType(itNoIndex),
		  Desc(0), Index(0), Struct(0), Occurs(0), tmpRefs(size/2),Loaded(false), MaxObjs(0)
{
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::Init(R::RConfig* config)
{
	if(C::HasDesc())
	{
		// Descriptions
		config->InsertParam(new R::RParamValue("BlockSize",1024),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("Tolerance",10),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Description");

		// Index
		config->InsertParam(new R::RParamValue("IndexType",0),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("BlockSize",1024),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("Tolerance",10),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Index");
      config->InsertParam(new R::RParamValue("Increment",false),"Indexer",ObjName,"Index");

		// Occurs
		config->InsertParam(new R::RParamValue("BlockSize",1024),"Indexer",ObjName,"Occurs");
		config->InsertParam(new R::RParamValue("Tolerance",10),"Indexer",ObjName,"Occurs");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Occurs");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Occurs");
	}

	if(C::HasStruct())
	{
		config->InsertParam(new R::RParamValue("Struct",false),"Indexer",ObjName,"Structure");
		config->InsertParam(new R::RParamValue("BlockSize",4096),"Indexer",ObjName,"Structure");
		config->InsertParam(new R::RParamValue("Tolerance",40),"Indexer",ObjName,"Structure");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Structure");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Structure");
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void GObjects<C>::OpenFiles(R::RConfig* config,R::RString subdir)
{
	R::RString Dir(GALILEIApp->GetIndexDir()+R::RFile::GetDirSeparator()+subdir+R::RFile::GetDirSeparator());
	try
	{
		if(C::HasDesc())
		{
			R::RDir::CreateDirIfNecessary(Dir,true);

			// Create the description file
			Desc=new R::RIndexFile(Dir+ObjName+".desc",
					config->GetUInt("BlockSize","Indexer",ObjName,"Description"),
					config->GetUInt("CacheSize","Indexer",ObjName,"Description"),
					config->GetUInt("Tolerance","Indexer",ObjName,"Description"));
			Desc->Open();
			Desc->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer",ObjName,"Description")));

			// Look if the index must be created
			switch(config->GetInt("IndexType","Indexer",ObjName,"Index"))
			{
				case 0: IndexType=itNoIndex; break;
				case 1: IndexType=itIndex; break;
				case 2: if(Type==otDoc)
							IndexType=itFullIndex;
						else
							IndexType=itIndex;
						break;
				default:
					ThrowGException("Invalid index type for "+GetObjType(Type,true,true));
			}
			if(IndexType!=itNoIndex)
			{
				Index=new R::RIndexFile(Dir+ObjName+".index",
						config->GetUInt("BlockSize","Indexer",ObjName,"Index"),
						config->GetUInt("CacheSize","Indexer",ObjName,"Index"),
						config->GetUInt("Tolerance","Indexer",ObjName,"Index"));
				Index->Open();
				Index->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer",ObjName,"Index")));
			}
		}
		else
			IndexType=itNoIndex;

		// Look if the structure file must be created
		bool ObjStruct(false);
		if(C::HasStruct())
		{
			ObjStruct=config->GetBool("Struct","Indexer",ObjName,"Structure");
			if(ObjStruct)
			{
				Struct=new R::RIndexFile(Dir+"Documents.struct",
						config->GetUInt("BlockSize","Indexer",ObjName,"Structure"),
						config->GetUInt("CacheSize","Indexer",ObjName,"Structure"),
						config->GetUInt("Tolerance","Indexer",ObjName,"Structure"));
				Struct->Open();
				Struct->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer","Documents","Structure")));
			}
		}

		// Look if the occurrences file must be created
		if((IndexType==itFullIndex)||ObjStruct)
		{
			Occurs=new R::RIndexFile(Dir+ObjName+".occurs",
					config->GetUInt("BlockSize","Indexer",ObjName,"Occurs"),
					config->GetUInt("CacheSize","Indexer",ObjName,"Occurs"),
					config->GetUInt("Tolerance","Indexer",ObjName,"Occurs"));
			Occurs->Open();
			Occurs->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer",ObjName,"Occurs")));
		}
	}
	catch(...)
	{
		std::cerr<<"GObjects<C>::Apply: Problems in creating directories in '"<<Dir<<"'"<<std::endl;
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void GObjects<C>::Clear(const C*)
{
	R::RObjectContainer<C,true>::Clear();
}


//-----------------------------------------------------------------------------
template<class C>
   bool GObjects<C>::HasIndex(const C*) const
{
    return(Index);
}


//-----------------------------------------------------------------------------
template<class C>
   bool GObjects<C>::HasStruct(const C*) const
{
    return(Struct);
}


//-----------------------------------------------------------------------------
template<class C>
	size_t GObjects<C>::GetNbObjs(const C*) const
{
	return(R::RObjectContainer<C,true>::GetNbObjs());
}


//-----------------------------------------------------------------------------
template<class C>
	size_t GObjects<C>::GetMaxObjId(const C*) const
{
	return(R::RObjectContainer<C,true>::GetMaxObjId());
}


//-----------------------------------------------------------------------------
template<class C>
	R::RCursor<C> GObjects<C>::GetObjs(const C*) const
{
	return(R::RObjectContainer<C,true>::GetObjs(1));
}


//-----------------------------------------------------------------------------
template<class C>
	size_t GObjects<C>::GetObjs(C** &tab,bool alloc)
{
	return(R::RObjectContainer<C,true>::GetObjs(tab,alloc,1));
}


//-------------------------------------------------------------------------------
template<class C>
	C* GObjects<C>::GetObj(const C*,size_t id,bool load,bool null)
{
	C* Obj(R::RObjectContainer<C,true>::GetObj(id));
	if(Obj)
		return(Obj);

	// Look, if necessary, in the database
	if(load&&(!Storage->IsAllInMemory()))
	{
		// Load it and put in the list
		Storage->LoadObj(Obj,id);
		if(Obj)
			InsertObj(Obj);
	}

	// Manage an non-existing object
	if(!Obj)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown "+GetObjType(Type,false,false)+" "+R::RString::Number(id));
	}
	return(Obj);
}


//-------------------------------------------------------------------------------
template<class C>
	C* GObjects<C>::GetObj(const C*,const R::RString& name,bool load,bool null)
{
	C* Obj(R::RObjectContainer<C,true>::GetObj(name));

	if(load&&(!Storage->IsAllInMemory()))
	{
		// Load it and put in the list
		C* Obj(0);
		Storage->LoadObj(Obj,name);
		if(Obj)
			InsertObj(Obj);
	}

	if(!Obj)
	{
		if(null)
			return(0);
		else
			ThrowGException("Unknown "+GetObjType(Type,false,false)+" '"+name+"'");
	}
	return(Obj);
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::AssignId(C* obj)
{
	// If all groups are not in memory -> use the database
	if(!Storage->IsAllInMemory())
	{
		Storage->AssignId(obj);
		return;
	}

	R::RObjectContainer<C,true>::AssignId(obj);
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::LoadObjs(const C* obj)
{
	if(Loaded)
		return;
	Storage->LoadObjs(obj);
	Loaded=true;
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::LoadDesc(const C*,R::RContainer<GVector,true,true>* &vectors,size_t blockid,size_t id)
{
	if(!Desc)
		ThrowGException(GetObjType(Type,true,true)+" do not have descriptions");

	try
	{
		size_t nb,size,concept;
		double weight;

		// Position the block file to the correct position
		Desc->Seek(blockid,id);

		// Read the number of vectors, create the container or clear it
		Desc->Read((char*)&nb,sizeof(size_t));
		if(!vectors)
			vectors=new R::RContainer<GVector,true,true>(nb);
		else
			vectors->Clear(nb);

		// Create and read each vector
		for(size_t i=0;i<nb;i++)
		{
			// Read the concept of the vector and the number of elements
			Desc->Read((char*)&concept,sizeof(size_t));
			Desc->Read((char*)&size,sizeof(size_t));

			// Create the corresponding vector and read it
			GVector* Vector(new GVector(GetConcept(concept),size));
			vectors->InsertPtrAt(Vector,i,false);

			// Load the vector
			for(size_t j=0;j<size;j++)
			{
				Desc->Read((char*)&concept,sizeof(size_t));
				Desc->Read((char*)&weight,sizeof(double));
				Vector->InsertPtrAt(new GConceptRef(GetConcept(concept),weight),j,false);
			}
		}
	}
	catch(R::RIOException e)
	{
		ThrowGException(e.GetMsg());
	}
	catch(std::bad_alloc)
	{
		ThrowGException("Cannot create a vector for "+GetObjType(Type,false,false)+" "+R::RString::Number(id));
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::SaveDesc(const C*,const R::RContainer<GVector,true,true>& vectors,size_t& blockid,size_t id)
{
	if(!Desc)
		ThrowGException(GetObjType(Type,true,true)+" do not have descriptions");

	try
	{
		size_t nb(vectors.GetNb());

		// Compute the total number of elements to write
		size_t TotalWrite(0);
		R::RCursor<GVector> Vector(vectors);
		for(Vector.Start();!Vector.End();Vector.Next())
			TotalWrite+=Vector()->GetNb();

		// If nothing to write -> return
		if(!TotalWrite)
			return;

		// Position the block file to the correct position
		Desc->Seek(blockid,id,sizeof(size_t)+(vectors.GetNb()*SizeT2)+(TotalWrite*SizeRecDesc));

		// Write the number of vectors and then each vector
		Desc->Write((char*)&nb,sizeof(size_t));
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Write the concept of the vector and the number of elements
			size_t concept(Vector()->GetConcept()->GetId());
			size_t size(Vector()->GetNb());
			Desc->Write((char*)&concept,sizeof(size_t));
			Desc->Write((char*)&size,sizeof(size_t));

			// Write the vector
			R::RCursor<GConceptRef> Cur(Vector()->GetRefs());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				size_t concept(Cur()->GetId());
				double weight(Cur()->GetWeight());
				Desc->Write((char*)&concept,sizeof(size_t));
				Desc->Write((char*)&weight,sizeof(double));
			}
		}
	}
	catch(R::RIOException e)
	{
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::FlushDesc(const C*)
{
	if(!Desc)
		ThrowGException(GetObjType(Type,true,true)+" do not have descriptions");
	Desc->Flush();
}



//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::LoadIndex(const C*,GConcept* concept,R::RNumContainer<size_t,true>& refs)
{
	if(!Index)
		ThrowGException(GetObjType(Type,true,true)+" do not have index");

	size_t Refs(concept->GetIndex(Type));
	if(!Refs)
	{
		refs.Clear();
		return;
	}
	Index->Read(Refs,concept->Id,refs);
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::UpdateIndex(const C*,const GDescription& desc,size_t id,bool add)
{
	if(!Index)
		ThrowGException(GetObjType(Type,true,true)+" do not have index");

	// Go trough each vector
	R::RCursor<GVector> Vector(desc.GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		// Update the index for all concepts
		R::RCursor<GConceptRef> Cur(Vector()->GetRefs());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Read the vector representing the current index
			GConcept* Concept(Cur()->GetConcept());
			if(Concept->GetIndex(Type))
				Index->Read(Concept->GetIndex(Type),Concept->GetId(),GObjects<C>::tmpRefs);
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
				size_t idx(Concept->GetIndex(Type));
				Index->Write(idx,Concept->GetId(),tmpRefs);
				Concept->SetIndex(Type,idx);
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::BuildIndex(const C*)
{
	if(!Index)
		ThrowGException(GetObjType(Type,true,true)+" do not have index");

	// Clear the file and put all block identifier of concepts to 0.
	Index->Clear();
	R::RCursor<GConcept> Concepts(GetConcepts());
	for(Concepts.Start();!Concepts.End();Concepts.Next())
	{
		Concepts()->SetIndex(Type,0);

		// If no cache is asked -> Save each time
		if(Index->GetCacheType()==R::RBlockFile::WriteThrough)
		{
			size_t idx(0);
			Storage->SaveIndex(Concepts(),Type,idx);
			Concepts()->SetIndex(Type,idx);
		}
	}

	// Go trough each object
	R::RCursor<C> Cur(GObjects<C>::GetObjs(static_cast<C*>(0)));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Position the block file to the correct position and read the size
		Desc->Seek(Cur()->BlockId,Cur()->Id);
		size_t size,concept;
		double weight;
		Desc->Read((char*)&size,sizeof(size_t));
		for(size_t i=0;i<size;i++)
		{
			// Read concept identifier and weight
			Desc->Read((char*)&concept,sizeof(size_t));
			Desc->Read((char*)&weight,sizeof(double));

			// Read the vector representing the current index
			GConcept* Concept(GetConcept(concept));
			if(Concept->GetIndex(Type))
				Index->Read(Concept->GetIndex(Type),Concept->GetId(),tmpRefs);
			else
				tmpRefs.Clear();

			// Add the document to the index
			size_t oldsize(tmpRefs.GetNb());
			tmpRefs.Insert(Cur()->Id);

			// If the size of vector has changed -> Save it back
			if(oldsize!=tmpRefs.GetNb())
			{
				size_t idx(Concept->GetIndex(Type));
				Index->Write(idx,Concept->GetId(),tmpRefs);
				Concept->SetIndex(GObjects<C>::Type,idx);

				// If no cache is asked -> Save each time
				if(Index->GetCacheType()==R::RBlockFile::WriteThrough)
					Storage->SaveIndex(Concept,Type,Concept->GetIndex(Type));
			}
		}
	}

	// If cache was asked, flush the cache and save the block identifier of the concepts
	if(Index->GetCacheType()==R::RBlockFile::WriteBack)
	{
		Index->Flush();
		R::RCursor<GConcept> Concepts(GetConcepts());
		for(Concepts.Start();!Concepts.End();Concepts.Next())
			Storage->SaveIndex(Concepts(),Type,Concepts()->GetIndex(Type));
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::LoadStruct(const C*,GConceptTree* &docstruct,size_t blockid,size_t id)
{
	if(!Struct)
		ThrowGException(GetObjType(Type,true,true)+" do not have structures");

	try
	{
		// Position the file to correct block
		Struct->Seek(blockid,id);

		// Read the number of nodes and number of local caches
		size_t nbrecs,nblcs;
		Struct->Seek(blockid,id);
		Struct->Read((char*)&nbrecs,sizeof(size_t));
		Struct->Read((char*)&nblcs,sizeof(size_t));
		if(!docstruct)
			docstruct=new GConceptTree(nbrecs,nblcs);
		else
			docstruct->SetSizes(nbrecs,nblcs);

		// Read the LCs
		for(size_t i=0;i<nblcs;i++)
		{
			size_t nb;
			Struct->Read((char*)&nb,sizeof(size_t));
			docstruct->SetNbLCEntries(i,nb);
		}

		// Read the nodes
		for(size_t i=0;i<nbrecs;i++)
		{
			size_t concept,pos,nbchild;
			char depth;
			Struct->Read((char*)&concept,sizeof(size_t));
			Struct->Read((char*)&pos,sizeof(size_t));
			Struct->Read((char*)&depth,sizeof(char));
			Struct->Read((char*)&nbchild,sizeof(size_t));
			docstruct->AddNode(GetConcept(concept),pos,depth,nbchild);
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		ThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GObjects<C>::SaveStruct(const C*,GConceptTree* docstruct,size_t& blockid,size_t id)
{
	if(!Struct)
		ThrowGException(GetObjType(Type,true,true)+" do not have structures");

	try
	{
	    if(!docstruct->GetNbNodes())
			return;

		// Position the file to correct block and announce that a given number of bytes will be written
	    size_t nbrecs(docstruct->GetNbNodes()),nblcs(docstruct->GetNbLCs());
	    size_t size(SizeT2+(nblcs*sizeof(size_t))+(nbrecs*SizeRecStruct));
	    Struct->Seek(blockid,id,size);

		// Save the size of the number of nodes and the number of LCs
		Struct->Write((char*)&nbrecs,sizeof(size_t));
		Struct->Write((char*)&nblcs,sizeof(size_t));

		// Save LCs
		for(size_t i=0;i<nblcs;i++)
		{
			size_t nb(docstruct->GetNbLCEntries(i));
			Struct->Write((char*)&nb,sizeof(size_t));
		}

		// Save the nodes
		R::RCursor<GConceptNode> Recs(docstruct->GetNodes());
		for(Recs.Start();!Recs.End();Recs.Next())
		{
			size_t nb;
			char car;
			nb=Recs()->GetConcept()->GetId();
			Struct->Write((char*)&nb,sizeof(size_t));
			nb=Recs()->GetPos();
			Struct->Write((char*)&nb,sizeof(size_t));
			car=Recs()->GetDepth();
			Struct->Write((char*)&car,sizeof(char));
			nb=docstruct->GetFirstChild(Recs());
			Struct->Write((char*)&nb,sizeof(size_t));
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		ThrowGException(e.GetMsg());
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void GObjects<C>::InsertObj(C* obj,C*)
{
	// If it is a new one, it should perhaps be saved
	if(R::RObjectContainer<C,true>::InsertObj(obj))
	{
		// If new one and all documents are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveObj(obj);
		obj->Emit(GEvent::eObjCreated);
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void GObjects<C>::DeleteObj(C* obj)
{
	 R::RObjectContainer<C,true>::DeleteObj(obj);
}


//-----------------------------------------------------------------------------
template<class C>
	GObjects<C>::~GObjects(void)
{
	delete Desc;
	delete Index;
	delete Struct;
	delete Occurs;
}
