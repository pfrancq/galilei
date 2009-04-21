/*

	GALILEI Research Project

	QGDocStruct.cpp

	Widget to display the description of an object - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
// include files for R Project
#include <rcursor.h>
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ui_qgdocstruct.h>
#include <qgdocstruct.h>
#include <gdoc.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <ggalileiapp.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGDocStruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGDocStruct::QGDocStruct(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QGDocStruct)
{
	static_cast<Ui_QGDocStruct*>(Ui)->setupUi(this);
}



//------------------------------------------------------------------------------
void QGDocStruct::Set(GDoc* obj)
{
	if(!obj) return;

	// Init
	QTreeWidget* RecsList(static_cast<Ui_QGDocStruct*>(Ui)->RecsList);
	GSession* Session=GALILEIApp->GetSession();
	RecsList->clear();


	// Show the information entities
	QTreeWidgetItem* ptr(0);
	GDocStruct* xml=obj->GetStruct();
	if(!xml)
		return;
	R::RCursor<GVTDRec> Recs(xml->GetRecs());
	for(Recs.Start();!Recs.End();Recs.Next())
	{
		QString name;
		name=ToQString(Session->GetStorage()->LoadConcept(Recs()->GetConcept()->GetId()));
		if(Recs()->GetType()==GVTDRec::Tag)
			name="<"+name+">";
		QString type=ToQString(Recs()->GetConcept()->GetType()->GetDescription());
		QString Child;
		size_t child=xml->GetFirstChild(Recs());
		if(child==SIZE_MAX)
			Child="-1";
		else
			Child.setNum(child);
		ptr=new QTreeWidgetItem(RecsList,QStringList()<<name<<type<<QString::number(Recs()->GetPos())<<QString::number(Recs()->GetDepth())<<Child);
	}
	obj->ReleaseStruct();

	RecsList->resizeColumnToContents(0);
	RecsList->resizeColumnToContents(1);
	RecsList->resizeColumnToContents(2);
	RecsList->resizeColumnToContents(3);
	RecsList->resizeColumnToContents(4);
}


//------------------------------------------------------------------------------
QGDocStruct::~QGDocStruct(void)
{
	delete static_cast<Ui_QGDocStruct*>(Ui);
}
