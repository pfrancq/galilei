/*

	GALILEI Research Project

	KViewDicts.cpp

	Window to show all the dictionaries - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rqt.h>
#include <gconcept.h>
#include <gstatement.h>
#include <ggalileiapp.h>
#include <gsession.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QInputDialog>
#include <QMessageBox>
#include <QResizeEvent>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewdicts.h>
#include <qgalileiwin.h>
#include <qsessionprogress.h>


//-----------------------------------------------------------------------------
// static sizes
static int Width;
static int Height;



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

	QGObject(QTreeWidget* parent,GConcept* concept,double docs,double profiles,double topics,double classes,double communities,const QString& id)
		: QTreeWidgetItem(parent,QStringList()<<id<<ToQString(concept->GetName())
			<<QString::number(docs)<<QString::number(profiles)<<QString::number(communities)<<QString::number(topics)<<QString::number(classes)), Type(otConcept)
	{
		Obj.Concept=concept;
		//setIcon(0,KIconLoader::global()->loadIcon("dashboard-show",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GConcept* concept,const QString& id)
		: QTreeWidgetItem(parent,QStringList()<<id<<ToQString(concept->GetName())
			<<"No Info"<<"No Info"<<"No Info"<<"No Info"<<"No Info"), Type(otConcept)
	{
		Obj.Concept=concept;
		//setIcon(0,KIconLoader::global()->loadIcon("dashboard-show",KIconLoader::Small));
	}

	QGObject(QTreeWidget* parent,GStatement* statement,const QString& subject,const QString& predicate,const QString& value)
		: QTreeWidgetItem(parent,QStringList()<<QString::number(statement->GetId())<<subject<<predicate<<value<<QString::number(statement->GetWeight())), Type(otStatement)
	{
		Obj.Statement=statement;
	}

private:

	bool operator<(const QTreeWidgetItem &other) const
	{
		switch(Type)
		{
			case otConceptType:
			{
				int column(treeWidget()->sortColumn());
				return(text(column).toLower()<other.text(column).toLower());
			}
			case otConcept:
			{
				int column(treeWidget()->sortColumn());
				if(column!=1)
					return(text(column).toUInt()<other.text(column).toUInt());
				return(text(column).toLower()<other.text(column).toLower());
			}
			case otStatement:
			{
				int column(treeWidget()->sortColumn());
				if(column==4)
					return(text(column).toDouble()<other.text(column).toDouble());
				else if(column==0)
					return(text(column).toUInt()<other.text(column).toUInt());
				return(text(column).toLower()<other.text(column).toLower());
			}

			default:
				return(text(0).toLower()<other.text(0).toLower());
		}
	}
};



//-----------------------------------------------------------------------------
//
// class QLoadDicts
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QLoadDicts : public QSessionProgress
{
private:

	QTreeWidget* Dicts;

public:
	QLoadDicts(QGALILEIWin* win,QTreeWidget* dicts) : QSessionProgress(win,"Load dictionnaries"), Dicts(dicts) {}
	virtual void DoIt(void)
	{
		// Go trough each language and create a Item.
		RCursor<GConceptType> Types(Win->getSession()->GetObjs(pConceptType));
		for(Types.Start();!Types.End();Types.Next())
		{
			new QGObject(Dicts,Types());
			WriteStr(Types()->GetDescription());
		}
	}
};



//-----------------------------------------------------------------------------
//
// class KViewDicts
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDicts::KViewDicts(QGALILEIWin* win)
	: QMdiSubWindow(), Ui_KViewDicts(), CurDict(0), Win(win)
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
	resize(Width,Height);
}


//-----------------------------------------------------------------------------
void KViewDicts::create(void)
{
//	QSessionProgressDlg Dlg(App,"Load Dictionaries",true);
	QLoadDicts Task(Win,Dicts);
	Dicts->sortItems(0,Qt::AscendingOrder);
	Dicts->resizeColumnToContents(0);
	Dicts->resizeColumnToContents(1);
	//connect(&Task,SIGNAL(finish()),this,SLOT(update()));
	QSessionProgress::execute(Task);
	update();
}


//------------------------------------------------------------------------------
void KViewDicts::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewDicts::Width",400));
	config.InsertParam(new RParamValue("KViewDicts::Height",300));
}


//------------------------------------------------------------------------------
void KViewDicts::readOptions(RConfig& config)
{
	Width=config.GetInt("KViewDicts::Width");
	Height=config.GetInt("KViewDicts::Height");
}


//------------------------------------------------------------------------------
void KViewDicts::saveOptions(RConfig& config)
{
	config.SetInt("KViewDicts::Width",Width);
	config.SetInt("KViewDicts::Height",Height);
}


//------------------------------------------------------------------------------
void KViewDicts::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	Width=resizeEvent->size().width();
	Height=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewDicts::update(void)
{
//	cout<<"Update"<<endl;
}


//-----------------------------------------------------------------------------
void KViewDicts::selectDict(QTreeWidgetItem* item,int)
{
	GMeasure* Weighting(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0));
	QGObject* ptr(dynamic_cast<QGObject*>(item));
	if(!ptr)
		return;

	QApplication::setOverrideCursor(Qt::WaitCursor);
	Dict->clear();
	CurDict=ptr->Obj.Dict;

    // Parse the double hash table
	 RCursor<GConcept> Cur(CurDict->GetObjs(pConcept));
	 for(Cur.Start();!Cur.End();Cur.Next())
	 {
		QString w(QString::number(Cur()->GetId()));
		while(w.length()<10)
			w.prepend(' ');
		if(Weighting)
		{
			double docs(NAN),profiles(NAN),communities(NAN),topics(NAN),classes(NAN);
			Weighting->Measure(0,const_cast<GConcept*>(Cur()),otDoc,&docs);
			Weighting->Measure(0,const_cast<GConcept*>(Cur()),otProfile,&profiles);
			Weighting->Measure(0,const_cast<GConcept*>(Cur()),otTopic,&topics);
			Weighting->Measure(0,const_cast<GConcept*>(Cur()),otClass,&classes);
			Weighting->Measure(0,const_cast<GConcept*>(Cur()),otCommunity,&communities);
			new QGObject(Dict,const_cast<GConcept*>(Cur()),docs,profiles,topics,classes,communities,w);
		}
		else
			new QGObject(Dict,const_cast<GConcept*>(Cur()),w);
    }

	 QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
QString KViewDicts::buildConcept(GConcept* concept)
{
	return("\""+ToQString(concept->GetName())+"\" ("+QString::number(concept->GetId())+","+QString::number(concept->GetType()->GetId())+")");
}


//-----------------------------------------------------------------------------
QString KViewDicts::buildObject(GObject* obj)
{
	return("\""+ToQString(obj->GetName())+"\" ("+QString::number(obj->GetId())+","+ToQString(GetObjType(obj->GetObjType(),false,false))+")");
}


//-----------------------------------------------------------------------------
void KViewDicts::buildStatements(GConcept* concept,tStatementElement what)
{
	RCursor<GStatement> Statement(concept->GetObjs(pStatement,what));
	for(Statement.Start();!Statement.End();Statement.Next())
	{
		new QGObject(Statements,Statement(),buildObject(Statement()->GetSubject()),buildObject(Statement()->GetPredicate()),buildObject(Statement()->GetValue()));
	}
}


//-----------------------------------------------------------------------------
void KViewDicts::selectConcept(QTreeWidgetItem* item,int)
{
	QGObject* ptr(dynamic_cast<QGObject*>(item));
	if(!ptr)
		return;
	if((!ptr)||(!CurDict))
		return;

	QApplication::setOverrideCursor(Qt::WaitCursor);
	GConcept* concept(ptr->Obj.Concept);

	// Show the corresponding statements
	Statements->clear();
	buildStatements(concept,seSubject);
	buildStatements(concept,sePredicate);
	buildStatements(concept,seValue);

	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void KViewDicts::newConcept(void)
{
	bool Ok;
	QString text(QInputDialog::getText(this,"New concept", "Enter the name of the concept:",QLineEdit::Normal,QString(),&Ok));
	if(Ok&&!text.isEmpty())
	{
		GConcept concept(Win->getSession(),FromQString(text),CurDict);
		GConcept* ptr=Win->getSession()->InsertObj(concept);
		QString w(QString::number(ptr->GetId()));
		while(w.length()<10)
			w.prepend(' ');
		new QGObject(Dict,ptr,0.0,0.0,0.0,0.0,0.0,w);
	}
}


//-----------------------------------------------------------------------------
void KViewDicts::delConcept(void)
{
	QGObject* ptr(dynamic_cast<QGObject*>(Dict->currentItem()));
	if(!ptr) return;
	GConcept* concept(ptr->Obj.Concept);
	if(QMessageBox::warning(this,tr("QGALILEI"),"Do you want to delete the concept "+buildConcept(concept)+"?",QMessageBox::Yes|QMessageBox::Cancel)==QMessageBox::Cancel)
	  return;
	delete ptr;
	Win->getSession()->DeleteObj(concept);
}


//-----------------------------------------------------------------------------
void KViewDicts::newStatement(void)
{
	mToDo("KViewDicts::newStatement(void)");
}


//-----------------------------------------------------------------------------
void KViewDicts::delStatement(void)
{
	QGObject* ptr(dynamic_cast<QGObject*>(Statements->currentItem()));
	if(!ptr) return;
	if(QMessageBox::warning(this,tr("QGALILEI"),"Do you want to delete the current statement?",QMessageBox::Yes|QMessageBox::Cancel)==QMessageBox::Cancel)
	  return;
	mToDo("KViewDicts::delStatement(void)");
}


//-----------------------------------------------------------------------------
KViewDicts::~KViewDicts(void)
{
}
