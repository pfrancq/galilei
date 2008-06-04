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

//------------------------------------------------------------------------------
// includes files for R
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include "ggenericsims_kde.h"
#include <gmeasure.h>


//-----------------------------------------------------------------------------
//
// class GGenericSimsDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGenericSimsDlg::GGenericSimsDlg(const QString& title)
	: QGMeasure2ElementsDlg(title)
{
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Panel(void)
{
    QHBoxLayout* layout = new QHBoxLayout(0,0,6);
    QLabel* text = new QLabel(MeasureSpecific);
    text->setText("Similarity Type");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	SimType = new QComboBox(MeasureSpecific,"SimType");
	SimType->insertItem("Multi-vector");
	SimType->insertItem("Language");
    layout->addWidget(SimType);
	MeasureSpecificLayout->addLayout(layout);

	layout = new QHBoxLayout(0,0,6);
   	text = new QLabel(MeasureSpecific);
    text->setText("Factor");
    layout->addWidget(text);
    layout->addItem(new QSpacerItem(140,20,QSizePolicy::Expanding, QSizePolicy::Minimum));
	Factor = new KDoubleNumInput(MeasureSpecific,"Factor");
	Factor->setPrecision(5);
    layout->addWidget(Factor);
	MeasureSpecificLayout->addLayout(layout);
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Init(GFactoryMeasure* params)
{
	QGMeasure2ElementsDlg::Init(params);
	SimType->setCurrentText(ToQString(params->Get("SimType")));
	Factor->setValue(params->GetDouble("Factor"));
}


//-----------------------------------------------------------------------------
void GGenericSimsDlg::Done(GFactoryMeasure* params)
{
	params->Set("SimType",FromQString(SimType->currentText()));
	params->SetDouble("Factor",Factor->value());
	QGMeasure2ElementsDlg::Done(params);
}
