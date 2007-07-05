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


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <qgmeasure2elementsdlg.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// QGMeasure2ElementsDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGMeasure2ElementsDlg::QGMeasure2ElementsDlg(const char* str)
	: DlgMeasure2Elements()
{
	setCaption(str);
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Configure(GFactoryMeasure* params)
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
void QGMeasure2ElementsDlg::Init(GFactoryMeasure* params)
{
	NullSimLevel->setPrecision(10);
	NullSimLevel->setValue(params->GetDouble("NullLevel"));
	MinSim->setValue(params->GetDouble("MinMeasure"));
	StaticMinSim->setChecked(!params->GetBool("AutomaticMinMeasure"));
	MinSim->setEnabled(!params->GetBool("AutomaticMinMeasure"));
	Memory->setChecked(params->GetBool("Memory"));
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Done(GFactoryMeasure* params)
{
	params->SetDouble("NullLevel",NullSimLevel->value());
	params->SetDouble("MinMeasure",MinSim->value());
	params->SetBool("Memory",Memory->isChecked());
	params->SetBool("AutomaticMinMeasure",!StaticMinSim->isChecked());
}


//-----------------------------------------------------------------------------
void QGMeasure2ElementsDlg::Panel(void)
{
	MeasureSpecific->hide();
}
