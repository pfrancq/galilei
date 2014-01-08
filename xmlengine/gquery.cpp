/*

	GALILEI Research Project

	GQuery.h

	Search Query - Implementation.

	Copyright 2004-2014 by Pascal Francq.
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



//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <gconcept.h>
#include <gsession.h>
#include <rnodecursor.h>


//------------------------------------------------------------------------------
// include files for current project
#include <gquery.h>
#include <genginexml.h>
#include <gresnodes.h>



//------------------------------------------------------------------------------
//
// class GQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GQuery::GQuery(GEngineXML* engine,const RString& query)
	: RTree<GQuery,GQueryNode,true>(), Engine(engine), Concepts(30),
	  DocIds(5000), Results(30)
{
	Set(query);
}


//------------------------------------------------------------------------------
void GQuery::Set(const RString& query)
{
	CreateToken(0,query);
}


//------------------------------------------------------------------------------
bool GQuery::CreateToken(GQueryNode* parent,const RString& str)
{
	const RChar* Car(str());
	size_t Pos(0);
	size_t Count(0);
	bool OnlyKeyword(true);
	size_t Namespaces(0);
	bool OnlyNonSpaces(false);
	bool InQuotes(false);

	// Search the position of the first operator
	while((*Car)!=0)
	{
		bool End(false);
		if(!InQuotes)
		{
			if((*Car)=='(')
				Count++;
			else if((*Car)==')')
				Count--;
			else if((*Car)==':')
					Namespaces++;
			else if((*Car)=='"')
				InQuotes=true;
		}
		else if((*Car)=='"')
			End=true;

		// The operator must be outside parentheses or quotes
		if((!Count)&&(!InQuotes)&&(GetOperator(*Car)))
			break;

		// If we are outside quotes -> Treat non-alphanumerical characters
		if((!InQuotes)&&((*Car)!=':')&&(!Car->IsAlNum()))
		{
			OnlyKeyword=false;
			if(!Car->IsSpace())
				OnlyNonSpaces=true;
		}

		if(End)
			InQuotes=false;

		// Increase position
		Pos++;
		Car++;
	}

	// Treat the result
	if(!(*Car))
	{
		if(OnlyKeyword)
		{
			if(Namespaces>1)
				mThrowGException("Multiple ':' in '"+str+"'");

			// Only one word
			if(str.IsEmpty())
				return(false);

			// Construct the keyword by removing the quotes
			RString Keyword;
			size_t NbQuotes(0);
			Car=str();
			while((*Car)!=0)
			{
				if((*Car)=='"')
					NbQuotes++;
				else
					Keyword+=(*Car);
				Car++;
			}
			if(NbQuotes>2)
				mThrowGException("Too many '\"' in '"+str+"'");

			GQueryNode* Token(NewNode(Keyword,Namespaces));
			InsertNode(parent,Token);
			if(Token->GetConcept())
				Concepts.InsertPtr(Token->GetConcept());
		}
		else
		{
			// Several words ?
			if(OnlyNonSpaces)
				mThrowGException("Invalid non-alphanumeric character '"+(*Car)+"' in '"+str+"'");

			// A space acts as a AND operator ; find first space
			Car=str();
			Pos=0;
			while(!Car->IsSpace())
			{
				Car++;
				Pos++;
			}

			// Create an AND operator
			GQueryNode* Token(NewNode(toAND));
			InsertNode(parent,Token);
			if(!CreateToken(Token,ClearString(str.Mid(0,Pos))))
				mThrowGException("Not a left expression in '"+str+"'");
			if(!CreateToken(Token,ClearString(RString(Car))))
				mThrowGException("Not a right expression in '"+str+"'");
		}
	}
	else
	{
		// Determine the operator
		tQueryOperator Op(GetOperator(*Car));
		if(Op==toNOT)
		{
			Car++; // Skip it
			if(!(*Car))
				mThrowGException("Invalid not expression '"+str+"'");
			Op=GetNotOperator(*Car);
			if(Op==toNOP)
				mThrowGException("Invalid not expression '"+str+"'");
		}
		Car++; // Skip it

		// Create the operator
		GQueryNode* Token(NewNode(Op));
		InsertNode(parent,Token);
		if(!CreateToken(Token,ClearString(str.Mid(0,Pos))))
			mThrowGException("Not a left expression in '"+str+"'");
		if(!CreateToken(Token,ClearString(RString(Car))))
			mThrowGException("Not a right expression in '"+str+"'");
	}
	return(true);
}


//------------------------------------------------------------------------------
GQueryNode* GQuery::NewNode(const RString& str,bool type)
{
	return(new GQueryNode(Engine->GetSession(),str,type));
}


//------------------------------------------------------------------------------
GQueryNode* GQuery::NewNode(tQueryOperator op)
{
	return(new GQueryNode(op));
}


//------------------------------------------------------------------------------
RString GQuery::ClearString(const RString& str)
{
	RString New(str.Trim());
	const RChar* Car(New());
	size_t Pos(0);

	if((*Car)!='(')
		return(New);

	// Search for the end of the parenthesis
	size_t Count(1);
	size_t Begin(Pos+1);
	while(((*Car)!=0)&&Count)
	{
		Car++;
		Pos++;
		if((*Car)=='(')
			Count++;
		else if((*Car)==')')
			Count--;
	}

	if(((*Car)==0)||((*Car)!=')'))
		mThrowGException("No ) Found in '"+str+"'");

	size_t Len(Pos-Begin);

	// Skip parenthesis
	Car++;

	if((*Car)!=0)
		mThrowGException("Something found after ')' in '"+str+"'");

	return(New.Mid(Begin,Len));
}


//------------------------------------------------------------------------------
void GQuery::FindOccurrences(GConcept* concept)
{
	// If no valid concept -> push an empty result
	if(!concept)
	{
		Results.Push(new GQueryRes());
		return;
	}

	// Find all documents containing the concept
	DocIds.Clear();
	Engine->GetSession()->LoadIndex(pDoc,concept,DocIds);

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
		Res->AddDoc(Engine,Cur(),concept);

	// Reduce the result and push it
	Res->Reduce();
	Results.Push(Res);
}


//------------------------------------------------------------------------------
void GQuery::ApplyOperator(tQueryOperator op,GResNodes* left,GResNodes* right,GResNodes* res)
{
	// If simple presence -> copy all nodes
	if(op==toOR)
	{
		// Add the concept nodes of left not in res
		RCursor<GResNode> Node(left->GetNodes());
		for(Node.Start();!Node.End();Node.Next())
		{
			bool Find;
			size_t Idx(res->GetIndex(*Node(),Find));
			if(!Find)
				res->InsertPtrAt(Node(),Idx);
			// else sum the distances.
		}

		// OR -> Add the concept nodes of right not in res
		Node=right->GetNodes();
		for(Node.Start();!Node.End();Node.Next())
		{
			bool Find;
			size_t Idx(res->GetIndex(*Node(),Find));
			if(!Find)
				res->InsertPtrAt(Node(),Idx);
			// else sum the distances.
		}

		// Reduce the result
		res->Reduce(false);
		return;
	}
	else if(op==toAND)
	{
		if((res->GetNbNodes())||(left->GetDocId()!=right->GetDocId()))
			mThrowGException("Big Problem");

		// Tree of both nodes
		const GConceptTree* Tree(Engine->GetTree(left->GetDocId()));

		// For each pair of nodes of both query -> find the most top node containing both
		RCursor<GResNode> Node(left->GetNodes());
		RCursor<GResNode> Node2(right->GetNodes());
		for(Node.Start();!Node.End();Node.Next())
		{
			for(Node2.Start();!Node2.End();Node2.Next())
			{
				// Find the root of both nodes
				GConceptNode* Root(Tree->FindRoot(Node()->GetNode(),Node2()->GetNode()));
				if(!Root)
					continue;

				// Create a new fragment
				GResNode* Res(res->InsertPtr(Root,false));

				// Copy both children of Node() and Node2() in Res
				RCursor<GConceptNode> Cur(Node()->GetChildren());
				for(Cur.Start();!Cur.End();Cur.Next())
					Res->AddChild(Cur());
				RCursor<GConceptNode> Cur2(Node2()->GetChildren());
				for(Cur2.Start();!Cur2.End();Cur2.Next())
					Res->AddChild(Cur2());
			}
		}

		// Reduce the result
		res->Reduce(true);
		return;
	}

	// If not OR -> Compare each pair of occurrences
	RCursor<GResNode> Left(left->GetNodes());
	for(Left.Start();!Left.End();Left.Next())
	{
		switch(op)
		{
			case toSIB:
				// Left() is OK if they a sibling node in Right() exists
				if(right->FindSibling(Left()))
				{
					bool Find;
					size_t Idx(res->GetIndex(*Left(),Find));
					if(!Find)
						res->InsertPtrAt(Left(),Idx);
					// else sum the distances.
				}
				break;

			case toNSIB:
				// Left() is OK if they a sibling node in Right() doesn't exist
				if(!right->FindSibling(Left()))
				{
					bool Find;
					size_t Idx(res->GetIndex(*Left(),Find));
					if(!Find)
						res->InsertPtrAt(Left(),Idx);
					// else sum the distances.
				}
				break;

			case toINC:
				// Left() is OK if they a child node in Right() exist
				if(right->FindChild(Left()))
				{
					bool Find;
					size_t Idx(res->GetIndex(*Left(),Find));
					if(!Find)
						res->InsertPtrAt(Left(),Idx);
					// else sum the distances.
				}
				break;

			case toNINC:
				// Left() is OK if they a child node in Right() doesn't exist
				if(!right->FindChild(Left()))
				{
					bool Find;
					size_t Idx(res->GetIndex(*Left(),Find));
					if(!Find)
						res->InsertPtrAt(Left(),Idx);
					// else sum the distances.
				}
				break;

			default:
				break;
		}
	}

	// Reduce the result
	res->Reduce(false);
}


//------------------------------------------------------------------------------
void GQuery::ApplyOperator(tQueryOperator op)
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
	RCursor<GResNodes> Left(LeftOperand->GetDocs());
	RCursor<GResNodes> Right(RightOperand->GetDocs());
	for(Left.Start(),Right.Start();(!Left.End()&&(!Right.End()));)
	{
		if(Left()->GetDocId()==Right()->GetDocId())
		{
			// It is the same document -> Merge the two lists
			GResNodes* New(new GResNodes(Left()->GetDocId()));

			// If NAND: Since the second list is not empty -> nothing to do
			if(op!=toNAND)
			{
				ApplyOperator(op,Left(),Right(),New);

				// Add the results if not empty
				if(New->GetNbNodes())
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
			RCursor<GResNodes>* Min;
			if(Left()->GetDocId()>Right()->GetDocId())
				Min=&Right;
			else
				Min=&Left;

			// If the operand is a OR or NAND -> Add the it
			switch(op)
			{
				case toNAND:
					if(Min==&Right)
						break;
				case toOR:
					Res->InsertDoc(new GResNodes(*(*Min)()));
					break;
				default:
					break;
			}

			 // Go to the next document.
			Min->Next();
		}
	}

	RCursor<GResNodes>* Finish;
	if(Left.End())
			Finish=&Right;
		else
			Finish=&Left;
	for(;!Finish->End();Finish->Next())
	{
		// If the operand is a OR or NAND -> Add the it
		switch(op)
		{
			case toNAND:
				if(Finish==&Right)
					break;
			case toOR:
				Res->InsertDoc(new GResNodes(*(*Finish)()));
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
void GQuery::Perform(GQueryNode* node)
{
	if(!node)
		node=GetTop();

	switch(node->GetType())
	{
		case ttConcept:
			FindOccurrences(node->GetConcept());
			break;
		case ttOperator:
			Perform(node->GetFirst()); // Left operand
			if((node->GetOperator()==toOR)||(Results()->GetNb()))
			{
				Perform(node->GetLast()); // Right operand
				ApplyOperator(node->GetOperator());
			}
			break;
	}

	if((!node)&&(Results.GetNb()))
		Results()->Reduce();
}


//------------------------------------------------------------------------------
void GQuery::Print(GQueryNode* node)
{
	if(!node)
		node=GetTop();

	// Print spaces
	RString Spaces;
	Spaces.SetLen(node->GetDepth()," ");
	cout<<Spaces;

	// Print node information
	switch(node->GetType())
	{
		case ttOperator:
			switch(node->GetOperator())
			{
				case toNOP:
					cout<<"NOP"<<endl;
					break;
				case toNOT:
					cout<<"NOT"<<endl;
					break;
				case toOR:
					cout<<"OR"<<endl;
					break;
				case toAND:
					cout<<"AND"<<endl;
					break;
				case toINC:
					cout<<"INC"<<endl;
					break;
				case toSIB:
					cout<<"SIB"<<endl;
					break;
				case toNAND:
					cout<<"NAND"<<endl;
					break;
				case toNINC:
					cout<<"NINC"<<endl;
					break;
				case toNSIB:
					cout<<"NSIB"<<endl;
					break;
			}
			break;
		case ttConcept:
			if(node->GetConcept())
				cout<<node->GetConcept()->GetName()<<endl;
			else
				cout<<"{}"<<endl;
			break;
	}

	RNodeCursor<GQuery,GQueryNode> Top(node);
	for(Top.Start();!Top.End();Top.Next())
		Print(Top());
}


//------------------------------------------------------------------------------
const GQueryRes* GQuery::GetResult(void) const
{
	return(Results());
}


//------------------------------------------------------------------------------
RCursor<GConcept> GQuery::GetConcepts(size_t min,size_t max) const
{
	return(RCursor<GConcept>(Concepts,min,max));
}


//------------------------------------------------------------------------------
bool GQuery::IsIn(const GConcept& concept) const
{
	return(Concepts.IsIn(concept));
}


//------------------------------------------------------------------------------
GQuery::~GQuery(void)
{
	// Remove the results still on the stack
	while(Results.GetNb())
	{
		delete Results();
		Results.Pop();
	}
}