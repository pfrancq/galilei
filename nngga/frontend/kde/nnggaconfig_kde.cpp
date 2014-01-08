/*

	GALILEI Research Project

	GAConfig_KDE.cpp

	Generic NNGGA Plug-in (KDE Part) - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <nnggaconfig_kde.h>



//------------------------------------------------------------------------------
//
// class GAConfigDlg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GAConfigDlg::GAConfigDlg(void)
{
	setCaption("Configure NNGGA Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
	adjustSize();
}


//------------------------------------------------------------------------------
bool GAConfigDlg::Configure(GPlugIn* fac)
{
	//  Init dialog box
	PopSize->setValue(fac->FindParam<RParamValue>("Population Size")->GetInt());
	MaxGen->setValue(fac->FindParam<RParamValue>("Max Gen")->GetInt());
	Step->setChecked(fac->FindParam<RParamValue>("Step")->GetBool());
	StepGen->setValue(fac->FindParam<RParamValue>("Step Gen")->GetInt());
	StepGen->setEnabled(fac->FindParam<RParamValue>("Step")->GetBool());
	MinAgreement->setValue(fac->FindParam<RParamValue>("Min Agreement")->GetDouble());
	MinDisagreement->setValue(fac->FindParam<RParamValue>("Min Disagreement")->GetDouble());
	Incremental->setChecked(fac->FindParam<RParamValue>("Incremental")->GetBool());
	InternalRandom->setChecked(fac->FindParam<RParamValue>("InternalRandom")->GetBool());
	Seed->setEnabled(!fac->FindParam<RParamValue>("InternalRandom")->GetBool());
	Seed->setValue(fac->FindParam<RParamValue>("Seed")->GetInt());
	SimP->setValue(fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->GetDouble());
	SimQ->setValue(fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->GetDouble());
	SimWeight->setValue(fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->GetDouble());
	AgreementP->setValue(fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->GetDouble());
	AgreementQ->setValue(fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	AgreementWeight->setValue(fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->GetDouble());
	DisagreementP->setValue(fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->GetDouble());
	DisagreementQ->setValue(fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->GetDouble());
	DisagreementWeight->setValue(fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->GetDouble());
	Cout->setChecked(fac->FindParam<RParamValue>("Cout")->GetBool());
	NbMinObjs->setText(QString::number(fac->FindParam<RParamValue>("NbMinObjs")->GetULong()));
	NbMaxObjs->setText(QString::number(fac->FindParam<RParamValue>("NbMaxObjs")->GetULong()));

	// Execute and if 'OK' -> Apply changes
	if(exec())
	{
		fac->FindParam<RParamValue>("Population Size")->SetUInt(PopSize->value());
		fac->FindParam<RParamValue>("Max Gen")->SetUInt(MaxGen->value());
		fac->FindParam<RParamValue>("Step")->SetBool(Step->isChecked());
		fac->FindParam<RParamValue>("Step Gen")->SetUInt(StepGen->value());
		fac->FindParam<RParamValue>("Min Agreement")->SetDouble(MinAgreement->value());
		fac->FindParam<RParamValue>("Min Disagreement")->SetDouble(MinDisagreement->value());
		fac->FindParam<RParamValue>("Incremental")->SetBool(Incremental->isChecked());
		fac->FindParam<RParamValue>("InternalRandom")->SetBool(InternalRandom->isChecked());
		fac->FindParam<RParamValue>("Seed")->SetUInt(Seed->value());
		fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("P")->SetDouble(SimP->value());
		fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Q")->SetDouble(SimQ->value());
		fac->FindParam<RParamStruct>("Sim Criterion")->Get<RParamValue>("Weight")->SetDouble(SimWeight->value());
		fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("P")->SetDouble(AgreementP->value());
		fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Q")->SetDouble(AgreementQ->value());
		fac->FindParam<RParamStruct>("Agreement Criterion")->Get<RParamValue>("Weight")->SetDouble(AgreementWeight->value());
		fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("P")->SetDouble(DisagreementP->value());
		fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Q")->SetDouble(DisagreementQ->value());
		fac->FindParam<RParamStruct>("Disagreement Criterion")->Get<RParamValue>("Weight")->SetDouble(DisagreementWeight->value());
		fac->FindParam<RParamValue>("Cout")->SetBool(Cout->isChecked());
		fac->FindParam<RParamValue>("NbMinObjs")->SetUInt(NbMinObjs->text().toULong());
		fac->FindParam<RParamValue>("NbMaxObjs")->SetUInt(NbMaxObjs->text().toULong());
		return(true);
 	}

	return(false);
}
