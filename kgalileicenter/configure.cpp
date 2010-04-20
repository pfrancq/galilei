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
#include <gpostcommunity.h>
#include <gposttopic.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gpostdoc.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
#include <gmeasure.h>
#include <gsession.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gsimulator.h>
#include <rcursor.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// includes files for current application
#include <kgalileicenter.h>
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
	DescBlock=GALILEIApp->GetSessionConfig()->GetUInt("BlockSize","Indexer",Name,"Description");
	DescTolerance=GALILEIApp->GetSessionConfig()->GetUInt("Tolerance","Indexer",Name,"Description");
	DescCache=GALILEIApp->GetSessionConfig()->GetUInt("CacheSize","Indexer",Name,"Description");
	DescType=GALILEIApp->GetSessionConfig()->GetInt("Type","Indexer",Name,"Description");
	IndexBlock=GALILEIApp->GetSessionConfig()->GetUInt("BlockSize","Indexer",Name,"Index");
	IndexTolerance=GALILEIApp->GetSessionConfig()->GetUInt("Tolerance","Indexer",Name,"Index");
	IndexCache=GALILEIApp->GetSessionConfig()->GetUInt("CacheSize","Indexer",Name,"Index");
	IndexType=GALILEIApp->GetSessionConfig()->GetInt("Type","Indexer",Name,"Index");
	IndexInc=GALILEIApp->GetSessionConfig()->GetBool("Increment","Indexer",Name,"Index");
	if(Name=="Documents")
	{
		StructBlock=GALILEIApp->GetSessionConfig()->GetUInt("BlockSize","Indexer","Documents","Structure");
		StructTolerance=GALILEIApp->GetSessionConfig()->GetUInt("Tolerance","Indexer","Documents","Structure");
		StructCache=GALILEIApp->GetSessionConfig()->GetUInt("CacheSize","Indexer","Documents","Structure");
		StructType=GALILEIApp->GetSessionConfig()->GetInt("Type","Indexer","Documents","Structure");
	}
}


//------------------------------------------------------------------------------
void Configure::Type::Apply(void)
{
	GALILEIApp->GetSessionConfig()->SetUInt("BlockSize",DescBlock,"Indexer",Name,"Description");
	GALILEIApp->GetSessionConfig()->SetUInt("Tolerance",DescTolerance,"Indexer",Name,"Description");
	GALILEIApp->GetSessionConfig()->SetUInt("CacheSize",DescCache,"Indexer",Name,"Description");
	GALILEIApp->GetSessionConfig()->SetInt("Type",DescType,"Indexer",Name,"Description");
	GALILEIApp->GetSessionConfig()->SetUInt("BlockSize",IndexBlock,"Indexer",Name,"Index");
	GALILEIApp->GetSessionConfig()->SetUInt("Tolerance",IndexTolerance,"Indexer",Name,"Index");
	GALILEIApp->GetSessionConfig()->SetUInt("CacheSize",IndexCache,"Indexer",Name,"Index");
	GALILEIApp->GetSessionConfig()->SetInt("Type",IndexType,"Indexer",Name,"Index");
	GALILEIApp->GetSessionConfig()->SetBool("Increment",IndexInc,"Indexer",Name,"Index");
	if(Name=="Documents")
	{
		GALILEIApp->GetSessionConfig()->SetUInt("BlockSize",StructBlock,"Indexer","Documents","Structure");
		GALILEIApp->GetSessionConfig()->SetUInt("Tolerance",StructTolerance,"Indexer","Documents","Structure");
		GALILEIApp->GetSessionConfig()->SetUInt("CacheSize",StructCache,"Indexer","Documents","Structure");
		GALILEIApp->GetSessionConfig()->SetInt("Type",StructType,"Indexer","Documents","Structure");
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
	connect(buttonAdd,SIGNAL(pressed()),this,SLOT(slotAddDir()));
	connect(buttonDel,SIGNAL(pressed()),this,SLOT(slotDelDir()));
}


//------------------------------------------------------------------------------
void Configure::exec(void)
{
	initGeneral();
	initSession();
	initSimulation();
	initPlugIns();

	MainTab->setCurrentIndex(MainTabIdx);
	PlugInsTab->setCurrentIndex(PlugInsTabIdx);
	DocsTab->setCurrentIndex(DocsTabIdx);
	ProfilesTab->setCurrentIndex(ProfilesTabIdx);
	CommunitiesTab->setCurrentIndex(CommunitiesTabIdx);
	SearchTab->setCurrentIndex(SearchTabIdx);
	SimulationTab->setCurrentIndex(SimulationTabIdx);
	MeasuresCats->setCurrentRow(MeasuresCatIdx);

	if(KDialog::exec())
	{
		applyGeneral();
		applySession();
		applySimulation();
		applyPlugIns();
		if(App->GetSession())
			App->GetSession()->ApplyConfig();
	}

	MainTabIdx=MainTab->currentIndex();
	PlugInsTabIdx=PlugInsTab->currentIndex();
	DocsTabIdx=DocsTab->currentIndex();
	ProfilesTabIdx=ProfilesTab->currentIndex();
	CommunitiesTabIdx=CommunitiesTab->currentIndex();
	SearchTabIdx=SearchTab->currentIndex();
	SimulationTabIdx=SimulationTab->currentIndex();
	MeasuresCatIdx=MeasuresCats->currentRow();
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
}


//------------------------------------------------------------------------------
void Configure::initGeneral(void)
{
	// GALILEI Library
	IndexDir->setUrl(ToQString(App->GetIndexDir()));
	R::RCursor<R::RString> Cur(App->GetPlugInsPath());
	for(Cur.Start();!Cur.End();Cur.Next())
		Dirs->addItem(ToQString(*Cur()));
}


//------------------------------------------------------------------------------
void Configure::applyGeneral(void)
{
	// // GALILEI Library
	App->SetIndexDir(FromQString(IndexDir->url().url()));
	App->ClearPlugInsPath();
	for(int i=0;i<Dirs->count();i++)
		App->AddPlugInsPath(R::FromQString(Dirs->item(i)->text()));
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
	// Information
	LogFile->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	LogFile->setUrl(R::ToQString(App->GetLogFileName()));
	DebugFile->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	DebugFile->setUrl(R::ToQString(App->GetDebugFileName()));

	// Indexer
	connect(Objects,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(objectChanged(const QString&)));
	RContainer<RString,true,false> TypesNames(10);
	App->GetSessionConfig()->GetCategories(TypesNames,"Indexer");
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
	// Information
	App->SetLogFileName(R::FromQString(LogFile->url().url()));
	App->SetDebugFileName(R::FromQString(DebugFile->url().url()));

	// Indexer
	objectChanged(Objects->currentText());
	RCursor<Type> Cur(Types);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Apply();
}


//------------------------------------------------------------------------------
void Configure::initSimulation(void)
{
	if(!App->GetSession())
	{
		MaxDepth->setEnabled(false);
		ManualSubjects->setEnabled(false);
		Subjects->setEnabled(false);
	}

	// Read Values
	NbOK->setValue(App->GetSessionConfig()->GetDouble("Nb","Simulator","Profiles","Relevant"));
	RelOK->setChecked(App->GetSessionConfig()->GetBool("Percentage","Simulator","Profiles","Relevant"));
	NbKO->setValue(App->GetSessionConfig()->GetDouble("Nb","Simulator","Profiles","Fuzzy Relevant"));
	RelKO->setChecked(App->GetSessionConfig()->GetBool("Percentage","Simulator","Profiles","Fuzzy Relevant"));
	NbH->setValue(App->GetSessionConfig()->GetDouble("Nb","Simulator","Profiles","Irrelevant"));
	RelH->setChecked(App->GetSessionConfig()->GetBool("Percentage","Simulator","Profiles","Irrelevant"));
	PercErr->setValue(App->GetSessionConfig()->GetDouble("PercErr","Simulator","Profiles"));
	NbProfMin->setValue(App->GetSessionConfig()->GetInt("NbProfMin","Simulator","Profiles"));
	NbProfMax->setValue(App->GetSessionConfig()->GetInt("NbProfMax","Simulator","Profiles"));
	MaxDepth->setValue(App->GetSessionConfig()->GetInt("MaxDepth","Simulator","Subjects"));
	PercSocial->setValue(App->GetSessionConfig()->GetDouble("PercSocial","Simulator","Profiles"));
	NbSubjects->setValue(App->GetSessionConfig()->GetDouble("Nb","Simulator","Subjects","NbSubjects"));
	RelSubjects->setChecked(App->GetSessionConfig()->GetBool("Percentage","Simulator","Subjects","NbSubjects"));
	NbMinDocsSubject->setValue(App->GetSessionConfig()->GetInt("NbMinDocsSubject","Simulator","Subjects"));
	NbDocsAssess->setValue(App->GetSessionConfig()->GetInt("NbDocsAssess","Simulator","Profiles"));
	SwitchPerc->setValue(App->GetSessionConfig()->GetDouble("SwitchPerc","Simulator","Documents"));
	ManualSubjects->setChecked(App->GetSessionConfig()->GetBool("ManualSubjects","Simulator","Subjects"));
	NbDocsPerSubject->setValue(App->GetSessionConfig()->GetDouble("Nb","Simulator","Documents","DocsPerSubject"));
	PercNbDocsPerSubject->setChecked(App->GetSessionConfig()->GetBool("Percentage","Simulator","Documents","DocsPerSubject"));
	ClusterSelectedDocs->setChecked(App->GetSessionConfig()->GetBool("ClusterSelectedDocs","Simulator","Documents"));
	MultipleSubjects->setChecked(App->GetSessionConfig()->GetBool("MultipleSubjects","Simulator","Documents"));
	CreateProfiles->setChecked(App->GetSessionConfig()->GetBool("CreateProfiles","Simulator","Profiles"));
	grpProfilesCreated->setEnabled(CreateProfiles->isChecked());
	grpPercentage->setEnabled(CreateProfiles->isChecked());
	grpType->setEnabled(CreateProfiles->isChecked());

	// Read Subjects
	if(!App->GetSession())
		return;
	MaxDepth->setMaximum(App->GetSession()->GetMaxDepth());
	Depth->setText(QString::number(App->GetSession()->GetMaxDepth()));
	Subjects->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	Subjects->header()->setResizeMode(1,QHeaderView::Stretch);
	connect(Subjects,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(subjectClicked(QTreeWidgetItem*,int)));
	RCursor<GSubject> Cur(App->GetSession()->GetTopSubjects());
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
			App->GetSessionConfig()->AddToList("SelectedSubjects",FromQString(item->text(1)),"Simulator");
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	App->GetSessionConfig()->SetDouble("Nb",NbOK->value(),"Simulator","Profiles","Relevant");
	App->GetSessionConfig()->SetBool("Percentage",RelOK->isChecked(),"Simulator","Profiles","Relevant");
	App->GetSessionConfig()->SetDouble("Nb",NbKO->value(),"Simulator","Profiles","Fuzzy Relevant");
	App->GetSessionConfig()->SetBool("Percentage",RelKO->isChecked(),"Simulator","Profiles","Fuzzy Relevant");
	App->GetSessionConfig()->SetDouble("Nb",NbH->value(),"Simulator","Profiles","Irrelevant");
	App->GetSessionConfig()->SetBool("Percentage",RelH->isChecked(),"Simulator","Profiles","Irrelevant");
	App->GetSessionConfig()->SetDouble("PercErr",PercErr->value(),"Simulator","Profiles");
	App->GetSessionConfig()->SetUInt("NbProfMin",NbProfMin->value(),"Simulator","Profiles");
	App->GetSessionConfig()->SetUInt("NbProfMax",NbProfMax->value(),"Simulator","Profiles");
	App->GetSessionConfig()->SetUInt("MaxDepth",MaxDepth->value(),"Simulator","Subjects");
	App->GetSessionConfig()->SetDouble("PercSocial",PercSocial->value(),"Simulator","Profiles");
	App->GetSessionConfig()->SetDouble("Nb",NbSubjects->value(),"Simulator","Subjects","NbSubjects");
	App->GetSessionConfig()->SetBool("Percentage",RelSubjects->isChecked(),"Simulator","Subjects","NbSubjects");
	App->GetSessionConfig()->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Simulator","Subjects");
	App->GetSessionConfig()->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Simulator","Profiles");
	App->GetSessionConfig()->SetDouble("SwitchPerc",SwitchPerc->value(),"Simulator","Documents");
	App->GetSessionConfig()->SetBool("ManualSubjects",ManualSubjects->isChecked(),"Simulator","Subjects");
	App->GetSessionConfig()->SetDouble("Nb",NbDocsPerSubject->value(),"Simulator","Documents","DocsPerSubject");
	App->GetSessionConfig()->SetBool("Percentage",PercNbDocsPerSubject->isChecked(),"Simulator","Documents","DocsPerSubject");
	App->GetSessionConfig()->SetBool("ClusterSelectedDocs",ClusterSelectedDocs->isChecked(),"Simulator","Documents");
	App->GetSessionConfig()->SetBool("MultipleSubjects",MultipleSubjects->isChecked(),"Simulator","Documents");
	App->GetSessionConfig()->SetBool("CreateProfiles",CreateProfiles->isChecked(),"Simulator","Profiles");

	// Apply subjects
	if(!App->GetSession())
		return;
	for(int i=0;i<Subjects->topLevelItemCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(Subjects->topLevelItem(i)));
		if(!item) continue;
		App->GetSession()->GetSimulator()->SetManualUsed(item->Subject,item->Select);
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::initPlugIns(void)
{
	// Sort POST_X Managers;
	GALILEIApp->GetManager("PostDoc")->ReOrder();
	GALILEIApp->GetManager("PreProfile")->ReOrder();
	GALILEIApp->GetManager("PostProfile")->ReOrder();
	GALILEIApp->GetManager("PostCommunity")->ReOrder();
	GALILEIApp->GetManager("PostTopic")->ReOrder();
	GALILEIApp->GetManager("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager("ComputeTrust")->ReOrder();

	// Goes through managers
	Storages->init(QPlugInsList::Storages);
	Filters->init(QPlugInsList::Filters);
	Langs->init(QPlugInsList::Langs);
	PostProfiles->init(QPlugInsList::PostProfiles);
	PostCommunities->init(QPlugInsList::PostCommunities);
	ProfileCalcs->init(QPlugInsList::ProfileCalcs);
	GroupProfiles->init(QPlugInsList::GroupProfiles);
	GroupDocs->init(QPlugInsList::GroupDocs);
	CommunityCalcs->init(QPlugInsList::CommunityCalcs);
	TopicCalcs->init(QPlugInsList::TopicCalcs);
	StatsCalcs->init(QPlugInsList::StatsCalcs);
	LinkCalcs->init(QPlugInsList::LinkCalcs);
	PostDocs->init(QPlugInsList::PostDocs);
	PreProfiles->init(QPlugInsList::PreProfiles);
	PostTopics->init(QPlugInsList::PostTopics);
	DocAnalyzes->init(QPlugInsList::DocAnalyzes);
	Engines->init(QPlugInsList::Engines);
	MetaEngines->init(QPlugInsList::MetaEngines);
	ComputeSugs->init(QPlugInsList::ComputeSugs);
	ComputeTrust->init(QPlugInsList::ComputeTrust);
	Tools->init(QPlugInsList::Tools);

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
}


//-----------------------------------------------------------------------------
void Configure::applyPlugIns(void)
{
	// Apply plug-ins
	Storages->apply();
	Filters->apply();
	ProfileCalcs->apply();
	GroupProfiles->apply();
	GroupDocs->apply();
	CommunityCalcs->apply();
	TopicCalcs->apply();
	StatsCalcs->apply();
	LinkCalcs->apply();
	PostDocs->apply();
	PreProfiles->apply();
	DocAnalyzes->apply();
	PostTopics->apply();
	Engines->apply();
	MetaEngines->apply();
	Langs->apply();
	PostProfiles->apply();
	PostCommunities->apply();
	ComputeSugs->apply();
	ComputeTrust->apply();
	Tools->apply();

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply();

	// Sort POST_X Managers;
	GALILEIApp->GetManager("PostDoc")->ReOrder();
	GALILEIApp->GetManager("PreProfile")->ReOrder();
	GALILEIApp->GetManager("PostProfile")->ReOrder();
	GALILEIApp->GetManager("PostCommunity")->ReOrder();
	GALILEIApp->GetManager("PostTopic")->ReOrder();
	GALILEIApp->GetManager("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager("ComputeTrust")->ReOrder();
}


//-----------------------------------------------------------------------------
void Configure::slotAddDir(void)
{
	QString newdir=KFileDialog::getExistingDirectory(KUrl::fromPath("~"),this,"Add new directory for plug-ins");
	if((!newdir.isEmpty())&&(!Dirs->findItems(newdir,Qt::MatchExactly).count()))
		Dirs->addItem(newdir);
}


//-----------------------------------------------------------------------------
void Configure::slotDelDir(void)
{
	int row=Dirs->currentRow();
	if(row>-1)
		delete Dirs->item(row);
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
