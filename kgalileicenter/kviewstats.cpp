/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
#include <rstring.h>
#include <rtextfile.h>
#include <rqt.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <gsession.h>
#include <gtool.h>
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
void KViewStats::ComputeStats(void)
{
	int i;
	GPlugInManager* Mng=GALILEIApp->GetManager("Tools");

	if(!Mng)
	{
		QMessageBox::critical(this,"KGALILEICenter","No manager for the statistics plug-ins");
		return;
	}
	KProgressDialog Dlg(this,"Compute Statistics");

	// Look for the tools that are similarities
	RContainer<GTool,false,false> Tools(10);
	RCursor<GPlugInList> Cur(Mng->GetPlugInLists());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName().FindStr("Statistics")==-1)
			continue;

		GTool* ptr(Mng->GetCurrentPlugIn<GTool>(Cur()->GetName(),0));
		if(ptr)
			Tools.InsertPtr(ptr);
	}

	// Set the progress bar correctly
	Dlg.progressBar()->setMaximum(static_cast<int>(Tools.GetNb()+1));

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	Dlg.progressBar()->setValue(0);
	KApplication::kApplication()->processEvents();
	R::RCursor<GTool> Tool(Tools);
	for(Tool.Start(),i=1;!Tool.End();Tool.Next(),i++)
	{
		Dlg.progressBar()->setValue(i);
		Dlg.setLabelText(ToQString(Tool()->GetName()));
		KApplication::kApplication()->processEvents();
		if(Dlg.wasCancelled())
			break;
		Tool()->Run(0);

		// Add the results to the text
		RParamValue* FileName(Tool()->FindParam<RParamValue>("Results"));
		if(!FileName)
			continue;
		RTextFile File(FileName->Get(),"utf-8");
		File.Open(RIO::Read);
		while(!File.End())
			Results->appendPlainText(ToQString(File.GetLine(false)));
		Results->appendPlainText("\n\n");
	}
	Dlg.progressBar()->setValue(i);
}


//-----------------------------------------------------------------------------
KViewStats::~KViewStats(void)
{
}
