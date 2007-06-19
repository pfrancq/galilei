/*

	GALILEI Research Project

	GLang.cpp

	Generic Language - Implementation.

	Copyright 2001-2006 by the Universit� Libre de Bruxelles.

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
GLang::GLang(GFactoryLang* fac,const RString& lang,const char* code)
	: RLang(lang,code), GPlugin<GFactoryLang>(fac), Dicts(10),
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
void GLang::SetReferences(unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
{
	NbRefDocs=refdocs;
	NbRefSubProfiles=refsubprofiles;
	NbRefGroups=refgroups;
}


//------------------------------------------------------------------------------
void GLang::Connect(GSession* session)
{
	Session=session;
	Session->GetStorage()->LoadLang(this);
}


//------------------------------------------------------------------------------
void GLang::Disconnect(GSession* /*session*/)
{
	Session=0;
	Dicts.Clear();
	SetReferences(0,0,0);
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
void GLang::AddDict(GDict* dict)
{
	Dicts.InsertPtr(dict);
}


//------------------------------------------------------------------------------
R::RCursor<GDict> GLang::GetDicts(void) const
{
	// Load all dictionnaries associated to a concept type
	R::RCursor<GConceptType> Types(Session->GetConceptTypes());
	for(Types.Start();!Types.End();Types.Next())
		GetDict(Types()->GetId());
	return(R::RCursor<GDict>(Dicts));
}


//------------------------------------------------------------------------------
GDict* GLang::GetDict(unsigned int type) const
{
	GDict* Dict=Dicts.GetPtr(type);
	if((!Dict)&&(Session)&&(Session->GetStorage()))
		Dict=Session->GetStorage()->LoadDic(const_cast<GLang*>(this),type);
	return(Dict);
}


//------------------------------------------------------------------------------
GDict* GLang::GetDict(void) const
{
	GDict* Dict=Dicts.GetPtr(1);
	if((!Dict)&&(Session)&&(Session->GetStorage()))
		Dict=Session->GetStorage()->LoadDic(const_cast<GLang*>(this),1);
	return(Dict);
}


//------------------------------------------------------------------------------
GDict* GLang::GetStop(void) const
{
	GDict* Stop=Dicts.GetPtr(2);
	if((!Stop)&&(Session)&&(Session->GetStorage()))
		Stop=Session->GetStorage()->LoadDic(const_cast<GLang*>(this),2);
	return(Stop);
}


//------------------------------------------------------------------------------
bool GLang::InStop(const RString& name) const
{
	return(GetStop()->IsIn(name));
}


//------------------------------------------------------------------------------
void GLang::IncRef(unsigned int id,unsigned int type,tObjType ObjType)
{
	unsigned int nb=GetDict(type)->IncRef(id,ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,type,id,nb);
}


//------------------------------------------------------------------------------
void GLang::DecRef(unsigned int id,unsigned int type,tObjType ObjType)
{
	unsigned int nb=GetDict(type)->DecRef(id,ObjType);
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,type,id,nb);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(unsigned int id,unsigned int type,tObjType ObjType)
{
	return(GetDict(type)->GetRef(id,ObjType));
}


//------------------------------------------------------------------------------
void GLang::IncRef(tObjType ObjType)
{
	unsigned int nb;

	switch(ObjType)
	{
		case otDoc:
			nb=++NbRefDocs;
			break;
		case otSubProfile:
			nb=++NbRefSubProfiles;
			break;
		case otGroup:
			nb=++NbRefGroups;
			break;
		default:
			throw GException ("Unkown type to increase");
			break;
	}
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,nb);
}


//------------------------------------------------------------------------------
void GLang::DecRef(tObjType ObjType)
{
	unsigned int nb;

	switch(ObjType)
	{
		case otDoc:
			if(!NbRefDocs)
				throw GException("Cannot decrease null number of references for documents");
			nb=--NbRefDocs;
			break;
		case otSubProfile:
			if(!NbRefSubProfiles)
				throw GException("Cannot decrease null number of references for subprofiles");
			nb=--NbRefSubProfiles;
			break;
		case otGroup:
			if(!NbRefGroups)
				throw GException("Cannot decrease null number of references for groups");
			nb=--NbRefGroups;
			break;
		default:
			throw GException ("Unkown type to decrease");
			break;
	}
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,nb);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetRef(tObjType ObjType) const
{
	switch(ObjType)
	{
		case otDoc:
			return(NbRefDocs);
			break;
		case otSubProfile:
			return(NbRefSubProfiles);
			break;
		case otGroup:
			return(NbRefGroups);
			break;
		default:
			return(NbRefDocs+NbRefSubProfiles+NbRefGroups);
			break;
	}
	return(0);
}


//------------------------------------------------------------------------------
void GLang::Clear(tObjType ObjType)
{
	R::RCursor<GDict> Dicts(GetDicts());
	for(Dicts.Start();!Dicts.End();Dicts.Next())
	{
		RCursor<GDict::Hash> Cur(Dicts()->GetCursor()); 
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RCursor<GDict::Hash2> Cur2(*Cur());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				RCursor<GConcept> Cur3(*Cur2());
				for(Cur3.Start();!Cur3.End();Cur3.Next())
				{
					Cur3()->Clear(ObjType);
					if(Session&&Session->MustSaveResults()&&Session->GetStorage())
						Session->GetStorage()->SaveRefs(ObjType,this,Cur3()->GetType(),Cur3()->GetId(),0);
				}					
			}
		}
	}

	switch(ObjType)
	{
		case otDoc:
			NbRefDocs=0;
			break;
		case otSubProfile:
			NbRefSubProfiles=0;
			break;
		case otGroup:
			NbRefGroups=0;
			break;
		case otDocSubProfile:
			NbRefDocs=0;
			NbRefSubProfiles=0;
			break;
		case otDocGroup:
			NbRefDocs=0;
			NbRefGroups=0;
			break;
		case otSubProfileGroup:
			NbRefSubProfiles=0;
			NbRefGroups=0;
			break;
		default:
			NbRefDocs=0;
			NbRefSubProfiles=0;
			NbRefGroups=0;
			break;
	}
	if(Session&&Session->MustSaveResults()&&Session->GetStorage())
		Session->GetStorage()->SaveRefs(ObjType,this,0);
}


//------------------------------------------------------------------------------
unsigned int GLang::GetTotal(unsigned int type) const
{
	return(GetDict(type)->GetNbConcepts());
}


//------------------------------------------------------------------------------
const GConcept** GLang::GetConcepts(unsigned int type) const
{
	return(const_cast<const GConcept**>(GetDict(type)->GetConcepts()));
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
