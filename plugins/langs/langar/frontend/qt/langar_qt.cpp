/*

	GALILEI Research Project

	LangAR_Qt.cpp

	Arabic Language (Qt Part) - Implementation.

	Copyright 2004-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by David Wartel.
	Copyright 2004 by Sarah Rolfo.
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <rqt.h>
#include <qraboutdialog.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Arabic Language","1.0");
	dlg.setDescription("This is the Arabic language plug-in using a Porter-based stemming algorithm.");
	dlg.setCopyright(QWidget::trUtf8("2004-2008 by the Université Libre de Bruxelles<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Sarah Rolfo"),QWidget::trUtf8("Linguist"));
	dlg.addAuthor(QWidget::trUtf8("David Wartel"),QWidget::trUtf8("Developer"));
	dlg.exec();
}
