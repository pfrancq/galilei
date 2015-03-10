/*

	GALILEI Research Project

	GEngineXML_KDE.cpp

	XML Search Engine (KDE Part) - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#include <gcomputerank.h>
#include <qraboutdialog.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QDialog>
#include <ui_gpromrank.h>



//------------------------------------------------------------------------------
class Config : public QDialog, public Ui_Config
{
public:
	Config(void)
	{
		setWindowTitle("Configure Promethee Ranking Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	QRAboutDialog dlg("Promethee Ranking","2.0");
	dlg.setDescription("This is a Promethee ranking method.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2004-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Faiza Abbaci"),QWidget::trUtf8("Contributor"));
	dlg.addAuthor(QWidget::trUtf8("Jean-Baptiste Valsamis"),QWidget::trUtf8("Contributor"));
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config Dlg;

	// Tf/Idf
	Dlg.TfIdfActive->setChecked(fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Active")->GetBool());
	Dlg.TfIdfP->setValue(fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("P")->GetDouble());
	Dlg.TfIdfQ->setValue(fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Q")->GetDouble());
	Dlg.TfIdfWeight->setValue(fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Weight")->GetDouble());

	// Distance
	Dlg.DistanceActive->setChecked(fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Active")->GetBool());
	Dlg.DistanceP->setValue(fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("P")->GetDouble());
	Dlg.DistanceQ->setValue(fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Q")->GetDouble());
	Dlg.DistanceWeight->setValue(fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Weight")->GetDouble());

	// Specificity
	Dlg.SpecificityActive->setChecked(fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Active")->GetBool());
	Dlg.SpecificityP->setValue(fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("P")->GetDouble());
	Dlg.SpecificityQ->setValue(fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Q")->GetDouble());
	Dlg.SpecificityWeight->setValue(fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Weight")->GetDouble());

	// Type
	Dlg.TypeActive->setChecked(fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Active")->GetBool());
	Dlg.TypeP->setValue(fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("P")->GetDouble());
	Dlg.TypeQ->setValue(fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Q")->GetDouble());
	Dlg.TypeWeight->setValue(fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Weight")->GetDouble());

	// Tf/Ief
	Dlg.TfIffActive->setChecked(fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Active")->GetBool());
	Dlg.TfIffP->setValue(fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("P")->GetDouble());
	Dlg.TfIffQ->setValue(fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Q")->GetDouble());
	Dlg.TfIffWeight->setValue(fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Weight")->GetDouble());

	if(Dlg.exec())
	{
		// Tf/Idf
		fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Active")->SetBool(Dlg.TfIdfActive->isChecked());
		fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("P")->SetDouble(Dlg.TfIdfP->value());
		fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Q")->SetDouble(Dlg.TfIdfQ->value());
		fac->FindParam<RParamStruct>("TfIdf")->Get<RParamValue>("Weight")->SetDouble(Dlg.TfIdfWeight->value());

		// Distance
		fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Active")->SetBool(Dlg.DistanceActive->isChecked());
		fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("P")->SetDouble(Dlg.DistanceP->value());
		fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Q")->SetDouble(Dlg.DistanceQ->value());
		fac->FindParam<RParamStruct>("Distance")->Get<RParamValue>("Weight")->SetDouble(Dlg.DistanceWeight->value());

		// Specificity
		fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Active")->SetBool(Dlg.SpecificityActive->isChecked());
		fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("P")->SetDouble(Dlg.SpecificityP->value());
		fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Q")->SetDouble(Dlg.SpecificityQ->value());
		fac->FindParam<RParamStruct>("Specificity")->Get<RParamValue>("Weight")->SetDouble(Dlg.SpecificityWeight->value());

		// Type
		fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Active")->SetBool(Dlg.TypeActive->isChecked());
		fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("P")->SetDouble(Dlg.TypeP->value());
		fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Q")->SetDouble(Dlg.TypeQ->value());
		fac->FindParam<RParamStruct>("Type")->Get<RParamValue>("Weight")->SetDouble(Dlg.TypeWeight->value());

		// Tf/Ief
		fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Active")->SetBool(Dlg.TfIffActive->isChecked());
		fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("P")->SetDouble(Dlg.TfIffP->value());
		fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Q")->SetDouble(Dlg.TfIffQ->value());
		fac->FindParam<RParamStruct>("TfIff")->Get<RParamValue>("Weight")->SetDouble(Dlg.TfIffWeight->value());

		return(true);
	}
	return(false);
}


 //------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
