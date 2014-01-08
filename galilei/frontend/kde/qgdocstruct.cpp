/*

	GALILEI Research Project

	QGDocStruct.cpp

	Widget to display the description of an object - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
#include <rnodecursor.h>
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

#include "gconceptnode.h"
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
void QGDocStruct::Display(GSession* session,GConceptNode* node)
{
	QTreeWidget* RecsList(static_cast<Ui_QGDocStruct*>(Ui)->RecsList);
	QString name(ToQString(session->GetStorage()->LoadConcept(node->GetConceptId())));
	QString ConceptType(ToQString(session->GetConcept(node->GetConceptId())->GetType()->GetDescription()));
	QString TokenType;
	switch(node->GetType())
	{
		case ttUnknown:
			TokenType="Unknown";
			break;
		case ttDeleted:
			TokenType="Deleted";
		case ttText:
			TokenType="Text";
			break;
		case ttSemantic:
			TokenType="Semantic";
			break;
		case ttDivision:
			TokenType="Division";
			break;
		case ttMetadata:
			TokenType="Metadata";
			break;
		case ttAttribute:
			TokenType="Attribute";
			break;
		case ttLink:
			TokenType="Link";
			break;
	}
	new QTreeWidgetItem(RecsList,QStringList()<<name<<TokenType<<ConceptType<<QString::number(node->GetPos())<<QString::number(node->GetSyntacticPos())<<QString::number(node->GetSyntacticDepth()));

	RNodeCursor<GConceptTree,GConceptNode> Node(node);
	for(Node.Start();!Node.End();Node.Next())
		Display(session,Node());
}


//------------------------------------------------------------------------------
void QGDocStruct::Set(GDoc* obj)
{
	if(!obj) return;

	// Init
	GSession* Session=obj->GetSession();
	QTreeWidget* RecsList(static_cast<Ui_QGDocStruct*>(Ui)->RecsList);
	RecsList->clear();

	// Show the tree
	RNodeCursor<GConceptTree,GConceptNode> Node(*obj->GetTree());
	for(Node.Start();!Node.End();Node.Next())
		Display(Session,Node());
	obj->ReleaseTree();

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
