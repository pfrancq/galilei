/*

	GALILEI Research Project

	MutualInfo_KDE.cpp

	Mutual Information (KDE Part) - Implementation.

	Copyright 2003-2013 by Pascal Francq (pascal@francq.info).

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
// include files for Qt/KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
#include <ui_config.h>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <gmeasure.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
public:
	Config(void)
	{
		setCaption("Configure Mutual Information Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		setMainWidget(widget);
		setButtons(KDialog::Cancel|KDialog::Apply);
		connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
//
extern "C" {
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "mutualinfo", 0, ki18n("Mutual Information"),
		"1.0", ki18n("Compute different measures evaluating the importance of the features"), KAboutData::License_GPL,
		ki18n("(C) 2003-2013 by Pascal Francq"),
		KLocalizedString(), "http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	GMeasure* mes(dynamic_cast<GMeasure*>(fac));
	if(!mes)
		return(false);

	Config dlg;

	// Fill CurWeights
	size_t MaxWeights(mes->FindParam<RParamValue>("MaxWeights")->GetUInt());
	for(size_t i=0;i<MaxWeights;i++)
	{
		RString Name;
		mes->Info(i,&Name);
		dlg.CurWeights->insertItem(i,ToQString(Name));
	}
 	dlg.CurWeights->setCurrentIndex(mes->FindParam<RParamValue>("CurWeights")->GetUInt());

	if(dlg.exec())
	{
		mes->FindParam<RParamValue>("CurWeights")->SetUInt(dlg.CurWeights->currentIndex());
		return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
