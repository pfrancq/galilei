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
#include <gprofilessims.h>
#include <gprofilesdocssims.h>
#include <glang.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gpluginmanagers.h>
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
#include "qplugins.h"
#include "qgalileiitem.h"



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
template<class Cursor,class Met,class Item>
	QListViewItem* Init(R::RCursor<Cursor> Cur,QListView* lst,QCheckBox* enable,QComboBox* sel=0,Met* current=0)
{
	QListViewItem* def;
	QListViewItem* cur;
	QString str;
	int idx;

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
			if((Cur()->GetPlugin())&&(Cur()->GetPlugin()==current))
				sel->setCurrentItem(idx);
		}
		if(!def)
			def=cur;
	}
	if(def)
	{
		lst->setSelected(def,true);
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
	while(item)
	{
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
void KGALILEICenterApp::slotPlugins(void)
{

	QPlugins dlg(this,"Plug-Ins Dialog");
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
	dlg.changeFilter(Init<GFactoryFilter,GFilter,QFilterItem>(GPluginManagers::GetManager<GFilterManager>("Filter")->GetFactories(),dlg.Filters,dlg.EnableFilter));
	dlg.changeProfileCalc(Init<GFactoryProfileCalc,GProfileCalc,QProfileCalcItem>(GPluginManagers::GetManager<GProfileCalcManager>("ProfileCalc")->GetFactories(),dlg.ProfileCalcs,dlg.EnableProfileCalc,dlg.CurrentProfileCalc,GPluginManagers::GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod()));
	dlg.changeGrouping(Init<GFactoryGrouping,GGrouping,QGroupingItem>(GPluginManagers::GetManager<GGroupingManager>("Grouping")->GetFactories(),dlg.Groupings,dlg.EnableGrouping,dlg.CurrentGrouping,GPluginManagers::GetManager<GGroupingManager>("Grouping")->GetCurrentMethod()));
	dlg.changeGroupCalc(Init<GFactoryGroupCalc,GGroupCalc,QGroupCalcItem>(GPluginManagers::GetManager<GGroupCalcManager>("GroupCalc")->GetFactories(),dlg.GroupCalcs,dlg.EnableGroupCalc,dlg.CurrentGroupCalc,GPluginManagers::GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod()));
	dlg.changeStatCalc(Init<GFactoryStatsCalc,GStatsCalc,QStatsCalcItem>(GPluginManagers::GetManager<GStatsCalcManager>("StatsCalc")->GetFactories(),dlg.Stats,dlg.EnableStat));
	dlg.changeLinkCalc(Init<GFactoryLinkCalc,GLinkCalc,QLinkCalcItem>(GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetFactories(),dlg.LinkCalcs,dlg.EnableLinkCalc,dlg.CurrentLinkCalc,GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod()));
	dlg.changePostDoc(Init<GFactoryPostDoc,GPostDoc,QPostDocItem>(GPluginManagers::GetManager<GPostDocManager>("PostDoc")->GetFactories(),dlg.PostDocs,dlg.EnablePostDoc));
	dlg.changePreProfile(Init<GFactoryPreProfile,GPreProfile,QPreProfileItem>(GPluginManagers::GetManager<GPreProfileManager>("PreProfile")->GetFactories(),dlg.PreProfile,dlg.EnablePreProfile));
	dlg.changePostProfile(Init<GFactoryPostProfile,GPostProfile,QPostProfileItem>(GPluginManagers::GetManager<GPostProfileManager>("PostProfile")->GetFactories(),dlg.PostProfile,dlg.EnablePostProfile));
	dlg.changePostGroup(Init<GFactoryPostGroup,GPostGroup,QPostGroupItem>(GPluginManagers::GetManager<GPostGroupManager>("PostGroup")->GetFactories(),dlg.PostGroups,dlg.EnablePostGroup));
	dlg.changeLang(Init<GFactoryLang,GLang,QLangItem>(GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories(),dlg.Langs,dlg.EnableLang));
	dlg.changeDocAnalyse(Init<GFactoryDocAnalyse,GDocAnalyse,QDocAnalyseItem>(GPluginManagers::GetManager<GDocAnalyseManager>("DocAnalyse")->GetFactories(),dlg.DocAnalyses,dlg.EnableDocAnalyse,dlg.CurrentDocAnalyse,GPluginManagers::GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod()));
	dlg.changeEngine(Init<GFactoryEngine,GEngine,QEngineItem>(GPluginManagers::GetManager<GEngineManager>("Engine")->GetFactories(),dlg.Engines,dlg.EnableEngine));
	dlg.changeMetaEngine(Init<GFactoryMetaEngine,GMetaEngine,QMetaEngineItem>(GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->GetFactories(),dlg.MetaEngines,dlg.EnableMetaEngine,dlg.CurrentMetaEngine,GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod()));
	dlg.changeProfilesSims(Init<GFactoryProfilesSims,GProfilesSims,QProfilesSimsItem>(GPluginManagers::GetManager<GProfilesSimsManager>("ProfilesSims")->GetFactories(),dlg.ProfilesSims,dlg.EnableProfilesSims,dlg.CurrentProfilesSims,GPluginManagers::GetManager<GProfilesSimsManager>("ProfilesSims")->GetCurrentMethod()));
	dlg.changeProfilesDocsSims(Init<GFactoryProfilesDocsSims,GProfilesDocsSims,QProfilesDocsSimsItem>(GPluginManagers::GetManager<GProfilesDocsSimsManager>("ProfilesDocsSims")->GetFactories(),dlg.ProfilesDocsSims,dlg.EnableProfilesDocsSims,dlg.CurrentProfilesDocsSims,GPluginManagers::GetManager<GProfilesDocsSimsManager>("ProfilesDocsSims")->GetCurrentMethod()));
	dlg.changeGroupsDocsSims(Init<GFactoryGroupsDocsSims,GGroupsDocsSims,QGroupsDocsSimsItem>(GPluginManagers::GetManager<GGroupsDocsSimsManager>("GroupsDocsSims")->GetFactories(),dlg.GroupsDocsSims,dlg.EnableGroupsDocsSims,dlg.CurrentGroupsDocsSims,GPluginManagers::GetManager<GGroupsDocsSimsManager>("GroupsDocsSims")->GetCurrentMethod()));
	dlg.changeProfilesGroupsSims(Init<GFactoryProfilesGroupsSims,GProfilesGroupsSims,QProfilesGroupsSimsItem>(GPluginManagers::GetManager<GProfilesGroupsSimsManager>("ProfilesGroupsSims")->GetFactories(),dlg.ProfilesGroupsSims,dlg.EnableProfilesGroupsSims,dlg.CurrentProfilesGroupsSims,GPluginManagers::GetManager<GProfilesGroupsSimsManager>("ProfilesGroupsSims")->GetCurrentMethod()));

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
				cout <<" path2="<<pluginsPaths.Mid(0,findindex)<<endl;
				pluginsPaths=pluginsPaths.Mid(findindex+1);
				pluginsPaths.FindStr(";",0);
				findindex=pluginsPaths.FindStr(";",0);
			}
			cout <<" path3="<<pluginsPaths.Mid(0,findindex)<<endl;
			if (!pluginsPaths.IsEmpty())
			pluginsPath.InsertPtr(new RString(pluginsPaths));
			QMessageBox::information(this,"Plug-Ins Path has changed","You changed the plugins path, please restart KGALILEICenter.");
		}

		GPluginManagers::GetManager<GPostDocManager>("PostDoc")->ReOrder();
		GPluginManagers::GetManager<GPreProfileManager>("PreProfile")->ReOrder();
		GPluginManagers::GetManager<GPostProfileManager>("PostProfile")->ReOrder();
		GPluginManagers::GetManager<GPostGroupManager>("PostGroup")->ReOrder();

		// Goes through managers
		try
		{
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
			Done<QProfilesSimsItem>(dlg.ProfilesSims,this);
			Done<QProfilesDocsSimsItem>(dlg.ProfilesDocsSims,this);
			Done<QGroupsDocsSimsItem>(dlg.GroupsDocsSims,this);
			Done<QProfilesGroupsSimsItem>(dlg.ProfilesGroupsSims,this);
		}
		catch(GException& e)
		{
			std::cerr<<e.GetMsg()<<std::endl;
		}
		// Set current method
		try
		{
			GPluginManagers::GetManager<GProfileCalcManager>("ProfileCalc")->SetCurrentMethod(dlg.CurrentProfileCalc->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GGroupingManager>("Grouping")->SetCurrentMethod(dlg.CurrentGrouping->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GGroupCalcManager>("GroupCalc")->SetCurrentMethod(dlg.CurrentGroupCalc->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GLinkCalcManager>("LinkCalc")->SetCurrentMethod(dlg.CurrentLinkCalc->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GDocAnalyseManager>("DocAnalyse")->SetCurrentMethod(dlg.CurrentDocAnalyse->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GMetaEngineManager>("MetaEngine")->SetCurrentMethod(dlg.CurrentMetaEngine->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GProfilesSimsManager>("ProfilesSims")->SetCurrentMethod(dlg.CurrentProfilesSims->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GProfilesDocsSimsManager>("ProfilesDocsSims")->SetCurrentMethod(dlg.CurrentProfilesDocsSims->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GGroupsDocsSimsManager>("GroupsDocsSims")->SetCurrentMethod(dlg.CurrentGroupsDocsSims->currentText().latin1());
		}
		catch(GException)
		{
		}
		try
		{
			GPluginManagers::GetManager<GProfilesGroupsSimsManager>("ProfilesGroupsSims")->SetCurrentMethod(dlg.CurrentProfilesGroupsSims->currentText().latin1());
		}
		catch(GException)
		{
		}
	}
	DlgMainTabIdx=dlg.MainTab->currentPageIndex();
	DlgDocsTabIdx=dlg.DocsTab->currentPageIndex();
	DlgProfilesTabIdx=dlg.ProfilesTab->currentPageIndex();
	DlgCommunitiesTabIdx=dlg.CommunitiesTab->currentPageIndex();
	DlgSearchTabIdx=dlg.SearchTab->currentPageIndex();
}
