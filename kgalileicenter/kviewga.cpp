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
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <profiles/gsubprofile.h>
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
KViewGA::KViewGA(KDoc* doc,unsigned int pop,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), RGASignalsReceiver<GInstIR,GChromoIR,GFitnessIR>(),
	  CurId(0), Instance(0), Gen(0), PopSize(pop)
{
	static char tmp[100];

	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(rect());
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Stat part
	StatSplitter=new QSplitter(QSplitter::Vertical,TabWidget,"Statistic");
	TabWidget->insertTab(StatSplitter,"Statistic");
	StatSplitter->setGeometry(rect());
	Monitor=new	QGAMonitor(StatSplitter);
	Monitor->setMaxGen(pop);
	connect(this,SIGNAL(signalSetGen(unsigned int,unsigned int,double)),Monitor,SLOT(slotSetGen(unsigned int,unsigned int,double)));
	Debug=new QXMLContainer(StatSplitter);

	// Solution part
	Best = new QListView(/*pDoc,*/TabWidget);
	TabWidget->insertTab(Best,"Best Solution");

	// Solution part
	Sol = new QListView(/*pDoc,*/TabWidget);
	sprintf(tmp,"Solution (0/%u)",PopSize-1);
	TabWidget->insertTab(Sol,tmp);

	// Create GA
	try
	{
//		Instance=new GInstIR(Doc->GetMaxGen(),PopSize,pDoc,theApp->GAHeur,Debug);
//		Instance->AddReceiver(this);
//		Instance->Init();
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
}


//-----------------------------------------------------------------------------
void KViewGA::update(unsigned int /*cmd*/)
{
}


//---------------------------------------------------------------------------
void KViewGA::receiveGenSig(GenSig* sig)
{
	emit signalSetGen(sig->Gen,sig->BestGen,sig->Best->Fitness->Value);
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

	sprintf(tmp,"Best Solution (Id=%u)",sig->Best->Id);
	TabWidget->changeTab(Best,tmp);
}


//---------------------------------------------------------------------------
void KViewGA::RunGA(void)
{
//	if(Instance)
//	{
//		try
//		{
//			if(Doc->GetMaxGen()>Gen)
//			{
//				if(Doc->GetStepGen()==0)
//					Gen=Doc->GetMaxGen();
//				else
//				{
//					Gen+=Doc->GetStepGen();
//					if(Gen>Doc->GetMaxGen()) Gen=Doc->GetMaxGen();
//				}
//			}
//			Instance->MaxGen=Gen;
//			Instance->Run();
//			KMessageBox::information(this,"Done");
//		}
//		catch(eGA& e)
//		{
//			KMessageBox::error(this,QString(e.Msg));
//		}
//	}
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
			sprintf(tmp,"Solution (%u/%u)",CurId,PopSize-1);
			TabWidget->changeTab(Sol,tmp);
			break;

		case Key_PageDown:
			if(CurId>0) CurId--; else CurId=PopSize-1;
			sprintf(tmp,"Solution (%u/%u)",CurId,PopSize-1);
			TabWidget->changeTab(Sol,tmp);
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
}
