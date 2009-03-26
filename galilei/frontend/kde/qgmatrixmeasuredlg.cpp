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


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <qgmatrixmeasuredlg.h>
#include <ui_qgmatrixmeasuredlg.h>
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
void QGMatrixMeasureDlg::Configure(GFactoryMeasure* params)
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
void QGMatrixMeasureDlg::Init(GFactoryMeasure* params)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NullSimLevel->setDecimals(10);
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NullSimLevel->setValue(params->GetDouble("NullLevel"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MinSim->setValue(params->GetDouble("MinMeasure"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->DeviationRate->setValue(params->GetDouble("DeviationRate"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->DeviationRate->setEnabled(params->GetBool("AutomaticMinMeasure"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->StaticMinSim->setChecked(!params->GetBool("AutomaticMinMeasure"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MinSim->setEnabled(!params->GetBool("AutomaticMinMeasure"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Memory->setChecked(params->GetBool("Memory"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->File->setChecked(params->GetBool("File"));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Dir->setUrl(ToQString(params->Get("Dir")));
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Dir->setEnabled(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->File->isChecked());
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Done(GFactoryMeasure* params)
{
	params->SetDouble("NullLevel",static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->NullSimLevel->value());
	params->SetDouble("MinMeasure",static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MinSim->value());
	params->SetDouble("DeviationRate",static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->DeviationRate->value());
	params->SetBool("AutomaticMinMeasure",!static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->StaticMinSim->isChecked());
	params->SetBool("Memory",static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Memory->isChecked());
	params->SetBool("File",static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->File->isChecked());
	params->Set("Dir",FromQString(static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->Dir->url().url()));
}


//-----------------------------------------------------------------------------
void QGMatrixMeasureDlg::Panel(void)
{
	static_cast<Ui_QGMatrixMeasureDlg*>(Ui)->MeasureSpecific->hide();
}


//-----------------------------------------------------------------------------
QGMatrixMeasureDlg::~QGMatrixMeasureDlg(void)
{
	delete static_cast<Ui_QGMatrixMeasureDlg*>(Ui);
}
