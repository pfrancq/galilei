/*

	GALILEI Research Project

	GSubjects.cpp

	Subjects - Implementation.

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
#include <gsubjects.h>
#include <gsubject.h>
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <glang.h>
#include <gprofile.h>
#include <gmeasure.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <gindexer.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(void)
	: RTree<GSubjects,GSubject,true>(0,200), Subjects(0,200),
	SelectedDocs(0), DocsSubjects(0), ProfilesSubject(0), ToLoad(true), DescType(dtNames)
{
}


//------------------------------------------------------------------------------
inline void GSubjects::VerifyLoad(void) const
{
	if(ToLoad)
	{
		const_cast<GSubjects*>(this)->ToLoad=false;
		const_cast<GSubjects*>(this)->Storage->LoadSubjects();
	}
}


//------------------------------------------------------------------------------
void GSubjects::ReInit(void)
{
	// Re-initialize the subjects
	VerifyLoad();
	RCursor<GSubject> Subjects(GetNodes());
	for(Subjects.Start();!Subjects.End();Subjects.Next())
		Subjects()->ReInit();

	// Clear the documents and profiles assignment
	ProfilesSubject.Clear();
	DocsSubjects.Clear();
	SelectedDocs.Clear();
}


//------------------------------------------------------------------------------
void GSubjects::SetDescType(tDescType type)
{
	if(type!=DescType)
	{
		// All descriptions of the subjects must be deleted.
		RCursor<GSubject> Cur(GetSubjects());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->Vector)
			{
				delete Cur()->Vector;
				Cur()->Vector=0;
			}
		}
	}
	DescType=type;
}

struct NewGenericSubject
{
	GSubject* Subject;
	GSubject* Parent;

	NewGenericSubject(GSubject* subject,GSubject* parent) : Subject(subject), Parent(parent) {}
	int Compare(const NewGenericSubject&) const {return(-1);}
};


//------------------------------------------------------------------------------
void GSubjects::TestSubjects(void)
{
	GSession* Session(GSession::Get());
	RContainer<GSubject,false,false> ToDel(500); // Subjects to delete
	RContainer<NewGenericSubject,true,false> ToIns(500);   // Subjects to insert

	size_t NbNoLeaf(0);
	size_t NbLeaf(0);
	RCursor<GSubject> Cur(GetSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GSubject* Subject(Cur());
		if(Subject->GetNbSubjects()&&Subject->Docs.GetNb())
		{
			// If no child subjects have documents or children -> it is OK.
			bool OK(true);
			RCursor<GSubject> Cur2(Subject->GetSubjects());
			for(Cur2.Start();(!Cur2.End())&&OK;Cur2.Next())
				if(Cur2()->GetNbSubjects()||Cur2()->CategorizedDocs.GetNb())
					OK=false;
			if(!OK)
			{
				NbNoLeaf++;

				// Create a new subject
				GSubject* NewSubject(new GSubject(GetNb()+NbNoLeaf,Subject->Name+" general",true));
				ToIns.InsertPtr(new NewGenericSubject(NewSubject,Cur()));

				// Transfer all the document from Cur() to subject
				RCursor<GDoc> Docs(Subject->CategorizedDocs);
				for(Docs.Start();!Docs.End();Docs.Next())
					NewSubject->CategorizedDocs.InsertPtr(Docs());
				Docs.Set(Subject->Docs);
				for(Docs.Start();!Docs.End();Docs.Next())
					NewSubject->Docs.InsertPtr(Docs());
				Subject->CategorizedDocs.Clear();
				Subject->Docs.Clear();
			}
		}
		else if(!Subject->GetNbSubjects()&&!Subject->CategorizedDocs.GetNb())
		{
			ToDel.InsertPtr(Subject);
			NbLeaf++;
		}
	}

	// Delete the nodes
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		DeleteNode(Cur(),true);

	// Insert the nodes
	RCursor<NewGenericSubject> New(ToIns);
	for(New.Start();!New.End();New.Next())
		InsertSubject(New()->Parent,New()->Subject);

	// Save if necessary
	if(Session->MustSaveResults())
	{
		Session->Storage->Clear(otSubject);
		Cur=GetSubjects();
		for(Cur.Start();!Cur.End();Cur.Next())
			Session->Storage->SaveSubject(Cur());
	}

	cout<<"There are "<<GetNbSubjects()<<" subjects:"<<endl;
	if(NbNoLeaf)
		cout<<"  "<<NbNoLeaf<<" non-leaf subjects have some documents attached"<<endl;
	if(NbLeaf)
		cout<<"  "<<NbLeaf<<" leaf subjects have no documents attached"<<endl;
	cout<<"  "<<static_cast<double>(NbNoLeaf+NbLeaf)*100.0/static_cast<double>(GetNbSubjects())<<"% of invalid subjects"<<endl;
}


//------------------------------------------------------------------------------
RCursor<GSubject> GSubjects::GetTopSubjects(void) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::GetTopNodes());
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbSubjects(void) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::GetNb());
}


//------------------------------------------------------------------------------
RCursor<GSubject> GSubjects::GetSubjects(void) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::GetNodes());
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetSubjects(const GSubject* subject) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::GetNodes(subject));
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbSubjects(const GSubject* subject) const
{
	VerifyLoad();
	return(subject->GetNbNodes());
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(const RString& subject) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::GetNode(subject));
}


//------------------------------------------------------------------------------
void GSubjects::InsertSubject(GSubject* to,GSubject* subject)
{
	VerifyLoad();
	R::RTree<GSubjects,GSubject,true>::InsertNode(to,subject);
	Subjects.InsertPtr(subject);
}


//------------------------------------------------------------------------------
void GSubjects::Clear(size_t nbsubjects,size_t nbdocs,size_t nbprofiles)
{
	RTree<GSubjects,GSubject,true>::Clear(nbsubjects);
	ProfilesSubject.Clear(nbprofiles);
	SelectedDocs.Clear(nbdocs);
	DocsSubjects.Clear(nbdocs);
	Subjects.Clear(nbsubjects);
	MaxDepth=0;
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetMaxDepth(void) const
{
	VerifyLoad();
	return(R::RTree<GSubjects,GSubject,true>::MaxDepth);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbIdealGroups(tObjType type) const
{
	VerifyLoad();
	size_t nb(0);
	RCursor<GSubject> Cur(GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbTopicsDocs(void) const
{
	VerifyLoad();
	size_t nb(0);
	RCursor<GSubject> Cur(GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(size_t subjectid) const
{
	VerifyLoad();
	return(Subjects.GetPtr(subjectid));
}


//------------------------------------------------------------------------------
void GSubjects::Insert(GProfile* profile,size_t subjectid)
{
	VerifyLoad();
	GSubject* subject(Subjects.GetPtr(subjectid));
	if(!subject) return;
	ProfilesSubject.InsertPtrAt(subject,profile->GetId(),true);
	subject->Profiles.InsertPtr(profile);
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetIdealGroup(GProfile* prof) const
{
	VerifyLoad();
	return(ProfilesSubject.GetPtrAt(prof->GetId()));
}


//------------------------------------------------------------------------------
void GSubjects::Insert(GDoc* doc,size_t subjectid,size_t usedid)
{
	VerifyLoad();
	GSubject* subject(RTree<GSubjects,GSubject,true>::GetNode(subjectid));
	if(subject)
		subject->CategorizedDocs.InsertPtr(doc);
	GSubject* used(RTree<GSubjects,GSubject,true>::GetNode(usedid));
	if(used)
	{
		R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
		if(!line)
			DocsSubjects.InsertPtrAt(line=new R::RContainer<GSubject,false,false>(10,5),doc->GetId(),true);
		bool Find;
		size_t Index(line->GetIndex(*used,Find));
		if(!Find)
			line->InsertPtrAt(used,Index,false);
		Index=used->Docs.GetIndex(*doc,Find);
		if(!Find)
			used->Docs.InsertPtrAt(doc,Index,false);
		Index=SelectedDocs.GetIndex(*doc,Find);
		if(!Find)
			SelectedDocs.InsertPtrAt(doc,Index,false);
	}
}


//-----------------------------------------------------------------------------
size_t GSubjects::FillSelectedDocs(GDoc** docs)
{
	return(SelectedDocs.GetTab(docs,1,SelectedDocs.GetMaxPos()));
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetIdealGroup(GDoc* doc) const
{
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	if(line->GetNb()>1)
		throw GException("GSubjects::GetIdealGroup(GDoc*) const : Document has multiple subjects");
	return((*line)[0]);

}


//------------------------------------------------------------------------------
bool GSubjects::IsFromIdealGroup(GDoc* doc,const GSubject* s) const
{
	VerifyLoad();
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(false);
	return(line->IsIn(*s));
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromParentIdealGroup(GDoc* doc,const GSubject* s) const
{
	VerifyLoad();

	// Verify that a parent exist and that it is not the root node
	if(!s->Parent)
		return(false);

	for(s=s->Parent;!s;s=s->Parent)
		if(s->Docs.IsIn(doc))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetIdealGroups(GDoc* doc) const
{
	VerifyLoad();
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(line)
		return(R::RCursor<GSubject>(*line));
	return(R::RCursor<GSubject>());
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbIdealGroups(GDoc* doc) const
{
	VerifyLoad();
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	return(line->GetNb());
}


//------------------------------------------------------------------------------
double GSubjects::GetUpOperationsCost(const GSubject* u,const GSubject* v) const
{
	return(R::RTree<GSubjects,GSubject,true>::GetUpOperationsCost(u,v));
}


//------------------------------------------------------------------------------
GSubjects::~GSubjects(void)
{
}
