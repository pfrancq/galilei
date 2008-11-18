/*

	GALILEI Research Project

	GProfileCalcFeedback_KDE.cpp

	A KDE about box for the feedback method - Implementation.

	Copyright 2003-2004 by the Université libre de Bruxelles.

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
#include <gprofilecalc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
#include <ui_config.h>


//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
public:
	Config(void)
	{
		setCaption("Configure Feedback Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		setMainWidget(widget);
		setButtons(KDialog::Cancel|KDialog::Apply);
		connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "feedback", 0, ki18n("Feedback Subprofile Computing Method "),
		"1.1",ki18n("This is the Feedback Profile Computing Method for GALILEI."), KAboutData::License_GPL,
		ki18n("(c) 1998-2004, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryProfileCalc* params)
{
 	Config dlg;

	dlg.MaxSize->setValue(static_cast<int>(params->GetUInt("MaxSize")));
	dlg.NegSize->setValue(static_cast<int>(params->GetUInt("NegSize")));
	dlg.RelFactor->setValue(params->GetDouble("RelFactor"));
	dlg.FuzzyFactor->setValue(params->GetDouble("FuzzyFactor"));
	dlg.IrrelFactor->setValue(params->GetDouble("IrrelFactor"));
	dlg.IncrementalMode->setChecked(params->GetBool("IncrementalMode"));
	if(dlg.exec())
	{
		params->SetUInt("MaxSize",dlg.MaxSize->value());
		params->SetUInt("NegSize",dlg.NegSize->value());
		params->SetDouble("RelFactor",dlg.RelFactor->value());
		params->SetDouble("FuzzyFactor",dlg.FuzzyFactor->value());
		params->SetDouble("IrrelFactor",dlg.IrrelFactor->value());
		params->SetBool("IncrementalMode",dlg.IncrementalMode->isChecked());
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
