/*

	GALILEI Research Project

	GSatSimDocProf.cpp

	Calc the similarity between subprofiles and documents using the ideal groupment - Implementation.

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
#include <tests/ggroupevaluatesubprof.h>
#include <tests/ggroupsevaluate.h>
#include <tests/gstatsimdocprof.h>
#include <sessions/gsession.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <docs/gdoc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// class SimGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GStatSimDocProf::SimGroup
{
public:

	bool Goodgrp;
	double Sim;

	SimGroup(bool gdgrp,double sim);


	int Compare(const SimGroup& sim) const
		{return(-1);}
	int Compare(const SimGroup* sim) const
		{return(-1);}

	~SimGroup(void);
};


//-----------------------------------------------------------------------------
GALILEI::GStatSimDocProf::SimGroup::SimGroup(bool gdgrp,double sim)
{
	Goodgrp=gdgrp;
	Sim=sim;
}



//-----------------------------------------------------------------------------
GALILEI::GStatSimDocProf::SimGroup::~SimGroup(void)
{
}





//-----------------------------------------------------------------------------
//
// class GStatSimDocProf
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GStatSimDocProf::GStatSimDocProf(GSession* ses,RContainer<GGroups,unsigned int,true,true>* ideal)
	: Session(ses), IdealGroups(ideal),Global(true)
{
}

//--------------------------------------------------------------------------
int GALILEI::GStatSimDocProf::sortOrder(const void *a,const void *b)
{
	double af=(*((SimGroup**)(a)))->Sim;
	double bf=(*((SimGroup**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDocProf::Run(void)
{
	//temp variable 
	const int nbrecall=10;
	
	double MeanIntraAll;
	double MeanExtraAll;
	double MinIntra;
	double MaxExtra;
	double MeanIntraMAll;
	double MeanExtraMAll;
	double MeanIntraJug;
	double MeanIntraMJug;
	double MeanIntraNotJug;
	double MeanIntraMNotJug;
	double overlap;
	double nbprofiles;
	double Precision[nbrecall];
	double Thresold[nbrecall];

	//Initialization.
	MeanIntraMAll=0.0;
	MeanExtraMAll=0.0;
	MeanIntraMJug=0.0;
	MeanIntraMNotJug=0.0;
	for(int i=0;i<nbrecall;i++)
	{
		Precision[i]=0.0;
		Thresold[i]=(nbrecall*(i+1));
	}
	
	overlap=0.0;
	nbprofiles=0.0;
	int nbtot=0;

    // the container of Documents
	RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDoc=Session->GetIdealDocs();

	// For each language
	for(IdealGroups->Start(),GroupsDoc->Start();!IdealGroups->End();IdealGroups->Next(),GroupsDoc->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		GGroupEvaluateCursor Cur=(*GroupsDoc)()->GetGroupEvaluateCursor();
		GGroupEvaluateCursor CurDocTemp=(*GroupsDoc)()->GetGroupEvaluateCursor();
		// For each group of the current languages.
		for(Cur2.Start(),Cur.Start();!Cur2.End();Cur2.Next(),Cur.Next())
		{
			GGroup* Group=Cur2();
			GSubProfileCursor subcur= Group->GetSubProfileCursor();
			// For each profiles in the current group.
		    for(subcur.Start();!subcur.End();subcur.Next())
			{
				RContainer<SimGroup,unsigned int,true,false>* sgrp=new RContainer<SimGroup,unsigned int,true,false> (100,100);  
				// Initialization.
				GSubProfile* SubProf=subcur();
				nbprofiles++;
				MinIntra=10;
				MaxExtra=0;
				int comptiall=0;
				int compteall=0;
				int comptijug=0;
				int comptinotjug=0;
				double N=0;
				MeanIntraAll=0.0;
				MeanExtraAll=0.0;
				MeanIntraJug=0.0;
				MeanIntraNotJug=0.0;

				GGroupEvaluate* Grp=Cur();
				GDocCursor docur= Session->GetDocsCursor();

				// Find Min Sim intra  And Max sim inter...
				N=Grp->NbPtr();

				//Calcultation of the group's Recalls,precision Rie,...
				for(Grp->Start();!Grp->End();Grp->Next())
				{
					bool isjudged=false;
					int idd=Grp->Current();
					GDoc* document=Session->GetDoc(idd);
					if(SubProf->GetProfile()->GetFeedback(document)!=0)
					{
						isjudged=true;
					}
					double similarity;
					if(Global)	similarity=SubProf->GlobalSimilarity(document);
					else similarity=SubProf->Similarity(document);
					if((similarity<10)&&(similarity>-10))
					{
	  					sgrp->InsertPtr(new SimGroup(true,similarity));
						if(similarity<MinIntra) MinIntra=similarity;
						if(isjudged)
						{
							//The doc is judged by at least one profiles
							comptiall++;
							comptijug++;
							MeanIntraAll+=similarity;
							MeanIntraJug+=similarity;
						}
						else
						{
							//The doc is not judged by at least one profiles
							comptinotjug++;
							MeanIntraNotJug+=similarity;
							comptiall++;
							MeanIntraAll+=similarity;
						}
					}
				}
				// For All the document that are not in the current group.
				for(docur.Start();!docur.End();docur.Next())
				{
					unsigned int grpid;
					bool samegroup=false;
					GDoc* Document=docur();

					//verify if the doc is in the current group.
					bool find=true;

					for(CurDocTemp.Start();((!CurDocTemp.End())|(find));CurDocTemp.Next())
					{
						GGroupEvaluate* GrpTemp=CurDocTemp();
						if(GrpTemp->IsIn(Document->GetId()))
						{
						 	grpid=GrpTemp->GetId();
						  	find=false; 
						}
					}
					if(grpid==Group->GetId()) samegroup=true;
					if(!samegroup)
					{
						double similarity;
						if(Global)	similarity=SubProf->GlobalSimilarity(Document);
						else similarity=SubProf->Similarity(Document);
						if((similarity<10)&&(similarity>-10))
						{
							sgrp->InsertPtr(new SimGroup(false,similarity));
							if(similarity>MaxExtra) MaxExtra=similarity;
							compteall++;
							MeanExtraAll+=similarity;
						}
					}
				}
				qsort(static_cast<void*>(sgrp->Tab),sgrp->NbPtr,sizeof(SimGroup*),sortOrder);

				//Calculation of recal,Precision
				for(int i=0;i<nbrecall;i++)
				{
					bool end=false;
					double compt=0;
					double nbgood=0;
					double nbdoc=Thresold[i]*N/100;
					for(sgrp->Start();(!end)&&(!sgrp->End());sgrp->Next())
					{
						if((*sgrp)()->Goodgrp) nbgood++;
						compt++;
						if(nbgood>=(nbdoc-1)) end=true;
					}
					if(compt!=0) Precision[i]+=(nbgood/compt);
					else Precision[i]+=1;
				}
				
				//Transformation of results and add to all groups Mean's.
				// if there are documents in the current group.
				if(comptiall>0)
				{
					MeanIntraAll/=comptiall;
					MeanIntraJug/=comptijug;
					MeanIntraNotJug/=comptinotjug;
					MeanExtraAll/=compteall;
					if(MaxExtra>=MinIntra) overlap++;
					MeanIntraMAll+=MeanIntraAll;
					MeanExtraMAll+=MeanExtraAll;
					MeanIntraMJug+=MeanIntraJug;
					MeanIntraMNotJug+=MeanIntraNotJug;
					nbtot++;
				}
			}
		}
	}
	overlap/=nbprofiles;
	MeanIntraMAll/=nbtot;
	MeanExtraMAll/=nbtot;
	MeanIntraMJug/=nbtot;
	MeanIntraMNotJug/=nbtot;

	cout<<"MeanIntraMAll"<<" "<<"MeanExtraMAll"<<" "<<"MeanIntraMJug"<<" "<<"MeanIntraMNotJug"<<" "<<"RIE"<<"overlapp "<<endl;
	cout<<MeanIntraMAll<<" "<<MeanExtraMAll<<" "<<MeanIntraMJug<<" "<<MeanIntraMNotJug<<" "<<((MeanIntraMAll)-(MeanExtraMAll))/(MeanIntraMAll)<<" "<<overlap<<" "<<endl;
	for(int i=0;i<nbrecall;i++)
	{
		Precision[i]/=nbtot;
		cout<<"Recall 0."<<i<<" Precision "<<Precision[i]<<endl;
	}	
}

//-----------------------------------------------------------------------------
char* GALILEI::GStatSimDocProf::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c",c);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDocProf::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%c",&c);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimDocProf::~GStatSimDocProf(void)
{
}
