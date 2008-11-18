/*

	GALILEI Research Project

	GFilterHTML_KDE.cpp

	A KDE about box for HTML filter - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData("msdoc",0, ki18n("MSDoc Filter"),
		"1.0",ki18n("This is the MSDoc filter for GALILEI. It is highly based on the wvWare project."), KAboutData::License_GPL,
		ki18n("(c) 2003, Université Libre de Bruxelles\nCAD/CAM Department\n"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Vandaele Valery"),ki18n("Maintainer"), "vavdaele@ulb.ac.be");
	aboutData.addAuthor(ki18n("Dom Lachowicz"),ki18n("wvWare Project"), "cinamod@hotmail.com");
	aboutData.addAuthor(ki18n("Shaheed Haque"),ki18n("wvWare Project"), "srhaque@iee.org");
	aboutData.addAuthor(ki18n("Werner Trobin"),ki18n("wvWare Project"), "trobin@kde.org");
	aboutData.addAuthor(ki18n("David Faure"),ki18n("wvWare Project"), "faure@kde.org");

	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

