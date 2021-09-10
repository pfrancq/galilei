/*

	GALILEI Research Project

	Gravitation_Topic_KDE.cpp

	Topic Description Computing Method (KDE Part) - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
		setWindowTitle("Configure Gravitation Method for Topics");
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	QRAboutDialog dlg("Gravitation Method for Topics","1.0");
	dlg.setDescription("Topic Description Computing Method.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.LMax->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMax")->GetUInt()));
	dlg.LMin->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMin")->GetUInt()));
	RString Method(fac->FindParam<RParamValue>("Method")->Get());
	if(Method=="Centroid Method")
		dlg.Method->setCurrentIndex(0);
	else if(Method=="Prototype Method")
		dlg.Method->setCurrentIndex(1);
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("LMax")->SetUInt(dlg.LMax->value());
		fac->FindParam<RParamValue>("LMin")->SetUInt(dlg.LMin->value());
		fac->FindParam<RParamValue>("Method")->Set(FromQString(dlg.Method->currentText()));
 		return(true);
 	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
