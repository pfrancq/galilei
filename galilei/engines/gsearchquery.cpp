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
GSearchQuery::GSearchQuery(GSession* session,bool expandstems)
	: RTree<GSearchQuery,GSearchQueryNode,true>(), Session(session), Tokens(30),
	  Concepts(30), OnlyAND(true), ExpandStems(expandstems),Stems(20)
{
}


//------------------------------------------------------------------------------
void GSearchQuery::Build(const RString& query)
{
	OnlyAND=true;
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

			GSearchQueryNode* Token;
			if(Namespaces)
				Token=CreateToken(parent,Keyword,GSearchToken::tRawConcept);
			else
				Token=CreateToken(parent,Keyword,GSearchToken::tTerm);
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
void GSearchQuery::InsertNode(GSearchQueryNode* parent,GSearchQueryNode* node)
{
	R::RTree<GSearchQuery,GSearchQueryNode,true>::InsertNode(parent,node);
	if(node->IsToken()&&(node->GetToken()->GetType()==GSearchToken::tTerm)&&node->GetToken()->GetConcept())
		Tokens.InsertPtr(new RString(node->GetToken()->GetConcept()->GetName()));
	else if((node->IsOperator())&&(node->GetOperator()!=GSearchQueryNode::oAND))
		OnlyAND=false;
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::CreateToken(GSearchQueryNode* parent,const R::RString& token,GSearchToken::tType type)
{
	GSearchQueryNode* pToken(0);

	if((type==GSearchToken::tTerm)&&ExpandStems)
	{
		// Expand the query with stems and the OR operator

		// Some initialization
		RString Token(token.Trim().ToLower());
		GConceptType* Terms(Session->GetObj(pConceptType,"Terms",false));
		Stems.Clear();

		// Find the concept corresponding to the token
		GConcept* cToken(Session->GetObj(pConcept,Terms,Token,true));

		// Compute all possible stems
		RCastCursor<GPlugIn,GLang> Langs(GALILEIApp->GetPlugIns<GLang>("Lang"));
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			RString Stem(Langs()->GetStemming(Token));

			// Look if the stemmed concept corresponds to the normal term
			if(Token==Stem)
				continue; // Yes -> nothing to do

			// Look if the stem is defined in the system
			GConcept* Concept(Session->GetObj(pConcept,Terms,Stem,true));
			if(!Concept)
				continue;  // No -> nothing to do

			// Look if the stem must be added
			bool Find;
			int Idx(Stems.GetIndex(Concept,Find));
			if(!Find)
				Stems.InsertPtrAt(Concept,Idx,false);
		}

		// Look if there are alternative stems
		if(Stems.GetNb()>0)
		{
			size_t NbOR(Stems.GetNb()-1); // Number of OR operator to insert

			// Insert first the term if necessary
			if(cToken)
			{
				GSearchQueryNode* OR(NewNode(GSearchQueryNode::oOR));
				bool Hold(OnlyAND);
				InsertNode(parent,OR);
				OnlyAND=Hold;
				parent=OR;  // It will be the parent of the next token to insert
				InsertNode(parent,pToken=NewNode(cToken,GSearchToken::tTerm));
			}

			// Add the stems
			RCursor<GConcept> Stem(Stems); // Treat the last later
			for(Stem.Start();!Stem.End();Stem.Next())
			{
				// Create a new OR operator
				if(NbOR)
				{
					GSearchQueryNode* OR(NewNode(GSearchQueryNode::oOR));
					bool Hold(OnlyAND);
					InsertNode(parent,OR);
					OnlyAND=Hold;
					parent=OR;  // It will be the parent of the next token to insert
					NbOR--;
				}

				// Add a stem node
				GSearchQueryNode* New;
				InsertNode(parent,New=NewNode(Stem(),GSearchToken::tStem));
				if(!pToken)
					pToken=New;
			}
		}
		else
			InsertNode(parent,pToken=NewNode(cToken,type));      // No stems -> simply insert the token
	}
	else
		InsertNode(parent,pToken=NewNode(token,type));          // No stems or no keyword -> simply insert the token
	return(pToken);
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::NewNode(const RString& token,GSearchToken::tType type)
{
	return(new GSearchQueryNode(Session,token,type));
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::NewNode(GConcept* concept,GSearchToken::tType type)
{
 	return(new GSearchQueryNode(concept,type));
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
		case GSearchQueryNode::nToken:
			cout<<"{";
			if(node->GetToken()->GetConcept())
				cout<<node->GetToken()->GetConcept()->GetName();
			cout<<"}"<<endl;
			break;
	}

	RNodeCursor<GSearchQuery,GSearchQueryNode> Top(node);
	for(Top.Start();!Top.End();Top.Next())
		Print(Top());
}


//------------------------------------------------------------------------------
RCursor<R::RString> GSearchQuery::GetTokens(void) const
{
	return(RCursor<R::RString>(Tokens));
}


//------------------------------------------------------------------------------
RCursor<GConcept> GSearchQuery::GetConcepts(size_t min,size_t max) const
{
	return(RCursor<GConcept>(Concepts,0,max));
}


//------------------------------------------------------------------------------
bool GSearchQuery::IsIn(GConcept* concept) const
{
	return(Concepts.IsIn(concept));
}


//------------------------------------------------------------------------------
GSearchQuery::~GSearchQuery(void)
{
}