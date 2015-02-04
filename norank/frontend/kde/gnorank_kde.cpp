/*

	GALILEI Research Project

	GNoRank_KDE.cpp

	No Ranking Method - Implementation (KDE Part).

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
#include <gnorank_kde.h>



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

//	dlg.UseWeight->setChecked(fac->FindParam<RParamValue>("UseWeight")->GetBool());

	if(dlg.exec())
	{
//		fac->FindParam<RParamValue>("UseWeight")->SetBool(dlg.UseWeight->isChecked());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
