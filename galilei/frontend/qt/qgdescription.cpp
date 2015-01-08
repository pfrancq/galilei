/*

	GALILEI Research Project

	QGDescription.cpp

	Description Widget - Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
// include files for Qt
#include <QtGui/QMessageBox>


//------------------------------------------------------------------------------
// include files for R Project
#include <rcursor.h>
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ui_qgdescription.h>
#include <qgdescription.h>
#include <gsession.h>
#include <gstorage.h>
#include <gconceptlist.h>
#include <ggalileiapp.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class Item
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class Item : public QTreeWidgetItem
{
public:

	const GVector* Vector;

	Item(QTreeWidget* parent,const GVector* vector,const QStringList & strings) : QTreeWidgetItem(parent,strings), Vector(vector) {}


private:

	bool operator<(const QTreeWidgetItem &other) const
	{
		int column(treeWidget()->sortColumn());
		if(column==2)
			return(text(column).toDouble()<other.text(column).toDouble());
		else if(column==3)
			return(text(column).toUInt()<other.text(column).toUInt());
		return(text(column).toLower()<other.text(column).toLower());
	}
};



//------------------------------------------------------------------------------
//
// class QGDescription
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGDescription::QGDescription(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QGDescription())
{
	static_cast<Ui_QGDescription*>(Ui)->setupUi(this);
	connect(static_cast<Ui_QGDescription*>(Ui)->Vectors,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(ChangeVector(QTreeWidgetItem*,int)));
}


//------------------------------------------------------------------------------
void QGDescription::Set(GSession*,const GDescription* desc)
{
	try
	{
		Desc=desc;
		QTreeWidget* Vectors(static_cast<Ui_QGDescription*>(Ui)->Vectors);
		Item* Cur(0);

		// Parser the vectors
		RConstCursor<GVector> Vector(Desc->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			// Create an item
			GConcept* Concept(Vector()->GetMetaConcept());
			Item* The(new Item(Vectors,Vector(),QStringList()
					  <<ToQString(Concept->GetName())
					  <<ToQString(Concept->GetType()->GetName())
					  <<QString::number(Concept->GetId())));
			if(!Cur)
				Cur=The;
		}

		// Resize the columns
		Vectors->sortItems(0,Qt::AscendingOrder);
		Vectors->resizeColumnToContents(0);
		Vectors->resizeColumnToContents(1);
		Vectors->resizeColumnToContents(2);
		Vectors->resizeColumnToContents(3);

		if(Cur)
		{
			Vectors->setCurrentItem(Cur);
			ChangeVector(Cur,0);
		}
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,QWidget::tr("GALILEI Exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
	}
	catch(RException& e)
	{
		QMessageBox::critical(this,QWidget::tr("R Exception"),ToQString(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QMessageBox::critical(this,QWidget::tr("std::Exception"),ToQString(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QMessageBox::critical(this,QWidget::tr("Exception"),ToQString("Undefined"),QMessageBox::Ok);
	}
}


//------------------------------------------------------------------------------
void QGDescription::ChangeVector(QTreeWidgetItem* item,int)
{
	Item* Choice(dynamic_cast<Item*>(item));
	QTreeWidget* Vector(static_cast<Ui_QGDescription*>(Ui)->Vector);
	Vector->clear();

	// Fill the vector
	RConstCursor< GConceptRef> Refs(Choice->Vector->GetRefs());
	for(Refs.Start();!Refs.End();Refs.Next())
	{
			// Create an item
			GConcept* Concept(Refs()->GetConcept());
			new Item(Vector,0,QStringList()
					  <<ToQString(Concept->GetName())
					  <<ToQString(Concept->GetType()->GetName())
					  <<QString::number(Refs()->GetWeight())
					  <<QString::number(Concept->GetId()));
	}

	// Resize the columns
	Vector->sortItems(0,Qt::AscendingOrder);
	Vector->resizeColumnToContents(0);
	Vector->resizeColumnToContents(1);
	Vector->resizeColumnToContents(2);
	Vector->resizeColumnToContents(3);
}


//------------------------------------------------------------------------------
QGDescription::~QGDescription(void)
{
	delete static_cast<Ui_QGDescription*>(Ui);
}
