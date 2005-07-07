/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rxmlstruct.h>
#include <rstd/rstring.h>
#include <rstd/rtextfile.h>
#include <frontend/kde/rqt.h>
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
#include <qmessagebox.h>


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
	Res->addColumn("Element");
	Res->addColumn("Value");
	Res->setRootIsDecorated(true);
	Res->setSorting(-1);
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
	R::RCursor<RXMLTag> Cur(t->GetNodes());
	QListViewItem* ptr=0,*ptr2;
	QListViewItem* prec=0;
	RString Val;

	parent->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xml_element.png",KIcon::Small)));
 	for(Cur.Start();!Cur.End();Cur.Next())
 	{
		Val=Cur()->GetAttrValue("Value");
 		if(!prec)
 		{
			if(Val.IsEmpty())
 				prec=ptr=new QListViewItem(parent,ToQString(Cur()->GetName()));
			else
				prec=ptr=new QListViewItem(parent,ToQString(Cur()->GetName()),ToQString(Val));
 		}
 		else
 		{
			if(Val.IsEmpty())
 				prec=ptr=new QListViewItem(parent,prec,ToQString(Cur()->GetName()));
			else
				prec=ptr=new QListViewItem(parent,prec,ToQString(Cur()->GetName()),ToQString(Val));
 		}
 		if(!Cur()->GetContent().IsEmpty())
 		{
 			ptr2=new QListViewItem(ptr,ToQString(Cur()->GetContent()));
			ptr2->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xml_etext.png",KIcon::Small)));
 		}
 		ConstructTag(Cur(),ptr);
 	}
}


//-----------------------------------------------------------------------------
void KViewStats::ComputeStats(void)
{
	R::RCursor<GFactoryStatsCalc> Cur;
	GStatsCalc* Calc;
	RXMLStruct xml;
	RXMLTag* Root;

	int i;
	GStatsCalcManager* Mng=(dynamic_cast<GStatsCalcManager*>(GPluginManager::GetManager("StatsCalc")));

	if(!Mng)
	{
		QMessageBox::critical(this,"KGALILEICenter","No manager for the statistics plug-ins");
		return;
	}
	QProgressDialog Dlg( "Compute Statistics", "Abort Compute", Mng->GetNb()+1 ,this, "progress", TRUE );


	// Create the root node
	Root=new RXMLTag("Statistics");
	xml.AddTag(0,Root);

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	Dlg.setProgress(0);
	KApplication::kApplication()->processEvents();
	Cur.Set(*Mng);
	for(Cur.Start(),i=1;!Cur.End();Cur.Next(),i++)
	{
		Dlg.setProgress(i);
		Dlg.setLabelText(ToQString(Cur()->GetName()));
		KApplication::kApplication()->processEvents();
		if(Dlg.wasCancelled())
			break;
		Calc=Cur()->GetPlugin();
		if(Calc)
		{
			Calc->Compute(&xml,*Root);
		}
	}
	Dlg.setProgress(i);

	// Show the results
	ConstructTag(Root,new QListViewItem(Res,"Statistics"));
}


//-----------------------------------------------------------------------------
void KViewStats::resizeEvent(QResizeEvent*)
{
	Res->resize(size());
}


//-----------------------------------------------------------------------------
KViewStats::~KViewStats(void)
{
}
