/*

	KGALILEICenter

	QChooseDlg.cpp

	Choose a tool to run - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI Project
#include <rqt.h>
#include <gtool.h>


//------------------------------------------------------------------------------
// include files
#include <qchoosedlg.h>
#include <qgalileiwin.h>
#include <qgalilei.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class QChooseDlg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QChooseDlg::QChooseDlg(QGALILEIWin* win)
	: QDialog(win), Ui_QChooseDlg(), Win(win)
{
	setupUi(this);
	setWindowTitle("Choose Tool to Run");
	QObject::connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
   QObject::connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	adjustSize();
}


////------------------------------------------------------------------------------
//void QChooseDlg::exec(void)
//{
//	GPlugInManager* Manager(GALILEIApp->GetManager("Tools"));
//
//	// Init the dialog box with the lists
//	Desc->setText("Choose the tool to run");
//	RCursor<GPlugInList> Lists(Manager->GetPlugInLists());
//	int Row(0),i;
//	for(Lists.Start(),i=0;!Lists.End();Lists.Next(),i++)
//	{
//		if(Lists()->GetName()==ToolCat)
//			Row=i;
//		List->addItem(ToQString(Lists()->GetName()));
//	}
//	List->setCurrentRow(Row);
//	if(!Choose.exec())
//		return;
//	QString ToolCat=FromQString(Ui.List->item(Ui.List->currentRow())->text());
//
//	// Init the dialog box with the plug-ins
//	RCastCursor<GPlugIn,GTool> Tools(Manager->GetPlugIns<GTool>(ToolCat));
//	List->clear();
//	for(Tools.Start(),i=0,Row=0;!Tools.End();Tools.Next(),i++)
//	{
//		if(Tools()->GetName()==Tool)
//			Row=i;
//		List->addItem(ToQString(Tools()->GetName()));
//	}
//	List->setCurrentRow(Row);
//
//
////	Tool=FromQString(Ui.List->item(Ui.List->currentRow())->text());
////	QRunTool(this,Tool,ToolCat).run();
//
//}


//------------------------------------------------------------------------------
void QChooseDlg::populateToolCats(const RString& cat)
{
	GPlugInManager* Manager(GALILEIApp->GetManager("Tools"));

	// Init the dialog box with the lists
	Desc->setText("Choose the tool category to run");
	RCursor<GPlugInList> Lists(Manager->GetPlugInLists());
	List->clear();
	int Row(0),i;
	for(Lists.Start(),i=0;!Lists.End();Lists.Next(),i++)
	{
		if(Lists()->GetName()==cat)
			Row=i;
		List->addItem(ToQString(Lists()->GetName()));
	}
	List->setCurrentRow(Row);
}


//------------------------------------------------------------------------------
void QChooseDlg::populateTools(const RString& cat,const RString& tool)
{
	GPlugInManager* Manager(GALILEIApp->GetManager("Tools"));

	// Init the dialog box with the plug-ins
	Desc->setText("Choose the tool to run");
	RCastCursor<GPlugIn,GTool> Tools(Manager->GetPlugIns<GTool>(cat));
	List->clear();
	int Row(0),i;
	for(Tools.Start(),i=0,Row=0;!Tools.End();Tools.Next(),i++)
	{
		if(Tools()->GetName()==tool)
			Row=i;
		List->addItem(ToQString(Tools()->GetName()));
	}
	List->setCurrentRow(Row);
}


//------------------------------------------------------------------------------
RString QChooseDlg::GetChoice(void) const
{
	return(FromQString(List->item(List->currentRow())->text()));
}
