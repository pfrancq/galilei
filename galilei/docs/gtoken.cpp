/*

	GALILEI Research Project

	GToken.h

	Document Token - Implementation.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <gtoken.h>
#include <gvector.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GToken::Occurrence
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTokenOccur::GTokenOccur(size_t nb)
	: Token(0), Vector(0), Children(nb,300), Pos(cNoRef), Depth(cNoRef), Index(cNoRef), Weight(0.0)
{
}


//------------------------------------------------------------------------------
GTokenOccur::GTokenOccur(GToken* token,GVector* vector,size_t pos,size_t depth,size_t spos,size_t nb)
	: Token(token), Vector(vector), Children(nb,300), Pos(pos), Depth(depth), SyntacticPos(spos), Index(cNoRef), Weight(0.0)
{
}



//------------------------------------------------------------------------------
//
// class GToken::Search
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GToken::Search::Search(R::RString token,tTokenType type)
	: Token(token), Type(type)
{
}



//------------------------------------------------------------------------------
//
// class GToken
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GToken::GToken(const RString& token)
	: Token(token), Type(ttUnknown), Concept(0), Occurs(20,20), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
int GToken::Compare(const GToken& token) const
{
	int Res(Token.Compare(token.Token));
	if(!Res)
		return(Type-token.Type);
	return(Res);
}


//------------------------------------------------------------------------------
int GToken::Compare(const Search& token) const
{
	int Res(Token.Compare(token.Token));
	if(!Res)
		return(Type-token.Type);
	return(Res);
}


//------------------------------------------------------------------------------
size_t GToken::HashIndex(size_t idx) const
{
	return(Token.HashIndex(idx));
}


//------------------------------------------------------------------------------
bool GToken::IsAlpha(void) const
{
	RCharCursor Cur(Token);
   for(Cur.Start();!Cur.End();Cur.Next())
      if(!Cur().IsAlpha())
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool GToken::IsUsed(GConcept* metaconcept) const
{
	if(!metaconcept)
		return(false);
	RCursor<GTokenOccur> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
		if(Occur()->Vector->GetMetaConcept()==metaconcept)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool GToken::IsUsed(tConceptCat cat) const
{
	RCursor<GTokenOccur> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
		if(Occur()->Vector->GetMetaConcept()->GetType()->GetCategory()==cat)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
GToken::~GToken(void)
{
}