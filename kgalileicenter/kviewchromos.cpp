/*

	GALILEI Research Project

	KViewChromos.cpp

	Window to manipulate a set of chromosomes - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <groups/gchromoir.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessim.h>
#include <groups/ggroup.h>
using namespace GALILEI;
using namespace RGA;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qpopupmenu.h>


//-----------------------------------------------------------------------------
// includes files for KDE
#include <kapplication.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kmessagebox.h>


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
// class QListViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QListViewChromos : public QListView
{
public:
	QListViewChromos(QWidget* parent=0,const char* name=0,WFlags f=0);

protected:
	virtual void mousePressEvent(QMouseEvent* e);
};


//-----------------------------------------------------------------------------
QListViewChromos::QListViewChromos(QWidget* parent,const char* name,WFlags f)
	: QListView(parent,name,f)
{
	addColumn("Id");
	addColumn("Precision");
	addColumn("Recall");
	addColumn("Global");
	addColumn("AvgSim");
	addColumn("J");
	addColumn("AvgRatio");
	addColumn("MinRatio");
	addColumn("Ratio");
	addColumn("W Over B");
	addColumn("Sim WB");
	for(int i=0;i<11;i++)
	{
		setColumnWidthMode(i,QListView::Maximum);
		setColumnAlignment(i,Qt::AlignHCenter);
	}
}


//-----------------------------------------------------------------------------
void QListViewChromos::mousePressEvent(QMouseEvent* e)
{
	if(e->button()==RightButton)
	{
		QPopupMenu* InfoBox=new QPopupMenu(parentWidget());
		InfoBox->insertItem("Save Statistics",parentWidget(),SLOT(slotMenu(int)));
		InfoBox->popup(e->globalPos());
	}
	else
		QWidget::mousePressEvent(e);
}



//-----------------------------------------------------------------------------
//
// class KViewChromos::Stats
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KViewChromos::Stat
{
public:
	tId Id;
	double Precision;
	double Recall;
	double Global;
	double AvgSim;
	double J;
	double AvgRatio;
	double MinRatio;
	double Ratio;
	double WOverB;
	double SimWB;

	Stat(void) : Id(NullId), Precision(0.0), Recall(0.0), Global(0.0), AvgSim(0.0), J(0.0),
		AvgRatio(0.0), MinRatio(0.0), Ratio(0.0), WOverB(0.0), SimWB(0.0) {}
	int Compare(const Stat&) {return(-1);}
	int Compare(const Stat*) {return(-1);}
};



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,const char* l,bool global,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), IdealGroups(2,1), Lang(Doc->GetSession()->GetLang(l)),
	  Global(global), Stats(50,25)
{
	// Construct chromosomes
	General = new QListViewChromos(this);
	ConstructChromosomes();
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomes(void)
{
	GChromoIR* c;
	unsigned int i;
	QListViewItem* g;
	char tmp[20];
	GInstIR* Instance;
	GSubProfileCursor Cur=Doc->GetSession()->GetSubProfilesCursor(Lang);
	RObjs<GObjIR> Objs(Cur.GetNb());
	RContainer<GSubProfile,unsigned int,false,true> SubProfiles(Cur.GetNb(),50);
	Stat* s;
	GSubProfile* sub;

	// Initialise the dialog box
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Stored Chromosomes");
	d->show();
	KApplication::kApplication()->processEvents();

	// Load Ideal Groups;
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

	// Construct the GA Objects
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sub=Cur();
		if(sub->IsDefined())
			SubProfiles.InsertPtr(sub);
	}
	for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
			Objs.InsertPtr(new GObjIR(i,SubProfiles()));
	GProfilesSim Sims(SubProfiles,Global);

	// Loal the chromosomes from the db
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,&Sims,Global);
	if(!Instance) return;
	Instance->SetIdealGroups(&IdealGroups);

	// Display the chromosomes
	for(i=0;i<=Instance->PopSize;i++)
	{
		if(i==Instance->PopSize)
			c=Instance->BestChromosome;
		else
			c=Instance->Chromosomes[i];
	
		Stats.InsertPtr(s=new Stat());

		s->Id=c->Id;
		sprintf(tmp,"%u",c->Id);
		d->receiveNextChromosome(c->Id);
		g=new MyListViewItem(General,tmp);

		c->CompareIdeal(Doc->GetSession(),&IdealGroups);
		s->Precision=c->GetPrecision();
		sprintf(tmp,"%f",c->GetPrecision());
		g->setText(1,tmp);
		s->Recall=c->GetRecall();
		sprintf(tmp,"%f",c->GetRecall());
		g->setText(2,tmp);
		s->Global=c->GetGlobal();
		sprintf(tmp,"%f",c->GetGlobal());
		g->setText(3,tmp);

		c->EvaluateAvgSim();
		s->AvgSim=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(4,tmp);

		c->EvaluateJ();
		s->J=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(5,tmp);

		c->EvaluateAvgRatio();
		s->AvgRatio=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(6,tmp);

		c->EvaluateMinRatio();
		s->MinRatio=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(7,tmp);

		c->EvaluateRatio();
		s->Ratio=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(8,tmp);

		c->EvaluateWOverB();
		s->WOverB=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(9,tmp);

		c->EvaluateSimWB();
		s->SimWB=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
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
void KViewChromos::slotMenu(int)
{
	RCursor<Stat,tId> Cur;
	int dlg;
	KURL url;

	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.txt"), this, i18n("Save Statistics..."));
		if(url.isEmpty()) return;
		QFile Test(url.path().latin1());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}
	RTextFile Res(url.path().latin1(),RTextFile::Create);
	Res.SetSeparator("\t");
	Res<<"Id"<<"Precision"<<"Recall"<<"Global"<<"AvgSim"<<"J"<<"AvgRatio"<<"MinRatio"<<"Ratio"<<"W Over B"<<"Sim WB"<<endl;
	Cur.Set(Stats);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Res<<Cur()->Id<<Cur()->Precision<<Cur()->Recall<<Cur()->Global;
		Res<<Cur()->AvgSim<<Cur()->J<<Cur()->AvgRatio<<Cur()->MinRatio<<Cur()->Ratio<<Cur()->WOverB<<Cur()->SimWB<<endl;
	}
}


//-----------------------------------------------------------------------------
KViewChromos::~KViewChromos(void)
{
}
