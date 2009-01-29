/*

	GALILEI Research Project

	GMetaEngineSum_KDE.cpp

	A KDE dialog box for Text analyser - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#include <gmetaengine.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <QtGui/QPushButton>
/*#include <qvariant.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>*/


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplicationdialog.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <ui_config.h>


//------------------------------------------------------------------------------
class Config : public KDialog, public Ui_Config
{
public:
	Config(void)
	{
		setCaption("Configure MetaEngine Plug-In");
		QWidget* widget=new QWidget(this);
		setupUi(widget);
		setMainWidget(widget);
		setButtons(KDialog::Cancel|KDialog::Apply);
		connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
		adjustSize();
	}
	void ChangeGeneralMode(int m)
	{
	    //Use single query
	    if(m==1)
	    {
		CombinaisonOptions->setEnabled(false);
	    }
	    //Use combinaison of query
	    if(m==2)
	    {
		CombinaisonOptions->setEnabled(true);
		PUpperPCLabel->setEnabled(false);
		PUpperPC->setEnabled(false);
		PUpperAbsLabel->setEnabled(false);
		PUpperAbs->setEnabled(false);
	    }
	    //Use Ranch combinaison of query
	    if(m==0)
	    {
		CombinaisonOptions->setEnabled(true);
/*		if(CombinaisonOptions->selectedId()==1)
		{
			PUpperPCLabel->setEnabled(true);
			PUpperPC->setEnabled(true);
		}
		else
		{
			PUpperAbsLabel->setEnabled(true);
			PUpperAbs->setEnabled(true);
		}*/
	    }
	}

	void ChangeOptionMode(int m)
	{
	    if(m==1)
	    {
		//set percentage values enabled
		PLowerPCLabel->setEnabled(true);
		PLowerPC->setEnabled(true);
/*		if(QueryModeOption->selectedId()==0)
		{
		    PUpperPCLabel->setEnabled(true);
		    PUpperPC->setEnabled(true);
		}
		else
		{
		    PUpperPCLabel->setEnabled(false);
		    PUpperPC->setEnabled(false);
		}*/
		//set absolute value enabled
		PLowerAbsLabel->setEnabled(false);
		PLowerAbs->setEnabled(false);
		PUpperAbsLabel->setEnabled(false);
		PUpperAbs->setEnabled(false);
	    }
	    if(m==0)
	    {
		//set percentage values enabled
		PLowerPCLabel->setEnabled(false);
		PLowerPC->setEnabled(false);
		PUpperPCLabel->setEnabled(false);
		PUpperPC->setEnabled(false);

		//set absolute value enabled
		PLowerAbsLabel->setEnabled(true);
		PLowerAbs->setEnabled(true);
/*		if(QueryModeOption->selectedId()==0)
		{
		    PUpperAbsLabel->setEnabled(true);
		    PUpperAbs->setEnabled(true);
		}
		else
		{
		    PUpperAbsLabel->setEnabled(false);
		    PUpperAbs->setEnabled(false);
		}*/

	    }
	}


	void LimitActivatedChanged( int s )
	{
	    if(s==1/*QButton::On*/)
	    {
		MaxResNb->setEnabled(true);
	    }
	    if(s==0/*QButton::Off*/)
	    {
		MaxResNb->setEnabled(false);
	    }
	 //   if(s==Qbutton:NoChange)
	}


};


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "text", 0, ki18n("Meta Engine"),
		"1.0", ki18n("This is a meta engine."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("Contributor"), "vvandaele@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryMetaEngine* params)
{
	Config dlg;

	dlg.UseWeight->setChecked(params->GetBool("UseWeight"));
	dlg.LimitRes->setChecked(params->GetBool("LimitRes"));
	dlg.LimitActivatedChanged(static_cast<int>(params->GetBool("LimitRes")));
	dlg.MaxResNb->setValue(params->GetUInt("MaxResNb"));
	dlg.NValue->setValue(params->GetUInt("NValue"));

	unsigned int mode=params->GetUInt("QueryMode");
//	dlg.QueryModeOption->setButton(mode);
	dlg.ChangeGeneralMode(mode);

//	dlg.CombinaisonOptions->setButton(static_cast<int>(params->GetBool("PercentValue")));
	dlg.ChangeOptionMode(static_cast<int>(params->GetBool("PercentValue")));

	dlg.PLowerPC->setValue(params->GetUInt("PLowerPC"));
	dlg.PUpperPC->setValue(params->GetUInt("PUpperPC"));
	dlg.PLowerAbs->setValue(params->GetUInt("PLowerAbs"));
	dlg.PUpperAbs->setValue(params->GetUInt("PUpperAbs"));

	if(dlg.exec())
	{
		params->SetBool("UseWeight",dlg.UseWeight->isChecked());
		params->SetBool("LimitRes",dlg.LimitRes->isChecked());
		params->SetUInt("MaxResNb",dlg.MaxResNb->value());
		params->SetUInt("NValue",dlg.NValue->value());
//		params->SetUInt("QueryMode",dlg.QueryModeOption->selectedId());
//		params->SetBool("PercentValue",static_cast<bool>(dlg.CombinaisonOptions->selectedId()));
		params->SetUInt("PLowerPC",dlg.PLowerPC->value());
		params->SetUInt("PUpperPC",dlg.PUpperPC->value());
		params->SetUInt("PLowerAbs",dlg.PLowerAbs->value());
		params->SetUInt("PUpperAbs",dlg.PUpperAbs->value());
		params->Apply();
	}
}

//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
