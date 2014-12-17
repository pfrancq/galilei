/*

	KGALILEICenter

	Preferences.cpp

	Preferences Dialog Box - Implementation.

	Copyright 2010 by Paul Otlet Institute.

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
// include files for ANSI C/C++
#include <iostream>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QFileDialog>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>


//------------------------------------------------------------------------------
// includes files for current application
#include <qgalileiwin.h>
#include <preferences.h>
#include <qgalilei.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class Preferences
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Preferences::Preferences(QGALILEIWin* win)
	: QDialog(win), Ui_Preferences(), Win(win)
{
	setupUi(this);
	setWindowTitle("Preferences");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
   connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(buttonAdd,SIGNAL(pressed()),this,SLOT(slotAddDir()));
	connect(buttonDel,SIGNAL(pressed()),this,SLOT(slotDelDir()));
	connect(EditStorage,SIGNAL(pressed()),this,SLOT(editIndexDir()));
	connect(EditScript,SIGNAL(pressed()),this,SLOT(editScriptDir()));
}


//------------------------------------------------------------------------------
void Preferences::exec(void)
{
	IndexDir->setText(ToQString(Win->GetApp()->GetIndexDir()));
	R::RCursor<R::RString> Cur(Win->GetApp()->GetPlugInsPaths());
	for(Cur.Start();!Cur.End();Cur.Next())
		Dirs->addItem(ToQString(*Cur()));
	PrgPath->setText(ToQString(Win->GetApp()->GetScriptPath()));

	if(QDialog::exec())
	{
		Win->GetApp()->SetIndexDir(FromQString(IndexDir->text()));
		RContainer<RString,true,false> dirs(Dirs->count());
		for(int i=0;i<Dirs->count();i++)
			dirs.InsertPtr(new RString(FromQString(Dirs->item(i)->text())));
		Win->GetApp()->SetPlugInsPaths(dirs);
		Win->GetApp()->SetScriptPath(PrgPath->text());
	}
}


//-----------------------------------------------------------------------------
void Preferences::slotAddDir(void)
{
	QString fileName(QFileDialog::getExistingDirectory(this,"Add new directory for plug-ins","",QFileDialog::ShowDirsOnly));
   if(!fileName.isEmpty())
		Dirs->addItem(fileName);
}


//-----------------------------------------------------------------------------
void Preferences::slotDelDir(void)
{
	int row=Dirs->currentRow();
	if(row>-1)
		delete Dirs->item(row);
}


//-----------------------------------------------------------------------------
void Preferences::editIndexDir(void)
{
	QString fileName(QFileDialog::getExistingDirectory(this,"Choose index directory",IndexDir->text(),QFileDialog::ShowDirsOnly));
   if(!fileName.isEmpty())
		IndexDir->setText(fileName);
}


//-----------------------------------------------------------------------------
void Preferences::editScriptDir(void)
{
	QString fileName(QFileDialog::getExistingDirectory(this,"Choose script directory",PrgPath->text(),QFileDialog::ShowDirsOnly));
   if(!fileName.isEmpty())
		PrgPath->setText(fileName);
}
