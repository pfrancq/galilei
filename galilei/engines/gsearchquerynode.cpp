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
	if(c=='-')
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