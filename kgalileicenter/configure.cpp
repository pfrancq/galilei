/*

	KGALILEICenter

	Configure.cpp

	Configure Dialog Box - Implementation.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <kfiledialog.h>
#include <QtGui/QMessageBox>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <rqt.h>
#include <gpostcommunity.h>
#include <gposttopic.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gpostdoc.h>
#include <gmeasure.h>
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
		applyPlugIns(session);
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
void Configure::initSimulation(void)
{
	NbOK->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbOK","Subjects"));
	RelOK->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelOK","Subjects"));
	NbKO->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbKO","Subjects"));
	RelKO->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelKO","Subjects"));
	NbH->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbH","Subjects"));
	RelH->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelH","Subjects"));
	PercErr->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("PercErr","Subjects"));
	NbProfMin->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbProfMin","Subjects"));
	NbProfMax->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbProfMax","Subjects"));
	PercSocial->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("PercSocial","Subjects"));
	NbSubjects->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("NbSubjects","Subjects"));
	RelSubjects->setChecked(GALILEIApp->GetGALILEIConfig()->GetBool("RelSubjects","Subjects"));
	NbMinDocsSubject->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbMinDocsSubject","Subjects"));
	NbDocsAssess->setValue(GALILEIApp->GetGALILEIConfig()->GetInt("NbDocsAssess","Subjects"));
	SwitchPerc->setValue(GALILEIApp->GetGALILEIConfig()->GetDouble("SwitchPerc","Subjects"));
}


//------------------------------------------------------------------------------
void Configure::applySimulation(void)
{
	GALILEIApp->GetGALILEIConfig()->SetDouble("NbOK",NbOK->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetBool("RelOK",RelOK->isChecked(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("NbKO",NbKO->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetBool("RelKO",RelKO->isChecked(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("NbH",NbH->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetBool("RelH",RelH->isChecked(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("PercErr",PercErr->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMin",NbProfMin->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMax",NbProfMax->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("PercSocial",PercSocial->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("NbSubjects",NbSubjects->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetBool("RelSubjects",RelSubjects->isChecked(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetUInt("NbMinDocsSubject",NbMinDocsSubject->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetUInt("NbDocsAssess",NbDocsAssess->value(),"Subjects");
	GALILEIApp->GetGALILEIConfig()->SetDouble("SwitchPerc",SwitchPerc->value(),"Subjects");
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
void Configure::applyPlugIns(GSession* session)
{
	// Apply plug-ins
	Storages->apply(session);
	Filters->apply(session);
	ProfileCalcs->apply(session);
	GroupProfiles->apply(session);
	GroupDocs->apply(session);
	CommunityCalcs->apply(session);
	TopicCalcs->apply(session);
	StatsCalcs->apply(session);
	LinkCalcs->apply(session);
	PostDocs->apply(session);
	PreProfiles->apply(session);
	DocAnalyses->apply(session);
	PostTopics->apply(session);
	Engines->apply(session);
	MetaEngines->apply(session);
	Langs->apply(session);
	PostProfiles->apply(session);
	PostCommunities->apply(session);

	// Apply Measures
	for(int row=0;row<Measures->count();row++)
		dynamic_cast<QPlugInsList*>(Measures->widget(row))->apply(session);

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
