/*

	GALILEI Research Project

	GEngineGoogle_KDE.cpp

	A KDE dialog box for Text analyser - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele(vavdaele@ulb.ac.be).

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
#include <gengine.h>
#include <rqt.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <QtGui/QColorGroup>
/*#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qradiobutton.h>
#include <qgroupbox.h>*/


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplicationdialog.h>
#include <knuminput.h>
#include <kurlrequester.h>

//-----------------------------------------------------------------------------
// include files for Current
//#include <ui_dlgconfig_qt.h>



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//static bool _first_use_plugin_ = true;
//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData("xmlengine", 0,ki18n("XML Search Engine"), "1.0", ki18n("This is a XML search engine."), KAboutData::License_GPL,
			ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("faiza abbaci"),ki18n("Contributor"), "jvalsami@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryEngine* params)
{
//	DlgConfig_Qt dlg;
/*	char *buff;
	RString paths;
	RContainer<RString, true, false> pathlist(10);
	RCursor<RString> curs;*/


	throw RException("Adapt to the new structure");
/*	dlg.Name->setUrl(ToQString(params->Get("Name")));
//	dlg.nbres = params->GetUInt("NbResults");
//	dlg.weight = params->GetDouble("Weight");
//	dlg.SetUpdate_Reset(params->GetBool("Update"), params->GetBool("Reset"));
	paths = params->Get("Paths");
	paths.Split(pathlist, '.');
	curs.Set(pathlist);
	for (curs.Start(); !curs.End(); curs.Next())
		dlg.ExternAddPath(curs()->Mid(1).Latin1(), curs()->Mid(0, 1) == "1");

	//promethee
	dlg.DocScP->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.DocScQ->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.DocScWeight->setValue(params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Weight")->GetDouble());

	dlg.DisP->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.DisQ->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.DisWeight->setValue(params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Weight")->GetDouble());

	dlg.TfiefP->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.TfiefQ->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.TfiefWeight->setValue(params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Weight")->GetDouble());

	dlg.SpecifP->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("P")->GetDouble());
	dlg.SpecifQ->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Q")->GetDouble());
	dlg.SpecifWeight->setValue(params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Weight")->GetDouble());


	if(dlg.exec())
	{


		params->Set("Name",FromQString(dlg.Name->url()));

		params->SetUInt("NbResults", dlg.nbres);
		params->SetDouble("Weight", dlg.weight);
		params->SetBool("Update", dlg.update);
		params->SetBool("Reset", dlg.reset);
		buff = dlg.GetAllPaths();
		params->Set("Paths", buff);
		delete buff;

		//promethee
		params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("P")->SetDouble(dlg.DocScP->value());
		params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.DocScQ->value());
		params->FindParam<RParamStruct>("DocSc Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.DocScWeight->value());

		params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("P")->SetDouble(dlg.DisP->value());
		params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.DisQ->value());
		params->FindParam<RParamStruct>("Distance Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.DisWeight->value());

		params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("P")->SetDouble(dlg.SpecifP->value());
		params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.SpecifQ->value());
		params->FindParam<RParamStruct>("Specificity Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.SpecifWeight->value());

		params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("P")->SetDouble(dlg.TfiefP->value());
		params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Q")->SetDouble(dlg.TfiefQ->value());
		params->FindParam<RParamStruct>("Tfief Criterion")->Get<RParamValue>("Weight")->SetDouble(dlg.TfiefWeight->value());

		params->Apply();
	}*/
}

//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
