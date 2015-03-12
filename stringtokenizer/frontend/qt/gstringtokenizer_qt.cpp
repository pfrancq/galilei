/*

	GALILEI Research Project

	GStringTokenizer_Qt.cpp

	Classic String Tokenizer (Qt Part) - Implementation.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
class Config : public QDialog, public Ui_Config
{
public:
	Config(void)
	{
		setWindowTitle("Configure String Indexer Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Classic String Tokenizer","1.0");
	dlg.setDescription("This is a classic string tokenizer for GALILEI.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2011-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();
}


//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	Config dlg;

	// Stems
	dlg.ExtractNonLetter->setChecked(fac->FindParam<RParamValue>("ExtractNonLetter")->GetBool());
	dlg.ExtractURI->setChecked(fac->FindParam<RParamValue>("ExtractURI")->GetBool());
	dlg.Filtering->setChecked(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.MaxConsecutiveOccurs->setValue(fac->FindParam<RParamValue>("MaxConsecutiveOccurs")->GetInt());
	dlg.MaxNonLetter->setValue(fac->FindParam<RParamValue>("MaxNonLetter")->GetInt());
	dlg.NormalRatio->setValue(fac->FindParam<RParamValue>("NormalRatio")->GetDouble());
	dlg.groupFiltering->setEnabled(dlg.ExtractNonLetter->isChecked()&&dlg.Filtering->isChecked());
	if(dlg.exec())
	{
		// Stems
		fac->FindParam<RParamValue>("ExtractNonLetter")->SetBool(dlg.ExtractNonLetter->isChecked());
		fac->FindParam<RParamValue>("ExtractURI")->SetBool(dlg.ExtractURI->isChecked());
		fac->FindParam<RParamValue>("Filtering")->SetBool(dlg.Filtering->isChecked());
		fac->FindParam<RParamValue>("MaxConsecutiveOccurs")->SetUInt(dlg.MaxConsecutiveOccurs->value());
		fac->FindParam<RParamValue>("MaxNonLetter")->SetUInt(dlg.MaxNonLetter->value());
		fac->FindParam<RParamValue>("NormalRatio")->SetDouble(dlg.NormalRatio->value());
		return(true);
	}
	return(false);
}
