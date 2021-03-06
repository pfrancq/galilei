/*

	GALILEI Research Project

	GenericSims_Qt.cpp

	Generic Tensor Space Model Similarity Measure (Qt Part) - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Universit√© Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QToolTip>
#include <QtGui/QWhatsThis>
#include <QMessageBox>


//------------------------------------------------------------------------------
// includes files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <genericsims_qt.h>
#include <gmeasure.h>



//-----------------------------------------------------------------------------
//
// class GGenericSimsDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGenericSimsDlg::GGenericSimsDlg(const QString& title)
	: QGMatrixMeasureDlg(title), Ui_Specific()
{
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::AddCapacity(QDoubleSpinBox* &cap,const char* str,QGridLayout* grid,int row,int col)
{
	QHBoxLayout* layout = new QHBoxLayout();
	QLabel* text = new QLabel(GetMeasureSpecific());
   text->setText(str);
   layout->addWidget(text);
	cap = new QDoubleSpinBox(GetMeasureSpecific());
	cap->setSpecialValueText(str);
   layout->addWidget(cap);
   grid->addItem(layout,row,col);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Panel(void)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	GetMeasureSpecificLayout()->addWidget(ptr);
	connect(SimType,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(simTypeChanged(const QString&)));
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Init(GPlugIn* plugin)
{
	QGMatrixMeasureDlg::Init(plugin);
	NbHops->setValue(plugin->FindParam<RParamValue>("NbHops")->GetUInt());
	SimType->setCurrentIndex(SimType->findText(ToQString(plugin->FindParam<RParamValue>("SimType")->Get())));
	simTypeChanged(ToQString(plugin->FindParam<RParamValue>("SimType")->Get()));
	Factor->setValue(plugin->FindParam<RParamValue>("Product Factor")->GetDouble());
	Token->setValue(plugin->FindParam<RParamValue>("Token")->GetDouble());
	Structure->setValue(plugin->FindParam<RParamValue>("Structure")->GetDouble());
	Metadata->setValue(plugin->FindParam<RParamValue>("Metadata")->GetDouble());
	Link->setValue(plugin->FindParam<RParamValue>("Link")->GetDouble());
	TokenMetadata->setValue(plugin->FindParam<RParamValue>("Token/Metadata")->GetDouble());
	TokenStructure->setValue(plugin->FindParam<RParamValue>("Token/Structure")->GetDouble());
	TokenLink->setValue(plugin->FindParam<RParamValue>("Token/Link")->GetDouble());
	MetadataStructure->setValue(plugin->FindParam<RParamValue>("Metadata/Structure")->GetDouble());
	MetadataLink->setValue(plugin->FindParam<RParamValue>("Metadata/Link")->GetDouble());
	StructureLink->setValue(plugin->FindParam<RParamValue>("Structure/Link")->GetDouble());
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Done(GPlugIn* plugin)
{
	plugin->FindParam<RParamValue>("NbHops")->SetUInt(NbHops->value());
	plugin->FindParam<RParamValue>("SimType")->Set(FromQString(SimType->currentText()));
	plugin->FindParam<RParamValue>("Product Factor")->SetDouble(Factor->value());
	plugin->FindParam<RParamValue>("Token")->SetDouble(Token->value());
	plugin->FindParam<RParamValue>("Structure")->SetDouble(Structure->value());
	plugin->FindParam<RParamValue>("Metadata")->SetDouble(Metadata->value());
	plugin->FindParam<RParamValue>("Link")->SetDouble(Link->value());
	plugin->FindParam<RParamValue>("Token/Metadata")->SetDouble(TokenMetadata->value());
	plugin->FindParam<RParamValue>("Token/Structure")->SetDouble(TokenStructure->value());
	plugin->FindParam<RParamValue>("Token/Link")->SetDouble(TokenLink->value());
	plugin->FindParam<RParamValue>("Metadata/Structure")->SetDouble(MetadataStructure->value());
	plugin->FindParam<RParamValue>("Metadata/Link")->SetDouble(MetadataLink->value());
	plugin->FindParam<RParamValue>("Structure/Link")->SetDouble(StructureLink->value());
	QGMatrixMeasureDlg::Done(plugin);
}


//-----------------------------------------------------------------------------
bool GGenericSimsDlg::IsDlgOK(void)
{
	// Look if the current similarity is the 2-Additive Choquet Integral
	if(SimType->currentText()=="2-Additive Choquet Integral")
	{
		// The sum of the capacities should be equals to 1.
		double Sum(0.0);
		Sum+=Token->value();
		Sum+=Metadata->value();
		Sum+=Structure->value();
		Sum+=Link->value();
		Sum+=TokenMetadata->value();
		Sum+=TokenStructure->value();
		Sum+=TokenLink->value();
		Sum+=MetadataStructure->value();
		Sum+=MetadataLink->value();
		Sum+=StructureLink->value();
		if(fabs(Sum-1.0)>=0.0000000001)
		{
			QMessageBox::critical(this,"Error in Weights","When the 2-Additive Choquet Integral is used, the sum of the weights should 1 and not "+QString::number(Sum));
			return(false);
		}
	}
	return(true);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::simTypeChanged(const QString& text)
{
	if((text=="Text Only")||(text=="Product"))
		Criteria->setEnabled(false);
	else
		Criteria->setEnabled(true);
	if(text=="2-Additive Choquet Integral")
		Interactions->setEnabled(true);
	else
		Interactions->setEnabled(false);
	if(text=="Product")
	{
		FactorText->setEnabled(true);
		Factor->setEnabled(true);
	}
	else
	{
		FactorText->setEnabled(false);
		Factor->setEnabled(false);
	}
}


//-----------------------------------------------------------------------------
GGenericSimsDlg::~GGenericSimsDlg(void)
{
}
