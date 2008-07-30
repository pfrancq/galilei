/*

	Genetic Algorithm

	GAConfig_KDE.cpp

	Configure Genetic Plug-in - Implementation.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

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
// include files for Qt
#include <qcheckbox.h>
#include <qcombobox.h>


//------------------------------------------------------------------------------
// include files for KDE
#include <knuminput.h>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <gaconfig_kde.h>



//------------------------------------------------------------------------------
//
// class GAConfigDlg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GAConfigDlg::GAConfigDlg(void)
	: DlgConfig_Qt()
{
}


//------------------------------------------------------------------------------
bool GAConfigDlg::Configure(RConfig* params )
{
	//  Init dialog box
	ClusteringMethod->setCurrentText(ToQString(params->Get("Clustering Method")));
	NbClusters->setValue(params->GetUInt("NbClusters"));
	PopSize->setValue(params->GetUInt("Population Size"));
	MaxGen->setValue(params->GetUInt("Max Gen"));
	Step->setChecked(params->GetBool("Step"));
	StepGen->setValue(params->GetUInt("Step Gen"));
	StepGen->setEnabled(params->GetBool("Step"));
	MaxKMeans->setValue(params->GetUInt("Max kMeans"));
	NbDivChromo->setValue(params->GetUInt("NbDivChromos"));
	MinAgreement->setValue(params->GetDouble("Min Agreement"));
	Convergence->setValue(params->GetDouble("Convergence"));
	MinDisagreement->setValue(params->GetDouble("Min Disagreement"));
	Incremental->setChecked(params->GetBool("Incremental"));
	LocalOptimisation->setChecked(params->GetBool("LocalOptimisation"));
	InternalRandom->setChecked(params->GetBool("InternalRandom"));
	Seed->setDisabled(params->GetBool("InternalRandom"));
	Seed->setValue(params->GetInt("Seed"));
	SimP->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->GetDouble());
	SimQ->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->GetDouble());
	SimWeight->setValue(params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->GetDouble());
	AgreementP->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->GetDouble());
	AgreementQ->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	AgreementWeight->setValue(params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->GetDouble());
	DisagreementP->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->GetDouble());
	DisagreementQ->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	DisagreementWeight->setValue(params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->GetDouble());

	// Execute and if 'OK' -> Apply changes
	if(exec())
	{
		params->Set("Clustering Method",FromQString(ClusteringMethod->currentText()));
		params->SetUInt("NbClusters",NbClusters->value());
		params->SetUInt("Population Size",PopSize->value());
		params->SetUInt("Max Gen",MaxGen->value());
		params->SetUInt("Max kMeans",MaxKMeans->value());
		params->SetDouble("Convergence",Convergence->value());
		params->SetUInt("NbDivChromos",NbDivChromo->value());
		params->SetBool("Step",Step->isChecked());
		params->SetUInt("Step Gen",StepGen->value());
		params->SetDouble("Min Agreement",MinAgreement->value());
		params->SetDouble("Min Disagreement",MinDisagreement->value());
		params->SetBool("Incremental",Incremental->isChecked());
		params->SetBool("LocalOptimisation",LocalOptimisation->isChecked());
		params->SetBool("InternalRandom",InternalRandom->isChecked());
		params->SetUInt("Seed",Seed->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->SetDouble(SimP->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->SetDouble(SimQ->value());
		params->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->SetDouble(SimWeight->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->SetDouble(AgreementP->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->SetDouble(AgreementQ->value());
		params->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->SetDouble(AgreementWeight->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->SetDouble(DisagreementP->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->SetDouble(DisagreementQ->value());
		params->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->SetDouble(DisagreementWeight->value());
		return(true);
 	}

	return(false);
}
