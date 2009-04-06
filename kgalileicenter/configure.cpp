/*

	KGALILEICenter

	Configure.cpp

	Configure Dialog Box - Implementation.

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
#include <gmeasure.h>
#include <gsession.h>
#include <gsubjects.h>
#include <gsubject.h>
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
// class Configure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Configure::Configure(QWidget* parent)
	: KDialog(parent), Ui_Configure()
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
void Configure::exec(GSession* session)
{
	Session=session;

	initGeneral();
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
		applySimulation();
		applyPlugIns();
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
	KGALILEICenter* App=dynamic_cast<KGALILEICenter*>(parentWidget());

	// General
	PlugInsConfig->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	PlugInsConfig->setUrl(R::ToQString(App->GetPlugInsConfigName()));
	LogFile->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	LogFile->setUrl(R::ToQString(App->GetLogFileName()));
	DebugFile->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	DebugFile->setUrl(R::ToQString(App->GetDebugFileName()));
	PrgPath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
	PrgPath->setUrl(R::ToQString(App->PrgPath));
	IndexDir->setUrl(ToQString(App->GetIndexDir()));

	// Directories
	R::RCursor<R::RString> Cur(App->GetPlugInsPath());
	for(Cur.Start();!Cur.End();Cur.Next())
		Dirs->addItem(ToQString(*Cur()));
}


//------------------------------------------------------------------------------
void Configure::applyGeneral(void)
{
	KGALILEICenter* App=dynamic_cast<KGALILEICenter*>(parentWidget());

	// General
	App->SetPlugInsConfigName(R::FromQString(PlugInsConfig->url().url()));
	App->SetLogFileName(R::FromQString(LogFile->url().url()));
	App->SetDebugFileName(R::FromQString(DebugFile->url().url()));
	App->PrgPath=R::FromQString(PrgPath->url().url());
	QString debug=DebugFile->url().url();
	App->SetIndexDir(FromQString(IndexDir->url().url()));
	if(!debug.isEmpty())
	{
	try
	{
		App->Debug=new R::RDebugXML(R::FromQString(debug));
		if(App->GetSession())
			App->GetSession()->SetDebug(App->Debug);
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter","Error while creating '"+debug+"'");
	}
	}

	// Directories
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
 	RCursor<GSubject> Cur(subject->GetNodes());
 	for(Cur.Start();!Cur.End();Cur.Next())
 		addSubject(Cur(),item);
}


//------------------------------------------------------------------------------
void Configure::initSimulation(void)
{
	// If no session -> No Simulation Parameters
	if(!Session)
	{
		SimulationTab->setEnabled(false);
		return;
	}

	// Read Values
	NbOK->setValue(Session->GetDouble("NbOK","Subjects"));
	RelOK->setChecked(Session->GetBool("RelOK","Subjects"));
	NbKO->setValue(Session->GetDouble("NbKO","Subjects"));
	RelKO->setChecked(Session->GetBool("RelKO","Subjects"));
	NbH->setValue(Session->GetDouble("NbH","Subjects"));
	RelH->setChecked(Session->GetBool("RelH","Subjects"));
	PercErr->setValue(Session->GetDouble("PercErr","Subjects"));
	NbProfMin->setValue(Session->GetInt("NbProfMin","Subjects"));
	NbProfMax->setValue(Session->GetInt("NbProfMax","Subjects"));
	PercSocial->setValue(Session->GetDouble("PercSocial","Subjects"));
	NbSubjects->setValue(Session->GetDouble("NbSubjects","Subjects"));
	RelSubjects->setChecked(Session->GetBool("RelSubjects","Subjects"));
	NbMinDocsSubject->setValue(Session->GetInt("NbMinDocsSubject","Subjects"));
	NbDocsAssess->setValue(Session->GetInt("NbDocsAssess","Subjects"));
	SwitchPerc->setValue(Session->GetDouble("SwitchPerc","Subjects"));
	ManualSubjects->setChecked(Session->GetBool("ManualSubjects","Subjects"));
	NbDocsPerSubject->setValue(Session->GetDouble("NbDocsPerSubject","Subjects"));
	PercNbDocsPerSubject->setChecked(Session->GetBool("PercNbDocsPerSubject","Subjects"));
	ClusterSelectedDocs->setChecked(Session->GetBool("ClusterSelectedDocs","Subjects"));

	// Read Subjects
	Subjects->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	Subjects->header()->setResizeMode(1,QHeaderView::Stretch);
	connect(Subjects,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(subjectClicked(QTreeWidgetItem*,int)));
	Subjects->setEnabled(Session->GetBool("ManualSubjects","Subjects"));

	if(!Session) return;
	GSubjects* TheSubjects(Session->GetSubjects());
	if(!TheSubjects) return;
	RCursor<GSubject> Cur(TheSubjects->GetTop()->GetNodes());
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
			Session->AddToList("SelectedSubjects",FromQString(item->text(1)),"Subjects");
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	if(!Session)
		return;

	Session->SetDouble("NbOK",NbOK->value(),"Subjects");
	Session->SetBool("RelOK",RelOK->isChecked(),"Subjects");
	Session->SetDouble("NbKO",NbKO->value(),"Subjects");
	Session->SetBool("RelKO",RelKO->isChecked(),"Subjects");
	Session->SetDouble("NbH",NbH->value(),"Subjects");
	Session->SetBool("RelH",RelH->isChecked(),"Subjects");
	Session->SetDouble("PercErr",PercErr->value(),"Subjects");
	Session->SetUInt("NbProfMin",NbProfMin->value(),"Subjects");
	Session->SetUInt("NbProfMax",NbProfMax->value(),"Subjects");
	Session->SetDouble("PercSocial",PercSocial->value(),"Subjects");
	Session->SetDouble("NbSubjects",NbSubjects->value(),"Subjects");
	Session->SetBool("RelSubjects",RelSubjects->isChecked(),"Subjects");
	Session->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Subjects");
	Session->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Subjects");
	Session->SetDouble("SwitchPerc",SwitchPerc->value(),"Subjects");
	Session->SetBool("ManualSubjects",ManualSubjects->isChecked(),"Subjects");
	Session->SetDouble("NbDocsPerSubject",NbDocsPerSubject->value(),"Subjects");
	Session->SetBool("PercNbDocsPerSubject",PercNbDocsPerSubject->isChecked(),"Subjects");
	Session->SetBool("ClusterSelectedDocs",ClusterSelectedDocs->isChecked(),"Subjects");
	Session->Reset("SelectedSubjects","Subjects");
	for(int i=0;i<Subjects->topLevelItemCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(Subjects->topLevelItem(i)));
		if(!item) continue;
		if(item->Select)
			Session->AddToList("SelectedSubjects",FromQString(item->text(1)),"Subjects");
		applySubSubjects(item);
	}
	Session->Apply();
}


//------------------------------------------------------------------------------
void Configure::initPlugIns(void)
{
	// Sort POST_X Managers;
	GALILEIApp->GetManager<GPostDocManager>("PostDoc")->ReOrder();
	GALILEIApp->GetManager<GPreProfileManager>("PreProfile")->ReOrder();
	GALILEIApp->GetManager<GPostProfileManager>("PostProfile")->ReOrder();
	GALILEIApp->GetManager<GPostCommunityManager>("PostCommunity")->ReOrder();
	GALILEIApp->GetManager<GPostTopicManager>("PostTopic")->ReOrder();

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
	DocAnalyses->init(QPlugInsList::DocAnalyses);
	Engines->init(QPlugInsList::Engines);
	MetaEngines->init(QPlugInsList::MetaEngines);

	// Init Measures
	GMeasureManager* Manager=GALILEIApp->GetManager<GMeasureManager>("Measures");
	RCursor<GTypeMeasureManager> Cur(Manager->GetMeasureCategories());
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
	Storages->apply(Session);
	Filters->apply(Session);
	ProfileCalcs->apply(Session);
	GroupProfiles->apply(Session);
	GroupDocs->apply(Session);
	CommunityCalcs->apply(Session);
	TopicCalcs->apply(Session);
	StatsCalcs->apply(Session);
	LinkCalcs->apply(Session);
	PostDocs->apply(Session);
	PreProfiles->apply(Session);
	DocAnalyses->apply(Session);
	PostTopics->apply(Session);
	Engines->apply(Session);
	MetaEngines->apply(Session);
	Langs->apply(Session);
	PostProfiles->apply(Session);
	PostCommunities->apply(Session);

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply(Session);

	// Sort POST_X Managers;
	GALILEIApp->GetManager<GPostDocManager>("PostDoc")->ReOrder();
	GALILEIApp->GetManager<GPreProfileManager>("PreProfile")->ReOrder();
	GALILEIApp->GetManager<GPostProfileManager>("PostProfile")->ReOrder();
	GALILEIApp->GetManager<GPostCommunityManager>("PostCommunity")->ReOrder();
	GALILEIApp->GetManager<GPostTopicManager>("PostTopic")->ReOrder();
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

