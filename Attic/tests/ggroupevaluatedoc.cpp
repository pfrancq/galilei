/*

	GALILEI Research Project

	GGroupEvaluatedoc.cpp

	Representation of groups of dociles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <tests/ggroupevaluatedoc.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupEvaluatedoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluateDoc::GGroupEvaluateDoc(unsigned int id,GLang* lang,RContainer<GDoc,unsigned int,false,true>* doc,GSession* session,RString  name)
	:GGroupEvaluate(lang),Lang(lang),Id(id),Doc(doc),Ses(session)
{
Name=name;
}


//-----------------------------------------------------------------------------	
void GALILEI::GGroupEvaluateDoc::Start(void)
{
	Doc->Start();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupEvaluateDoc::Next(void)
{
	Doc->Next();
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupEvaluateDoc::End(void)
{
	return(Doc->End());
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateDoc::NbPtr(void)
{
	return(Doc->NbPtr);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateDoc::Current(void)
{
	return((*Doc)()->GetId());
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateDoc::Similarity(unsigned int id1,unsigned int id2)
{
	GDoc * s = Doc->GetPtr(id2);
	return(s->Similarity(Ses->GetDoc(id1)));
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateDoc::GlobalSimilarity(unsigned int id1,unsigned int id2)
{
	GDoc * s = Doc->GetPtr(id2);
	return(s->GlobalSimilarity(Ses->GetDoc(id1)));
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupEvaluateDoc::IsIn(unsigned int i)
{
	if(Doc->GetPtr(i)) return (true);
	else return (false);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateDoc::RelevantSubDoc(void)
{
	GDoc* finalsub;
	GDoc* sub1;
	double refsum=0.0;
	for(Doc->Start(); !Doc->End(); Doc->Next())
	{
		sub1 = (*Doc)();
		double sum=GroupSumSimilarity(sub1->GetId());
		if (sum>=refsum)
		{
			finalsub=sub1;
			refsum=sum;
		}
	}
	return(finalsub->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluateDoc::NbProfDoc(void)
{
	return(Doc->NbPtr);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateDoc::SumSimilarity(unsigned int id1)
{

	GDocCursor SubCursor=Ses->GetDocsCursor();
	double sum=0.0;
	GDoc* sub1;

	for (SubCursor.Start();!SubCursor.End();SubCursor.Next())
	{
		sub1=SubCursor();
		if(sub1->GetN()!=0)
		{
			double temp=Ses->GetDoc(id1)->Similarity(sub1);
			if((temp<2)&&(temp>-2))	sum=sum+temp;
		}
	}
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateDoc::GroupSumSimilarity(unsigned int subdoc)
{
	double sum=0.0;
	GDoc * doc=Ses->GetDoc(subdoc);
	GDoc** sub1;
	unsigned i;
	for(sub1=Doc->Tab,i=Doc->NbPtr;--i;sub1++)
	{
		double temp=doc->Similarity((*sub1));
		if((sum<=2)&&(sum>=-2))	sum=sum+temp;
	}
	return(sum);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluateDoc::Compare(const GGroupEvaluateDoc&)
{
	return(-1);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluateDoc::Compare(const GGroupEvaluateDoc*)
{
	return(-1);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluateDoc::~GGroupEvaluateDoc(void)
{
}
