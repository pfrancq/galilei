/*

	GALILEI Research Project

	MSDocL_Qt.cpp

	A MSDoc filter (Qt Part) - Implementation.

	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI
#include <rqt.h>
#include <qraboutdialog.h>
#include <gfilter.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("MSDOC Filter","1.0");
	dlg.setDescription("This is the MSDoc filter for GALILEI. It is highly based on the wvWare project.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Valéry Vandaele"),QWidget::trUtf8("Developper"));
	dlg.addAuthor(QWidget::trUtf8("Dom Lachowicz"),QWidget::trUtf8("mvWare Project"),"cinamod@hotmail.com");
	dlg.addAuthor(QWidget::trUtf8("Shaheed Haque"),QWidget::trUtf8("mvWare Project"),"srhaque@iee.org");
	dlg.addAuthor(QWidget::trUtf8("Werner Trobin"),QWidget::trUtf8("mvWare Project"),"trobin@kde.org");
	dlg.addAuthor(QWidget::trUtf8("David Faure"),QWidget::trUtf8("mvWare Project"),"faure@kde.org");
	dlg.exec();
}
