/*

	KGALILEICenter

	Configure.cpp

	Configure Dialog Box - Implementation.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QMessageBox>
#include <QtGui/QHeaderView>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
#include <gmeasure.h>
#include <gsession.h>
#include <gsubject.h>
#include <gsimulator.h>
#include <rcursor.h>
#include <rnodecursor.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// includes files for current application
#include <qgalilei.h>
#include <qgalileiwin.h>
#include <configure.h>


//------------------------------------------------------------------------------
// Current tabs selected
static int MainTabIdx;
static int PlugInsTabIdx;
static int SimulationTabIdx;
static int DocsTabIdx;
static int ProfilesTabIdx;
static int CommunitiesTabIdx;
static int SearchTabIdx;
static int MeasuresCatIdx;
static int ToolsCatIdx;
static int FunctionsCatIdx;



//------------------------------------------------------------------------------
//
// class QSubjectItem
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class QSubjectItem : public QTreeWidgetItem
{
public:
	GSubject* Subject;
	bool Select;

	void select(bool select)
	{
		if(select)
		{
//			setIcon(0,KIconLoader::global()->loadIcon("dialog-ok-apply.png",KIconLoader::Small));
			Select=true;
		}
		else
		{
//			setIcon(0,KIconLoader::global()->loadIcon("edit-delete.png",KIconLoader::Small));
			Select=false;
		}
	}
	void set(GSubject* subject)
	{
		Subject=subject;
		select(subject->IsUsed());
		setText(1,ToQString(subject->GetName()));
	}

	QSubjectItem(QTreeWidget* parent,GSubject* subject) : QTreeWidgetItem(parent) {set(subject);}
	QSubjectItem(QTreeWidgetItem* parent,GSubject* subject) : QTreeWidgetItem(parent) {set(subject);}
};



//------------------------------------------------------------------------------
//
// class Configure::Type
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Configure::Type::Type(GSession* session,const RString& name)
	: Session(session), Name(name)
{
	// Descriptions
	DescBlock=Session->GetUInt("BlockSize","Indexer",Name,"Description");
	DescTolerance=Session->GetUInt("Tolerance","Indexer",Name,"Description");
	DescCache=Session->GetUInt("CacheSize","Indexer",Name,"Description");
	DescType=Session->GetUInt("Type","Indexer",Name,"Description");

	// Index
	CreateIndex=Session->GetBool("CreateIndex","Indexer",Name,"Index");
	IndexBlock=Session->GetUInt("BlockSize","Indexer",Name,"Index");
	IndexTolerance=Session->GetUInt("Tolerance","Indexer",Name,"Index");
	IndexCache=Session->GetUInt("CacheSize","Indexer",Name,"Index");
	IndexType=Session->GetUInt("Type","Indexer",Name,"Index");

	// Structure (only for documents)
	if(Name=="Documents")
	{
		CreateTree=Session->GetBool("CreateTree","Indexer","Documents","Tree");
		TreeBlock=Session->GetUInt("BlockSize","Indexer","Documents","Tree");
		TreeTolerance=Session->GetUInt("Tolerance","Indexer","Documents","Tree");
		TreeCache=Session->GetUInt("CacheSize","Indexer","Documents","Tree");
		TreeType=Session->GetUInt("Type","Indexer","Documents","Tree");
	}
}


//------------------------------------------------------------------------------
void Configure::Type::Apply(void)
{
	// Descriptions
	Session->SetUInt("BlockSize",DescBlock,"Indexer",Name,"Description");
	Session->SetUInt("Tolerance",DescTolerance,"Indexer",Name,"Description");
	Session->SetUInt("CacheSize",DescCache,"Indexer",Name,"Description");
	Session->SetUInt("Type",DescType,"Indexer",Name,"Description");

	// Index
	Session->SetBool("CreateIndex",CreateIndex,"Indexer",Name,"Index");
	Session->SetUInt("BlockSize",IndexBlock,"Indexer",Name,"Index");
	Session->SetUInt("Tolerance",IndexTolerance,"Indexer",Name,"Index");
	Session->SetUInt("CacheSize",IndexCache,"Indexer",Name,"Index");
	Session->SetUInt("Type",IndexType,"Indexer",Name,"Index");

	// Structure (only for documents)
	if(Name=="Documents")
	{
		Session->SetBool("CreateTree",CreateTree,"Indexer","Documents","Tree");
		Session->SetUInt("BlockSize",TreeBlock,"Indexer","Documents","Tree");
		Session->SetUInt("Tolerance",TreeTolerance,"Indexer","Documents","Tree");
		Session->SetUInt("CacheSize",TreeCache,"Indexer","Documents","Tree");
		Session->SetInt("Type",TreeType,"Indexer","Documents","Tree");
	}
}



//------------------------------------------------------------------------------
//
// class Configure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Configure::Configure(QGALILEIWin* win)
	: QDialog(win), Ui_Configure(), Types(10), CurType(0), Win(win)
{
	setupUi(this);
	setWindowTitle("Preferences");
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
	adjustSize();
}


//------------------------------------------------------------------------------
void Configure::exec(void)
{
	initSession();
	initSimulation();
	initPlugIns();

	MainTab->setCurrentIndex(MainTabIdx);
	PlugInsTab->setCurrentIndex(PlugInsTabIdx);
	DocsTab->setCurrentIndex(DocsTabIdx);
	CommunitiesTab->setCurrentIndex(CommunitiesTabIdx);
	SearchTab->setCurrentIndex(SearchTabIdx);
	SimulationTab->setCurrentIndex(SimulationTabIdx);
	MeasuresCats->setCurrentRow(MeasuresCatIdx);
	ToolsCats->setCurrentRow(ToolsCatIdx);
	FunctionsCats->setCurrentRow(FunctionsCatIdx);

	if(QDialog::exec())
	{
		applySession();
		applySimulation();
		applyPlugIns();
		if(Win->getSession())
			Win->getSession()->ApplyConfig();
	}

	MainTabIdx=MainTab->currentIndex();
	PlugInsTabIdx=PlugInsTab->currentIndex();
	DocsTabIdx=DocsTab->currentIndex();
	CommunitiesTabIdx=CommunitiesTab->currentIndex();
	SearchTabIdx=SearchTab->currentIndex();
	SimulationTabIdx=SimulationTab->currentIndex();
	MeasuresCatIdx=MeasuresCats->currentRow();
	ToolsCatIdx=ToolsCats->currentRow();
	FunctionsCatIdx=FunctionsCats->currentRow();
}


//------------------------------------------------------------------------------
void Configure::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("MainTabIdx",0));
	config.InsertParam(new RParamValue("PlugInsTabIdx",0));
	config.InsertParam(new RParamValue("DocsTabIdx",0));
	config.InsertParam(new RParamValue("ProfilesTabIdx",0));
	config.InsertParam(new RParamValue("CommunitiesTabIdx",0));
	config.InsertParam(new RParamValue("SearchTabIdx",0));
	config.InsertParam(new RParamValue("SimulationTabIdx",0));
	config.InsertParam(new RParamValue("MeasuresCatIdx",0));
	config.InsertParam(new RParamValue("ToolsCatIdx",0));
	config.InsertParam(new RParamValue("FunctionsCatIdx",0));
}


//------------------------------------------------------------------------------
void Configure::readOptions(RConfig& config)
{
	MainTabIdx=config.GetInt("MainTabIdx");
	PlugInsTabIdx=config.GetInt("PlugInsTabIdx");
	DocsTabIdx=config.GetInt("DocsTabIdx");
	ProfilesTabIdx=config.GetInt("ProfilesTabIdx");
	CommunitiesTabIdx=config.GetInt("CommunitiesTabIdx");
	SearchTabIdx=config.GetInt("SearchTabIdx");
	SimulationTabIdx=config.GetInt("SimulationTabIdx");
	MeasuresCatIdx=config.GetInt("MeasuresCatIdx");
	ToolsCatIdx=config.GetInt("ToolsCatIdx");
	FunctionsCatIdx=config.GetInt("FunctionsCatIdx");
}


//------------------------------------------------------------------------------
void Configure::saveOptions(RConfig& config)
{
	config.SetInt("MainTabIdx",MainTabIdx);
	config.SetInt("PlugInsTabIdx",PlugInsTabIdx);
	config.SetInt("DocsTabIdx",DocsTabIdx);
	config.SetInt("ProfilesTabIdx",ProfilesTabIdx);
	config.SetInt("CommunitiesTabIdx",CommunitiesTabIdx);
	config.SetInt("SearchTabIdx",SearchTabIdx);
	config.SetInt("SimulationTabIdx",SimulationTabIdx);
	config.SetInt("MeasuresCatIdx",MeasuresCatIdx);
	config.SetInt("ToolsCatIdx",ToolsCatIdx);
	config.SetInt("FunctionsCatIdx",FunctionsCatIdx);
}


//------------------------------------------------------------------------------
void Configure::addSubject(GSubject* subject,QTreeWidgetItem* parent)
{
	QSubjectItem* item;
	if(parent)
		item=new QSubjectItem(parent,subject);
	else
		item=new QSubjectItem(Subjects,subject);

	// Child subjects
 	RNodeCursor<GSubjects,GSubject> Cur(subject);
 	for(Cur.Start();!Cur.End();Cur.Next())
 		addSubject(Cur(),item);
}


//------------------------------------------------------------------------------
void Configure::initSession(void)
{
	// Indexer
	connect(Objects,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(objectChanged(const QString&)));
	RContainer<RString,true,false> TypesNames(10);
	Win->getSession()->GetCategories(TypesNames,"Indexer");
	RCursor<RString> Cur(TypesNames);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Objects->addItem(ToQString(*Cur()));
		Types.InsertPtr(new Type(Win->getSession(),*Cur()));
	}
	Objects->setCurrentIndex(1);
}


//------------------------------------------------------------------------------
void Configure::applySession(void)
{
	// Indexer
	objectChanged(Objects->currentText());
	RCursor<Type> Cur(Types);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Apply();
}


//------------------------------------------------------------------------------
void Configure::initSimulation(void)
{
	// Read Values
	NbOK->setValue(Win->getSession()->GetDouble("Nb","Simulator","Profiles","Relevant"));
	RelOK->setChecked(Win->getSession()->GetBool("Percentage","Simulator","Profiles","Relevant"));
	NbKO->setValue(Win->getSession()->GetDouble("Nb","Simulator","Profiles","Fuzzy Relevant"));
	RelKO->setChecked(Win->getSession()->GetBool("Percentage","Simulator","Profiles","Fuzzy Relevant"));
	NbH->setValue(Win->getSession()->GetDouble("Nb","Simulator","Profiles","Irrelevant"));
	RelH->setChecked(Win->getSession()->GetBool("Percentage","Simulator","Profiles","Irrelevant"));
	PercErr->setValue(Win->getSession()->GetDouble("PercErr","Simulator","Profiles"));
	NbProfMin->setValue(Win->getSession()->GetInt("NbProfMin","Simulator","Profiles"));
	NbProfMax->setValue(Win->getSession()->GetInt("NbProfMax","Simulator","Profiles"));
	MaxDepth->setValue(Win->getSession()->GetInt("MaxDepth","Simulator","Subjects"));
	PercSocial->setValue(Win->getSession()->GetDouble("PercSocial","Simulator","Profiles"));
	NbSubjects->setValue(Win->getSession()->GetDouble("Nb","Simulator","Subjects","NbSubjects"));
	RelSubjects->setChecked(Win->getSession()->GetBool("Percentage","Simulator","Subjects","NbSubjects"));
	NbMinDocsSubject->setValue(Win->getSession()->GetInt("NbMinDocsSubject","Simulator","Subjects"));
	NbDocsAssess->setValue(Win->getSession()->GetInt("NbDocsAssess","Simulator","Profiles"));
	SwitchPerc->setValue(Win->getSession()->GetDouble("SwitchPerc","Simulator","Documents"));
	ManualSubjects->setChecked(Win->getSession()->GetBool("ManualSubjects","Simulator","Subjects"));
	NbDocsPerSubject->setValue(Win->getSession()->GetDouble("Nb","Simulator","Documents","DocsPerSubject"));
	PercNbDocsPerSubject->setChecked(Win->getSession()->GetBool("Percentage","Simulator","Documents","DocsPerSubject"));
	ClusterSelectedDocs->setChecked(Win->getSession()->GetBool("ClusterSelectedDocs","Simulator","Documents"));
	MultipleSubjects->setChecked(Win->getSession()->GetBool("MultipleSubjects","Simulator","Documents"));
	CreateProfiles->setChecked(Win->getSession()->GetBool("CreateProfiles","Simulator","Profiles"));
	grpProfilesCreated->setEnabled(CreateProfiles->isChecked());
	grpPercentage->setEnabled(CreateProfiles->isChecked());
	grpType->setEnabled(CreateProfiles->isChecked());


	// Set subjects
	MaxDepth->setMaximum(Win->getSession()->GetMaxDepth());
	Depth->setText(QString::number(Win->getSession()->GetMaxDepth()));
	Subjects->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	Subjects->header()->setResizeMode(1,QHeaderView::Stretch);
	connect(Subjects,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(subjectClicked(QTreeWidgetItem*,int)));
	RNodeCursor<GSubjects,GSubject> Cur(Win->getSession()->GetObjs(pSubject,pSubject));
	for(Cur.Start();!Cur.End();Cur.Next())
		addSubject(Cur(),0);
	Subjects->setEnabled(ManualSubjects->isChecked());
}


//------------------------------------------------------------------------------
void Configure::applySubSubjects(QTreeWidgetItem* parent)
{
	for(int i=0;i<parent->childCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(parent->child(i)));
		if(!item) continue;
		if(item->Select)
			Win->getSession()->AddToList("SelectedSubjects",FromQString(item->text(1)),"Simulator");
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	Win->getSession()->SetDouble("Nb",NbOK->value(),"Simulator","Profiles","Relevant");
	Win->getSession()->SetBool("Percentage",RelOK->isChecked(),"Simulator","Profiles","Relevant");
	Win->getSession()->SetDouble("Nb",NbKO->value(),"Simulator","Profiles","Fuzzy Relevant");
	Win->getSession()->SetBool("Percentage",RelKO->isChecked(),"Simulator","Profiles","Fuzzy Relevant");
	Win->getSession()->SetDouble("Nb",NbH->value(),"Simulator","Profiles","Irrelevant");
	Win->getSession()->SetBool("Percentage",RelH->isChecked(),"Simulator","Profiles","Irrelevant");
	Win->getSession()->SetDouble("PercErr",PercErr->value(),"Simulator","Profiles");
	Win->getSession()->SetUInt("NbProfMin",NbProfMin->value(),"Simulator","Profiles");
	Win->getSession()->SetUInt("NbProfMax",NbProfMax->value(),"Simulator","Profiles");
	Win->getSession()->SetUInt("MaxDepth",MaxDepth->value(),"Simulator","Subjects");
	Win->getSession()->SetDouble("PercSocial",PercSocial->value(),"Simulator","Profiles");
	Win->getSession()->SetDouble("Nb",NbSubjects->value(),"Simulator","Subjects","NbSubjects");
	Win->getSession()->SetBool("Percentage",RelSubjects->isChecked(),"Simulator","Subjects","NbSubjects");
	Win->getSession()->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Simulator","Subjects");
	Win->getSession()->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Simulator","Profiles");
	Win->getSession()->SetDouble("SwitchPerc",SwitchPerc->value(),"Simulator","Documents");
	Win->getSession()->SetBool("ManualSubjects",ManualSubjects->isChecked(),"Simulator","Subjects");
	Win->getSession()->SetDouble("Nb",NbDocsPerSubject->value(),"Simulator","Documents","DocsPerSubject");
	Win->getSession()->SetBool("Percentage",PercNbDocsPerSubject->isChecked(),"Simulator","Documents","DocsPerSubject");
	Win->getSession()->SetBool("ClusterSelectedDocs",ClusterSelectedDocs->isChecked(),"Simulator","Documents");
	Win->getSession()->SetBool("MultipleSubjects",MultipleSubjects->isChecked(),"Simulator","Documents");
	Win->getSession()->SetBool("CreateProfiles",CreateProfiles->isChecked(),"Simulator","Profiles");

	// Apply subjects
	if(!Win->getSession())
		return;
	for(int i=0;i<Subjects->topLevelItemCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(Subjects->topLevelItem(i)));
		if(!item) continue;
		Win->getSession()->SetUsed(item->Subject,item->Select);
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::initPlugIns(void)
{
	// Sort POST_X Managers;
	GALILEIApp->GetManager("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager("ComputeTrust")->ReOrder();

	// Goes through managers
	Storages->init(QPlugInsList::Storages);
	Filters->init(QPlugInsList::Filters);
	Langs->init(QPlugInsList::Langs);
	ProfileCalcs->init(QPlugInsList::ProfileCalcs);
	GroupProfiles->init(QPlugInsList::GroupProfiles);
	GroupDocs->init(QPlugInsList::GroupDocs);
	CommunityCalcs->init(QPlugInsList::CommunityCalcs);
	TopicCalcs->init(QPlugInsList::TopicCalcs);
	LinkCalcs->init(QPlugInsList::LinkCalcs);
	Tokenizers->init(QPlugInsList::Tokenizers);
	Analyzers->init(QPlugInsList::Analyzers);
	Engines->init(QPlugInsList::Engines);
	MetaEngines->init(QPlugInsList::MetaEngines);
	ComputeSugs->init(QPlugInsList::ComputeSugs);
	ComputeTrust->init(QPlugInsList::ComputeTrust);

	// Init Measures
	GPlugInManager* Manager(GALILEIApp->GetManager("Measures"));
	RCursor<GPlugInList> Cur(Manager->GetPlugInLists());
	int row;
	Measures->removeWidget(Measures->widget(0));
	for(Cur.Start(),row=0;!Cur.End();Cur.Next(),row++)
	{
		QListWidgetItem* item=new QListWidgetItem();
		item->setText(ToQString(Cur()->GetName()));
		MeasuresCats->insertItem(row,item);
		QPlugInsList* ptr=new QPlugInsList(this);
		Measures->insertWidget(row,ptr);
		ptr->init(QPlugInsList::Measures,Cur()->GetName());
	}
	MeasuresCats->setCurrentRow(0);

	// Init Tools
	GPlugInManager* Manager2(GALILEIApp->GetManager("Tools"));
	RCursor<GPlugInList> Cur2(Manager2->GetPlugInLists());
	int row2;
	Tools->removeWidget(Tools->widget(0));
	for(Cur2.Start(),row2=0;!Cur2.End();Cur2.Next(),row2++)
	{
		QListWidgetItem* item=new QListWidgetItem();
		item->setText(ToQString(Cur2()->GetName()));
		ToolsCats->insertItem(row,item);
		QPlugInsList* ptr=new QPlugInsList(this);
		Tools->insertWidget(row,ptr);
		ptr->init(QPlugInsList::Tools,Cur2()->GetName());
	}
	ToolsCats->setCurrentRow(0);

		// Init Functions
	GPlugInManager* Manager3(GALILEIApp->GetManager("Functions"));
	RCursor<GPlugInList> Cur3(Manager3->GetPlugInLists());
	int row3;
	Functions->removeWidget(Functions->widget(0));
	for(Cur3.Start(),row3=0;!Cur3.End();Cur3.Next(),row3++)
	{
		QListWidgetItem* item=new QListWidgetItem();
		item->setText(ToQString(Cur3()->GetName()));
		FunctionsCats->insertItem(row,item);
		QPlugInsList* ptr=new QPlugInsList(this);
		Functions->insertWidget(row,ptr);
		ptr->init(QPlugInsList::Functions,Cur3()->GetName());
	}
	FunctionsCats->setCurrentRow(0);
}


//-----------------------------------------------------------------------------
void Configure::applyPlugIns(void)
{
	// Apply plug-ins
	Storages->apply(Win->getSession());
	Filters->apply(Win->getSession());
	ProfileCalcs->apply(Win->getSession());
	GroupProfiles->apply(Win->getSession());
	GroupDocs->apply(Win->getSession());
	CommunityCalcs->apply(Win->getSession());
	TopicCalcs->apply(Win->getSession());
	LinkCalcs->apply(Win->getSession());
	Tokenizers->apply(Win->getSession());
	Analyzers->apply(Win->getSession());
	Engines->apply(Win->getSession());
	MetaEngines->apply(Win->getSession());
	Langs->apply(Win->getSession());
	ComputeSugs->apply(Win->getSession());
	ComputeTrust->apply(Win->getSession());

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply(Win->getSession());

	// Apply Tools
	for(int row=0;row<Tools->count();row++)
		dynamic_cast<QPlugInsList*>(Tools->widget(row))->apply(Win->getSession());

	// Apply Functions
	for(int row=0;row<Functions->count();row++)
		dynamic_cast<QPlugInsList*>(Functions->widget(row))->apply(Win->getSession());

	// Sort POST_X Managers;
	GALILEIApp->GetManager("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager("ComputeTrust")->ReOrder();
}


//-----------------------------------------------------------------------------
void Configure::subjectClicked(QTreeWidgetItem* item, int column)
{
	QSubjectItem* Subject(dynamic_cast<QSubjectItem*>(item));
	if(!item) return;
	if(column==0)
	{
		Subject->select(!Subject->Select);
	}
	else
	{
		Subjects->expandItem(item);
	}
}


//-----------------------------------------------------------------------------
void Configure::objectChanged(const QString& obj)
{
	// Save the current elements
	if(CurType)
	{
		CurType->DescBlock=DescBlock->value();
		CurType->DescTolerance=DescTolerance->value();
		CurType->DescCache=DescCache->value();
		CurType->DescType=DescType->currentIndex();
		CurType->IndexBlock=IndexBlock->value();
		CurType->IndexTolerance=IndexTolerance->value();
		CurType->IndexCache=IndexCache->value();
		CurType->IndexType=IndexType->currentIndex();
		CurType->CreateIndex=CreateIndex->isChecked();
		CurType->CreateTree=CreateTree->isChecked();
		CurType->TreeBlock=TreeBlock->value();
		CurType->TreeTolerance=TreeTolerance->value();
		CurType->TreeCache=TreeCache->value();
		CurType->TreeType=TreeType->currentIndex();
	}

	// Set the current element
	CurType=Types.GetPtr(FromQString(obj));
	if(!CurType)
	{
		Tree->setVisible(false);
		return;
	}
	DescBlock->setValue(CurType->DescBlock);
	DescTolerance->setValue(CurType->DescTolerance);
	DescCache->setValue(CurType->DescCache);
	DescType->setCurrentIndex(CurType->DescType);
	IndexBlock->setValue(CurType->IndexBlock);
	IndexTolerance->setValue(CurType->IndexTolerance);
	IndexCache->setValue(CurType->IndexCache);
	IndexType->setCurrentIndex(CurType->IndexType);
	CreateIndex->setChecked(CurType->CreateIndex);

	if(CurType->Name=="Documents")
	{
		Tree->setVisible(true);
		CreateTree->setChecked(CurType->CreateTree);
		TreeBlock->setValue(CurType->TreeBlock);
		TreeTolerance->setValue(CurType->TreeTolerance);
		TreeCache->setValue(CurType->TreeCache);
		TreeType->setCurrentIndex(CurType->TreeType);
	}
	else
		Tree->setVisible(false);
}
