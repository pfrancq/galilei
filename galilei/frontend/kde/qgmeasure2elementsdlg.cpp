/*

	GALILEI Research Project

	QGMeasure2ElementsDlg.cpp

	Configuration For Measures Between Two Elements - Implementation.

	Copyright 2007 by the Université Libre de Bruxelles.

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
#include <gmeasure.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <qcheckbox.h>
#include <qgroupbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <knuminput.h>
#include <kurlrequester.h>


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <qgmeasure2elementsdlg.h>


//-----------------------------------------------------------------------------
// include the file generated by Qt
#include "dlgmeasure2elements.h"



//-----------------------------------------------------------------------------
//
// QGMeasure2ElementsDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGMeasure2ElementsDlg::QGMeasure2ElementsDlg(const char* str)
	: Dlg(0)
{
	Dlg=new DlgMeasure2Elements();
	Dlg->setCaption(str);
	Dlg->Dir->setMode(KFile::Directory);
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Configure(GFactoryMeasure* params)
{
	Panel();
	Init(params);
	Dlg->resize(Dlg->sizeHint());
	if(Dlg->exec())
	{
		Done(params);
		params->Apply();
 	}
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Init(GFactoryMeasure* params)
{
	Dlg->NullSimLevel->setPrecision(10);
	Dlg->NullSimLevel->setValue(params->GetDouble("NullLevel"));
	Dlg->MinSim->setValue(params->GetDouble("MinMeasure"));
	Dlg->StaticMinSim->setChecked(!params->GetBool("AutomaticMinMeasure"));
	Dlg->MinSim->setEnabled(!params->GetBool("AutomaticMinMeasure"));
	Dlg->Memory->setChecked(params->GetBool("Memory"));
	Dlg->File->setChecked(params->GetBool("File"));
	Dlg->Dir->setURL(ToQString(params->Get("Dir")));
	Dlg->Dir->setEnabled(Dlg->File->isChecked());
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Done(GFactoryMeasure* params)
{
	params->SetDouble("NullLevel",Dlg->NullSimLevel->value());
	params->SetDouble("MinMeasure",Dlg->MinSim->value());	
	params->SetBool("AutomaticMinMeasure",!Dlg->StaticMinSim->isChecked());
	params->SetBool("Memory",Dlg->Memory->isChecked());
	params->SetBool("File",Dlg->File->isChecked());
	params->Set("Dir",FromQString(Dlg->Dir->url()));	
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Panel(void)
{
	Dlg->MeasureSpecific->hide();
}


//-----------------------------------------------------------------------------
QVBoxLayout* QGMeasure2ElementsDlg::GetMeasureSpecificLayout(void)
{
	return(Dlg->GetMeasureSpecificLayout());
}


//-----------------------------------------------------------------------------
QGroupBox* QGMeasure2ElementsDlg::GetMeasureSpecific(void)
{
	return(Dlg->MeasureSpecific);
}


//-----------------------------------------------------------------------------
QGMeasure2ElementsDlg::~QGMeasure2ElementsDlg(void)
{
	delete Dlg;
}
