/*

	Genetic Community Algorithm

	GGCAGrouping_KDE.cpp

	Config and about dialogs - Implementation.

	Copyright 2003-2007 by the Université Libre de Bruxelles.

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
#include <ggrouping.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <../../ggca.h>


//-----------------------------------------------------------------------------
// include files for QT
#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
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
	I18N_NOOP("This is the Genetic Community Algorithm for GALILEI.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "GVCA", I18N_NOOP("Genetic Community Algorithm"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2005, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryGrouping* params)
{
	DlgConfig_Qt dlg;
	RPromCriterionParams ret;

	dlg.PopSize->setValue(params->GetUInt("Population Size"));
	dlg.MaxGen->setValue(params->GetUInt("Max Gen"));
	dlg.Step->setChecked(params->GetBool("Step"));
	dlg.StepGen->setValue(params->GetUInt("Step Gen"));
	dlg.StepGen->setEnabled(params->GetBool("Step"));
	dlg.MaxKMeans->setValue(params->GetUInt("Max kMeans"));
	dlg.NbDivChromo->setValue(params->GetUInt("NbDivChromo"));
	dlg.MinAgreement->setValue(params->GetDouble("Min Agreement"));
	dlg.Convergence->setValue(params->GetDouble("Convergence"));
	dlg.MinDisagreement->setValue(params->GetDouble("Min Disagreement"));
	dlg.SimP->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.SimQ->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.SimWeight->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->GetDouble());
	dlg.AgreementP->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.AgreementQ->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.AgreementWeight->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->GetDouble());
	dlg.DisagreementP->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.DisagreementQ->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.DisagreementWeight->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->GetDouble());
	if(dlg.exec())
	{
		params->SetUInt("Population Size",dlg.PopSize->value());
		params->SetUInt("Max Gen",dlg.MaxGen->value());
		params->SetUInt("Max kMeans",dlg.MaxKMeans->value());
		params->SetDouble("Convergence",dlg.Convergence->value());
		params->SetUInt("NbDivChromo",dlg.NbDivChromo->value());
		params->SetBool("Step",dlg.Step->isChecked());
		params->SetUInt("Step Gen",dlg.StepGen->value());
		params->SetDouble("Min Agreement",dlg.MinAgreement->value());
		params->SetDouble("Min Disagreement",dlg.MinDisagreement->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->SetDouble(dlg.SimP->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.SimQ->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.SimWeight->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->SetDouble(dlg.AgreementP->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.AgreementQ->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.AgreementWeight->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->SetDouble(dlg.DisagreementP->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.DisagreementQ->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.DisagreementWeight->value());
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

