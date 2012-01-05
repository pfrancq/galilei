/*

	GALILEI Research Project

	GenericSims_KDE.cpp

	Generic Tensor Space Model Similarity Measure (KDE Part) - Implementation.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Université Libre de Bruxelles (ULB).

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
#include <QtGui/QLayout>
#include <QtGui/QToolTip>
#include <QtGui/QWhatsThis>
#include <kmessagebox.h>


//------------------------------------------------------------------------------
// includes files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include "genericsims_kde.h"
#include <gmeasure.h>



//-----------------------------------------------------------------------------
//
// class GGenericSimsDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGenericSimsDlg::GGenericSimsDlg(const QString& title)
	: QGMatrixMeasureDlg(title)
{
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::AddCapacity(KDoubleNumInput* &cap,const char* str,QGridLayout* grid,int row,int col)
{
	QHBoxLayout* layout = new QHBoxLayout();
	QLabel* text = new QLabel(GetMeasureSpecific());
   text->setText(str);
   layout->addWidget(text);
	cap = new KDoubleNumInput(GetMeasureSpecific());
	cap->setSpecialValueText(str);
   layout->addWidget(cap);
   grid->addItem(layout,row,col);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Panel(void)
{
	QHBoxLayout* layout = new QHBoxLayout();
   QLabel* text = new QLabel(GetMeasureSpecific());
   text->setText("Similarity Type");
   layout->addWidget(text);
	SimType = new QComboBox(GetMeasureSpecific());
	SimType->addItem("Integral of Choquet");
	SimType->addItem("Product");
	SimType->addItem("Sum");
	SimType->addItem("Text Only");
   layout->addWidget(SimType);
   SimType->setToolTip("The similarity measure can be the classical cosine between the vectors (textual) or the one of the tensor space model." );
   const char* simText= "<p>Choose the <b>similarity measure</b> that will be used. <br/>"
                        "The <em>tensor space model</em> measure computes a similarity based on the different concepts categories where the "
    	                  "elements are indexed (textual, metadata and semantic). Each category have its own measure"
    	                  ", such as the adapted cosine between the vectors in this space. ";
   SimType->setWhatsThis(simText);
	layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	GetMeasureSpecificLayout()->addLayout(layout);

	// Factor
	layout = new QHBoxLayout();
   text = new QLabel(GetMeasureSpecific());
   text->setText("Factor for the product");
   layout->addWidget(text);
	Factor = new KDoubleNumInput(GetMeasureSpecific());
	Factor->setSpecialValueText("Product Factor");
	Factor->setDecimals(5);
   layout->addWidget(Factor);
	layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	GetMeasureSpecificLayout()->addLayout(layout);

	// Parameters
	QGridLayout* layout2=new QGridLayout();
	AddCapacity(TextualCapacity,"Textual Capacity",layout2,0,0);
	AddCapacity(SemanticCapacity,"Semantic Capacity",layout2,1,0);
	AddCapacity(MetadataCapacity,"Metadata Capacity",layout2,2,0);
   layout2->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum),0,1);
	AddCapacity(TextualSemanticCapacity,"Textual/Semantic Capacity",layout2,0,2);
	AddCapacity(TextualMetadataCapacity,"Textual/Metadata Capacity",layout2,1,2);
	AddCapacity(SemanticMetadataCapacity,"Semantic/Metadata Capacity",layout2,2,2);
	GetMeasureSpecificLayout()->addLayout(layout2);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Init(GPlugIn* plugin)
{
	QGMatrixMeasureDlg::Init(plugin);
	SimType->setCurrentIndex(SimType->findText(ToQString(plugin->FindParam<RParamValue>("SimType")->Get())));
	Factor->setValue(plugin->FindParam<RParamValue>("Product Factor")->GetDouble());
	TextualCapacity->setValue(plugin->FindParam<RParamValue>("Textual Capacity")->GetDouble());
	SemanticCapacity->setValue(plugin->FindParam<RParamValue>("Semantic Capacity")->GetDouble());
	MetadataCapacity->setValue(plugin->FindParam<RParamValue>("Metadata Capacity")->GetDouble());
	TextualSemanticCapacity->setValue(plugin->FindParam<RParamValue>("Textual/Semantic Capacity")->GetDouble());
	TextualMetadataCapacity->setValue(plugin->FindParam<RParamValue>("Textual/Metadata Capacity")->GetDouble());
	SemanticMetadataCapacity->setValue(plugin->FindParam<RParamValue>("Semantic/Metadata Capacity")->GetDouble());
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Done(GPlugIn* plugin)
{
	plugin->FindParam<RParamValue>("SimType")->Set(FromQString(SimType->currentText()));
	plugin->FindParam<RParamValue>("Product Factor")->SetDouble(Factor->value());
	plugin->FindParam<RParamValue>("Textual Capacity")->SetDouble(TextualCapacity->value());
	plugin->FindParam<RParamValue>("Semantic Capacity")->SetDouble(SemanticCapacity->value());
	plugin->FindParam<RParamValue>("Metadata Capacity")->SetDouble(MetadataCapacity->value());
	plugin->FindParam<RParamValue>("Textual/Semantic Capacity")->SetDouble(TextualSemanticCapacity->value());
	plugin->FindParam<RParamValue>("Textual/Metadata Capacity")->SetDouble(TextualMetadataCapacity->value());
	plugin->FindParam<RParamValue>("Semantic/Metadata Capacity")->SetDouble(SemanticMetadataCapacity->value());
	QGMatrixMeasureDlg::Done(plugin);
}


//-----------------------------------------------------------------------------
bool GGenericSimsDlg::IsDlgOK(void)
{
	// Look if the current similarity is the integral of Choquet
	if(SimType->currentText()=="Integral of Choquet")
	{
		// The sum of the capacities should be equals to 1.
		double Sum(0.0);
		Sum+=TextualCapacity->value();
		Sum+=SemanticCapacity->value();
		Sum+=MetadataCapacity->value();
		Sum+=TextualSemanticCapacity->value();
		Sum+=TextualMetadataCapacity->value();
		Sum+=SemanticMetadataCapacity->value();
		if(Sum!=1.0)
		{
			KMessageBox::error(this,"When the integral of Choquet is used as aggregating function, the sum of the capacities should 1 and not "+QString::number(Sum));
			return(false);
		}
	}
	return(true);
}