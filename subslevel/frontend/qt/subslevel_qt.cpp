/*

	GALILEI Research Project

	SubsLevel_Qt.cpp

	Compute Profiles and Documents Levels (Qt part) - Implementation.

	Copyright 2005-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by David Wartel.
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
		setWindowTitle("Configure Level Computation");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		adjustSize();
	}
};


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Profiles/Documents Trust Computation","1.0");
	dlg.setDescription("his plug-in computes the level of each profiles (expert, normal, etc.).");
	dlg.setCopyright(QWidget::trUtf8("C) 2003-2008 by Université Libre de Bruxelles<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("David Wartel"),QWidget::trUtf8("Contributor"));
	dlg.exec();
}


 //------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.NbLevels->setValue(fac->FindParam<RParamValue>("NbLevels")->GetInt());

	if (dlg.exec())
	{
		fac->FindParam<RParamValue>("NbLevels")->SetUInt(dlg.NbLevels->value());
		return(true);
	}
	return(false);
}

