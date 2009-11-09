/*

	GALILEI Research Project

	StatSims_KDE.cpp

	Similarity Statistics (KDE Part) - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
		ki18n("(C) 2003-2009 by Pascal Francq\n(C) 2003-2004 by Valery Vandaele\n(C) 2003-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Julien Lamoral"),ki18n("Past researcher"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GPlugInFactory* params)
{
 	Config dlg;

	dlg.Docs->setChecked(params->GetBool("Docs"));
	dlg.ProfDoc->setChecked(params->GetBool("ProfDoc"));
	dlg.GroupDoc->setChecked(params->GetBool("GroupDoc"));
	dlg.Profiles->setChecked(params->GetBool("Profiles"));
	dlg.SameDocProf->setChecked(params->GetBool("SameDocProf"));
	dlg.GroupProf->setChecked(params->GetBool("GroupProf"));
	dlg.SaveResults->setChecked(params->GetBool("SaveResults"));
	dlg.Results->setUrl(ToQString(params->Get("Results")));
	dlg.Results->setEnabled(dlg.SaveResults->isChecked());
	dlg.ExportDocsSims->setChecked(params->GetBool("ExportDocsSims"));
	dlg.DocsSims->setUrl(ToQString(params->Get("DocsSims")));
	dlg.DocsSims->setEnabled(dlg.ExportDocsSims->isChecked());
	dlg.ExportDocsIncs->setChecked(params->GetBool("ExportDocsIncs"));
	dlg.DocsIncs->setUrl(ToQString(params->Get("DocsIncs")));
	dlg.DocsIncs->setEnabled(dlg.ExportDocsIncs->isChecked());
	if(dlg.exec())
	{
		params->SetBool("Docs",dlg.Docs->isChecked());
		params->SetBool("ProfDoc",dlg.ProfDoc->isChecked());
		params->SetBool("GroupDoc",dlg.GroupDoc->isChecked());
		params->SetBool("Profiles",dlg.Profiles->isChecked());
		params->SetBool("SameDocProf",dlg.SameDocProf->isChecked());
		params->SetBool("GroupProf",dlg.GroupProf->isChecked());
		params->SetBool("SaveResults",dlg.SaveResults->isChecked());
		params->Set("Results",FromQString(dlg.Results->url().url()));
		params->SetBool("ExportDocsSims",dlg.ExportDocsSims->isChecked());
		params->Set("DocsSims",FromQString(dlg.DocsSims->url().url()));
		params->SetBool("ExportDocsIncs",dlg.ExportDocsIncs->isChecked());
		params->Set("DocsIncs",FromQString(dlg.DocsIncs->url().url()));
 		params->Apply();
 	}
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
