/*

	GALILEI Research Project

	GPromRank_Qt.cpp

	Promethee Ranking Method (Qt Part) - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#include <gcomputerank.h>
#include <qraboutdialog.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QDialog>
#include <ui_gpromrank.h>



//------------------------------------------------------------------------------
class Config : public QDialog, public Ui_Config
{
public:
	Config(void)
	{
		setupUi(this);
		setWindowTitle("Configure Promethee Ranking");
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
		connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
		setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
		adjustSize();
	}
};



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	QRAboutDialog dlg("Promethee Ranking","2.0");
	dlg.setDescription("This is a Promethee ranking method.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2004-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Faiza Abbaci"),QWidget::trUtf8("Contributor"));
	dlg.addAuthor(QWidget::trUtf8("Jean-Baptiste Valsamis"),QWidget::trUtf8("Contributor"));
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	Config Dlg;

	Dlg.TfIdf->Set(fac->FindParam<RParamStruct>("TfIdf"));
	Dlg.Distance->Set(fac->FindParam<RParamStruct>("Distance"));
	Dlg.Specificity->Set(fac->FindParam<RParamStruct>("Specificity"));
	Dlg.Type->Set(fac->FindParam<RParamStruct>("Type"));
	Dlg.TfIff->Set(fac->FindParam<RParamStruct>("TfIff"));

	if(Dlg.exec())
	{
		Dlg.TfIdf->Get(fac->FindParam<RParamStruct>("TfIdf"));
		Dlg.Distance->Get(fac->FindParam<RParamStruct>("Distance"));
		Dlg.Specificity->Get(fac->FindParam<RParamStruct>("Specificity"));
		Dlg.Type->Get(fac->FindParam<RParamStruct>("Type"));
		Dlg.TfIff->Get(fac->FindParam<RParamStruct>("TfIff"));
		return(true);
	}
	return(false);
}


 //------------------------------------------------------------------------------
 }
 //------------------------------------------------------------------------------
