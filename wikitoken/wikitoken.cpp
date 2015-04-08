/*

	Wikipedia Plug-Ins

	WikiToken.h

	Wikipedia Content Token - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for current project
#include <wikitoken.h>
#include <rstringbuilder.h>



//-----------------------------------------------------------------------------
// Static variables
static WikiTokens* Instance=0;
const RChar Space(' ');
const RChar Underscore('_');



//------------------------------------------------------------------------------
//
// class WikiToken
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WikiToken::WikiToken(void)
	: Depth(0), Pos(0), SyntacticPos(0)
{
	Content.SetLen(256);
	Content.Clear();
}


//------------------------------------------------------------------------------
void WikiToken::Clear(void)
{
	Content.Clear();
	Depth=0;
	Pos=0;
	SyntacticPos=0;
}


//------------------------------------------------------------------------------
RString WikiToken::GetWikiTitle(const RString& title)
{
	RStringBuilder Title;
	const RChar* ptr(title());
	bool First(true);

	while(!ptr->IsNull())
	{
		if(First)
		{
			Title+=RChar::ToUpper(*ptr);
			First=false;
		}
		else
		{
			if((*ptr)==Space)
				Title+=Underscore;
			else
				Title+=(*ptr);
		}
		ptr++;
	}


	return(Title());
}



//------------------------------------------------------------------------------
//
// class WikiTokens
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
WikiTokens::WikiTokens(void)
	: MemTokens(50000), Tokens(50000), Doc(0)
{
	if(Instance)
		mThrowGException("Already one instance of Wikipedia Tokens must run");
	Instance=this;
}

//------------------------------------------------------------------------------
WikiToken* WikiTokens::CreateToken(void)
{
	WikiToken* Token(0);

	if(Tokens.GetNb()>=MemTokens.GetNb())
		MemTokens.InsertPtr(Token=new WikiToken());
	else
	{
		Token=MemTokens[Tokens.GetNb()];
		Token->Clear();
	}
	Tokens.InsertPtr(Token);
	return(Token);
}


//------------------------------------------------------------------------------
RCursor<WikiToken> WikiTokens::GetTokens(void) const
{
	return(RCursor<WikiToken>(Tokens));
}


//------------------------------------------------------------------------------
void WikiTokens::Clear(void)
{
	Tokens.Clear();
	Doc=0;
}


//------------------------------------------------------------------------------
void WikiTokens::DeleteLastToken(void)
{
	Tokens.DeletePtrAt(Tokens.GetNb()-1);
}


//------------------------------------------------------------------------------
WikiTokens* WikiTokens::Get(void)
{
	if(!Instance)
		new WikiTokens();
	return(Instance);
}
