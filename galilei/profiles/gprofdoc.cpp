/*

	GALILEI Research Project

	GProfDoc.cpp

	Profile's Judgement of a document - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

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
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GProfDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GProfDoc::GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,const char* date)
  : Doc(doc), Profile(prof), Fdbk(fdbk), Updated(date)
{
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc& profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc.Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc.Profile->GetId());
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GProfDoc* profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc->Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc->Profile->GetId());
}


//---------------------------------------------------------------------------
int GALILEI::GProfDoc::Compare(const GDoc* doc) const
{
	return(Doc->GetId()-doc->GetId());
}


//---------------------------------------------------------------------------
double GALILEI::GProfDoc::Similarity(void)
{
	GSubProfile* sub;
	GLang* lang;

	lang=Doc->GetLang();
	if(!lang) return(0.0);
	sub=Profile->GetPtr<const GLang*>(lang);
	if(!sub) return(0.0);
	return(sub->Similarity(Doc));
}
