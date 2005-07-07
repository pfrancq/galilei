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
#include <docs/gdoc.h>
#include <docs/glink.h>
#include <infos/glang.h>
#include <profiles/gprofileproxy.h>
#include <infos/gweightinfo.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,unsigned int id,GLang* lang,const RString& mime,const RString& u,const RString& a,unsigned int f,unsigned int ownerid,unsigned int nbf) throw(std::bad_alloc)
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
}


//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,const RString& mime) throw(std::bad_alloc)
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
void GDoc::SetName(const R::RString& name) throw(std::bad_alloc)
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
void GDoc::SetId(unsigned int id) throw(GException)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a document");
	Id=id;
}


//------------------------------------------------------------------------------
RCursor<GProfileProxy> GDoc::GetFdbks(void)
{
	return(RCursor<GProfileProxy>(Fdbks));
}


//------------------------------------------------------------------------------
RCursor<GWeightInfo> GDoc::GetWeightInfoCursor(void)
{
	return(RCursor<GWeightInfo>(*this));
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(doc));
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(doc,otDoc,Lang));
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GSubProfile* sub) const
{
	return(GWeightInfos::Similarity(sub));
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GSubProfile* sub) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(sub,otDocSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GGroup* grp) const
{
	return(GWeightInfos::Similarity(grp));
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GGroup* grp) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(grp,otDocGroup,Lang));
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
	RCursor<GProfileProxy> Cur(Fdbks);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->HasUpdate(Id,computed);
}


//------------------------------------------------------------------------------
void GDoc::InsertFdbk(unsigned int id) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(GALILEI::New<GProfileProxy>(id));
}


//------------------------------------------------------------------------------
void GDoc::DeleteFdbk(unsigned int id) throw(std::bad_alloc)
{
	Fdbks.DeletePtr(Fdbks.GetPtr<unsigned int>(id));
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
unsigned int GDoc::GetNbLinks(void)
{
	unsigned int res = LinkSet.GetNb();
	return(res);
}


//------------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc) throw(std::bad_alloc)
{
	GLink* link ;
	link = LinkSet.GetInsertPtr(doc);
	link->IncOccurs();
}


//------------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc,unsigned int nbOccurs) throw(std::bad_alloc)
{
	GLink* link ;
	link = LinkSet.GetInsertPtr(doc);
	link->SetOccurs(nbOccurs);
}


//------------------------------------------------------------------------------
R::RCursor<GLink> GDoc::GetLinkCursor(void)
{
	return(R::RCursor<GLink>(LinkSet));
}


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
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
