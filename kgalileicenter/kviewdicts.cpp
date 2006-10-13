/*

	GALILEI Research Project

	KViewDicts.cpp

	Window to show all the dictionnaries - Implementation.

	Copyright 2006 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
// include files for GALILEI
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <gdoc.h>
#include <gdict.h>
#include <gconcept.h>
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qlistview.h>
#include <qlayout.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// includes files for current application
#include "kviewdicts.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// QListViewItemDict
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QListViewItemDict : public QListViewItem
{
public:
	GDict* Dict;

	QListViewItemDict(QListViewItem* parent,KDoc* doc,GDict* dict)
		: QListViewItem(parent,ToQString(doc->GetSession()->GetConceptType(dict->GetType()))), Dict(dict)
	{
		setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("xmag.png",KIcon::Small)));
	}
};




//-----------------------------------------------------------------------------
//
// class KViewDicts
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDicts::KViewDicts(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Dicts(0), Dict(0)
{
	setCaption("Dictionnaries");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_list.png",KIcon::Small)));

	QHBoxLayout* tabLayout = new QHBoxLayout(this, 0, 0, "tabLayout");

	Dicts = new QListView(this,"QListView of KViewDicts");
	Dicts->addColumn(QString("Dictionnaries"));
	Dicts->setRootIsDecorated(true);
	connect(Dicts,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(slotDoubleClick(QListViewItem*)));
	tabLayout->addWidget(Dicts);

	Dict = new QListView(this,"QListView of KViewDict");
	Dict->addColumn(QString("ID"));
	Dict->addColumn(QString("Name"));
	Dict->setRootIsDecorated(false);
	tabLayout->addWidget(Dict);

	CreateDictsListView();
}


//-----------------------------------------------------------------------------
void KViewDicts::CreateDictsListView(void)
{
	// Go trough each language and create a Item.
	R::RCursor<GLang> CurLang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
	for(CurLang.Start(); !CurLang.End(); CurLang.Next())
	{
		QListViewItemType* ptr=new QListViewItemType(Dicts,ToQString(CurLang()->GetName()));
		ptr->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));

		// Go trough each dictionnary and create an item
		RCursor<GDict> Dicts(CurLang()->GetDicts());
		for(Dicts.Start();!Dicts.End();Dicts.Next())
			new QListViewItemDict(ptr,Doc,Dicts());
	}
}


//-----------------------------------------------------------------------------
void KViewDicts::slotDoubleClick(QListViewItem* item)
{
	class LocalItem : QListViewItem
	{
	public:
		unsigned int Id;

		LocalItem(QListView* v,unsigned int id,QString str) : QListViewItem(v,QString::number(id),str), Id(id) {}
		virtual int compare( QListViewItem *i, int col, bool ascending ) const
    	{
			if(col==0)
			{
				double d=static_cast<double>(Id)-static_cast<double>(static_cast<LocalItem*>(i)->Id);
				if(d==0.0) return(key(0, ascending ).compare( i->key(0, ascending)));
				if(d>0)
					return(1);
				return(-1);
			}
			return(key( col, ascending ).compare( i->key( col, ascending)));
    	}
	};

	QListViewItemDict* ptr=dynamic_cast<QListViewItemDict*>(item);
	if(!ptr)
		return;
	Dict->clear();

	unsigned int i;
	const GConcept** concepts;
	for(i=ptr->Dict->GetConceptMaxId()+1,concepts=ptr->Dict->GetConcepts();--i;concepts++)
		if(*concepts)
			new LocalItem(Dict,(*concepts)->GetId(),ToQString((*concepts)->GetName()));
}


//-----------------------------------------------------------------------------
void KViewDicts::update(unsigned int cmd)
{
	if(cmd!=0) return;
	Dict->clear();
}


//-----------------------------------------------------------------------------
KViewDicts::~KViewDicts(void)
{
}
