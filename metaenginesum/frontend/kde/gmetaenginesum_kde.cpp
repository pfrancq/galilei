/*

	GALILEI Research Project

	GMetaEngineSum_KDE.cpp

	A KDE dialog box for Text analyser - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#include <gmetaengine.h>
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
#include <qgroupbox.h>
#include <qbuttongroup.h>


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
	I18N_NOOP("This is a meta engine.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "text", I18N_NOOP("Meta Engine"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Valery Vandaele",I18N_NOOP("Contributor"), "vvandaele@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryMetaEngine* params)
{
	DlgConfig_Qt dlg;

	dlg.UseWeight->setChecked(params->GetBool("UseWeight"));
	dlg.LimitRes->setChecked(params->GetBool("LimitRes"));
	dlg.LimitActivatedChanged(static_cast<int>(params->GetBool("LimitRes")));
	dlg.MaxResNb->setValue(params->GetUInt("MaxResNb"));
	dlg.NValue->setValue(params->GetUInt("NValue"));

	unsigned int mode=params->GetUInt("QueryMode");
	dlg.QueryModeOption->setButton(mode);
	dlg.ChangeGeneralMode(mode);

	dlg.CombinaisonOptions->setButton(static_cast<int>(params->GetBool("PercentValue")));
	dlg.ChangeOptionMode(static_cast<int>(params->GetBool("PercentValue")));

	dlg.PLowerPC->setValue(params->GetUInt("PLowerPC"));
	dlg.PUpperPC->setValue(params->GetUInt("PUpperPC"));
	dlg.PLowerAbs->setValue(params->GetUInt("PLowerAbs"));
	dlg.PUpperAbs->setValue(params->GetUInt("PUpperAbs"));

	if(dlg.exec())
	{
		params->SetBool("UseWeight",dlg.UseWeight->isChecked());
		params->SetBool("LimitRes",dlg.LimitRes->isChecked());
		params->SetUInt("MaxResNb",dlg.MaxResNb->value());
		params->SetUInt("NValue",dlg.NValue->value());
		params->SetUInt("QueryMode",dlg.QueryModeOption->selectedId());
		params->SetBool("PercentValue",static_cast<bool>(dlg.CombinaisonOptions->selectedId()));
		params->SetUInt("PLowerPC",dlg.PLowerPC->value());
		params->SetUInt("PUpperPC",dlg.PUpperPC->value());
		params->SetUInt("PLowerAbs",dlg.PLowerAbs->value());
		params->SetUInt("PUpperAbs",dlg.PUpperAbs->value());
		params->Apply();
	}
}

//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
