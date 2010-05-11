/*

	GALILEI Research Project

	Sugs_KDE.cpp

	Create suggestions (KDE Part) - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gcomputesugs.h>
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
		setCaption("Configure Suggestions Plug-In");
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
	KAboutData aboutData( "sugs", 0, ki18n("Suggestions"),
		"1.0", ki18n("This plugin computes an orderd list of relevant documents for each profile."), KAboutData::License_GPL,
		ki18n("(C) 2003-2010 by Pascal Francq\n(C) 2003-2008 by Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config dlg;

	dlg.NbSugs->setValue(fac->FindParam<RParamValue>("NbSugs")->GetUInt());
	dlg.UseLevels->setChecked(fac->FindParam<RParamValue>("UseLevels")->GetBool());
	if (dlg.exec())
	{
		fac->FindParam<RParamValue>("NbSugs")->SetUInt(dlg.NbSugs->value());
		fac->FindParam<RParamValue>("UseLevels")->SetBool(dlg.UseLevels->isChecked());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
