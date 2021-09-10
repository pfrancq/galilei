/*

	GALILEI Research Project

	StatFeatures_Qt.cpp

	Features Statistics (Qt Part) - Implementation.

	Copyright 2010-2014 by Pascal Francq (pascal@francq.info).

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
#include <statfeatures_qt.h>



//------------------------------------------------------------------------------
Config::Config(void)
{
	setupUi(this);
	setWindowTitle("Configure Features Statistics");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(Edit,SIGNAL(pressed()),this,SLOT(chooseFile()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::chooseFile(void)
{
	QString fileName(QFileDialog::getSaveFileName(this,"Choose Output File"));
   if(!fileName.isEmpty())
		Results->setText(fileName);
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Features Statistics","1.0");
	dlg.setDescription("This statistic computes several measures based on the features extracted from the documents.");
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

	dlg.Results->setText(ToQString(fac->FindParam<RParamValue>("Results")->Get()));
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Results")->Set(FromQString(dlg.Results->text()));
		return(true);
 	}
	return(false);
}
