/*

	GALILEI Research Project

	GStopWords_KDE.cpp

	Stopwords Filter - Implementation.

	Copyright 2011 by Pascal Francq (pascal@francq.info).

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
		setCaption("Configure Stopwords Plug-In");
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
	KAboutData aboutData( "txt", 0, ki18n("Stopwords Filter"),
		"1.0", ki18n("This analyzer filters the stopwords of the document."), KAboutData::License_GPL,
		ki18n("(C) 2011 by Paul Otlet Institute"),
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
	dlg.StaticLang->setChecked(fac->FindParam<RParamValue>("StaticLang")->GetBool());
	dlg.MinStopWords->setValue(fac->FindParam<RParamValue>("MinStopWords")->GetDouble());

	if(dlg.exec())
	{
		// Stems
		fac->FindParam<RParamValue>("StaticLang")->SetBool(dlg.StaticLang->isChecked());
		fac->FindParam<RParamValue>("MinStopWords")->SetDouble(dlg.MinStopWords->value());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
