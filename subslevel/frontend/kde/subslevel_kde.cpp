/*

	GALILEI Research Project

	SubsLevel_KDE.cpp

	A KDE about box for the computation of the profile ranking. - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@e-parkos.com).

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
#include <gpostcommunity.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
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
		setCaption("Configure Level Computation");
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
	KAboutData aboutData( "sugs", 0, ki18n("Suggestions"),
		"1.0", ki18n("This plugin computes the level of each profiles (expert, normal,...)."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Maintainer"), "dwartel@e-parkos.com");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


 //------------------------------------------------------------------------------
void Configure(GFactoryPostCommunity* params)
{
 	Config dlg;

	dlg.NbLevels->setValue(params->GetInt("NbLevels"));

	if (dlg.exec())
	{
		params->SetUInt("NbLevels", dlg.NbLevels->value());
		params->Apply();
	}
}


 //------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
