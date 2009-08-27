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
#include <gcomputesugs.h>
#include <gcomputetrust.h>
#include <gmeasure.h>
#include <gsession.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gsimulator.h>
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
	initIndexer();
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
		applyIndexer();
		applySimulation();
		applyPlugIns();
		if(Session)
			Session->Apply();
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
 	RCursor<GSubject> Cur(subject->GetSubjects());
 	for(Cur.Start();!Cur.End();Cur.Next())
 		addSubject(Cur(),item);
}


//------------------------------------------------------------------------------
void Configure::initIndexer(void)
{
	KGALILEICenter* App=dynamic_cast<KGALILEICenter*>(parentWidget());
	IndexDir->setUrl(ToQString(App->GetIndexDir()));

	// If no session -> No other Indexer Parameters
	if(!Session)
	{
		DocsDesc->setEnabled(false);
		DocsStruct->setEnabled(false);
		DocsIndex->setEnabled(false);
		ProfilesDesc->setEnabled(false);
		CommunitiesDesc->setEnabled(false);
		TopicsDesc->setEnabled(false);
		ClassesDesc->setEnabled(false);
		return;
	}

	IndexDocsInc->setChecked(Session->GetBool("IndexDocsInc","Indexer"));
	QList<QString> Types;
	Types << "DocsDesc" << "DocsStruct" << "DocsIndex" << "ProfilesDesc" << "CommunitiesDesc" << "TopicsDesc" << "ClassesDesc";
	for(int i=0;i<Types.size();++i)
	{
		QString name(Types.at(i)+"Block");
		KIntNumInput* Block(findChild<KIntNumInput*>(name));
		Block->setValue(Session->GetUInt(FromQString(name),"Indexer"));
		name=Types.at(i)+"Tolerance";
		KIntNumInput* Tolerance(findChild<KIntNumInput*>(name));
		Tolerance->setValue(Session->GetUInt(FromQString(name),"Indexer"));
		name=Types.at(i)+"Cache";
		KIntNumInput* Cache(findChild<KIntNumInput*>(name));
		Cache->setValue(Session->GetUInt(FromQString(name),"Indexer"));
		name=Types.at(i)+"Type";
		QComboBox* Type(findChild<QComboBox*>(name));
		Type->setCurrentIndex(Session->GetInt(FromQString(name),"Indexer"));
	 }
}


//------------------------------------------------------------------------------
void Configure::applyIndexer(void)
{
	KGALILEICenter* App=dynamic_cast<KGALILEICenter*>(parentWidget());
	App->SetIndexDir(FromQString(IndexDir->url().url()));
	if(!Session)
		return;

	Session->SetBool("IndexDocsInc",IndexDocsInc->isChecked(),"Indexer");
	QList<QString> Types;
	Types << "DocsDesc" << "DocsStruct" << "DocsIndex" << "ProfilesDesc" << "CommunitiesDesc" << "TopicsDesc" << "ClassesDesc";
	for(int i=0;i<Types.size();++i)
	{
		QString name(Types.at(i)+"Block");
		KIntNumInput* Block(findChild<KIntNumInput*>(name));
		Session->SetUInt(FromQString(name),Block->value(),"Indexer");
		name=Types.at(i)+"Tolerance";
		KIntNumInput* Tolerance(findChild<KIntNumInput*>(name));
		Session->SetUInt(FromQString(name),Tolerance->value(),"Indexer");
		name=Types.at(i)+"Cache";
		KIntNumInput* Cache(findChild<KIntNumInput*>(name));
		Session->SetUInt(FromQString(name),Cache->value(),"Indexer");
		name=Types.at(i)+"Type";
		QComboBox* Type(findChild<QComboBox*>(name));
		Session->SetInt(FromQString(name),Type->currentIndex(),"Indexer");
	 }

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
	NbOK->setValue(Session->GetDouble("NbOK","Simulator"));
	RelOK->setChecked(Session->GetBool("RelOK","Simulator"));
	NbKO->setValue(Session->GetDouble("NbKO","Simulator"));
	RelKO->setChecked(Session->GetBool("RelKO","Simulator"));
	NbH->setValue(Session->GetDouble("NbH","Simulator"));
	RelH->setChecked(Session->GetBool("RelH","Simulator"));
	PercErr->setValue(Session->GetDouble("PercErr","Simulator"));
	NbProfMin->setValue(Session->GetInt("NbProfMin","Simulator"));
	NbProfMax->setValue(Session->GetInt("NbProfMax","Simulator"));
	MaxDepth->setValue(Session->GetInt("MaxDepth","Simulator"));
	MaxDepth->setMaximum(Session->GetMaxDepth());
	Depth->setText(QString::number(Session->GetMaxDepth()));
	PercSocial->setValue(Session->GetDouble("PercSocial","Simulator"));
	NbSubjects->setValue(Session->GetDouble("NbSubjects","Simulator"));
	RelSubjects->setChecked(Session->GetBool("RelSubjects","Simulator"));
	NbMinDocsSubject->setValue(Session->GetInt("NbMinDocsSubject","Simulator"));
	NbDocsAssess->setValue(Session->GetInt("NbDocsAssess","Simulator"));
	SwitchPerc->setValue(Session->GetDouble("SwitchPerc","Simulator"));
	ManualSubjects->setChecked(Session->GetBool("ManualSubjects","Simulator"));
	NbDocsPerSubject->setValue(Session->GetDouble("NbDocsPerSubject","Simulator"));
	PercNbDocsPerSubject->setChecked(Session->GetBool("PercNbDocsPerSubject","Simulator"));
	ClusterSelectedDocs->setChecked(Session->GetBool("ClusterSelectedDocs","Simulator"));
	MultipleSubjects->setChecked(Session->GetBool("MultipleSubjects","Simulator"));

	// Read Subjects
	Subjects->header()->setResizeMode(0,QHeaderView::ResizeToContents);
	Subjects->header()->setResizeMode(1,QHeaderView::Stretch);
	connect(Subjects,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(subjectClicked(QTreeWidgetItem*,int)));
	Subjects->setEnabled(Session->GetBool("ManualSubjects","Subjects"));

	if(!Session) return;
	RCursor<GSubject> Cur(Session->GetTopSubjects());
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
			Session->AddToList("SelectedSubjects",FromQString(item->text(1)),"Simulator");
		applySubSubjects(item);
	}
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	if(!Session)
		return;

	Session->SetDouble("NbOK",NbOK->value(),"Simulator");
	Session->SetBool("RelOK",RelOK->isChecked(),"Simulator");
	Session->SetDouble("NbKO",NbKO->value(),"Simulator");
	Session->SetBool("RelKO",RelKO->isChecked(),"Simulator");
	Session->SetDouble("NbH",NbH->value(),"Simulator");
	Session->SetBool("RelH",RelH->isChecked(),"Simulator");
	Session->SetDouble("PercErr",PercErr->value(),"Simulator");
	Session->SetUInt("NbProfMin",NbProfMin->value(),"Simulator");
	Session->SetUInt("NbProfMax",NbProfMax->value(),"Simulator");
	Session->SetUInt("MaxDepth",MaxDepth->value(),"Simulator");
	Session->SetDouble("PercSocial",PercSocial->value(),"Simulator");
	Session->SetDouble("NbSubjects",NbSubjects->value(),"Simulator");
	Session->SetBool("RelSubjects",RelSubjects->isChecked(),"Simulator");
	Session->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Simulator");
	Session->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Simulator");
	Session->SetDouble("SwitchPerc",SwitchPerc->value(),"Simulator");
	Session->SetBool("ManualSubjects",ManualSubjects->isChecked(),"Simulator");
	Session->SetDouble("NbDocsPerSubject",NbDocsPerSubject->value(),"Simulator");
	Session->SetBool("PercNbDocsPerSubject",PercNbDocsPerSubject->isChecked(),"Simulator");
	Session->SetBool("ClusterSelectedDocs",ClusterSelectedDocs->isChecked(),"Simulator");
	Session->SetBool("MultipleSubjects",MultipleSubjects->isChecked(),"Simulator");
	for(int i=0;i<Subjects->topLevelItemCount();i++)
	{
		QSubjectItem* item(dynamic_cast<QSubjectItem*>(Subjects->topLevelItem(i)));
		if(!item) continue;
		Session->GetSimulator()->SetManualUsed(item->Subject,item->Select);
		applySubSubjects(item);
	}
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
	GALILEIApp->GetManager<GComputeSugsManager>("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager<GComputeTrustManager>("ComputeTrust")->ReOrder();

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
	ComputeSugs->init(QPlugInsList::ComputeSugs);
	ComputeTrust->init(QPlugInsList::ComputeTrust);
	Tools->init(QPlugInsList::Tools);

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
	ComputeSugs->apply(Session);
	ComputeTrust->apply(Session);
	Tools->apply(Session);

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply(Session);

	// Sort POST_X Managers;
	GALILEIApp->GetManager<GPostDocManager>("PostDoc")->ReOrder();
	GALILEIApp->GetManager<GPreProfileManager>("PreProfile")->ReOrder();
	GALILEIApp->GetManager<GPostProfileManager>("PostProfile")->ReOrder();
	GALILEIApp->GetManager<GPostCommunityManager>("PostCommunity")->ReOrder();
	GALILEIApp->GetManager<GPostTopicManager>("PostTopic")->ReOrder();
	GALILEIApp->GetManager<GComputeSugsManager>("ComputeSugs")->ReOrder();
	GALILEIApp->GetManager<GComputeTrustManager>("ComputeTrust")->ReOrder();
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

