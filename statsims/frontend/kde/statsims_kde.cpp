/*

	GALILEI Research Project

	StatSims_KDE.cpp

	Similarity Statistics (KDE Part) - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
#include <gplugin.h>
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
		setCaption("Configure Statistics Plug-In");
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
		ki18n("(C) 2003-2014 by Pascal Francq\n(C) 2003-2004 by Valery Vandaele\n(C) 2003-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Julien Lamoral"),ki18n("Past researcher"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.Docs->setChecked(fac->FindParam<RParamValue>("Docs")->GetBool());
	dlg.ProfDoc->setChecked(fac->FindParam<RParamValue>("ProfDoc")->GetBool());
	dlg.GroupDoc->setChecked(fac->FindParam<RParamValue>("GroupDoc")->GetBool());
	dlg.Profiles->setChecked(fac->FindParam<RParamValue>("Profiles")->GetBool());
	dlg.SameDocProf->setChecked(fac->FindParam<RParamValue>("SameDocProf")->GetBool());
	dlg.GroupProf->setChecked(fac->FindParam<RParamValue>("GroupProf")->GetBool());
	dlg.Results->setUrl(ToQString(fac->FindParam<RParamValue>("Results")->Get()));
	dlg.ExportDocsSims->setChecked(fac->FindParam<RParamValue>("ExportDocsSims")->GetBool());
	dlg.DocsSims->setUrl(ToQString(fac->FindParam<RParamValue>("DocsSims")->Get()));
	dlg.DocsSims->setEnabled(dlg.ExportDocsSims->isChecked());
	dlg.ExportDocsIncs->setChecked(fac->FindParam<RParamValue>("ExportDocsIncs")->GetBool());
	dlg.DocsIncs->setUrl(ToQString(fac->FindParam<RParamValue>("DocsIncs")->Get()));
	dlg.DocsIncs->setEnabled(dlg.ExportDocsIncs->isChecked());
	dlg.MeasureType->setCurrentIndex(dlg.MeasureType->findText(ToQString(fac->FindParam<RParamValue>("MeasureType")->Get())));
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Docs")->SetBool(dlg.Docs->isChecked());
		fac->FindParam<RParamValue>("ProfDoc")->SetBool(dlg.ProfDoc->isChecked());
		fac->FindParam<RParamValue>("GroupDoc")->SetBool(dlg.GroupDoc->isChecked());
		fac->FindParam<RParamValue>("Profiles")->SetBool(dlg.Profiles->isChecked());
		fac->FindParam<RParamValue>("SameDocProf")->SetBool(dlg.SameDocProf->isChecked());
		fac->FindParam<RParamValue>("GroupProf")->SetBool(dlg.GroupProf->isChecked());
		fac->FindParam<RParamValue>("Results")->Set(FromQString(dlg.Results->url().url()));
		fac->FindParam<RParamValue>("ExportDocsSims")->SetBool(dlg.ExportDocsSims->isChecked());
		fac->FindParam<RParamValue>("DocsSims")->Set(FromQString(dlg.DocsSims->url().url()));
		fac->FindParam<RParamValue>("ExportDocsIncs")->SetBool(dlg.ExportDocsIncs->isChecked());
		fac->FindParam<RParamValue>("DocsIncs")->Set(FromQString(dlg.DocsIncs->url().url()));
		fac->FindParam<RParamValue>("MeasureType")->Set(FromQString(dlg.MeasureType->currentText()));
		return(true);
 	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
