/*

	GALILEI Research Project

	GMySQL_KDE.cpp

	Storage Manager using a MySQL Database (KDE Part)- Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstorage.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for KDE/Qt
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
		setCaption("Configure MySQl Plug-In");
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
	KAboutData aboutData("mysql", 0, ki18n("MySQL"),
		"1.0",ki18n("This is a storage based on MySQL database."), KAboutData::License_GPL,
		ki18n("(C) 2001-2011 by Pascal Francq\n(C) 2001-2004 by Julien Lamoral, Valery Vandaele and David Wartel\n(C) 2001-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addAuthor(ki18n("Julien Lamoral"),ki18n("Developer"));
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Developer"));
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Developer"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
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
}     // End of extern
//------------------------------------------------------------------------------
