/*

	GALILEI Research Project

	GObjects.h

	Object List - Inline implementation.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
const size_t SizeRecNode=sizeof(tTokenType)+sizeof(size_t)+sizeof(size_t)+sizeof(size_t)+sizeof(size_t)+sizeof(size_t);
const size_t SizeT=sizeof(size_t);
const size_t SizeT2=2*sizeof(size_t);
const size_t SizeT3=3*sizeof(size_t);
const size_t SizeRec=sizeof(tTokenType)+5*sizeof(size_t);
const size_t SizeRec2=2*SizeRec;



//-----------------------------------------------------------------------------
//
// class GObjects
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	GObjects<C,hEvents>::GObjects(GSession* session,size_t size,const R::RString& name,tObjType type)
		: GKB(session), R::RObjectContainer<C,true>(1,size), ObjName(name), Type(type), MaxObjs(0),
		  Desc(0), Index(0), Tree(0), Loaded(false), tmpRefs(size/2)
{
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::Init(R::RConfig* config)
{
	if(C::HasDesc())
	{
		// Descriptions
		config->InsertParam(new R::RParamValue("BlockSize",1024),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("Tolerance",10),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Description");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Description");

		// Index
		config->InsertParam(new R::RParamValue("CreateIndex",false),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("BlockSize",1024),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("Tolerance",10),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Index");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Index");
	}

	if(C::HasTree())
	{
		config->InsertParam(new R::RParamValue("CreateTree",false),"Indexer",ObjName,"Tree");
		config->InsertParam(new R::RParamValue("BlockSize",4096),"Indexer",ObjName,"Tree");
		config->InsertParam(new R::RParamValue("Tolerance",40),"Indexer",ObjName,"Tree");
		config->InsertParam(new R::RParamValue("CacheSize",20),"Indexer",ObjName,"Tree");
		config->InsertParam(new R::RParamValue("Type",R::RBlockFile::WriteBack),"Indexer",ObjName,"Tree");
	}
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::Clear(const C* obj,bool del)
{
	// Clear the references and the index
	if(C::HasDesc())
		ClearIndex(C::GetType());

	// If the results must be saved -> clear the objects files
	if(SaveResults)
	{
		if(C::HasDesc())
		{
			if(Desc)
				Desc->Clear();
			if(Index)
				Index->Clear();
		}
		if(C::HasTree()&&Tree)
			Tree->Clear();
	}


	// Treat the objects
	if(del)
	{
		// Delete the objects from the container and the storage
		R::RObjectContainer<C,true>::Clear();
		if(SaveResults)
			Storage->Clear(C::GetType());
	}
	else
	{
		// Clear each object individually from its description(s)
		R::RCursor<C> Obj(GetObjs(obj));
		for(Obj.Start();!Obj.End();Obj.Next())
		{
			Obj()->Clear(SaveResults);
			if(SaveResults)
				Storage->SaveObj(Obj());
		}
	}
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::OpenFiles(R::RConfig* config,R::RString subdir)
{
	R::RString Dir(GALILEIApp->GetIndexDir()+R::RFile::GetDirSeparator()+subdir+R::RFile::GetDirSeparator());
	try
	{
		if(C::HasDesc())
		{
			R::RDir::CreateDirIfNecessary(Dir,true);

			// Create the description file
			Desc=new R::RKeyValueFile<R::RIntKey>(Dir+ObjName+".desc",
					config->GetUInt("BlockSize","Indexer",ObjName,"Description"),
					config->GetUInt("CacheSize","Indexer",ObjName,"Description"),
					config->GetUInt("Tolerance","Indexer",ObjName,"Description"));
			Desc->Open();
			Desc->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer",ObjName,"Description")));

			// Look if the index must be created
			CreateIndex=config->GetBool("CreateIndex","Indexer",ObjName,"Index");
			if(CreateIndex)
			{
				Index=new R::RKeyValueFile<R::RIntKey>(Dir+ObjName+".index",
						config->GetUInt("BlockSize","Indexer",ObjName,"Index"),
						config->GetUInt("CacheSize","Indexer",ObjName,"Index"),
						config->GetUInt("Tolerance","Indexer",ObjName,"Index"));
				Index->Open();
				Index->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer",ObjName,"Index")));
			}
		}
		else
			CreateIndex=false;

		// Look if the structure file must be created
		if(C::HasTree())
		{
			CreateTree=config->GetBool("CreateTree","Indexer",ObjName,"Tree");
			if(CreateTree)
			{
				Tree=new R::RKeyValueFile<R::RIntKey>(Dir+"Documents.tree",
						config->GetUInt("BlockSize","Indexer",ObjName,"Tree"),
						config->GetUInt("CacheSize","Indexer",ObjName,"Tree"),
						config->GetUInt("Tolerance","Indexer",ObjName,"Tree"));
				Tree->Open();
				Tree->SetCacheType(static_cast<R::RBlockFile::CacheType>(config->GetInt("Type","Indexer","Documents","Tree")));
			}
		}
		else
			CreateTree=false;
	}
	catch(GException& e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
	}
	catch(R::RException& e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
	}
	catch(...)
	{
		std::cerr<<"GObjects<C>::OpenFiles(R::RConfig*,R::RString): Problems in creating directories in '"<<Dir<<"' for "<<ObjName<<std::endl;
	}
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::DoCreateIndex(const C*) const
{
	return(CreateIndex);
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::DoCreateTree(const C*) const
{
	return(CreateTree);
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetNbObjs(const C*) const
{
	return(R::RObjectContainer<C,true>::GetNbObjs());
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMaxObjId(const C*) const
{
	return(R::RObjectContainer<C,true>::GetMaxObjId());
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMaxObjPos(const C*) const
{
	return(R::RObjectContainer<C,true>::GetMaxObjPos());
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	R::RCursor<C> GObjects<C,hEvents>::GetObjs(const C*) const
{
	return(R::RObjectContainer<C,true>::GetObjs(1));
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetObjs(C** &tab,bool alloc)
{
	return(R::RObjectContainer<C,true>::GetObjs(tab,alloc,1));
}


//-------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	C* GObjects<C,hEvents>::GetObj(const C*,size_t id,bool load,bool null)
{
	C* Obj(R::RObjectContainer<C,true>::operator[](id));
	if(Obj)
		return(Obj);

	// Look, if necessary, in the database
	if(load&&(!Storage->IsAllInMemory()))
	{
		// Load it and put in the list
		State=osOnDemand;
		Storage->LoadObj(Obj,id);
		if(Obj)
			InsertObj(Obj);
		State=osLatest;
	}

	// Manage an non-existing object
	if(!Obj)
	{
		if(null)
			return(0);
		else
			mThrowGException("Unknown "+GetObjType(Type,false,false)+" "+R::RString::Number(id));
	}
	return(Obj);
}


//-------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	C* GObjects<C,hEvents>::GetObj(const C*,const R::RString& name,bool load,bool null)
{
	C* Obj(R::RObjectContainer<C,true>::GetObj(name));

	if(load&&(!Storage->IsAllInMemory()))
	{
		// Load it and put in the list
		State=osOnDemand;
		C* Obj(0);
		Storage->LoadObj(Obj,name);
		if(Obj)
			InsertObj(Obj);
		State=osLatest;
	}

	if(!Obj)
	{
		if(null)
			return(0);
		else
			mThrowGException("Unknown "+GetObjType(Type,false,false)+" '"+name+"'");
	}
	return(Obj);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::AssignId(C* obj)
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
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::LoadObjs(const C* obj)
{
	if(Loaded)
		return;
	State=osOnDemand;
	Storage->LoadObjs(obj);
	Loaded=true;
	State=osLatest;
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::LoadDesc(const C*,R::RContainer<GVector,true,true>* &vectors,size_t blockid,size_t id)
{
	if(!Desc)
		mThrowGException(GetObjType(Type,true,true)+" do not have descriptions");

	try
	{
		size_t nb,size,concept;
		double weight;

		// Position the block file to the correct position
		R::RIntKey Key(id);
		Desc->Seek(blockid,Key);

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
			GVector* Vector(new GVector(GetObj(pConcept,concept),size));
			vectors->InsertPtrAt(Vector,i,false);

			// Load the vector
			for(size_t j=0;j<size;j++)
			{
				Desc->Read((char*)&concept,sizeof(size_t));
				Desc->Read((char*)&weight,sizeof(double));
				Vector->InsertPtrAt(new GConceptRef(GetObj(pConcept,concept),weight),j,false);
			}
		}
	}
	catch(R::RIOException e)
	{
		mThrowGException(e.GetMsg());
	}
	catch(std::bad_alloc)
	{
		mThrowGException("Cannot create a vector for "+GetObjType(Type,false,false)+" "+R::RString::Number(id));
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::SaveDesc(const C*,const R::RContainer<GVector,true,true>& vectors,size_t& blockid,size_t id)
{
	if(!Desc)
		mThrowGException(GetObjType(Type,true,true)+" do not have descriptions");

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
		R::RIntKey Key(id);
		Desc->Seek(blockid,Key,sizeof(size_t)+(vectors.GetNb()*SizeT2)+(TotalWrite*SizeRecDesc));

		// Write the number of vectors and then each vector
		Desc->Write((char*)&nb,sizeof(size_t));
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Write the concept of the vector and the number of elements
			size_t concept(Vector()->GetMetaConcept()->GetId());
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
		mThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::FlushDesc(const C*)
{
	if(!Desc)
		mThrowGException(GetObjType(Type,true,true)+" do not have descriptions");
	Desc->Flush();
}



//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::LoadIndex(const C*,GConcept* concept,R::RNumContainer<size_t,true>& refs)
{
	if(!Index)
		mThrowGException(GetObjType(Type,true,true)+" do not have index");

	size_t Refs(concept->GetIndex(Type));
	if(!Refs)
	{
		refs.Clear();
		return;
	}
	R::RIntKey Key(concept->GetId());
	Index->Read(Refs,Key,refs);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::UpdateIndex(const C*,const GDescription& desc,size_t id,bool add)
{
	if(!Index)
		mThrowGException(GetObjType(Type,true,true)+" do not have index");

	// Go trough each vector
	R::RConstCursor<GVector> Vector(desc.GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		// Update the index for all concepts
		R::RConstCursor<GConceptRef> Cur(Vector()->GetRefs());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Read the vector representing the current index
			GConcept* Concept(Cur()->GetConcept());
			size_t idx(Concept->GetIndex(Type));
			R::RIntKey Key(Concept->GetId());
			if(idx)
				Index->Read(idx,Key,tmpRefs);
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
				Index->Write(idx,Key,tmpRefs);
				Concept->SetIndex(Type,idx);
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::BuildIndex(const C*)
{
	if(!Index)
		mThrowGException(GetObjType(Type,true,true)+" do not have index");

	// Clear the file and put all block identifier of concepts to 0.
	Index->Clear();
	R::RCursor<GConcept> Concepts(GetObjs(pConcept));
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
	R::RCursor<C> Cur(GObjects<C,hEvents>::GetObjs(static_cast<C*>(0)));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Position the block file to the correct position and read the size
		R::RIntKey Key(Cur()->GetId());
		Desc->Seek(Cur()->BlockId,Key);
		size_t v,size,concept;
		double weight;
		Desc->Read((char*)&v,sizeof(size_t)); // Load number of vectors
		for(size_t i=0;i<v;i++)
		{
			Desc->Read((char*)&concept,sizeof(size_t));   // Load meta-concept identifier
			Desc->Read((char*)&size,sizeof(size_t));      // Load size of the current vector
			for(size_t j=0;j<size;j++)
			{
				// Read concept identifier and weight
				Desc->Read((char*)&concept,sizeof(size_t));
				Desc->Read((char*)&weight,sizeof(double));

				// Read the vector representing the current index
				GConcept* Concept(GetObj(pConcept,concept));
				R::RIntKey Key2(Concept->GetId());
				if(Concept->GetIndex(Type))
					Index->Read(Concept->GetIndex(Type),Key2,tmpRefs);
				else
					tmpRefs.Clear();

				// Add the document to the index
				size_t oldsize(tmpRefs.GetNb());
				tmpRefs.Insert(Cur()->Id);

				// If the size of vector has changed -> Save it back
				if(oldsize!=tmpRefs.GetNb())
				{
					size_t idx(Concept->GetIndex(Type));
					Index->Write(idx,Key2,tmpRefs);
					Concept->SetIndex(GObjects<C,hEvents>::Type,idx);

					// If no cache is asked -> Save each time
					if(Index->GetCacheType()==R::RBlockFile::WriteThrough)
						Storage->SaveIndex(Concept,Type,Concept->GetIndex(Type));
				}
			}
		}
	}

	// If cache was asked, flush the cache and save the block identifier of the concepts
	if(Index->GetCacheType()==R::RBlockFile::WriteBack)
	{
		Index->Flush();
		R::RCursor<GConcept> Concepts(GetObjs(pConcept));
		for(Concepts.Start();!Concepts.End();Concepts.Next())
			Storage->SaveIndex(Concepts(),Type,Concepts()->GetIndex(Type));
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::LoadNode(GConceptTree* tree,GConceptNode* parent)
{
	// Load the information concerning the current node
	tTokenType type;
	Tree->Read((char*)&type,sizeof(tTokenType));
	size_t children,concept,pos,synpos,depth;
	Tree->Read((char*)&concept,sizeof(size_t));
	Tree->Read((char*)&synpos,sizeof(size_t));
	Tree->Read((char*)&pos,sizeof(size_t));
	Tree->Read((char*)&depth,sizeof(size_t));
	Tree->Read((char*)&children,sizeof(size_t));

	// Create the node
	GConceptNode* Node(tree->InsertNode(parent,type,concept,synpos,pos,depth));

	// Load its children
	for(size_t i=children+1;--i;)
		LoadNode(tree,Node);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::SaveNode(GConceptNode* node)
{
	// Save the information concerning the current node
	tTokenType type(node->GetType());
	Tree->Write((char*)&type,sizeof(tTokenType));
	size_t nb;
	nb=node->GetConceptId();
	Tree->Write((char*)&nb,sizeof(size_t));
	nb=node->GetSyntacticPos();
	Tree->Write((char*)&nb,sizeof(size_t));
	nb=node->GetPos();
	Tree->Write((char*)&nb,sizeof(size_t));
	nb=node->GetSyntacticDepth();
	Tree->Write((char*)&nb,sizeof(size_t));
	nb=node->GetNbNodes();
	Tree->Write((char*)&nb,sizeof(size_t));

	// Save each of its children
	R::RNodeCursor<GConceptTree,GConceptNode> Node(node);
	for(Node.Start();!Node.End();Node.Next())
		SaveNode(Node());
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::LoadTree(const C*,GConceptTree* &tree,size_t blockid,size_t id)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(id);
		Tree->Seek(blockid,Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));
		if(!tree)
			tree=new GConceptTree(id,nbnodes,nbrefs);
		else
			tree->Verify(id,nbnodes,nbrefs);

		// Read the nodes
		for(size_t i=topnodes+1;--i;)
			LoadNode(tree,0);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::SaveTree(const C*,const GConceptTree& tree,size_t& blockid,size_t id)
{
	if(!CreateTree)
		return;

	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
	    if(!tree.GetNbNodes())
			return;

		// Position the file to the correct block and announce that a given number of bytes will be written
	    size_t size(SizeT3+(tree.GetNbNodes()*SizeRecNode));
		 R::RIntKey Key(id);
	    Tree->Seek(blockid,Key,size);

		// Save the number of nodes, references and top nodes.
		size_t nb(tree.GetNbNodes());
		Tree->Write((char*)&nb,sizeof(size_t));
		nb=tree.GetNbRefs();
		Tree->Write((char*)&nb,sizeof(size_t));
		nb=tree.GetNbTopNodes();
		Tree->Write((char*)&nb,sizeof(size_t));

		// Save the nodes
		R::RNodeCursor<GConceptTree,GConceptNode> Node(tree);
		for(Node.Start();!Node.End();Node.Next())
			SaveNode(Node());
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::FlushTree(const C*)
{
	if((!CreateTree)||(!Tree))
		return;
	Tree->Flush();
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::SeekRecord(const C* obj,size_t idx)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		if(idx>=nbnodes)
			mThrowGException("Index outside the record");

		// Skip nbrefs and topnodes before going to the right record
		Tree->SeekRel(idx*SizeRec+SizeT2);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindDepthFirstRecord(const C* obj,GConceptRecord& rec,size_t idx)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");
	if((rec.ConceptId==0)||(rec.ConceptId==R::cNoRef))
		mThrowGException("Invalid identifier for a "+GetObjType(Type,false,false));

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if((!nbnodes)||(idx>=nbnodes))
		{
			rec.Type=ttUnknown;
			rec.ConceptId=R::cNoRef;
			rec.SyntacticPos=R::cNoRef;
			rec.Pos=R::cNoRef;
			rec.SyntacticDepth=R::cNoRef;
			return(false);
		}

		// Put the internal cursor to the right index
		Tree->SeekRel(idx*SizeRec);

		// Parse each node
		for(size_t i=idx;i<nbnodes;i++)
		{
			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children,concept;
			Tree->Read((char*)&concept,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(concept==rec.ConceptId)
			{
				rec.Index=i;
				return(true);
			}
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.Type=ttUnknown;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindRecord(const C* obj,GConceptRecord& rec,size_t synpos)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if(!nbnodes)
		{
			rec.Type=ttUnknown;
			rec.ConceptId=R::cNoRef;
			rec.SyntacticPos=R::cNoRef;
			rec.Pos=R::cNoRef;
			rec.SyntacticDepth=R::cNoRef;
			return(false);
		}

		// Put the internal cursor to the right index
		size_t i;
		if(synpos<nbnodes)
		{
			i=synpos+2;
			Tree->SeekRel(synpos*SizeRec);
		}
		else
		{
			i=nbnodes+1;
			Tree->SeekRel((nbnodes-1)*SizeRec);
		}

		// Parse each node
		bool Rewind(false);  // Go to the beginning of the previous record?
		for(;--i;)
		{
			if(Rewind)
				Tree->SeekRel(-SizeRec2);
			else
				Rewind=true;

			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(synpos==rec.SyntacticPos)
			{
				rec.Index=i-1;
				return(true);
			}
			else if(synpos>rec.SyntacticPos)
				break;
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.Type=ttUnknown;
	rec.ConceptId=R::cNoRef;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindParentRecord(const C* obj,const GConceptRecord& search,GConceptRecord& rec)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	if((search.Index==0)||(search.SyntacticDepth==0))
	{
		rec.Type=ttUnknown;
		rec.ConceptId=R::cNoRef;
		rec.SyntacticPos=R::cNoRef;
		rec.Pos=R::cNoRef;
		rec.SyntacticDepth=R::cNoRef;
		return(false);
	}

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if((!nbnodes)||(search.Index>=nbnodes))
		{
			rec.Type=ttUnknown;
			rec.ConceptId=R::cNoRef;
			rec.SyntacticPos=R::cNoRef;
			rec.Pos=R::cNoRef;
			rec.SyntacticDepth=R::cNoRef;
			return(false);
		}

		// Put the internal cursor to the right index
		Tree->SeekRel((search.Index-1)*SizeRec);

		// Parse each previous node : the first that has a lower syntactic depth is the parent
		bool Rewind(false);  // Go to the beginning of the previous record?
		for(size_t i=search.Index;--i;)
		{
			if(Rewind)
				Tree->SeekRel(-SizeRec2);
			else
				Rewind=true;

			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(search.SyntacticDepth>rec.SyntacticDepth)
			{
				rec.Index=i;
				return(true);
			}
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.Type=ttUnknown;
	rec.ConceptId=R::cNoRef;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindFirstChildRecord(const C* obj,const GConceptRecord& search,GConceptRecord& rec,size_t idx)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if((!nbnodes)||(search.Index>=nbnodes-1))
		{
			rec.Type=ttUnknown;
			rec.ConceptId=R::cNoRef;
			rec.SyntacticPos=R::cNoRef;
			rec.Pos=R::cNoRef;
			rec.SyntacticDepth=R::cNoRef;
			return(false);
		}

		// Put the internal cursor to the right index
		Tree->SeekRel(search.Index*SizeRec);

		// Load the information concerning the search node
		Tree->Read((char*)&search.Type,sizeof(tTokenType));
		size_t children;
		Tree->Read((char*)&search.ConceptId,sizeof(size_t));
		Tree->Read((char*)&search.SyntacticPos,sizeof(size_t));
		Tree->Read((char*)&search.Pos,sizeof(size_t));
		Tree->Read((char*)&search.SyntacticDepth,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		if(idx>search.Index+children)
		{
			rec.Type=ttUnknown;
			rec.ConceptId=R::cNoRef;
			rec.SyntacticPos=R::cNoRef;
			rec.Pos=R::cNoRef;
			rec.SyntacticDepth=R::cNoRef;
			return(false);
		}

		// Parse each previous node : the first that has a lower syntactic depth is the parent
		for(size_t i=search.Index+1;i<nbnodes;)
		{
			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(rec.SyntacticDepth==search.SyntacticDepth+1)
			{
				if(i>=idx)
				{
					rec.Index=i;
					return(true);
				}
			}
			else
				break;
		}

	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.Type=ttUnknown;
	rec.ConceptId=R::cNoRef;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindNearestRecord(const C* obj,GConceptRecord& rec,size_t synpos)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if(!nbnodes)
			return(false);

		// Put the internal cursor to the right index
		size_t i;
		if(synpos<nbnodes)
		{
			i=synpos+2;
			Tree->SeekRel(synpos*SizeRec);
		}
		else
		{
			i=nbnodes+1;
			Tree->SeekRel((nbnodes-1)*SizeRec);
		}

		// Parse each node
		bool Rewind(false);  // Go to the beginning of the previous record?
		for(;--i;)
		{
			if(Rewind)
				Tree->SeekRel(-SizeRec2);
			else
				Rewind=true;

			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(synpos==rec.SyntacticPos)
			{
				rec.Index=i-1;
				return(true);
			}
			else if(synpos>rec.SyntacticPos)
				break;
		}

		// If we are are -> the syntactic position was not found.
		if(!i)
		{
			// First record is the correct one
			rec.Index=0;
			return(true);
		}

		// Read the next record after than
		GConceptRecord rec2;
		Tree->Read((char*)&rec2.Type,sizeof(tTokenType));
		size_t children;
		Tree->Read((char*)&rec2.ConceptId,sizeof(size_t));
		Tree->Read((char*)&rec2.SyntacticPos,sizeof(size_t));
		Tree->Read((char*)&rec2.Pos,sizeof(size_t));
		Tree->Read((char*)&rec2.SyntacticDepth,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));

		if(synpos-rec.SyntacticPos<=rec2.SyntacticPos-synpos)
		{
			// Choose the syntactic position just before the one searched
			rec.Index=i-1;
			return(true);
		}
		else
		{
			// Choose the syntactic position just next the one searched
			rec.Index=i;
			rec.Type=rec2.Type;
			rec.ConceptId=rec2.ConceptId;
			rec.SyntacticPos=rec2.SyntacticPos;
			rec.Pos=rec2.Pos;
			rec.SyntacticDepth=rec2.SyntacticDepth;
			return(true);
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.ConceptId=R::cNoRef;
	rec.Type=ttUnknown;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindNearestRecord(const C* obj,GConceptRecord& rec,size_t synpos,bool after)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));

		if(!nbnodes)
			return(false);

		// Put the internal cursor to the right index
		size_t i;
		if(synpos<nbnodes)
		{
			i=synpos+2;
			Tree->SeekRel(synpos*SizeRec);
		}
		else
		{
			i=nbnodes+1;
			Tree->SeekRel((nbnodes-1)*SizeRec);
		}

		// Parse each node
		bool Rewind(false);  // Go to the beginning of the previous record?
		for(;--i;)
		{
			if(Rewind)
				Tree->SeekRel(-SizeRec2);
			else
				Rewind=true;

			// Load the information concerning the current node
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));

			if(synpos==rec.SyntacticPos)
			{
				rec.Index=i-1;
				return(true);
			}
			else if(synpos>rec.SyntacticPos)
			{
				if(after)
					break;
				else
				{
					rec.Index=i-1;
					return(true);
				}
			}
		}

		// If we are are -> the syntactic position was not found.
		if(!i)
		{
			// First record is the correct one
			rec.Index=0;
			return(true);
		}

		// The next record must be taken
		Tree->Read((char*)&rec.Type,sizeof(tTokenType));
		size_t children;
		Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
		Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
		Tree->Read((char*)&rec.Pos,sizeof(size_t));
		Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		rec.Index=i;
		return(true);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Nothing was found
	rec.ConceptId=R::cNoRef;
	rec.Type=ttUnknown;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	bool GObjects<C,hEvents>::FindRootRecord(const C* obj,const GConceptRecord& rec1,const GConceptRecord& rec2,GConceptRecord& rec)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	if((rec1.SyntacticDepth==0)||(rec2.SyntacticDepth==0))
	{
		rec.ConceptId=R::cNoRef;
		rec.Type=ttUnknown;
		rec.SyntacticPos=R::cNoRef;
		rec.Pos=R::cNoRef;
		rec.SyntacticDepth=R::cNoRef;
		return(false);
	}
	if(rec1==rec2)
	{
		rec=rec;
		return(true);
	}

	try
	{
		// Search Method:
		// 1. Start from the highest index
		// 2. Find the first record which a syntactic depth lower than rec1 and rec2
		// 3. If both records were parsed -> it is a common root record
		// 4. If a record with a syntactic depth of 0 is found before the second record -> no common root record

		// Search the highest index (Idx)
		size_t Idx;
		if(rec1.GetIndex()>rec2.GetIndex())
			Idx=rec1.GetIndex();
		else
			Idx=rec2.GetIndex();

		// Start to search from the previous record
		Idx--;
		SeekRecord(obj,Idx);

		// Find the first record which a syntactic depth lower than rec1 and rec2
		bool Cont(true);
		while(Cont)
		{
			// Read the record
			Tree->Read((char*)&rec.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&rec.ConceptId,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&rec.Pos,sizeof(size_t));
			Tree->Read((char*)&rec.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));
			rec.Index=Idx;

			if((rec.SyntacticDepth<rec1.SyntacticDepth)&&(rec.SyntacticDepth<rec2.SyntacticDepth))
				Cont=false;
			else
			{
				Tree->SeekRel(-SizeRec2);
				Idx--;
			}
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	// Look if both records were parsed
	if((rec.Index<rec1.Index)&&(rec.Index<rec2.Index))
		return(true);

	// Nothing was found
	rec.ConceptId=R::cNoRef;
	rec.Type=ttUnknown;
	rec.SyntacticPos=R::cNoRef;
	rec.Pos=R::cNoRef;
	rec.SyntacticDepth=R::cNoRef;
	return(false);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMinPosRecord(const C* obj,const GConceptRecord& rec,size_t nbbefore)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	if(nbbefore>rec.GetSyntacticPos())
		nbbefore=0;
	else
		nbbefore=rec.GetSyntacticPos()-nbbefore;

	GConceptRecord search;
	if(!FindNearestRecord(obj,search,nbbefore,false))
		mThrowGException("Cannot find a valid position");
	return(search.GetPos());
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMaxPosRecord(const C* obj,const GConceptRecord& rec,size_t nbafter)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	nbafter+=rec.GetSyntacticPos();
	size_t Max(GetMaxSyntacticPosRecord(obj));
	if(nbafter>Max)
		nbafter=Max;
	GConceptRecord search;
	if(!FindNearestRecord(obj,search,nbafter,true))
		mThrowGException("Cannot find a valid position");
	return(search.GetPos());
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMaxPosRecord(const C* obj)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));
		if(!nbnodes)
			return(0);

		// Go to the last record
		Tree->SeekRel((nbnodes-1)*SizeRec);

		// Load the information concerning the current node
		tTokenType Type;
		Tree->Read((char*)&Type,sizeof(tTokenType));
		size_t children,pos;
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&pos,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		return(pos);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
	return(0);
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetMaxSyntacticPosRecord(const C* obj)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));
		if(!nbnodes)
			return(0);

		// Go to the last record
		Tree->SeekRel((nbnodes-1)*SizeRec);

		// Load the information concerning the current node
		tTokenType Type;
		Tree->Read((char*)&Type,sizeof(tTokenType));
		size_t children,pos;
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&pos,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		return(pos);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
	return(0);
}

//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetNbChildRecords(const C* obj,const GConceptRecord& rec)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");

	try
	{
		SeekRecord(obj,rec.GetIndex());

		// Load the information concerning the current node
		GConceptRecord read;
		Tree->Read((char*)&read.Type,sizeof(tTokenType));
		size_t children;
		Tree->Read((char*)&read.ConceptId,sizeof(size_t));
		Tree->Read((char*)&read.SyntacticPos,sizeof(size_t));
		Tree->Read((char*)&read.Pos,sizeof(size_t));
		Tree->Read((char*)&read.SyntacticDepth,sizeof(size_t));
		Tree->Read((char*)&children,sizeof(size_t));
		return(children);
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	size_t GObjects<C,hEvents>::GetNbTotalChildRecords(const C* obj,const GConceptRecord& rec)
{
	if(!Tree)
		mThrowGException(GetObjType(Type,true,true)+" do not have concept trees");
	size_t Nb(0);

	try
	{
	// Position the file to correct block
		R::RIntKey Key(obj->GetId());
		Tree->Seek(obj->GetStructId(),Key);

		// Read the number of nodes, references and top nodes.
		size_t nbnodes,nbrefs,topnodes;
		Tree->Read((char*)&nbnodes,sizeof(size_t));
		Tree->Read((char*)&nbrefs,sizeof(size_t));
		Tree->Read((char*)&topnodes,sizeof(size_t));
		if(rec.Index+1>=nbnodes)
			return(0);

		// Go to the next record after rec
		Tree->SeekRel((rec.Index+1)*SizeRec);

		// Count the number of records with a deeper syntactic depth
		for(size_t i=rec.Index+1;i<nbnodes;i++)
		{
			// Load the information concerning the current node
			GConceptRecord read;
			Tree->Read((char*)&read.Type,sizeof(tTokenType));
			size_t children;
			Tree->Read((char*)&read.ConceptId,sizeof(size_t));
			Tree->Read((char*)&read.SyntacticPos,sizeof(size_t));
			Tree->Read((char*)&read.Pos,sizeof(size_t));
			Tree->Read((char*)&read.SyntacticDepth,sizeof(size_t));
			Tree->Read((char*)&children,sizeof(size_t));
			if(read.SyntacticDepth>=rec.SyntacticDepth)
				break;

			Nb++;
		}
	}
	catch(R::RIOException e)
	{
		std::cerr<<e.GetMsg()<<std::endl;
		mThrowGException(e.GetMsg());
	}

	return(Nb);
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::InsertObj(C* obj,C*)
{
	// If it is a new one, it should perhaps be saved
	if(R::RObjectContainer<C,true>::InsertObj(obj)||(State!=osOnDemand))
	{
		// If new one and all documents are in memory -> store it in the database
		if(Storage->IsAllInMemory())
			Storage->SaveObj(obj);
		obj->PostNotification(hEvents[oeAdded]);
	}
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	void GObjects<C,hEvents>::DeleteObj(C* obj)
{
	obj->PostNotification(hEvents[oeAboutToBeDeleted]);
	R::RObjectContainer<C,true>::DeleteObj(obj);
}


//-----------------------------------------------------------------------------
template<class C,const R::hNotification* hEvents>
	GObjects<C,hEvents>::~GObjects(void)
{
	delete Desc;
	delete Index;
	delete Tree;
}
