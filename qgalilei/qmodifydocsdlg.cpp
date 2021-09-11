/*

	KGALILEICenter

	QModifyDocsDlg.cpp

	Modify Documents Dialog - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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




//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>


//------------------------------------------------------------------------------
// includes files for current application
#include <qgalileiwin.h>
#include <qmodifydocsdlg.h>
#include <qgalilei.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <gdoc.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class QModifyDocsDlg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QModifyDocsDlg::QModifyDocsDlg(QGALILEIWin* win)
	: QDialog(win), Ui_QModifyDocsDlg(), Win(win)
{
	setupUi(this);
	setWindowTitle("Modify Documents");
	if(Win->getSession()->GetNbObjs(pDoc))
		Document->setMinimum(1);
	else
		Document->setMinimum(0);
	Document->setMaximum(Win->getSession()->GetMaxObjId(pDoc));
	populateConceptTypes();
   connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	connect(bAdd,SIGNAL(pressed()),this,SLOT(addExpression()));
	connect(bModify,SIGNAL(pressed()),this,SLOT(modifyDescription()));
	connect(Dictionary2,SIGNAL(currentIndexChanged(int)),this,SLOT(populateConcepts(int)));
	connect(Dictionary,SIGNAL(currentIndexChanged(int)),this,SLOT(editConceptType(int)));
	connect(Dictionary,SIGNAL(editTextChanged(const QString&)),this,SLOT(editConceptType(const QString&)));
	adjustSize();
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::populateConceptTypes(void)
{
	EditType=false;
	Dictionary->clear();
	Dictionary2->clear();
	RCursor<GConceptType> Type(Win->getSession()->GetObjs (pConceptType));
	int i;
	for(Type.Start(),i=0;!Type.End();Type.Next(),i++)
	{
		if((Type()->GetName().Mid(2)!="Stopwords")&&(Type()->GetName()!="Tokens"))
			Dictionary->insertItem(i,ToQString(Type()->GetDescription()),QVariant(ToQString(Type()->GetName())));
		Dictionary2->insertItem(i,ToQString(Type()->GetDescription()),QVariant(ToQString(Type()->GetName())));
	}

	// Go through all languages
	RCastCursor<GPlugIn,GLang> cLang(GALILEIApp->GetPlugIns<GLang>("Lang"));
	for(cLang.Start();!cLang.End();cLang.Next())
		Lang->addItem(ToQString(cLang()->GetDesc()),QVariant(ToQString(cLang()->GetName())));
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::populateConcepts(int index)
{
	GConceptType* Type(Win->getSession()->GetObj(pConceptType,FromQString(Dictionary2->itemData(Dictionary2->currentIndex()).toString()),false));
	QApplication::setOverrideCursor(Qt::WaitCursor);
	Concept->clear();
	RCursor<GConcept> Cur(Type->GetObjs(pConcept));
	int i;
	for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
		Concept->insertItem(i,ToQString(Cur()->GetName()));
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::editConceptType(const QString&)
{
	EditType=true;
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::editConceptType(int)
{
	EditType=false;
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::addExpression(void)
{
	if((Dictionary->currentIndex()==-1)||(Expression->text().isEmpty()))
		return;

	// Treat the concept type
	GConceptType* Type(0);
	if(EditType)
	{
		if(QMessageBox::question(this,"Add Concept Type","Insert new type '"+Dictionary->currentText()+"'?",QMessageBox::Ok|QMessageBox::Cancel)!=QMessageBox::Ok)
			return;
		RString Text(FromQString(Dictionary->currentText()));
		Type=Win->getSession()->InsertObj(pConceptType,ccMetadata,cNoRef,Text,Text);
		populateConceptTypes();
	}
	else
		Type=Win->getSession()->GetObj(pConceptType,FromQString(Dictionary->itemData(Dictionary->currentIndex()).toString()),false);

	// Treat the expression
	Lang->currentIndex();
	GLang* pLang(GALILEIApp->GetPlugIn<GLang>("Lang",FromQString(Lang->itemData(Lang->currentIndex()).toString())));
	pLang->CreateExpression(FromQString(Expression->text()),Type);
}


//-----------------------------------------------------------------------------
void QModifyDocsDlg::modifyDescription(void)
{
	if((Concept->currentIndex()==-1)||(Document->value()==0))
		return;
	GConceptType* Type(Win->getSession()->GetObj(pConceptType,FromQString(Dictionary2->itemData(Dictionary2->currentIndex()).toString()),false));
	GConcept* Ptr(Win->getSession()->GetObj(pConcept,Type,FromQString(Concept->currentText()),false));
	GDoc* Doc(Win->getSession()->GetObj(pDoc,Document->value(),false));

	// Modify the vector
	GDescription Desc(Doc->operator()());
	GVector* Vector(Desc.GetInsertVector(Type->GetNeutral()));
	if(Vector->IsIn(Ptr))
		return; // Nothing to do
	Vector->InsertRef(new GConceptRef(Ptr,1.0));

	// Modify the tree
	R::RContainer<GConceptRecord,false,true> Recs(1000);
	size_t NbRecords,NbTopRecords,NbRefs;
	Doc->LoadTree(Recs,NbRecords,NbTopRecords,NbRefs);
	GConceptRecord* Last(Recs[NbRecords-1]);
	Recs.InsertPtrAt(new GConceptRecord(ttMetadata,Ptr->GetId(),Last->GetSyntacticPos()+1,Last->GetPos()+100,0,NbRecords,0),NbRecords);
	NbRecords++;
	NbTopRecords++;
	NbRefs++;

	// Modify the document
	Doc->Update(Doc->GetLang(),Desc,Recs,NbRecords,NbTopRecords,NbRefs);

	// Cleaning stuff
	Recs.Clear(0,0,true);
}
