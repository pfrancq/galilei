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
	General->addColumn("Calinsky");
	General->addColumn("IntraMinInter");
	General->addColumn("Variance");
	ConstructGeneral();
	
	// Initialisation of the User Widget
	Variance = new QListView(Infos);
	Infos->insertTab(Variance,"Variance");
	Variance->addColumn("Group name");
	Variance->addColumn("Variance Docs ");
	Variance->addColumn("Variance Subprofile");
	ConstructVariance();
	
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
	ConstructSimilarityDoc();
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructGeneral(void)
{
	RContainer<GGroupsEvaluate,unsigned int,true,true>* groups = new RContainer<GGroupsEvaluate,unsigned int,true,true> (2,2);
	GSession* session=Doc->GetSession();
	session->LoadIdealDocument(groups);
	
	GEvaluateGroupingCalinski Cal (session, groups);
	GEvaluateGroupingVariance Var (session, groups);
	GEvaluateGroupingIntraMinInter Minintra (session, groups);
	General->clear();
	new QListViewItem(General,"Document",QString(dtoa(Cal.Run())),QString(dtoa(Minintra.Run())),QString(dtoa(Var.Run())));
	
	RContainer<GGroupsEvaluate,unsigned int,true,true>* Groups=new RContainer<GGroupsEvaluate,unsigned int,true,true> (2,2);
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
	new QListViewItem(General,"SubProfiles",QString(dtoa(CalG.Run())),QString(dtoa(MinintraG.Run())),QString(dtoa(VarG.Run())));
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructVariance(void)
{
	Variance->clear();
	new QListViewItem(Variance,"jazz","1","1");
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructSimilaritySubProf()
{
}


//-----------------------------------------------------------------------------
void KViewEvaluateGroup::ConstructSimilarityDoc()
{
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
