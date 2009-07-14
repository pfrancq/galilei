/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2004 by Julien Lamoral.
	Copyright 2002-2004 by David Wartel.
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gsubject.h>
#include <gsubjects.h>
#include <gsession.h>
#include <gprofile.h>
#include <guser.h>
#include <glang.h>
#include <gdoc.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <glink.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(size_t id,const RString& name,bool u)
	 : RNode<GSubjects,GSubject,true>(),  Id(id), Name(name), Used(u),
	   CategorizedDocs(1000), Docs(1000), WhereDocs(1000), Profiles(10,5), Community(0), Topic(0)
{
}


//------------------------------------------------------------------------------
void GSubject::ReInit(void)
{
	Profiles.Clear();
	Docs.Clear();
	WhereDocs.Clear();
	Community=0;
	Topic=0;
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(CompareIds(Id,sub.Id));
}


//------------------------------------------------------------------------------
int GSubject::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int GSubject::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSubject::BuildInfos(R::RContainer<GWeightInfo,false,true>& infos)
{
	GSession* Session(GSession::Get());

	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/deallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Info(infos);
	for(Info.Start();!Info.End();Info.Next())
		delete Info();
	infos.Clear();

	// Guess the language of the subject
	GLang* Lang(GuessLang());
	if(!Lang)
		return;

	// Build the description
	GConceptType* Type(Session->GetConceptType(Lang->GetCode()+RString("Terms"),false));  // Get a pointer to the corresponding dictionary.
	if(!Type)
		ThrowGException("Internal problem");
	for(GSubject* cur(this);cur;cur=cur->GetParent())
	{
		GConcept concept(cur->GetName(),Type);
		GConcept* ptr(Session->InsertConcept(&concept));
		GWeightInfo* ins(infos.GetPtr(ptr));
		if(ins)
			(*ins)+=1.0;
		else
			infos.InsertPtr(new GWeightInfo(ptr,1.0));
	}
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GDoc* doc) const
{
	return(Docs.IsIn(*doc));
}


//------------------------------------------------------------------------------
bool GSubject::IsCategorized(GDoc* doc) const
{
	return(CategorizedDocs.IsIn(*doc));
}


//------------------------------------------------------------------------------
void GSubject::FillDocs(GDoc** docs,size_t& nb,size_t maxdepth)
{
	nb+=CategorizedDocs.GetTab(&docs[nb]);
	if(maxdepth&&(Depth>=maxdepth))
	{
		RCursor<GSubject> Cur(GetSubjects());
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->FillDocs(docs,nb,maxdepth);
	}
}


//------------------------------------------------------------------------------
size_t GSubject::GetMaxDocs(size_t maxdepth)
{
	size_t nb(CategorizedDocs.GetNb());
	if(maxdepth&&(Depth>=maxdepth))
	{
		RCursor<GSubject> Cur(GetSubjects());
		for(Cur.Start();!Cur.End();Cur.Next())
			nb+=Cur()->GetMaxDocs(maxdepth);
	}
	return(nb);
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GProfile* prof) const
{
	return(Profiles.IsIn(*prof));
}


//------------------------------------------------------------------------------
void GSubject::ClearIdealGroup(tObjType type)
{
	switch(type)
	{
		case otCommunity:
			Community=0;
			break;
		case otTopic:
			Topic=0;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GCommunity* com)
{
	Community=com;
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GTopic* top)
{
	Topic=top;
}


//------------------------------------------------------------------------------
GCommunity* GSubject::GetIdealCommunity(void) const
{
	return(Community);
}


//------------------------------------------------------------------------------
GTopic* GSubject::GetIdealTopic(void) const
{
	return(Topic);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbIdealGroups(tObjType type) const
{
	size_t nb(0);

	switch(type)
	{
		case otProfile:
			if(Profiles.GetNb())
				nb++;
			break;
		case otDoc:
			if(Docs.GetNb())
				nb++;
			break;
		default:
			throw GException("GSubject::GetNbIdealGroups(tObjType) doesn't support type"+GetObjType(type));
	}
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbTopicsDocs(void) const
{
	size_t nb(0);

	if(Docs.GetNb())
		nb++;
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbObjs(const GCommunity* com) const
{
	size_t tot(0);
	RCursor<GProfile> Prof(Profiles);
	for(Prof.Start();!Prof.End();Prof.Next())
		if(com->IsIn(Prof()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbObjs(const GTopic* top) const
{
	size_t tot(0);
	RCursor<GDoc> Doc(Docs);
	for(Doc.Start();!Doc.End();Doc.Next())
		if(top->IsIn(Doc()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbDocs(const RContainer<GDoc,false,false>* docs) const
{
	size_t tot;
	RCursor<GDoc> sub(Docs);

	for(sub.Start(),tot=0;!sub.End();sub.Next())
	{
		if(docs->IsIn(*sub()))
			tot++;
	}
	return(tot);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GALILEI::GSubject::GetObjs(GProfile*) const
{
	return(R::RCursor<GProfile>(Profiles));
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetObjs(GDoc*) const
{
	return(R::RCursor<GDoc>(Docs));
}


//------------------------------------------------------------------------------
size_t GALILEI::GSubject::GetNbObjs(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			return(Docs.GetNb());
		case otProfile:
			return(Profiles.GetNb());
		default:
			return(0);
	}
}


//------------------------------------------------------------------------------
RString GSubject::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RString GSubject::GetFullName(void) const
{
	RString ret=Name;
	GSubject* parent=GetParent();

	while(parent)
	{
		ret=parent->GetName()+"/"+ret;
		parent=parent->GetParent();
	}
	return(ret);
}


//------------------------------------------------------------------------------
// Structure representing the number of documents for each language.
struct LangCount
{
	GLang* Lang;
	size_t NbDocs;

	LangCount(GLang* lang) : Lang(lang), NbDocs(0) {}
	int Compare(const LangCount& lang) const {return(Lang->Compare(lang.Lang));}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
	static int SortOrderNbDocs(const void* a,const void* b)
	{
		size_t af=(*((LangCount**)(a)))->NbDocs;
		size_t bf=(*((LangCount**)(b)))->NbDocs;

		if(af==bf) return(0);
		if(af>bf)
			return(-1);
		else
			return(1);
	}
};


//------------------------------------------------------------------------------
GLang* GSubject::GuessLang(void) const
{
	RContainer<LangCount,true,true> Langs(20);

	RCursor<GDoc> Doc(CategorizedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		LangCount* ptr(Langs.GetInsertPtr(Doc()->GetLang()));
		ptr->NbDocs++;
	}

	// If no language found, looks in the childs
	if(!Langs.GetNb())
	{
		RCursor<GSubject> Cur(GetSubjects());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			GLang* Lang(Cur()->GuessLang());
			if(Lang) return(Lang);
		}
		return(0);
	}

	// Order the container to find the language with the maximum number of documents
	Langs.ReOrder(LangCount::SortOrderNbDocs);
	return(Langs[0]->Lang);
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
