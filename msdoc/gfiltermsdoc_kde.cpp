/*

	GALILEI Research Project

	GFilterHTML_KDE.cpp

	A KDE about box for HTML filter - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the MSDoc filter for GALILEI.");



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "msdoc", I18N_NOOP("MSDoc Filter"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department\n"
		" It is highly based on the wvWare project.", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Vandaele Valery",I18N_NOOP("Maintainer"), "vavdaele@ulb.ac.be");
	aboutData.addAuthor("Dom Lachowicz",I18N_NOOP("wvWare Project"), "cinamod@hotmail.com");
	aboutData.addAuthor("Shaheed Haque",I18N_NOOP("wvWare Project"), "srhaque@iee.org");
	aboutData.addAuthor("Werner Trobin",I18N_NOOP("wvWare Project"), "trobin@kde.org");
	aboutData.addAuthor("David Faure",I18N_NOOP("wvWare Project"), "faure@kde.org");

	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

