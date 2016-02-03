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
#include <gqueryres.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <genginexml.h>



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
	  GEngine(session,fac), NbResults(40), Trees(1000), MaxNbTrees(1000), DocIds(5000), Results(30), Recs(10000,5000)
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
	InsertParam(new RParamValue("MaxNbTrees",100,"Maximum number of description to hold in memory (>=0)."));
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
	MaxNbTrees=FindParam<RParamValue>("MaxNbTrees")->GetUInt()*1000;
	if((MaxNbTrees<1000)||(MaxNbTrees>1000000))
		MaxNbTrees=1000;
}


//------------------------------------------------------------------------------
void GEngineXML::PerformRequest(GSearchQuery* query,size_t caller)
{
	// Make all the loaded trees unused
	RCursor<cTreeRef> Tree(Trees);
	for(Tree.Start();!Tree.End();Tree.Next())
		Tree()->Use=false;

	// Suppose no records are used
	NbRecsUsed=0;

	// Retrieve all relevant nodes and get the results
	Results.Clear();
	Query=query;
	Perform(0,caller);

	// Verify that there are some results
	if(!Results.GetNb())
		return;
	const GQueryRes* Res(Results());
	 if((!Res)||(!Res->GetNb()))
		 return;
	 if(Debug)
		Res->Print();

	RCursor<GDocRef> Doc(Res->GetDocs());
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		// Determine Min and Max
		RCursor<GDocFragment> Fragment(Doc()->GetFragments());
		for(Fragment.Start();!Fragment.End();Fragment.Next())
			AddResult(Fragment()->GetDoc(),
					  Fragment()->GetRecord(),
					  Fragment()->GetPos(),
					  Fragment()->GetSyntacticPos(),
					  Fragment()->GetBegin(),
					  Fragment()->GetEnd(),
					  0.0,caller);
	}
}


//------------------------------------------------------------------------------
void GEngineXML::FindOccurrences(GConcept* concept)
{
	// If no valid concept -> push an empty result
	if(!concept)
	{
		Results.Push(new GQueryRes());
		return;
	}

	// Find all documents containing the concept
	DocIds.Clear();
	GetSession()->LoadIndex(pDoc,concept,DocIds);
//	cout<<"Documents "<<DocIds.GetNb()<<endl;

	// If no results -> push an empty result
	if(!DocIds.GetNb())
	{
		Results.Push(new GQueryRes());
		return;
	}

	// Build a GQueryRes, reduce it and push it on the stack
	GQueryRes* Res(new GQueryRes(DocIds.GetNb()));
	RNumCursor<size_t> Cur(DocIds);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GDoc* Doc(GetSession()->GetObj(pDoc,Cur()));
		if(Doc)
			Res->AddDoc(this,Doc,concept);
	}

	// Reduce the result and push it
	Res->Reduce();
	Results.Push(Res);
}


//------------------------------------------------------------------------------
void GEngineXML::ApplyOperator(GSearchQueryNode::tOperator op,GDocRef* left,GDocRef* right,GDocRef* res)
{
	// If simple presence -> copy all nodes
	if(op==GSearchQueryNode::oOR)
	{
		// Add the concept nodes of left not in res
		res->CopyFragments(left);

		// OR -> Add the concept nodes of right not in res
		res->CopyFragments(right);

		// Reduce the result
		res->Reduce(false);
		return;
	}
	else if(op==GSearchQueryNode::oAND)
	{
		if((res->GetNbFragments())||(left->GetDoc()->GetId()!=right->GetDoc()->GetId()))
			mThrowGException("Big Problem");

		// Tree of both nodes
		//const GConceptTree* Tree(GetTree(left->GetDoc()->GetId()));

		// For each pair of nodes of both query -> find the most top node containing both
		RCursor<GDocFragment> Fragment(left->GetFragments());
		RCursor<GDocFragment> Fragment2(right->GetFragments());
		for(Fragment.Start();!Fragment.End();Fragment.Next())
		{
			for(Fragment2.Start();!Fragment2.End();Fragment2.Next())
			{
				GDocFragment* Res;

				// Treat differently the case of flat documents and structured ones
				if((Fragment()->IsFlat())&&(Fragment2()->IsFlat()))
				{
					// No fragment have a parent -> Look if they overlap
					if(!Fragment()->Overlap(Fragment2()))
						continue;

					// The position is in the middle of both fragments
					size_t SyntacticPos;
					if(Fragment()->GetSyntacticPos()<Fragment2()->GetSyntacticPos())
						SyntacticPos=Fragment()->GetSyntacticPos()+((Fragment2()->GetSyntacticPos()-Fragment()->GetSyntacticPos())/2);
					else
						SyntacticPos=Fragment2()->GetSyntacticPos()+((Fragment()->GetSyntacticPos()-Fragment2()->GetSyntacticPos())/2);
					GConceptRecord Node;
					if(!Session->FindNearestRecord(left->GetDoc(),Node,SyntacticPos))
						mThrowGException("Big Problem");

					// Create a new fragment
					bool Exist;
					Res=res->AddFragment(0,
												Node.GetPos(),
											   Node.GetSyntacticPos(),
												Session->GetMinPosRecord(left->GetDoc(),Node,BeginWindowPos),
												Session->GetMaxPosRecord(left->GetDoc(),Node,EndWindowPos),
												Exist);
				}
				else
				{
					// Both fragments must have a concept node
					if(Fragment()->IsFlat()||Fragment2()->IsFlat())
						continue;

					// At least one fragment has a parent -> Find the root of both nodes
					GConceptRecord Root;
					if(!Session->FindRootRecord(left->GetDoc(),*Fragment()->GetRecord(),*Fragment2()->GetRecord(),Root))
						continue;

					// Create a new fragment
					bool Exist;
					Res=res->AddFragment(GetRec(Root),Root.GetPos(),Root.GetSyntacticPos(),Root.GetPos(),Root.GetPos(),Exist);
				}

				// Copy both children of Node() and Node2() in Res
				RCursor<const GConceptRecord> Cur(Fragment()->GetChildren());
				for(Cur.Start();!Cur.End();Cur.Next())
					Res->AddChild(Cur());
				RCursor<const GConceptRecord> Cur2(Fragment2()->GetChildren());
				for(Cur2.Start();!Cur2.End();Cur2.Next())
					Res->AddChild(Cur2());
			}
		}

		// Reduce the result
		res->Reduce(true);
		return;
	}

	// If not OR -> Compare each pair of occurrences
	RCursor<GDocFragment> Left(left->GetFragments());
	for(Left.Start();!Left.End();Left.Next())
	{
		bool Find;
		switch(op)
		{
			case GSearchQueryNode::oSIB:
				// Left() is OK if they a sibling node in Right() exists
				if(right->FindSibling(Left()))
					res->CopyFragment(Left(),Find);
				break;

			case GSearchQueryNode::oNSIB:
				// Left() is OK if they a sibling node in Right() doesn't exist
				if(!right->FindSibling(Left()))
					res->CopyFragment(Left(),Find);
				break;

			case GSearchQueryNode::oINC:
				// Left() is OK if they a child node in Right() exist
				if(right->FindChild(Left()))
					res->CopyFragment(Left(),Find);
				break;

			case GSearchQueryNode::oNINC:
				// Left() is OK if they a child node in Right() doesn't exist
				if(!right->FindChild(Left()))
					res->CopyFragment(Left(),Find);
				break;

			default:
				break;
		}
	}

	// Reduce the result
	res->Reduce(false);
}


//------------------------------------------------------------------------------
void GEngineXML::ApplyOperator(GSearchQueryNode::tOperator op)
{
	// Pop the two operands from the stack
	GQueryRes* RightOperand(Results());
	Results.Pop();
	GQueryRes* LeftOperand(Results());
	Results.Pop();

	// Create a result and push it on the stack
	GQueryRes* Res(new GQueryRes(RightOperand->GetNb()+LeftOperand->GetNb()));
	Results.Push(Res);

	// Parse the two operands
	RCursor<GDocRef> Left(LeftOperand->GetDocs());
	RCursor<GDocRef> Right(RightOperand->GetDocs());
	for(Left.Start(),Right.Start();(!Left.End()&&(!Right.End()));)
	{
		if(Left()->GetDoc()->GetId()==Right()->GetDoc()->GetId())
		{
			// It is the same document -> Merge the two lists
			GDocRef* New(new GDocRef(Left()->GetDoc()));

			// If NAND: Since the second list is not empty -> nothing to do
			if(op!=GSearchQueryNode::oNAND)
			{
				ApplyOperator(op,Left(),Right(),New);

				// Add the results if not empty
				if(New->GetNbFragments())
					Res->InsertDoc(New);
				else
					delete New;
			}


			// Next element in both lists
			Left.Next();
			Right.Next();
		}
		else
		{
			// Look for the operand with the minimum document identifier
			RCursor<GDocRef>* Min;
			if(Left()->GetDoc()->GetId()>Right()->GetDoc()->GetId())
				Min=&Right;
			else
				Min=&Left;

			// If the operand is a OR or NAND -> Add the it
			switch(op)
			{
				case GSearchQueryNode::oNAND:
					if(Min==&Right)
						break;
				case GSearchQueryNode::oOR:
					Res->InsertDoc(new GDocRef(*(*Min)()));
					break;
				default:
					break;
			}

			 // Go to the next document.
			Min->Next();
		}
	}

	RCursor<GDocRef>* Finish;
	if(Left.End())
			Finish=&Right;
		else
			Finish=&Left;
	for(;!Finish->End();Finish->Next())
	{
		// If the operand is a OR or NAND -> Add the it
		switch(op)
		{
			case GSearchQueryNode::oNAND:
				if(Finish==&Right)
					break;
			case GSearchQueryNode::oOR:
				Res->InsertDoc(new GDocRef(*(*Finish)()));
				break;
			default:
				break;
		}
	}

	// Delete the operands
	delete LeftOperand;
	delete RightOperand;
}


//------------------------------------------------------------------------------
void GEngineXML::Perform(GSearchQueryNode* node,size_t caller)
{
	if(!node)
		node=Query->GetTop();
	if(!node)
		return;

	switch(node->GetType())
	{
		case GSearchQueryNode::nToken:
			FindOccurrences(node->GetToken()->GetConcept());
			break;
		case GSearchQueryNode::nOperator:
			Perform(node->GetFirst(),caller); // Left operand
			if((node->GetOperator()==GSearchQueryNode::oOR)||(Results()->GetNb()))
			{
				Perform(node->GetLast(),caller); // Right operand
				ApplyOperator(node->GetOperator());
			}
			break;
	}

	if((node==Query->GetTop())&&(Results.GetNb()))
		Results()->Reduce();
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
//const GConceptTree* GEngineXML::GetTree2(size_t docid)
//{
//	// Look if the tree is already loaded
//	cTreeRef* Tree(Trees.GetPtr(docid));
//	if(Tree)
//	{
//		Tree->NbAccess++;
//		return(Tree->Tree);
//	}
//
//	// Look if the container is full and remove the less used structure
//	if(Trees.GetNb()>=MaxNbTrees)
//	{
//		// Cache is full -> The block must replace another one
//		// Select the less 10% used blocks in cache
//		Trees.ReOrder(sortOrderAccess);
//
//		// Find the first tree not used
///*		RCursor<cTreeRef> Cur(Trees);
//		for(Cur.Start();!Cur.End();Cur.Next())
//			if(!Cur()->Use)
//				break;
//
//		if(Cur.End())
//		{
//			// Must increase the container
//			//Trees.InsertPtr(Tree=new cTreeRef(docid));
//			mThrowGException("Memory allocated ("+RString::Number(MaxNbTrees)+") from the trees is full");
//		}
//		else
//		{
//			Tree=Cur();
//			Cur()->NbAccess=1;
//			Cur()->DocId=docid;
//		}*/
//		Tree=Trees[Trees.GetNb()-1];
//		Tree->NbAccess=1;
//		Tree->DocId=docid;
//
//		// Since a identifier was replaced, Cache must be re-ordered by identifiers
//		Trees.ReOrder();
//	}
//	else
//		Trees.InsertPtr(Tree=new cTreeRef(docid));
//
//	// Load the tree
//	Session->GetObj(pDoc,docid)->LoadTree(Tree->Tree);
//
//	return(Tree->Tree);
//}


//------------------------------------------------------------------------------
GConceptRecord* GEngineXML::GetRec(GConceptRecord& rec)
{
	if(NbRecsUsed>=Recs.GetNb())
	{
		size_t NewSize(Recs.GetNb()+5000);
		Recs.VerifyTab(NewSize);
		while(Recs.GetNb()<NewSize)
			Recs.InsertPtr(new GConceptRecord());
	}
	GConceptRecord* Rec(Recs[NbRecsUsed]);
	NbRecsUsed++;
	(*Rec)=rec;
	return(Rec);
}



//------------------------------------------------------------------------------
GEngineXML::~GEngineXML(void)
{
}



//------------------------------------------------------------------------------
CREATE_ENGINE_FACTORY("Simple Document Fragment Selection Engine","Simple Document Fragment Selection Engine",GEngineXML)
