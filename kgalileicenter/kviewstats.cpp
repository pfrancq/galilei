/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for Qt/KDE
#include <kprogressdialog.h>
#include <QtGui/QMessageBox>
#include <kapplication.h>
//#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewstats.h>



//-----------------------------------------------------------------------------
//
// class KViewStats
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStats::KViewStats(void)
	: QMdiSubWindow(), Ui_KViewStats()
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Statistics");

	// Compute Statistics
	ComputeStats();
}


//-----------------------------------------------------------------------------
void KViewStats::ConstructTag(RXMLTag* t,QTreeWidgetItem* parent)
{
	QTreeWidgetItem* ptr;
	R::RCursor<RXMLTag> Cur(t->GetNodes());
 	for(Cur.Start();!Cur.End();Cur.Next())
 	{
		ptr=new QTreeWidgetItem(parent,QStringList()<<ToQString(Cur()->GetName())<<ToQString(Cur()->GetAttrValue("Value")));
 		if(!Cur()->GetContent().IsEmpty())
 			ptr=new QTreeWidgetItem(parent,QStringList()<<ToQString(Cur()->GetContent())<<"");
 		ConstructTag(Cur(),ptr);
 	}
}


//-----------------------------------------------------------------------------
void KViewStats::ComputeStats(void)
{
	RXMLStruct xml;
	RXMLTag* Root;

	int i;
	GPluginManager* Mng=GALILEIApp->GetManager("StatsCalc");

	if(!Mng)
	{
		QMessageBox::critical(this,"KGALILEICenter","No manager for the statistics plug-ins");
		return;
	}
	KProgressDialog Dlg(this,"Compute Statistics");
	Dlg.progressBar()->setMaximum(static_cast<int>(Mng->GetNbPlugIns())+1);

	// Create the root node
	Root=new RXMLTag("Statistics");
	xml.AddTag(0,Root);

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	Dlg.progressBar()->setValue(0);
	KApplication::kApplication()->processEvents();
	R::RCastCursor<GPlugin,GStatsCalc> Cur(Mng->GetPlugIns<GStatsCalc>());
	for(Cur.Start(),i=1;!Cur.End();Cur.Next(),i++)
	{
		Dlg.progressBar()->setValue(i);
		Dlg.setLabelText(ToQString(Cur()->GetName()));
		KApplication::kApplication()->processEvents();
		if(Dlg.wasCancelled())
			break;
		Cur()->Compute(&xml,*Root);
	}
	Dlg.progressBar()->setValue(i);

	// Show the results
	ConstructTag(Root,new QTreeWidgetItem(Results,QStringList()<<"Statistics"<<""));
}


//-----------------------------------------------------------------------------
KViewStats::~KViewStats(void)
{
}
