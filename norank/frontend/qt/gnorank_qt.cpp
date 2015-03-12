/*

	GALILEI Research Project

	GNoRank_Qt.cpp

	No Ranking Method - Implementation (Qt Part).

   Copyright 2003-2014 by Pascal Francq.

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
// include files for current project
#include <gnorank_qt.h>



//------------------------------------------------------------------------------
//
// class Config
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Config::Config(void)
{
	setWindowTitle("Configure No Ranking Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(Type,SIGNAL(currentIndexChanged(int)),this,SLOT(changeType(int)));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::changeType(int index)
{
	switch(index)
	{
		case 0:
			PercentValue->setEnabled(false);
			kMin->setEnabled(false);
			kMax->setEnabled(false);
			break;
		case 1:
			PercentValue->setEnabled(true);
			kMin->setEnabled(true);
			kMax->setEnabled(true);
			break;
	}
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("No Ranking","1.0");
	dlg.setDescription("This plug-in provides a dummy ranking method that does nothing.");
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

//	dlg.UseWeight->setChecked(fac->FindParam<RParamValue>("UseWeight")->GetBool());

	if(dlg.exec())
	{
//		fac->FindParam<RParamValue>("UseWeight")->SetBool(dlg.UseWeight->isChecked());
		return(true);
	}
	return(false);
}
