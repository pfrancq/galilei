/*

	GALILEI Research Project

	KViewEvaluateGroup.cpp

	Window to see differents statistique for the ideal group of profile and document.

	(C) 2002 by Julien lamoral

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <tests/ggroupevaluate.h>
#include <tests/ggroupevaluatesubprof.h>
#include <tests/ggroupevaluatedoc.h>
#include <tests/ggroupsevaluate.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/gevaluategroupingcalinski.h>
#include <groups/gevaluategroupingvariance.h>
#include <groups/gevaluategroupingintramininter.h>
#include <sessions/gsession.h>
#include <profiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qtabwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewevaluategroup.h"
#include "qsessionprogress.h"
#include "kdoc.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class KViewEvaluateGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewEvaluateGroup::KViewEvaluateGroup(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	// Window initialisation
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/actions/find.png"));
	setCaption("Similarity, GlobalSimilarity and intra/inter distances for document and profiles");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Group Type");
	General->addColumn("Between/Within");
	General->addColumn("IntraMinInter");
	General->addColumn("Variance");
	ConstructGeneral();
	
	// Initialisation of the Variance Doc Widget
	VarianceDoc = new QListView(Infos);
	Infos->insertTab(VarianceDoc,"Variance for document");
	VarianceDoc->addColumn("Group name");
	VarianceDoc->addColumn("Variance Docs ");
//	ConstructVarianceDoc();

	// Initialisation of the Variance Subprof Widget
	VarianceSubProf = new QListView(Infos);
	Infos->insertTab(VarianceSubProf,"Variance for SubProfiles");
	VarianceSubProf->addColumn("Group name");
	VarianceSubProf->addColumn("Variance Subprofile");
//	ConstructVarianceSubProf();

	SimilaritySubProf= new QListView(Infos);
	Infos->insertTab(SimilaritySubProf,"Similarity SubProfile");
	SimilaritySubProf->addColumn("Group name");
	SimilaritySubProf->addColumn("intra min");
	SimilaritySubProf->addColumn("intra mean");
	SimilaritySubProf->addColumn("extra max");
	SimilaritySubProf->addColumn("extra mean");
	SimilaritySubProf->addColumn("similar min");
	SimilaritySubProf->addColumn("similar max");
	SimilaritySubProf->addColumn("similar mean");
	ConstructSimilaritySubProf();
	
	SimilarityDoc= new QListView(Infos);
	Infos->insertTab(SimilarityDoc,"Similarity Doc");
	SimilarityDoc->addColumn("Group name");
	SimilarityDoc->addColumn("intra min");
	SimilarityDoc->addColumn("intra mean");
	SimilarityDoc->addColumn("extra max");
	SimilarityDoc->addColumn("extra mean");
	SimilarityDoc->addColumn("similar min");
	SimilarityDoc->addColumn("similar max");
	SimilarityDoc->addColumn("similar mean");
//	ConstructSimilarityDoc();
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructGeneral(void)
{
	RContainer<GGroupsEvaluate,unsigned int,false,false>* groups = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
	GSession* session=Doc->GetSession();
//	session->LoadIdealDocument(groups);
	
//	GEvaluateGroupingCalinski Cal (session, groups);
//	GEvaluateGroupingVariance Var (session, groups);
//	GEvaluateGroupingIntraMinInter Minintra (session, groups);
//	General->clear();
//	new QListViewItem(General,"Document",QString(dtoa(Cal.Run())),QString(dtoa(Minintra.Run())),QString(dtoa(Var.Run())));

	RContainer<GGroupsEvaluate,unsigned int,false,false>* Groups=new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
	RContainer<GGroups,unsigned int,true,true>* IdealGroups=new RContainer<GGroups,unsigned int,true,true> (2,2);

	session->LoadIdealGroupment(IdealGroups);

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupsEvaluate* GS = new GGroupsEvaluate((*IdealGroups)()->GetLang());
		GGroupCursor& Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			GGroupEvaluateSubprof* G = new GGroupEvaluateSubprof(Cur2()->GetId(),(*IdealGroups)()->GetLang(),Cur2(),session);
			GS->InsertPtr(G);
		}
		Groups->InsertPtr(GS);
	}

	GEvaluateGroupingCalinski CalG (session, Groups);
	GEvaluateGroupingVariance VarG (session, Groups);
	GEvaluateGroupingIntraMinInter MinintraG (session,Groups);
	//new QListViewItem(General,"SubProfiles",QString(dtoa(CalG.Run())),QString(dtoa(MinintraG.Run())),QString(dtoa(VarG.Run())));
	cout<<" "<<CalG.Run()<<" "<<MinintraG.Run()<<" "<<VarG.Run();
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructVarianceSubProf(void)
{
	VarianceSubProf->clear();

	GSession* session=Doc->GetSession();

	RContainer<GGroups,unsigned int,true,true>* IdealGroups=new RContainer<GGroups,unsigned int,true,true> (2,2);
	session->LoadIdealGroupment(IdealGroups);

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor& Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsSubProf =new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
			GGroupsEvaluate* GS = new GGroupsEvaluate((*IdealGroups)()->GetLang());
			GGroupEvaluateSubprof* G = new GGroupEvaluateSubprof(Cur2()->GetId(),(*IdealGroups)()->GetLang(),Cur2(),session);
			GS->InsertPtr(G);
			GroupsSubProf->InsertPtr(GS);
			GEvaluateGroupingVariance Var (session, GroupsSubProf);
			bool end=true;
			for(Cur2()->Start();end;Cur2()->Next())
			{
				new QListViewItem(VarianceSubProf,QString((*Cur2())()->GetProfile()->GetName()), QString(dtoa(Var.Run())));
				end=false;
			}
			delete (GS);
			delete (G);
			delete (GroupsSubProf);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructVarianceDoc(void)
{
	VarianceDoc->clear();
	
	GSession* session=Doc->GetSession();

	RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDoc = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);

	session->LoadIdealDocument(GroupsDoc);
	
	for(GroupsDoc->Start();!GroupsDoc->End();GroupsDoc->Next())
	{
		for((*GroupsDoc)()->Start();!(*GroupsDoc)()->End();(*GroupsDoc)()->Next())
		{
			RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDocTemp = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
			GGroupsEvaluate* GS = new GGroupsEvaluate((*GroupsDoc)()->GetLang());
			GS->InsertPtr((*(*GroupsDoc)())());
			GroupsDocTemp->InsertPtr(GS);
			GEvaluateGroupingVariance Var (session, GroupsDocTemp);
			new QListViewItem(VarianceDoc, QString((*(*GroupsDoc)())()->GetName()), QString(dtoa(Var.Run())));
			delete (GS);
			delete (GroupsDocTemp);
		}
	}            	
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructSimilaritySubProf()
{
	RString name;
	double MinIntra,MeanIntra;
	double MaxExtra,MeanExtra;

	int nbtot=0;
	double MinIntraM,MeanIntraM;
	double MaxExtraM,MeanExtraM;
	MinIntraM=0.0;
	MeanIntraM=0.0;
	MaxExtraM=0.0;
	MeanExtraM=0.0;

	SimilaritySubProf->clear();

	GSession* session=Doc->GetSession();

	RContainer<GGroups,unsigned int,true,true>* IdealGroups=new RContainer<GGroups,unsigned int,true,true> (2,2);
	session->LoadIdealGroupment(IdealGroups);

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			MinIntra=1.0;
			MeanIntra=0.0;
			MaxExtra=0.0;
			MeanExtra=0.0;
			int compt=0;
			GGroup * Group=Cur2();
			GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
			GSubProfileCursor SubCur2= Group->GetSubProfileCursor();
			for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
			{
				name=SubCur1()->GetProfile()->GetName();
				for(SubCur2.Start();!SubCur2.End();SubCur2.Next())
				{
					double temp=SubCur1()->Similarity(SubCur2());
					if(SubCur1()->GetId()!=SubCur2()->GetId())
					{
						MeanIntra+=temp;
						compt++;
						if(temp<MinIntra) MinIntra=temp;
					}
				}
			}
			if(compt!=0) MeanIntra/=compt;
			else MeanIntra=0;
			GGroupCursor Cur=(*IdealGroups)()->GetGroupCursor();
			compt=0;
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GGroup* GroupExtra=Cur();
				if(GroupExtra->GetId()!=Group->GetId())
				{
					GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
					GSubProfileCursor SubCur2= GroupExtra->GetSubProfileCursor();
					for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
					{
						for(SubCur2.Start();!SubCur2.End();SubCur2.Next())
						{
							double temp=SubCur1()->Similarity(SubCur2());
							MeanExtra+=temp;
							compt++;
							if((temp>MaxExtra)&&(temp<0.99)) MaxExtra=temp;
						}
					}
				}
			}
			MeanExtra/=compt;

			nbtot++;
			MinIntraM+=MinIntra;
			MeanIntraM+=MeanIntra;
			MaxExtraM+=MaxExtra;
			MeanExtraM+=MeanExtra;
//			new QListViewItem(SimilaritySubProf,QString(name),QString(dtoa(MinIntra)),QString(dtoa(MeanIntra)),QString(dtoa(MaxExtra)),QString(dtoa(MeanExtra)),"similarmin","similarmax","similarmean");
		}
	}
//	new QListViewItem(SimilaritySubProf,QString("Mean"),QString(dtoa(MinIntraM/nbtot)),QString(dtoa(MeanIntraM/nbtot)),QString(dtoa(MaxExtraM/nbtot)),QString(dtoa(MeanExtraM/nbtot)),"similarmin","similarmax","similarmean");
	cout<<" "<<MinIntraM/nbtot<<" "<<MeanIntraM/nbtot<<" "<<MaxExtraM/nbtot<<" "<<MeanExtraM/nbtot<<endl;
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructSimilarityDoc()
{
	RString name;
	double MinIntra,MeanIntra;
	double MaxExtra,MeanExtra;
	GSession* session=Doc->GetSession();
	SimilarityDoc->clear();

	RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDoc = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);

	session->LoadIdealDocument(GroupsDoc);
	
	for(GroupsDoc->Start();!GroupsDoc->End();GroupsDoc->Next())
	{
		GGroupEvaluateCursor& Cur2=(*GroupsDoc)()->GetGroupEvaluateCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			MinIntra=1.0;
			MeanIntra=0.0;
			MaxExtra=0.0;
			MeanExtra=0.0;
			int compt=0;
			GGroupEvaluate* Group=Cur2();
			name=Group->GetName();
			int vector[Group->NbPtr()];
			int i=0;
			for(Group->Start();!Group->End();Group->Next())
			{
				vector[i]=Group->Current();
				i++;
			}
			for(i=0;i<Group->NbPtr();i++)
			{
				for(int j=0;j<Group->NbPtr();j++)
				{
					double temp=Group->Similarity(vector[i],vector[j]);
 					if((i!=j)&&(temp>0))
					{
				
						MeanIntra+=temp;
						compt++;
						if((temp<MinIntra)) MinIntra=temp;
					}
				}
			}
			MeanIntra/=compt;
			GGroupEvaluateCursor& Cur=(*GroupsDoc)()->GetGroupEvaluateCursor();
			compt=0;
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GGroupEvaluate* GroupExtra=Cur();
				int vectorextra[GroupExtra->NbPtr()];
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
						for(int j=0;j<GroupExtra->NbPtr();j++)
						{
							double temp=Group->Similarity(vectorextra[j],vector[i]);
							MeanExtra+=temp;
							compt++;
							if((temp>MaxExtra)&&(temp<0.99))
							{
								MaxExtra=temp;
							}
						}
					}
				}
			}
			MeanExtra/=compt;
			new QListViewItem(SimilarityDoc,QString(name),QString(dtoa(MinIntra)),QString(dtoa(MeanIntra)),QString(dtoa(MaxExtra)),QString(dtoa(MeanExtra)),"similarmin","similarmax","similarmean");
		}
	}
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::update(unsigned int )
{
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
KViewEvaluateGroup::~KViewEvaluateGroup(void)
{
}
