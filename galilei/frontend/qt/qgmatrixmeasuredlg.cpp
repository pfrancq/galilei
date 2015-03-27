/*

	GALILEI Research Project

	QGMatrixMeasureDlg.cpp

	Configuration For Measures Between Two Elements - Implementation.

	Copyright 2007-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 Université Libre de Bruxelles (ULB).

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


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <qgmatrixmeasuredlg.h>
#include <ui_qgmatrixmeasuredlg.h>
#include <gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
// QGMatrixMeasureDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGMatrixMeasureDlg::QGMatrixMeasureDlg(const QString& title)
	: Ui(new Ui_QGMatrixMeasureDlg())
{
	Ui->setupUi(this);
	connect(Ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(Ui->buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(Ui->Type,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeType(int)));
	setWindowTitle(title);
}

//-----------------------------------------------------------------------------
QWidget* QGMatrixMeasureDlg::GetMeasureSpecific(void)
{
	return(Ui->MeasureSpecific);
}


//-----------------------------------------------------------------------------
QVBoxLayout* QGMatrixMeasureDlg::GetMeasureSpecificLayout(void)
{
	return(Ui->MeasureSpecificLayout);
}


//-----------------------------------------------------------------------------
bool QGMatrixMeasureDlg::Configure(GPlugIn* plugin)
{
	Panel();
	Ui->MeasureSpecificLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
	Init(plugin);
	adjustSize();
	if(QDialog::exec())
	{
		Done(plugin);
		return(true);
 	}
	return(false);
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Init(GPlugIn* plugin)
{
	Ui->CutoffFrequency->setDecimals(10);
	Ui->CutoffFrequency->setValue(plugin->FindParam<RParamValue>("Cutoff Frequency")->GetDouble());
	Ui->MinSim->setValue(plugin->FindParam<RParamValue>("MinMeasure")->GetDouble());
	Ui->DeviationRate->setValue(plugin->FindParam<RParamValue>("DeviationRate")->GetDouble());
	Ui->DeviationRate->setEnabled(plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Ui->StaticMinSim->setChecked(!plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Ui->MinSim->setEnabled(!plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Ui->Memory->setChecked(plugin->FindParam<RParamValue>("Memory")->GetBool());
	Ui->File->setChecked(plugin->FindParam<RParamValue>("Storage")->GetBool());
	Ui->Dir->setText(ToQString(plugin->FindParam<RParamValue>("Dir")->Get()));
	Ui->Dir->setEnabled(Ui->File->isChecked());
	Ui->Type->setCurrentIndex(plugin->FindParam<RParamValue>("Type")->GetInt());
	Ui->NbNearest->setValue(plugin->FindParam<RParamValue>("NbNearest")->GetUInt());
	Ui->NbSamples->setValue(plugin->FindParam<RParamValue>("NbSamples")->GetUInt());
	Ui->FastNN->setChecked(plugin->FindParam<RParamValue>("FastNN")->GetBool());
	ChangeType(plugin->FindParam<RParamValue>("Type")->GetInt());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Done(GPlugIn* plugin)
{
	plugin->FindParam<RParamValue>("Cutoff Frequency")->SetDouble(Ui->CutoffFrequency->value());
	plugin->FindParam<RParamValue>("MinMeasure")->SetDouble(Ui->MinSim->value());
	plugin->FindParam<RParamValue>("DeviationRate")->SetDouble(Ui->DeviationRate->value());
	plugin->FindParam<RParamValue>("AutomaticMinMeasure")->SetBool(!Ui->StaticMinSim->isChecked());
	plugin->FindParam<RParamValue>("NbNearest")->SetUInt(Ui->NbNearest->value());
	plugin->FindParam<RParamValue>("NbSamples")->SetUInt(Ui->NbSamples->value());
	plugin->FindParam<RParamValue>("Memory")->SetBool(Ui->Memory->isChecked());
	plugin->FindParam<RParamValue>("Storage")->SetBool(Ui->File->isChecked());
	plugin->FindParam<RParamValue>("Dir")->Set(FromQString(Ui->Dir->text()));
	plugin->FindParam<RParamValue>("Type")->SetInt(Ui->Type->currentIndex());
	plugin->FindParam<RParamValue>("FastNN")->SetBool(Ui->FastNN->isChecked());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Panel(void)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->hide();
}


//-----------------------------------------------------------------------------
bool QGMatrixMeasureDlg::IsDlgOK(void)
{
	return(true);
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::ChangeType(int type)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NbNearest->setEnabled(type==2);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NbSamples->setEnabled(type==2);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->FastNN->setEnabled(type==2);
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::ApplyDlg(void)
{
	if(IsDlgOK())
		accept();
}


//-----------------------------------------------------------------------------
QGMatrixMeasureDlg::~QGMatrixMeasureDlg(void)
{
	delete static_cast<Ui_QGMatrixMeasureDlg*>(Ui);
}
