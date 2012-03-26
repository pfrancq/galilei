/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#include <glink.h>
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
void GDoc::PrivateInit(void)
{
	SetState(osNew);
	SaveDesc();
	AddRefs(Session,otDoc);
	SetId(cNoRef);
}


//------------------------------------------------------------------------------
GDoc::GDoc(GSession* session,const RURI& uri,const RString& name,GLang* lang,const RString& mime)
	: GDescriptionObject<GDoc>(session,cNoRef,0,otDoc,name,osNew), URI(uri),
	  Lang(lang),MIMEType(mime), Updated(RDate::GetToday()), Computed(RDate::Null),
	  Fdbks(0), LinkSet(5,2), GroupId(0), Attached(RDate::Null),
	  StructId(0)
{
	// Verify if the topic exists in memory
	if(GroupId)
	{
		GTopic* grp=Session->GetObj(pTopic,GroupId,false,false);
		if(grp)
			grp->InsertObj(this);
	}
}


//------------------------------------------------------------------------------
GDoc::GDoc(GSession* session,const RURI& uri,const RString& name,size_t id,size_t blockid,size_t structid,GLang* lang,const RString& mime,size_t grpid,const RDate& c,const RDate& u,const RDate& a)
	: GDescriptionObject<GDoc>(session,id,blockid,otDoc,name,osNew), URI(uri),
	  Lang(lang),MIMEType(mime), Updated(u), Computed(c),
	  Fdbks(0), LinkSet(5,2), GroupId(grpid), Attached(a),
	  StructId(structid)
{
	// Verify if the topic exists in memory
	RAssert(GroupId!=cNoRef);
	if(GroupId)
	{
		GTopic* grp=Session->GetObj(pTopic,GroupId,false,false);
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
int GDoc::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GDoc::ClearInfos(bool disk)
{
	// Clear the information
	GDescriptionObject<GDoc>::Clear(disk);

	// Make sure that it will be re-computed
	Computed=RDate::Null;
}


//------------------------------------------------------------------------------
void GDoc::ClearTree(bool disk)
{
	if(disk)
		StructId=0;
}


//------------------------------------------------------------------------------
void GDoc::LoadTree(GConceptTree* &tree) const
{
	Session->LoadTree(pDoc,tree,StructId,Id);
}


//------------------------------------------------------------------------------
void GDoc::SetGroup(size_t groupid)
{
	RAssert(groupid!=cNoRef);
	GroupId=groupid;
	if(GroupId)
		Attached.SetToday();
}


//------------------------------------------------------------------------------
RDate GDoc::GetAttached(void) const
{
	return(Attached);
}


//------------------------------------------------------------------------------
void GDoc::SetName(const R::RString& name)
{
	Name=name;
}


//------------------------------------------------------------------------------
RDate GDoc::GetUpdated(void) const
{
	return(Updated);
}


//------------------------------------------------------------------------------
RDate GDoc::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
RString GDoc::GetMIMEType(void) const
{
	return(MIMEType);
}


//------------------------------------------------------------------------------
void GDoc::SetMIMEType(const RString& mime)
{
	MIMEType=mime;
}


//------------------------------------------------------------------------------
R::RNumContainer<size_t,true>* GDoc::GetFdbks(void) const
{
	return(Fdbks);
}


//------------------------------------------------------------------------------
void GDoc::InsertFdbk(size_t id)
{
	if(!Fdbks)
		Fdbks=new R::RNumContainer<size_t,true>(500);
	Fdbks->Insert(id);
}


//------------------------------------------------------------------------------
void GDoc::DeleteFdbk(size_t id)
{
	if(Fdbks)
		Fdbks->Delete(id);
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
size_t GDoc::GetNbLinks(void) const
{
	return(LinkSet.GetNb());
}


//------------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc,size_t nboccurs)
{
	GLink* link=LinkSet.GetInsertPtr(doc);
	if(nboccurs)
		link->SetOccurs(nboccurs);
}


//------------------------------------------------------------------------------
R::RCursor<GLink> GDoc::GetLinks(void) const
{
	return(R::RCursor<GLink>(LinkSet));
}


//------------------------------------------------------------------------------
void GDoc::Update(GLang* lang,GDescription& desc,bool ram,bool delref)
{
	// Look if the references must be modified
	if(delref&&(!ram))
	{
		DelRefs(Session,otDoc);
		if(Session->DoCreateIndex(pDoc))
			Session->UpdateIndex(pDoc,desc,Id,false);
	}

	// Assign language and information
	Lang=lang;
	GDescription::operator=(desc);

	if(Id!=cNoRef)
	{
		if(ram)
			State=osUpdated;
		else
			State=osNeedLoad;
		Computed.SetToday();

		// Update the profiles that have assessed it.
		Session->UpdateProfiles(Id);
	}
	else
		State=osUpToDate;

	// Look if the references must be modified
	if(!ram)
	{
		AddRefs(Session,otDoc);
		if(Session->DoCreateIndex(pDoc))
			Session->UpdateIndex(pDoc,desc,Id,true);
	}

	desc.Clear(); // Clear the description

	// Emit an event that it was modified
	if(Id!=cNoRef)
		Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
	// Delete feedbacks vector
	delete Fdbks;
}
