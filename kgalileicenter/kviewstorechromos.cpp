/*

	GALILEI Research Project

	KViewStoreChromos.cpp

	Window for the GA - Implementation.

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
#include "kviewstorechromos.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewStoreChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStoreChromos::KViewStoreChromos(KDoc* doc,const char* l,GIRParams* p,bool global,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), CurId(0), Instance(0), SubProfiles(0), Objs(0)
{
	static char tmp[100];
	GLang* lang;
	unsigned int i;
	GSubProfileCursor Cur;
	GSubProfile* sub;

	// Window
	lang=Doc->GetSession()->GetLang(l);
	setCaption(QString("Show Store Chromosomes - ")+lang->GetName());

	// Tab
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(rect());
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Construct the GA Objects
	Cur=Doc->GetSession()->GetSubProfilesCursor(lang);
	GProfileCursor cur=Doc->GetSession()->GetProfilesCursor();
	SubProfiles=new RStd::RContainer<GSubProfile,unsigned int,false,true>(cur.GetNb());
	Objs=new RGA::RObjs<GObjIR>(cur.GetNb());

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sub=Cur();
//		if(sub->IsDefined())
			SubProfiles->InsertPtr(sub);
	}
	for(SubProfiles->Start(),i=0;!SubProfiles->End();SubProfiles->Next(),i++)
			Objs->InsertPtr(new GObjIR(i,(*SubProfiles)()));

	// Load the chromosomes from the db
	Instance=Doc->GetSession()->LoadInstIR(lang,Objs,p);
	if(!Instance) return;

	// Solutions Part
	Sol = new QGGroupsIR(TabWidget,Objs);
	sprintf(tmp,"Solution (%u/%u) - Fitness=%f",CurId,Instance->PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
	TabWidget->insertTab(Sol,tmp);
	connect(Sol,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	Sol->setGroups(Instance->Chromosomes[CurId]);
	Sol->setChanged();
}


//-----------------------------------------------------------------------------
void KViewStoreChromos::update(unsigned int /*cmd*/)
{
}


//---------------------------------------------------------------------------
void KViewStoreChromos::keyReleaseEvent(QKeyEvent* e)
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
			if(CurId<Instance->PopSize-1) CurId++; else CurId=0;
			sprintf(tmp,"Solution (%u/%u) - Fitness=%f",CurId,Instance->PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
			TabWidget->changeTab(Sol,tmp);
			Sol->setGroups(Instance->Chromosomes[CurId]);
			Sol->setChanged();
			break;

		case Key_PageDown:
			if(CurId>0) CurId--; else CurId=Instance->PopSize-1;
			sprintf(tmp,"Solution (%u/%u) - Fitness=%f",CurId,Instance->PopSize-1,Instance->Chromosomes[CurId]->GetGlobal());
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
void KViewStoreChromos::resizeEvent(QResizeEvent*)
{
	TabWidget->resize(size());
}


//-----------------------------------------------------------------------------
KViewStoreChromos::~KViewStoreChromos(void)
{
	if(Instance)
		delete Instance;
	if(SubProfiles)
		delete SubProfiles;
	if(Objs)
		delete Objs;
}
