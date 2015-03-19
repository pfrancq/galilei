/*

	GALILEI Research Project

	GEngineXML.cpp

	XML Search Engine - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gsession.h>
#include <rpromcriterion.h>
#include <gdoc.h>
#include <gdocanalyze.h>
#include <rbinaryfile.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <genginexml.h>
#include <gquery.h>



//------------------------------------------------------------------------------
// Constant
const bool Debug=false;



//------------------------------------------------------------------------------
//
// class GEngineXML::cTreeRef
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GEngineXML::cTreeRef
{
public:
	GConceptTree* Tree;
	size_t DocId;
	size_t NbAccess;
	bool Use;

	cTreeRef(size_t docid)
		: Tree(0), DocId(docid), NbAccess(1), Use(true)
	{

	}

	int Compare(const cTreeRef& tree) const {return(CompareIds(DocId,tree.DocId));}
	int Compare(size_t docid) const {return(CompareIds(DocId,docid));}
	~cTreeRef(void)
	{
		delete Tree;
	}
};



//------------------------------------------------------------------------------
//
// class GEngineXML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngineXML::GEngineXML(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GEngine(session,fac),
	  NbResults(40), Trees(1000), Query(0)
{
}


//------------------------------------------------------------------------------
void GEngineXML::CreateConfig(void)
{
	GEngine::CreateConfig();
	InsertParam(new RParamValue("NbResults",40,"Number of chunks to retrieve."));
	InsertParam(new RParamValue("Weight",1.0,"Weight of the engine."));
	InsertParam(new RParamValue("OnlyDocs",false,"Retrieve only documents?"));
	InsertParam(new RParamValue("BeginWindowPos",30,"Beginning synaptic position of a window (<=0)."));
	InsertParam(new RParamValue("EndWindowPos",30,"Ending synaptic position of a window (>=0)."));
}


//------------------------------------------------------------------------------
void GEngineXML::ApplyConfig()
{
	GEngine::ApplyConfig();
	NbResults=FindParam<RParamValue>("NbResults")->GetUInt();
	Weight=FindParam<RParamValue>("Weight")->GetDouble();
	OnlyDocs=FindParam<RParamValue>("OnlyDocs")->GetBool();
	BeginWindowPos=FindParam<RParamValue>("BeginWindowPos")->GetUInt();
	EndWindowPos=FindParam<RParamValue>("EndWindowPos")->GetUInt();
}


//------------------------------------------------------------------------------
void GEngineXML::PerformRequest(const RString& query)
{
	// Make all the loaded trees unused
	RCursor<cTreeRef> Tree(Trees);
	for(Tree.Start();!Tree.End();Tree.Next())
		Tree()->Use=false;

	 // Construct the query
	if(Query)
	{
		delete Query;
		Query=0;
	}

	Query=new GQuery(this,query);
	if(Debug)
		Query->Print(0);

	 // Retrieve all relevant nodes and get the results
	 Query->Perform(0);
	 const GQueryRes* Res(Query->GetResult());
	 if((!Res)||(!Res->GetNb()))
		 return;
	 if(Debug)
		Res->Print();

	 if(OnlyDocs)
	 {
		RCursor<GDocRef> Doc(Res->GetDocs());
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			// Determine Min and Max
			size_t Min(cNoRef), Max(0);
			RCursor<GDocFragment> Fragment(Doc()->GetFragments());
			for(Fragment.Start();!Fragment.End();Fragment.Next())
			{
				if(Min>Fragment()->GetBegin())
					Min=Fragment()->GetBegin();
				if(Max<Fragment()->GetEnd())
					Max=Fragment()->GetEnd();
			}

			AddResult(Doc()->GetDoc()->GetId(),Fragment()->GetNode(),0,0,Min,Max,0.0);
		}
	 }
	 else
	 {
		RCursor<GDocRef> Doc(Res->GetDocs());
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			// Determine Min and Max
			RCursor<GDocFragment> Fragment(Doc()->GetFragments());
			for(Fragment.Start();!Fragment.End();Fragment.Next())
				AddResult(Fragment()->GetDoc(),
					 Fragment()->GetNode(),
					 Fragment()->GetPos(),
					 Fragment()->GetSyntacticPos(),
					 Fragment()->GetBegin(),
					 Fragment()->GetEnd(),
					 0.0);
		}
	}
}


//------------------------------------------------------------------------------
int GEngineXML::sortOrderAccess(const void* a,const void* b)
{
	size_t af((*((cTreeRef**)(a)))->NbAccess);
	size_t bf((*((cTreeRef**)(b)))->NbAccess);

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
const GConceptTree* GEngineXML::GetTree(size_t docid)
{
	// Look if the tree is already loaded
	cTreeRef* Tree(Trees.GetPtr(docid));
	if(Tree)
	{
		Tree->NbAccess++;
		return(Tree->Tree);
	}

	// Look if the container is full and remove the less used structure
	if(Trees.GetNb()==Trees.GetMaxNb())
	{
		// Cache is full -> The block must replace another one
		// Select the less used block in cache
		Trees.ReOrder(sortOrderAccess);

		// Find the first tree not used
		RCursor<cTreeRef> Cur(Trees);
		for(Cur.Start();!Cur.End();Cur.Next())
			if(!Cur()->Use)
				break;

		if(Cur.End())
		{
			// Must increase the container
			Trees.InsertPtr(Tree=new cTreeRef(docid));
		}
		else
		{
			Tree=Cur();
			Cur()->NbAccess=1;
			Cur()->DocId=docid;
		}

		// Since a identifier was replaced, Cache must be re-ordered by identifiers
		Trees.ReOrder();
	}
	else
		Trees.InsertPtr(Tree=new cTreeRef(docid));

	// Load the tree
	Session->GetObj(pDoc,docid)->LoadTree(Tree->Tree);

	return(Tree->Tree);
}


//------------------------------------------------------------------------------
GEngineXML::~GEngineXML(void)
{
	if(Query)
	{
		delete Query;
		Query=0;
	}
}



//------------------------------------------------------------------------------
CREATE_ENGINE_FACTORY("Simple Document Fragment Selection Engine","Simple Document Fragment Selection Engine",GEngineXML)
