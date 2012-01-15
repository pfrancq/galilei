/*

	GALILEI Research Project

	GStringTokenizer_KDE.cpp

	Classic String Tokenizer - Implementation.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#include <gplugin.h>
#include <rqt.h>
using namespace R;
using namespace std;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>


//------------------------------------------------------------------------------
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
		setCaption("Configure String Indexer Plug-In");
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
	KAboutData aboutData( "txt", 0, ki18n("Classic String Tokenizer"),
		"1.0", ki18n("This is a classic string tokenizer for GALILEI."), KAboutData::License_GPL,
		ki18n("(C) 2011-2012 by Paul Otlet Institute"),
		KLocalizedString(), "http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config dlg;

	// Stems
	dlg.ExtractNonLetter->setChecked(fac->FindParam<RParamValue>("ExtractNonLetter")->GetBool());
	dlg.Filtering->setChecked(fac->FindParam<RParamValue>("Filtering")->GetBool());
	dlg.MaxConsecutiveOccurs->setValue(fac->FindParam<RParamValue>("MaxConsecutiveOccurs")->GetInt());
	dlg.MaxNonLetter->setValue(fac->FindParam<RParamValue>("MaxNonLetter")->GetInt());
	dlg.NormalRatio->setValue(fac->FindParam<RParamValue>("NormalRatio")->GetDouble());
	dlg.groupFiltering->setEnabled(dlg.ExtractNonLetter->isChecked()&&dlg.Filtering->isChecked());
	if(dlg.exec())
	{
		// Stems
		fac->FindParam<RParamValue>("ExtractNonLetter")->SetBool(dlg.ExtractNonLetter->isChecked());
		fac->FindParam<RParamValue>("Filtering")->SetBool(dlg.Filtering->isChecked());
		fac->FindParam<RParamValue>("MaxConsecutiveOccurs")->SetUInt(dlg.MaxConsecutiveOccurs->value());
		fac->FindParam<RParamValue>("MaxNonLetter")->SetUInt(dlg.MaxNonLetter->value());
		fac->FindParam<RParamValue>("NormalRatio")->SetDouble(dlg.NormalRatio->value());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
