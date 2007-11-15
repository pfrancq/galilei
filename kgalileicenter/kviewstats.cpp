/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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
#include <rxmlstruct.h>
#include <rstring.h>
#include <rtextfile.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <gsession.h>
#include <gstatscalc.h>
#include <ggalileiapp.h>
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
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("mathematica.png",KIcon::Small)));

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
 			ptr2=new QListViewItem(ptr,ToQString(Cur()->GetContent()));
 		ConstructTag(Cur(),ptr);
 	}
}


//-----------------------------------------------------------------------------
void KViewStats::ComputeStats(void)
{
	RXMLStruct xml;
	RXMLTag* Root;

	int i;
	GStatsCalcManager* Mng=GALILEIApp->GetManager<GStatsCalcManager>("StatsCalc");

	if(!Mng)
	{
		QMessageBox::critical(this,"KGALILEICenter","No manager for the statistics plug-ins");
		return;
	}
	QProgressDialog Dlg( "Compute Statistics", "Abort Compute", Mng->GetNbPlugIns()+1 ,this, "progress", TRUE );


	// Create the root node
	Root=new RXMLTag("Statistics");
	xml.AddTag(0,Root);

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	Dlg.setProgress(0);
	KApplication::kApplication()->processEvents();
	R::RCursor<GStatsCalc> Cur(Mng->GetPlugIns());
	for(Cur.Start(),i=1;!Cur.End();Cur.Next(),i++)
	{
		Dlg.setProgress(i);
		Dlg.setLabelText(ToQString(Cur()->GetPlugInName()));
		KApplication::kApplication()->processEvents();
		if(Dlg.wasCancelled())
			break;
		Cur()->Compute(&xml,*Root);
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
