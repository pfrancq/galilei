/*

	GALILEI Research Project

	GSearchQuery.h

	Search Query - Implementation.

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



//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <gconcept.h>
#include <gsession.h>
#include <rnodecursor.h>
#include <gdocref.h>
#include <gsearchquery.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSearchQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSearchQuery::GSearchQuery(GSession* session,const RString& query)
	: RTree<GSearchQuery,GSearchQueryNode,true>(), Session(session), Concepts(30)
{
	Set(query);
}


//------------------------------------------------------------------------------
void GSearchQuery::Set(const RString& query)
{
	CreateToken(0,query);
}


//------------------------------------------------------------------------------
bool GSearchQuery::CreateToken(GSearchQueryNode* parent,const RString& str)
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
		if((!Count)&&(!InQuotes)&&(GSearchQueryNode::GetOperator(*Car)))
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

			GSearchQueryNode* Token(NewNode(Keyword,Namespaces));
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
			GSearchQueryNode* Token(NewNode(GSearchQueryNode::oAND));
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
		GSearchQueryNode::tOperator Op(GSearchQueryNode::GetOperator(*Car));
		if(Op==GSearchQueryNode::oNOT)
		{
			Car++; // Skip it
			if(!(*Car))
				mThrowGException("Invalid not expression '"+str+"'");
			Op=GSearchQueryNode::GetNotOperator(*Car);
			if(Op==GSearchQueryNode::oNOP)
				mThrowGException("Invalid not expression '"+str+"'");
		}
		Car++; // Skip it

		// Create the operator
		GSearchQueryNode* Token(NewNode(Op));
		InsertNode(parent,Token);
		if(!CreateToken(Token,ClearString(str.Mid(0,Pos))))
			mThrowGException("Not a left expression in '"+str+"'");
		if(!CreateToken(Token,ClearString(RString(Car))))
			mThrowGException("Not a right expression in '"+str+"'");
	}
	return(true);
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::NewNode(const RString& str,bool type)
{
	return(new GSearchQueryNode(Session,str,type));
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::NewNode(GSearchQueryNode::tOperator op)
{
	return(new GSearchQueryNode(op));
}


//------------------------------------------------------------------------------
RString GSearchQuery::ClearString(const RString& str)
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
void GSearchQuery::Print(GSearchQueryNode* node)
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
		case GSearchQueryNode::nOperator:
			switch(node->GetOperator())
			{
				case GSearchQueryNode::oNOP:
					cout<<"NOP"<<endl;
					break;
				case GSearchQueryNode::oNOT:
					cout<<"NOT"<<endl;
					break;
				case GSearchQueryNode::oOR:
					cout<<"OR"<<endl;
					break;
				case GSearchQueryNode::oAND:
					cout<<"AND"<<endl;
					break;
				case GSearchQueryNode::oINC:
					cout<<"INC"<<endl;
					break;
				case GSearchQueryNode::oSIB:
					cout<<"SIB"<<endl;
					break;
				case GSearchQueryNode::oNAND:
					cout<<"NAND"<<endl;
					break;
				case GSearchQueryNode::oNINC:
					cout<<"NINC"<<endl;
					break;
				case GSearchQueryNode::oNSIB:
					cout<<"NSIB"<<endl;
					break;
			}
			break;
		case GSearchQueryNode::nConcept:
			if(node->GetConcept())
				cout<<node->GetConcept()->GetName()<<endl;
			else
				cout<<"{}"<<endl;
			break;
	}

	RNodeCursor<GSearchQuery,GSearchQueryNode> Top(node);
	for(Top.Start();!Top.End();Top.Next())
		Print(Top());
}


//------------------------------------------------------------------------------
RCursor<GConcept> GSearchQuery::GetConcepts(size_t min,size_t max) const
{
	return(RCursor<GConcept>(Concepts,min,max));
}


//------------------------------------------------------------------------------
bool GSearchQuery::IsIn(const GConcept& concept) const
{
	return(Concepts.IsIn(concept));
}


//------------------------------------------------------------------------------
GSearchQuery::~GSearchQuery(void)
{
}