/*

	GALILEI Research Project

	KViewR.cpp

	Window to excute R - Implementation.

	(C) 2002 by Pascal Francq

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

#include <iostream.h>

//-----------------------------------------------------------------------------
// include files for Qt
#include <qmultilineedit.h>
QMultiLineEdit* Interface;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kprocess.h>
KShellProcess* Proc=0;


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewr.h"



//-----------------------------------------------------------------------------
//
// class KViewR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewR::KViewR(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	Proc=new KShellProcess("/bin/sh");
	(*Proc)<<"R";
	Interface = new QMultiLineEdit(this);
	Interface->resize(size());
	connect(Proc,SIGNAL(receivedStdout(KProcess*,char*,int)),this,SLOT(slotStdout(KProcess*,char*,int)));
 	connect(Proc,SIGNAL(receivedStderr(KProcess*,char*,int)),this,SLOT(slotStdout(KProcess*,char*,int)));
	Proc->start(KProcess::NotifyOnExit,KProcess::All);
//QString cmd = "write.table(4,'/home/jlamoral/tables')";
//if(proc.writeStdin(cmd.data(),cmd.length())) cout<<1<<endl;
//if(proc.isRunning ())cout<<4<<endl;
}


//-----------------------------------------------------------------------------
void KViewR::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewR::slotStdout(KProcess* /*proc*/,char* buffer,int /*buflen*/)
{
	QString s(buffer);
	Interface->append(s);
}


//-----------------------------------------------------------------------------
void KViewR::resizeEvent(QResizeEvent *)
{
	Interface->resize(size());
}


//-----------------------------------------------------------------------------
KViewR::~KViewR(void)
{
	if(Proc)
		delete Proc;
}
