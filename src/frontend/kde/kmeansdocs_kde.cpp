/*

	GALILEI Research Project

	kMeansDocs_KDE.cpp

	k-Means Plug-in for Documents (KDE Part) - Implementation.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI Project
#include <rqt.h>
#include <gplugin.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <kdialog.h>
#include <KDE/KLocale>


//-----------------------------------------------------------------------------
// include files for current project
#include <ui_config.h>


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData("kmeans_docs", 0, ki18n("k-Means Plug-in for Documents"),
		"1.0", ki18n("This is a set of k-Means algorithms to cluster documents."),KAboutData::License_GPL,
		ki18n("(C) 2010 by Pascal Francq"),
		KLocalizedString(), "http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	// Create the dialog box
	KDialog Dlg;
	Dlg.setCaption("k-Means Configure");
	Ui_Config Ui;
	QWidget* widget(new QWidget(&Dlg));
	Ui.setupUi(widget);
	Dlg.setMainWidget(widget);
	Dlg.setButtons(KDialog::Cancel|KDialog::Apply);
	QObject::connect(&Dlg,SIGNAL(applyClicked()),&Dlg,SLOT(accept()));

	// Run it
	Ui.NbClusters->setValue(fac->FindParam<RParamValue>("NbClusters")->GetInt());
	Ui.MaxKMeans->setValue(fac->FindParam<RParamValue>("Max kMeans")->GetInt());
	Ui.Convergence->setValue(fac->FindParam<RParamValue>("Convergence")->GetDouble());
	Ui.Alpha->setValue(fac->FindParam<RParamValue>("Alpha")->GetDouble());
	Ui.Incremental->setChecked(fac->FindParam<RParamValue>("Incremental")->GetBool());
	Ui.Spherical->setChecked(fac->FindParam<RParamValue>("Spherical")->GetBool());
	Ui.InternalRandom->setChecked(fac->FindParam<RParamValue>("InternalRandom")->GetBool());
	Ui.Seed->setDisabled(fac->FindParam<RParamValue>("InternalRandom")->GetBool());
	Ui.Seed->setValue(fac->FindParam<RParamValue>("Seed")->GetInt());
	Ui.Cout->setChecked(fac->FindParam<RParamValue>("Cout")->GetBool());
	Ui.MeasureType->setCurrentIndex(Ui.MeasureType->findText(ToQString(fac->FindParam<RParamValue>("MeasureType")->Get())));
	Ui.kMeansType->setCurrentIndex(Ui.kMeansType->findText(ToQString(fac->FindParam<RParamValue>("kMeansType")->Get())));
	if(Dlg.exec())
	{
		fac->FindParam<RParamValue>("NbClusters")->SetUInt(Ui.NbClusters->value());
		fac->FindParam<RParamValue>("Max kMeans")->SetUInt(Ui.MaxKMeans->value());
		fac->FindParam<RParamValue>("Convergence")->SetDouble(Ui.Convergence->value());
		fac->FindParam<RParamValue>("Alpha")->SetDouble(Ui.Alpha->value());
		fac->FindParam<RParamValue>("Incremental")->SetBool(Ui.Incremental->isChecked());
		fac->FindParam<RParamValue>("Spherical")->SetBool(Ui.Spherical->isChecked());
		fac->FindParam<RParamValue>("InternalRandom")->SetBool(Ui.InternalRandom->isChecked());
		fac->FindParam<RParamValue>("Seed")->SetUInt(Ui.Seed->value());
		fac->FindParam<RParamValue>("Cout")->SetBool(Ui.Cout->isChecked());
		fac->FindParam<RParamValue>("MeasureType")->Set(FromQString(Ui.MeasureType->currentText()));
		fac->FindParam<RParamValue>("kMeansType")->Set(FromQString(Ui.kMeansType->currentText()));
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

