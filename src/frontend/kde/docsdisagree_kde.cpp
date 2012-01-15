/*

	GALILEI Research Project

	DocsDisagree_KDE.cpp

	Disagreement between documents (KDE Part) - Implementation.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSpinBox>
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgmatrixmeasuredlg.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// Specific Dialog class
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyDlg : public QGMatrixMeasureDlg
{
	QSpinBox* MinProfiles;

public:

	MyDlg(void) : QGMatrixMeasureDlg("Disagreement ratios between documents") {}
	virtual void Panel(void);
	virtual void Init(GPlugIn* fac);
	virtual void Done(GPlugIn* fac);
};


//-----------------------------------------------------------------------------
void MyDlg::Panel(void)
{
    QHBoxLayout* layout = new QHBoxLayout();
    QLabel* text = new QLabel(GetMeasureSpecific());
    text->setText("Minimum common profiles");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
   	MinProfiles = new QSpinBox(GetMeasureSpecific());
    layout->addWidget(MinProfiles);
	GetMeasureSpecificLayout()->addLayout(layout);
}



//-----------------------------------------------------------------------------
void MyDlg::Init(GPlugIn* fac)
{
	QGMatrixMeasureDlg::Init(fac);
	MinProfiles->setValue(fac->FindParam<RParamValue>("MinProfiles")->GetInt());
}


//-----------------------------------------------------------------------------
void MyDlg::Done(GPlugIn* fac)
{
	fac->FindParam<RParamValue>("MinProfiles")->SetUInt(MinProfiles->value());
	QGMatrixMeasureDlg::Done(fac);
}



//------------------------------------------------------------------------------
//
extern "C" {
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "docsdisagree",0, ki18n("Count Method"),
		"1.0", ki18n("The count method is used to computed the disagreement ratios between the documents."), KAboutData::License_GPL,
		ki18n("(C) 2003-2012 by Pascal Francq\n(C) 2003-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	MyDlg dlg;
	return(dlg.Configure(fac));
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
