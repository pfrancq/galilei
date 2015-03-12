/*

	GALILEI Research Project

	Wikipedia_Qt.cpp

	Build a GALILEI database for Wikipedia (Qt Part) - Implementation.

	Copyright 2012-2014 by Pascal Francq (pascal@francq.info).

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



//-----------------------------------------------------------------------------
// include files for current plug-in
#include <wikipedia_qt.h>


//------------------------------------------------------------------------------
Config::Config(void)
{
	setWindowTitle("Configure Wikipedia Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(EditDir,SIGNAL(pressed()),this,SLOT(chooseDir()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::chooseDir(void)
{
	QString fileName(QFileDialog::getExistingDirectory(this,"Choose Directory"));
   if(!fileName.isEmpty())
		Dir->setText(fileName);
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Wikipedia Import","1.0");
	dlg.setDescription("This plug-in import a Wikipedia dump.");
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

	dlg.Dir->setText(ToQString(fac->FindParam<RParamValue>("Dir")->Get()));
	RDate Dump(fac->FindParam<RParamValue>("Dump")->Get());
	dlg.Dump->setDate(QDate(Dump.GetYear(),Dump.GetMonth(),Dump.GetDay()));
	dlg.NbArticles->setValue(fac->FindParam<RParamValue>("NbArticles")->GetInt());
	dlg.NbContributions->setValue(fac->FindParam<RParamValue>("NbContributions")->GetInt());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Dir")->Set(FromQString(dlg.Dir->text()));
		QDate QtDate(dlg.Dump->date());
		Dump.SetDate(QtDate.day(),QtDate.month(),QtDate.year());
		fac->FindParam<RParamValue>("Dump")->Set(Dump);
		fac->FindParam<RParamValue>("NbArticles")->SetUInt(dlg.NbArticles->value());
		fac->FindParam<RParamValue>("NbContributions")->SetUInt(dlg.NbContributions->value());
		return(true);
	}
	return(false);
}
