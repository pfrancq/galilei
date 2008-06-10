/*

	GALILEI Research Project

	QDocsDisagree_KDE.cpp

	A KDE about box for the documents disagreement measure - Implementation.

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



//-----------------------------------------------------------------------------
// include files for Qt
#include <qlabel.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qspinbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <knuminput.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgmeasure2elementsdlg.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("The count method is used to computed the disagreements between the documents.");



//-----------------------------------------------------------------------------
//
// Specific Dialog class
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyDlg : public QGMeasure2ElementsDlg
{
	QSpinBox* MinProfiles;
	
public:

	MyDlg(void) : QGMeasure2ElementsDlg("Disagreement ratios between documents") {}
	virtual void Panel(void);
	virtual void Init(GFactoryMeasure* params);
	virtual void Done(GFactoryMeasure* params);
};


//-----------------------------------------------------------------------------
void MyDlg::Panel(void)
{
    QHBoxLayout* layout = new QHBoxLayout(0,0,6);
    QLabel* text = new QLabel(GetMeasureSpecific());
    text->setText("Minimum common profiles");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
   	MinProfiles = new QSpinBox(GetMeasureSpecific(),"MinProfiles");
    layout->addWidget(MinProfiles);
	GetMeasureSpecificLayout()->addLayout(layout);
}


//-----------------------------------------------------------------------------
void MyDlg::Init(GFactoryMeasure* params)
{
	QGMeasure2ElementsDlg::Init(params);
	MinProfiles->setValue(params->GetUInt("MinProfiles"));
}


//-----------------------------------------------------------------------------
void MyDlg::Done(GFactoryMeasure* params)
{
	params->SetUInt("MinProfiles",MinProfiles->value());
	QGMeasure2ElementsDlg::Done(params);
}



//------------------------------------------------------------------------------
//
extern "C" {
//	
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "count", I18N_NOOP("Count Method"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 2005, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Contributor"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryMeasure* params)
{
	MyDlg dlg;
	dlg.Configure(params);
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
