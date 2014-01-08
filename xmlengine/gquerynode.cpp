/*

	GALILEI Research Project

	GQueryNode.h

	Search Query Node - Implementation.

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
#include <ggalileiapp.h>
#include <gsession.h>


//------------------------------------------------------------------------------
// include files for current project
#include <gquerynode.h>



//------------------------------------------------------------------------------
//
// Global functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
tQueryOperator GetOperator(const RChar c)
{
	if(c=='|')
		return(toOR);
	if((c=='+')||(c=='&'))
		return(toAND);
	if(c=='/')
		return(toINC);
	if(c=='.')
		return(toSIB);
	if(c=='-')
		return(toNOT);
	return(toNOP);
}


//------------------------------------------------------------------------------
tQueryOperator GetNotOperator(const RChar c)
{
	if(c=='|')
		return(toOR);
	if((c=='+')||(c=='&'))
		return(toNAND);
	if(c=='/')
		return(toNINC);
	if(c=='.')
		return(toNSIB);
	return(toNOP);
}



//------------------------------------------------------------------------------
//
// class GQueryNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GQueryNode::GQueryNode(tQueryOperator op)
	: RNode<GQuery,GQueryNode,true>(), Type(ttOperator)
{
	Value.Operator=op;
}


//------------------------------------------------------------------------------
GQueryNode::GQueryNode(GSession* session,const RString& str,bool type)
	: RNode<GQuery,GQueryNode,true>(), Type(ttConcept)
{
	if(type)
	{
		// Find the right most :
		int Pos(str.Find(':',-1));
		RString ConceptType(str.Mid(0,Pos));
		RString Concept(str.Mid(Pos+1));
		GConceptType* Type(session->GetConceptType(ConceptType,false));
		Value.Concept=Type->GetConcept(Concept);
	}
	else
	{
		GLang* Lang(GALILEIApp->GetPlugIn<GLang>("Lang","en"));
		GConceptType* Terms(session->GetConceptType("Terms",false));
		Value.Concept=Terms->GetConcept(Lang->GetStemming(str.Trim()));
	}
}


//------------------------------------------------------------------------------
tQueryOperator GQueryNode::GetOperator(void) const
{
	if(Type!=ttOperator)
		mThrowGException("Query token is not an operator");
	return(Value.Operator);
}


//------------------------------------------------------------------------------
GConcept* GQueryNode::GetConcept(void) const
{
	if(Type!=ttConcept)
		mThrowGException("Query token is not a concept");
	return(Value.Concept);
}

