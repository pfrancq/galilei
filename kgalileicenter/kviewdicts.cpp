/*

	GALILEI Research Project

	KViewDicts.cpp

	Window to show all the dictionnaries - Implementation.

	Copyright 2006-2007 by the Université Libre de Bruxelles.

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
#include <gconcept.h>
#include <gconcepttype.h>
#include <grelationtype.h>
#include <grelation.h>
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
#include <qsplitter.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qinputdialog.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// includes files for current application
#include "kviewdicts.h"
#include "qsessionprogress.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
//
// class KViewDicts::LocalItem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KViewDicts::LocalItem : public QListViewItem
{
public:
	size_t Id;

	LocalItem(QListView* v,size_t id,QString str) : QListViewItem(v,QString::number(id),str), Id(id) {}
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


//-----------------------------------------------------------------------------
class KViewDicts::LocalItem2 : public QListViewItem
{
public:
	GRelation* Rel;

	LocalItem2(QListView* v,GRelation* rel,GSession* session,KViewDicts* d) :
	    QListViewItem(v,d->BuildConcept(rel->GetSubject()),
	    ToQString(session->GetRelationType(rel->GetType(),false)->GetName()),
	    d->BuildConcept(rel->GetObject())) , Rel(rel) {}
};



//-----------------------------------------------------------------------------
//
// class KViewDicts
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDicts::KViewDicts(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Dicts(0), Dict(0), CurDict(0), CurConcept(0), Rels(0)
{
	setCaption("Dictionnaries");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_list.png",KIcon::Small)));

	QHBoxLayout* tabLayout = new QHBoxLayout(this, 0, 0, "tabLayout");
	QSplitter* splitter=new QSplitter(Qt::Horizontal,this);
	tabLayout->addWidget(splitter);

	Dicts = new QListView(splitter,"QListView of KViewDicts");
	Dicts->addColumn(QString("Dictionnaries"));
	Dicts->setRootIsDecorated(true);
	connect(Dicts,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(slotDoubleClick(QListViewItem*)));
	//tabLayout->addWidget(Dicts);

	QSplitter* splitter2=new QSplitter(Qt::Vertical,splitter);
	Dict = new QListView(splitter2,"QListView of KViewDict");
	Dict->addColumn(QString("ID"));
	Dict->addColumn(QString("Name"));
	Dict->setRootIsDecorated(false);
	connect(Dict,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(slotRelations(QListViewItem*)));
	connect(Dict,SIGNAL(rightButtonPressed(QListViewItem*,const QPoint&,int)),this,SLOT(slotRightButtonDict(QListViewItem*,const QPoint&,int)));

	QFrame* frm=new QFrame(splitter2);
	QVBoxLayout* tab2 = new QVBoxLayout(frm, 0, 0, "tab2");
//	splitter->setLayout(tab2);
	CurConcept=new QLabel("Nothing",frm);
	tab2->addWidget(CurConcept);
	Relations=new QListView(frm,"Relations of KViewDict");
	Relations->addColumn(QString("Subject"));
	Relations->addColumn(QString("Type"));
	Relations->addColumn(QString("Object"));
	connect(Relations,SIGNAL(rightButtonPressed(QListViewItem*,const QPoint&,int)),this,SLOT(slotRightButtonRelations(QListViewItem*,const QPoint&,int)));
	tab2->addWidget(Relations);

	CreateDictsListView();
}


//-----------------------------------------------------------------------------
void KViewDicts::CreateDictsListView(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Load Dictionaries",true);
	Dlg.Run(new QLoadDictionaries(Dicts,Doc->GetSession()));
}


//-----------------------------------------------------------------------------
void KViewDicts::slotDoubleClick(QListViewItem* item)
{
	QLoadDictionaries::QListViewItemDict* ptr=dynamic_cast<QLoadDictionaries::QListViewItemDict*>(item);
	if(!ptr)
		return;
	Dict->clear();
	CurDict=ptr->Dict;

	size_t i;
	const GConcept** concepts;
	for(i=ptr->Dict->GetConceptMaxId()+2,concepts=ptr->Dict->GetConcepts();--i;concepts++)
		if(*concepts)
			new LocalItem(Dict,(*concepts)->GetId(),ToQString((*concepts)->GetName()));
}

//-----------------------------------------------------------------------------
QString KViewDicts::BuildConcept(GConcept* concept)
{
	return("\""+ToQString(concept->GetName())+"\" ("+QString::number(concept->GetId())+","+QString::number(concept->GetType()->GetId())+")");
}


//-----------------------------------------------------------------------------
void KViewDicts::slotRelations(QListViewItem* item)
{
	LocalItem* ptr=dynamic_cast<LocalItem*>(item);
	if((!ptr)||(!CurDict))
		return;
	GConcept* concept=CurDict->GetConcept(ptr->Id);
	CurConcept->setText(BuildConcept(concept));
	Relations->clear();
	Rels.Clear();
	Doc->GetSession()->GetRelations(Rels,concept,cNoRef,0,true);
	RCursor<GRelation> Cur(Rels);
	for(Cur.Start();!Cur.End();Cur.Next())
		new LocalItem2(Relations,Cur(),Doc->GetSession(),this);
}


//-----------------------------------------------------------------------------
void KViewDicts::slotRightButtonDict(QListViewItem*,const QPoint& pos,int)
{
	if(!CurDict)
		return;
	QPopupMenu Menu(this,"Local");
	if(Dict->currentItem())
		Menu.insertItem("Delete Concept",this,SLOT(slotDeleteConcept(void)));
	Menu.insertItem("&Create New Concept",this,SLOT(slotNewConcept(void)));
	Menu.exec(pos);
}


//-----------------------------------------------------------------------------
void KViewDicts::slotDeleteConcept(void)
{
	LocalItem* ptr=dynamic_cast<LocalItem*>(Dict->currentItem());
	GConcept* concept=CurDict->GetConcept(ptr->Id);
	if(KMessageBox::warningYesNo(this,"Do you want to delete the concept "+BuildConcept(concept)+"?","Warning")==KMessageBox::No)
		return;
	Dict->takeItem(ptr);
	CurDict->DeleteConcept(concept);
}


//-----------------------------------------------------------------------------
void KViewDicts::slotNewConcept(void)
{
	bool ok;
	QString text = QInputDialog::getText("New concept", "Enter the name of the concept:",QLineEdit::Normal,QString::null,&ok,this);
	if(ok&&!text.isEmpty())
	{
		GConcept concept(FromQString(text),CurDict);
		GConcept* ptr=CurDict->InsertConcept(&concept);
		new LocalItem(Dict,ptr->GetId(),text);
	}
}


//-----------------------------------------------------------------------------
void KViewDicts::slotRightButtonRelations(QListViewItem*,const QPoint& pos,int)
{
	if(CurConcept->text()=="Nothing")
		return;
	QPopupMenu Menu(this,"Local");
	if(Relations->currentItem())
		Menu.insertItem("Delete Relation",this,SLOT(slotDeleteRelation(void)));
	Menu.insertItem("&Create New Relation",this,SLOT(slotNewRelation(void)));
	Menu.exec(pos);
}


//-----------------------------------------------------------------------------
void KViewDicts::slotDeleteRelation(void)
{
//	LocalItem2* ptr=dynamic_cast<LocalItem2*>(Relations->currentItem());
	if(KMessageBox::warningYesNo(this,"Do you want to delete the current relation?","Warning")==KMessageBox::No)
		return;
}


//-----------------------------------------------------------------------------
void KViewDicts::slotNewRelation(void)
{
}


//-----------------------------------------------------------------------------
void KViewDicts::update(tObjType type)
{
	if(type!=otLang) return;
	Dict->clear();
	Relations->clear();
	CurDict=0;
	CurConcept=0;
	CurConcept->setText("Nothing");
}


//-----------------------------------------------------------------------------
KViewDicts::~KViewDicts(void)
{
}
