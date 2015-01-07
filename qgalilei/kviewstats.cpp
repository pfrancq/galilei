/*

	GALILEI Research Project

	KViewStats.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
#include <rcsvfile.h>
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
#include <QtGui/QTableWidget>
#include <QProgressDialog>


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
	connect(Stats,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
	connect(Sorting,SIGNAL(stateChanged(int)),this,SLOT(checkSorting(int)));
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
   QProgressDialog Dlg("Compute Statistics","Cancel",0,Tools.GetNb(),this);
	Dlg.setWindowModality(Qt::WindowModal);

	// Compute the statistics
	Dlg.setMinimumDuration(0);
	Dlg.setValue(0);
	QApplication::processEvents();
	R::RCursor<GTool> Tool(Tools);
	for(Tool.Start(),i=1;!Tool.End();Tool.Next(),i++)
	{
		Dlg.setValue(i);
		Dlg.setLabelText(ToQString(Tool()->GetName()));
		QApplication::processEvents();
		if(Dlg.wasCanceled())
			break;

		try
		{
			Tool()->Run(0);

			// Add the results if there is a 'Results' file
			RParamValue* FileName(Tool()->FindParam<RParamValue>("Results"));
			if(!FileName)
				continue;

			// Create a new tab with a new QTableWidget
			size_t line(0),maxline(100);
			size_t col(0);
			QTableWidget* Results(new QTableWidget(100,100));
			Stats->addTab(Results,ToQString(Tool()->GetFactory()->GetList()));

			RCSVFile File(FileName->Get(),';',"utf8");
			File.Open();
			bool Header(false);
			while(!File.End())
			{
				File.Read();

				if(col<File.GetNbValues())
				{
					col=File.GetNbValues();
					Results->setColumnCount(col);
				}

				if(!Header)
				{
					for(size_t j=0;j<File.GetNbValues();j++)
						Results->setHorizontalHeaderItem(j,new QTableWidgetItem(ToQString(File.Get(j))));
					Header=true;
					continue;
				}

				Results->setItem(line,0,new QTableWidgetItem(ToQString(File.Get(0))));
				for(size_t j=1;j<File.GetNbValues();j++)
				{
					if(File.Get(j)!="NAN")
						Results->setItem(line,j,new QTableWidgetItem(QString::number(File.Get(j).ToDouble())));
					else
						Results->setItem(line,j,new QTableWidgetItem("----"));
				}

				line++;
				if(line>maxline)
				{
					maxline=line+100;
					Results->setRowCount(maxline);
				}
			}

			Results->setRowCount(line);
			Results->setSortingEnabled(false);
		}
		catch(RException& e)
		{
			QMessageBox::critical(this,ToQString(Tool()->GetFactory()->GetList()),ToQString(e.GetMsg()));
		}
		catch(std::exception& e)
		{
			QMessageBox::critical(this,ToQString(Tool()->GetFactory()->GetList()),ToQString(e.what()));
		}
		catch(...)
		{
			QMessageBox::critical(this,ToQString(Tool()->GetFactory()->GetList()),"Undefined error");
		}
	}
	Dlg.setValue(i);
	Stats->setCurrentIndex(0);
}


//-----------------------------------------------------------------------------
void KViewStats::tabChanged(int)
{
	QTableWidget* Widget(dynamic_cast<QTableWidget*>(Stats->currentWidget()));
	Sorting->setChecked(Widget&&Widget->isSortingEnabled());
}


//-----------------------------------------------------------------------------
void KViewStats::checkSorting(int state)
{
	QTableWidget* Widget(dynamic_cast<QTableWidget*>(Stats->currentWidget()));
	if(Widget)
		Widget->setSortingEnabled(state);
}


//-----------------------------------------------------------------------------
KViewStats::~KViewStats(void)
{
}
