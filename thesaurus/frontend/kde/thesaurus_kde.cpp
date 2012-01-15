/*

	GALILEI Research Project

	Theasurus_KDE.cpp

	Thesaurus Creation (KDE part) - Implementation.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#include <gplugin.h>
#include <rqt.h>
using namespace R;
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
		setCaption("Configure Theasurus Creation");
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
	KAboutData aboutData("thesaurus", 0, ki18n("Thesaurus Creation"),
		"1.0", ki18n("This plug-in computes a thesaurus based on the topic."), KAboutData::License_GPL,
		ki18n("(C) 2008-2012 by Pascal Francq"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


 //------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
 	Config dlg;

 	RString Heuristic(fac->FindParam<RParamValue>("GAHeuristic")->Get());
 	int i;
 	for(i=0;i<dlg.Heuristic->count();i++)
 	{
 		if(Heuristic==FromQString(dlg.Heuristic->itemText(i)))
 			break;
 	}
	dlg.Heuristic->setCurrentIndex(i);
	dlg.MaxGen->setValue(fac->FindParam<RParamValue>("MaxGen")->GetUInt());
	dlg.PopSize->setValue(fac->FindParam<RParamValue>("PopSize")->GetUInt());
	dlg.Verify->setChecked(fac->FindParam<RParamValue>("Verify")->GetBool());
	dlg.NumInfos->setValue(fac->FindParam<RParamValue>("NumInfos")->GetUInt());
	if(dlg.exec())
	{
		fac->FindParam<RParamValue>("Heuristic")->Set(FromQString(dlg.Heuristic->currentText()));
		fac->FindParam<RParamValue>("MaxGen")->SetUInt(dlg.MaxGen->value());
		fac->FindParam<RParamValue>("PopSize")->SetUInt(dlg.PopSize->value());
		fac->FindParam<RParamValue>("Verify")->SetBool(dlg.Verify->isChecked());
		fac->FindParam<RParamValue>("NumInfos")->SetUInt(dlg.NumInfos->value());
		return(true);
	}
	return(false);
}


 //------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
