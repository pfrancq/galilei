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
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <groups/gchromoir.h>
#include <groups/ggroup.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewchromos.h"
#include "qsessionprogress.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Chromos(40,20), IdealGroups(2,1)
{
	// Load Ideal Groups;
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

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
	ConstructChromosomes();
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomes(void)
{
	GChromoIR** c;
	unsigned int i;
	QListViewItem* g;
	char tmp[20];
	GInstIR* inst;

	// Loal the chromosomes from the db
	//LoadChromosomes();

	// Display the chromosomes
	for(i=inst->PopSize+1,c=inst->Chromosomes;--i;c++)
	{
		sprintf(tmp,"%u",(*c)->Id);
		g=new QListViewItem(General,tmp);

		(*c)->CompareIdeal(Doc->GetSession(),&IdealGroups);
		sprintf(tmp,"%f",(*c)->GetPrecision());
		g->setText(2,tmp);
		sprintf(tmp,"%f",(*c)->GetRecall());
		g->setText(3,tmp);
		sprintf(tmp,"%f",(*c)->GetGlobal());
		g->setText(4,tmp);

		(*c)->EvaluateAvgSim();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(5,tmp);

		(*c)->EvaluateSumRel();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(6,tmp);

		(*c)->EvaluateAvgMinMax();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(7,tmp);

		(*c)->EvaluateMinMinMax();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(8,tmp);

		(*c)->EvaluateMinRel();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(9,tmp);

		(*c)->EvaluateAvgVarMinRel();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(10,tmp);

		(*c)->EvaluateAvgVarMinRel();
		sprintf(tmp,"%f",(*c)->GetSimCriterion());
		g->setText(11,tmp);

		g->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/locale.png"));

	}
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
