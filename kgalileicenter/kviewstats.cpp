/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

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
// include file for R Project
#include <rstd/rstring.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/gsession.h>
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstatscalc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qprogressdialog.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewstats.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
//
// class KViewStats
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStats::KViewStats(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	// Init part
	setCaption("Compute Statistics");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_new",KIcon::Small)));

	// Results
	Res = new QListView(this,"Results");
	Res->addColumn("tag name");
	Res->setRootIsDecorated(true);
	Res->setSorting(0,false);
	Res->setSorting(1,false);
	Res->setSorting(2,false);
	Res->setResizeMode(QListView::AllColumns);

	// Compute Statistics
	ComputeStats();
}


//-----------------------------------------------------------------------------
void KViewStats::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewStats::ConstructTag(RXMLTag* t,QListViewItem* parent)
{
 	RXMLTagCursor Cur=t->GetXMLTagsCursor();
 	QListViewItem* ptr=0,*ptr2;
 	QListViewItem* prec=0;

	parent->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xml_element.png",KIcon::Small)));
 	for(Cur.Start();!Cur.End();Cur.Next())
 	{
 		if(!prec)
 		{
 			prec=ptr=new QListViewItem(parent,Cur()->GetName().Latin1());
 		}
 		else
 		{
 			prec=ptr=new QListViewItem(parent,prec,Cur()->GetName().Latin1());
 		}
 		if(strlen(Cur()->GetContent()))
 		{
 			ptr2=new QListViewItem(ptr,Cur()->GetContent());
			ptr2->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xml_etext.png",KIcon::Small)));
 		}
 		ConstructTag(Cur(),ptr);
 	}
}


//-----------------------------------------------------------------------------
void KViewStats::ComputeStats(void)
{
	GFactoryStatsCalcCursor Cur;
	GStatsCalc* Calc;
	RXMLTag Root("Statistics");
	QProgressDialog Dlg("Compute Statistics","Abort",Doc->GetSession()->GetStatsCalcMng()->NbPtr,this, "progress", TRUE );
	int i;

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	KApplication::kApplication()->processEvents();
	Cur.Set(Doc->GetSession()->GetStatsCalcMng());
	for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
	{
		Dlg.setProgress(i);
		Dlg.setLabelText(Cur()->GetName());
		KApplication::kApplication()->processEvents();
		if(Dlg.wasCancelled())
			break;
		Calc=Cur()->GetPlugin();
		if(Calc)
			Calc->Compute(Root);
	}
	Dlg.setProgress(i);

	// Show the results
	ConstructTag(&Root,new QListViewItem(Res,"Statistics"));
}


//-----------------------------------------------------------------------------
void KViewStats::resizeEvent(QResizeEvent* e)
{
	Res->resize(size());
}


//-----------------------------------------------------------------------------
KViewStats::~KViewStats(void)
{
}
