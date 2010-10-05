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
// include files for Qt/KDE
#include <kfiledialog.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>


//------------------------------------------------------------------------------
// includes files for current application
#include <kgalileicenter.h>
#include <preferences.h>
#include <kapplication.h>



//------------------------------------------------------------------------------
//
// class Preferences
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Preferences::Preferences(KGALILEICenter* app)
	: KDialog(app), Ui_Preferences(), App(app)
{
	setCaption("Preferences");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
	connect(buttonAdd,SIGNAL(pressed()),this,SLOT(slotAddDir()));
	connect(buttonDel,SIGNAL(pressed()),this,SLOT(slotDelDir()));
}


//------------------------------------------------------------------------------
void Preferences::exec(void)
{
	IndexDir->setUrl(ToQString(App->GetIndexDir()));
	R::RCursor<R::RString> Cur(App->GetPlugInsPath());
	for(Cur.Start();!Cur.End();Cur.Next())
		Dirs->addItem(ToQString(*Cur()));
	PrgPath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
	PrgPath->setUrl(ToQString(App->PrgPath));

	if(KDialog::exec())
	{
		App->SetIndexDir(FromQString(IndexDir->url().url()));
		App->ClearPlugInsPath();
		for(int i=0;i<Dirs->count();i++)
			App->AddPlugInsPath(R::FromQString(Dirs->item(i)->text()));
		App->PrgPath=R::FromQString(PrgPath->url().url());
	}
}


//-----------------------------------------------------------------------------
void Preferences::slotAddDir(void)
{
	QString newdir=KFileDialog::getExistingDirectory(KUrl::fromPath("~"),this,"Add new directory for plug-ins");
	if((!newdir.isEmpty())&&(!Dirs->findItems(newdir,Qt::MatchExactly).count()))
		Dirs->addItem(newdir);
}


//-----------------------------------------------------------------------------
void Preferences::slotDelDir(void)
{
	int row=Dirs->currentRow();
	if(row>-1)
		delete Dirs->item(row);
}
