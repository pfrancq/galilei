/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#if GALILEITEST
	#include <groups/gsubject.h>
#endif
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDoc::GDoc(const RString& url,const RString& name,unsigned int id,GLang* lang,const RString& mime,const RString& u,const RString& a,unsigned int f,unsigned int nbf) throw(std::bad_alloc)
	: URL(url), Name(name), Id(id),
	  Lang(lang), MIMEType(mime), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2),
	  Failed(f), LinkSet(5,2)
#if GALILEITEST
	  ,Subjects(2,1)
#endif
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
	: URL(url), Name(name), Id(cNoRef),
	  Lang(0), MIMEType(mime), Updated(), Computed(), Fdbks(50,25),
	  Failed(0), LinkSet(5,2)
#if GALILEITEST
	  ,Subjects(2,1)
#endif
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
void GDoc::ClearInfos(bool l)
{
	if(l)
		Lang=0;
}


//------------------------------------------------------------------------------
void GDoc::ClearFdbks(void)
{
	Fdbks.Clear();
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
void GDoc::SetLang(GLang *l)
{
	GSubProfile* sub;

	Lang=l;
	State=osUpdated;
	Computed.SetToday();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		sub=Fdbks()->GetProfile()->GetSubProfile(l);
		if(sub)
			sub->SetState(osModified);
	}
}


//------------------------------------------------------------------------------
void GDoc::SetId(unsigned int id) throw(GException)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a document");
	Id=id;
}


//------------------------------------------------------------------------------
unsigned int GDoc::GetNbFdbks(void) const
{
	return(Fdbks.NbPtr);
}


//------------------------------------------------------------------------------
GProfDocCursor GDoc::GetProfDocCursor(void)
{
	GProfDocCursor cur(Fdbks);
	return(cur);
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GDoc*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GDoc*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GSubProfile*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GDoc::Similarity(const GGroup*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GDoc::SimilarityIFF(const GGroup*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
void GDoc::AddAssessment(GProfDoc* j) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(j);
}


//------------------------------------------------------------------------------
unsigned int GDoc::GetNbLinks(void)
{
	unsigned int res = LinkSet.NbPtr;
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
GLinkCursor GDoc::GetLinkCursor(void)
{
	GLinkCursor cur(LinkSet);
	return(cur);
}


#if GALILEITEST
//------------------------------------------------------------------------------
void GDoc::InsertSubject(GSubject* s)
{
	Subjects.InsertPtr(s);
}


//------------------------------------------------------------------------------
bool GDoc::IsFromSubject(const GSubject* s)
{
	return(Subjects.IsIn(s));
}


//------------------------------------------------------------------------------
bool GDoc::IsFromParentSubject(const GSubject* s)
{
	GSubjectCursor Sub;

	if(!s->Parent) return(false);
	Sub.Set(s->Parent);
	for(Sub.Start();!Sub.End();Sub.Next())
	{
		if(Sub()==s) continue;
		if(Subjects.IsIn(Sub())) return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
GSubjectCursor GDoc::GetSubjectCursor(void)
{
	GSubjectCursor cur(Subjects);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GDoc::GetNbSubjects(void)
{
	return(Subjects.GetNb());
}


#endif


//------------------------------------------------------------------------------
GDoc::~GDoc(void)
{
}
