/*

	GALILEI Research Project

	KViewDicts.cpp

	Window to show all the dictionaries - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI Projects
#include <rqt.h>
#include <gconcept.h>
#include <gpredicate.h>
#include <gstatement.h>
#include <ggalileiapp.h>
#include <gsession.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QInputDialog>
#include <kmessagebox.h>
#include <kapplication.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewdicts.h>
#include <qsessionprogress.h>



//-----------------------------------------------------------------------------
//
// class QDict
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QGObject : public QTreeWidgetItem
{
public:

	union
	{
		GConceptType* Dict;
		GConcept* Concept;
		GStatement* Statement;
	} Obj;

	tObjType Type;

	QGObject(QTreeWidget* parent,GConceptType* dict)
		: QTreeWidgetItem(parent,QStringList()<<ToQString(dict->GetDescription())<<ToQString(GetConceptCat(dict->GetCategory(),true,false))), Type(otConceptType)
	{
		Obj.Dict=dict;
		//setIcon(0,KIconLoader::global()->loadIcon("dashboard-show",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GConcept* concept,const QString& id)
		: QTreeWidgetItem(parent,QStringList()<<id<<ToQString(concept->GetName())
			<<QString::number(concept->GetIF(otDoc))<<QString::number(concept->GetIF(otProfile))
	        <<QString::number(concept->GetIF(otCommunity))<<QString::number(concept->GetIF(otTopic))
	        <<QString::number(concept->GetIF(otClass))), Type(otConcept)
	{
		Obj.Concept=concept;
		//setIcon(0,KIconLoader::global()->loadIcon("dashboard-show",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GStatement* statement,const QString& str1,const QString& str2,const QString& str3)
		: QTreeWidgetItem(parent,QStringList()<<str1<<str2<<str3), Type(otStatement)
	{
		Obj.Statement=statement;
	}
};



//-----------------------------------------------------------------------------
//
// class QLoadDicts
//
//-----------------------------------------------------------------------------

class QLoadDicts : public QSessionThread
{
private:

	QTreeWidget* Dicts;

public:
	QLoadDicts(KGALILEICenter* app,QTreeWidget* dicts) : QSessionThread(app), Dicts(dicts) {}
	virtual void DoIt(void)
	{
		// Go trough each language and create a Item.
		RCursor<GConceptType> Types(App->getSession()->GetConceptTypes());
		for(Types.Start();!Types.End();Types.Next())
			new QGObject(Dicts,Types());
	}
};



//-----------------------------------------------------------------------------
//
// class KViewDicts
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDicts::KViewDicts(KGALILEICenter* app)
	: QMdiSubWindow(), Ui_KViewDicts(), CurDict(0), App(app)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Dictionaries");
	connect(Dicts,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(selectDict(QTreeWidgetItem*,int)));
	connect(Dict,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(selectConcept(QTreeWidgetItem*,int)));
	connect(NewConcept,SIGNAL(pressed()),this,SLOT(newConcept()));
	connect(DelConcept,SIGNAL(pressed()),this,SLOT(delConcept()));
	connect(NewStatement,SIGNAL(pressed()),this,SLOT(newStatement()));
	connect(DelStatement,SIGNAL(pressed()),this,SLOT(delStatement()));
}


//-----------------------------------------------------------------------------
void KViewDicts::create(void)
{
	QSessionProgressDlg Dlg(App,"Load Dictionaries",true);
	QLoadDicts* Task(new QLoadDicts(App,Dicts));
	Dicts->sortItems(0,Qt::AscendingOrder);
	Dicts->resizeColumnToContents(0);
	Dicts->resizeColumnToContents(1);
	connect(Task,SIGNAL(finish()),this,SLOT(update()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
void KViewDicts::update(void)
{
//	cout<<"Update"<<endl;
}


//-----------------------------------------------------------------------------
void KViewDicts::selectDict(QTreeWidgetItem* item,int)
{
	QGObject* ptr(dynamic_cast<QGObject*>(item));
	if(!ptr)
		return;
	Dict->clear();
	CurDict=ptr->Obj.Dict;

    // Parse the double hash table
    RCursor<RDblHashContainer<GConcept,false>::Hash> Cur(CurDict->GetConcepts());
    for(Cur.Start();!Cur.End();Cur.Next())
    {
       RCursor<RDblHashContainer<GConcept,false>::Hash2> Cur2(*Cur());
       for(Cur2.Start();!Cur2.End();Cur2.Next())
       {
          RCursor<GConcept> Cur3(*Cur2());
          for(Cur3.Start();!Cur3.End();Cur3.Next())
          {
        	  QString w(QString::number(Cur3()->GetId()));
        	  while(w.length()<10)
        		  w.prepend(' ');
        	  new QGObject(Dict,const_cast<GConcept*>(Cur3()),w);
          }
       }
    }
}


////-----------------------------------------------------------------------------
QString KViewDicts::BuildConcept(GConcept* concept)
{
	return("\""+ToQString(concept->GetName())+"\" ("+QString::number(concept->GetId())+","+QString::number(concept->GetType()->GetId())+")");
}


//-----------------------------------------------------------------------------
void KViewDicts::selectConcept(QTreeWidgetItem* item,int)
{
	QGObject* ptr(dynamic_cast<QGObject*>(item));
	if(!ptr)
		return;
	if((!ptr)||(!CurDict))
		return;
//	GConcept* concept(ptr->Obj.Concept);
	Statements->clear();
//	Rels.Clear();
//	GALILEIApp->GetSession()->GetRelations(Rels,concept,cNoRef,0,true);
//	RCursor<GRelation> Cur(Rels);
//	for(Cur.Start();!Cur.End();Cur.Next())
//		new QGObject(Relations,Cur(),
//				 BuildConcept(Cur()->GetSubject()),
//				 ToQString(GALILEIApp->GetSession()->GetPredicate(Cur()->GetType(),false)->GetName()),
//				 BuildConcept(Cur()->GetObject()));
}


//-----------------------------------------------------------------------------
void KViewDicts::newConcept(void)
{
	bool Ok;
	QString text(QInputDialog::getText(this,"New concept", "Enter the name of the concept:",QLineEdit::Normal,QString(),&Ok));
	if(Ok&&!text.isEmpty())
	{
		GConcept concept(App->getSession(),FromQString(text),CurDict);
		GConcept* ptr=App->getSession()->InsertConcept(&concept);
		QString w(QString::number(ptr->GetId()));
		while(w.length()<10)
			w.prepend(' ');
		new QGObject(Dict,ptr,w);
	}
}


//-----------------------------------------------------------------------------
void KViewDicts::delConcept(void)
{
	QGObject* ptr(dynamic_cast<QGObject*>(Dict->currentItem()));
	if(!ptr) return;
	GConcept* concept(ptr->Obj.Concept);
	if(KMessageBox::warningYesNo(this,"Do you want to delete the concept "+BuildConcept(concept)+"?","Warning")==KMessageBox::No)
		return;
	delete ptr;
	App->getSession()->DeleteConcept(concept);
}


//-----------------------------------------------------------------------------
void KViewDicts::newStatement(void)
{
	RToDo("KViewDicts::newStatement(void)");
}


//-----------------------------------------------------------------------------
void KViewDicts::delStatement(void)
{
	QGObject* ptr(dynamic_cast<QGObject*>(Statements->currentItem()));
	if(!ptr) return;
	if(KMessageBox::warningYesNo(this,"Do you want to delete the current statement?","Warning")==KMessageBox::No)
		return;
	RToDo("KViewDicts::delStatement(void)");
}


//-----------------------------------------------------------------------------
KViewDicts::~KViewDicts(void)
{
}
