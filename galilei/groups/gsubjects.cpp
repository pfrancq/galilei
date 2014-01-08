/*

	GALILEI Research Project

	GSubjects.cpp

	Subjects - Implementation.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
#include <gsubjects.h>
#include <gprofile.h>
#include <guser.h>
#include <glang.h>
#include <gdoc.h>
#include <gcommunity.h>
#include <gtopic.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
//  GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(void)
	: RTree<GSubjects,GSubject,true>(), Subjects(0), SelectedDocs(0), DocsStatus(0), DocsSubjects(0),
	  ProfilesSubject(0), MustLoad(true), DescType(sdNames)
{
}


//------------------------------------------------------------------------------
void GSubjects::Init(const GSubject*,size_t nbsubjects,size_t nbdocs,size_t nbprofiles)
{
	Subjects.VerifyTab(nbsubjects);
	SelectedDocs.VerifyTab(nbdocs);
	DocsSubjects.VerifyTab(nbdocs);
	DocsStatus.Verify(nbdocs);
	ProfilesSubject.VerifyTab(nbprofiles);
}


//------------------------------------------------------------------------------
void GSubjects::ReInit(void)
{
	// Deselect the documents
	RCursor<GDoc> Doc(SelectedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
		Doc()->PostNotification(hDocs[oeDeselected]);

	// ReInit the subject
	RNodeCursor<GSubjects,GSubject> Subject(*this);
	for(Subject.Start();!Subject.End();Subject.Next())
		Subject()->ReInit();

	// Clear the documents and profiles assignment
	ProfilesSubject.Clear();
	DocsSubjects.Clear();
	SelectedDocs.Clear();
	//Storage->Clear(otSubject);
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	// Deselect the documents
	RCursor<GDoc> Doc(SelectedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
		Doc()->PostNotification(hDocs[oeDeselected]);

	// Clear the documents and profiles assignment
	ProfilesSubject.Clear();
	DocsSubjects.Clear();
	SelectedDocs.Clear();

	// Clear the subjects
	RTree<GSubjects,GSubject,true>::Clear();
	Subjects.Clear();
	MaxDepth=0;
	Storage->Clear(otSubject);
}


//------------------------------------------------------------------------------
void GSubjects::LoadObjs(const GSubject*)
{
	if(MustLoad&&(State!=osOnDemand))
	{
		State=osOnDemand;
		Storage->LoadObjs(pSubject);
		MustLoad=false;
		State=osLatest;
	}
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetMaxDepth(void) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(MaxDepth);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbObjs(const GSubject*) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(GetNbNodes());
}

//------------------------------------------------------------------------------
RCursor<GSubject> GSubjects::GetObjs(const GSubject*) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(R::RCursor<GSubject>(Subjects));
}


//------------------------------------------------------------------------------
RNodeCursor<GSubjects,GSubject> GSubjects::GetObjs(const GSubject*,const GSubject* parent) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	if(parent)
		return(RNodeCursor<GSubjects,GSubject>(parent));
	return(RNodeCursor<GSubjects,GSubject>(*this));
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetObjs(GSubject** subjects)
{
	LoadObjs(pSubject);
	return(Subjects.GetTab(subjects));
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetObj(const GSubject*,size_t id) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(Subjects.GetPtr(id));
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetObj(const GSubject*,const RString& name) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(Subjects.GetPtr(name,false));
}


//------------------------------------------------------------------------------
void GSubjects::InsertObj(GSubject* to,GSubject* subject)
{
	LoadObjs(pSubject);
	InsertNode(to,subject);
	Subjects.InsertPtr(subject);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbObjs(const GSubject*,tObjType type) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	size_t nb(0);
	RNodeCursor<GSubjects,GSubject> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbObjs(pSubject,type);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbObjs(const GSubject*,GDoc* doc) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	return(line->GetNb());
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetObjs(const GSubject*,const GDoc* doc) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(line)
		return(R::RCursor<GSubject>(*line));
	return(R::RCursor<GSubject>());
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetObj(const GSubject*,const GDoc* doc,bool one) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(0);
	if(one&&(line->GetNb()>1))
		mThrowGException("Document has multiple subjects");
	return((*line)[0]);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetSelectedObjs(GDoc** docs)
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(SelectedDocs.GetTab(docs,1,SelectedDocs.GetMaxPos()));
}


//------------------------------------------------------------------------------
RCursor<GDoc> GSubjects::GetSelectedObjs(const GDoc*) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(RCursor<GDoc>(SelectedDocs));
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromParentSubject(const GDoc* doc,const GSubject* subject) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);

	// Verify that a parent exist and that it is not the root node
	if(!subject->Parent)
		return(false);

	for(subject=subject->Parent;!subject;subject=subject->Parent)
		if(subject->Docs.IsIn(doc))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromSubject(const GDoc* doc,const GSubject* subject) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* line(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!line)
		return(false);
	return(line->IsIn(*subject));
}


//------------------------------------------------------------------------------
void GSubjects::InsertObj(GSubject* subject,GDoc* doc)
{
	// Some Verification (including loading the subjects)
	if(!subject)
		mThrowGException("No subject specified");
	if(!doc)
		mThrowGException("No document specified");
	LoadObjs(pSubject);

	// Look if the document must be insert in CategorizedDocs
	bool Find;
	size_t Pos(subject->CategorizedDocs.GetIndex(doc,Find));
	if(!Find)
		subject->CategorizedDocs.InsertPtrAt(doc,Pos,false);

	// Suppose the document is deselected.
	DocsStatus.Set(false,doc->GetId());

	// If the subjects are not currently loaded -> save the subject
	if(State!=osOnDemand)
		Storage->SaveObj(subject);
}


//------------------------------------------------------------------------------
void GSubjects::SetSelected(GDoc* doc,GSubject* subject,bool select)
{
	bool Find;

	// Some Verification (including loading the subjects)
	if(!subject)
		mThrowGException("No subject specified");
	if(!doc)
		mThrowGException("No document specified");
	LoadObjs(pSubject);

	// Get the subjects associated with the document
	R::RContainer<GSubject,false,false>* subjects(DocsSubjects.GetPtrAt(doc->GetId()));

	if(select)
	{
		// The document is selected
		DocsStatus.Set(true,doc->GetId());
		size_t Pos(SelectedDocs.GetIndex(doc,Find));
		if(!Find)
			SelectedDocs.InsertPtr(doc);

		// Add an association between the document and the subject
		if(!subjects)
			DocsSubjects.InsertPtrAt(subjects=new R::RContainer<GSubject,false,false>(10,5),doc->GetId(),true);
		Pos=subjects->GetIndex(*subject,Find);
		if(!Find)
			subjects->InsertPtrAt(subject,Pos,false);

		// Add the document in the subject
		Pos=subject->Docs.GetIndex(doc,Find);
		if(!Find)
		{
			subject->Docs.InsertPtrAt(doc,Pos,false);

			// Emit a selection signal if this is the first time the document is selected
			if((subjects->GetNb()==1)&&(State!=osOnDemand))
				doc->PostNotification(hDocs[oeSelected]);
		}

	}
	else
	{
		// Remove the association between a document and a subject
		if(!subjects)
			return;
		subjects->DeletePtr(*subject);

		// Remove the document from the subject
		subject->Docs.DeletePtr(doc);

		// Emit a deselection signal if the document has no subject associated anymore
		if(subjects->GetNb()==0)
		{
			SelectedDocs.DeletePtr(*doc);
			DocsStatus.Set(false,doc->GetId());
			if(State!=osOnDemand)
				doc->PostNotification(hDocs[oeDeselected]);
		}
	}

	// If the subjects are not currently loaded -> save the subject
	if(State!=osOnDemand)
		Storage->SaveObj(subject);
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetObj(const GSubject*,const GProfile* profile) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(ProfilesSubject.GetPtrAt(profile->GetId()));
}


//------------------------------------------------------------------------------
void GSubjects::InsertObj(GSubject* subject,GProfile* profile)
{
	// Some Verification (including loading the subjects)
	if(!subject)
		mThrowGException("No subject specified");
	if(!profile)
		mThrowGException("No profile specified");
	LoadObjs(pSubject);

	ProfilesSubject.InsertPtrAt(subject,profile->GetId(),true);
	subject->Profiles.InsertPtr(profile);

	// If the subjects are not currently loaded -> save the subject
	if(State!=osOnDemand)
		Storage->SaveObj(subject);
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetObj(const GSubject*,const GCommunity* com) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	RCursor<GSubject> Cur(Subjects);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Community==com)
			return(Cur());
	}
	return(0);
}


//------------------------------------------------------------------------------
void GSubjects::SetDescType(tSubjectDesc type)
{
	LoadObjs(pSubject);
	if(type!=DescType)
	{
		// All descriptions of the subjects must be deleted.
		RCursor<GSubject> Cur(Subjects);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->Vectors)
			{
				delete Cur()->Vectors;
				Cur()->Vectors=0;
			}
		}
	}
	DescType=type;
}


//------------------------------------------------------------------------------
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
	LoadObjs(pSubject);

	RContainer<GSubject,false,false> ToDel(500);            // Subjects to delete
	RContainer<NewGenericSubject,true,false> ToIns(500);    // Subjects to insert
	GSession* Session(dynamic_cast<GSession*>(this));
	if(!Session)
		mThrowGException("The GSubject object is not a GSession one");

	size_t NbNoLeaf(0);
	size_t NbLeaf(0);
	RCursor<GSubject> Cur(Subjects);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GSubject* Subject(Cur());
		if(Subject->GetNbSubjects()&&Subject->Docs.GetNb())
		{
			// If no child subjects have documents or children -> it is OK.
			bool OK(true);
			RNodeCursor<GSubjects,GSubject> Cur2(Subject);
			for(Cur2.Start();(!Cur2.End())&&OK;Cur2.Next())
				if(Cur2()->GetNbSubjects()||Cur2()->CategorizedDocs.GetNb())
					OK=false;
			if(!OK)
			{
				NbNoLeaf++;

				// Create a new subject
				GSubject* NewSubject(new GSubject(Session,GetNbNodes()+NbNoLeaf,Subject->Name+" general",true));
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
		InsertObj(New()->Parent,New()->Subject);

	// Save if necessary
	if(SaveResults)
	{
		Storage->Clear(otSubject);
		Cur=GetObjs(pSubject);
		for(Cur.Start();!Cur.End();Cur.Next())
			Storage->SaveObj(Cur());
	}

	cout<<"There are "<<GetNbObjs(pSubject)<<" subjects:"<<endl;
	if(NbNoLeaf)
		cout<<"  "<<NbNoLeaf<<" non-leaf subjects have some documents attached"<<endl;
	if(NbLeaf)
		cout<<"  "<<NbLeaf<<" leaf subjects have no documents attached"<<endl;
	cout<<"  "<<static_cast<double>(NbNoLeaf+NbLeaf)*100.0/static_cast<double>(GetNbObjs(pSubject))<<"% of invalid subjects"<<endl;
}
