/*

	GALILEI Research Project

	kMeansProfiles_KDE.cpp

	k-Means Plug-in for Profiles (KDE Part) - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <rqt.h>
#include <qraboutdialog.h>
#include <gplugin.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QDialog>
#include <ui_config.h>



//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("k-Means Plug-in for Profiles","1.0");
	dlg.setDescription("This is a set of k-Means algorithms to cluster profiles.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();

}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	// Create the dialog box
	QDialog Dlg;
	Ui_Config Ui;
	Ui.setupUi(&Dlg);
	Dlg.setWindowTitle("Configure k-Means for Profiles");
	QObject::connect(Ui.buttonBox,SIGNAL(accepted()),&Dlg,SLOT(accept()));
	QObject::connect(Ui.buttonBox,SIGNAL(rejected()),&Dlg,SLOT(reject()));

	// Run it
	Ui.NbClusters->setValue(fac->FindParam<RParamValue>("NbClusters")->GetInt());
	Ui.MaxKMeans->setValue(fac->FindParam<RParamValue>("Max kMeans")->GetInt());
	Ui.Convergence->setValue(fac->FindParam<RParamValue>("Convergence")->GetDouble());
	Ui.Alpha->setValue(fac->FindParam<RParamValue>("Alpha")->GetDouble());
	Ui.Incremental->setChecked(fac->FindParam<RParamValue>("Incremental")->GetBool());
	Ui.Spherical->setChecked(fac->FindParam<RParamValue>("Spherical")->GetBool());
	Ui.InternalRandom->setChecked(fac->FindParam<RParamValue>("InternalRandom")->GetBool());
	Ui.Seed->setEnabled(!fac->FindParam<RParamValue>("InternalRandom")->GetBool());
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
