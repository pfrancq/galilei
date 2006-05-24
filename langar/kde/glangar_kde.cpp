/*

	GALILEI Research Project

	GLangAR_KDE.cpp

	A KDE about box for HTML filter - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
		Sarah Rolfo (srolfo@ulb.ac.be).

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
// include files for R Project
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gfilter.h>
#include <../glangar.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <kurlrequester.h>


//------------------------------------------------------------------------------
// include files for QtWidget
#include <qlineedit.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the arabic language plugin using the Porter stemming algorithm.");



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "fr", I18N_NOOP("Arabic Language"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("David Wartel",I18N_NOOP("Contributor"), "dwartel@ulb.ac.be");
	aboutData.addAuthor("Sarah Rolfo",I18N_NOOP("linguist"), "srolfo@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryLang* params)
{
 	DlgConfig_Qt dlg;

	dlg.Rules->setURL(ToQString(params->Get("RulesFile")));
	dlg.Encoding->setText(ToQString(params->Get("Encoding")));

	if(dlg.exec())
	{
		params->Set("RulesFile",FromQString(dlg.Rules->url()));
		params->Set("Encoding", FromQString(dlg.Encoding->text()));
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
