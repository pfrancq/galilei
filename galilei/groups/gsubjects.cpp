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
	: RTree<GSubjects,GSubject,true>(0,200), Subjects(0,200), MaxDepth(0),
	SelectedDocs(0), DocsSubjects(0), ProfilesSubject(0), ToLoad(true)
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
R::RCursor<GSubject> GSubjects::GetTopSubjects(void) const
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
R::RCursor<GSubject> GSubjects::GetSubjects(void) const
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
	if(to)
		subject->Depth=to->Depth+1;
	else
		subject->Depth=1;
	if(subject->Depth>MaxDepth)
		MaxDepth=subject->Depth;
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
	return(MaxDepth);
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
GSubjects::~GSubjects(void)
{
}
