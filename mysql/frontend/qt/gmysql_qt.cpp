/*

	GALILEI Research Project

	GMySQL_Qt.cpp

	Storage Manager using a MySQL Database (Qt Part)- Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rqt.h>
#include <qraboutdialog.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstorage.h>
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
		setWindowTitle("Configure MySQl Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
//extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("MySQL","1.0");
	dlg.setDescription("This is a storage based on MySQL database.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Julien Lamoral"),QWidget::trUtf8("Developer"));
	dlg.addAuthor(QWidget::trUtf8("Valery Vandaele"),QWidget::trUtf8("Developer"));
	dlg.addAuthor(QWidget::trUtf8("David Wartel"),QWidget::trUtf8("Developer"));
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	Config dlg;

	dlg.txtDb->setText(ToQString(fac->FindParam<RParamValue>("Database")->Get()));
	dlg.txtLogin->setText(ToQString(fac->FindParam<RParamValue>("User")->Get()));
	dlg.txtPwd->setText(ToQString(fac->FindParam<RParamValue>("Password")->Get()));
	dlg.txtHost->setText(ToQString(fac->FindParam<RParamValue>("Host")->Get()));
	dlg.cbEncoding->setCurrentIndex(dlg.cbEncoding->findText(ToQString(fac->FindParam<RParamValue>("Encoding")->Get())));
	dlg.Filter->setDate(QDate::fromString(ToQString(fac->FindParam<RParamValue>("Filter")->Get()),Qt::ISODate));
	dlg.Modified->setChecked(!fac->FindParam<RParamValue>("All")->GetBool());
	dlg.Filtering->setChecked(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.Filter->setEnabled(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.groupBox1_2->setEnabled(fac->FindParam<RParamValue>("Filtering")->GetBool());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Database")->Set(FromQString(dlg.txtDb->text()));
		fac->FindParam<RParamValue>("Host")->Set(FromQString(dlg.txtHost->text()));
		fac->FindParam<RParamValue>("User")->Set(FromQString(dlg.txtLogin->text()));
		fac->FindParam<RParamValue>("Password")->Set(FromQString(dlg.txtPwd->text()));
		fac->FindParam<RParamValue>("Encoding")->Set(FromQString(dlg.cbEncoding->currentText()));
		fac->FindParam<RParamValue>("All")->SetBool(!dlg.Modified->isChecked());
		fac->FindParam<RParamValue>("Filter")->Set(FromQString(dlg.Filter->date().toString(Qt::ISODate)));
		fac->FindParam<RParamValue>("Filtering")->SetBool(dlg.Filtering->isChecked());
		return(true);
	}
	return(false);
}

//------------------------------------------------------------------------------
//}     // End of extern
//------------------------------------------------------------------------------
