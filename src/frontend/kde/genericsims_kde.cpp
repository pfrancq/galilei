/*

	GALILEI Research Project

	GenericSims_KDE.cpp

	Generic Similarity Measure (KDE Part) - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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
//    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	cap = new KDoubleNumInput(GetMeasureSpecific());
	cap->setSpecialValueText(str);
    layout->addWidget(cap);
    grid->addItem(layout,row,col);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Panel(void)
{
	// Transform
	QHBoxLayout* layout = new QHBoxLayout();
	Transform=new QCheckBox("Transform the similarity from [-1,+1] to [0,1]",GetMeasureSpecific());
    layout->addWidget(Transform);
	GetMeasureSpecificLayout()->addLayout(layout);

    layout = new QHBoxLayout();
    QLabel* text = new QLabel(GetMeasureSpecific());
    text->setText("Similarity Type");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	SimType = new QComboBox(GetMeasureSpecific());
	SimType->addItem("Integral of Choquet");
	SimType->addItem("Product");
	SimType->addItem("Sum");
	SimType->addItem("Language only");
    layout->addWidget(SimType);
    SimType->setToolTip("Similarity measure can be the classical cosine between the vectors (Language) or an adapted one to manage multiple spaces (Multi-space)." );
    const char* simText="<p>Choose the <b>similarity measure</b> that will be used. <br/>"
                        "The <em>Multi-space</em> measure computes a similarity based on the different spaces where the "
    	                "elements are indexed (Content space, index space and structure space). Each space may have its own measure"
    	                ", such as the cosine between the vectors in this space. "
    	                "They are then combined with the multiplication operator (logical AND).<br/>"
    	                "The <em>Language</em> measure is the classical cosine between the two vectors. It is supposed that "
    	                "they are both indexed in one common language. If the vectors are indexed in several language spaces, the result is "
    	                "undefined.</p>";
    SimType->setWhatsThis(simText);
	GetMeasureSpecificLayout()->addLayout(layout);

	// Factor
	layout = new QHBoxLayout();
   	text = new QLabel(GetMeasureSpecific());
    text->setText("Factor for the product");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	Factor = new KDoubleNumInput(GetMeasureSpecific());
	Factor->setSpecialValueText("Factor");
	Factor->setDecimals(5);
    layout->addWidget(Factor);
	GetMeasureSpecificLayout()->addLayout(layout);

	// Parameters
	QGridLayout* layout2=new QGridLayout();
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
void GGenericSimsDlg::Init(GPluginFactory* params)
{
	QGMatrixMeasureDlg::Init(params);
	SimType->setCurrentIndex(SimType->findText(ToQString(params->Get("SimType"))));
	Factor->setValue(params->GetDouble("Factor"));
	ContentCapacity->setValue(params->GetDouble("ContentCapacity"));
	StructCapacity->setValue(params->GetDouble("StructCapacity"));
	MetaCapacity->setValue(params->GetDouble("MetaCapacity"));
	ContentStructCapacity->setValue(params->GetDouble("ContentStructCapacity"));
	ContentMetaCapacity->setValue(params->GetDouble("ContentMetaCapacity"));
	MetaStructCapacity->setValue(params->GetDouble("MetaStructCapacity"));
	Transform->setChecked(params->GetBool("Transform"));
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Done(GPluginFactory* params)
{
	params->Set("SimType",FromQString(SimType->currentText()));
	params->SetDouble("Factor",Factor->value());
	params->SetDouble("ContentCapacity",ContentCapacity->value());
	params->SetDouble("StructCapacity",StructCapacity->value());
	params->SetDouble("MetaCapacity",MetaCapacity->value());
	params->SetDouble("ContentStructCapacity",ContentStructCapacity->value());
	params->SetDouble("ContentMetaCapacity",ContentMetaCapacity->value());
	params->SetDouble("MetaStructCapacity",MetaStructCapacity->value());
	params->SetBool("Transform",Transform->isChecked());
	QGMatrixMeasureDlg::Done(params);
}
