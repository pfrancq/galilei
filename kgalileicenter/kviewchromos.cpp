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
	virtual void contentsMousePressEvent(QMouseEvent* e);
};


//-----------------------------------------------------------------------------
QListViewChromos::QListViewChromos(QWidget* parent,const char* name,WFlags f)
	: QListView(parent,name,f)
{
}


//-----------------------------------------------------------------------------
void QListViewChromos::contentsMousePressEvent(QMouseEvent* e)
{
	if(e->button()==RightButton)
	{
		QPopupMenu* InfoBox=new QPopupMenu(parentWidget());
		InfoBox->insertItem("Save Statistics",parentWidget(),SLOT(slotMenu(int)));
		InfoBox->popup(e->globalPos());
	}
	else
		QListView::contentsMousePressEvent(e);
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
	GChromoIR* Chromo;
	tId Id;
	double Precision;
	double Recall;
	double E1;
	double E05;
	double E15;
	double Global;

	Stat(GChromoIR* c) : Chromo(c), Id(NullId), Precision(0.0), Recall(0.0),
	                     E1(0.0), E05(0.0), E15(0.0), Global(0.0) {}
	int Compare(const Stat&) {return(-1);}
	int Compare(const Stat*) {return(-1);}
};



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,const char* l,GIRParams* p,bool sim,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Lang(Doc->GetSession()->GetLang(l)),
	 Sim(sim), Stats(50,25), Params(p), Instance(0)
{
	// Construct chromosomes
	IdealGroups= new RStd::RContainer<GALILEI::GGroups, unsigned int, true, true> (2,1);
	General = new QListViewChromos(this);
	if(Sim)
	{
		General->addColumn("Id");
		General->addColumn("Precision");
		General->addColumn("Recall");
		General->addColumn("E05");
		General->addColumn("E1");
		General->addColumn("E15");
		General->addColumn("Global");
		General->addColumn("AvgSim");
		General->addColumn("J");
		General->addColumn("AvgRatio");
		General->addColumn("MinRatio");
		General->addColumn("Ratio");
		General->addColumn("WOverB");
		General->addColumn("SimWB");
		General->addColumn("NbGrps");
		for(int i=0;i<15;i++)
		{
			General->setColumnWidthMode(i,QListView::Maximum);
			General->setColumnAlignment(i,Qt::AlignHCenter);
		}
		ConstructChromosomesSim();
	}
	else
	{
		unsigned NbCols=8;
		General->addColumn("Id");
		General->addColumn("Precision");
		General->addColumn("Recall");
		General->addColumn("E05");
		General->addColumn("E1");
		General->addColumn("E15");
		General->addColumn("Global");
		General->addColumn("Ranking");

		if(Params->SimMeasures==sctCorl)
		{
			General->addColumn("CritSim");
			NbCols++;
		}
		else
		{
			if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
			{
				General->addColumn("CritAvgSim");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("J")->Use)
			{
				General->addColumn("CritJ");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
			{
				General->addColumn("CritAvgRatio");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
			{
				General->addColumn("CritMinRatio");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
			{
				General->addColumn("CritRatio");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
			{
				General->addColumn("CritWoverB");
				NbCols++;
			}
			if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
			{
				General->addColumn("CritSimWB");
				NbCols++;
			}
		}
		if(Params->ParamsInfo.Weight)
		{
			General->addColumn("CritInfo");
			NbCols++;
		}
		if(Params->ParamsEntropy.Weight)
		{
			General->addColumn("CritEntropy");
			NbCols++;
		}
		if(Params->ParamsSameFeedbacks.Weight)
		{
			General->addColumn("CritSame");
			NbCols++;
		}
		if(Params->ParamsDiffFeedbacks.Weight)
		{
			General->addColumn("CritDiff");
			NbCols++;
		}
		if(Params->ParamsSocial.Weight)
		{
			General->addColumn("CritSocial");
			NbCols++;
		}
		General->addColumn("Fi+");
		General->addColumn("Fi-");
		General->addColumn("Fi");
		General->addColumn("Nb Groups");
		NbCols+=4;
		for(unsigned int i=0;i<NbCols;i++)
		{
			General->setColumnWidthMode(i,QListView::Maximum);
			General->setColumnAlignment(i,Qt::AlignHCenter);
		}
		ConstructChromosomesRanking();
	}
}


//-----------------------------------------------------------------------------
void KViewChromos::PutDouble(QListViewItem* g,unsigned int col,double val)
{
	char tmp[50];

	sprintf(tmp,"%f",val);
	g->setText(col,tmp);
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomesSim(void)
{
	GChromoIR* c;
	unsigned int i;
	QListViewItem* g;
	char tmp[50];
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
	IdealGroups=Doc->GetSession()->GetIdealGroups();

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

	// Load the chromosomes from the db
	d->PutText("Load chromosomes");
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,Params);
	if(!Instance) return;
	Instance->SetIdealGroups(IdealGroups);

	// Display the chromosomes
	for(i=0;i<=Instance->PopSize;i++)
	{
		if(i==Instance->PopSize)
			c=Instance->BestChromosome;
		else
			c=Instance->Chromosomes[i];
	
		Stats.InsertPtr(s=new Stat(c));

		s->Id=c->Id;
		sprintf(tmp,"%u",c->Id);
		d->receiveNextChromosome(c->Id);
		g=new MyListViewItem(General,tmp);

		c->CompareIdeal(Doc->GetSession(),IdealGroups);
		s->Precision=c->GetPrecision();
		sprintf(tmp,"%f",c->GetPrecision());
		g->setText(1,tmp);
		s->Recall=c->GetRecall();
		sprintf(tmp,"%f",c->GetRecall());
		g->setText(2,tmp);
		if((s->Precision>0.0)&&(s->Recall>0.0))
		{
			s->E05=1-(1+(0.5*0.5))/((1/s->Precision)+((0.5*0.5)/s->Recall));
			s->E1=1-2/((1/s->Precision)+(1/s->Recall));
			s->E15=1-(1+(1.5*1.5))/((1/s->Precision)+((1.5*1.5)/s->Recall));
		}
		else
		{
			s->E05=0;
			s->E1=0;
			s->E15=0;
		}
		PutDouble(g,3,s->E05);
		PutDouble(g,4,s->E1);
		PutDouble(g,5,s->E15);
		s->Global=c->GetGlobal();
		sprintf(tmp,"%f",c->GetGlobal());
		g->setText(6,tmp);

		c->EvaluateSim(stAvgSim);
		sprintf(tmp,"%f",c->GetSimAvgSim());
		g->setText(7,tmp);

		c->EvaluateSim(stJ);
		sprintf(tmp,"%f",c->GetSimJ());
		g->setText(8,tmp);

		c->EvaluateSim(stAvgRatio);
		sprintf(tmp,"%f",c->GetSimAvgRatio());
		g->setText(9,tmp);

		c->EvaluateSim(stMinRatio);
		sprintf(tmp,"%f",c->GetSimMinRatio());
		g->setText(10,tmp);

		c->EvaluateSim(stRatio);
		sprintf(tmp,"%f",c->GetSimRatio());
		g->setText(11,tmp);

		c->EvaluateSim(stWOverB);
		sprintf(tmp,"%f",c->GetSimWOverB());
		g->setText(12,tmp);

		c->EvaluateSim(stSimWB);
		sprintf(tmp,"%f",c->GetSimSimWB());
		g->setText(13,tmp);

		sprintf(tmp,"%u",c->Used.NbPtr);
		g->setText(14,tmp);
	}

	// Finish.
	d->Finish();
}


//-----------------------------------------------------------------------------
void KViewChromos::ConstructChromosomesRanking(void)
{
	GChromoIR* c;
	unsigned int i;
	QListViewItem* g;
	char tmp[50];
	GSubProfileCursor Cur=Doc->GetSession()->GetSubProfilesCursor(Lang);
	RObjs<GObjIR> Objs(Cur.GetNb());
	RContainer<GSubProfile,unsigned int,false,true> SubProfiles(Cur.GetNb(),50);
	Stat* s;
	GSubProfile* sub;
	unsigned int NbCols;

	// Initialise the dialog box
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Stored Chromosomes");
	d->Begin();

	// Load Ideal Groups;
	d->PutText("Load Ideal Groups");
	IdealGroups=Doc->GetSession()->GetIdealGroups();

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

	// Load the chromosomes from the db
	d->PutText("Load Chromosomes");
	Instance=Doc->GetSession()->LoadInstIR(Lang,&Objs,Params);
	if(!Instance) return;
	Instance->SetIdealGroups(IdealGroups);
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


		Stats.InsertPtr(s=new Stat(c));

		s->Id=c->Id;
		sprintf(tmp,"%u",c->Id);
		d->receiveNextChromosome(c->Id);
		g=new MyListViewItem(General,tmp);

		c->CompareIdeal(Doc->GetSession(),IdealGroups);
		s->Precision=c->GetPrecision();
		sprintf(tmp,"%f",c->GetPrecision());
		g->setText(1,tmp);
		s->Recall=c->GetRecall();
		sprintf(tmp,"%f",c->GetRecall());
		g->setText(2,tmp);
		if((s->Precision>0.0)&&(s->Recall>0.0))
		{
			s->E05=1-(1+(0.5*0.5))/((1/s->Precision)+((0.5*0.5)/s->Recall));
			s->E1=1-2/((1/s->Precision)+(1/s->Recall));
			s->E15=1-(1+(1.5*1.5))/((1/s->Precision)+((1.5*1.5)/s->Recall));
		}
		else
		{
			s->E05=0;
			s->E1=0;
			s->E15=0;
		}
		PutDouble(g,3,s->E05);
		PutDouble(g,4,s->E1);
		PutDouble(g,5,s->E15);
		s->Global=c->GetGlobal();
		sprintf(tmp,"%f",c->GetGlobal());
		g->setText(6,tmp);
		sprintf(tmp,"%f",c->Fitness->Value);
		g->setText(7,tmp);
		NbCols=8;
		if(Params->SimMeasures==sctCorl)
		{
			sprintf(tmp,"%f",c->GetSimCriterion());
			g->setText(NbCols++,tmp);
		}
		else
		{
			if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
			{
				sprintf(tmp,"%f",c->GetSimAvgSim());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("J")->Use)
			{
				sprintf(tmp,"%f",c->GetSimJ());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
			{
				sprintf(tmp,"%f",c->GetSimAvgRatio());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
			{
				sprintf(tmp,"%f",c->GetSimMinRatio());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
			{
				sprintf(tmp,"%f",c->GetSimRatio());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
			{
				sprintf(tmp,"%f",c->GetSimWOverB());
				g->setText(NbCols++,tmp);
			}
			if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
			{
				sprintf(tmp,"%f",c->GetSimSimWB());
				g->setText(NbCols++,tmp);
			}
		}
		if(Params->ParamsInfo.Weight)
		{
			sprintf(tmp,"%f",c->GetInfoCriterion());
			g->setText(NbCols++,tmp);
		}
		if(Params->ParamsEntropy.Weight)
		{
			sprintf(tmp,"%f",c->GetEntropyCriterion());
			g->setText(NbCols++,tmp);
		}
		if(Params->ParamsSameFeedbacks.Weight)
		{
			sprintf(tmp,"%f",c->GetSameFeedbacksCriterion());
			g->setText(NbCols++,tmp);
		}
		if(Params->ParamsDiffFeedbacks.Weight)
		{
			sprintf(tmp,"%f",c->GetDiffFeedbacksCriterion());
			g->setText(NbCols++,tmp);
		}
		if(Params->ParamsSocial.Weight)
		{
			sprintf(tmp,"%f",c->GetSocialCriterion());
			g->setText(NbCols++,tmp);
		}
		sprintf(tmp,"%f",c->GetFiPlus());
		g->setText(NbCols++,tmp);
		sprintf(tmp,"%f",c->GetFiMinus());
		g->setText(NbCols++,tmp);
		sprintf(tmp,"%f",c->GetFi());
		g->setText(NbCols++,tmp);
		sprintf(tmp,"%u",c->Used.NbPtr);
		g->setText(NbCols,tmp);
	}

	// Finish.
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
	
	Res<<"Id"<<"Precision"<<"Recall"<<"E05"<<"E1"<<"E15"<<"Global";
	if(Sim)
		Res<<"AvgSim"<<"J"<<"AvgRatio"<<"MinRatio"<<"Ratio"<<"WOverB"<<"SimWB";
	else
	{
		Res<<"Ranking";
		if(Params->SimMeasures==sctCorl)
			Res<<"CritSim";
		else
		{
			if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
				Res<<"CritAvgSim";
			if(Params->Measures.GetPtr<const char*>("J")->Use)
				Res<<"CritJ";
			if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
				Res<<"CritAvgRatio";
			if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
				Res<<"CritMinRatio";
			if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
				Res<<"CritRatio";
			if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
				Res<<"CritWOverB";
			if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
				Res<<"CritSimWB";
		}
		if(Params->ParamsInfo.Weight)
			Res<<"CritInfo";
		if(Params->ParamsEntropy.Weight)
			Res<<"CritEntropy";
		if(Params->ParamsSameFeedbacks.Weight)
			Res<<"CritSame";
		if(Params->ParamsDiffFeedbacks.Weight)
			Res<<"CritDiff";
		if(Params->ParamsSocial.Weight)
			Res<<"CritSocial";
		Res<<"Fi+"<<"Fi-"<<"Fi";
	}
	Res<<"NbGroups"<<endl;
	Cur.Set(Stats);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Res<<Cur()->Id;
		Res<<Cur()->Precision;
		Res<<Cur()->Recall;
		Res<<Cur()->E05;
		Res<<Cur()->E1;
		Res<<Cur()->E15;
		Res<<Cur()->Global;
		if(Sim)
		{
			Res<<Cur()->Chromo->GetSimAvgSim();
			Res<<Cur()->Chromo->GetSimJ();
			Res<<Cur()->Chromo->GetSimAvgRatio();
			Res<<Cur()->Chromo->GetSimMinRatio();
			Res<<Cur()->Chromo->GetSimRatio();
			Res<<Cur()->Chromo->GetSimWOverB();
			Res<<Cur()->Chromo->GetSimSimWB();
		}
		else
		{
			Res<<Cur()->Chromo->Fitness->Value;
			if(Params->SimMeasures==sctCorl)
				Res<<Cur()->Chromo->GetSimCriterion();
			else
			{
				if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
					Res<<Cur()->Chromo->GetSimAvgSim();
				if(Params->Measures.GetPtr<const char*>("J")->Use)
					Res<<Cur()->Chromo->GetSimJ();
				if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
					Res<<Cur()->Chromo->GetSimAvgRatio();
				if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
					Res<<Cur()->Chromo->GetSimMinRatio();
				if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
					Res<<Cur()->Chromo->GetSimRatio();
				if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
					Res<<Cur()->Chromo->GetSimWOverB();
				if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
					Res<<Cur()->Chromo->GetSimSimWB();
			}
			if(Params->ParamsInfo.Weight)
				Res<<Cur()->Chromo->GetInfoCriterion();
			if(Params->ParamsEntropy.Weight)
				Res<<Cur()->Chromo->GetEntropyCriterion();
			if(Params->ParamsSameFeedbacks.Weight)
				Res<<Cur()->Chromo->GetSameFeedbacksCriterion();
			if(Params->ParamsDiffFeedbacks.Weight)
				Res<<Cur()->Chromo->GetDiffFeedbacksCriterion();
			if(Params->ParamsSocial.Weight)
				Res<<Cur()->Chromo->GetSocialCriterion();
			Res<<Cur()->Chromo->GetFiPlus();
			Res<<Cur()->Chromo->GetFiMinus();
			Res<<Cur()->Chromo->GetFi();
		}
		Res<<Cur()->Chromo->Used.NbPtr<<endl;
	}
}


//-----------------------------------------------------------------------------
KViewChromos::~KViewChromos(void)
{
	if(Instance)
		delete Instance;
}
