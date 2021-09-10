/*

	GALILEI Research Project

	GSearchQueryNode.h

	Search Query Node - Implementation.

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
#include <ggalileiapp.h>
#include <gsession.h>
#include <gsearchquerynode.h>
#include <gsearchquery.h>
#include <glang.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSearchQueryNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSearchQueryNode::GSearchQueryNode(tOperator op)
	: RNode<GSearchQuery,GSearchQueryNode,true>(), Type(nOperator)
{
	Value.Operator=op;
}


//------------------------------------------------------------------------------
GSearchQueryNode::GSearchQueryNode(GConcept* concept,GSearchToken::tType type)
	: RNode<GSearchQuery,GSearchQueryNode,true>(), Type(nToken)
{
	Value.Token=new GSearchToken(concept,type);
}


//------------------------------------------------------------------------------
GSearchQueryNode::GSearchQueryNode(GSession* session,const RString& token,GSearchToken::tType type)
	: RNode<GSearchQuery,GSearchQueryNode,true>(), Type(nToken)
{
	Value.Token=new GSearchToken(session,token,type);
}


//------------------------------------------------------------------------------
int GSearchQueryNode::Compare(const GSearchQueryNode&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
GSearchQueryNode::tOperator GSearchQueryNode::GetOperator(void) const
{
	if(Type!=nOperator)
		mThrowGException("Query token is not an operator");
	return(Value.Operator);
}


//------------------------------------------------------------------------------
GSearchToken* GSearchQueryNode::GetToken(void) const
{
	if(Type!=nToken)
		mThrowGException("Query token is not a keyword");
	return(Value.Token);
}


//------------------------------------------------------------------------------
RString GSearchQueryNode::GetOperatorName(void) const
{
	RString Ret;
	if(Type!=nOperator)
		mThrowGException("Query token is not an operator");
	switch(Value.Operator)
	{
		case oNOT:
			Ret="NOT";
			break;

		case oOR:
			Ret="OR";
			break;

		case oAND:
			Ret="AND";
			break;

		case oINC:
			Ret="INCLUDE";
			break;

		case oSIB:
			Ret="SIBLING";
			break;

		case oNAND:
			Ret="NOT AND";
			break;

		case oNINC:
			Ret="NOT INCLUDE";
			break;

		case oNSIB:
			Ret="NOT SIBLING";
			break;
	}
	return(Ret);
}


//------------------------------------------------------------------------------
GSearchQueryNode::tOperator GSearchQueryNode::GetOperator(const RChar c)
{
	if(c=='|')
		return(oOR);
	if((c=='+')||(c=='&'))
		return(oAND);
	if(c=='/')
		return(oINC);
	if(c=='.')
		return(oSIB);
	if(c=='~')
		return(oNOT);
	return(oNOP);
}


//------------------------------------------------------------------------------
GSearchQueryNode::tOperator GSearchQueryNode::GetNotOperator(const RChar c)
{
	if(c=='|')
		return(oOR);
	if((c=='+')||(c=='&'))
		return(oNAND);
	if(c=='/')
		return(oNINC);
	if(c=='.')
		return(oNSIB);
	return(oNOP);
}


//------------------------------------------------------------------------------
GSearchQueryNode::~GSearchQueryNode(void)
{
	if(Type==nToken)
		delete Value.Token;
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ostream& std::operator<<(ostream& os, const GSearchQueryNode& node)
{
	if(node.GetDepth())
	{
		if(node.GetDepth()>1)
		{
			for(size_t i=0;i<node.GetDepth();i++)
				os<<" ";
		}
		os<<"┕";
		if(node.GetDepth()>1)
			os<<"╼";
		os<<" ";
	}
	if(node.GetType()==GSearchQueryNode::nOperator)
	{
		os<<node.GetOperatorName();
	}
	else
	{
		GConcept* Concept(node.GetToken()->GetConcept());
		os<<Concept->GetName()<<" ["<<Concept->GetId()<<"]";
	}
	os<<endl;
	RNodeCursor<GSearchQuery,GSearchQueryNode> Cur(&node);
   for(Cur.Start();!Cur.End();Cur.Next())
		os<<(*Cur());
   return(os);
}
