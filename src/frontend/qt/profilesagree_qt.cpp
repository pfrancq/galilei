/*

	GALILEI Research Project

	ProfilesAgree_KDE.cpp

	Agreement between profiles (KDE Part) - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <qraboutdialog.h>
#include <gplugin.h>
#include <qgmatrixmeasuredlg.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <QSpinBox>
#include <QLabel>



//-----------------------------------------------------------------------------
//
// Specific Dialog class
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyDlg : public QGMatrixMeasureDlg
{
	QSpinBox* MinDocs;

public:

	MyDlg(void) : QGMatrixMeasureDlg("Agreement ratios between profiles") {}
	virtual void Panel(void);
	virtual void Init(GPlugIn* fac);
	virtual void Done(GPlugIn* fac);
};


//-----------------------------------------------------------------------------
void MyDlg::Panel(void)
{
	QHBoxLayout* layout = new QHBoxLayout();
	QLabel* text = new QLabel(GetMeasureSpecific());
	text->setText("Minimum common documents");
	layout->addWidget(text);
	layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	MinDocs = new QSpinBox(GetMeasureSpecific());
	layout->addWidget(MinDocs);
	GetMeasureSpecificLayout()->addLayout(layout);
}


//-----------------------------------------------------------------------------
void MyDlg::Init(GPlugIn* fac)
{
	QGMatrixMeasureDlg::Init(fac);
	MinDocs->setValue(fac->FindParam<RParamValue>("MinDocs")->GetInt());
}


//-----------------------------------------------------------------------------
void MyDlg::Done(GPlugIn* fac)
{
	fac->FindParam<RParamValue>("MinDocs")->SetUInt(MinDocs->value());
	QGMatrixMeasureDlg::Done(fac);
}


//------------------------------------------------------------------------------
extern "C" void About(void)
{
	QRAboutDialog dlg("Count Method","1.0");
	dlg.setDescription("The count method is used to computed the agreement ratios between the profiles.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2003-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.exec();

}



//------------------------------------------------------------------------------
extern "C" bool Configure(GPlugIn* fac)
{
	MyDlg dlg;
	return(dlg.Configure(fac));
}
