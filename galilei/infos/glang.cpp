/*

	GALILEI Research Project

	GLang.cpp

	Generic Language - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <infos/glang.h>
#include <infos/gdict.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
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
	int Compare(const SkipWord* w) {return(Word.Compare(w->Word));}
	int Compare(const SkipWord& w) {return(Word.Compare(w.Word));}
	int Compare(const R::RChar* w) {return(Word.Compare(w));}
};



//------------------------------------------------------------------------------
//
// class GLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLang::GLang(GFactoryLang* fac,const RString& lang,const char* code) throw(std::bad_alloc)
	: RLang(lang,code), GPlugin<GFactoryLang>(fac), Session(0), Dict(0), Stop(0),
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
void GLang::Connect(GSession* session) throw(GException)
{
	Session=session;
	if(Dict&&Stop) return;
	if(!Dict)
		session->GetStorage()->LoadDic(Dict,this,false);
	if(!Stop)
		session->GetStorage()->LoadDic(Stop,this,true);
}


//------------------------------------------------------------------------------
void GLang::Disconnect(GSession* /*session*/) throw(GException)
{
	Session=0;
	if((!Dict)&&(!Stop)) return;
	if(Dict)
	{
		delete Dict;
		Dict=0;
	}
	if(Stop)
	{
		delete Stop;
		Stop=0;
	}
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
void GLang::SkipSequence(const RString& word) throw(std::bad_alloc)
{
	SkipWords.InsertPtr(new SkipWord(word));
}


//------------------------------------------------------------------------------
RString GLang::GetStemming(const RString& _kwd) throw(GException)
{
	return(_kwd.ToLower());
}


//------------------------------------------------------------------------------
GDict* GLang::GetDict(void) const
{
	return(Dict);
}


//------------------------------------------------------------------------------
GDict* GLang::GetStop(void) const
{
	return(Stop);
}


//------------------------------------------------------------------------------
bool GLang::InStop(const RString& name) const throw(GException)
{
	if(!Stop)
		throw GException("No dictionary of stopwords defined.");
	return(Stop->IsIn(name));
}


//------------------------------------------------------------------------------
void GLang::IncRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	Dict->IncRef(id,ObjType);
}


//------------------------------------------------------------------------------
void GLang::DecRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	Dict->DecRef(id,ObjType);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(unsigned int id,tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	return(Dict->GetRef(id,ObjType));
}


//------------------------------------------------------------------------------
void GLang::IncRef(tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	Dict->IncRef(ObjType);
}


//------------------------------------------------------------------------------
void GLang::DecRef(tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	Dict->DecRef(ObjType);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(tObjType ObjType) throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	return(Dict->GetRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GLang::GetTotal(void) const throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	return(Dict->GetNbDatas());
}


//------------------------------------------------------------------------------
const GData** GLang::GetDatas(void) const throw(GException)
{
	if(!Dict)
		throw GException("No dictionary of data defined.");
	return(const_cast<const GData**>(Dict->GetDatas()));
}


//------------------------------------------------------------------------------
bool GLang::MustSkipSequence(const RChar* seq) throw(GException)
{
	return(SkipWords.IsIn<const RChar*>(seq));
}


//------------------------------------------------------------------------------
GLang::~GLang(void)
{
	if(Dict)
		delete Dict;
	if(Stop)
		delete Stop;
}
