/*

	GALILEI Research Project

	GMetaEngineSum_Qt.cpp

	Meta-engine based on a Weighted Sum - Implementation (Qt Part).

   Copyright 2003-2015 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#include <gmetaenginesum_qt.h>
#include <../../gmetaenginesum.h>



//------------------------------------------------------------------------------
//
// class Config
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Config::Config(void)
{
	setWindowTitle("Configure MetaEngine Plug-In");
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
	QRAboutDialog dlg("Meta Engine Sum","1.0");
	dlg.setDescription("This is a simple meta engine that sums the scores of each engine's results.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Valéry Vandaele"),QWidget::trUtf8("Contributor"));
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	Config dlg;

	dlg.UseWeight->setChecked(fac->FindParam<RParamValue>("UseWeight")->GetBool());
	switch(fac->FindParam<RParamValue>("Type")->GetUInt())
	{
		case GMetaEngineSum::Single:
			dlg.Type->setCurrentIndex(0);
			dlg.changeType(0);
			break;
		case GMetaEngineSum::kCombinations:
			dlg.Type->setCurrentIndex(1);
			dlg.changeType(1);
			break;
	}
	dlg.PercentValue->setChecked(fac->FindParam<RParamValue>("PercentValue")->GetBool());
	dlg.kMin->setValue(fac->FindParam<RParamValue>("kMin")->GetUInt());
	dlg.kMax->setValue(fac->FindParam<RParamValue>("kMax")->GetUInt());

	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("UseWeight")->SetBool(dlg.UseWeight->isChecked());
		switch(dlg.Type->currentIndex())
		{
			case 0:
				fac->FindParam<RParamValue>("Type")->SetUInt(GMetaEngineSum::Single);
				break;
			case 1:
				fac->FindParam<RParamValue>("Type")->SetUInt(GMetaEngineSum::kCombinations);
				break;
		}
		fac->FindParam<RParamValue>("PercentValue")->SetBool(dlg.PercentValue->isChecked());
		fac->FindParam<RParamValue>("kMin")->SetUInt(dlg.kMin->value());
		fac->FindParam<RParamValue>("kMax")->SetUInt(dlg.kMax->value());
		return(true);
	}
	return(false);
}
