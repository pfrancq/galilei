/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <pwd.h>
#include <unistd.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlfile.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <sessions/gsessionmysql.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gsubprofilevector.h>
#include <groups/ggroup.h>
#include <groups/gsubject.h>
#include <groups/gsubjecttree.h>
#include <sessions/gmixidealgroups.h>
#include <langs/glang.h>
#include <langs/gdict.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qprinter.h>
#include <qmessagebox.h>
#include <qworkspace.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qspinbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaction.h>
#include <kapp.h>
#include <kmessagebox.h>
#include <kmimetype.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <kpopupmenu.h>
#include <kstatusbar.h>
#include <kurlrequester.h>


//-----------------------------------------------------------------------------
// include files for process running
#include <kprocess.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewdocs.h"
#include "kviewdoc.h"
#include "kviewusers.h"
#include "kviewstats.h"
#include "kviewthgroups.h"
#include "kviewgroups.h"
#include "kviewgroup.h"
#include "kviewprg.h"
#include "kviewstems.h"
#include "kviewprofile.h"
#include "kviewr.h"
#include "kviewhistory.h"
#include "qconnectmysql.h"
#include "qsessionprogress.h"
#include "qmixidealconfig.h"
#include "qcreatedatabase.h"
#include "qfilldatabase.h"
#include "qhistorydlg.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDoc* KGALILEICenterApp::getDocument(void) const
{
	return(Doc);
}


//-----------------------------------------------------------------------------
GSession* KGALILEICenterApp::getSession(void) const
{
	if(!Doc)
		return(0);
	return(Doc->GetSession());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionConnect(void)
{
	QConnectMySQL dlg(this,0,true);
	QString method;
	GSessionMySQL* Sess;

	dlg.txtDb->setText(dbName.Latin1());
	dlg.txtLogin->setText(dbUser.Latin1());
	dlg.txtPwd->setText(dbPwd.Latin1());
	dlg.txtHost->setText(dbHost.Latin1());
	if(dlg.exec())
	{
		slotStatusMsg(i18n("Connecting..."));
		dbName=dlg.txtDb->text().latin1();
		dbHost=dlg.txtHost->text().latin1();
		dbUser=dlg.txtLogin->text().latin1();
		dbPwd=dlg.txtPwd->text().latin1();
		try
		{
			Sess = new GSessionMySQL(dbHost,dbUser,dbPwd,dbName,&SessionParams,true);
			unsigned int cmd=dlg.cbLoad->currentItem();
			QSessionProgressDlg* d=new QSessionProgressDlg(this,Sess,"Loading from Database");
			d->LoadSession(cmd,&Langs,&URLManager,&DocAnalyseManager,&ProfilingManager,&GroupingManager,&GroupCalcManager,&StatsCalcManager,&LinkCalcManager,&PostDocManager);
			Doc=new KDoc(this,Sess);
			sessionDisconnect->setEnabled(true);
			sessionCompute->setEnabled(true);
			sessionStats->setEnabled(true);
			sessionConnect->setEnabled(false);
			wordsClustering->setEnabled(true);
			removeCluster->setEnabled(true);
			showGroupsHistory->setEnabled(true);
 			rRunR->setEnabled(true);
			textFrench->setEnabled(true);
			textEnglish->setEnabled(true);
			runProgram->setEnabled(true);
			UpdateMenusEntries();
			dbStatus->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("connect_established",KIcon::Small)));
		}
		catch(GException& e)
		{
			QMessageBox::critical(this,"KGALILEICenter",e.GetMsg().Latin1());
			if(Doc)
			{
				delete Doc;
				Doc=0;
			}
		}
		catch(R::RMySQLError& e)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString(e.GetError()));
			if(Doc)
			{
				delete Doc;
				Doc=0;
			}
		}
		slotStatusMsg(i18n("Ready"));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionAutoConnect(const char* host,const char* user,const char* passwd,const char* db)
{
	QConnectMySQL dlg(this,0,true);
	GSessionMySQL* Sess;
	Sess = new GSessionMySQL(host,user,passwd,db,&SessionParams,true);
	unsigned int cmd=dlg.cbLoad->currentItem();
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Sess,"Loading from Database");
	d->LoadSession(cmd,&Langs,&URLManager,&DocAnalyseManager,&ProfilingManager,&GroupingManager,&GroupCalcManager,&StatsCalcManager,&LinkCalcManager,&PostDocManager);
	Doc=new KDoc(this,Sess);
	sessionDisconnect->setEnabled(true);
	sessionCompute->setEnabled(true);
	sessionConnect->setEnabled(false);
	wordsClustering->setEnabled(true);
	removeCluster->setEnabled(true);
	showGroupsHistory->setEnabled(true);
 	rRunR->setEnabled(true);
	textFrench->setEnabled(true);
	textEnglish->setEnabled(true);
	runProgram->setEnabled(true);
	sessionStats->setEnabled(true);
	UpdateMenusEntries();
	dbStatus->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("connect_established",KIcon::Small)));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Complete Session");
	d->ComputeAll(!sessionAlwaysCalc->isChecked(),profileAlwaysSave->isChecked()||groupAlwaysSave->isChecked());
	Doc->updateAllViews(0);
	Doc->updateAllViews(1);
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDisconnect(void)
{
	if(Doc)
	{
		Doc->closeDocument();
		delete Doc;
		Doc=0;
	}
	DisableAllActions();
	sessionConnect->setEnabled(true);
	statusBar()->changeItem("Not Connected !",1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionStats(void)
{
	createClient(Doc,new KViewStats(Doc,pWorkspace,"View statistics",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionQuit(void)
{
	slotStatusMsg(i18n("Exiting..."));
	saveOptions();
	slotStatusMsg(i18n("Ready."));
	close();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCreateDatabase(void)
{
//	RString arg = RString(" ");
	QString strTmp;
	QString modeleDb , stopList , users;
	ErrMsgList = QString();

	KApplication::kApplication()->processEvents();
	QCreateDatabase dlg(this,0,true);

	dlg.DBdatabasesql->setURL("$HOME/prj/kgalileicenter/kgalileicenter/DbModel.sql");
	dlg.DBstoplistsql->setURL("$HOME/prj/kgalileicenter/kgalileicenter/DbStopList.sql");
	dlg.DBUserssql->setURL("$HOME/prj/kgalileicenter/kgalileicenter/DbUsers.sql");

	// set the moreGroup unvisible
	dlg.moreGroup->hide();
	dlg.adjustSize();

	// after ok button clicked
	if(dlg.exec())
	{
		QString dbName = dlg.DbName->text().latin1();
		QString host = dlg.hostName->text().latin1();
		QString user = dlg.userName->text().latin1();
		QString pass = dlg.password->text().latin1();
		if (pass.length() == 0){pass="\\  ";} // put a space caracter if no password is entered

		// if the database name field is empty -> ERROR
		if( dbName.length() == 0)
	    {
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a name for the database! "));
			return;
		}

		// if the user or host is not specified -> ERROR
		if( (host.length()== 0) || (user.length() == 0))
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a user and a host for the database! "));
			return;
		}


		//************************* ------------- Find a Database Modele -----------------------*************************
		strTmp = dlg.DBdatabasesql->url();
		// if no url is specified then return error
		if (strTmp.length() == 0)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must select a database shema file ! "));
			return;
		}

		if (strTmp.find("://",0) < 0 )   // if the file is a locale one -> ok
		{
			modeleDb = strTmp;
		}
		else
		{
			QString tmp= QString("");
			if (!KIO::NetAccess::download(KURL(*strTmp),tmp) )     // if the file can't be downloaded ->error
			{
				QMessageBox::critical(this,"KGALILEICenter",QString("Can't download url : ") + *strTmp );
				return;
			}
			modeleDb = tmp.latin1();
		}

		//************************* ------------- Find a kwds Stop List  -----------------------*************************
		strTmp = dlg.DBstoplistsql->url();
		// if no url is specified then return error
		if (strTmp.length() == 0)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must select a Database Stop List File ! "));
			return;
		}

		if (strTmp.find("://",0) < 0 )   // if the file is a locale one -> ok
		{
			stopList = strTmp;
		}
		else
		{
			QString tmp= QString("");
			

			if (!KIO::NetAccess::download(KURL(*strTmp),tmp) )     // if the file can't be downloaded ->error
			{
				QMessageBox::critical(this,"KGALILEICenter",QString("Can't download url : ") + *strTmp );
				return;
			}
			stopList = tmp.latin1();
		}


		//************************* ------------- Find a  List of Users -----------------------*************************
		strTmp = dlg.DBUserssql->url();
		// if no url is specified then return error
		if (strTmp.length() == 0)
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must select a Database Users File ! "));
			return;
		}

		if (strTmp.find("://",0) < 0 )   // if the file is a locale one -> ok
		{
			users = strTmp;
		}
		else
		{
			QString tmp= QString("");
			

			if (!KIO::NetAccess::download(KURL(*strTmp),tmp) )     // if the file can't be downloaded ->error
			{
				QMessageBox::critical(this,"KGALILEICenter",QString("Can't download url : ") + *strTmp );
				return;
			}
			users = tmp.latin1();
		}

		//************************* ---------find the OS used----------*************************
		cout <<"text="<<dlg.execFile->currentText().latin1()<<endl; //scriptName
		strTmp = dlg.execFile->currentText().latin1(); //scriptName
		if (strTmp.find("Linux",0) <0)
		{
			QMessageBox::critical(this,"KGALILEICenter", *strTmp + QString("version : Not yet implemented !")  );
			return;
		}

		// enter all the arg in this sequence:
		// scriptName "DbName" "host" "user" "pass" "DbModele" "DbStopList  "DbUsers"  ...
		RString cmdline = RString("");
		cmdline+= "./DbCreation-mysql ";
		cmdline+=" ";
		cmdline+= dbName;
		cmdline+= " ";
		cmdline+= host;
		cmdline+= " ";
		cmdline+= user;
		cmdline+= " ";
		cmdline+= pass;
		cmdline+=" ";
		cmdline+= modeleDb;
		cmdline+= " ";
		cmdline+= stopList;
		cmdline+= " ";
		cmdline+= users;

		cmdline+= "\n";

		cout <<"commande line "<<cmdline<<endl;
		// creation of the database using a shell script.
		KShellProcess *process = new KShellProcess("/bin/bash");

		connect(process,SIGNAL(receivedStdout(KProcess*,char*,int)),this,SLOT(slotStdout(KProcess*,char*,int)));
		connect(process,SIGNAL(receivedStderr(KProcess*,char*,int)),this,SLOT(slotStderr(KProcess*,char*,int)));
		connect(process,SIGNAL(processExited(KProcess*)),this,SLOT(slotProcessExited(KProcess*)));

		d=new QSessionProgressDlg(this,0,"create database ...");       
		d->Begin();

		*process << cmdline;
		 process->start(KProcess::NotifyOnExit,KProcess::All);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotFillEmptyDb(void)
{
	KShellProcess *process = new KShellProcess("/bin/bash");
	QFillEmptyDatabase dlg(this,0,true);
	dlg.KUScript->setURL("$HOME/prj/kgalileicenter/kgalileicenter/CreateDbFromDir.py");
	dlg.KUDirectory->setMode(KFile::Directory);
	dlg.LEHost->setText("127.0.0.1");
	dlg.LEUser->setText("root");

	if(dlg.exec())
	{
		QString catdirectory  = dlg.KUDirectory->url();
		QString scriptpath = dlg.KUScript->url();
		QString name = dlg.LEName->text();
		QString host = dlg.LEHost->text();
		QString user = dlg.LEUser->text();
		QString password = dlg.LEPassword->text();
		QString lang = dlg.CBLang->currentText();
		QString mime = dlg.CBMimeType->currentText();

		if (password=="")
			password="no";

		RString cmdline = RString("");
		cmdline+= "python  ";
		cmdline+=scriptpath;
		cmdline+=" ";
		cmdline+= name;
		cmdline+= " ";
		cmdline+= host;
		cmdline+= " ";
		cmdline+= user;
		cmdline+= " ";
		cmdline+= password;
		cmdline+= " ";
		cmdline+= lang;
		cmdline+= " ";
		cmdline+= mime;
		cmdline+= " ";
		cmdline+= catdirectory;
		cmdline+= "\n";
		cout << cmdline<<endl;

		// creation of the database using a shell script.
		d=new QSessionProgressDlg(this,0,"filling database ...");
		d->Begin();
		connect(process,SIGNAL(receivedStderr(KProcess*,char*,int)),this,SLOT(slotStderr(KProcess*,char*,int)));
		connect(process,SIGNAL(processExited(KProcess*)),this,SLOT(slotProcessExited(KProcess*)));
		*process << cmdline;
		process->start(KProcess::NotifyOnExit,KProcess::All);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotStdout(KProcess* /*proc*/,char* buffer,int buflen)
{
	char tmp[500];
  strncpy(tmp,buffer,buflen);
  tmp[buflen-1] = '\0';
	d->PutText(tmp);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotStderr(KProcess* /*proc*/,char* buffer,int buflen)
{
	char tmp[250];
	strncpy(tmp,buffer,buflen);
	tmp[buflen] = '\0';
	ErrMsgList+=tmp;
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProcessExited(KProcess*)
{
	if ( !ErrMsgList.isEmpty())
	{
		d->close();
		QMessageBox::critical(this,"KGALILEICenter",ErrMsgList);
	}
  else
	d->Finish();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowUsers(void)
{
	createClient(Doc,new KViewUsers(Doc,pWorkspace,"View users",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfileCalc(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gProfile) return;
	((KViewProfile*)m)->ComputeProfile();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesCalc(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Profiles");
	d->ComputeProfiles(!profileAlwaysCalc->isChecked(),profileAlwaysSave->isChecked());
	Doc->updateAllViews(1);

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowGroups(void)
{
	createClient(Doc,new KViewGroups(Doc,pWorkspace,"View Groups",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsCalc(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Make Groups");
	d->GroupProfiles(!groupAlwaysCalc->isChecked(),groupAlwaysSave->isChecked());
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupingCompareFromFile(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.grp|Ideal Groupement files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewThGroups(Doc,url.path(),pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupingCompare(void)
{
	if(Doc->GetSession()->GetSubjects())
		createClient(Doc,new KViewThGroups(Doc,Doc->GetSession()->GetSubjects()->GetIdealGroups(),pWorkspace,"View Theoritical Groups",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowDocs(void)
{
	createClient(Doc,new KViewDocs(Doc,pWorkspace,"View Docs",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocAnalyse(void)
{
	KViewDoc* m;

	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.*"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,m=new KViewDoc(url.path(),KMimeType::findByURL(url)->name(),Doc,pWorkspace,"View Document",0));
		m->CreateDocXML();
		slotWindowActivated(m);
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsAnalyse(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Documents");
	d->AnalyseDocs(!docAlwaysCalc->isChecked());
	Doc->updateAllViews(0);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotCreateXML(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	((KViewDoc*)m)->CreateDocXML();
	slotWindowActivated(m);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSaveXML(void)
{
	int dlg;
	KURL url;

	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.docxml"), this, i18n("Save DocXML File..."));
		if(url.isEmpty()) return;
		QFile Test(url.path().latin1());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}
	((KViewDoc*)m)->SaveDocXML(url.path());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotAnalyseXML(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gDoc) return;
	((KViewDoc*)m)->AnalyseDocXML();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWordsClustering()
{
	Doc->GetSession()->AnalyseAssociation(wordsClusteringSave->isChecked());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRemoveCluster()
{
	Doc->GetSession()->RemoveAssociation();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTextFrench(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.mm|MMorph dictionnary files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewStems("fr",url.path(),Doc,pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotTextEnglish(void)
{
	slotStatusMsg(i18n("Opening file..."));
	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString::null,i18n("*.mm|MMorph dictionnary files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		createClient(Doc,new KViewStems("en",url.path(),Doc,pWorkspace,"View Theoritical Groups",0));
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowHistory(void)
{
	GFactoryLangCursor curlang;
	GLang* lang;
	unsigned int size, min, max;

	curlang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	size=Doc->GetSession()->GetHistorySize();

	QHistoryDlg dlg(this,0,true);
	dlg.TLMaxHistory->setText(QString("Max Historic ID (<")+dtou(size).Latin1()+QString(")"));
	dlg.SBMaxId->setMaxValue(size-1);
	for (curlang.Start(); !curlang.End(); curlang.Next())
	{
		lang=curlang()->GetPlugin();
		if(!lang) continue;
		dlg.CBLang->insertItem(lang->GetCode());
	}

	KApplication::kApplication()->processEvents();
	if(dlg.exec())
	{
		min=dlg.SBMinId->value() ;
		max=dlg.SBMaxId->value();
		createClient(Doc,new KViewHistory(Doc,dlg.CBLang->currentText(),dlg.CBGlobal->isChecked(),pWorkspace,"Show Chromosomes",0,min,max));
	}
}

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotViewToolBar(void)
{
	slotStatusMsg(i18n("Toggle the toolbar..."));
	if(!viewToolBar->isChecked())
	{
		toolBar("mainToolBar")->hide();
	}
	else
	{
		toolBar("mainToolBar")->show();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotViewStatusBar(void)
{
	slotStatusMsg(i18n("Toggle the statusbar..."));
	if(!viewStatusBar->isChecked())
	{
		statusBar()->hide();
	}
	else
	{
		statusBar()->show();
	}
	slotStatusMsg(i18n("Ready."));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::windowMenuAboutToShow(void)
{
	windowMenu->popupMenu()->clear();
	windowMenu->insert(windowCascade);
	windowMenu->insert(windowTile);
	if(pWorkspace->windowList().isEmpty())
	{
		windowCascade->setEnabled(false);
		windowTile->setEnabled(false);
	}
	else
	{
		windowCascade->setEnabled(true);
		windowTile->setEnabled(true);
	}
	windowMenu->popupMenu()->insertSeparator();

	QWidgetList windows = pWorkspace->windowList();
	for(int i=0;i<int(windows.count());++i)
	{
		int id = windowMenu->popupMenu()->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(),this,SLOT(windowMenuActivated(int)));
		windowMenu->popupMenu()->setItemParameter(id,i);
		windowMenu->popupMenu()->setItemChecked(id,pWorkspace->activeWindow()==windows.at(i));
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::windowMenuActivated(int id)
{
	QWidget* w = pWorkspace->windowList().at( id );
	if(w)
	w->setFocus();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowTile(void)
{
	pWorkspace->tile();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowCascade(void)
{
	pWorkspace->cascade();
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotStatusMsg(const QString& text)
{
	statusBar()->clear();
	statusBar()->changeItem(text,1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRunProgram(void)
{
	QString tmpfile;
	char tmp[100];
	KViewPrg* o;

	KApplication::kApplication()->processEvents();
	KURL url=KFileDialog::getOpenURL(QString(getpwuid(getuid())->pw_dir)+QString("/galilei/prg"),i18n("*.kprg|KGALILEICenter Programs"), this, i18n("Open File..."));
	if(url.isEmpty())
	{
		QMessageBox::critical(this,"KGALILEICenter","File could not be found");
		return;
	}
	KIO::NetAccess::download(url,tmpfile);
	strcpy(tmp,tmpfile);
	try
	{
		createClient(Doc,o=new KViewPrg(Doc,pWorkspace,tmp,0));
		KApplication::kApplication()->processEvents();
		Doc->GetSession()->RunPrg(o,tmp);
 		QMessageBox::information(this,"KGALILEICenter","Program Executed");
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",e.GetMsg().Latin1());
	}
	catch(R::RMySQLError& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",e.GetError());
	}
	catch(bad_alloc)
	{
		QMessageBox::critical(this,"KGALILEICenter","Memory Error");
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter","Undefined Error");
	}
	KIO::NetAccess::removeTempFile( tmpfile );
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotMixIdealGroups(void)
 {
	GMixIdealGroups* mix;
	const char *nbgroups, *level;
	bool random, ideal, merge, split;

	try
	{
		QMixIdealConfig dlg(this,0,true);
		if(!dlg.exec()) return;
		slotStatusMsg(i18n("Connecting..."));
		nbgroups=dlg.LENbMixedGroups->text().latin1();
		level=dlg.LELevel->text().latin1();
		random=dlg.RBRandom->isChecked();
		ideal=dlg.RBIdeal->isChecked();
		merge=dlg.RBMerge->isChecked();
		split=dlg.RBSplit->isChecked();

		// Initialise the dialog box
		QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Mix Ideal Groups");
		d->Begin();

		// Init Part
		d->PutText("Initialisation");

		// Create new solution
		d->PutText("Create new solutions");
		mix = new GMixIdealGroups(Doc->GetSession(),(Doc->GetSession()->GetSubjects()->GetIdealGroups()), atoi(nbgroups), atoi(level), merge, split, random,ideal);
		mix->Run(d);
		d->Finish();
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",e.GetMsg().Latin1());
	}
	catch(R::RMySQLError& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",QString(e.GetError()));
	}
	catch(bad_alloc)
	{
		QMessageBox::critical(this,"KGALILEICenter","Memory Error");
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter","Undefined Error");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotWindowActivated(QWidget*)
{
	KView* m = dynamic_cast<KView*>(pWorkspace->activeWindow());
	GViewType type;

	if(m)
	{
		// Update caption
		setCaption(m->caption());
		type=m->getType();
	}
	else
	{
		setCaption("");
		type=gNothing;
	}

	// Update menu
	switch(type)
	{
		case gUsers:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			break;

		case gDocs:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			break;

		case gDoc:
			createXML->setEnabled(!(((KViewDoc*)m)->IsDocXML()));
			saveXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			analyseXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			profileCalc->setEnabled(false);
			break;

		case gProfile:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(true);
			break;

		case gGroups:
			saveXML->setEnabled(false);
			createXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			break;

		case gGA:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			break;

		case gNothing:
		default:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			profileCalc->setEnabled(false);
			break;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotHandleItem(QListViewItem* item)
{
	QListViewItemType* obj=(QListViewItemType*)item;

	switch(obj->Type)
	{
		case QListViewItemType::tProfile:
			createClient(Doc,new KViewProfile(obj->Obj.Profile,Doc,pWorkspace,"View Profile",0));
			break;

		case QListViewItemType::tDocument:
			createClient(Doc,new KViewDoc(obj->Obj.Doc,Doc,pWorkspace,"View Document",0));
			break;

		case QListViewItemType::tGroup:
			createClient(Doc,new KViewGroup(obj->Obj.Group,Doc,pWorkspace,"View Group",0));
			break;

		case QListViewItemType::tNothing:
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotRRunR(void)
{
	KApplication::kApplication()->processEvents();
	createClient(Doc,new KViewR(Doc,pWorkspace,"Run R",0));
}


//-----------------------------------------------------------------------------
KGALILEICenterApp::~KGALILEICenterApp(void)
{
	delete Printer;
	if(Doc)
		delete Doc;
}
