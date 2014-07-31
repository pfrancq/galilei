/*

	GALILEI Research Project

	Wikipedia_KDE.cpp

	Build a GALILEI database for Wikipedia (KDE Part) - Implementation.

	Copyright 2012-2014 by Pascal Francq (pascal@francq.info).

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
#include <gtool.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE
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
		setCaption("Configure Wikipedia Plug-In");
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
	KAboutData aboutData( "Wikipedia Import", 0, ki18n("Wikipedia Import"),
		"1.0", ki18n("This plug-in import a Wikipedia dump."), KAboutData::License_GPL,
		ki18n("(C) 2011-2014 by Pascal Francq"),
		KLocalizedString(), "http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config dlg;

	dlg.Dir->setText(ToQString(fac->FindParam<RParamValue>("Dir")->Get()));
	RDate Dump(fac->FindParam<RParamValue>("Dump")->Get());
	dlg.Dump->setDate(QDate(Dump.GetYear(),Dump.GetMonth(),Dump.GetDay()));
	dlg.NbArticles->setValue(fac->FindParam<RParamValue>("NbArticles")->GetInt());
	dlg.NbContributions->setValue(fac->FindParam<RParamValue>("NbContributions")->GetInt());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Dir")->Set(FromQString(dlg.Dir->text()));
		QDate QtDate(dlg.Dump->date());
		Dump.SetDate(QtDate.day(),QtDate.month(),QtDate.year());
		fac->FindParam<RParamValue>("Dump")->Set(Dump);
		fac->FindParam<RParamValue>("NbArticles")->SetUInt(dlg.NbArticles->value());
		fac->FindParam<RParamValue>("NbContributions")->SetUInt(dlg.NbContributions->value());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
