/*

	GALILEI Research Project

	KViewGA.cpp

	Window for the GA - Implementation.

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
// include files for GALILEI
#include <langs/glang.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gprofilessim.h>
#include <sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewga.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGA::KViewGA(KDoc* doc,const char* l,bool global,bool scratch,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), RGASignalsReceiver<GInstIR,GChromoIR,GFitnessIR>(),
	  CurId(0), Instance(0), Gen(0), PopSize(0),ParamsSim(0.2,0.05,1.0),
	  ParamsNb(0.2,0.05,1.0), ParamsOK(0.2,0.05,1.0), ParamsDiff(0.2,0.05,1.0),
	  SubProfiles(0), Objs(0), Sims(0), IdealGroups(2,1)
{
	static char tmp[100];
	GLang* lang;
	GSubProfile* sub;
	GGroupDataIR g;
	unsigned int i;
	char c,c1;
	unsigned int t;

	// Window
	lang=Doc->GetSession()->GetLang(l);
	setCaption(QString("GALILEI Genetic Algorithms - ")+lang->GetName());

	// Values
	sscanf(Doc->GetSession()->GetGroupingMethodSettings("Grouping Genetic Algorithms"),
           "%u %c %u %u %c %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &t,&c1,&PopSize,&MaxGen,&c,&StepGen,&MinSimLevel,&MinCommonOK,&MinCommonDiff,
	       &ParamsSim.P,&ParamsSim.Q,&ParamsSim.Weight,
	       &ParamsNb.P,&ParamsNb.Q,&ParamsNb.Weight,
	       &ParamsOK.P,&ParamsOK.Q,&ParamsOK.Weight,
	       &ParamsDiff.P,&ParamsDiff.Q,&ParamsDiff.Weight,
	       &ParamsSocial.P,&ParamsSocial.Q,&ParamsSocial.Weight);
	if(c=='1') Step=true; else Step=false;

	// Tab
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(rect());
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Stat part
	StatSplitter=new QSplitter(QSplitter::Vertical,TabWidget,"Statistic");
	TabWidget->insertTab(StatSplitter,"Statistic");
	StatSplitter->setGeometry(rect());
	Monitor=new QGAMonitor(StatSplitter);
	Monitor->setMaxGen(MaxGen);
	#ifdef RGADEBUG
		Monitor->setMaxFitness(1.0);
	#else
		Monitor->setMaxFitness(MaxGen+1.&);
	#endif
	connect(this,SIGNAL(signalSetGen(const unsigned int,const unsigned int,const double)),Monitor,SLOT(slotSetGen(const unsigned int,const unsigned int,const double)));
	Debug=new QXMLContainer(StatSplitter,"GALILEI Genetic Algorithms","Pascal Francq");

	// Load Ideal Groups;
	Doc->GetSession()->LoadIdealGroupment(&IdealGroups);

	// Go through the profiles corresponding to the language and that are
	// to inserted.
	GProfileCursor cur=Doc->GetSession()->GetProfilesCursor();
	SubProfiles=new RStd::RContainer<GSubProfile,unsigned int,false,true>(cur.GetNb());
	Objs=new RGA::RObjs<GObjIR>(cur.GetNb());
	for(cur.Start(),i=0;!cur.End();cur.Next())
	{
		sub=cur()->GetSubProfile(lang);
		if(sub->IsDefined())
		{
			SubProfiles->InsertPtr(sub);
			i++;
		}
	}
	for(SubProfiles->Start(),i=0;!SubProfiles->End();SubProfiles->Next(),i++)
			Objs->InsertPtr(new GObjIR(i,(*SubProfiles)()));
	Sims=new GProfilesSim(SubProfiles,global);

	// Create GA
	try
	{
		if(scratch)
			Instance=new GInstIR(Doc->GetSession(),lang,MinSimLevel,MaxGen,PopSize,0,Objs,global,Sims,static_cast<SimType>(t),Debug);
		else
			Instance=new GInstIR(Doc->GetSession(),lang,MinSimLevel,MaxGen,PopSize,Doc->GetSession()->GetGroups(lang),Objs,global,Sims,static_cast<SimType>(t),Debug);
		Instance->SetCriterionParam("Similarity",ParamsSim.P,ParamsSim.Q,ParamsSim.Weight);
		Instance->SetCriterionParam("Nb Profiles",ParamsNb.P,ParamsNb.Q,ParamsNb.Weight);
		Instance->SetCriterionParam("OK Factor",ParamsOK.P,ParamsOK.Q,ParamsOK.Weight);
		Instance->SetCriterionParam("Diff Factor",ParamsDiff.P,ParamsDiff.Q,ParamsDiff.Weight);
		Instance->AddReceiver(this);
		Instance->Init(&g);
		Instance->SetIdealGroups(&IdealGroups);
	}
	catch(eGA& e)
	{
		KMessageBox::error(this,QString(e.Msg));
		Instance=0;
	}
	catch(bad_alloc)
	{
		KMessageBox::error(this,"Memory Problems");
		Instance=0;
	}
	catch(...)
	{
		KMessageBox::error(this,"Unknow Problem");
		Instance=0;
	}

	// Solutions, ideal and Best Part
	Best = new QGGroupsIR(TabWidget,Objs);
	TabWidget->insertTab(Best,"Best Solution");
	connect(Best,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	Sol = new QGGroupsIR(TabWidget,Objs);
	#ifdef RGADEBUG
		sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
	#else
		sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->Fitness->Value);
	#endif
	TabWidget->insertTab(Sol,tmp);
	connect(Sol,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	Ideal = new QGGroupsIR(TabWidget,IdealGroups.GetPtr<const GLang*>(lang),Objs);
	TabWidget->insertTab(Ideal,"Ideal Solution");
}


//-----------------------------------------------------------------------------
void KViewGA::update(unsigned int /*cmd*/)
{
}


//---------------------------------------------------------------------------
void KViewGA::receiveGenSig(GenSig* sig)
{
	static char tmp[100];

	#ifdef RGADEBUG
		emit signalSetGen(sig->Gen,sig->BestGen,sig->Best->GetGlobal());
	#else
		emit signalSetGen(sig->Gen,sig->BestGen,sig->Best->Fitness->Value);
	#endif
	Sol->setGroups(Instance->Chromosomes[CurId]);
	Sol->setChanged();
	#ifdef RGADEBUG
		sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
	#else
		sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->Fitness->Value);
	#endif
	TabWidget->changeTab(Sol,tmp);
}


//---------------------------------------------------------------------------
void KViewGA::receiveInteractSig(InteractSig* /*sig*/)
{
	KApplication::kApplication()->processEvents(1000);
}


//---------------------------------------------------------------------------
void KViewGA::receiveBestSig(BestSig* sig)
{
	static char tmp[100];

	#ifdef RGADEBUG
		sprintf(tmp,"Best Solution (Id=%u) - Fitness=%lf",sig->Best->Id,sig->Best->GetGlobal());
	#else
		sprintf(tmp,"Best Solution (Id=%u) - Fitness=%lf",sig->Best->Id,sig->Best->Fitness->Value);
	#endif
	TabWidget->changeTab(Best,tmp);
	Best->setGroups(sig->Best);
	Best->setChanged();
}


//----------------------------------------------------------------------------
void KViewGA::RunGA(void)
{
	if(Instance)
	{
		try
		{
			if(MaxGen>Gen)
			{
				if(!StepGen)
					Gen=MaxGen;
				else
				{
					Gen+=StepGen;
					if(Gen>MaxGen) Gen=MaxGen;
				}
			}
			Instance->SetMaxGen(Gen);
			Instance->Run();
			KMessageBox::information(this,"Done");
		}
		catch(eGA& e)
		{
			KMessageBox::error(this,QString(e.Msg));
		}
	}
}


//---------------------------------------------------------------------------
void KViewGA::PauseGA(void)
{
	ExternBreak=true;
}


//---------------------------------------------------------------------------
void KViewGA::StopGA(void)
{
}


//---------------------------------------------------------------------------
void KViewGA::keyReleaseEvent(QKeyEvent* e)
{
	static char tmp[100];
//	QGoToPopDlg *dlg;

	if(TabWidget->currentPage()!=Sol)
	{
		KView::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Key_PageUp:
			if(CurId<PopSize-1) CurId++; else CurId=0;
			#ifdef RGADEBUG
				sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
			#else
				sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->Fitness->Value);
			#endif
			TabWidget->changeTab(Sol,tmp);
			Sol->setGroups(Instance->Chromosomes[CurId]);
			Sol->setChanged();
			break;

		case Key_PageDown:
			if(CurId>0) CurId--; else CurId=PopSize-1;
			#ifdef RGADEBUG
				sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
			#else
				sprintf(tmp,"Solution (%u/%u) - Fitness=%lf",CurId,PopSize-1,Instance->Chromosomes[CurId]->Fitness->Value);
			#endif
			TabWidget->changeTab(Sol,tmp);
			Sol->setGroups(Instance->Chromosomes[CurId]);
			Sol->setChanged();
			break;

//		case Key_G:
//			if(e->state()==ControlButton)
//			{
//				dlg= new QGoToPopDlg(Doc->getPopSize());
//				if(dlg->exec())
//				{
//					CurId=dlg->PopIndex->value();
//					slotNewChromo();
//				}
//				delete dlg;
//			}
//			break;

		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
void KViewGA::resizeEvent(QResizeEvent*)
{
	TabWidget->resize(size());
}


//-----------------------------------------------------------------------------
KViewGA::~KViewGA(void)
{
	if(Instance)
		delete Instance;
	if(SubProfiles)
		delete SubProfiles;
	if(Sims)
		delete Sims;
	if(Objs)
		delete Objs;
}
