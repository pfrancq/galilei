/*

	KGALILEICenter

	Configure.cpp

	Configure Dialog Box - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
// include files for Qt/KDE
#include <kfiledialog.h>
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
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// includes files for current application
#include <kgalileicenter.h>
#include <configure.h>
#include <kapplication.h>


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
			setIcon(0,KIconLoader::global()->loadIcon("dialog-ok-apply.png",KIconLoader::Small));
			Select=true;
		}
		else
		{
			setIcon(0,KIconLoader::global()->loadIcon("edit-delete.png",KIconLoader::Small));
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
Configure::Type::Type(const RString& name)
	: Name(name)
{
	DescBlock=KGALILEICenter::App->getSession()->GetUInt("BlockSize","Indexer",Name,"Description");
	DescTolerance=KGALILEICenter::App->getSession()->GetUInt("Tolerance","Indexer",Name,"Description");
	DescCache=KGALILEICenter::App->getSession()->GetUInt("CacheSize","Indexer",Name,"Description");
	DescType=KGALILEICenter::App->getSession()->GetInt("Type","Indexer",Name,"Description");
	IndexBlock=KGALILEICenter::App->getSession()->GetUInt("BlockSize","Indexer",Name,"Index");
	IndexTolerance=KGALILEICenter::App->getSession()->GetUInt("Tolerance","Indexer",Name,"Index");
	IndexCache=KGALILEICenter::App->getSession()->GetUInt("CacheSize","Indexer",Name,"Index");
	IndexType=KGALILEICenter::App->getSession()->GetInt("Type","Indexer",Name,"Index");
	IndexInc=KGALILEICenter::App->getSession()->GetBool("Increment","Indexer",Name,"Index");
	if(Name=="Documents")
	{
		StructBlock=KGALILEICenter::App->getSession()->GetUInt("BlockSize","Indexer","Documents","Structure");
		StructTolerance=KGALILEICenter::App->getSession()->GetUInt("Tolerance","Indexer","Documents","Structure");
		StructCache=KGALILEICenter::App->getSession()->GetUInt("CacheSize","Indexer","Documents","Structure");
		StructType=KGALILEICenter::App->getSession()->GetInt("Type","Indexer","Documents","Structure");
	}
}


//------------------------------------------------------------------------------
void Configure::Type::Apply(void)
{
	KGALILEICenter::App->getSession()->SetUInt("BlockSize",DescBlock,"Indexer",Name,"Description");
	KGALILEICenter::App->getSession()->SetUInt("Tolerance",DescTolerance,"Indexer",Name,"Description");
	KGALILEICenter::App->getSession()->SetUInt("CacheSize",DescCache,"Indexer",Name,"Description");
	KGALILEICenter::App->getSession()->SetInt("Type",DescType,"Indexer",Name,"Description");
	KGALILEICenter::App->getSession()->SetUInt("BlockSize",IndexBlock,"Indexer",Name,"Index");
	KGALILEICenter::App->getSession()->SetUInt("Tolerance",IndexTolerance,"Indexer",Name,"Index");
	KGALILEICenter::App->getSession()->SetUInt("CacheSize",IndexCache,"Indexer",Name,"Index");
	KGALILEICenter::App->getSession()->SetInt("Type",IndexType,"Indexer",Name,"Index");
	KGALILEICenter::App->getSession()->SetBool("Increment",IndexInc,"Indexer",Name,"Index");
	if(Name=="Documents")
	{
		KGALILEICenter::App->getSession()->SetUInt("BlockSize",StructBlock,"Indexer","Documents","Structure");
		KGALILEICenter::App->getSession()->SetUInt("Tolerance",StructTolerance,"Indexer","Documents","Structure");
		KGALILEICenter::App->getSession()->SetUInt("CacheSize",StructCache,"Indexer","Documents","Structure");
		KGALILEICenter::App->getSession()->SetInt("Type",StructType,"Indexer","Documents","Structure");
	}
}



//------------------------------------------------------------------------------
//
// class Configure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Configure::Configure(KGALILEICenter* app)
	: KDialog(app), Ui_Configure(), Types(10), CurType(0), App(app)
{
	setCaption("Preferences");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
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

	if(KDialog::exec())
	{
		applySession();
		applySimulation();
		applyPlugIns();
		if(App->getSession())
			App->getSession()->ApplyConfig();
	}

	MainTabIdx=MainTab->currentIndex();
	PlugInsTabIdx=PlugInsTab->currentIndex();
	DocsTabIdx=DocsTab->currentIndex();
	CommunitiesTabIdx=CommunitiesTab->currentIndex();
	SearchTabIdx=SearchTab->currentIndex();
	SimulationTabIdx=SimulationTab->currentIndex();
	MeasuresCatIdx=MeasuresCats->currentRow();
	ToolsCatIdx=ToolsCats->currentRow();
}


//------------------------------------------------------------------------------
void Configure::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");

	MainTabIdx=General.readEntry("MainTabIdx",0);
	PlugInsTabIdx=General.readEntry("PlugInsTabIdx",0);
	DocsTabIdx=General.readEntry("DocsTabIdx",0);
	ProfilesTabIdx=General.readEntry("ProfilesTabIdx",0);
	CommunitiesTabIdx=General.readEntry("CommunitiesTabIdx",0);
	SearchTabIdx=General.readEntry("SearchTabIdx",0);
	SimulationTabIdx=General.readEntry("SimulationTabIdx",0);
	MeasuresCatIdx=General.readEntry("MeasuresCatIdx",0);
	ToolsCatIdx=General.readEntry("ToolsCatIdx",0);
}


//------------------------------------------------------------------------------
void Configure::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");

	General.writeEntry("MainTabIdx",MainTabIdx);
	General.writeEntry("PlugInsTabIdx",PlugInsTabIdx);
	General.writeEntry("DocsTabIdx",DocsTabIdx);
	General.writeEntry("ProfilesTabIdx",ProfilesTabIdx);
	General.writeEntry("CommunitiesTabIdx",CommunitiesTabIdx);
	General.writeEntry("SearchTabIdx",SearchTabIdx);
	General.writeEntry("SimulationTabIdx",SimulationTabIdx);
	General.writeEntry("MeasuresCatIdx",MeasuresCatIdx);
	General.writeEntry("ToolsCatIdx",ToolsCatIdx);
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
 	RCursor<GSubject> Cur(subject->GetSubjects());
 	for(Cur.Start();!Cur.End();Cur.Next())
 		addSubject(Cur(),item);
}


//------------------------------------------------------------------------------
void Configure::initSession(void)
{
	// Indexer
	connect(Objects,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(objectChanged(const QString&)));
	RContainer<RString,true,false> TypesNames(10);
	App->getSession()->GetCategories(TypesNames,"Indexer");
	RCursor<RString> Cur(TypesNames);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Objects->addItem(ToQString(*Cur()));
		Types.InsertPtr(new Type(*Cur()));
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
	if(!App->getSession())
	{
		MaxDepth->setEnabled(false);
		ManualSubjects->setEnabled(false);
		Subjects->setEnabled(false);
	}

	// Read Values
	NbOK->setValue(App->getSession()->GetDouble("Nb","Simulator","Profiles","Relevant"));
	RelOK->setChecked(App->getSession()->GetBool("Percentage","Simulator","Profiles","Relevant"));
	NbKO->setValue(App->getSession()->GetDouble("Nb","Simulator","Profiles","Fuzzy Relevant"));
	RelKO->setChecked(App->getSession()->GetBool("Percentage","Simulator","Profiles","Fuzzy Relevant"));
	NbH->setValue(App->getSession()->GetDouble("Nb","Simulator","Profiles","Irrelevant"));
	RelH->setChecked(App->getSession()->GetBool("Percentage","Simulator","Profiles","Irrelevant"));
	PercErr->setValue(App->getSession()->GetDouble("PercErr","Simulator","Profiles"));
	NbProfMin->setValue(App->getSession()->GetInt("NbProfMin","Simulator","Profiles"));
	NbProfMax->setValue(App->getSession()->GetInt("NbProfMax","Simulator","Profiles"));
	MaxDepth->setValue(App->getSession()->GetInt("MaxDepth","Simulator","Subjects"));
	PercSocial->setValue(App->getSession()->GetDouble("PercSocial","Simulator","Profiles"));
	NbSubjects->setValue(App->getSession()->GetDouble("Nb","Simulator","Subjects","NbSubjects"));
	RelSubjects->setChecked(App->getSession()->GetBool("Percentage","Simulator","Subjects","NbSubjects"));
	NbMinDocsSubject->setValue(App->getSession()->GetInt("NbMinDocsSubject","Simulator","Subjects"));
	NbDocsAssess->setValue(App->getSession()->GetInt("NbDocsAssess","Simulator","Profiles"));
	SwitchPerc->setValue(App->getSession()->GetDouble("SwitchPerc","Simulator","Documents"));
	ManualSubjects->setChecked(App->getSession()->GetBool("ManualSubjects","Simulator","Subjects"));
	NbDocsPerSubject->setValue(App->getSession()->GetDouble("Nb","Simulator","Documents","DocsPerSubject"));
	PercNbDocsPerSubject->setChecked(App->getSession()->GetBool("Percentage","Simulator","Documents","DocsPerSubject"));
	ClusterSelectedDocs->setChecked(App->getSession()->GetBool("ClusterSelectedDocs","Simulator","Documents"));
	MultipleSubjects->setChecked(App->getSession()->GetBool("MultipleSubjects","Simulator","Documents"));
	CreateProfiles->setChecked(App->getSession()->GetBool("CreateProfiles","Simulator","Profiles"));
	grpProfilesCreated->setEnabled(CreateProfiles->isChecked());
	grpPercentage->setEnabled(CreateProfiles->isChecked());
	grpType->setEnabled(CreateProfiles->isChecked());

	// Read Subjects
	if(!App->getSession())
	{
		return;
	}
	MaxDepth->setMaximum(App->getSession()->GetMaxDepth());
	Depth->setText(QString::number(App->getSession()->GetMaxDepth()));
	Subjects->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	Subjects->header()->setResizeMode(1,QHeaderView::Stretch);
	connect(Subjects,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(subjectClicked(QTreeWidgetItem*,int)));
	RCursor<GSubject> Cur(App->getSession()->GetTopSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
		addSubject(Cur(),0);
}


//------------------------------------------------------------------------------
void Configure::applySubSubjects(QTreeWidgetItem* parent)
{
	for(int i=0;i<parent->childCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(parent->child(i)));
		if(!item) continue;
		if(item->Select)
			App->getSession()->AddToList("SelectedSubjects",FromQString(item->text(1)),"Simulator");
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	App->getSession()->SetDouble("Nb",NbOK->value(),"Simulator","Profiles","Relevant");
	App->getSession()->SetBool("Percentage",RelOK->isChecked(),"Simulator","Profiles","Relevant");
	App->getSession()->SetDouble("Nb",NbKO->value(),"Simulator","Profiles","Fuzzy Relevant");
	App->getSession()->SetBool("Percentage",RelKO->isChecked(),"Simulator","Profiles","Fuzzy Relevant");
	App->getSession()->SetDouble("Nb",NbH->value(),"Simulator","Profiles","Irrelevant");
	App->getSession()->SetBool("Percentage",RelH->isChecked(),"Simulator","Profiles","Irrelevant");
	App->getSession()->SetDouble("PercErr",PercErr->value(),"Simulator","Profiles");
	App->getSession()->SetUInt("NbProfMin",NbProfMin->value(),"Simulator","Profiles");
	App->getSession()->SetUInt("NbProfMax",NbProfMax->value(),"Simulator","Profiles");
	App->getSession()->SetUInt("MaxDepth",MaxDepth->value(),"Simulator","Subjects");
	App->getSession()->SetDouble("PercSocial",PercSocial->value(),"Simulator","Profiles");
	App->getSession()->SetDouble("Nb",NbSubjects->value(),"Simulator","Subjects","NbSubjects");
	App->getSession()->SetBool("Percentage",RelSubjects->isChecked(),"Simulator","Subjects","NbSubjects");
	App->getSession()->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Simulator","Subjects");
	App->getSession()->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Simulator","Profiles");
	App->getSession()->SetDouble("SwitchPerc",SwitchPerc->value(),"Simulator","Documents");
	App->getSession()->SetBool("ManualSubjects",ManualSubjects->isChecked(),"Simulator","Subjects");
	App->getSession()->SetDouble("Nb",NbDocsPerSubject->value(),"Simulator","Documents","DocsPerSubject");
	App->getSession()->SetBool("Percentage",PercNbDocsPerSubject->isChecked(),"Simulator","Documents","DocsPerSubject");
	App->getSession()->SetBool("ClusterSelectedDocs",ClusterSelectedDocs->isChecked(),"Simulator","Documents");
	App->getSession()->SetBool("MultipleSubjects",MultipleSubjects->isChecked(),"Simulator","Documents");
	App->getSession()->SetBool("CreateProfiles",CreateProfiles->isChecked(),"Simulator","Profiles");

	// Apply subjects
	if(!App->getSession())
		return;
	for(int i=0;i<Subjects->topLevelItemCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(Subjects->topLevelItem(i)));
		if(!item) continue;
		App->getSession()->GetSimulator()->SetManualUsed(item->Subject,item->Select);
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
	DocAnalyzes->init(QPlugInsList::DocAnalyzes);
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
}


//-----------------------------------------------------------------------------
void Configure::applyPlugIns(void)
{
	// Apply plug-ins
	Storages->apply(App->getSession());
	Filters->apply(App->getSession());
	ProfileCalcs->apply(App->getSession());
	GroupProfiles->apply(App->getSession());
	GroupDocs->apply(App->getSession());
	CommunityCalcs->apply(App->getSession());
	TopicCalcs->apply(App->getSession());
	LinkCalcs->apply(App->getSession());
	DocAnalyzes->apply(App->getSession());
	Engines->apply(App->getSession());
	MetaEngines->apply(App->getSession());
	Langs->apply(App->getSession());
	ComputeSugs->apply(App->getSession());
	ComputeTrust->apply(App->getSession());

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply(App->getSession());

	// Apply Tools
	for(int row=0;row<Tools->count();row++)
		dynamic_cast<QPlugInsList*>(Tools->widget(row))->apply(App->getSession());

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
		CurType->IndexInc=IndexInc->isChecked();
		CurType->StructBlock=StructBlock->value();
		CurType->StructTolerance=StructTolerance->value();
		CurType->StructCache=StructCache->value();
		CurType->StructType=StructType->currentIndex();
	}

	// Set the current element
	CurType=Types.GetPtr(FromQString(obj));
	if(!CurType)
	{
		DocsStruct->setVisible(false);
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
	IndexInc->setChecked(CurType->IndexInc);

	if(CurType->Name=="Documents")
	{
		DocsStruct->setVisible(true);
		StructBlock->setValue(CurType->StructBlock);
		StructTolerance->setValue(CurType->StructTolerance);
		StructCache->setValue(CurType->StructCache);
		StructType->setCurrentIndex(CurType->StructType);
	}
	else
		DocsStruct->setVisible(false);
}
