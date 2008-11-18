/*

	GALILEI Research Project

	GEvalGroups_KDE.cpp

	A KDE about box for the groups evaluation. - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gstatscalc.h>
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
	KAboutData aboutData( "statssims", 0, ki18n("Similarity Statistics"),
		"1.0", ki18n("This statistic computes several measures based on the similarities."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Researcher"), "vavdaele@ulb.ac.be");
	aboutData.addCredit(ki18n("Julien Lamoral"),ki18n("Past researcher"), "jlamoral@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryStatsCalc* params)
{
 	Config dlg;

	dlg.Docs->setChecked(params->GetBool("Docs"));
	dlg.ProfDoc->setChecked(params->GetBool("ProfDoc"));
	dlg.GroupDoc->setChecked(params->GetBool("GroupDoc"));
	dlg.Profiles->setChecked(params->GetBool("Profiles"));
	dlg.SameDocProf->setChecked(params->GetBool("SameDocProf"));
	dlg.GroupProf->setChecked(params->GetBool("GroupProf"));
	dlg.File->setChecked(params->GetBool("File"));
	dlg.Name->setUrl(ToQString(params->Get("Name")));
	dlg.Name->setEnabled(params->GetBool("File"));
	if(dlg.exec())
	{
		params->SetBool("Docs",dlg.Docs->isChecked());
		params->SetBool("ProfDoc",dlg.ProfDoc->isChecked());
		params->SetBool("GroupDoc",dlg.GroupDoc->isChecked());
		params->SetBool("Profiles",dlg.Profiles->isChecked());
		params->SetBool("SameDocProf",dlg.SameDocProf->isChecked());
		params->SetBool("GroupProf",dlg.GroupProf->isChecked());
		params->SetBool("File",dlg.File->isChecked());
		params->Set("Name",FromQString(dlg.Name->url().url()));
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
