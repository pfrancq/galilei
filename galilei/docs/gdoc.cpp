/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,unsigned int id,GLang* lang,const RString& mime,const R::RDate& u,const R::RDate& a,unsigned int f,unsigned int ownerid)
	:  GWeightInfos(60), URL(url), Name(name), Id(id),
	  Lang(lang), MIMEType(mime), Updated(u), Computed(a), Fdbks(0),
	  Failed(f), LinkSet(5,2), OwnerId(ownerid)
{
	GSession::Event(this,eObjNewMem);
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
	return(Id-doc.Id);
}


//------------------------------------------------------------------------------
int GDoc::Compare(const GDoc* doc) const
{
	return(Id-doc->Id);
}


//------------------------------------------------------------------------------
int GDoc::Compare(const unsigned id) const
{
	return(Id-id);
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
	Computed=RDate::null;
}


//------------------------------------------------------------------------------
void GDoc::LoadInfos(void) const
{
	RContainer<GWeightInfo,false,true> Infos(1000,500);
	GSession* session=GSession::Get();
	if(session&&session->GetStorage())
		session->GetStorage()->LoadInfos(Infos,Lang,otDoc,Id);
	const_cast<GDoc*>(this)->Update(Lang,&Infos,false);
}


//------------------------------------------------------------------------------
RString GDoc::GetURL(void) const
{
	return(URL);
}


//------------------------------------------------------------------------------
RString GDoc::GetName(void) const
{
	return(Name);
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
void GDoc::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a document");
	Id=id;
}


//------------------------------------------------------------------------------
R::RVectorInt<true>* GDoc::GetFdbks(void) const
{
	return(Fdbks);
}


//------------------------------------------------------------------------------
void GDoc::Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed)
{
	// If document had a language -> remove its references
	if(computed&&Lang)
		DelRefs(otDoc,Lang);

	// Assign language and information
	GWeightInfos::Clear();
	Lang=lang;
	if(computed)
	{
		State=osUpdated;
		Computed.SetToday();

		// Update the profiles that have assessed it.
		if(GSession::Get())
			GSession::Get()->UpdateProfiles(Id,Lang);
	}
	else
		State=osUpToDate;
	GWeightInfos::operator=(*infos);

	// Clear infos
	infos->Clear();

	// If document has a language -> update its references
	if(computed&&Lang)
		AddRefs(otDoc,Lang);

	// Emit an event that it was modified
	GSession::Event(this,eObjModified);
}


//------------------------------------------------------------------------------
void GDoc::InsertFdbk(unsigned int id)
{
	if(!Fdbks)
		Fdbks=new R::RVectorInt<true>(500);
	Fdbks->Insert(id);
}


//------------------------------------------------------------------------------
void GDoc::DeleteFdbk(unsigned int id)
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
unsigned int GDoc::GetCommonOKProfiles(const GDoc* doc) const
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;

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
unsigned int GDoc::GetCommonProfiles(const GDoc* doc) const
{
	GFdbk* cor;
	unsigned int nb;

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
unsigned int GDoc::GetCommonDiffProfiles(const GDoc* doc) const
{
	GFdbk* cor;
	unsigned int nb;
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
unsigned int GDoc::GetNbFdbks(void) const
{
	if(Fdbks)
		return(Fdbks->GetNbInt());
	return(0);
}


//------------------------------------------------------------------------------
unsigned int GDoc::GetNbLinks(void) const
{
	return(LinkSet.GetNb());
}


//------------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc,unsigned int nboccurs)
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
GDoc::~GDoc(void)
{
	GSession::Event(this,eObjDeleteMem);
	try
	{
		// Delete feedbacks vector
		delete Fdbks;

		// If document have a language -> remove its references
		if(Lang&&(State==osDeleteMem))  // The object has modified the references count but was not saved
			DelRefs(otDoc,Lang);
	}
	catch(...)
	{
	}
}
