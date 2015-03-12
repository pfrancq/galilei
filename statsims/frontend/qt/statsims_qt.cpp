/*

	GALILEI Research Project

	StatSims_Qt.cpp

	Similarity Statistics (Qt Part) - Implementation.

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



//-----------------------------------------------------------------------------
// include files for current plug-in
#include <statsims_qt.h>


//------------------------------------------------------------------------------
Config::Config(void)
{
	setWindowTitle("Configure Statistics Plug-In");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(EditResults,SIGNAL(pressed()),this,SLOT(chooseResults()));
	connect(EditDocsSims,SIGNAL(pressed()),this,SLOT(chooseDocsSims()));
	connect(EditDocsIncs,SIGNAL(pressed()),this,SLOT(chooseDocsIncs()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Config::chooseResults(void)
{
	QString fileName(QFileDialog::getSaveFileName(this,"Choose Output File"));
   if(!fileName.isEmpty())
		Results->setText(fileName);
}


//------------------------------------------------------------------------------
void Config::chooseDocsSims(void)
{
	QString fileName(QFileDialog::getSaveFileName(this,"Choose Document Similarities"));
   if(!fileName.isEmpty())
		DocsSims->setText(fileName);
}


//------------------------------------------------------------------------------
void Config::chooseDocsIncs(void)
{
	QString fileName(QFileDialog::getSaveFileName(this,"Choose Document Inclusions"));
   if(!fileName.isEmpty())
		DocsIncs->setText(fileName);
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Similarity Statistics","1.0");
	dlg.setDescription("This statistic computes several measures based on the similarities.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2003-2008 by the Université Libre de Bruxelles<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Julien Lamoral"),QWidget::trUtf8("Contributor"));
	dlg.addAuthor(QWidget::trUtf8("Valéry Vandaele"),QWidget::trUtf8("Contributor"));
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.Docs->setChecked(fac->FindParam<RParamValue>("Docs")->GetBool());
	dlg.ProfDoc->setChecked(fac->FindParam<RParamValue>("ProfDoc")->GetBool());
	dlg.GroupDoc->setChecked(fac->FindParam<RParamValue>("GroupDoc")->GetBool());
	dlg.Profiles->setChecked(fac->FindParam<RParamValue>("Profiles")->GetBool());
	dlg.SameDocProf->setChecked(fac->FindParam<RParamValue>("SameDocProf")->GetBool());
	dlg.GroupProf->setChecked(fac->FindParam<RParamValue>("GroupProf")->GetBool());
	dlg.Results->setText(ToQString(fac->FindParam<RParamValue>("Results")->Get()));
	dlg.ExportDocsSims->setChecked(fac->FindParam<RParamValue>("ExportDocsSims")->GetBool());
	dlg.DocsSims->setText(ToQString(fac->FindParam<RParamValue>("DocsSims")->Get()));
	dlg.DocsSims->setEnabled(dlg.ExportDocsSims->isChecked());
	dlg.ExportDocsIncs->setChecked(fac->FindParam<RParamValue>("ExportDocsIncs")->GetBool());
	dlg.DocsIncs->setText(ToQString(fac->FindParam<RParamValue>("DocsIncs")->Get()));
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
		fac->FindParam<RParamValue>("Results")->Set(FromQString(dlg.Results->text()));
		fac->FindParam<RParamValue>("ExportDocsSims")->SetBool(dlg.ExportDocsSims->isChecked());
		fac->FindParam<RParamValue>("DocsSims")->Set(FromQString(dlg.DocsSims->text()));
		fac->FindParam<RParamValue>("ExportDocsIncs")->SetBool(dlg.ExportDocsIncs->isChecked());
		fac->FindParam<RParamValue>("DocsIncs")->Set(FromQString(dlg.DocsIncs->text()));
		fac->FindParam<RParamValue>("MeasureType")->Set(FromQString(dlg.MeasureType->currentText()));
		return(true);
 	}
	return(false);
}
