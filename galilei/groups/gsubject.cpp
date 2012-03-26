/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2012 by Pascal Francq (pascal@francq.info).
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
// include files for R Project
#include <rnodecursor.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gsubject.h>
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
GSubject::GSubject(GSession* session,size_t id,const RString& name,bool u)
	 : RNode<GSubjects,GSubject,true>(), Session(session), Id(id), Name(name), Used(u),
	   CategorizedDocs(1000), Docs(1000), WhereDocs(1000), Profiles(10,5),
	   Community(0), Topic(0)
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
void GSubject::CreateDescription(void)
{
	// If a description exists, leave it
	if(IsDefined())
		return;

	switch(Tree->DescType)
	{
		// Description is build based on the name of the subject and its parents
		case sdNames:
		{
			// Get a pointer to the corresponding dictionary.
			GConceptType* Type(Session->GetConceptType("Tokens",true));

			// Create one content vector
			GConcept Struct(Session,"Content",Session->GetConceptType("Structure",true));
			GVector* Vector(Vectors->GetInsertPtr(Session->InsertConcept(&Struct)));

			// Build the description
			for(const GSubject* cur(this);cur;cur=cur->GetParent())
			{
				RString Name;

				// Verify that the subject has not the same name as the parent
				if(cur->GetParent())
				{
					Name=cur->GetName();
					if(cur->GetParent()->GetName()==cur->GetName())
						Name+="_2";
				}
				else
					Name=cur->GetName();
				GConcept concept(Session,Name,Type);
				GConcept* ptr(Session->InsertConcept(&concept));
				GConceptRef* ins(Vector->GetRef(ptr));
				(*ins)+=1.0;
			}
			break;
		}

		// Description is build:
		// 1. If a leaf node : The center of gravitation of the documents.
		// 2. If a non-leaf node : The center of gravitation of the children.
		case sdDocs:
		{
			// If the subject is not selected -> next one.
			if(!Used)
				return;

			// Look if a leaf-node
			if(GetNbSubjects())
			{
				// Compute the common information entities of all the children
				RNodeCursor<GSubjects,GSubject> Child(this);
				Child.Start();
				Child()->CreateDescription();
				(*Vectors)=(*Child()->Vectors);
				for(;!Child.End();Child.Next())
				{
					RCursor<GVector> Vector(*Vectors);
					for(Vector.Start();!Vector.End();Vector.End())
						Vector()->Intersection(*Child()->GetVector(Vector()->GetMetaConcept()));
				}
				RCursor<GVector> Vector(*Vectors);
				for(Vector.Start();!Vector.End();Vector.End())
					(*Vector())/=Child.GetNb();
			}
			else
			{
				// No, it has no documents
				if(!Docs.GetNb())
					ThrowGException("Leaf subject '"+Name+"' has no documents attached");

				// Compute the center of gravitation of all documents
				RCursor<GDoc> Cur(Docs);
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					RCursor<GVector> Vector(Cur()->GetVectors());
					for(Vector.Start();!Vector.End();Vector.End())
					{
						GVector* MyVector(Vectors->GetInsertPtr(Vector()->GetMetaConcept()));
						(*MyVector)+=(*Vector());
					};
				}
				RCursor<GVector> Vector(*Vectors);
				for(Vector.Start();!Vector.End();Vector.End())
					(*Vector())/=Docs.GetNb();
			}

			break;
		}
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
		RNodeCursor<GSubjects,GSubject> Cur(this);
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
		RNodeCursor<GSubjects,GSubject> Cur(this);
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
void GSubject::Clear(void)
{
	R::RNode<GSubjects,GSubject,true>::Clear();
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
void GSubject::AssignIdeal(GCommunity* com)
{
	Community=com;
}


//------------------------------------------------------------------------------
void GSubject::AssignIdeal(GTopic* top)
{
	Topic=top;
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
			ThrowGException(GetObjType(type,true,true)+" are not assigned to subjects");
	}
	RNodeCursor<GSubjects,GSubject> Cur(this);
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
	RNodeCursor<GSubjects,GSubject> Cur(this);
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
R::RCursor<GProfile> GALILEI::GSubject::GetObjs(const GProfile*) const
{
	return(R::RCursor<GProfile>(Profiles));
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetObjs(const GDoc*) const
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
GLang* GSubject::GuessLang(bool lookparent) const
{
	RContainer<LangCount,true,true> Langs(20);

	RCursor<GDoc> Doc(CategorizedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		LangCount* ptr(Langs.GetInsertPtr(Doc()->GetLang()));
		ptr->NbDocs++;
	}

	// If no language found, look in the children and then in parents
	if(!Langs.GetNb())
	{
		// Look in the children (without to search in their parents of course).
		RNodeCursor<GSubjects,GSubject> Cur(this);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			GLang* Lang(Cur()->GuessLang(false));
			if(Lang) return(Lang);
		}

		// Look in the parents
		if(lookparent)
		{
			for(GSubject* cur(GetParent());cur;cur=cur->GetParent())
			{
				GLang* Lang(cur->GuessLang());
				if(Lang) return(Lang);
			}
		}

		// Really not found
		return(0);
	}

	// Order the container to find the language with the maximum number of documents
	Langs.ReOrder(LangCount::SortOrderNbDocs);
	return(Langs[0]->Lang);
}


//------------------------------------------------------------------------------
double GSubject::GetUpOperationCost(void) const
{
	double Cost(0.0);

	if(Parent)
	{
		RCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Look if the parent has a vector for that concept
			const GVector* Correspondance(Parent->GetVector(Vector()->GetMetaConcept()));
			if(Correspondance)
			{
				// The cost is the difference between the number of information entities of
				// the current class with its parents (it is of course supposed that all the
				// information entities of the parent are in the current class).
				Cost+=(Vector()->GetNb()-Correspondance->GetNb());
			}
			else
				Cost+=Vector()->GetNb();
		}
	}
	else
	{
		// No parent -> all the concepts references are to be 'added'.
		RCursor<GVector> Vector(GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
			Cost+=Vector()->GetNb();
	}

	return(Cost);
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
	delete Vectors;
}



//------------------------------------------------------------------------------
//
//  GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(size_t max)
	: RTree<GSubjects,GSubject,true>(), Subjects(max), SelectedDocs(0), DocsSubjects(0),
	  ProfilesSubject(0), SubjectsLoaded(false), DescType(sdNames)
{
}

//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	RTree<GSubjects,GSubject,true>::Clear();
	ProfilesSubject.Clear();
	SelectedDocs.Clear();
	DocsSubjects.Clear();
	MaxDepth=0;
}