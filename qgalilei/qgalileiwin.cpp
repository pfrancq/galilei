/*

	GALILEI Project.

	QGALILEIWin.cpp

	Main windows - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).

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
#include <cmath>


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>
#include <qgobjectslist.h>
#include <gmetaengine.h>
#include <gtool.h>
#include <qraboutdialog.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QProgressDialog>


//-----------------------------------------------------------------------------
// include files for current project
#include <qgalileiwin.h>
#include <qgalilei.h>
#include <qsessionprogress.h>
#include <kviewdicts.h>
#include <preferences.h>
#include <configure.h>
#include <kviewdoc.h>
#include <kviewmetaengine.h>
#include <kprgconsole.h>
#include <kviewprg.h>
#include <kviewstats.h>
#include <qcreatedatabase.h>
#include <qfilldatabase.h>
#include <ui_qchooselist.h>
#include <kviewgroup.h>
#include <kviewgroups.h>
#include <kviewclasses.h>
#include <kviewclass.h>
#include <kviewprofile.h>
#include <kviewusers.h>
#include <kviewidealgroups.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
const double QGALILEIVersion=0.1;



//-----------------------------------------------------------------------------
//
// class QGALILEIWin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGALILEIWin::QGALILEIWin(QGALILEI* app)
	: QMainWindow(0), App(app), Session(0)
{
	setupUi(this);
	connectMenus();
	sessionConnected();
	setWindowTitle("QGALILEI "+QString::number(QGALILEIVersion));
}


//-----------------------------------------------------------------------------
void QGALILEIWin::connectMenus(void)
{
	// Menu 'Session'
	connect(aConnect,SIGNAL(triggered()),this,SLOT(sessionConnect()));
	connect(aDisconnect,SIGNAL(triggered()),this,SLOT(sessionDisconnect()));
	connect(aComputeSession,SIGNAL(triggered()),this,SLOT(computeSession()));
	connect(aConsole,SIGNAL(triggered()),this,SLOT(console()));
	connect(aRunScript,SIGNAL(triggered()),this,SLOT(runScript()));
	connect(aStatistics,SIGNAL(triggered()),this,SLOT(statistics()));
	connect(aCreateSession,SIGNAL(triggered()),this,SLOT(createSession()));
	connect(aImportDocs,SIGNAL(triggered()),this,SLOT(importDocs()));
	connect(aRunTool,SIGNAL(triggered()),this,SLOT(runTool()));
	connect(aExit,SIGNAL(triggered()),this,SLOT(close()));

	// Menu 'Documents'
	connect(aDicts,SIGNAL(triggered()),this,SLOT(showDicts()));
	connect(aShowDocs,SIGNAL(triggered()),this,SLOT(showDocs()));
	connect(aExportDocDecs,SIGNAL(triggered()),this,SLOT(exportDocDecs()));
	connect(aIndexDocs,SIGNAL(triggered()),this,SLOT(indexDocs()));
	connect(aClearDocs,SIGNAL(triggered()),this,SLOT(clearDocs()));
	connect(aAnalyzeDocs,SIGNAL(triggered()),this,SLOT(analyzeDocs()));
	connect(aExportDocs,SIGNAL(triggered()),this,SLOT(exportDocs()));
	connect(aAnalyzeDoc,SIGNAL(triggered()),this,SLOT(analyzeDoc()));
	connect(aQuery,SIGNAL(triggered()),this,SLOT(queryMetaEngine()));
	connect(aComputeTrust,SIGNAL(triggered()),this,SLOT(computeTrust()));
	connect(aComputeSugs,SIGNAL(triggered()),this,SLOT(computeSugs()));

	// Menu 'Topics'
	connect(aShowClasses,SIGNAL(triggered()),this,SLOT(showClasses()));
	connect(aShowTopics,SIGNAL(triggered()),this,SLOT(showTopics()));
	connect(aClearTopics,SIGNAL(triggered()),this,SLOT(clearTopics()));
	connect(aGroupDocs,SIGNAL(triggered()),this,SLOT(groupDocs()));

	// Menu 'Users'
	connect(aShowUsers,SIGNAL(triggered()),this,SLOT(showUsers()));
	connect(aClearProfiles,SIGNAL(triggered()),this,SLOT(clearProfiles()));
	connect(aComputeProfiles,SIGNAL(triggered()),this,SLOT(computeProfiles()));
	connect(aComputeProfile,SIGNAL(triggered()),this,SLOT(computeProfile()));

	// Menu 'Communities'
	connect(aShowCommunities,SIGNAL(triggered()),this,SLOT(showCommunities()));
	connect(aClearCommunities,SIGNAL(triggered()),this,SLOT(clearCommunities()));
	connect(aGroupProfiles,SIGNAL(triggered()),this,SLOT(groupProfiles()));

	// Menu 'Simulation'
	connect(aRepairSubjects,SIGNAL(triggered()),this,SLOT(repairSubjects()));
	connect(aInitSimulation,SIGNAL(triggered()),this,SLOT(initSimulation()));
	connect(aCreateIdealClasses,SIGNAL(triggered()),this,SLOT(createIdealClasses()));
	connect(aCreateIdealClassesDocs,SIGNAL(triggered()),this,SLOT(createIdealClassesDocs()));
	connect(aCreateIdealTopics,SIGNAL(triggered()),this,SLOT(createIdealTopics()));
	connect(aCreateIdealTopicsClasses,SIGNAL(triggered()),this,SLOT(createIdealTopicsClasses()));
	connect(aCreateIdealCommunities,SIGNAL(triggered()),this,SLOT(createIdealCommunities()));
	connect(aFeedbackCycle,SIGNAL(triggered()),this,SLOT(feedbackCycle()));
	connect(aAssessmentCycle,SIGNAL(triggered()),this,SLOT(assessmentCycle()));
	connect(aCompareIdealClasses,SIGNAL(triggered()),this,SLOT(compareIdealClasses()));
	connect(aCompareIdealTopics,SIGNAL(triggered()),this,SLOT(compareIdealTopics()));
	connect(aCompareIdealCommunities,SIGNAL(triggered()),this,SLOT(compareIdealCommunities()));

	// Menu 'Window'
	connect(aCloseAll,SIGNAL(triggered()),Desktop,SLOT(closeAllSubWindows()));
	connect(aTile,SIGNAL(triggered()),Desktop,SLOT(tileSubWindows()));
	connect(aCascade,SIGNAL(triggered()),Desktop,SLOT(cascadeSubWindows()));

	// Menu 'Settings'
	connect(aConfigSession,SIGNAL(triggered()),this,SLOT(configSession()));
	connect(aConfigQGALILEI,SIGNAL(triggered()),this,SLOT(configQGALILEI()));

	// Menu 'Help'
	connect(aAbout,SIGNAL(triggered()),this,SLOT(about()));
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionConnected(void)
{
	// Menu 'Session'
	aConnect->setEnabled(Session==0);
	aDisconnect->setEnabled(Session);
	aComputeSession->setEnabled(Session);
	aConsole->setEnabled(Session);
	aRunScript->setEnabled(Session);
	aStatistics->setEnabled(Session);
	aCreateSession->setEnabled(Session);
	aImportDocs->setEnabled(Session);
	aRunTool->setEnabled(Session);

	// Menu 'Documents'
	aDicts->setEnabled(Session);
	aShowDocs->setEnabled(Session);
	aExportDocDecs->setEnabled(Session);
	aIndexDocs->setEnabled(Session);
	aClearDocs->setEnabled(Session);
	aAnalyzeDocs->setEnabled(Session);
	aExportDocs->setEnabled(Session);
	aAnalyzeDoc->setEnabled(Session);
	aQuery->setEnabled(Session);
	aComputeTrust->setEnabled(Session);
	aComputeSugs->setEnabled(Session);

	// Menu 'Topics'
	aShowClasses->setEnabled(Session);
	aShowTopics->setEnabled(Session);
	aClearTopics->setEnabled(Session);
	aGroupDocs->setEnabled(Session);

	// Menu 'Users'
	aShowUsers->setEnabled(Session);
	aClearProfiles->setEnabled(Session);
	aComputeProfiles->setEnabled(Session);
	aComputeProfile->setEnabled(false);

	// Menu 'Communities'
	aShowCommunities->setEnabled(Session);
	aClearCommunities->setEnabled(Session);
	aGroupProfiles->setEnabled(Session);

	// Menu 'Simulation'
	aRepairSubjects->setEnabled(Session);
	aInitSimulation->setEnabled(Session);
	aCreateIdealClasses->setEnabled(Session);
	aCreateIdealClassesDocs->setEnabled(Session);
	aCreateIdealTopics->setEnabled(Session);
	aCreateIdealCommunities->setEnabled(Session);
	aFeedbackCycle->setEnabled(Session);
	aAssessmentCycle->setEnabled(Session);
	aCompareIdealClasses->setEnabled(Session);
	aCompareIdealTopics->setEnabled(Session);
	aCompareIdealCommunities->setEnabled(Session);

	// Menu 'Window'
	aCloseAll->setEnabled(Session);
	aTile->setEnabled(Session);
	aCascade->setEnabled(Session);

	// Menu 'Settings'
	aConfigSession->setEnabled(Session);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionConnect(void)
{
	statusBar()->showMessage(tr("Connecting..."));
   bool ok;
   SessionName=FromQString(QInputDialog::getText(this,tr("Connect to a session"),tr("Session name:"),QLineEdit::Normal,ToQString(SessionName),&ok));
   if((!ok)||(SessionName.IsEmpty()))
		return;
	if(QCreateSession(this,Session,SessionName).run())
	{
		sessionConnected();
		statusBar()->showMessage(tr("Connected"));
	}
	else
	{
		statusBar()->showMessage(tr("Ready"));
		Session=0;
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionDisconnect(void)
{
	if(Session)
	{
		Desktop->closeAllSubWindows();
		App->DeleteSession(Session);
		Session=0;
		sessionConnected();
		statusBar()->showMessage(tr("Not Connected!"));
	}
}

//-----------------------------------------------------------------------------
void QGALILEIWin::computeSession(void)
{
	if(QComputeAll(this).run())
	{
		emit docsChanged();
		emit topicsChanged();
		emit profilesChanged();
		emit communitiesChanged();
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::console(void)
{
	KPrgConsole* ptr(new KPrgConsole());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::runScript(void)
{
	QString file(QFileDialog::getSaveFileName(this,"Open Script File...","","*.kprg|KGALILEICenter Programs"));
	if(file.isEmpty())
		return;
	KViewPrg* ptr(new KViewPrg(this,FromQString(file)));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::statistics(void)
{
	KViewStats* ptr(new KViewStats());
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createSession(void)
{
	QCreateDatabase(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::importDocs(void)
{
	QFillDatabase(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::runTool(void)
{
	GPlugInManager* Manager(GALILEIApp->GetManager("Tools"));

	// Create the dialog box
	QDialog Choose(this);
	Ui_QChooseList Ui;
	QWidget* widget(new QWidget(&Choose));
	Ui.setupUi(widget);

	// Init the dialog box with the lists
	Ui.Desc->setText("Choose the tool to run");
	RCursor<GPlugInList> Lists(Manager->GetPlugInLists());
	int Row(0),i;
	for(Lists.Start(),i=0;!Lists.End();Lists.Next(),i++)
	{
		if(Lists()->GetName()==ToolCat)
			Row=i;
		Ui.List->addItem(ToQString(Lists()->GetName()));
	}
	Ui.List->setCurrentRow(Row);
	if(!Choose.exec())
		return;
	ToolCat=FromQString(Ui.List->item(Ui.List->currentRow())->text());

	// Init the dialog box with the plug-ins
	RCastCursor<GPlugIn,GTool> Tools(Manager->GetPlugIns<GTool>(ToolCat));
	Ui.List->clear();
	for(Tools.Start(),i=0,Row=0;!Tools.End();Tools.Next(),i++)
	{
		if(Tools()->GetName()==Tool)
			Row=i;
		Ui.List->addItem(ToQString(Tools()->GetName()));
	}
	Ui.List->setCurrentRow(Row);
	if(!Choose.exec())
		return;

	Tool=FromQString(Ui.List->item(Ui.List->currentRow())->text());
	QRunTool(this,Tool,ToolCat).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showDicts(void)
{
	KViewDicts* ptr(new KViewDicts(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->create();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showDocs(void)
{
	QGObjectsList* ptr(new QGObjectsList());
	connect(ptr,SIGNAL(Show(GDoc*)),this,SLOT(showDoc(GDoc*)));
	ptr->setAttribute(Qt::WA_DeleteOnClose);
	Desktop->addSubWindow(ptr)->setWindowTitle("Documents");
	ptr->Set(Session,QGObjectsList::Docs);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showDoc(GDoc* doc)
{
	KViewDoc* ptr(new KViewDoc(this,doc));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::exportDocDecs(void)
{
	QString dir(QFileDialog::getExistingDirectory(this,"Choose directory where to export the files","",QFileDialog::ShowDirsOnly));
	if(dir.isEmpty())
		return;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QProgressDialog dlg(this);
	dlg.setWindowTitle("Export documents");
	dlg.show();
	QApplication::processEvents();
	RString pre(FromQString(dir));
	pre+="/doc";
	RCursor<GDoc> Docs(Session->GetObjs(pDoc));
	size_t nb;
	dlg.setRange(0,Docs.GetNb());
	dlg.setValue(0);
	for(Docs.Start(),nb=0;!Docs.End();Docs.Next(),nb++)
	{
		if(dlg.wasCanceled())
			break;
		dlg.setValue(static_cast<int>((nb*100)/Docs.GetNb()));
		QApplication::processEvents();
		if(!Docs()->GetLang())
			continue;
		RString name(pre+RString::Number(Docs()->GetId())+".txt");
		dlg.setLabelText(ToQString(name));
		QApplication::processEvents();
		RTextFile file(name);
		file.Open(RIO::Create);
		RConstCursor<GVector> Vector(Docs()->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			RConstCursor<GConceptRef> Ref(Vector()->GetRefs());
			for(Ref.Start();!Ref.End();Ref.Next())
			{
				for(size_t i=lround(Ref()->GetWeight())+1;--i;)
				{
					file<<Session->GetStorage()->LoadConcept(Ref()->GetId());
				}
				file<<endl;
			}
		}
		file.Close();
		dlg.setValue(Docs.GetPos()+1);
	}
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::indexDocs(void)
{
	QIndexDocs(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::clearDocs(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	Session->ReInit(pDoc,false);
	updateWins<KViewDoc>();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::analyzeDocs(void)
{
	if(QAnalyzeDocs(this).run())
		updateWins<KViewDoc>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::exportDocs(void)
{
	QString file(QFileDialog::getSaveFileName(this,"Choose file where to export the files descriptions","","*.*"));
	if(file.isEmpty())
		return;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QProgressDialog dlg(this);
	dlg.setWindowTitle("Export documents");
	dlg.show();
	QApplication::processEvents();
	RTextFile Export(FromQString(file),"utf-8");
	Export.SetAddSeparator(false);
	Export.Open(RIO::Create);
	Export<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
	Export<<"<!DOCTYPE documents>"<<endl;
	Export<<"<documents>"<<endl;
	RCursor<GDoc> Docs(Session->GetObjs(pDoc));
	dlg.setRange(0,Docs.GetNb());
	dlg.setValue(0);
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		Export<<"\t<document id=\""<<Docs()->GetId()<<"\" url=\""<<Docs()->GetURI()()<<"\">"<<endl;
		RConstCursor<GVector> Vector(Docs()->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			Export<<"\t\t<vector type=\""<<Vector()->GetMetaConcept()->GetName()<<"\">"<<endl;
			RConstCursor<GConceptRef> Ref(Vector()->GetRefs());
			for(Ref.Start();!Ref.End();Ref.Next())
			{
				Export<<"\t\t\t<concept>"<<endl;
				Export<<"\t\t\t\t<id>"<<Ref()->GetId()<<"</id>"<<endl;
				Export<<"\t\t\t\t<weight>"<<Ref()->GetWeight()<<"</weight>"<<endl;
				Export<<"\t\t\t</concept>"<<endl;
			}
			Export<<"\t\t</vector>"<<endl;
		}
		Export<<"\t</document>"<<endl;
		dlg.setValue(Docs.GetPos()+1);
	}
	Export<<"</documents>"<<endl;
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::analyzeDoc(void)
{
	QString file(QFileDialog::getOpenFileName(this,"Document to analyse","","*.*"));
	if(file.isEmpty())
		return;
	KViewDoc* ptr(new KViewDoc(this,FromQString(file)));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->AnalyzeDoc();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showClasses(void)
{
	KViewClasses* ptr(new KViewClasses(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showClass(GClass* theclass)
{
	KViewClass* ptr(new KViewClass(this,theclass));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::clearTopics(void)
{
	if(Session)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		Session->ReInit(pTopic);
		updateWins<KViewTopics>();
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::groupDocs(void)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QGroupDocs(this).run();
	emit topicsChanged();
	updateWins<KViewTopic>();
	QApplication::setOverrideCursor(Qt::ArrowCursor);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showTopics(void)
{
	KViewTopics* ptr(new KViewTopics(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showTopic(GTopic* topic)
{
	KViewTopic* ptr(new KViewTopic(this,topic));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::clearProfiles(void)
{
	if(Session)
	{
		Session->ReInit(pProfile,false);
		updateWins<KViewProfile>();
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showUsers(void)
{
	KViewUsers* ptr(new KViewUsers(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showProfile(GProfile* profile)
{
	KViewProfile* ptr(new KViewProfile(this,profile));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::computeProfiles(void)
{
	QComputeProfiles(this).run();
	emit profilesChanged();
	updateWins<KViewProfile>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::computeProfile(void)
{
	KViewProfile* win(dynamic_cast<KViewProfile*>(Desktop->activeSubWindow()));
	if(!win) return;
	win->ComputeProfile();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::clearCommunities()
{
	if(Session)
	{
		Session->ReInit(pCommunity);
		updateWins<KViewCommunities>();
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showCommunities(void)
{
	KViewCommunities* ptr(new KViewCommunities(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showCommunity(GCommunity* community)
{
	KViewCommunity* ptr(new KViewCommunity(this,community));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::groupProfiles(void)
{
	QGroupProfiles(this).run();
	emit communitiesChanged();
	updateWins<KViewCommunity>();
}



//-----------------------------------------------------------------------------
void QGALILEIWin::queryMetaEngine(void)
{
	if(!App->GetCurrentPlugIn<GMetaEngine>("MetaEngine",false))
	{
		QMessageBox::information(this,"Error","No Meta Engine Method selected!!");
		return;
	}
	KViewMetaEngine* ptr(new KViewMetaEngine(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::computeTrust(void)
{
	QComputeTrust(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::computeSugs(void)
{
	QComputeSugs(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::repairSubjects(void)
{
	QRepairSubjects(this).run();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::initSimulation(void)
{
	QInitSimulation(this).run();
	emit docsChanged();
	emit topicsChanged();
	emit profilesChanged();
	emit communitiesChanged();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createIdealTopics(void)
{
	QCreateIdealTopics(this).run();
	emit topicsChanged();
	updateWins<KViewTopics>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createIdealTopicsClasses(void)
{
	QCreateIdealTopicsClasses(this).run();
	emit topicsChanged();
	updateWins<KViewTopics>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createIdealClasses(void)
{
	QCreateIdealClasses(this).run();
	emit topicsChanged();
	updateWins<KViewClasses>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createIdealClassesDocs(void)
{
	QCreateIdealClassesDocs(this).run();
	emit topicsChanged();
	updateWins<KViewClasses>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::createIdealCommunities(void)
{
	QCreateIdealCommunities(this).run();
	emit communitiesChanged();
	updateWins<KViewCommunities>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::feedbackCycle(void)
{
	QFeedbackCycle(this).run();
	emit profilesChanged();
	updateWins<KViewProfile>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::assessmentCycle(void)
{
	QAssessmentCycle(this).run();
	emit profilesChanged();
	updateWins<KViewProfile>();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::compareIdealCommunities(void)
{
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		KViewIdealCommunities* ptr(new KViewIdealCommunities(this));
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
	}
	catch(GALILEI::GException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(R::RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Standard exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::compareIdealTopics(void)
{
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		KViewIdealTopics* ptr(new KViewIdealTopics(this));
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(GALILEI::GException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(R::RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Standard exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::compareIdealClasses(void)
{
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		KViewIdealClasses* ptr(new KViewIdealClasses(this));
		Desktop->addSubWindow(ptr);
		ptr->adjustSize();
		ptr->show();
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
	catch(GALILEI::GException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(R::RException& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("R Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	catch(std::exception& e)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Standard exception"),QWidget::trUtf8(e.what()),QMessageBox::Ok);
	}
	catch(...)
	{
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		QMessageBox::critical(0,QWidget::tr("Unknown exception"),QWidget::trUtf8("Unknown problem"),QMessageBox::Ok);
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::configQGALILEI(void)
{
	Preferences Dlg(this);
	Dlg.exec();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::configSession(void)
{
	Configure Dlg(this);
	Dlg.exec();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::about(void)
{
	QRAboutDialog dlg("QGALILEI","1.0");
	dlg.setDescription("Qt-based GALILEI frontend.");
	dlg.setCopyright(QWidget::trUtf8("(C) 2001-2008 by the Université Libre de Bruxelles (ULB)<br/>(C) 2010-2015 by the Paul Otlet Institute"));
	dlg.setURL("http://www.otlet-institute.org/GALILEI_Platform_en.html");
	dlg.setLicense(QRAboutDialog::License_GPL);
	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
	dlg.addAuthor(QWidget::trUtf8("Faiza Gaultier"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Julien Lamoral"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Nicolas Kumps"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Marjorie Paternostre"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Stéphane Rideau"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Sarah Rolfo"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Xavier Sacré"),QWidget::trUtf8("CMake Support"));
	dlg.addAuthor(QWidget::trUtf8("Marco Saerens"),QWidget::trUtf8("(Scientific Adviso"));
	dlg.addAuthor(QWidget::trUtf8("Jean-Baptiste Valsamis"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("Valéry Vandaele"),QWidget::trUtf8("Past Researcher"));
	dlg.addAuthor(QWidget::trUtf8("David Wartel"),QWidget::trUtf8("Past Researcher"));
	dlg.exec();
}


//-----------------------------------------------------------------------------
QGALILEIWin::~QGALILEIWin(void)
{
	App=0;
}
