/*

	GALILEI Research Project

	KGALILEICenter_dlg.cpp

	Main Window - Implementation of the Dialog part.

	Copyright 2002 by the Université libre de Bruxelles.

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
// include files for R
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <gpostdoc.h>
#include <glang.h>
#include <gfiltermanagerkde.h>
#include <qlistviewitemtype.h>
#include <gsession.h>
#include <gstatscalc.h>
#include <guser.h>
#include <gprofile.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
#include <ggrouping.h>
#include <gpostgroup.h>
#include <gprofilecalc.h>
#include <glang.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gpluginmanagers.h>
#include <gstorage.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qworkspace.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <klocale.h>
#include <kurlrequester.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewusers.h"
#include "kviewdoc.h"
#include "kviewprofile.h"
#include "qsessionprogress.h"
#include "qgalileiitem.h"




QMyPlugins::QMyPlugins(KGALILEICenterApp* app,QString title)
		: QPlugins(app,title), Tabs(30,15)
	{
		    connect(EnableMeasure,SIGNAL(toggled(bool)),this,SLOT(slotMeasureEnable(bool)));
		    connect(ConfigMeasure,SIGNAL(clicked()),this,SLOT(slotConfigMeasure()));
    		connect(AboutMeasure,SIGNAL(clicked()),this,SLOT(slotAboutMeasure()));
			connect(Measures,SIGNAL(currentChanged(QWidget*)),this,SLOT(slotChangeCat(QWidget*)));
			connect(CurrentMeasure,SIGNAL(activated(const QString&)),this,SLOT(changeCurrent(const QString&)));
	}


	void QMyPlugins::slotChangeCat(QWidget*)
	{
		int idx;
		QString str;
		GTypeMeasureManager* Manager=GPluginManagers::GetManager<GMeasureManager>("Measures")->GetMeasureCategory(Tabs[Measures->currentPageIndex()]->Type);
		RCursor<GFactoryMeasure> Cur(Manager->GetFactories());
		QString current=Tabs[Measures->currentPageIndex()]->Current;
		CurrentMeasure->clear();
		CurrentMeasure->insertItem("None",0);
		for(Cur.Start(),idx=1;!Cur.End();Cur.Next(),idx++)
		{
			str=ToQString(Cur()->GetName());
			str+=" [";
			str+=ToQString(Cur()->GetLib());
			str+="]";
			CurrentMeasure->insertItem(ToQString(Cur()->GetName()),idx);
			if(Cur()->GetName()==FromQString(current))
				CurrentMeasure->setCurrentItem(idx);
		}
		CurrentMeasure->setEnabled(idx>1);
		EnableMeasure->setEnabled(idx>1);
		ConfigMeasure->setEnabled(idx>1);
		AboutMeasure->setEnabled(idx>1);
		changeMeasure(Tabs[Measures->currentPageIndex()]->List->currentItem());
	}

	void QMyPlugins::changeMeasure( QListViewItem * item )
	{
		if(!item) return;
		QMeasureItem* f=dynamic_cast<QMeasureItem*>(item);
		if(!f) return;
		EnableMeasure->setChecked(f->Enable);
		ConfigMeasure->setEnabled(f->Fac->HasConfigure());
		AboutMeasure->setEnabled(f->Fac->HasAbout());
	}


	void QMyPlugins::slotAboutMeasure()
	{
		if(!Tabs[Measures->currentPageIndex()]->List->currentItem()) return;
		QMeasureItem* f=dynamic_cast<QMeasureItem*>(Tabs[Measures->currentPageIndex()]->List->currentItem());
		if(!f) return;
		f->Fac->About();
	}


	void QMyPlugins::slotConfigMeasure()
	{
		if(!Tabs[Measures->currentPageIndex()]->List->currentItem()) return;
		QMeasureItem* f=dynamic_cast<QMeasureItem*>(Tabs[Measures->currentPageIndex()]->List->currentItem());
		if(!f) return;
		f->Fac->Configure();
	}

	void QMyPlugins::slotMeasureEnable( bool state )
	{
		if(!Tabs[Measures->currentPageIndex()]->List->currentItem()) return;
		QMeasureItem* f=dynamic_cast<QMeasureItem*>(Tabs[Measures->currentPageIndex()]->List->currentItem());
		if(!f) return;
		f->Enable=state;
	}

void QMyPlugins::changeCurrent(const QString& string)
{
	Tabs[Measures->currentPageIndex()]->Current=string;
}


//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void KGALILEICenterApp::StartJob(const R::RString job)
{
	if(!QSessionProgressDlg::Main)
	{
		QSessionProgressDlg* Jobs=new QSessionProgressDlg(this,Doc->GetSession(),"Job",false);
		QSessionProgressDlg::Main->PutText(job+"...");
		Jobs->Begin();
	}
	else
	{
		QSessionProgressDlg::NbJobs++;
		QSessionProgressDlg::Main->PutText(job+"...");
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::EndJob(void)
{
	if(!(--QSessionProgressDlg::NbJobs))
	{
		delete QSessionProgressDlg::Main;
	}
}


//-----------------------------------------------------------------------------
template<class Factory,class Manager,class Item>
	QListViewItem* Init(RString manager,QListView* lst,QCheckBox* enable,QComboBox* sel=0)
{
	QListViewItem* def;
	QListViewItem* cur;
	QString str;
	int idx;

	Manager* Mng=GPluginManagers::GetManager<Manager>(manager);
	R::RCursor<Factory> Cur(Mng->GetFactories());
	Factory* current=Mng->GetCurrentFactory(false);

	// Goes through the profiles computing method
	def=cur=0;
	if(sel)
		sel->insertItem("None",0);
	cur=0;
	for(Cur.Start(),idx=1;!Cur.End();Cur.Next(),idx++)
	{
		str=ToQString(Cur()->GetName());
		str+=" [";
		str+=ToQString(Cur()->GetLib());
		str+="]";
		cur=new Item(lst,Cur(),str,cur);
		if(sel)
		{
			sel->insertItem(ToQString(Cur()->GetName()),idx);
			if(Cur()==current)
				sel->setCurrentItem(idx);
		}
		if(!def)
			def=cur;
	}
	if(def)
	{
		lst->setSelected(def,true);
		if(enable)
			enable->setEnabled(true);
		if(sel)
			sel->setEnabled(true);
	}
	return(def);
}


//-----------------------------------------------------------------------------
template<class Item>
	void Done(QListView* lst,KGALILEICenterApp* app=0)
{
	Item* item=dynamic_cast<Item*>(lst->firstChild());
	unsigned int i=0;
	while(item)
	{
		item->Fac->Set("Level",i++);
		item->Fac->Apply();
		if(app)
		{
			if(item->Enable)
				item->Fac->Create(app->getSession());
			else
				item->Fac->Delete(app->getSession());
		}
		else
		{
			if(item->Enable)
				item->Fac->Create();
			else
				item->Fac->Delete();
		}
		item=dynamic_cast<Item*>(item->itemBelow());
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::InitMeasure(QMyPlugins* dlg)
{
	QString str;
	int tabs,idx;

	GMeasureManager* Manager=GPluginManagers::GetManager<GMeasureManager>("Measures");
	R::RCursor<GTypeMeasureManager> Cur(Manager->GetMeasureCategories());
	for(Cur.Start(),tabs=0;!Cur.End();Cur.Next(),tabs++)
	{
		QMyPlugins::Tab* tab=new QMyPlugins::Tab(Cur()->GetName(),new QListView(dlg->Measures));
		dlg->Measures->insertTab(tab->List,ToQString(Cur()->GetName()),tabs);
		dlg->Tabs.InsertPtrAt(tab,tabs);
		if(Cur()->GetCurrentFactory(false))
			tab->Current=ToQString(Cur()->GetCurrentFactory(false)->GetName());
		else
			tab->Current="None";
		tab->List->addColumn( tr2i18n( "Method" ) );
		tab->List->setResizePolicy( QScrollView::Manual );
		tab->List->setResizeMode( QListView::AllColumns );
    	connect(tab->List,SIGNAL(selectionChanged(QListViewItem*)),dlg,SLOT(changeMeasure(QListViewItem*)));

		//TabPageLayout->addWidget( Storages );
		RCursor<GFactoryMeasure> Cur2(Cur()->GetFactories());
		QMeasureItem* ptr=0;
		for(Cur2.Start(),idx=0;!Cur2.End();Cur2.Next(),idx++)
		{
			str=ToQString(Cur2()->GetName());
			str+=" [";
			str+=ToQString(Cur2()->GetLib());
			str+="]";
			ptr=new QMeasureItem(tab->List,Cur2(),str,ptr);
		}
		tab->List->setEnabled(idx>0);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::DoneMeasure(QMyPlugins* dlg)
{
	RCursor<QMyPlugins::Tab> Cur(dlg->Tabs);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		QMeasureItem* item=dynamic_cast<QMeasureItem*>(Cur()->List->firstChild());
		while(item)
		{
			if(item->Enable)
				item->Fac->Create(getSession());
			else
				item->Fac->Delete(getSession());
			item=dynamic_cast<QMeasureItem*>(item->itemBelow());
		}
		GPluginManagers::GetManager<GMeasureManager>("Measures")->GetMeasureCategory(Cur()->Type)->SetCurrentMethod(Cur()->Current.latin1(),false);
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPlugins(void)
{

	QMyPlugins dlg(this,"Plug-Ins Dialog");
	QString str;

	//set the plugins path
	RString paths("");
	RCursor<RString> cPath(pluginsPath);
	for(cPath.Start();!cPath.End();cPath.Next())
		paths+=(*cPath())+RString(";");
	dlg.PluginsPath->setMode(KFile::Directory);
	dlg.PluginsPath->setURL(ToQString(paths));

	//sort POST_X Managers;
	GPluginManagers::GetManager<GPostDocManager>("PostDoc")->ReOrder();
	GPluginManagers::GetManager<GPreProfileManager>("PreProfile")->ReOrder();
	GPluginManagers::GetManager<GPostProfileManager>("PostProfile")->ReOrder();
	GPluginManagers::GetManager<GPostGroupManager>("PostGroup")->ReOrder();

	// Goes through managers
	dlg.changeFilter(Init<GFactoryFilter,GFilterManager,QFilterItem>("Filter",dlg.Filters,dlg.EnableFilter));
	dlg.changeProfileCalc(Init<GFactoryProfileCalc,GProfileCalcManager,QProfileCalcItem>("ProfileCalc",dlg.ProfileCalcs,dlg.EnableProfileCalc,dlg.CurrentProfileCalc));
	dlg.changeGrouping(Init<GFactoryGrouping,GGroupingManager,QGroupingItem>("Grouping",dlg.Groupings,dlg.EnableGrouping,dlg.CurrentGrouping));
	dlg.changeGroupCalc(Init<GFactoryGroupCalc,GGroupCalcManager,QGroupCalcItem>("GroupCalc",dlg.GroupCalcs,dlg.EnableGroupCalc,dlg.CurrentGroupCalc));
	dlg.changeStatCalc(Init<GFactoryStatsCalc,GStatsCalcManager,QStatsCalcItem>("StatsCalc",dlg.Stats,dlg.EnableStat));
	dlg.changeLinkCalc(Init<GFactoryLinkCalc,GLinkCalcManager,QLinkCalcItem>("LinkCalc",dlg.LinkCalcs,dlg.EnableLinkCalc,dlg.CurrentLinkCalc));
	dlg.changePostDoc(Init<GFactoryPostDoc,GPostDocManager,QPostDocItem>("PostDoc",dlg.PostDocs,dlg.EnablePostDoc));
	dlg.changePreProfile(Init<GFactoryPreProfile,GPreProfileManager,QPreProfileItem>("PreProfile",dlg.PreProfile,dlg.EnablePreProfile));
	dlg.changePostProfile(Init<GFactoryPostProfile,GPostProfileManager,QPostProfileItem>("PostProfile",dlg.PostProfile,dlg.EnablePostProfile));
	dlg.changePostGroup(Init<GFactoryPostGroup,GPostGroupManager,QPostGroupItem>("PostGroup",dlg.PostGroups,dlg.EnablePostGroup));
	dlg.changeLang(Init<GFactoryLang,GLangManager,QLangItem>("Lang",dlg.Langs,dlg.EnableLang));
	dlg.changeDocAnalyse(Init<GFactoryDocAnalyse,GDocAnalyseManager,QDocAnalyseItem>("DocAnalyse",dlg.DocAnalyses,dlg.EnableDocAnalyse,dlg.CurrentDocAnalyse));
	dlg.changeEngine(Init<GFactoryEngine,GEngineManager,QEngineItem>("Engine",dlg.Engines,dlg.EnableEngine));
	dlg.changeMetaEngine(Init<GFactoryMetaEngine,GMetaEngineManager,QMetaEngineItem>("MetaEngine",dlg.MetaEngines,dlg.EnableMetaEngine,dlg.CurrentMetaEngine));
	dlg.changeStorage(Init<GFactoryStorage,GStorageManager,QStorageItem>("Storage",dlg.Storages,0,dlg.CurrentStorage));
	InitMeasure(&dlg);

	dlg.MainTab->setCurrentPage(DlgMainTabIdx);
	dlg.DocsTab->setCurrentPage(DlgDocsTabIdx);
	dlg.ProfilesTab->setCurrentPage(DlgProfilesTabIdx);
	dlg.CommunitiesTab->setCurrentPage(DlgCommunitiesTabIdx);
	dlg.SearchTab->setCurrentPage(DlgSearchTabIdx);
	if(dlg.exec())
	{
		// read the plugins path
		if (strcmp(paths, dlg.PluginsPath->url()))
		{
			pluginsPath.Clear();
			RString pluginsPaths=dlg.PluginsPath->url().ascii();
			int findindex=pluginsPaths.FindStr(";",0);
			while(findindex!=-1)
			{
				if (!pluginsPaths.Mid(0,findindex).IsEmpty())
					pluginsPath.InsertPtr(new RString(pluginsPaths.Mid(0,findindex)));
				pluginsPaths=pluginsPaths.Mid(findindex+1);
				pluginsPaths.FindStr(";",0);
				findindex=pluginsPaths.FindStr(";",0);
			}
			if (!pluginsPaths.IsEmpty())
			pluginsPath.InsertPtr(new RString(pluginsPaths));
			QMessageBox::information(this,"Plug-Ins Path has changed","You changed the plugins path, please restart KGALILEICenter.");
		}

		// Goes through managers
		Done<QFilterItem>(dlg.Filters);
		Done<QProfileCalcItem>(dlg.ProfileCalcs,this);
		Done<QGroupingItem>(dlg.Groupings,this);
		Done<QGroupCalcItem>(dlg.GroupCalcs,this);
		Done<QStatsCalcItem>(dlg.Stats);
		Done<QLinkCalcItem>(dlg.LinkCalcs,this);
		Done<QPostDocItem>(dlg.PostDocs,this);
		Done<QPreProfileItem>(dlg.PreProfile,this);
		Done<QPostProfileItem>(dlg.PostProfile,this);
		Done<QLangItem>(dlg.Langs,this);
		Done<QDocAnalyseItem>(dlg.DocAnalyses,this);
		Done<QPostGroupItem>(dlg.PostGroups,this);
		Done<QEngineItem>(dlg.Engines);
		Done<QMetaEngineItem>(dlg.MetaEngines,this);
		DoneMeasure(&dlg);

		GPluginManagers::GetManager<GPostDocManager>("PostDoc")->ReOrder();
		GPluginManagers::GetManager<GPreProfileManager>("PreProfile")->ReOrder();
		GPluginManagers::GetManager<GPostProfileManager>("PostProfile")->ReOrder();
		GPluginManagers::GetManager<GPostGroupManager>("PostGroup")->ReOrder();

		// Set current method
		GPluginManagers::GetManager<GProfileCalcManager>("ProfileCalc")->SetCurrentMethod(dlg.CurrentProfileCalc->currentText().latin1(),false);
		GPluginManagers::GetManager<GGroupingManager>("Grouping")->SetCurrentMethod(dlg.CurrentGrouping->currentText().latin1(),false);
		GPluginManagers::GetManager<GGroupCalcManager>("GroupCalc")->SetCurrentMethod(dlg.CurrentGroupCalc->currentText().latin1(),false);
		GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->SetCurrentMethod(dlg.CurrentLinkCalc->currentText().latin1(),false);
		GPluginManagers::GetManager<GDocAnalyseManager>("DocAnalyse")->SetCurrentMethod(dlg.CurrentDocAnalyse->currentText().latin1(),false);
		GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->SetCurrentMethod(dlg.CurrentMetaEngine->currentText().latin1(),false);
		GPluginManagers::GetManager<GStorageManager>("Storage")->SetCurrentMethod(dlg.CurrentStorage->currentText().latin1(),false);
	}
	DlgMainTabIdx=dlg.MainTab->currentPageIndex();
	DlgDocsTabIdx=dlg.DocsTab->currentPageIndex();
	DlgProfilesTabIdx=dlg.ProfilesTab->currentPageIndex();
	DlgCommunitiesTabIdx=dlg.CommunitiesTab->currentPageIndex();
	DlgSearchTabIdx=dlg.SearchTab->currentPageIndex();
}
