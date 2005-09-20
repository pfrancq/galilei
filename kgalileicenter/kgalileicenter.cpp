/*

	GALILEI Research Project

	KGALILEICenter.cpp

	Main Window - Implementation of the Slots Part.

	Copyright 2001-2002 by the Universit�Libre de Bruxelles.

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
#include <pwd.h>
#include <unistd.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rmysql.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocxml.h>
#include <gstorage.h>
#include <gsubprofile.h>
#include <gsubprofile.h>
#include <ggroup.h>
#include <gsubject.h>
#include <gsubjects.h>
#include <glang.h>
#include <gdict.h>
#include <gweightinfo.h>
#include <gpluginmanagers.h>
using namespace GALILEI;
using namespace std;


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
#include <kdatepicker.h>


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
#include <kprogress.h>
#include <kprocess.h>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewdocs.h"
#include "kviewdoc.h"
#include "kviewmetaengine.h"
#include "kviewusers.h"
#include "kviewstats.h"
#include "kviewthgroups.h"
#include "kviewgroups.h"
#include "kviewgroup.h"
#include "kviewprg.h"
#include "kviewstems.h"
#include "kviewprofile.h"
#include "kviewhistory.h"
#include "kviewsom.h"
#include "qsessionprogress.h"
#include "qcreatedatabase.h"
#include "qfilldatabase.h"
#include "qhistorydlg.h"
#include "qexportmatrixdlg.h"
#include "qchoosemap.h"
#include "qsimulationdlg.h"



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
	QString method;
	GSession* Sess;

	slotStatusMsg(i18n("Connecting..."));
	try
	{
		Doc=new KDoc(this);
		Sess = new GSession(this);
		Doc->SetSession(Sess);
		Sess->Init();
			slotSaveModifier();
		QSessionProgressDlg dlg(this,Sess,"Loading from Database");
		if(dlg.Run(new QLoadSession()))
		{
			sessionConnect->setEnabled(false);
			UpdateMenusEntries();
			dbStatus->setPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("connect_established",KIcon::Small)));
		}
		else
		{
			if(Doc)
			{
				delete Doc;
				Doc=0;
			}
		}
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
/*			if(Doc)
			{
				delete Doc;*/
			Doc=0;
//			}
/*			if(d)
				d->close();*/
		return;
	}
	slotStatusMsg(i18n("Connected"));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionCompute(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Complete Session");
	if(!Dlg.Run(new QComputeAll()))
		return;
	Doc->updateAllViews(0);
	Doc->updateAllViews(1);
	Doc->updateAllViews(2);

	if(GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
	{
		Doc->updateAllViews(3);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionDisconnect(void)
{
	try
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
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",e.GetMsg());
	}
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

	RString strTmp;

	//Init dlg box
	QCreateDatabase dlg(this,0,true);
	dlg.UseStopList->setChecked(CreateDbUseStopList);
	dlg.UseUsers->setChecked(CreateDbUseUsers);
	dlg.sqlPath->setURL(ToQString(CreateDbSQLpath));
	//Only a directory can be choosed
	dlg.sqlPath->setMode(2);
	// set the moreGroup unvisible
	dlg.moreGroup->hide();
	dlg.adjustSize();

	// after ok button clicked
	if(dlg.exec())
	{
		//Get new values
		RString dbName = FromQString(dlg.DbName->text());
		RString host = FromQString(dlg.hostName->text());
		RString user = FromQString(dlg.userName->text());
		RString pass = FromQString(dlg.password->text());

		CreateDbUseStopList = dlg.UseStopList->isChecked();
		CreateDbUseUsers= dlg.UseUsers->isChecked();

		// if the database name field is empty -> ERROR
		if( dbName.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a name for the database! "));
			return;
		}

		// if the user or host is not specified -> ERROR
		if( (host.IsEmpty()) || (user.IsEmpty()))
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a user and a host for the database! "));
			return;
		}

		//************************* ------------- Find a Database Modele -----------------------*************************
		strTmp = FromQString(dlg.sqlPath->url());
		// if no url is specified then return error
		if (strTmp.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must select the path to the SQL files! "));
			return;
		}
		CreateDbSQLpath=strTmp;

		//************************* ---------find the OS used----------*************************
		strTmp = FromQString(dlg.execFile->currentText()); //scriptName
		if (strTmp.FindStr("Linux") <0)
		{
			QMessageBox::critical(this,"KGALILEICenter", *strTmp + QString("version : Not yet implemented !")  );
			return;
		}

		try
		{
			//Create de DB
			QSessionProgressDlg d(this,0,"Create Database");
			if(!d.Run(new QCreateDB(dbName,host,user,pass,CreateDbSQLpath,CreateDbUseStopList,CreateDbUseUsers)))
				return;
		}
		catch(GException& e)
		{
			QMessageBox::critical(this,"KGALILEICenter - GALILEI Exception",e.GetMsg());
			return;
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotFillEmptyDb(void)
{
	QFillEmptyDatabase dlg(this,0,true);
	dlg.KUDirectory->setMode(KFile::Directory);
	dlg.LEHost->setText("127.0.0.1");
	dlg.LEUser->setText("root");

	if(dlg.exec())
	{
		RString catDirectory  = FromQString(dlg.KUDirectory->url());
		RString dbname = FromQString(dlg.LEName->text());
		RString host = FromQString(dlg.LEHost->text());
		RString user = FromQString(dlg.LEUser->text());
		RString password = FromQString(dlg.LEPassword->text());
		int depth = dlg.Depth->value();
		RString parentName = FromQString(dlg.ParentName->text());

		// if the database name field is empty -> ERROR
		if( dbname.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a name for the database to fill! "));
			return;
		}

		// if the user or host is not specified -> ERROR
		if( (host.IsEmpty()) || (user.IsEmpty()))
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a user and a host for the database! "));
			return;
		}

		// if the database name field is empty -> ERROR
		if( catDirectory.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must specify a directory containing all the categories! "));
			return;
		}
		if((dlg.HasParent->isChecked())&& parentName.IsEmpty())
		{
			QMessageBox::critical(this,"KGALILEICenter",QString("You must insert a NAME for the parent or unchecked the \"Has Parent\" option! "));
			return;
		}

		QSessionProgressDlg Dlg(this,0,"Fill Database");
		if(!Dlg.Run(new QFillDB(dbname,host,user,password,catDirectory,depth,parentName,GPluginManagers::GetManager<GFilterManager>("Filter"))))
			return;
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowUsers(void)
{
	createClient(Doc,new KViewUsers(Doc,pWorkspace,"View users",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfilesCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Profiles");
	if(!Dlg.Run(new QComputeProfiles()))
		return;
	Doc->updateAllViews(1);
	//test whether a linking method has been used during Profile computation.
	//if true -->refresh Links
	if(GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
	{
		Doc->updateAllViews(3);
	}

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotProfileCalc(void)
{
	KView* m = (KView*)pWorkspace->activeWindow();
	if(m->getType()!=gProfile) return;
	((KViewProfile*)m)->ComputeProfile();
	slotWindowActivated(m);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotShowGroups(void)
{
	createClient(Doc,new KViewGroups(Doc,pWorkspace,"View Groups",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsCalc(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Make Groups");
	if(!Dlg.Run(new QGroupProfiles()))
		return;
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSimulationDlg(void)
{
	QSimulationDlg dlg(this);

	dlg.PercOK->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercOK")));
	dlg.PercKO->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercKO")));
	dlg.PercH->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercH")));
	dlg.PercErr->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercErr")));
	dlg.NbProfMin->setText(QString::number(Doc->GetSession()->GetSubjects()->GetUInt("NbProfMin")));
	dlg.NbProfMax->setText(QString::number(Doc->GetSession()->GetSubjects()->GetUInt("NbProfMax")));
	dlg.PercSocial->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercSocial")));
	dlg.PercSubjects->setText(QString::number(Doc->GetSession()->GetSubjects()->GetDouble("PercSubjects")));
	dlg.NbMinDocsSubject->setText(QString::number(Doc->GetSession()->GetSubjects()->GetUInt("NbMinDocsSubject")));
	dlg.NbDocsAssess->setText(QString::number(Doc->GetSession()->GetSubjects()->GetUInt("NbDocsAssess")));
	if(dlg.exec())
	{
		Doc->GetSession()->GetSubjects()->Set("PercOK",dlg.PercOK->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("PercKO",dlg.PercKO->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("PercH",dlg.PercH->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("PercErr",dlg.PercErr->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("NbProfMin",dlg.NbProfMin->text().toUInt());
		Doc->GetSession()->GetSubjects()->Set("NbProfMax",dlg.NbProfMax->text().toUInt());
		Doc->GetSession()->GetSubjects()->Set("PercSocial",dlg.PercSocial->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("PercSubjects",dlg.PercSubjects->text().toDouble());
		Doc->GetSession()->GetSubjects()->Set("NbMinDocsSubject",dlg.NbMinDocsSubject->text().toUInt());
		Doc->GetSession()->GetSubjects()->Set("NbDocsAssess",dlg.NbDocsAssess->text().toUInt());
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsCreate(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Create Ideal Groups");
	Dlg.Run(new QCreateIdealGroups());
	Doc->updateAllViews(2);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDoFdbks(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Feedback Cycle");
	Dlg.Run(new QMakeFdbks());
	Doc->updateAllViews(1);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotChooseSOM(void)
{
	QChooseMap* dlg=new QChooseMap(pWorkspace);
	RQuery* select, *select2;
	QString tmp;
	char slang[10];

	connect(dlg->GeneralList, SIGNAL(doubleClicked (QListViewItem*)), this, SLOT(slotViewSOM(QListViewItem*)));
	connect(dlg->GroupProfilesList, SIGNAL(doubleClicked (QListViewItem*)), this, SLOT(slotViewSOM(QListViewItem*)));
	connect(dlg->GroupDocumentsList, SIGNAL(doubleClicked (QListViewItem*)), this, SLOT(slotViewSOM(QListViewItem*)));
	connect(dlg->ProfileDocumentsList, SIGNAL(doubleClicked (QListViewItem*)), this, SLOT(slotViewSOM(QListViewItem*)));

	//select all map from db;
	select = Doc->GetSession()->GetStorage()->SelectDummyEntry("Som_maps",0,RString::Null,0,0);

	//set widget title
	tmp=QString::number(select->GetNbRows())+" Maps available - Double click on map to display";
	dlg->groupBox->setTitle(tmp);
	dlg->show();

	for (select->Start(); !select->End(); select->Next())
	{
		if((*select)[2]=="groups")
		{
			R::RCursor<GFactoryLang> langscur=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
			for(langscur.Start(); !langscur.End(); langscur.Next())
			{
				if (!langscur()->GetPlugin()) continue;
				sprintf(slang, "Lang-%s",langscur()->GetPlugin()->GetCode());
				select2=Doc->GetSession()->GetStorage()->SelectDummyEntry("Som_maps_details",atoi((*select)[0]),slang,0,3);
				if (select2->GetNbRows())
				{
					tmp="GALILEI ["+QString(langscur()->GetPlugin()->GetCode())+"] Groups";
					new QListViewItem(dlg->GeneralList, tmp, ToQString((*select)[0]),"Community");
				}
			}
			continue;
		}
		if((*select)[2]=="profiles")
		{
			R::RCursor<GFactoryLang> langscur=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
			for(langscur.Start(); !langscur.End(); langscur.Next())
			{
				if (!langscur()->GetPlugin()) continue;
				sprintf(slang, "Lang-%s",langscur()->GetPlugin()->GetCode());
				select2=Doc->GetSession()->GetStorage()->SelectDummyEntry("Som_maps_details",atoi((*select)[0]),slang,0,3);
				if (select2->GetNbRows())
				{
					tmp="GALILEI ["+QString(langscur()->GetPlugin()->GetCode())+"] Profiles";
					new QListViewItem(dlg->GeneralList, tmp, ToQString((*select)[0]), "SubProfile");
				}
			}
			continue;
		}
		if((*select)[2]=="documents")
		{
			R::RCursor<GFactoryLang> langscur=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
			for(langscur.Start(); !langscur.End(); langscur.Next())
			{
				if (!langscur()->GetPlugin()) continue;
				sprintf(slang, "Lang-%s",langscur()->GetPlugin()->GetCode());
				select2=Doc->GetSession()->GetStorage()->SelectDummyEntry("Som_maps_details",atoi((*select)[0]),slang,0,3);
				if (select2->GetNbRows())
				{
					tmp="GALILEI ["+QString(langscur()->GetPlugin()->GetCode())+"] Documents";
					new QListViewItem(dlg->GeneralList, tmp, ToQString((*select)[0]), "Document");
				}
			}
			continue;

		}
		if((*select)[2]=="groupsprofiles")
		{
			tmp="Group ["+ToQString((*select)[1])+"] [Profiles]";
			new QListViewItem(dlg->GroupProfilesList, tmp, ToQString((*select)[0]), "SubProfile");
			continue;
		}
		if((*select)[2]=="groupsdocuments")
		{
			tmp="Group ["+ToQString((*select)[1])+"] [Documents]";
			new QListViewItem(dlg->GroupDocumentsList, tmp, ToQString((*select)[0]), "Document");
			continue;
		}
		if((*select)[2]=="profilesdocuments")
		{
			tmp="SubProfile ["+ToQString((*select)[1])+"] [Documents]";
			new QListViewItem(dlg->ProfileDocumentsList, tmp, ToQString((*select)[0]), "Documents");
			continue;
		}
	}

	delete select;
	if (select2)
		delete select2;
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotViewSOM(QListViewItem* item)
{
	createClient(Doc,new KViewSom(Doc, pWorkspace, "Self Organized Map for "+item->text(0),item->text(2),0,atoi(item->text(1))));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupingCompare(void)
{
	if(Doc->GetSession()->GetSubjects())
		createClient(Doc,new KViewThGroups(Doc,Doc->GetSession()->GetSubjects(),pWorkspace,"View Theoritical Groups",0));
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
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Analyse Documents");
	if(!Dlg.Run(new QAnalyzeDocs()))
		return;
	Doc->updateAllViews(0);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsIndexer(void)
{
	QString dir=KFileDialog::getExistingDirectory(QString::null,this,"Choose directory where to export the files");
	if(!dir.isEmpty())
	{
	 	KProgressDialog dlg(this,"Progress","Export documents");
		dlg.show();
		KApplication::kApplication()->processEvents();
		RString pre(dir.latin1());
		pre+="/doc";
		RCursor<GDoc> Docs=Doc->GetSession()->GetDocs();
		unsigned int nb;
		for(Docs.Start(),nb=0;!Docs.End();Docs.Next(),nb++)
		{
			if(dlg.wasCancelled())
				break;
			dlg.progressBar()->setProgress((nb*100)/Docs.GetNb());
			KApplication::kApplication()->processEvents();
			if(!Docs()->GetLang())
				continue;
			RString name=pre+RString::Number(Docs()->GetId())+".txt";
			dlg.setLabel(name.Latin1());
			KApplication::kApplication()->processEvents();
			RTextFile file(name.Latin1());
			file.Open(RIO::Create);
			RCursor<GWeightInfo> Words(Docs()->GetInfos());
			for(Words.Start();!Words.End();Words.Next())
			{
				for(unsigned int i=lround(Words()->GetWeight())+1;--i;)
				{
					file<<Doc->GetSession()->GetStorage()->LoadWord(Words()->GetId(),Docs()->GetLang()->GetCode());
				}
				file<<endl;
			}
			file.Close();
		}
	}
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
		QFile Test(url.path());
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
void KGALILEICenterApp::slotQueryMetaEngine(void)
{
	if(!GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod(false))
	{
		QMessageBox::information(this," Error "," No Meta Engine Method selected!!");
		return;
	}
	createClient(Doc,new KViewMetaEngine(Doc,pWorkspace,"View Meta Engine Results",0));
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotFillMIMETypes(void)
{
	QSessionProgressDlg Dlg(this,0,"Analyse KDE MIME types");
	Dlg.Run(new QFillMIMETypes("/usr/share/mimelnk"));
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
	R::RCursor<GFactoryLang> curlang;
	unsigned int size, min, max;

	curlang=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
	size=Doc->GetSession()->GetStorage()->GetHistorySize();

	if (!size)
	{
		QMessageBox::information(this," Warning "," No history recorded in the database !!");
		return;
	}

	QHistoryDlg dlg(this,0,true);
	dlg.TLMaxHistory->setText(QString("Max Historic ID (max: ")+QString::number(size)+QString(")"));
	dlg.SBMinId->setMinValue(1);
	dlg.SBMinId->setMaxValue(size);
	dlg.SBMaxId->setMinValue(1);
	dlg.SBMaxId->setMaxValue(size);
	dlg.bDate->setChecked(true);
	dlg.MinDate->setText("0000-00-00");
	dlg.MaxDate->setText("0000-00-00");

	KApplication::kApplication()->processEvents();
	if(dlg.exec())
	{
		min=dlg.SBMinId->value() ;
		max=dlg.SBMaxId->value();
		if(min>max) return;
		createClient(Doc,new KViewHistory(Doc,0/*dlg.CBGlobal->isChecked()*/,pWorkspace,"Show Chromosomes",0,min,max, dlg.MinDate->text(),dlg.MaxDate->text(), dlg.bDate->isChecked()));
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
void KGALILEICenterApp::slotSaveModifier(void)
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->SetSaveResults(sessionSave->isChecked());
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otDocs);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotUsersClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otUsers);
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotGroupsClear()
{
	if(Doc&&Doc->GetSession())
		Doc->GetSession()->ForceReCompute(otGroups);
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
		o->run();
	}
	catch(std::bad_alloc)
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
			break;

		case gDocs:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			break;

		case gDoc:
			createXML->setEnabled(!(((KViewDoc*)m)->IsDocXML()));
			saveXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			analyseXML->setEnabled(((KViewDoc*)m)->IsDocXML());
			break;

		case gProfile:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			break;

		case gGroups:
			saveXML->setEnabled(false);
			createXML->setEnabled(false);
			analyseXML->setEnabled(false);
			break;

		case gGA:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
			break;

		case gNothing:
		default:
			createXML->setEnabled(false);
			saveXML->setEnabled(false);
			analyseXML->setEnabled(false);
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

		case QListViewItemType::tSubProfile:
			createClient(Doc,new KViewProfile(obj->Obj.SubProfile->GetProfile(),Doc,pWorkspace,"View Profile",0));
			break;

		case QListViewItemType::tNothing:
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
KGALILEICenterApp::~KGALILEICenterApp(void)
{
	try
	{
		if(Doc)
			delete Doc;
	}
	catch(GException& e)
	{
		QMessageBox::critical(this,"KGALILEICenter",e.GetMsg());
	}
}
