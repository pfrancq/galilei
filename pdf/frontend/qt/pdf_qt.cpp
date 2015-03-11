/*

	GALILEI Research Project

	PDF_Qt.cpp

	A PDF filter (Qt Part) - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).
	Copyright 1996-2005 by Glyph & Cog, LLC.

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
	QRAboutDialog dlg("PDF Filter","1.0");
	dlg.setDescription("This is the PDF filter for GALILEI.  It is highly based on the Xpdf project 3.01.");
	dlg.setCopyright(QWidget::trUtf8("(C) 1996-2005 by Glyph & Cog, LLC<br/>(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Glyph & Cog, LLC"),QWidget::trUtf8("Xpdf Project"), "derekn@foolabs.com");
	dlg.exec();
}
