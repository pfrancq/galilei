/*

	GALILEI Research Project

	KViewR.cpp

	Window to excute R - Implementation.

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
// include files for C++
#include <iostream>

//-----------------------------------------------------------------------------
// include files for Qt
#include <qmultilineedit.h>
//QMultiLineEdit* Interface;
#include <qframe.h>

#include <qlayout.h>
#include <kparts/part.h>
#include <klibloader.h>

//-----------------------------------------------------------------------------
// include files for KDE
//#include <kprocess.h>
//KShellProcess* Proc=0;


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewr.h"


class CKonsoleWidgetPrivate
{
public:

  CKonsoleWidgetPrivate()
    : part(0)
  {
  }

  void setDirectory(QString dirname)
  {
    if (url == dirname)
      return;
    url = dirname;
    if (part) {
      KURL url(dirname);
      part->openURL(url);
    }
  }

  void activate(QWidget *parent)
  {
    if (part)
      return;

    KLibFactory *factory = KLibLoader::self()->factory("libkonsolepart");
    if (!factory)
      return;

    part = (KParts::ReadOnlyPart *) factory->create(parent);
    if (!part)
      return;
    part->widget()->setFocusPolicy(QWidget::WheelFocus);
    parent->setFocusProxy(part->widget());
    part->widget()->setFocus();

    if (part->widget()->inherits("QFrame"))
      ((QFrame*)part->widget())->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    QVBoxLayout *vbox = new QVBoxLayout(parent);
    vbox->addWidget(part->widget());

    if (part && !url.isEmpty())
      setDirectory(url);

    part->widget()->show();
  }


private:

  QString url;
  KParts::ReadOnlyPart *part;

};

//-----------------------------------------------------------------------------
//
// class KViewR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewR::KViewR(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	d = new CKonsoleWidgetPrivate();
    d->setDirectory("R");
//	Proc=new KShellProcess("/bin/sh");
//	(*Proc)<<"R";
//	Interface = new QMultiLineEdit(this);
//	Interface->resize(size());
//	connect(Proc,SIGNAL(receivedStdout(KProcess*,char*,int)),this,SLOT(slotStdout(KProcess*,char*,int)));
// 	connect(Proc,SIGNAL(receivedStderr(KProcess*,char*,int)),this,SLOT(slotStdout(KProcess*,char*,int)));
//	Proc->start(KProcess::NotifyOnExit,KProcess::All);
//QString cmd = "write.table(4,'/home/jlamoral/tables')";
//if(proc.writeStdin(cmd.data(),cmd.length())) cout<<1<<endl;
//if(proc.isRunning ())cout<<4<<endl;
}


//-----------------------------------------------------------------------------
void KViewR::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
//void KViewR::slotStdout(KProcess* /*proc*/,char* buffer,int /*buflen*/)
//{
//	QString s(buffer);
//	Interface->append(s);
//}


//-----------------------------------------------------------------------------
void KViewR::resizeEvent(QResizeEvent *)
{
//	Interface->resize(size());
}

void KViewR::setDirectory(QString dirname)
{
  d->setDirectory(dirname);
}


void KViewR::showEvent(QShowEvent *ev)
{
  QWidget::showEvent(ev);
  d->activate(this);
}


//-----------------------------------------------------------------------------
KViewR::~KViewR(void)
{
//	if(Proc)
//		delete Proc;
  delete d;
}
