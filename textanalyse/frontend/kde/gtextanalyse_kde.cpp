/*

	GALILEI Research Project

	GTextAnalyse_KDE.cpp

	A KDE dialog box for Text analyser - Implementation.

	Copyright 2003-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocanalyse.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <qvariant.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qgroupbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <knuminput.h>
#include <kurlrequester.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is a text analyser for documents.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "text", I18N_NOOP("Text Analyser"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Valery Vandaele",I18N_NOOP("Contributor"), "vvandaele@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryDocAnalyse* params)
{
	DlgConfig_Qt dlg;

	// Stems
	dlg.StaticLang->setChecked(params->GetBool("StaticLang"));
	dlg.MinStopWords->setValue(params->GetDouble("MinStopWords"));
	dlg.MinWordSize->setValue(params->GetUInt("MinWordSize"));
	dlg.MinStemSize->setValue(params->GetUInt("MinStemSize"));
	dlg.MinOccur->setValue(params->GetUInt("MinOccur"));
	dlg.StoreFullWords->setChecked(params->GetBool("StoreFullWords"));
	dlg.NonLetterWords->setChecked(params->GetBool("NonLetterWords"));
	dlg.Filtering->setChecked(params->GetBool("Filtering"));
	dlg.NbSameOccur->setValue(params->GetUInt("NbSameOccur"));
	dlg.NormalRatio->setValue(params->GetDouble("NormalRatio"));
	dlg.NbSameOccur->setEnabled(params->GetBool("NbSameOccur"));
	dlg.NormalRatio->setEnabled(params->GetBool("Filtering"));
	dlg.groupFiltering->setEnabled(params->GetBool("NonLetterWords"));

	// Structure - Tags and attributes
	dlg.ExtractStruct->setChecked(params->GetBool("ExtractStruct"));
	dlg.StructIsContent->setChecked(params->GetBool("StructIsContent"));
	dlg.WeightStruct->setValue(params->GetDouble("WeightStruct"));
	dlg.AttrValues->setChecked(params->GetBool("AttrValues"));
	dlg.WeightValues->setValue(params->GetDouble("WeightValues"));
	dlg.toggleStruct(params->GetBool("ExtractStruct"));

	// Structure - Declarative tags
	dlg.ExtractIndex->setChecked(params->GetBool("ExtractIndex"));
	dlg.MaxTerms->setValue(params->GetUInt("MaxTerms"));
	dlg.MaxDepth->setValue(params->GetUInt("MaxDepth"));
	dlg.MaxOccurs->setValue(params->GetUInt("MaxOccurs"));
	dlg.MaxPercOccurs->setValue(params->GetDouble("MaxPercOccurs"));
	dlg.ChildTags->setChecked(params->GetBool("ChildTags"));
	dlg.MaxTerms->setEnabled(params->GetBool("ExtractIndex"));
	dlg.MaxDepth->setEnabled(params->GetBool("ExtractIndex"));
	dlg.ChildTags->setEnabled(params->GetBool("ExtractIndex"));
	dlg.MaxPercOccurs->setEnabled(params->GetBool("ExtractIndex"));
	dlg.MaxOccurs->setEnabled(params->GetBool("ExtractIndex"));

	if(dlg.exec())
	{
		// Stems
		params->SetBool("StaticLang",dlg.StaticLang->isChecked());
		params->SetDouble("MinStopWords",dlg.MinStopWords->value());
		params->SetUInt("MinWordSize",dlg.MinWordSize->value());
		params->SetUInt("MinStemSize",dlg.MinStemSize->value());
		params->SetUInt("MinOccur",dlg.MinOccur->value());
		params->SetBool("StoreFullWords",dlg.StoreFullWords->isChecked());
		params->SetBool("NonLetterWords",dlg.NonLetterWords->isChecked());
		params->SetBool("Filtering",dlg.Filtering->isChecked());
		params->SetUInt("NbSameOccur",dlg.NbSameOccur->value());
		params->SetDouble("NormalRatio",dlg.NormalRatio->value());

		// Structure - Tags and attributes
		params->SetBool("ExtractStruct",dlg.ExtractStruct->isChecked());
		params->SetBool("StructIsContent",dlg.StructIsContent->isChecked());
		params->SetDouble("WeightStruct",dlg.WeightStruct->value());
		params->SetBool("AttrValues",dlg.AttrValues->isChecked());
		params->SetDouble("WeightValues",dlg.WeightValues->value());

		// Structure - Tags and attributes
		params->SetBool("ExtractIndex",dlg.ExtractIndex->isChecked());
		params->SetUInt("MaxTerms",dlg.MaxTerms->value());
		params->SetUInt("MaxDepth",dlg.MaxDepth->value());
		params->SetBool("ChildTags",dlg.ChildTags->isChecked());
		params->SetDouble("MaxPercOccurs",dlg.MaxPercOccurs->value());
		params->SetUInt("MaxOccurs",dlg.MaxOccurs->value());
		params->Apply();
	}
}

//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
