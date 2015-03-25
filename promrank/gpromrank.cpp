/*

	GALILEI Research Project

	GPromRank.cpp

	Promethee Ranking Method - Implementation.

   Copyright 2015-2015 by Pascal Francq.
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
// include files for GALILEI
#include <gsession.h>
#include <rbinaryfile.h>
#include <gmeasure.h>
#include <gengine.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <gpromrank.h>
#include <gprom.h>


//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;




//------------------------------------------------------------------------------
//
// class cKeyword
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class cKeyword
{
public:
	size_t Id;
	size_t NbRef;
	double Iff;

	cKeyword(size_t id) : Id(id), NbRef(0), Iff(NAN) {}
	int Compare(const cKeyword& keyword) const
	{
		return(CompareIds(Id,keyword.Id));
	}
	int Compare(const size_t id) const
	{
		return(CompareIds(Id,id));
	}
};



//------------------------------------------------------------------------------
//
// class GPromRank::cIff
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GPromRank::cIff
{
public:

	size_t Id;
	size_t NbRef;
	RContainer<cKeyword,true,true> Children;

	cIff(size_t id) : Id(id), NbRef(0), Children(2000) {}
	int Compare(const cIff&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// class GPromRank::cRef
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GPromRank::cRef
{
public:
	enum tType {Text,Semantic};

	tType Type;
	RNumContainer<size_t,true> Parents;
	size_t Nb;

	cRef(tType type) : Type(type), Parents(200), Nb(0) {}

	int Compare(const cRef&) const {return(-1);}
	inline void Clear(void)
	{
		Parents.Clear();
		Nb=0;
	}
};



//------------------------------------------------------------------------------
//
// class GPromRank
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPromRank::GPromRank(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GComputeRank(session,fac),
	  TfIdf(0), Distance(0), Specificity(0), TfIff(0),
	  Iffs(10000), IffsDirty(false), TmpRefs(10000)
{
	// Document notifications
	InsertObserver(HANDLER(GPromRank::HandleAddDoc),hDocs[oeUpdated]);
	InsertObserver(HANDLER(GPromRank::HandleDelDoc),hDocs[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(GPromRank::HandleDelDoc),hDocs[oeAboutToBeDeleted]);
	InsertObserver(HANDLER(GPromRank::HandleReInit),hReInit,session);

}


//------------------------------------------------------------------------------
void GPromRank::Init(void)
{
	GComputeRank::Init();
	InsertObserver(HANDLER(GPromRank::HandleCurrentPlugIn),hCurrentPlugIn,GALILEIApp->GetManager("Measures")->GetPlugInList("Features Evaluation"));

	// Verify the size of the array
	Iffs.VerifyTab(Session->GetMaxObjId(pConcept)+1);
	SaveIffs=false;

	// Look if a binary file exists ?
	const RURI uri(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+"GPromRank.bin");
	if(!RFile::Exists(uri))
	{
		// No -> We may suppose that the Iffs must be recomputed
		IffsDirty=true;
		return;
	}

	// Yes -> Load the existing number of references
	IffsDirty=false;
	RBinaryFile File(uri);
	File.Open(RIO::Read);
	while(true)
	{
		size_t Id,Nb;
		File>>Id;
		if(!Id)  // End of file
			break;
		cIff* Ref(new cIff(Id));
		Iffs.InsertPtrAt(Ref,Id,true);
		File>>Ref->NbRef>>Nb;
		Ref->Children.VerifyTab(Nb);
		for(size_t i=0;i<Nb;i++)
		{
			File>>Id;
			cKeyword* Keyword(new cKeyword(Id));
			File>>Keyword->NbRef;
			Ref->Children.InsertPtrAt(Keyword,i,true);
		}
	}
}


//------------------------------------------------------------------------------
void GPromRank::HandleCurrentPlugIn(const R::RNotification& notification)
{
	Weighting=dynamic_cast<GMeasure*>(GetData<GPlugIn*>(notification));
}


//------------------------------------------------------------------------------
void GPromRank::Done(void)
{
	// Look if something to save
	if((!Session->MustSaveResults())||IffsDirty||(!SaveIffs))
		return;

	// Save the factors
	const RURI uri(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+"GPromRank.bin");
	RBinaryFile File(uri);
   File.Open(RIO::Create);
	RCursor<cIff> Cur(Iffs);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		File<<Cur()->Id<<Cur()->NbRef<<Cur()->Children.GetNb();
		RCursor<cKeyword> Cur2(Cur()->Children);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			File<<Cur2()->Id<<Cur2()->NbRef;
	}
	File<<size_t(0); // End of the file
	Iffs.Clear();
}


//------------------------------------------------------------------------------
void GPromRank::HandleAddDoc(const RNotification& notification)
{
	// If the references are dirty -> wait to recompute everything if needed
	if(IffsDirty)
		return;

	// Verify that the sender is a document
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc)
	{
		Iffs.VerifyTab(Session->GetMaxObjId(pConcept)+1);
		UpdateRefs(Doc->GetTree(),true);
	}
}


//------------------------------------------------------------------------------
void GPromRank::HandleDelDoc(const RNotification& notification)
{
	// If the references are dirty -> wait to recompute everything if needed
	if(IffsDirty)
		return;

	// Verify that the sender is a document
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsUsed(Doc))
	{
		Iffs.VerifyTab(Session->GetMaxObjId(pConcept)+1);
		UpdateRefs(Doc->GetTree(),false);
	}
}


//------------------------------------------------------------------------------
void GPromRank::Reset(void)
{
	// ForceReCompute IFF
	IffsDirty=true;

	// Remove the file if the results must be saved
	if(Session->MustSaveResults())
	{
		const RURI uri(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+"GPromRank.bin");
		RFile::RemoveFile(uri);
	}
}


//------------------------------------------------------------------------------
void GPromRank::HandleReInit(const R::RNotification& notification)
{
	// Manage only documents
	GSessionMsg& Msg(GetData<GSessionMsg&>(notification));
	if(Msg.GetType()!=otDoc)
		return;
	Reset();
}


//------------------------------------------------------------------------------
void GPromRank::Rank(GEngine* engine,GSearchQuery* query)
{
	if(!engine->GetNbResults())
		return;

	Weighting=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0);
	if(!Weighting)
		mThrowGException("No plug-in selected for \"Features Evaluation\"");

	Query=query;

	// Recompute References if necessary
	if(IffsDirty&&TfIff->Get<RParamValue>("Active")->GetBool())
		RecomputeRefs();


	// Create a PROMETHEE kernel and a solution for each fragment
 	GProm Prom(this,Weighting);
	RCursor<GDocFragment> Fragment(engine->GetResults());
	for(Fragment.Start();!Fragment.End();Fragment.Next())
		Prom.Add(Fragment());

	// Perform a PROMETHEE ranking on each document fragment and insert them in
	// the meta-search engine
	// Make their ranking ranging from [0,1]
	RCursor<RPromSol> Sol(Prom.Compute());
	double Min(Prom.GetMinFi());
	double Max(Prom.GetMaxFi());
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		GDocFragment* Fragment(dynamic_cast<GPromSol*>(Sol())->Fragment);
		double Ranking((Sol()->GetFi()-Min)/(Max-Min));
		Fragment->SetRanking(Ranking);
	}
}


//------------------------------------------------------------------------------
void GPromRank::CreateConfig(void)
{
	InsertParam(RPromLinearCriterion::CreateParam("TfIdf","Tf/Idf Criterion"));
	InsertParam(RPromLinearCriterion::CreateParam("Type","Type Criterion"));
	InsertParam(RPromLinearCriterion::CreateParam("Distance","Distance Criterion"));
	InsertParam(RPromLinearCriterion::CreateParam("Specificity","Specificity Criterion"));
	InsertParam(RPromLinearCriterion::CreateParam("TfIff","Tf/Iff Criterion"));

}


//-----------------------------------------------------------------------------
void GPromRank::ApplyConfig(void)
{
	TfIdf=FindParam<RParamStruct>("TfIdf");
	Type=FindParam<RParamStruct>("Type");
	Distance=FindParam<RParamStruct>("Distance");
	Specificity=FindParam<RParamStruct>("Specificity");
	TfIff=FindParam<RParamStruct>("TfIff");
}


//------------------------------------------------------------------------------
void GPromRank::BuildRefs(RNodeCursor<GConceptTree,GConceptNode>& nodes)
{
	// Go trough each node : a concept appearing can only be treated once for a same parent.
	// The TmpIds holds every identifier already treated
	for(nodes.Start();!nodes.End();nodes.Next())
	{
		// Look for the type of node
		GPromRank::cRef::tType Type;
		if( (nodes()->GetType()==ttText) ||
			 (nodes()->GetType()==ttLink) )
			Type=GPromRank::cRef::Text;
		else
			Type=GPromRank::cRef::Semantic;

		// Look for the reference
		cRef* Ref(TmpRefs.GetPtrAt(nodes()->GetConceptId()));
		if(!Ref)
		{
			Ref=new cRef(Type);
			TmpRefs.InsertPtrAt(Ref,nodes()->GetConceptId(),true);
		}

		// Treatment depends of the node type
		if(Type==GPromRank::cRef::Text)
		{
			// Text : Verify that all parents notice that the concept is presented
			GConceptNode* Parent(nodes()->GetParent());
			while(Parent)
			{
				//	If it is the first occurrence of the concept in Parent -> Save it
				bool Find;
				size_t Idx(Ref->Parents.GetIndex(Parent->GetSyntacticPos(),Find));
				if(!Find)
					Ref->Parents.InsertAt(Parent->GetSyntacticPos(),Idx,false);
				Parent=Parent->GetParent();
			}
		}
		else
			Ref->Nb++; // Simply increase the number of

		// Update the child nodes
		RNodeCursor<GConceptTree,GConceptNode> Cur(nodes());
		BuildRefs(Cur);
	}
}


//------------------------------------------------------------------------------
GPromRank::cIff* GPromRank::GetIff(size_t conceptid)
{
	cIff* Ref(Iffs.GetPtrAt(conceptid));
	if(!Ref)
	{
		Ref=new cIff(conceptid);
		Iffs.InsertPtrAt(Ref,conceptid,true);
	}
	return(Ref);
}


//------------------------------------------------------------------------------
void GPromRank::UpdateRefs(const GConceptTree* tree,bool add)
{
	// Verify the size
	TmpRefs.VerifyTab(Session->GetMaxObjId(pConcept)+1);

	// Build the nodes for the tree passed
	RNodeCursor<GConceptTree,GConceptNode> Cur(*tree);
	BuildRefs(Cur);

	// Treat the information gathered
	RCursor<cRef> TmpRef(TmpRefs);
	for(TmpRef.Start();!TmpRef.End();TmpRef.Next())
	{
		// Treat depends from the type
		if(TmpRef()->Type==GPromRank::cRef::Text)
		{
			// Update the number if references corresponding to each parent (if any)
			RNumCursor<size_t> Parent(TmpRef()->Parents);
			for(Parent.Start();!Parent.End();Parent.Next())
			{
				cIff* Ref(GetIff(tree->GetNode(Parent())->GetConceptId()));
				cKeyword* Keyword(Ref->Children.GetInsertPtr(TmpRef.GetPos()));
				if(add)
					Keyword->NbRef++;
				else
					Keyword->NbRef--;
			}
		}
		else
		{
			cIff* Ref(GetIff(TmpRef.GetPos()));
			// Update the total number of references
			if(add)
				Ref->NbRef+=TmpRef()->Nb;
			else
				Ref->NbRef-=TmpRef()->Nb;
		}

		// Clear the position for next call
		TmpRef()->Clear();
	}

	SaveIffs=true;
}


//------------------------------------------------------------------------------
void GPromRank::RecomputeRefs(void)
{
	// Clear everything
	Iffs.Clear(Session->GetMaxObjId(pConcept)+1);

	// Go trough each document and load the current structure
	GConceptTree* Tree(0);
	RCursor<GDoc> Doc(Session->GetObjs(pDoc));
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		Doc()->LoadTree(Tree);
		UpdateRefs(Tree,true);
	}
	delete Tree;

	// References are OK
	IffsDirty=false;
}


//------------------------------------------------------------------------------
double GPromRank::GetIff(size_t conceptid,size_t parentid)
{
	// First search for the concept entry
	cIff* Ref(Iffs.GetPtrAt(parentid));
	if(!Ref)
		return(0.0);

	// Find the corresponding parent
	cKeyword* Keyword(Ref->Children.GetPtr(conceptid));
	if(!Keyword)
		return(0.0);

	// If necessary -> recompute the factor
	if(Keyword->Iff!=Keyword->Iff)
	{
		if(Ref->NbRef)
			Keyword->Iff=log10(static_cast<double>(Ref->NbRef)/static_cast<double>(Keyword->NbRef));
		else
			Keyword->Iff=0.0;
	}
	return(Keyword->Iff);
}

//------------------------------------------------------------------------------
CREATE_COMPUTERANK_FACTORY("Promethee Ranking","Promethee Ranking Method",GPromRank)
