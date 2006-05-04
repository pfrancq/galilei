/*

	GALILEI Research Project

	GLang.cpp

	Generic Language - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
#include <glang.h>
#include <gdict.h>
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
GLang::GLang(GFactoryLang* fac,const RString& lang,const char* code)
	: RLang(lang,code), GPlugin<GFactoryLang>(fac), Dict(0), Stop(0),
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
	GSession::Event(this,eObjNewMem);
}


//------------------------------------------------------------------------------
void GLang::Connect(GSession* session)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GLang::Disconnect(GSession* /*session*/)
{
	Session=0;
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
void GLang::SkipSequence(const RString& word)
{
	SkipWords.InsertPtr(new SkipWord(word));
}


//------------------------------------------------------------------------------
/*RString GLang::GetStemming(const RString& _kwd)
{
	return(_kwd.ToLower());
}*/


//------------------------------------------------------------------------------
GDict* GLang::GetDict(void) const
{
	if((!Dict)&&(Session)&&(Session->GetStorage()))
		Session->GetStorage()->LoadDic(const_cast<GLang*>(this)->Dict,const_cast<GLang*>(this),false);
	return(Dict);
}


//------------------------------------------------------------------------------
GDict* GLang::GetStop(void) const
{
	if((!Stop)&&(Session)&&(Session->GetStorage()))
		Session->GetStorage()->LoadDic(const_cast<GLang*>(this)->Stop,const_cast<GLang*>(this),true);
	return(Stop);
}


//------------------------------------------------------------------------------
bool GLang::InStop(const RString& name) const
{
	return(GetStop()->IsIn(name));
}


//------------------------------------------------------------------------------
void GLang::IncRef(unsigned int id,tObjType ObjType)
{
	unsigned int nb=GetDict()->IncRef(id,ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,id,nb);
}


//------------------------------------------------------------------------------
void GLang::DecRef(unsigned int id,tObjType ObjType)
{
	unsigned int nb=GetDict()->DecRef(id,ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,id,nb);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(unsigned int id,tObjType ObjType)
{
	return(GetDict()->GetRef(id,ObjType));
}


//------------------------------------------------------------------------------
void GLang::IncRef(tObjType ObjType)
{
	unsigned int nb=GetDict()->IncRef(ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,nb);
}


//------------------------------------------------------------------------------
void GLang::DecRef(tObjType ObjType)
{
	unsigned int nb=GetDict()->DecRef(ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,nb);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(tObjType ObjType) const
{
	return(GetDict()->GetRef(ObjType));
}


//------------------------------------------------------------------------------
unsigned int GLang::GetTotal(void) const
{
	return(GetDict()->GetNbDatas());
}


//------------------------------------------------------------------------------
const GData** GLang::GetDatas(void) const
{
	return(const_cast<const GData**>(GetDict()->GetDatas()));
}


//------------------------------------------------------------------------------
bool GLang::MustSkipSequence(const RChar* seq)
{
	return(SkipWords.IsIn<const RChar*>(seq));
}


//------------------------------------------------------------------------------
GLang::~GLang(void)
{
	GSession::Event(this,eObjDeleteMem);
	if(Dict)
		delete Dict;
	if(Stop)
		delete Stop;
}



//------------------------------------------------------------------------------
//
// class GLangManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLangManager::GLangManager(void)
  : GPluginManager<GLangManager,GFactoryLang,GLang>("Lang",API_LANG_VERSION,ptList)
{
}


//------------------------------------------------------------------------------
GLangManager::~GLangManager(void)
{
}
