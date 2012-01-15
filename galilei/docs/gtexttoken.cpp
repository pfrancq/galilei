/*

	GALILEI Research Project

	GTextToken.h

	Document Token - Implementation.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#include <gtexttoken.h>
#include <gvector.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GTextToken::Occurrence
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTextToken::Occurrence::Occurrence(void)
	: Token(0), Vector(0), Pos(cNoRef), Depth(cNoRef), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
GTextToken::Occurrence::Occurrence(GTextToken* token,GVector* vector,size_t pos,size_t depth)
	: Token(token), Vector(vector), Pos(pos), Depth(depth), Index(cNoRef)
{
}



//------------------------------------------------------------------------------
//
// class GTextToken
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTextToken::GTextToken(const RString& token)
	: Token(token), Concept(0), Occurs(20,20), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
int GTextToken::Compare(const GTextToken& token) const
{
	return(Token.Compare(token.Token));
}


//------------------------------------------------------------------------------
int GTextToken::Compare(const R::RString& token) const
{
	return(Token.Compare(token));
}

//------------------------------------------------------------------------------
size_t GTextToken::HashIndex(size_t idx) const
{
	return(Token.HashIndex(idx));
}


//------------------------------------------------------------------------------
bool GTextToken::IsAlpha(void) const
{
	RCharCursor Cur(Token);
   for(Cur.Start();!Cur.End();Cur.Next())
      if(!Cur().IsAlpha())
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool GTextToken::IsUsed(GConcept* metaconcept) const
{
	if(!metaconcept)
		return(false);
	RCursor<Occurrence> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
		if(Occur()->Vector->GetMetaConcept()==metaconcept)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool GTextToken::IsUsed(GConceptCat* cat) const
{
	if(!cat)
		return(false);
	RCursor<Occurrence> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
		if(Occur()->Vector->GetMetaConcept()->GetType()->GetCategory()==cat)
			return(true);
	return(false);
}