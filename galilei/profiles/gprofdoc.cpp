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
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <docs/gdoc.h>
using namespace GALILEI;
using namespace R;



//---------------------------------------------------------------------------
//
// class GProfDoc
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GProfDoc::GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,const char* date)
  : Doc(doc), Profile(prof), Fdbk(fdbk), Updated(date)
{
}


//---------------------------------------------------------------------------
GProfDoc::GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,RDate& date)
  : Doc(doc), Profile(prof), Fdbk(fdbk), Updated(date)
{
}


//---------------------------------------------------------------------------
int GProfDoc::Compare(const GProfDoc& profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc.Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc.Profile->GetId());
}


//---------------------------------------------------------------------------
int GProfDoc::Compare(const GProfDoc* profdoc) const
{
	int diff;

	diff=Doc->GetId()-profdoc->Doc->GetId();
	if(diff) return(diff);
	return(Profile->GetId()-profdoc->Profile->GetId());
}


//---------------------------------------------------------------------------
int GProfDoc::Compare(const GDoc* doc) const
{
	return(Doc->GetId()-doc->GetId());
}


//---------------------------------------------------------------------------
void GProfDoc::UpdateFdbk(tDocJudgement fdbk,const char* date)
{
	Fdbk=fdbk;
	Updated=date;
}


//---------------------------------------------------------------------------
void GProfDoc::UpdateFdbk(tDocJudgement fdbk,RDate& date)
{
	Fdbk=fdbk;
	Updated=date;
}


//---------------------------------------------------------------------------
tDocJudgement GProfDoc::GetFdbk(void) const
{
	return(Fdbk);
}


//---------------------------------------------------------------------------
GDoc* GProfDoc::GetDoc(void) const
{
	return(Doc);
}


//---------------------------------------------------------------------------
GProfile* GProfDoc::GetProfile(void) const
{
	return(Profile);
}


//---------------------------------------------------------------------------
RDate& GProfDoc::GetUpdated(void) const
{
	RDate* d=RDate::GetDate();

	(*d)=Updated;
	return(*d);
}


//---------------------------------------------------------------------------
bool GProfDoc::FdbkTreated(void) const
{
	GSubProfile* sub;
	GLang* lang;

	lang=Doc->GetLang();
	if(!lang) return(false);
	sub=Profile->GetPtr<const GLang*>(lang);
	if(!sub) return(false);
	if(Updated>sub->GetComputed()) return(false);
	return(true);
}


//---------------------------------------------------------------------------
double GProfDoc::Similarity(void)
{
	GSubProfile* sub;
	GLang* lang;

	lang=Doc->GetLang();
	if(!lang) return(0.0);
	sub=Profile->GetPtr<const GLang*>(lang);
	if(!sub) return(0.0);
	return(sub->Similarity(Doc));
}


//-----------------------------------------------------------------------------
tDocJudgement GProfDoc::ErrorJudgment(tDocJudgement fdbk,double PercErr,RRandom* rand)
{
	double random=rand->Value()*100+1.0;

	// If there is Random change the judgment.
	if(random<PercErr)
	{
		random=rand->Value()*100+1.0;;
		switch(fdbk & djMaskJudg)
		{
			case djOK:
				if(random<25.0)
					return(djOutScope);
				else
					return(djKO);
			case djKO:
				if(random<50.0)
					return(djOK);
				else
					return(djOutScope);
			case djOutScope:
				if(random<25.0)
					return(djOK);
				else
					return(djKO);
				break;
		}
	}
	return(fdbk);
}


//-----------------------------------------------------------------------------
GProfDoc::~GProfDoc(void)
{
}
