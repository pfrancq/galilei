/*

	GALILEI Research Project

	GEvalGroups_KDE.cpp

	A KDE about box for the groups evaluation. - Implementation.

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
// include files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstatscalc.h>
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
#include <kurlrequester.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This statistic computes several measures based on the similarities.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "statssims", I18N_NOOP("Similarity Statistics"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Valery Vandaele",I18N_NOOP("Researcher"), "vavdaele@ulb.ac.be");
	aboutData.addCredit("Julien Lamoral",I18N_NOOP("Past researcher"), "jlamoral@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryStatsCalc* params)
{
 	DlgConfig_Qt dlg;

	dlg.Docs->setChecked(params->GetBool("Docs"));
	dlg.ProfDoc->setChecked(params->GetBool("ProfDoc"));
	dlg.GroupDoc->setChecked(params->GetBool("GroupDoc"));
	dlg.Profiles->setChecked(params->GetBool("Profiles"));
	dlg.SameDocProf->setChecked(params->GetBool("SameDocProf"));
	dlg.GroupProf->setChecked(params->GetBool("GroupProf"));
	dlg.File->setChecked(params->GetBool("File"));
	dlg.Name->setURL(ToQString(params->Get("Name")));
	dlg.Name->setEnabled(params->GetBool("File"));
	if(dlg.exec())
	{
		params->SetBool("Docs",dlg.Docs->isChecked());
		params->SetBool("ProfDoc",dlg.ProfDoc->isChecked());
		params->SetBool("GroupDoc",dlg.GroupDoc->isChecked());
		params->SetBool("Profiles",dlg.Profiles->isChecked());
		params->SetBool("SameDocProf",dlg.SameDocProf->isChecked());
		params->SetBool("GroupProf",dlg.GroupProf->isChecked());
		params->SetBool("File",dlg.File->isChecked());
		params->Set("Name",FromQString(dlg.Name->url()));
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
