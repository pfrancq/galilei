/*

	GALILEI Research Project

	GProfileCalcFeedback_KDE.cpp

	A KDE about box for the feedback method - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

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
#include <profiles/gprofilecalc.h>
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
	I18N_NOOP("This is the Feedback Subprofile Computing Method for GALILEI.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "feedback", I18N_NOOP("Feedback Subprofile Computing Method "),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, UniversitÃ© Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryProfileCalc* params)
{
 	DlgConfig_Qt dlg;

	dlg.MaxSize->setValue(params->GetUInt("MaxSize"));
	dlg.isf->setChecked(params->GetBool("isf"));
	dlg.Positive->setChecked(params->GetBool("Positive"));
	dlg.RelFactor->setValue(params->GetDouble("RelFactor"));
	dlg.FuzzyFactor->setValue(params->GetDouble("FuzzyFactor"));
	dlg.IrrelFactor->setValue(params->GetDouble("IrrelFactor"));
	if(dlg.exec())
	{
		params->Set("MaxSize",dlg.MaxSize->value());
		params->Set("isf",dlg.isf->isChecked());
		params->Set("Positive",dlg.Positive->isChecked());
		params->Set("RelFactor",dlg.RelFactor->value());
		params->Set("FuzzyFactor",dlg.FuzzyFactor->value());
		params->Set("IrrelFactor",dlg.IrrelFactor->value());
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------