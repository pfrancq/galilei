/*

	GALILEI Research Project

	Feedback_KDE.cpp

	Feedback-based Profile Computing Method (KDE part) - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
		ki18n("(C) 2001-2010 by Pascal Francq\n(C) 2001-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.MaxSize->setValue(static_cast<int>(fac->FindParam<RParamValue>("MaxSize")->GetUInt()));
	dlg.NegSize->setValue(static_cast<int>(fac->FindParam<RParamValue>("NegSize")->GetUInt()));
	dlg.RelFactor->setValue(fac->FindParam<RParamValue>("RelFactor")->GetDouble());
	dlg.FuzzyFactor->setValue(fac->FindParam<RParamValue>("FuzzyFactor")->GetDouble());
	dlg.IrrelFactor->setValue(fac->FindParam<RParamValue>("IrrelFactor")->GetDouble());
	dlg.IncrementalMode->setChecked(fac->FindParam<RParamValue>("IncrementalMode")->GetBool());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("MaxSize")->SetUInt(dlg.MaxSize->value());
		fac->FindParam<RParamValue>("NegSize")->SetUInt(dlg.NegSize->value());
		fac->FindParam<RParamValue>("RelFactor")->SetDouble(dlg.RelFactor->value());
		fac->FindParam<RParamValue>("FuzzyFactor")->SetDouble(dlg.FuzzyFactor->value());
		fac->FindParam<RParamValue>("IrrelFactor")->SetDouble(dlg.IrrelFactor->value());
		fac->FindParam<RParamValue>("IncrementalMode")->SetBool(dlg.IncrementalMode->isChecked());
 		return(true);
 	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
