/*

	GALILEI Research Project

	Gravitation_Topic_KDE.cpp

	Topic Description Computing Method (KDE Part) - Implementation.

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
// include files for GALILEI
#include <gtopiccalc.h>
#include <rqt.h>
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
	KAboutData aboutData( "gravitation_topics", 0, ki18n("Gravitation Method for Topics"),
		"1.0", ki18n("Topic Description Computing Method"), KAboutData::License_GPL,
		ki18n("(C) 2008-2014 by Pascal Francq"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

	dlg.LMax->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMax")->GetUInt()));
	dlg.LMin->setValue(static_cast<int>(fac->FindParam<RParamValue>("LMin")->GetUInt()));
	RString Method(fac->FindParam<RParamValue>("Method")->Get());
	if(Method=="Centroid Method")
		dlg.Method->setCurrentIndex(0);
	else if(Method=="Prototype Method")
		dlg.Method->setCurrentIndex(1);
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("LMax")->SetUInt(dlg.LMax->value());
		fac->FindParam<RParamValue>("LMin")->SetUInt(dlg.LMin->value());
		fac->FindParam<RParamValue>("Method")->Set(FromQString(dlg.Method->currentText()));
 		return(true);
 	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
