/*

	GALILEI Research Project

	GSatSimDoc.cpp

	Calc the similarity between Documents using the ideal groupment - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
#include <tests/ggroupevaluate.h>
#include <tests/ggroupevaluatesubprof.h>
#include <tests/ggroupsevaluate.h>
#include <sessions/gsession.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofilecalc.h>
#include <tests/gstatsimdoc.h>
#include <docs/gdoc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GStatSimDoc
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GStatSimDoc::GStatSimDoc(GSession* ses)
	: Session(ses),Global(true)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDoc::Run(void)
{
	// temp Intra,extra similarity
	double MinIntra;
	double MaxExtra;

	//Initialization
	MeanIntraM=0.0;
	MeanExtraM=0.0;
	Overlap=0.0;

	int ComptIntra=0;
	int ComptExtra=0;
	int ComptOverlap=0;

	//The container of documents.
	RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDoc = Session->GetIdealDocs();

	// Load the ideal document container.

	for(GroupsDoc->Start();!GroupsDoc->End();GroupsDoc->Next())
	{
		GGroupEvaluateCursor Cur2=(*GroupsDoc)()->GetGroupEvaluateCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			// For each group... Initialization.
			MinIntra=1.0;
			MaxExtra=0.0;

			GGroupEvaluate* Group=Cur2();
			// A vector used to memorise the id of the documents
			int* vector;
			vector=new int[Group->NbPtr()];
			unsigned int i=0;
			for(Group->Start();!Group->End();Group->Next())
			{
				vector[i]=Group->Current();
				i++;
			}
			// For all the document in this group
			for(i=0;i<Group->NbPtr();i++)
			{
				for(unsigned int j=0;j<Group->NbPtr();j++)
				{
					double temp;
					if(Global) temp=Group->GlobalSimilarity(vector[i],vector[j]);
					else temp=Group->Similarity(vector[i],vector[j]);
 					if((i!=j)&&(temp>-10)&&(temp<10))
					{
						MeanIntraM+=temp;
						ComptIntra++;
						if((temp<MinIntra)) MinIntra=temp;
					}
				}
			}

			// Calc the similarity for all the document which are not in the current group.
			GGroupEvaluateCursor Cur=(*GroupsDoc)()->GetGroupEvaluateCursor();
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GGroupEvaluate* GroupExtra=Cur();
				int* vectorextra;
				vectorextra=new int[GroupExtra->NbPtr()];
				int ii=0;
				for(GroupExtra->Start();!GroupExtra->End();GroupExtra->Next())
				{
					vectorextra[ii]=GroupExtra->Current();
					ii++;
				}
				if(GroupExtra->GetId()!=Group->GetId())
				{
					for(i=0;i<Group->NbPtr();i++)
					{
						for(unsigned int j=0;j<GroupExtra->NbPtr();j++)
						{
							double temp;
							if(Global) temp=Group->GlobalSimilarity(vectorextra[j],vector[i]);
							else temp=Group->Similarity(vectorextra[j],vector[i]);
							if ((temp<0.98)&&(temp>-0.98))	
							{
								MeanExtraM+=temp;
								ComptExtra++;
							}
							if((temp>MaxExtra)&&(temp<0.98))
							{
								MaxExtra=temp;
							}
						}
					}
				}
			}
			ComptOverlap++;
			if(MaxExtra>MinIntra) Overlap+=(MaxExtra-MinIntra);
		}
	}
	MeanIntraM/=ComptIntra;
	MeanExtraM/=ComptExtra;
	if(ComptOverlap>0)
		Overlap/=ComptOverlap;
	Rie=(MeanIntraM-MeanExtraM)/MeanIntraM;
}


//-----------------------------------------------------------------------------
char* GALILEI::GStatSimDoc::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c",c);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDoc::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%c",&c);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimDoc::~GStatSimDoc(void)
{
}
