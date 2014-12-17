/*

	GALILEI Research Project

	QGMatrixMeasureDlg.cpp

	Configuration For Measures Between Two Elements - Implementation.

	Copyright 2007-2014 by Pascal Francq (pascal@francq.info).
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
	QWidget* widget=new QWidget(this);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->setupUi(widget);
//	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(ApplyDlg()));
	connect(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Type,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeType(int)));
	setWindowTitle(title);
//	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Dir->setMode(KFile::Directory);
	adjustSize();
}

//-----------------------------------------------------------------------------
QWidget* QGMatrixMeasureDlg::GetMeasureSpecific(void)
{
	return(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific);
}


//-----------------------------------------------------------------------------
QVBoxLayout* QGMatrixMeasureDlg::GetMeasureSpecificLayout(void)
{
	//return(dynamic_cast<QBoxLayout*>(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->layout()));
	//return(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->layout());
	return(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecificLayout);
}


//-----------------------------------------------------------------------------
bool QGMatrixMeasureDlg::Configure(GPlugIn* plugin)
{
	Panel();
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecificLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
	Init(plugin);
	resize(sizeHint());
	if(exec())
	{
		Done(plugin);
		return(true);
 	}
	return(false);
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Init(GPlugIn* plugin)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	Dlg->CutoffFrequency->setDecimals(10);
	Dlg->CutoffFrequency->setValue(plugin->FindParam<RParamValue>("Cutoff Frequency")->GetDouble());
	Dlg->MinSim->setValue(plugin->FindParam<RParamValue>("MinMeasure")->GetDouble());
	Dlg->DeviationRate->setValue(plugin->FindParam<RParamValue>("DeviationRate")->GetDouble());
	Dlg->DeviationRate->setEnabled(plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->StaticMinSim->setChecked(!plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->MinSim->setEnabled(!plugin->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->Memory->setChecked(plugin->FindParam<RParamValue>("Memory")->GetBool());
	Dlg->File->setChecked(plugin->FindParam<RParamValue>("Storage")->GetBool());
	Dlg->Dir->setText(ToQString(plugin->FindParam<RParamValue>("Dir")->Get()));
	Dlg->Dir->setEnabled(Dlg->File->isChecked());
	Dlg->Type->setCurrentIndex(plugin->FindParam<RParamValue>("Type")->GetInt());
	Dlg->NbNearest->setValue(plugin->FindParam<RParamValue>("NbNearest")->GetUInt());
	Dlg->NbSamples->setValue(plugin->FindParam<RParamValue>("NbSamples")->GetUInt());
	Dlg->FastNN->setChecked(plugin->FindParam<RParamValue>("FastNN")->GetBool());
	ChangeType(plugin->FindParam<RParamValue>("Type")->GetInt());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Done(GPlugIn* plugin)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	plugin->FindParam<RParamValue>("Cutoff Frequency")->SetDouble(Dlg->CutoffFrequency->value());
	plugin->FindParam<RParamValue>("MinMeasure")->SetDouble(Dlg->MinSim->value());
	plugin->FindParam<RParamValue>("DeviationRate")->SetDouble(Dlg->DeviationRate->value());
	plugin->FindParam<RParamValue>("AutomaticMinMeasure")->SetBool(!Dlg->StaticMinSim->isChecked());
	plugin->FindParam<RParamValue>("NbNearest")->SetUInt(Dlg->NbNearest->value());
	plugin->FindParam<RParamValue>("NbSamples")->SetUInt(Dlg->NbSamples->value());
	plugin->FindParam<RParamValue>("Memory")->SetBool(Dlg->Memory->isChecked());
	plugin->FindParam<RParamValue>("Storage")->SetBool(Dlg->File->isChecked());
	plugin->FindParam<RParamValue>("Dir")->Set(FromQString(Dlg->Dir->text()));
	plugin->FindParam<RParamValue>("Type")->SetInt(Dlg->Type->currentIndex());
	plugin->FindParam<RParamValue>("FastNN")->SetBool(Dlg->FastNN->isChecked());
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
