/*

	GALILEI Research Project

	ExportDocs_KDE.cpp

	Export Documents (KDE Part) - Implementation.

	Copyright 2009-2011 by Pascal Francq.

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
#include <gplugin.h>
using namespace R;
using namespace GALILEI;




//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Wikipedia Dump Parser","1.0");
	dlg.setDescription("This plug-in parses a Wikipedia dump.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2011-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}
