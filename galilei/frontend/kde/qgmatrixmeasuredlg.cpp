/*

	GALILEI Research Project

	QGMatrixMeasureDlg.cpp

	Configuration For Measures Between Two Elements - Implementation.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
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
void QGMatrixMeasureDlg::Configure(GPlugInFactory* params)
{
	Panel();
	Init(params);
	resize(sizeHint());
	if(exec())
	{
		Done(params);
		params->Apply();
 	}
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Init(GPlugInFactory* params)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	Dlg->CutoffFrequency->setDecimals(10);
	Dlg->CutoffFrequency->setValue(params->GetDouble("Cutoff Frequency"));
	Dlg->MinSim->setValue(params->GetDouble("MinMeasure"));
	Dlg->DeviationRate->setValue(params->GetDouble("DeviationRate"));
	Dlg->DeviationRate->setEnabled(params->GetBool("AutomaticMinMeasure"));
	Dlg->StaticMinSim->setChecked(!params->GetBool("AutomaticMinMeasure"));
	Dlg->MinSim->setEnabled(!params->GetBool("AutomaticMinMeasure"));
	Dlg->Memory->setChecked(params->GetBool("Memory"));
	Dlg->Memory->setEnabled(!GSession::IsConnected());
	Dlg->File->setChecked(params->GetBool("Storage"));
	Dlg->File->setEnabled(!GSession::IsConnected());
	Dlg->Dir->setUrl(ToQString(params->Get("Dir")));
	Dlg->Dir->setEnabled(!GSession::IsConnected());
	Dlg->Dir->setEnabled(Dlg->File->isChecked());
	Dlg->Type->setCurrentIndex(params->GetInt("Type"));
	Dlg->Type->setEnabled(!GSession::IsConnected());
	Dlg->NbNearest->setValue(params->GetUInt("NbNearest"));
	Dlg->NbSamples->setValue(params->GetUInt("NbSamples"));
	ChangeType(params->GetInt("Type"));
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Done(GPlugInFactory* params)
{
	Ui_QGMatrixMeasureDlg* Dlg(static_cast<Ui_QGMatrixMeasureDlg*>(Ui));
	params->SetDouble("Cutoff Frequency",Dlg->CutoffFrequency->value());
	params->SetDouble("MinMeasure",Dlg->MinSim->value());
	params->SetDouble("DeviationRate",Dlg->DeviationRate->value());
	params->SetBool("AutomaticMinMeasure",!Dlg->StaticMinSim->isChecked());
	params->SetUInt("NbNearest",Dlg->NbNearest->value());
	params->SetUInt("NbSamples",Dlg->NbSamples->value());
	params->SetBool("Memory",Dlg->Memory->isChecked());
	params->SetBool("Storage",Dlg->File->isChecked());
	params->Set("Dir",FromQString(Dlg->Dir->url().url()));
	params->SetInt("Type",Dlg->Type->currentIndex());
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
