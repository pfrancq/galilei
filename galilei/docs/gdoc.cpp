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
// include files for GALILEI
#include <gdoc.h>
#include <glink.h>
#include <glang.h>
#include <gweightinfo.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,unsigned int id,GLang* lang,const RString& mime,const RString& u,const RString& a,unsigned int f,unsigned int ownerid,unsigned int nbf)
	:  GWeightInfos(60), URL(url), Name(name), Id(id),
	  Lang(lang), MIMEType(mime), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2),
	  Failed(f), LinkSet(5,2)/*,Subjects(2,1)*/, OwnerId(ownerid)
{
	if(Updated>Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
	GSession::Event(this,eObjCreated);
}


//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,const RString& mime)
	: GWeightInfos(60), URL(url), Name(name), Id(cNoRef),
	  Lang(0), MIMEType(mime), Updated(), Computed(), Fdbks(50,25),
	  Failed(0), LinkSet(5,2)//,Subjects(2,1)
{
	if(Updated>Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
}


//------------------------------------------------------------------------------
bool GDoc::IsDefined(void) const
{
	return(!GWeightInfos::IsEmpty());
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
tObjState GDoc::GetState(void) const
{
	return(State);
}


//------------------------------------------------------------------------------
void GDoc::SetState(tObjState state)
{
	State=state;
}


//------------------------------------------------------------------------------
void GDoc::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a document");
	Id=id;
}


//------------------------------------------------------------------------------
RCursor<GProfile> GDoc::GetFdbks(void) const
{
	return(RCursor<GProfile>(Fdbks));
}


//------------------------------------------------------------------------------
void GDoc::Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed)
{
	// If document had a language (and a dictionnary) -> remove its references
	if(Lang&&Lang->GetDict())
		DelRefs(otDoc,Lang);

	// Assign language and information
	GWeightInfos::Clear();
	Lang=lang;
	if(computed)
	{
		State=osUpdated;
		Computed.SetToday();
	}
	GWeightInfos::operator=(*infos);

	// Clear infos
	infos->Clear();

	// if document has a language (and a dictionnary) -> update its references
	if(Lang&&Lang->GetDict())
		AddRefs(otDoc,Lang);

	// Signal to the profiles that the document has changed
	RCursor<GProfile> Cur(Fdbks);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->HasUpdate(Id,computed);

	// Emit an event that it was modified
	GSession::Event(this,eObjModified);
}


//------------------------------------------------------------------------------
void GDoc::InsertFdbk(GProfile* prof)
{
	Fdbks.InsertPtr(prof);
}


//------------------------------------------------------------------------------
void GDoc::DeleteFdbk(unsigned int id)
{
	Fdbks.DeletePtr(id);
}


//------------------------------------------------------------------------------
void GDoc::ClearFdbks(void)
{
 	Fdbks.Clear();
}


//------------------------------------------------------------------------------
unsigned int GDoc::GetNbFdbks(void) const
{
	return(Fdbks.GetNb());
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
R::RCursor<GLink> GDoc::GetLinkCursor(void) const
{
	return(R::RCursor<GLink>(LinkSet));
}


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
	GSession::Event(this,eObjDeleted);
	try
	{
		// If document have a language -> remove its references
		if(Lang)
			DelRefs(otDoc,Lang);
	}
	catch(...)
	{
	}
}
