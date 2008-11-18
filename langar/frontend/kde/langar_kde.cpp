/*

	GALILEI Research Project

	GLangAR_KDE.cpp

	A KDE about box for HTML filter - Implementation.

	Copyright 2006 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
		Sarah Rolfo (srolfo@ulb.ac.be).

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
// include files for R Project
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
#include <ui_config.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
public:
	Config(void)
	{
		setCaption("Configure E-mail Plug-In");
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
	KAboutData aboutData( "ar", 0, ki18n("Arabic Language"),
		"1.0", ki18n("This is the arabic language plugin using the Porter stemming algorithm."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Contributor"), "dwartel@ulb.ac.be");
	aboutData.addAuthor(ki18n("Sarah Rolfo"),ki18n("Linguist"), "srolfo@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryLang* params)
{
 	Config dlg;

	dlg.Rules->setUrl(ToQString(params->Get("RulesFile")));
	dlg.Encoding->setText(ToQString(params->Get("Encoding")));

	if(dlg.exec())
	{
		params->Set("RulesFile",FromQString(dlg.Rules->url().url()));
		params->Set("Encoding", FromQString(dlg.Encoding->text()));
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
