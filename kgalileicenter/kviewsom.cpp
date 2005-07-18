/*

	GALILEI Research Project

	KViewSom.cpp

	Window for manipulating a specific document - Implementation.

	Copyright 2001-2002 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include files ANSI C/C++
#include <stdlib.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R Project
#include<rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <qlistviewitemtype.h>
#include <qgdocxml.h>
#include <gdoc.h>
#include <gdocxml.h>
#include <guser.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpushbutton.h>
#include <qpointarray.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qsplitter.h>
#include <qpoint.h>
#include <qcolordialog.h>
#include <qstring.h>
#include <qsommapconfig.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <klocale.h>
#include <kdoc.h>

//-----------------------------------------------------------------------------
// application specific includes
#include "kviewsom.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KViewSom
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewSom::KViewSom(KDoc* doc,  QWidget* parent,const char* name, const char* dataname, int wflags, unsigned int mapid)
	: KView(doc,parent,name,wflags), MapId(mapid), bMapLoaded(false), DataName(dataname)
{
	//icon & capton
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	setCaption(name);

	//general Layout
	QVBoxLayout* GeneralVL = new QVBoxLayout(this, 20, 20, "GeneralVL");

	//first splitter
	QSplitter* splitter  = new QSplitter( this, "splitter" );
	splitter->setOrientation( QSplitter::Vertical );
	QSplitter* splitter1 = new QSplitter( splitter, "splitter1" );
	splitter1->setOrientation( QSplitter::Horizontal );

	//map box
	QGroupBox* mapbox = new QGroupBox( splitter1, "MapBox" );
	mapbox->setTitle( tr2i18n( "Map" ));
	QVBoxLayout* mapboxL = new QVBoxLayout(mapbox, 20, 20, "mapboxL");

	// map widget
	SomMap=new KSomMap(mapbox);
	SomMap->setBackgroundColor(black);
	mapboxL->addWidget(SomMap);

	//map buttons
	QHBoxLayout* buttonsLayout=new QHBoxLayout(0,0,0,"buttonLayout");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	//load map button
	loadMapButton=new QPushButton(mapbox,"LoadMap");
	loadMapButton->setText( tr2i18n( "&LoadMap" ));
	buttonsLayout->addWidget(loadMapButton);
	//refresh button
	optionsButton=new QPushButton(mapbox," Draw Map");
	optionsButton->setText( tr2i18n( "&Options[...]" ));
	optionsButton->setEnabled(false);
	buttonsLayout->addWidget(optionsButton);
	//choose color button
	QPushButton* colorButton=new QPushButton(mapbox,"Color button");
	colorButton->setText( tr2i18n( "&Color[...]" ));
	buttonsLayout->addWidget(colorButton);
	buttonsLayout->addItem(spacer);
	//show hide labels button
	showLabelsButton=new QPushButton(mapbox,"Show/Hide Labels");
	showLabelsButton->setText( tr2i18n( "&Show Labels" ));
	showLabelsButton->setEnabled(false);
	buttonsLayout->addWidget(showLabelsButton);
	NbLabels=new KIntNumInput(mapbox,"nblabels");
	NbLabels->setValue(SomMap->GetNbDisplayedLabels());
	buttonsLayout->addWidget(NbLabels);
	mapboxL->addLayout(buttonsLayout);

	//infos box
	QGroupBox* infosbox = new QGroupBox( splitter1, "InfoBox" );
	infosbox->setTitle( tr2i18n( "Neuron Information [ - click on map - ]" ));
	QVBoxLayout* infosboxVL = new QVBoxLayout(infosbox, 20, 20, "infosboxVL");
	NeuronInfos=new QListView(infosbox);
	NeuronInfos->setSorting(-1);
	NeuronInfos->addColumn(QString("Labels"));
	new QListViewItem(NeuronInfos," - no map loaded - ");
	infosboxVL->addWidget(NeuronInfos);

	//map infos box
	QGroupBox* mapinfosbox = new QGroupBox(splitter, "MapInfoBox" );
 	mapinfosbox->setTitle( tr2i18n( "General Map Information" ));
	mapinfosbox->setMaximumSize(32767,200);
	QVBoxLayout* mapinfosboxVL = new QVBoxLayout(mapinfosbox, 20, 20, "mapinfosboxVL");

	//map infos listview
 	MapInfos=new QListView(mapinfosbox, "Map Infos ListView");
	MapInfos->addColumn(QString("Map parameters"));
	MapInfos->addColumn(QString(" Value"));
	mapinfosboxVL->addWidget(MapInfos);

	//insert element into layout
	GeneralVL->addWidget(splitter);

	//conections
	connect(loadMapButton, SIGNAL( clicked()), this, SLOT( slotInitMap()));
	connect(optionsButton, SIGNAL( clicked()), this, SLOT( slotMapOptions()));
	connect(NbLabels, SIGNAL( valueChanged(int)), SomMap, SLOT( slotNbLabelsChanged(int)));
	connect(colorButton, SIGNAL( clicked()), this, SLOT( slotChooseColor()));
	connect(showLabelsButton, SIGNAL( clicked()), this, SLOT( slotHideShowLabels()));
	connect(SomMap, SIGNAL( ClickedNeuron(int)), this, SLOT(slotClickedNeuron(int)));

	//resize window
	resize( QSize(765, 606).expandedTo(minimumSizeHint()));
}


//-----------------------------------------------------------------------------
void KViewSom::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewSom::resizeEvent(QResizeEvent *)
{
}


//-----------------------------------------------------------------------------
void KViewSom::slotMapOptions(void)
{
	QSomMapConfig dlg(0, "SOM Configuration");
	//init values
	dlg.NbLabels->setValue(SomMap->GetNbDisplayedLabels());

	//set connections
	connect(dlg.CBGeometry, SIGNAL( activated(const QString&)), SomMap, SLOT( slotGeometryChanged(const QString&)));
	connect(dlg.NbLabels, SIGNAL( valueChanged(int)), SomMap, SLOT( slotNbLabelsChanged(int)));

	if (dlg.exec())
		return;
}


//-----------------------------------------------------------------------------
void KViewSom::Init(void)
{
	QListViewItem* neuron;
	unsigned int i, j, xdim, ydim;
	RQuery* select;
	char tmp[10];

	//load the map dimensions from database
	GStorage* store=Doc->GetSession()->GetStorage();
	select=store->SelectDummyEntry("Som_maps_details", MapId,"XDim",0, 3);
	select->Start();
	xdim=atoi((*select)[1]);
	select=store->SelectDummyEntry("Som_maps_details", MapId,"YDim",0,3);
	select->Start();
	ydim=atoi((*select)[1]);
 	SomMap->SetDim(xdim,ydim);
 	SomMap->DrawInitMap();

	//load each neurons;
	NeuronInfos->clear();
	neuron=0;
 	for (i=0; i<(SomMap->X()*SomMap->Y()); i++)
 	{
 		SomMap->DrawInitNeuron(i); // display the neuron being loaded
		sprintf(tmp,"%u", i);
		//load the labels of each neurons;
		select=store->SelectDummyEntry("Som_codebooks",i,RString::Null,MapId, 5);
 		SomMap->InitNeuronLabels(i, select->GetNbRows());
		for (j=0,select->Start(); !select->End(); select->Next(),j++)
			SomMap->Neurons[i].labels[j]=ToQString((*select)[2]);

		//load data behind each neurons.
		select=store->SelectDummyEntry("Som_projection",0,tmp,MapId, 6);
		SomMap->InitNeuronData(i, select->GetNbRows());
		for (j=0,select->Start(); !select->End(); select->Next(),j++)
			SomMap->Neurons[i].datas[j].id=atoi((*select)[0]);

		//displayed the loaded neuron
		neuron=new QListViewItem(NeuronInfos, neuron,QString("Neuron ")+tmp+QString(" loaded !"));
	}

	//reload the map with gradient colors
	SomMap->DrawMap();

	//manage button
 	loadMapButton->setEnabled(false);
 	optionsButton->setEnabled(true);
 	showLabelsButton->setEnabled(true);

	//fills the map info
	bMapLoaded=true;
	FillMapInfos();
}


//-----------------------------------------------------------------------------
void KViewSom::slotClickedNeuron(int i)
{
	QListViewItem* item, *subitem, *data, *labels, *label=0;
	unsigned int j;

	NeuronInfos->clear();
	//sprintf(tmp,"Neuron %u", i);
	item=new QListViewItem(NeuronInfos,"Neuron "+QString::number(i));
	item->setOpen(true);
	subitem=new QListViewItem(item,ToQString(DataName)+" - Number  : "+QString::number(SomMap->Neurons[i].nbcontent));
	for (j=0; j<(SomMap->Neurons[i].nbcontent); j++)
	{
		data=new QListViewItem(subitem, ToQString(DataName)+" "+QString::number(SomMap->Neurons[i].datas[j].id));
	}
	labels=new QListViewItem(item, "Number of Labels : "+QString::number(SomMap->Neurons[i].nblabels));
	labels->setOpen(true);
	for (j=0; j<(SomMap->Neurons[i].nblabels); j++)
		label=new QListViewItem(labels, label, SomMap->Neurons[i].labels[j]);
}


//-----------------------------------------------------------------------------
void KViewSom::slotHideShowLabels(void)
{
	SomMap->SwitchShowLabels();

	if (SomMap->GetbShowLabels())
	{
		SomMap->ShowLabels();
		showLabelsButton->setText( tr2i18n( "&HideLabels" ));
	}
	else
	{
		SomMap->HideLabels();
		showLabelsButton->setText( tr2i18n( "&ShowLabels" ));
	}
}


//-----------------------------------------------------------------------------
void KViewSom::slotChooseColor(void)
{
	QColor c=QColorDialog::getColor((*SomMap->GetColor()));
	SomMap->GetColor()->setRgb(c.red(), c.green(), c.blue());
	SomMap->DrawMap();
}


//-----------------------------------------------------------------------------
void KViewSom::FillMapInfos(void)
{
	char tmp[100];
	unsigned int maxdata, mindata, maxid, minid,i;
	QListViewItem* item;

	//if no map is loaded, nothong to display
	if (!bMapLoaded) return ;

	sprintf(tmp,"%u", SomMap->X()*SomMap->Y());
	item=new QListViewItem(MapInfos,"Number of Neurons :",tmp);
	sprintf(tmp,"%u units", SomMap->X());
	item=new QListViewItem(MapInfos,"X-Dimension :",tmp);
	sprintf(tmp,"%u units", SomMap->Y());
	item=new QListViewItem(MapInfos,"Y-Dimension :",tmp);

	//find the max and min data contained in map:
	if(SomMap->X())
	{
		maxdata=SomMap->Neurons[0].nbcontent;
		maxid=SomMap->Neurons[0].id;
		mindata=SomMap->Neurons[0].nbcontent;
		minid=SomMap->Neurons[0].id;
	}
	else
	{
		maxdata=maxid=mindata=minid=0;
		return;
	}

	for (i=1; i<(SomMap->X()*SomMap->Y()); i++)
	{
		if (SomMap->Neurons[i].nbcontent>maxdata)
		{
			maxdata=SomMap->Neurons[i].nbcontent;
			maxid=SomMap->Neurons[i].id;
		}
		if (SomMap->Neurons[i].nbcontent<mindata&&SomMap->Neurons[i].nbcontent)
		{
			mindata=SomMap->Neurons[i].nbcontent;
			minid=SomMap->Neurons[i].id;
		}
	}

	sprintf(tmp,"neuron Id = %u, number of data = %u", maxid, maxdata );
	item=new QListViewItem(MapInfos,"Thick Neuron :",tmp);
	sprintf(tmp,"neuron Id = %u, number of data = %u", minid, mindata );
	item=new QListViewItem(MapInfos,"Thin Neuron :",tmp);
}


//-----------------------------------------------------------------------------
KViewSom::~KViewSom(void)
{
	delete SomMap;
}
