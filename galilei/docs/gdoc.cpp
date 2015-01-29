/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#include <gconcepttree.h>
#include <gconceptnode.h>
#include <gconceptnodes.h>
#include <gdoc.h>
#include <gtopic.h>
#include <glang.h>
#include <gsession.h>
#include <gprofile.h>
#include <gfdbk.h>
#include <gdescriptionobject.hh>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(GSession* session,const RURI& uri,const RString& name,GLang* lang,const RString& mime)
	: GDescriptionObject<GDoc>(session,cNoRef,0,otDoc,name), URI(uri),
	  Lang(lang),MIMEType(mime), Updated(RDate::GetToday()), Computed(RDate::Null),
	  Fdbks(0), GroupId(0), Attached(RDate::Null),
	  StructId(0), Tree(0)
{
	// Verify if the topic exists in memory
	if(GroupId)
	{
		GTopic* grp(Session->GetObj(pTopic,GroupId,false,false));
		if(grp)
			grp->InsertObj(this);
	}
}


//------------------------------------------------------------------------------
GDoc::GDoc(GSession* session,const RURI& uri,const RString& name,size_t id,size_t blockid,size_t structid,GLang* lang,const RString& mime,size_t grpid,const RDate& c,const RDate& u,const RDate& a)
	: GDescriptionObject<GDoc>(session,id,blockid,otDoc,name), URI(uri),
	  Lang(lang),MIMEType(mime), Updated(u), Computed(c),
	  Fdbks(0), GroupId(grpid), Attached(a),
	  StructId(structid), Tree(0)
{
	// Verify if the topic exists in memory
	mAssert(GroupId!=cNoRef);
	if(GroupId)
	{
		GTopic* grp(Session->GetObj(pTopic,GroupId,false,false));
		if(grp)
			grp->InsertObj(this);
	}
	NotificationCenter.PostNotification<GDoc*>("DocChanged",this);
}


//------------------------------------------------------------------------------
R::RString GDoc::GetSearchStr(void) const
{
	return(URI());
}


//------------------------------------------------------------------------------
bool GDoc::MustCompute(void) const
{
	if(Updated>Computed)
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
int GDoc::Compare(const GDoc& doc) const
{
	return(CompareIds(Id,doc.Id));
}


//------------------------------------------------------------------------------
int GDoc::Compare(const GDoc* doc) const
{
	return(CompareIds(Id,doc->Id));
}


//------------------------------------------------------------------------------
int GDoc::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int GDoc::Compare(const R::RString& name) const
{
	return(URI.Compare(name));
}


//------------------------------------------------------------------------------
int GDoc::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GDoc::Clear(bool disk)
{
	// Clear the information
	GDescriptionObject<GDoc>::Clear(disk);

	if(disk)
		StructId=0;

	if(Tree)
	{
		delete Tree;
		Tree=0;
	}

	// Make sure that it will be re-computed
	Computed=RDate::Null;
}


//------------------------------------------------------------------------------
void GDoc::ReleaseTree(void)
{
	if(Tree)
	{
		delete Tree;
		Tree=0;
	}
}


//------------------------------------------------------------------------------
void GDoc::LoadTree(GConceptTree* &tree) const
{
	if(StructId)
		Session->LoadTree(pDoc,tree,StructId,Id);
	else
		tree=new GConceptTree(Id,0,0);
}


//------------------------------------------------------------------------------
GConceptTree* GDoc::GetTree(void) const
{
	if(!Tree)
		const_cast<GDoc*>(this)->LoadTree(const_cast<GConceptTree* &>(Tree));
	return(Tree);
}


//------------------------------------------------------------------------------
void GDoc::SetTree(GConceptTree& tree)
{
	ReleaseTree();
	Tree=new GConceptTree(tree);
}


//------------------------------------------------------------------------------
void GDoc::SetGroup(size_t groupid)
{
	mAssert(groupid!=cNoRef);
	GroupId=groupid;
	if(GroupId)
		Attached.SetToday();
}


//------------------------------------------------------------------------------
void GDoc::SetURI(RURI uri)
{
	URI=uri;
}


//------------------------------------------------------------------------------
void GDoc::SetMIMEType(const RString& mime)
{
	MIMEType=mime;
}


//------------------------------------------------------------------------------
R::RNumCursor<size_t> GDoc::GetFdbks(void) const
{
	if(Fdbks)
		return(R::RNumCursor<size_t>(*Fdbks));
	return(R::RNumCursor<size_t>());
}


//------------------------------------------------------------------------------
bool GDoc::InsertFdbk(size_t profileid)
{
	if(!Fdbks)
		Fdbks=new R::RNumContainer<size_t,true>(100);
	bool NewFdbk;
	size_t Idx(Fdbks->GetIndex(profileid,NewFdbk));
	Fdbks->InsertAt(profileid,Idx,true);
	return(NewFdbk);
}


//------------------------------------------------------------------------------
void GDoc::DeleteFdbk(size_t profileid)
{
	if(Fdbks)
		Fdbks->Delete(profileid);
}


//------------------------------------------------------------------------------
void GDoc::ClearFdbks(void)
{
 	if(Fdbks)
		Fdbks->Clear();
}


//------------------------------------------------------------------------------
double GDoc::GetAgreementRatio(const GDoc* doc,size_t nbmin) const
{
	double nbcommon(0.0), nbagree(0.0);

	// If no feedback -> null ratio
	if((!Fdbks)||(!doc->Fdbks))
		return(0.0);

	// Go through the profiles that have asses this document
	RNumCursor<size_t> fdbks(*Fdbks);
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// Find the corresponding feedback (first find the profile)
		GProfile* Profile(Session->GetObj(pProfile,fdbks(),true));
		if(!Profile)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number(fdbks()));
		GFdbk* f1(Profile->GetFdbk(Id));
		if(!f1)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number(fdbks()));

		// Look for the other assessment of the same profile for the other document.
		GFdbk* f2(Profile->GetFdbk(doc->GetId()));
		if(!f2) continue;

		nbcommon+=1.0;  // A common document
		if((f1->GetFdbk()==ftRelevant)&&(f2->GetFdbk()==ftRelevant))
			nbagree+=1.0;
	}
	if(nbcommon<nbmin)
		return(0.0);
	return(nbagree/nbcommon);
}


//------------------------------------------------------------------------------
double GDoc::GetDisagreementRatio(const GDoc* doc,size_t nbmin) const
{
	double nbcommon(0.0), nbdisagree(0.0);

	// If no feedback -> null ratio
	if((!Fdbks)||(!doc->Fdbks))
		return(0.0);

	// Go through the profiles that have asses this document
	RNumCursor<size_t> fdbks(*Fdbks);
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// Find the corresponding feedback (first find the profile)
		GProfile* Profile(Session->GetObj(pProfile,fdbks(),true));
		if(!Profile)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number(fdbks()));
		GFdbk* f1(Profile->GetFdbk(Id));
		if(!f1)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number(fdbks()));

		// Look for the other assessment of the same profile for the other document.
		GFdbk* f2(Profile->GetFdbk(doc->GetId()));
		if(!f2) continue;

		nbcommon+=1.0;  // A common document
		if(((f1->GetFdbk()==ftRelevant)&&(f2->GetFdbk()!=ftRelevant))||((f1->GetFdbk()!=ftRelevant)&&(f2->GetFdbk()==ftRelevant)))
			nbdisagree+=1.0;
	}
	if(nbcommon<nbmin)
		return(0.0);
	return(nbdisagree/nbcommon);
}


//------------------------------------------------------------------------------
size_t GDoc::GetNbFdbks(void) const
{
	if(Fdbks)
		return(Fdbks->GetNb());
	return(0);
}


//------------------------------------------------------------------------------
void GDoc::SetUpdated(RDate& date)
{
	GStorage* Storage(Session->GetStorage());
	Updated=date;
	if((!Storage->IsAllInMemory())||(Session->MustSaveResults()))
		Storage->SaveObj(this);
}


//------------------------------------------------------------------------------
void GDoc::Update(GLang* lang,GDescription& desc,GConceptTree& tree)
{
	bool Save(Session->MustSaveResults());  // Must the results be saved on disk?
	bool NullDesc;                          // The description must not stayed in memory?
	bool NullTree;                          // The tree must not stayed in memory?

	// Look if the document is internal one : Modify the references and indexes
	GetVectors();  // Make sure the vectors are loaded
	if(Id!=cNoRef)
	{
		// Emit an event that it is about to be updated
		PostNotification(hDocs[oeAboutToBeUpdated]);

		// Look if the index must be modified
		if(Save&&Session->DoCreateIndex(pDoc))
			Session->UpdateIndex(pDoc,*this,Id,false);
	}

	// Assign language
	Lang=lang;

	// The description must be saved only for external documents or when a description is already loaded
	if((Id==cNoRef)||Vectors)
	{
		GDescription::operator=(desc);
		NullDesc=false;
	}
	else
	{
		Vectors=desc.Vectors;
		NullDesc=true;
	}

	// The tree must be saved only for external documents or when a description is already loaded
	if((Id==cNoRef)||Tree)
	{
		ReleaseTree();
		Tree=new GConceptTree(tree);
		NullTree=false;
	}
	else
	{
		Tree=&tree;
		NullTree=true;
	}

	// Set the computed date and the status
	Computed.SetToday();
	State=osLatest;

	// Look if the document is internal one : Modify the references and indexes
	if(Id!=cNoRef)
	{
		// Update the profiles that have assessed it.
		Session->UpdateProfiles(Id);

		// Look if the index must be modified and the description and tree saved
		if(Save)
		{
			if(Session->DoCreateIndex(pDoc))
				Session->UpdateIndex(pDoc,desc,Id,true);

			if(desc.IsDefined())
				Session->SaveDesc(pDoc,*desc.Vectors,BlockId,Id);

			if(Session->DoCreateTree(pDoc))
				Session->SaveTree(pDoc,*Tree,StructId,Id);

			Session->GetStorage()->SaveObj(this);
		}

		// Emit an event that it was updated
		PostNotification(hDocs[oeUpdated]);

		// Verify if description and tree must stay in memory
		if(NullDesc)
			Vectors=0;
		if(NullTree)
			Tree=0;
	}

	// Clear the description
	desc.Clear();
	tree.Clear();
}


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
	// Delete feedbacks vector
	delete Fdbks;
	delete Tree;
}
