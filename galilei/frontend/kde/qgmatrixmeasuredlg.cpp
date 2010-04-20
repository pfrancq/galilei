/*

	GALILEI Research Project

	QGMatrixMeasureDlg.cpp

	Configuration For Measures Between Two Elements - Implementation.

	Copyright 2007-2010 by Pascal Francq (pascal@francq.info).
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
#include <knuminput.h>
#include <kurlrequester.h>
#include <kmessagebox.h>


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
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
	connect(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Type,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeType(int)));
	setCaption(title);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Dir->setMode(KFile::Directory);
	adjustSize();
}

//-----------------------------------------------------------------------------
QGroupBox* QGMatrixMeasureDlg::GetMeasureSpecific(void)
{
	return(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific);
}


//-----------------------------------------------------------------------------
QBoxLayout* QGMatrixMeasureDlg::GetMeasureSpecificLayout(void)
{
	return(dynamic_cast<QBoxLayout*>(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->layout()));
}


//-----------------------------------------------------------------------------
bool QGMatrixMeasureDlg::Configure(GPlugInFactory* factory)
{
	Panel();
	Init(factory);
	resize(sizeHint());
	if(exec())
	{
		Done(factory);
		return(true);
 	}
	return(false);
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Init(GPlugInFactory* factory)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	Dlg->CutoffFrequency->setDecimals(10);
	Dlg->CutoffFrequency->setValue(factory->FindParam<RParamValue>("Cutoff Frequency")->GetDouble());
	Dlg->MinSim->setValue(factory->FindParam<RParamValue>("MinMeasure")->GetDouble());
	Dlg->DeviationRate->setValue(factory->FindParam<RParamValue>("DeviationRate")->GetDouble());
	Dlg->DeviationRate->setEnabled(factory->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->StaticMinSim->setChecked(!factory->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->MinSim->setEnabled(!factory->FindParam<RParamValue>("AutomaticMinMeasure")->GetBool());
	Dlg->Memory->setChecked(factory->FindParam<RParamValue>("Memory")->GetBool());
	Dlg->Memory->setEnabled(!GSession::IsConnected());
	Dlg->File->setChecked(factory->FindParam<RParamValue>("Storage")->GetBool());
	Dlg->File->setEnabled(!GSession::IsConnected());
	Dlg->Dir->setUrl(ToQString(factory->FindParam<RParamValue>("Dir")->Get()));
	Dlg->Dir->setEnabled(!GSession::IsConnected());
	Dlg->Dir->setEnabled(Dlg->File->isChecked());
	Dlg->Type->setCurrentIndex(factory->FindParam<RParamValue>("Type")->GetInt());
	Dlg->Type->setEnabled(!GSession::IsConnected());
	Dlg->NbNearest->setValue(factory->FindParam<RParamValue>("NbNearest")->GetUInt());
	Dlg->NbSamples->setValue(factory->FindParam<RParamValue>("NbSamples")->GetUInt());
	ChangeType(factory->FindParam<RParamValue>("Type")->GetInt());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Done(GPlugInFactory* factory)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	factory->FindParam<RParamValue>("Cutoff Frequency")->SetDouble(Dlg->CutoffFrequency->value());
	factory->FindParam<RParamValue>("MinMeasure")->SetDouble(Dlg->MinSim->value());
	factory->FindParam<RParamValue>("DeviationRate")->SetDouble(Dlg->DeviationRate->value());
	factory->FindParam<RParamValue>("AutomaticMinMeasure")->SetBool(!Dlg->StaticMinSim->isChecked());
	factory->FindParam<RParamValue>("NbNearest")->SetUInt(Dlg->NbNearest->value());
	factory->FindParam<RParamValue>("NbSamples")->SetUInt(Dlg->NbSamples->value());
	factory->FindParam<RParamValue>("Memory")->SetBool(Dlg->Memory->isChecked());
	factory->FindParam<RParamValue>("Storage")->SetBool(Dlg->File->isChecked());
	factory->FindParam<RParamValue>("Dir")->Set(FromQString(Dlg->Dir->url().url()));
	factory->FindParam<RParamValue>("Type")->SetInt(Dlg->Type->currentIndex());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Panel(void)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->hide();
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::ChangeType(int type)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NbNearest->setEnabled(type==2);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NbSamples->setEnabled(type==2);
}


//-----------------------------------------------------------------------------
QGMatrixMeasureDlg::~QGMatrixMeasureDlg(void)
{
	delete static_cast<Ui_QGMatrixMeasureDlg*>(Ui);
}
