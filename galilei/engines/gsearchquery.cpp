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
#include <gstatement.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSearchQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSearchQuery::GSearchQuery(GSession* session,size_t caller,int options,R::RContainer<GLang,false,false>* langs)
	: RTree<GSearchQuery,GSearchQueryNode,true>(), Session(session), Tokens(30),
	  Concepts(30), Tmp(30), Found(30), OnlyAND(true), Options(options), Stems(20), Langs(langs),
	  Extractor(session), Caller(caller)
{
	if(Options==0)
		mThrowGException("No query options are specified");
	if(Options>7)
		mThrowGException("No valid query options specified");
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
	return(Concepts.IsIn(*concept));
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
void GSearchQuery::MoveNode(GSearchQueryNode* to,GSearchQueryNode* node)
{
	R::RTree<GSearchQuery,GSearchQueryNode,true>::MoveNode(to,node);
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
void GSearchQuery::Build(const RString& query)
{
	OnlyAND=true;
	CreateToken(0,query);
	if(Options&qoExpressions)
	{
	   RNodeCursor<GSearchQuery,GSearchQueryNode> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			TreatNode(Cur());
	}
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
		if((!InQuotes)&&((*Car)!=':')&&((*Car)!='-')&&(!Car->IsAlNum()))
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
			bool Multiple(false);
			Car=str();
			while((*Car)!=0)
			{
				if((*Car)=='"')
					NbQuotes++;
				else
				{
					if(Car&&Car->IsSpace())
						Multiple=true;
					Keyword+=(*Car);
				}
				Car++;
			}
			if(NbQuotes>2)
				mThrowGException("Too many '\"' in '"+str+"'");

			GSearchQueryNode* Token;
			if(Namespaces)
				Token=CreateToken(parent,Keyword,GSearchToken::tRawConcept,false);
			else
				Token=CreateToken(parent,Keyword,GSearchToken::tTerm,Multiple);
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
			if(Token->GetNbNodes()!=2)
				mThrowGException("The AND operator ("+RString::Number(Pos)+") as not two operands in '"+str+"'");
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
		if(Token->GetNbNodes()!=2)
			mThrowGException("The "+Token->GetOperatorName()+" operator ("+RString::Number(Pos)+") as not two operands in '"+str+"'");
	}
	return(true);
}


//------------------------------------------------------------------------------
template<class Cursor>
	inline void ComputeStems(GSession* session,Cursor& lang,GConceptType* terms,const RString& token,R::RContainer<GConcept,false,true>& stems)
{
	for(lang.Start();!lang.End();lang.Next())
	{
		RString Stem(lang()->GetStemming(token));

		// Look if the stem is defined in the system
		GConcept* Concept(session->GetObj(pConcept,terms,Stem,true));
		if(!Concept)
			continue;  // No -> nothing to do

		// Look if the stem must be added
		bool Find;
		int Idx(stems.GetIndex(*Concept,Find));
		if(!Find)
			stems.InsertPtrAt(Concept,Idx,false);
	}
}

//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::AddORs(GSearchQueryNode* parent)
{
	if(!Found.GetNb())
		return(0);

	GSearchQueryNode* pToken(0);

	size_t NbOR(Found.GetNb()-1); // Number of OR operator to insert
	RCursor<GConceptRef> Node(Found);
	for(Node.Start();!Node.End();Node.Next())
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
		InsertNode(parent,New=NewNode(Node()->GetConcept(),GSearchToken::tStem));
		if(!pToken)
			pToken=New;
	}

	return(pToken);
}


//------------------------------------------------------------------------------
GSearchQueryNode* GSearchQuery::CreateToken(GSearchQueryNode* parent,const R::RString& token,GSearchToken::tType type,bool multiple)
{
	GSearchQueryNode* pToken(0);

	if(type==GSearchToken::tTerm)
	{
		// Extract the different tokens
		Extractor.Search(token,Found,Caller);

		// Build the OR nodes
		if(Found.GetNb())
			pToken=AddORs(parent);
		else
			InsertNode(parent,pToken=NewNode(Session->GetUnknown(),type));  // Create an unknown concept
	}
	else
		InsertNode(parent,pToken=NewNode(token,type));          // No stems or no keyword -> simply insert the token

	return(pToken);
}


//------------------------------------------------------------------------------
void GSearchQuery::TreatNode(GSearchQueryNode* node)
{
	if(node->GetType()!=GSearchQueryNode::nOperator)
		return;

	if(node->GetOperator()==GSearchQueryNode::oAND)
	{
		RString Str;

		TreatANDNode(node->GetFirst(),Str);
		if(Str.IsEmpty())
			return;
		TreatANDNode(node->GetLast(),Str);
		if(Str.IsEmpty())
			return;

		// Look for expression
		Extractor.Search(Str,Found,Caller);
		if(!Found.GetNb())
			return;

		// Some expressions are found -> Must replace the parent AND by an OR and move the AND
		GSearchQueryNode* Ref(node);
		GSearchQueryNode* MainOR(new GSearchQueryNode(GSearchQueryNode::oOR));
		InsertNode(Ref->GetParent(),MainOR);
		MoveNode(MainOR,Ref);

		// Add sub expressions
		AddORs(MainOR);
	}
	else
	{
		RNodeCursor<GSearchQuery,GSearchQueryNode> Cur(node);
		for(Cur.Start();!Cur.End();Cur.Next())
			TreatNode(Cur());
	}
}


//------------------------------------------------------------------------------
void GSearchQuery::TreatANDNode(GSearchQueryNode* node,RString& str)
{
	if(node->GetType()==GSearchQueryNode::nToken)
	{
		// Can not treat unknown concepts
		if(node->GetToken()->GetConcept()==Session->GetUnknown())
		{
			str.Clear();
			return;
		}

		if(!str.IsEmpty())
			str+=" ";
		str+=node->GetToken()->GetConcept()->GetName();
	}
	else if((node->GetOperator()==GSearchQueryNode::oAND))
	{
		TreatANDNode(node->GetFirst(),str);
		TreatANDNode(node->GetLast(),str);
	}
	else
		str.Clear();
}


//------------------------------------------------------------------------------
GSearchQuery::~GSearchQuery(void)
{
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ostream& std::operator<<(ostream& os, const GSearchQuery& query)
{
	if(query.GetTop())
		os<<(*query.GetTop())<<endl;
	else
		os<<"Null query";
   return(os);
}
