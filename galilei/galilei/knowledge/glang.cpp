/*

	GALILEI Research Project

	GLang.cpp

	Generic Language - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
// include files for GALILEI
#include <glang.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GLang::SkipWord
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GLang::SkipWord
{
public:
	R::RString Word;
	SkipWord(const RString& w) : Word(w) {}
	int Compare(const SkipWord* w) const {return(Word.Compare(w->Word));}
	int Compare(const SkipWord& w) const {return(Word.Compare(w.Word));}
	int Compare(const R::RChar* w) const {return(Word.Compare(w));}
};



//------------------------------------------------------------------------------
//
// class GLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLang::GLang(GSession* session,GPlugInFactory* fac,const RString& lang,const char* code)
	: RLang(lang,code), GPlugIn(session,fac), Stop(0), MustLoadStop(true), Dict(0), MustLoadDict(true),
	  SkipWords(50,20)
{
	SkipWords.InsertPtr(new SkipWord("min"));
	SkipWords.InsertPtr(new SkipWord("sec"));
	SkipWords.InsertPtr(new SkipWord("msec"));
	SkipWords.InsertPtr(new SkipWord("s"));
	SkipWords.InsertPtr(new SkipWord("nm"));
	SkipWords.InsertPtr(new SkipWord("mm"));
	SkipWords.InsertPtr(new SkipWord("cm"));
	SkipWords.InsertPtr(new SkipWord("dm"));
	SkipWords.InsertPtr(new SkipWord("m"));
	SkipWords.InsertPtr(new SkipWord("dam"));
	SkipWords.InsertPtr(new SkipWord("km"));
	SkipWords.InsertPtr(new SkipWord("mg"));
	SkipWords.InsertPtr(new SkipWord("cg"));
	SkipWords.InsertPtr(new SkipWord("dg"));
	SkipWords.InsertPtr(new SkipWord("g"));
	SkipWords.InsertPtr(new SkipWord("gr"));
	SkipWords.InsertPtr(new SkipWord("dag"));
	SkipWords.InsertPtr(new SkipWord("hg"));
	SkipWords.InsertPtr(new SkipWord("kg"));
	SkipWords.InsertPtr(new SkipWord("t"));
	SkipWords.InsertPtr(new SkipWord("ml"));
	SkipWords.InsertPtr(new SkipWord("cl"));
	SkipWords.InsertPtr(new SkipWord("dl"));
	SkipWords.InsertPtr(new SkipWord("dal"));
	SkipWords.InsertPtr(new SkipWord("hl"));
	SkipWords.InsertPtr(new SkipWord("in"));
	SkipWords.InsertPtr(new SkipWord("ft"));
	SkipWords.InsertPtr(new SkipWord("yd"));
	SkipWords.InsertPtr(new SkipWord("ml"));
}


//------------------------------------------------------------------------------
int GLang::Compare(const GLang& lang) const
{
	return(strcmp(Code,lang.Code));
}


//------------------------------------------------------------------------------
int GLang::Compare(const GLang* lang) const
{
	if(!lang)
		return(1);
	return(strcmp(Code,lang->Code));
}


//------------------------------------------------------------------------------
int GLang::Compare(const RLang* lang) const
{
	if(!lang)
		return(1);
	return(strcmp(Code,lang->GetCode()));
}


//------------------------------------------------------------------------------
int GLang::Compare(const char* code) const
{
	return(strcmp(Code,code));
}


//------------------------------------------------------------------------------
void GLang::SkipSequence(const RString& word)
{
	SkipWords.InsertPtr(new SkipWord(word));
}


//------------------------------------------------------------------------------
GConceptType* GLang::GetDict(void) const
{
	if((!Dict)&&(MustLoadDict))
	{
		const_cast<GLang*>(this)->Dict=Session->GetObj(pConceptType,GetCode()+RString("Terms"),true);
		const_cast<GLang*>(this)->MustLoadDict=false;
	}
	return(Dict);
}


//------------------------------------------------------------------------------
GConceptType* GLang::GetStop(void) const
{
	if((!Stop)&&(MustLoadStop))
	{
		const_cast<GLang*>(this)->Stop=Session->GetObj(pConceptType,GetCode()+RString("Stopwords"),true);
		const_cast<GLang*>(this)->MustLoadStop=false;
	}
	return(Stop);
}


//------------------------------------------------------------------------------
bool GLang::InStop(const RString& name) const
{
	GConceptType* Stop=GetStop();
	if(Stop)
		return(Stop->GetObj(pConcept,name,true));
	return(false);
}


//------------------------------------------------------------------------------
bool GLang::MustSkipSequence(const RChar* seq)
{
	return(SkipWords.IsIn<const RChar*>(seq));
}


//------------------------------------------------------------------------------
void GLang::GetStemming(const R::RContainer<R::RString,true,false>& tokens,R::RContainer<R::RString,true,false>& stems)
{
	stems.Clear();
	RCursor<RString> Token(tokens);
	for(Token.Start();!Token.End();Token.Next())
	{
		if(GetStop()->GetObj(pConcept,*Token(),true))
			continue;
		stems.InsertPtr(new RString(GetStemming(*Token())));
	}
}


//------------------------------------------------------------------------------
GConcept* GLang::CreateExpression(const R::RString& expr,GConceptType* type)
{
	// Add the expression
	RString Expr(expr.Trim());
	GConcept* New(Session->InsertObj(pConcept,type,Expr));

	// Treat the separate tokens
	GConceptType* Text(Session->GetObj(pConceptType,"Tokens",false));
	RContainer<RString,true,false> Tokens(5);
	Expr.Split(Tokens,' ');
	RCursor<RString> Token(Tokens);
	for(Token.Start();!Token.End();Token.Next())
	{
		// Exclude stopwords
		if(GetStop()->GetObj(pConcept,*Token(),true))
			continue;

		GConcept* Ptr(Session->InsertObj(pConcept,Text,GetStemming(Token()->ToLower())));
		Session->InsertObj(pStatement,cNoRef,Ptr,Session->GetPartOf(),New,1.0);
	}

	return(New);
}


//------------------------------------------------------------------------------
GLang::~GLang(void)
{
}
