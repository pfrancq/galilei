/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
//#include <docs/gdocxml.h>
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



//-----------------------------------------------------------------------------
//
// class GDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDoc::GDoc(const char* url,const char* name,unsigned int id,GLang* lang,const char* mime,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf) throw(bad_alloc)
	: URL(url), Name(name), Id(id), N(n), V(v), Ndiff(ndiff), Vdiff(vdiff),
	  Lang(lang), MIMEType(mime), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2),
	  Failed(f)
#if GALILEITEST
	  ,Subjects(2,1)
#endif
{
	LinkSet=new RContainer<GLink,unsigned int,false,true>(5,2);

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


//-----------------------------------------------------------------------------
bool GDoc::HasRepresentation(void) const
{
	return(false);
}



//-----------------------------------------------------------------------------
int GDoc::Compare(const GDoc& doc) const
{
	return(Id-doc.Id);
}


//-----------------------------------------------------------------------------
int GDoc::Compare(const GDoc* doc) const
{
	return(Id-doc->Id);
}


//-----------------------------------------------------------------------------
int GDoc::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GDoc::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
void GDoc::ClearInfos(bool l)
{
	if(l)
		Lang=0;
	N=0;
	Ndiff=0;
	V=0;
	Vdiff=0;
}


//-----------------------------------------------------------------------------
void GDoc::ClearFdbks(void)
{
	Fdbks.Clear();
}


//-----------------------------------------------------------------------------
RString& GDoc::GetURL(void) const
{
	RString* tmp=RString::GetString();

	(*tmp)=URL;
	return(*tmp);
}


//-----------------------------------------------------------------------------
RString& GDoc::GetName(void) const
{
	RString* tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//-----------------------------------------------------------------------------
const R::RDate* GDoc::GetUpdated(void) const
{
	return(&Updated);
}


//-----------------------------------------------------------------------------
const R::RDate* GDoc::GetComputed(void) const
{
	return(&Computed);
}


//-----------------------------------------------------------------------------
const char* GDoc::GetMIMEType(void) const
{
	if(MIMEType.IsEmpty()) return(0);
	return(MIMEType);
}


//-----------------------------------------------------------------------------
void GDoc::SetMIMEType(const char* mime)
{
	MIMEType=mime;
}


//-----------------------------------------------------------------------------
tObjState GDoc::GetState(void) const
{
	return(State);
}


//-----------------------------------------------------------------------------
void GDoc::SetState(tObjState state)
{
	State=state;
}


//-----------------------------------------------------------------------------
void GDoc::SetInfos(GLang *l,unsigned int n,unsigned int nd,unsigned int v,unsigned int vd)
{
	GSubProfile* sub;

	Lang=l;
	N=n;
	Ndiff=nd;
	V=v;
	Vdiff=vd;
	State=osUpdated;
	Computed.SetToday();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		sub=Fdbks()->GetProfile()->GetSubProfile(l);
		if(sub)
			sub->SetState(osModified);
	}
}


//-----------------------------------------------------------------------------
unsigned int GDoc::GetNbFdbks(void) const
{
	return(Fdbks.NbPtr);
}


//-----------------------------------------------------------------------------
GProfDocCursor& GDoc::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
double GDoc::Similarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GDoc::GlobalSimilarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GDoc::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GDoc::GlobalSimilarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GDoc::Similarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GDoc::GlobalSimilarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
void GDoc::AddJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.InsertPtr(j);
}


//-----------------------------------------------------------------------------
unsigned int GDoc::GetNbLinks(void)
{
	unsigned int res = LinkSet->NbPtr;
	return res;
}


//-----------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc) throw(bad_alloc)
{
	GLink* link ;
	link = LinkSet->GetInsertPtr(doc);
	link->AddOccurence();
}


//-----------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc,unsigned int nbOccurs) throw(bad_alloc)
{
	GLink* link ;
	link = LinkSet->GetInsertPtr(doc);
	link->SetOccurs(nbOccurs);
}


//-----------------------------------------------------------------------------
void GDoc::InsertLink(const GDoc* doc,char* /*format*/,char* /*type*/) throw(bad_alloc)
{
	GLink* link ;
	link = LinkSet->GetInsertPtr(doc);
	link->AddOccurence();
//	if (format) link->SetFormat(format);
//	if (type) link->SetType(type);
}



//-----------------------------------------------------------------------------
GLinkCursor& GDoc::GetLinkCursor(void)
{
	GLinkCursor *cur = GLinkCursor::GetTmpCursor();
	cur->Set(LinkSet);
	return(*cur);
}



#if GALILEITEST
//-----------------------------------------------------------------------------
void GDoc::InsertSubject(GSubject* s)
{
	Subjects.InsertPtr(s);
}


//-----------------------------------------------------------------------------
bool GDoc::IsFromSubject(const GSubject* s)
{
	return(Subjects.IsIn(s));
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
GSubjectCursor& GDoc::GetSubjectCursor(void)
{
	GSubjectCursor *cur=GSubjectCursor::GetTmpCursor();
	cur->Set(Subjects);
	return(*cur);
}
#endif


//-----------------------------------------------------------------------------
GDoc::~GDoc(void)
{
	delete LinkSet;
}
