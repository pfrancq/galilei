/*

	GALILEI Research Project

	GGroupEvaluateSubprof.cpp

	Representation of groups of subprofiles and groups of documents.

	(C) 2002 by Julien Lamoral

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
#include <tests/ggroupevaluatesubprof.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupEvaluateSubprof
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluateSubprof::GGroupEvaluateSubprof(unsigned int id,GLang* lang,RContainer<GSubProfile,unsigned int,false,true>* sub,GSession* session)
	: GGroupEvaluate(lang),Lang(lang),SubProfile(sub),Ses(session),Id(id)
{
}


//-----------------------------------------------------------------------------	
void GALILEI::GGroupEvaluateSubprof::Start(void)
{
	SubProfile->Start();
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupEvaluateSubprof::Next(void)
{
	SubProfile->Next();
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupEvaluateSubprof::End(void)
{
	return(SubProfile->End());
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateSubprof::NbPtr(void)
{
	return(SubProfile->NbPtr);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateSubprof::Current(void)
{
	return((*SubProfile)()->GetId());
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateSubprof::Similarity(unsigned int id1,unsigned int id2)
{
	GSubProfile* s=SubProfile->GetPtr(id2);
	return(s->Similarity(Ses->GetSubProfile(id1)));
}


//-----------------------------------------------------------------------------
bool GALILEI::GGroupEvaluateSubprof::IsIn(unsigned int i)
{
	if(SubProfile->GetPtr(i)) return (true);
	else return (false);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupEvaluateSubprof::RelevantSubDoc(void)
{
	GSubProfile* finalsub;
	GSubProfile* sub1;
	double refsum=0.0;
	for(SubProfile->Start(); !SubProfile->End(); SubProfile->Next())
	{
		sub1 = (*SubProfile)();
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
int GALILEI::GGroupEvaluateSubprof::NbProfDoc(void)
{
	return(SubProfile->NbPtr);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateSubprof::SumSimilarity(unsigned int id1)
{

	GSubProfileCursor SubCursor=Ses->GetSubProfilesCursor(Lang);
	double sum=0.0;
	GSubProfile* sub1;

	for (SubCursor.Start();!SubCursor.End();SubCursor.Next())
	{
		sub1=SubCursor();
		if(sub1->GetNbJudgedDocs()!=0)
		{
			double temp=Ses->GetSubProfile(id1)->Similarity(sub1);
			cout<<id1<<" "<<sub1->GetId()<<" "<<temp<<endl;
			sum=sum+temp;
		}
	}
	return(sum);
}


//-----------------------------------------------------------------------------
double GALILEI::GGroupEvaluateSubprof::GroupSumSimilarity(unsigned int subdoc)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for(sub1=SubProfile->Tab,i=SubProfile->NbPtr;--i;sub1++)
		sum=sum+Ses->GetSubProfile(subdoc)->Similarity((*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluateSubprof::Compare(const GGroupEvaluateSubprof& )
{
	return(-1);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupEvaluateSubprof::Compare(const GGroupEvaluateSubprof* )
{
	return(-1);
}


//-----------------------------------------------------------------------------
GALILEI::GGroupEvaluateSubprof::~GGroupEvaluateSubprof(void)
{
}
