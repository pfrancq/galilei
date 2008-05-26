/*

	GALILEI Research Project

	GCommunityCalcGravitation_KDE.cpp

	A KDE about box for the statistical method. - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
#include <gcommunitycalc.h>
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
	I18N_NOOP("This is the Gravitation method for GALILEI.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "gravitation", I18N_NOOP("Gravitation Method"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryCommunityCalc* params)
{
 	DlgConfig_Qt dlg;

	dlg.MaxSize->setValue(params->GetUInt("Max Size"));
	if(dlg.exec())
	{
		params->SetUInt("Max Size",dlg.MaxSize->value());
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
