/*

	GALILEI Research Project

	KViewChromos.cpp

	Window to manipulate a set of chromosomes - Implementation.

	(C) 2002 by Pascal Francq

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <groups/gchromoir.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessim.h>
#include <groups/ggroup.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// includes files for KDE
#include <kapplication.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewchromos.h"
#include "qsessionprogress.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class MyListViewItem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyListViewItem : public QListViewItem
{
public:
	MyListViewItem(QListView *parent,QString label1) : QListViewItem(parent,label1) {}
	virtual int compare(QListViewItem *i,int col,bool) const
	{
		double diff=atof(text(col).latin1())-atof(i->text(col).latin1());
		if(!diff) return(0);
		if(diff>0)
		    return(-1);
		return(1);
	}
};



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,const char* l,bool global,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), IdealGroups(2,1), Lang(Doc->GetSession()->GetLang(l)),
	  Global(global)
{
	// Construct chromosomes
	General = new QListView(this);
	General->addColumn("Id");
	General->addColumn("Precision");
	General->addColumn("Recall");
	General->addColumn("Global");
	General->addColumn("Avg Similarity");
	General->addColumn("Proto: Sum(intra)/Min(inter)");
	General->addColumn("Avg min(intra)/max(inter)");
	General->addColumn("Min min(intra)/max(inter)");
	General->addColumn("Proto: Min Max(intra)/Min(inter)");
	General->addColumn("Proto: Avg Max(intra)/Min(inter)");
	General->addColumn("Avg Var(intra)/Avg Var(inter)");
	for(int i=0;i<11;i++)
	{
		General->setColumnWidthMode(i,QListView::Maximum);
		General->setColumnAlignment(i,Qt::AlignHCenter);
	}
	ConstructChromosomes();
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomes(void)
{
	GChromoIR** c;
	unsigned int i;
	QListViewItem* g;
	char tmp[20];
	GInstIR* Instance;
	GSubProfileCursor SubProfiles;
	RGA::RObjs<GObjIR> Objs(SubProfiles.GetNb());

	// Initialise the dialog box
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Stored Chromosomes");
	d->show();
	KApplication::kApplication()->processEvents();

	// Load Ideal Groups;
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

	// Construct the GA Objects
	SubProfiles=Doc->GetSession()->GetSubProfilesCursor(Lang);
	for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
	{
		Objs.InsertPtr(new GObjIR(i,SubProfiles()));
	}
	GProfilesSim Sims(SubProfiles,Global);

	// Loal the chromosomes from the db
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,&Sims,Global);
	if(!Instance) return;
	Instance->SetIdealGroups(&IdealGroups);

	// Display the chromosomes
	for(i=Instance->PopSize+1,c=Instance->Chromosomes;--i;c++)
	{
		sprintf(tmp,"%u",(*c)->Id);
		d->receiveNextChromosome((*c)->Id);
		g=new MyListViewItem(General,tmp);

		(*c)->CompareIdeal(Doc->GetSession(),&IdealGroups);
		sprintf(tmp,"%lf",(*c)->GetPrecision());
		g->setText(1,tmp);
		sprintf(tmp,"%lf",(*c)->GetRecall());
		g->setText(2,tmp);
		sprintf(tmp,"%lf",(*c)->GetGlobal());
		g->setText(3,tmp);

		(*c)->EvaluateAvgSim();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(4,tmp);

		(*c)->EvaluateSumRel();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(5,tmp);

		(*c)->EvaluateAvgMinMax();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(6,tmp);

		(*c)->EvaluateMinMinMax();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(7,tmp);

		(*c)->EvaluateMinRel();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(8,tmp);

		(*c)->EvaluateAvgVarMinRel();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(9,tmp);

		(*c)->EvaluateAvgVarMinRel();
		sprintf(tmp,"%lf",(*c)->GetSimCriterion());
		g->setText(10,tmp);
	}

	// Finish.
	delete Instance;
	d->Finish();
}


//-----------------------------------------------------------------------------
void KViewChromos::update(unsigned int)
{
}


//-----------------------------------------------------------------------------
void KViewChromos::resizeEvent(QResizeEvent *)
{
	General->resize(size());
}


//-----------------------------------------------------------------------------
KViewChromos::~KViewChromos(void)
{
}
