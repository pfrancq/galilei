/*

	GALILEI Research Project

	KViewMetaEngine.cpp

	Window to manipulate a specific group - Implementation.

	Copyright 2001 by the Universite Libre de Bruxelles.

	Authors:
		Valery Vandaele(vavdaele@ulb.ac.be).

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
// include files for R Project
#include <rstring.h>
#include <rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gstorage.h>
#include <gpluginmanagers.h>
#include <genginedoc.h>
#include <gmetaengine.h>
#include <rqt.h>
#include <qlistviewitemtype.h>
using namespace GALILEI;
using namespace R;

//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qpopmenu.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kapp.h>
#include <knuminput.h>

//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewmetaengine.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KViewMetaEngine
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewMetaEngine::KViewMetaEngine(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("Query meta engine");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));

	//Declare Vertical layout
	QVBoxLayout* vLayout=new QVBoxLayout( this,5, 5, "vLayout");

	//Create a group box containing all the fields
	//to initiane a new search
	QGroupBox* groupBoxSearch = new QGroupBox( this, "groupBoxSearch" );
	groupBoxSearch->setColumnLayout(0, Qt::Vertical );
	groupBoxSearch->layout()->setSpacing( 4 );
	groupBoxSearch->layout()->setMargin( 8 );
	//groupBoxSearch->setTitle("search with the meta engine");
	QVBoxLayout* groupBoxSearchLayout = new QVBoxLayout( groupBoxSearch->layout() );
	groupBoxSearchLayout->setAlignment( Qt::AlignTop );

	//Option Nb Results to display
	QHBoxLayout* layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
	QLabel* icoLabel = new QLabel(groupBoxSearch,"icoLabel");
	icoLabel->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	layout1->addWidget(icoLabel);

	QSpacerItem* spacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    	layout1->addItem( spacer1 );
	QLabel* labelNb = new QLabel(groupBoxSearch,"labelNb");
	labelNb->setText("Number of results to display:");
	layout1->addWidget(labelNb);
	NbRes = new KIntSpinBox( groupBoxSearch, "NbRes" );
	NbRes->setMaxValue(1000);
	//Set Value: default = the value from meta engine option
	unsigned int tmp=dynamic_cast<GMetaEngineManager*>(GPluginManagers::PluginManagers.GetManager("MetaEngine"))->GetCurrentMethod()->GetNbResUsed();
	if(tmp==-1)
		NbRes->setValue(100);
	else
		NbRes->setValue(tmp);
	layout1->addWidget(NbRes);
	groupBoxSearchLayout->addLayout( layout1);

	QLabel* label = new QLabel(groupBoxSearch,"label");
	label->setText("Enter the query:");
	groupBoxSearchLayout->addWidget(label);

	/*QSpacerItem* spacerH = new QSpacerItem( 10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum );
	groupBoxSearchLayout->addItem(spacerH);
	*/
	QHBoxLayout* layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
	//Enter the query
	TxtQuery = new QLineEdit( groupBoxSearch, "TxtQuery" );
	TxtQuery->setFocus();
	layout2->addWidget( TxtQuery );
    	QSpacerItem* spacer2 = new QSpacerItem( 10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    	layout2->addItem( spacer2 );

	//search Button
	Search = new QPushButton(this,"Search");
	QFont btnSearch_font( Search->font() );
	btnSearch_font.setBold(true);
	Search->setFont( btnSearch_font );
	Search->setText("Search");
	Search->setAutoDefault( TRUE );
	Search->setDefault( TRUE );
	layout2->addWidget( Search);
	groupBoxSearchLayout->addLayout( layout2 );
	connect(Search,SIGNAL(clicked()),this,SLOT(QueryEngine()));

	vLayout->addWidget(groupBoxSearch);

	//Label to show number of results
	ResLabel = new QLabel(this,"ResLabel");
	ResLabel->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	ResLabel->setText("<b> No Results </b>");
	ResLabel->setIndent(10);
	vLayout->addWidget(ResLabel);

	//List view containg results details
	Results = new QListView(this);
	Results->addColumn(QString("Ranking"));
	Results->addColumn(QString("Title"),250);
	Results->addColumn(QString("URL"));
	Results->setSorting(-1,true);
	Results->setRootIsDecorated(true);
	connect(Results,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	//Add results to vertical layout
	vLayout->addWidget(Results);
	//CreateMetaEngineResultsListView();
}

//-----------------------------------------------------------------------------
GEngineDoc* KViewMetaEngine::GetCurrentEngineDoc(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Results->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=QListViewItemType::tEngineDoc)
		return(0);
	return(t->Obj.EngineDoc);
}


//-----------------------------------------------------------------------------
void KViewMetaEngine::CreateMetaEngineResultsListView(void)
{
	R::RCursor<GEngineDoc> engCur;
	engCur=(dynamic_cast<GMetaEngineManager*>(GPluginManagers::PluginManagers.GetManager("MetaEngine")))->GetCurrentMethod()->GetEngineDocs();
	QListViewItemType* urlitem=0, *afteritem=0;
	QListViewItemType* desc;
	unsigned int i=1;
	int nbRes=NbRes->value();

	for(engCur.Start();!engCur.End();engCur.Next() )
	{
		urlitem = new QListViewItemType(engCur(), Results,QString::number(i),ToQString(engCur()->GetTitle()),ToQString(engCur()->GetUrl()));
		urlitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
		//urlitem->setPixmap(1,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
		urlitem->moveItem(afteritem);
		afteritem=urlitem;

		desc=new QListViewItemType(engCur(), urlitem,"desc: ",ToQString(engCur()->GetDescription()));
		i++;
		if(i>nbRes)
			break;
	}
	QString qstr="<b> ";
	qstr+=QString::number(--i);
	qstr+=" Results displayed :</b>";
	ResLabel->setText(qstr);
}

//-----------------------------------------------------------------------------
void KViewMetaEngine::QueryEngine()
{
	RContainer<RString, true,false> keyWords(5,3);
	RString str="";
	int idF=0;

	//If no keywords specified -->Error
	if(TxtQuery->text().isEmpty())
	{
		ResLabel->setText("<b> Enter first a query!</b>");
		return;
	}


	//Retreive all keywords
	str=FromQString(TxtQuery->text());
	do
	{
		//Skip space at the beginning
		idF=str.Find(' ');
		while(idF==0)
		{
			str=str.Mid(1);
			idF=str.Find(' ');
		}
		idF=str.Find(' ');
		if(idF>0)
		{
			keyWords.InsertPtr(new RString(str.Mid(0,idF)));
			idF++;
			str=str.Mid(idF);
		}

	}while ((idF>0) &&(str.GetLen()>0));
	if(str.GetLen()>0)
		keyWords.InsertPtr(new RString(str));


	//Set text "Searching";
	ResLabel->setText("<b> Searching...</b>");
	Results->clear();
	KApplication::kApplication()->processEvents();

	Doc->GetSession()->QueryMetaEngine(keyWords);

	CreateMetaEngineResultsListView();
}

//-----------------------------------------------------------------------------
void KViewMetaEngine::update(unsigned int)
{
}


//-----------------------------------------------------------------------------
void KViewMetaEngine::resizeEvent(QResizeEvent *)
{
}
