/*

	GALILEI Research Project

	Sugs_Qt.cpp

	Create suggestions (Qt Part) - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
// include files for R/GALILEI
#include <rqt.h>
#include <qraboutdialog.h>
#include <gplugin.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QDialog>
#include <ui_config.h>


//------------------------------------------------------------------------------
class Config : public QDialog, public Ui_Config
{
public:
	Config(void)
	{
		setupUi(this);
		setWindowTitle("Configure Suggestions");
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		adjustSize();
	}
};


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Suggestions","1.0");
	dlg.setDescription("This plugin computes an orderd list of relevant documents for each profile.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
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
