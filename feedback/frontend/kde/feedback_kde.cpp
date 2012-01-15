/*

	GALILEI Research Project

	Feedback_KDE.cpp

	Feedback-based Profile Computing Method (KDE part) - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
using namespace R;


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
	KAboutData aboutData( "feedback", 0, ki18n("Feedback Profile Computing Method "),
		"1.1",ki18n("This is the Feedback Profile Computing Method for GALILEI."), KAboutData::License_GPL,
		ki18n("(C) 2001-2012 by Pascal Francq\n(C) 2001-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.LMax->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMax")->GetUInt()));
	dlg.LMin->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMin")->GetUInt()));
	dlg.RelFactor->setValue(fac->FindParam<RParamValue>("Relevant Factor")->GetDouble());
	dlg.FuzzyFactor->setValue(fac->FindParam<RParamValue>("Fuzzy Factor")->GetDouble());
	dlg.IrrelFactor->setValue(fac->FindParam<RParamValue>("Irrelevant Factor")->GetDouble());
	dlg.IncrementalMode->setChecked(fac->FindParam<RParamValue>("Incremental")->GetBool());
	dlg.Alpha->setValue(fac->FindParam<RParamValue>("Alpha")->GetDouble());
	dlg.Alpha->setEnabled(dlg.IncrementalMode->isChecked());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("LMax")->SetUInt(dlg.LMax->value());
		fac->FindParam<RParamValue>("LMin")->SetUInt(dlg.LMin->value());
		fac->FindParam<RParamValue>("Relevant Factor")->SetDouble(dlg.RelFactor->value());
		fac->FindParam<RParamValue>("Fuzzy Factor")->SetDouble(dlg.FuzzyFactor->value());
		fac->FindParam<RParamValue>("Irrelevant Factor")->SetDouble(dlg.IrrelFactor->value());
		fac->FindParam<RParamValue>("Incremental")->SetBool(dlg.IncrementalMode->isChecked());
		fac->FindParam<RParamValue>("Alpha")->SetDouble(dlg.Alpha->value());
 		return(true);
 	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
