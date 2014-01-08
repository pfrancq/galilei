/*

	GALILEI Research Project

	GMetaEngineSum_KDE.cpp

	Meta-engine based on a Weighted Sum - Implementation (KDE Part).

   Copyright 2003-2014 by Pascal Francq.
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
#include <gmetaenginesum_kde.h>



//------------------------------------------------------------------------------
//
// class Config
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Config::Config(void)
{
	setCaption("Configure MetaEngine Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(Type,SIGNAL(currentIndexChanged(int)),this,SLOT(changeType(int)));
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
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
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData("metaengine", 0, ki18n("Meta Engine"),
		"1.0", ki18n("This is a meta engine."), KAboutData::License_GPL,
		ki18n("(C) 2003-2014 by Pascal Francq\n(C)2003-2004 by Valery Vandaele\n(C) 2003-2008 by Université Libre de Bruxelles\n"),
		KLocalizedString(), "http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Contributor") );
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
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


//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
