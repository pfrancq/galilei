/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001-2008 by the Université libre de Bruxelles.

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
// include files for R Project
#include <rvectorint.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <glink.h>
#include <glang.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <gstorage.h>
#include <gprofile.h>
#include <gdocstruct.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(const RURI& url,const RString& name,size_t id,GLang* lang,const RString& mime,size_t grpid,const R::RDate& c,const R::RDate& u,const R::RDate& a,size_t nbrecs,size_t nblcs,size_t ownerid)
	: GWeightInfos(60), URL(url), Name(name), Id(id), Struct(0),
	  Lang(lang),MIMEType(mime), Updated(u), Computed(c),
	  Fdbks(0), LinkSet(5,2), OwnerId(ownerid), GroupId(grpid), Attached(a),
	  NbRecs(nbrecs), NbLCs(nblcs)
{
	if(Id!=cNoRef)
		GSession::Event(this,eObjNew);
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
void GDoc::ClearInfos(void)
{
	// Clear the information
	GWeightInfos::Clear();

	// Make sure that it will be re-computed
	Computed=RDate::Null;
}


//------------------------------------------------------------------------------
void GDoc::ClearStruct(void)
{
	if(Struct)
		Struct->Clear();
}


//------------------------------------------------------------------------------
GDocStruct* GDoc::GetStruct(void) const
{
	if((!Struct)&&NbRecs)
	{
		// Create the structure and load it from the database
		GetInfos();
		const_cast<GDoc*>(this)->Struct=new GDocStruct(NbRecs,NbLCs);
		GSession* session=GSession::Get();
		if(session&&session->GetStorage())
			session->GetStorage()->LoadStruct(*const_cast<GDoc*>(this)->Struct,const_cast<GDoc*>(this));
	}
	return(Struct);
}


//------------------------------------------------------------------------------
void GDoc::ReleaseStruct(void)
{
	if(Struct)
	{
		delete Struct;
		Struct=0;
	}
}


//------------------------------------------------------------------------------
void GDoc::SetGroup(size_t groupid)
{
	GroupId=groupid;
	if(GroupId!=cNoRef)
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
void GDoc::SetId(size_t id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a document");
	Id=id;
	GSession::Event(this,eObjNew);
}


//------------------------------------------------------------------------------
R::RVectorInt<size_t,true>* GDoc::GetFdbks(void) const
{
	return(Fdbks);
}


//------------------------------------------------------------------------------
void GDoc::InsertFdbk(size_t id)
{
	if(!Fdbks)
		Fdbks=new R::RVectorInt<size_t,true>(500);
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
size_t GDoc::GetCommonOKProfiles(const GDoc* doc) const
{
	tDocAssessment f;
	GFdbk* cor;
	size_t nb;

	GSession* Session=GSession::Get();
	if(!Session)
		throw GException("No current session");

	// Verify that the two documents have the same language
	if(Lang!=doc->Lang) return(0);
	nb=0;

	if((!Fdbks)||(!doc->Fdbks))
		return(0);

	// Go through the profiles that have asses this documents
	for(Fdbks->Start();!Fdbks->End();Fdbks->Next())
	{
		// Find the corresponding feedback
		GProfile* Profile=Session->GetProfile((*Fdbks)(),true);
		if(!Profile)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));
		cor=Profile->GetFdbk(Id);
		if(!cor)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));

		// If the assessment was not "OK"  -> Nothing
		f=cor->GetFdbk();
		if(!(f & djOK)) continue;

		// Search if the second document was also assessed as relevant by the profile
		cor=Profile->GetFdbk(doc->GetId());
		if(!cor)
			continue;
		f=cor->GetFdbk();
		if(!(f & djOK)) continue;

		// Increase the number of common profiles
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
size_t GDoc::GetCommonProfiles(const GDoc* doc) const
{
	GFdbk* cor;
	size_t nb;

	GSession* Session=GSession::Get();
	if(!Session)
		throw GException("No current session");

	// Verify that the two documents have the same language
	if(Lang!=doc->Lang) return(0);
	nb=0;

	if((!Fdbks)||(!doc->Fdbks))
		return(0);

	// Go through the profiles that have asses this documents
	for(Fdbks->Start();!Fdbks->End();Fdbks->Next())
	{
		// Find the corresponding feedback
		GProfile* Profile=Session->GetProfile((*Fdbks)(),true);
		if(!Profile)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));
		cor=Profile->GetFdbk(Id);
		if(!cor)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));

		// Search if the second document was also assessed by the profile
		cor=Profile->GetFdbk(doc->GetId());
		if(cor)
			nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
size_t GDoc::GetCommonDiffProfiles(const GDoc* doc) const
{
	GFdbk* cor;
	size_t nb;
	bool IsOK1;
	bool IsOK2;

	GSession* Session=GSession::Get();
	if(!Session)
		throw GException("No current session");

	// Verify that the two documents have the same language
	if(Lang!=doc->Lang) return(0);
	nb=0;

	if((!Fdbks)||(!doc->Fdbks))
		return(0);

	// Go through the profiles that have asses this documents
	for(Fdbks->Start();!Fdbks->End();Fdbks->Next())
	{
		// Find the corresponding feedback
		GProfile* Profile=Session->GetProfile((*Fdbks)(),true);
		if(!Profile)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));
		cor=Profile->GetFdbk(Id);
		if(!cor)
			throw GException("Document "+RString::Number(Id)+" was not assessed by profile "+RString::Number((*Fdbks)()));
		IsOK1=(cor->GetFdbk() & djOK);

		// Search if the second document was also assessed differently by the profile
		cor=Profile->GetFdbk(doc->GetId());
		if(!cor)
			continue;
		IsOK2=(cor->GetFdbk() & djOK);

		// Increase the number of common profiles
		if((IsOK1&&(!IsOK2))||((!IsOK1)&&IsOK2))
			nb++;
	}
	return(nb);
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
void GDoc::Update(GLang* lang,GWeightInfos& infos,GDocStruct& docstruct,bool ram)
{
	// If document had a language -> remove its references
	if(Lang&&(Id!=cNoRef))
		DelRefs(otDoc);

	// Assign language and information
	GWeightInfos::Clear();
	Lang=lang;
	NbRecs=docstruct.GetNbRecs();
	NbLCs=docstruct.GetNbLCs();
	if(Id!=cNoRef)
	{
		if(ram)
			State=osUpdated;
		else
			State=osNeedLoad;
		Computed.SetToday();

		// Update the profiles that have assessed it.
		if(GSession::Get())
			GSession::Get()->UpdateProfiles(Id);
	}
	else
		State=osUpToDate;

	// If document has a language -> update its references
	if(Lang&&(Id!=cNoRef))
	{
		GConceptType* type(0);
		RCursor<GWeightInfo> ptr(infos);
		for(ptr.Start();!ptr.End();ptr.Next())
		{
			// Look if the type of the concept have changed since that the last concept treated
			if(ptr()->GetConcept()->GetType()!=type)
			{
				// Yes -> A new object uses this concept type.
				type=ptr()->GetConcept()->GetType();
				type->IncRef(otDoc);
			}

			// AddRef for the concept
			type->IncRef(ptr()->GetConcept()->GetId(),otDoc);
		}
	}

	if(ram)
	{
		Transfer(infos);
		delete Struct;
		Struct=0;
		Struct=new GDocStruct(docstruct);
	}

	// Clear infos
	infos.Clear();

	// Emit an event that it was modified
	if(Id!=cNoRef)
		GSession::Event(this,eObjModified);
}


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
	GSession::Event(this,eObjDelete);
	if(Struct)
		delete Struct;
	try
	{
		// Delete feedbacks vector
		delete Fdbks;

		// If document have a language -> remove its references
		if(Lang&&(State==osDelete))  // The object has modified the references count but was not saved
			DelRefs(otDoc);
	}
	catch(...)
	{
	}
}
