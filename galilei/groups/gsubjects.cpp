/*

	GALILEI Research Project

	GSubjects.cpp

	Subjects - Implementation.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
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
	: RTree<GSubjects,GSubject,true>(), Subjects(0), UsedSubjects(0), UsedDocs(0), DocsStatus(0), DocsSubjects(0),
	  ProfilesSubject(0), MustLoad(true), DescType(sdNames)
{
}


//------------------------------------------------------------------------------
void GSubjects::Init(const GSubject*,size_t nbsubjects)
{
	Subjects.VerifyTab(nbsubjects);
	UsedSubjects.VerifyTab(nbsubjects);
	UsedDocs.VerifyTab(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);
	DocsSubjects.VerifyTab(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);
	DocsStatus.ReSize(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);
	ProfilesSubject.VerifyTab(dynamic_cast<GSession*>(this)->GetMaxObjId(pProfile)+1);
}


//------------------------------------------------------------------------------
void GSubjects::ReInit(void)
{
	// Deselect the documents
	RCursor<GDoc> Doc(UsedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
		Doc()->PostNotification(hDocs[oeDeselected]);

	// ReInit the subject
	RNodeCursor<GSubjects,GSubject> Subject(*this);
	for(Subject.Start();!Subject.End();Subject.Next())
		Subject()->ReInit();

	// Clear the documents and profiles assignment
	ProfilesSubject.Clear();
	DocsSubjects.Clear();
	UsedDocs.Clear();
	//Storage->Clear(otSubject);
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	// Deselect the documents
	RCursor<GDoc> Doc(UsedDocs);
	for(Doc.Start();!Doc.End();Doc.Next())
		Doc()->PostNotification(hDocs[oeDeselected]);

	// Clear the documents and profiles assignment
	ProfilesSubject.Clear();
	DocsSubjects.Clear();
	UsedDocs.Clear();

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
	return(Subjects.GetNb());
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

	// If the subjects are not currently loaded -> save the subject if necessary
	if(SaveResults&&(State!=osOnDemand))
		Storage->SaveObj(subject);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbUsedObjs(tObjType type) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	if(type==otDoc)
		return(UsedDocs.GetNb());
	size_t nb(0);
	RNodeCursor<GSubjects,GSubject> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbUsedObjs(pSubject,type);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbObjs(const GSubject*,GDoc* doc) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!Attached)
		return(0);
	return(Attached->GetNb());
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetObjs(const GSubject*,const GDoc* doc) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(doc->GetId()));
	if(Attached)
		return(R::RCursor<GSubject>(*Attached));
	return(R::RCursor<GSubject>());
}


//------------------------------------------------------------------------------
const GSubject* GSubjects::GetObj(const GSubject*,const GDoc* doc,bool one) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	const R::RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!Attached)
		return(0);
	if(one&&(Attached->GetNb()>1))
		mThrowGException("Document has multiple subjects");
	return((*Attached)[0]);
}


//-----------------------------------------------------------------------------
void GSubjects::SetUsed(GSubject* subject,bool select)
{
	if(subject->Used==select)
		return;

	subject->Used=select;
	if(select)
		UsedSubjects.InsertPtr(subject);
	else
	{
		 RContainer<GDoc,false,true> Tmp(subject->UsedDocs);
		 RCursor<GDoc> Doc(Tmp);
		 for(Doc.Start();!Doc.End();Doc.Next())
			  SetUsed(Doc(),subject,false);
		 UsedSubjects.DeletePtr(*subject);
	}

	// If the subjects are not currently loaded -> save the subject if necessary
	if(SaveResults&&(State!=osOnDemand))
		Storage->SaveObj(subject);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetUsedObjs(GDoc** docs)
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(UsedDocs.GetTab(docs,1,UsedDocs.GetMaxPos()));
}


//------------------------------------------------------------------------------
RCursor<GDoc> GSubjects::GetUsedObjs(const GDoc*) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);
	return(RCursor<GDoc>(UsedDocs));
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
	const R::RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(doc->GetId()));
	if(!Attached)
		return(false);
	return(Attached->IsIn(*subject));
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
	UsedDocs.VerifyTab(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);
	DocsSubjects.VerifyTab(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);
	DocsStatus.ReSize(dynamic_cast<GSession*>(this)->GetMaxObjId(pDoc)+1);

	// Look if the document must be insert in CategorizedDocs
	bool Find;
	size_t Pos(subject->Docs.GetIndex(doc,Find));
	if(!Find)
		subject->Docs.InsertPtrAt(doc,Pos,false);

	// Suppose the document is deselected.
	DocsStatus.Set(false,doc->GetId());

	// If the subjects are not currently loaded -> save the subject if necessary
	if(SaveResults&&(State!=osOnDemand))
		Storage->SaveObj(subject);
}


//------------------------------------------------------------------------------
void GSubjects::SetUsed(GDoc* doc,GSubject* subject,bool select)
{
	bool Find;

	// Some Verification (including loading the subjects)
	if(!subject)
		mThrowGException("No subject specified");
	if(!doc)
		mThrowGException("No document specified");
	LoadObjs(pSubject);

	// Get the subjects associated with the document
	R::RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(doc->GetId()));

	if(select)
	{
		// The document is selected
		DocsStatus.Set(true,doc->GetId());
		size_t Pos(UsedDocs.GetIndex(doc,Find));
		if(!Find)
			UsedDocs.InsertPtr(doc);

		// Add an association between the document and the subject
		if(!Attached)
			DocsSubjects.InsertPtrAt(Attached=new R::RContainer<GSubject,false,false>(10,5),doc->GetId(),true);
		Pos=Attached->GetIndex(*subject,Find);
		if(!Find)
			Attached->InsertPtrAt(subject,Pos,false);

		// Add the document in the subject
		Pos=subject->UsedDocs.GetIndex(doc,Find);
		if(!Find)
		{
			subject->UsedDocs.InsertPtrAt(doc,Pos,false);

			// Emit a selection signal if this is the first time the document is selected
			if((Attached->GetNb()==1)&&(State!=osOnDemand))
				doc->PostNotification(hDocs[oeSelected]);
		}
	}
	else
	{
		// Remove the association between a document and a subject
		if(!Attached)
			return;
		Attached->DeletePtr(*subject);

		// Remove the document from the subject
		subject->UsedDocs.DeletePtr(doc);

		// Emit a deselection signal if the document has no subject associated anymore
		if(Attached->GetNb()==0)
		{
			UsedDocs.DeletePtr(*doc);
			DocsStatus.Set(false,doc->GetId());
			if(State!=osOnDemand)
				doc->PostNotification(hDocs[oeDeselected]);
		}
	}

	// If the subjects are not currently loaded -> save the subject if necessary
	if(SaveResults&&(State!=osOnDemand))
		Storage->SaveObj(subject);
}


//------------------------------------------------------------------------------
bool GSubjects::IsUsed(const GDoc* doc) const
{
	const_cast<GSubjects*>(this)->LoadObjs(pSubject);

	// If no subjects -> considered that all documents are used
	if(doc->GetId()>Subjects.GetMaxPos())
		return(true);

	return(DocsStatus[doc->GetId()]);
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

	// If the subjects are not currently loaded -> save the subject if necessary
	if(SaveResults&&(State!=osOnDemand))
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
void GSubjects::Repair(void)
{
	LoadObjs(pSubject);

	RContainer<GSubject,false,false> ToDel(500);            // Subjects to delete
	RContainer<NewGenericSubject,true,false> ToIns(500);    // Subjects to insert
	GSession* Session(dynamic_cast<GSession*>(this));
	if(!Session)
		mThrowGException("The GSubject object is not a GSession one");

	size_t NbNoLeaf(0);                   // Number of no-leaf subjects with documents
	size_t NbLeaf(0);                     // Number of leaf subjects without documents
	RCursor<GSubject> Subject(Subjects);
	for(Subject.Start();!Subject.End();Subject.Next())
	{
		if(Subject()->GetNbSubjects()&&Subject()->Docs.GetNb())
		{
			// If all child subjects are leaf node without -> it is OK.
			bool OK(true);
			RNodeCursor<GSubjects,GSubject> Child(Subject());
			for(Child.Start();(!Child.End())&&OK;Child.Next())
				if(Child()->GetNbSubjects()||Child()->Docs.GetNb())
					OK=false;
			if(!OK)
			{
				NbNoLeaf++;

				// Create a new subject
				GSubject* NewSubject(new GSubject(Session,GetNbNodes()+NbNoLeaf,Subject()->Name+" general"));
				ToIns.InsertPtr(new NewGenericSubject(NewSubject,Subject()));

				// Transfer all the document from Cur() to subject
				RCursor<GDoc> Docs(Subject()->Docs);
				for(Docs.Start();!Docs.End();Docs.Next())
				{
					NewSubject->Docs.InsertPtr(Docs());
					RContainer<GSubject,false,false>* Attached(DocsSubjects.GetPtrAt(Docs()->GetId()));
					Attached->DeletePtr(*Subject());
					Attached->InsertPtr(NewSubject);
				}
				Docs.Set(Subject()->UsedDocs);
				for(Docs.Start();!Docs.End();Docs.Next())
					NewSubject->UsedDocs.InsertPtr(Docs());
				Subject()->Docs.Clear();
				Subject()->UsedDocs.Clear();
			}
		}
		else if(!Subject()->GetNbSubjects()&&!Subject()->Docs.GetNb())
		{
			ToDel.InsertPtr(Subject());
			NbLeaf++;
		}
	}

	// Delete the nodes
	Subject.Set(ToDel);
	for(Subject.Start();!Subject.End();Subject.Next())
	{
		SetUsed(Subject(),true);
		DeleteNode(Subject(),true);
	}

	// Insert the nodes
	RCursor<NewGenericSubject> New(ToIns);
	for(New.Start();!New.End();New.Next())
	{
		InsertObj(New()->Parent,New()->Subject);
		if(New()->Parent->Used)
			 SetUsed(New()->Subject,true);
	}

	// Save if necessary
	if(SaveResults)
	{
		Storage->Clear(otSubject);
		Subject=GetObjs(pSubject);
		for(Subject.Start();!Subject.End();Subject.Next())
			Storage->SaveObj(Subject());
	}

	cout<<"There are "<<GetNbObjs(pSubject)<<" subjects:"<<endl;
	if(NbNoLeaf)
		cout<<"  "<<NbNoLeaf<<" non-leaf subjects have some documents attached"<<endl;
	if(NbLeaf)
		cout<<"  "<<NbLeaf<<" leaf subjects have no documents attached"<<endl;
	cout<<"  "<<static_cast<double>(NbNoLeaf+NbLeaf)*100.0/static_cast<double>(GetNbObjs(pSubject))<<"% of invalid subjects"<<endl;
}
