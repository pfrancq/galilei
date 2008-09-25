/*

	GALILEI Research Project

	GenericSims_KDE.cpp

	Similarities between documents and groups - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

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
#include <qtooltip.h>
#include <qwhatsthis.h>


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
	QHBoxLayout*layout = new QHBoxLayout(0,0,6);
	QLabel* text = new QLabel(GetMeasureSpecific());
    text->setText(str);
    layout->addWidget(text);
//    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	cap = new KDoubleNumInput(GetMeasureSpecific(),str);
    layout->addWidget(cap);
    grid->addItem(layout,row,col);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Panel(void)
{
    QHBoxLayout* layout = new QHBoxLayout(0,0,6);
    QLabel* text = new QLabel(GetMeasureSpecific());
    text->setText("Similarity Type");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	SimType = new QComboBox(GetMeasureSpecific(),"SimType");
	SimType->insertItem("Integral of Choquet");
	SimType->insertItem("Product");
	SimType->insertItem("Sum");
	SimType->insertItem("Language only");
    layout->addWidget(SimType);
    QToolTip::add(SimType,"Similarity measure can be the classical cosine between the vectors (Language) or an adapted one to manage multiple spaces (Multi-space)." );
    const char* simText="<p>Choose the <b>similarity measure</b> that will be used. <br/>"
                        "The <em>Multi-space</em> measure computes a similarity based on the different spaces where the "
    	                "elements are indexed (Content space, index space and structure space). Each space may have its own measure"
    	                ", such as the cosine between the vectors in this space. "
    	                "They are then combined with the multiplication operator (logical AND).<br/>"
    	                "The <em>Language</em> measure is the classical cosine between the two vectors. It is supposed that "
    	                "they are both indexed in one common language. If the vectors are indexed in several language spaces, the result is "
    	                "undefined.</p>";
    QWhatsThis::add(SimType,simText);
	GetMeasureSpecificLayout()->addLayout(layout);

	// Factor
	layout = new QHBoxLayout(0,0,6);
   	text = new QLabel(GetMeasureSpecific());
    text->setText("Factor for the product");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	Factor = new KDoubleNumInput(GetMeasureSpecific(),"Factor");
	Factor->setPrecision(5);
    layout->addWidget(Factor);
	GetMeasureSpecificLayout()->addLayout(layout);

	// Parameters
	QGridLayout* layout2=new QGridLayout(0,3,3);
	AddCapacity(ContentCapacity,"Content Capacity",layout2,0,0);
	AddCapacity(StructCapacity,"Structure Capacity",layout2,1,0);
	AddCapacity(MetaCapacity,"Metadata Capacity",layout2,2,0);
    layout2->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum),0,1);
	AddCapacity(ContentStructCapacity,"Content/Structure Capacity",layout2,0,2);
	AddCapacity(ContentMetaCapacity,"Content/Metadata Capacity",layout2,1,2);
	AddCapacity(MetaStructCapacity,"Metadata/Structure Capacity",layout2,2,2);
	GetMeasureSpecificLayout()->addLayout(layout2);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Init(GFactoryMeasure* params)
{
	QGMatrixMeasureDlg::Init(params);
	SimType->setCurrentText(ToQString(params->Get("SimType")));
	Factor->setValue(params->GetDouble("Factor"));
	ContentCapacity->setValue(params->GetDouble("ContentCapacity"));
	StructCapacity->setValue(params->GetDouble("StructCapacity"));
	MetaCapacity->setValue(params->GetDouble("MetaCapacity"));
	ContentStructCapacity->setValue(params->GetDouble("ContentStructCapacity"));
	ContentMetaCapacity->setValue(params->GetDouble("ContentMetaCapacity"));
	MetaStructCapacity->setValue(params->GetDouble("MetaStructCapacity"));
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Done(GFactoryMeasure* params)
{
	params->Set("SimType",FromQString(SimType->currentText()));
	params->SetDouble("Factor",Factor->value());
	params->SetDouble("ContentCapacity",ContentCapacity->value());
	params->SetDouble("StructCapacity",StructCapacity->value());
	params->SetDouble("MetaCapacity",MetaCapacity->value());
	params->SetDouble("ContentStructCapacity",ContentStructCapacity->value());
	params->SetDouble("ContentMetaCapacity",ContentMetaCapacity->value());
	params->SetDouble("MetaStructCapacity",MetaStructCapacity->value());
	QGMatrixMeasureDlg::Done(params);
}
