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
#include <docs/gdocxml.h>
#include <langs/glang.h>
#include <filters/gmimefilter.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RXML;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class GDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc(const char* url,const char* name,unsigned int id,GLang* lang,GMIMEFilter* t,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf) throw(bad_alloc)
	: URL(url), Name(name), Id(id), N(n), V(v), Ndiff(ndiff), Vdiff(vdiff),
	  Lang(lang), Type(t), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2),
	  Failed(f)
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


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc& doc) const
{
	return(Id-doc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc* doc) const
{
	return(Id-doc->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::ClearInfos(bool l)
{
	if(l)
		Lang=0;
	N=0;
	Ndiff=0;
	V=0;
	Vdiff=0;
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::ClearFdbks(void)
{
	Fdbks.Clear();
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::SetInfos(GLang *l,unsigned int n,unsigned int nd,unsigned int v,unsigned int vd)
{
	Lang=l;
	N=n;
	Ndiff=nd;
	V=v;
	Vdiff=vd;
	State=osUpdated;
	Computed.SetToday();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		Fdbks()->GetProfile()->SetState(osModified);
}


//-----------------------------------------------------------------------------
GProfDocCursor& GALILEI::GDoc::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::Similarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::GlobalSimilarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::GlobalSimilarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::Similarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GDoc::GlobalSimilarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::AddJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.InsertPtr(j);
}


//-----------------------------------------------------------------------------
GALILEI::GDoc::~GDoc(void)
{
}
