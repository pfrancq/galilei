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
	double Fitness;
	double CritSim;
	double CritInfo;
	double CritSame;
	double CritDiff;
	double CritSocial;
	double FiPlus;
	double FiMinus;
	double Fi;
	unsigned int NbGroups;

	Stat(void) : Id(NullId), Precision(0.0), Recall(0.0), Global(0.0), AvgSim(0.0), J(0.0),
		AvgRatio(0.0), MinRatio(0.0), Ratio(0.0), WOverB(0.0), SimWB(0.0), Fitness(0.0),
		CritSim(0.0), CritInfo(0.0), CritSame(0.0), CritDiff(0.0), CritSocial(0.0),
		FiPlus(0.0), FiMinus(0.0), Fi(0.0) {}
	int Compare(const Stat&) {return(-1);}
	int Compare(const Stat*) {return(-1);}
};



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,const char* l,bool global,bool sim,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), IdealGroups(2,1), Lang(Doc->GetSession()->GetLang(l)),
	  Global(global), Sim(sim), Stats(50,25)
{
	// Construct chromosomes
	General = new QListViewChromos(this);
	if(Sim)
	{
		General->addColumn("Id");
		General->addColumn("Precision");
		General->addColumn("Recall");
		General->addColumn("Global");
		General->addColumn("AvgSim");
		General->addColumn("J");
		General->addColumn("AvgRatio");
		General->addColumn("MinRatio");
		General->addColumn("Ratio");
		General->addColumn("W Over B");
		General->addColumn("Sim WB");
		General->addColumn("Nb Groups");
		for(int i=0;i<12;i++)
		{
			General->setColumnWidthMode(i,QListView::Maximum);
			General->setColumnAlignment(i,Qt::AlignHCenter);
		}
		ConstructChromosomesSim();
	}
	else
	{
		General->addColumn("Id");
		General->addColumn("Precision");
		General->addColumn("Recall");
		General->addColumn("Global");
		General->addColumn("Ranking");
		General->addColumn("CritSim");
		General->addColumn("CritInfo");
		General->addColumn("CritSame");
		General->addColumn("CritDiff");
		General->addColumn("CritSocial");
		General->addColumn("Fi+");
		General->addColumn("Fi-");
		General->addColumn("Fi");
		General->addColumn("Nb Groups");
		for(int i=0;i<14;i++)
		{
			General->setColumnWidthMode(i,QListView::Maximum);
			General->setColumnAlignment(i,Qt::AlignHCenter);
		}
		ConstructChromosomesRanking();
	}
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomesSim(void)
{
	GChromoIR* c;
	unsigned int i;
	QListViewItem* g;
	char tmp[50];
	GInstIR* Instance;
	GSubProfileCursor Cur=Doc->GetSession()->GetSubProfilesCursor(Lang);
	RObjs<GObjIR> Objs(Cur.GetNb());
	RContainer<GSubProfile,unsigned int,false,true> SubProfiles(Cur.GetNb(),50);
	Stat* s;
	GSubProfile* sub;

	// Initialise the dialog box
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Stored Chromosomes");
	d->Begin();

	// Load Ideal Groups;
	d->PutText("Load Ideal Groups");
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

	// Construct the GA Objects
	d->PutText("Construct the GA Objects");
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sub=Cur();
		if(sub->IsDefined())
			SubProfiles.InsertPtr(sub);
	}
	for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
			Objs.InsertPtr(new GObjIR(i,SubProfiles()));
	GProfilesSim Sims(SubProfiles,Global);

	// Load the chromosomes from the db
	d->PutText("Load chromosomes");
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,&Sims,Global,static_cast<SimType>(1));
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

		s->NbGroups=c->Used.NbPtr;
		sprintf(tmp,"%u",c->Used.NbPtr);
		g->setText(11,tmp);
	}

	// Finish.
	delete Instance;
	d->Finish();
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomesRanking(void)
{
	GChromoIR* c;
	unsigned int i;
	QListViewItem* g;
	char tmp[50];
	GInstIR* Instance;
	GSubProfileCursor Cur=Doc->GetSession()->GetSubProfilesCursor(Lang);
	RObjs<GObjIR> Objs(Cur.GetNb());
	RContainer<GSubProfile,unsigned int,false,true> SubProfiles(Cur.GetNb(),50);
	Stat* s;
	GSubProfile* sub;
	unsigned int MaxGen;
	unsigned int StepGen;
	bool Step;
	double MinSimLevel;
	double MinCommonOK;
	double MinCommonDiff;
	unsigned int PopSize;
	RPromethee::RPromCriterionParams ParamsSim;
	RPromethee::RPromCriterionParams ParamsNb;
	RPromethee::RPromCriterionParams ParamsOK;
	RPromethee::RPromCriterionParams ParamsDiff;
	RPromethee::RPromCriterionParams ParamsSocial;
	char car,c1;
	unsigned int t;

	// Values
	sscanf(Doc->GetSession()->GetGroupingMethodSettings("Grouping Genetic Algorithms"),
           "%u %c %u %u %c %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &t,&c1,&PopSize,&MaxGen,&car,&StepGen,&MinSimLevel,&MinCommonOK,&MinCommonDiff,
	       &ParamsSim.P,&ParamsSim.Q,&ParamsSim.Weight,
	       &ParamsNb.P,&ParamsNb.Q,&ParamsNb.Weight,
	       &ParamsOK.P,&ParamsOK.Q,&ParamsOK.Weight,
	       &ParamsDiff.P,&ParamsDiff.Q,&ParamsDiff.Weight,
	       &ParamsSocial.P,&ParamsSocial.Q,&ParamsSocial.Weight);
	if(car=='1') Step=true; else Step=false;

	// Initialise the dialog box
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Stored Chromosomes");
	d->Begin();

	// Load Ideal Groups;
	d->PutText("Load Ideal Groups");
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

	// Construct the GA Objects
	d->PutText("Construct the GA Objects");
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sub=Cur();
		if(sub->IsDefined())
			SubProfiles.InsertPtr(sub);
	}
	for(SubProfiles.Start(),i=0;!SubProfiles.End();SubProfiles.Next(),i++)
			Objs.InsertPtr(new GObjIR(i,SubProfiles()));
	GProfilesSim Sims(SubProfiles,Global);

	// Load the chromosomes from the db
	d->PutText("Load Chromosomes");
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,&Sims,Global,static_cast<SimType>(t));
	if(!Instance) return;
	Instance->SetIdealGroups(&IdealGroups);
	Instance->SetCriterionParam("Similarity",ParamsSim.P,ParamsSim.Q,ParamsSim.Weight);
	Instance->SetCriterionParam("Information",ParamsNb.P,ParamsNb.Q,ParamsNb.Weight);
	Instance->SetCriterionParam("Same Feedbacks",ParamsOK.P,ParamsOK.Q,ParamsOK.Weight);
	Instance->SetCriterionParam("Diff Feedbacks",ParamsDiff.P,ParamsDiff.Q,ParamsDiff.Weight);
	Instance->SetCriterionParam("Social",ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
	d->PutText("Evaluate the solutions");
	Instance->Evaluate();
	Instance->BestChromosome->Evaluate();
	Instance->PostEvaluate();

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
		s->Fitness=c->Fitness->Value;
		sprintf(tmp,"%f",c->Fitness->Value);
		g->setText(4,tmp);
		s->CritSim=c->GetSimCriterion();
		sprintf(tmp,"%f",c->GetSimCriterion());
		g->setText(5,tmp);
		s->CritInfo=c->GetInfoCriterion();
		sprintf(tmp,"%f",c->GetInfoCriterion());
		g->setText(6,tmp);
		s->CritSame=c->GetSameFeedbacksCriterion();
		sprintf(tmp,"%f",c->GetSameFeedbacksCriterion());
		g->setText(7,tmp);
		s->CritDiff=c->GetDiffFeedbacksCriterion();
		sprintf(tmp,"%f",c->GetDiffFeedbacksCriterion());
		g->setText(8,tmp);
		s->CritSocial=c->GetSocialCriterion();
		sprintf(tmp,"%f",c->GetSocialCriterion());
		g->setText(9,tmp);
		s->FiPlus=c->GetFiPlus();
		sprintf(tmp,"%f",c->GetFiPlus());
		g->setText(10,tmp);
		s->FiMinus=c->GetFiMinus();
		sprintf(tmp,"%f",c->GetFiMinus());
		g->setText(11,tmp);
		s->Fi=c->GetFi();
		sprintf(tmp,"%f",c->GetFi());
		g->setText(12,tmp);
		s->NbGroups=c->Used.NbPtr;
		sprintf(tmp,"%u",c->Used.NbPtr);
		g->setText(13,tmp);
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
	RTextFile Res(url.path().latin1(),RIO::Create);
	Res.SetSeparator("\t");
	if(Sim)
		Res<<"Id"<<"Precision"<<"Recall"<<"Global"<<"AvgSim"<<"J"<<"AvgRatio"<<"MinRatio"<<"Ratio"<<"WOverB"<<"SimWB"<<"NbGroups"<<endl;
	else
		Res<<"Id"<<"Precision"<<"Recall"<<"Global"<<"Ranking"<<"CritSim"<<"CritInfo"<<"CritSame"<<"CritDiff"<<"CritSocial"<<"Fi+"<<"Fi-"<<"Fi"<<"NbGroups"<<endl;
	Cur.Set(Stats);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Res<<Cur()->Id<<Cur()->Precision<<Cur()->Recall<<Cur()->Global;
		if(Sim)
		{
			Res<<Cur()->AvgSim<<Cur()->J<<Cur()->AvgRatio<<Cur()->MinRatio;
			Res<<Cur()->Ratio<<Cur()->WOverB<<Cur()->SimWB<<Cur()->NbGroups<<endl;
		}
		else
		{
			Res<<Cur()->Fitness<<Cur()->CritSim<<Cur()->CritInfo<<Cur()->CritSame<<Cur()->CritDiff;
			Res<<Cur()->CritSocial<<Cur()->FiPlus<<Cur()->FiMinus<<Cur()->Fi<<Cur()->NbGroups<<endl;
		}
	}
}


//-----------------------------------------------------------------------------
KViewChromos::~KViewChromos(void)
{
}
