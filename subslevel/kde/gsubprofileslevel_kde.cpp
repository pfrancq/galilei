/*

	GALILEI Research Project

	GSubProfilesLevel_KDE.cpp

	A KDE about box for the statistical method. - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@e-parkos.com).

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
#include <gpostgroup.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <qvariant.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qcombobox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This plugin computes the level of each subprofiles (expert, normal,...).");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "sugs", I18N_NOOP("Suggestions"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("David Wartel",I18N_NOOP("Maintainer"), "dwartel@e-parkos.com");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


 //------------------------------------------------------------------------------
void Configure(GFactoryPostGroup* params)
{
 	DlgConfig_Qt dlg;

	dlg.NbLevels->setValue(params->GetUInt("NbLevels"));

	if (dlg.exec())
	{
		params->SetUInt("NbLevels", dlg.NbLevels->value());
		params->Apply();
	}
}


 //------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
