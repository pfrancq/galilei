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
//#include <kfiledialog.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>


//------------------------------------------------------------------------------
// includes files for current application
#include <qgalileiwin.h>
#include <preferences.h>
//#include <kapplication.h>
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
}


//------------------------------------------------------------------------------
void Preferences::exec(void)
{
//	IndexDir->setUrl(ToQString(Win->GetIndexDir()));
//	R::RCursor<R::RString> Cur(Win->GetPlugInsPath());
//	for(Cur.Start();!Cur.End();Cur.Next())
//		Dirs->addItem(ToQString(*Cur()));
//	PrgPath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
//	PrgPath->setUrl(ToQString(Win->PrgPath));
//
	if(QDialog::exec())
	{
		cout<<"OK"<<endl;
//		Win->SetIndexDir(FromQString(IndexDir->url().url()));
//		Win->ClearPlugInsPath();
//		for(int i=0;i<Dirs->count();i++)
//			Win->AddPlugInsPath(R::FromQString(Dirs->item(i)->text()));
//		Win->PrgPath=R::FromQString(PrgPath->url().url());
	}
}


//-----------------------------------------------------------------------------
void Preferences::slotAddDir(void)
{
//	QString newdir=KFileDialog::getExistingDirectory(KUrl::fromPath("~"),this,"Add new directory for plug-ins");
//	if((!newdir.isEmpty())&&(!Dirs->findItems(newdir,Qt::MatchExactly).count()))
//		Dirs->addItem(newdir);
}


//-----------------------------------------------------------------------------
void Preferences::slotDelDir(void)
{
	int row=Dirs->currentRow();
	if(row>-1)
		delete Dirs->item(row);
}
