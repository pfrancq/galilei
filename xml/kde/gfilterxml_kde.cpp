/*

	GALILEI Research Project

	GFilterXML_KDE.cpp

	A KDE about box for the XML filter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

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
// include files for R
#include <frontend/kde/rqt.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <kurlrequester.h>

//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the XML filter for GALILEI.");



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "xml", I18N_NOOP("XML Filter"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Valery Vandaele",I18N_NOOP("Maintainer"), "vvandaele@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}

//------------------------------------------------------------------------------
void Configure(GFactoryFilter* params)
{
	DlgConfig_Qt dlg;

	//dlg.xmlDefPath->setURL(ToQt(params->GetString("xmlDefinitionPath")));
	dlg.xmlDefPath->setMode(KFile::Directory);
	dlg.xmlDefPath->setURL(ToQString(params->GetString("xmlDefPath")));
	if(dlg.exec())
	{
		params->Set("xmlDefPath",FromQString(dlg.xmlDefPath->url()));
		params->Apply();
	}
}



//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
