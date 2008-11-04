/*

	GALILEI Research Project

	QGDocStruct.cpp

	Widget to display the description of an object - Implementation.

	Copyright 2008 by the Université libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rcursor.h>
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <qgdocstruct.h>
#include <gdoc.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <gstorage.h>
#include <gxmlindex.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class QGDocStruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGDocStruct::QGDocStruct(QWidget* parent,GDoc* obj,GSession* session)
	: QListView(parent), Session(session)
{
	setSorting(-1);
	addColumn("Concept");
	addColumn("Concept Type");
	addColumn("Position");
	addColumn("Level");
	addColumn("Child");
	IndexSpace=Session->GetInsertConceptType("XMLIndex","XML Index");
	SetObject(obj);
}



//------------------------------------------------------------------------------
void QGDocStruct::SetObject(GDoc* obj)
{
	clear();
	Object=obj;
	if(!Object) return;

	// Show the information entities
	QListViewItem* ptr(0);
	GDocStruct* xml=Object->GetStruct();
	if(!xml)
		return;
	R::RCursor<GVTDRec> Recs(xml->GetRecs());
	for(Recs.Start();!Recs.End();Recs.Next())
	{
		QString name;
		name=ToQString(Session->GetStorage()->LoadConcept(Recs()->GetConcept()->GetId(),Recs()->GetConcept()->GetType()));
		if(Recs()->GetType()==GVTDRec::Tag)
			name="<"+name+">";
		QString type=ToQString(Recs()->GetConcept()->GetType()->GetDescription());
		QString Child;
		size_t child=xml->GetFirstChild(Recs());
		if(child==SIZE_MAX)
			Child="-1";
		else
			Child.setNum(child);
		ptr=new QListViewItem(this,ptr,name,type,QString::number(Recs()->GetPos()),QString::number(Recs()->GetDepth()),Child);
	}
	Object->ReleaseStruct();
}
