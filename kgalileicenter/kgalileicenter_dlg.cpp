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
#include <frontend/kde/rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <infos/glang.h>
#include <frontend/kde/gfiltermanagerkde.h>
#include <frontend/kde/qlistviewitemtype.h>
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gpostprofile.h>
#include <profiles/gpostprofilemanager.h>
#include <groups/ggrouping.h>
#include <profiles/gprofilecalc.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <engines/gengine.h>
#include <engines/gmetaengine.h>
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
#include "qsessionoptions.h"
#include "qplugins.h"
#include "qgalileiitem.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

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
	for(Cur.Start(),idx=1;!Cur.End();Cur.Next(),idx++)
	{
		str=ToQString(Cur()->GetName());
		str+=" [";
		str+=ToQString(Cur()->GetLib());
		str+="]";
		cur=new Item(lst,Cur(),str);
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
	R::RCursor<GFactoryPostDoc> PostDoc;
	GFactoryPostDocOrder* pdorder;
	R::RCursor<GFactoryPostProfile> PostProfile;
	GFactoryPostProfileOrder* pporder;
	R::RCursor<GFactoryPostGroup> PostGroup;
	GFactoryPostGroupOrder* pgorder;
	R::RCursor<GFactoryMetaEngine> MetaEngine;
	QPlugins dlg(this,"Plugins Dialog");
	QString str;
	int idx;


	//set the plugins path
	dlg.PluginsPath->setMode(KFile::Directory);
	dlg.PluginsPath->setURL(ToQString(pluginsPath));

	// Goes through managers
	dlg.changeFilter(Init<GFactoryFilter,GFilter,QFilterItem>(URLManager->GetFiltersCursor(),dlg.Filters,dlg.EnableFilter));
	dlg.changeProfileCalc(Init<GFactoryProfileCalc,GProfileCalc,QProfileCalcItem>(ProfilingManager->GetProfileCalcsCursor(),dlg.ProfileCalcs,dlg.EnableProfileCalc,dlg.CurrentProfileCalc,ProfilingManager->GetCurrentMethod()));
	dlg.changeGrouping(Init<GFactoryGrouping,GGrouping,QGroupingItem>(GroupingManager->GetGroupingsCursor(),dlg.Groupings,dlg.EnableGrouping,dlg.CurrentGrouping,GroupingManager->GetCurrentMethod()));
	dlg.changeGroupCalc(Init<GFactoryGroupCalc,GGroupCalc,QGroupCalcItem>(GroupCalcManager->GetGroupCalcsCursor(),dlg.GroupCalcs,dlg.EnableGroupCalc,dlg.CurrentGroupCalc,GroupCalcManager->GetCurrentMethod()));
	dlg.changeStatCalc(Init<GFactoryStatsCalc,GStatsCalc,QStatsCalcItem>(StatsCalcManager->GetStatsCalcsCursor(),dlg.Stats,dlg.EnableStat));
	dlg.changeLinkCalc(Init<GFactoryLinkCalc,GLinkCalc,QLinkCalcItem>(LinkCalcManager->GetLinkCalcsCursor(),dlg.LinkCalcs,dlg.EnableLinkCalc,dlg.CurrentLinkCalc,LinkCalcManager->GetCurrentMethod()));
	RContainer<GFactoryPostGroupOrder,true,true> postgroupsordered(PostGroup.GetNb());
	PostGroup=PostGroupManager->GetPostGroupsCursor();
	for(PostGroup.Start(),idx=1;!PostGroup.End();PostGroup.Next(), idx++)
	{
		pgorder=new GFactoryPostGroupOrder;
		pgorder->Fac=PostGroup();
		postgroupsordered.InsertPtr(pgorder);
	}
	dlg.changePostGroup(Init<GFactoryPostGroupOrder,GPostGroup,QPostGroupItem>(RCursor<GFactoryPostGroupOrder>(postgroupsordered),dlg.PostGroups,dlg.EnablePostGroup));
	RContainer<GFactoryPostDocOrder,true,true> postdocsordered(PostDoc.GetNb());
	PostDoc=PostDocManager->GetPostDocsCursor();
	for(PostDoc.Start(),idx=1;!PostDoc.End();PostDoc.Next(),idx++)
	{
		pdorder=new GFactoryPostDocOrder;
		pdorder->Fac=PostDoc();
		postdocsordered.InsertPtr(pdorder);
	}
	dlg.changePostDoc(Init<GFactoryPostDocOrder,GPostDoc,QPostDocItem>(RCursor<GFactoryPostDocOrder>(postdocsordered),dlg.PostDocs,dlg.EnablePostDoc));
	RContainer<GFactoryPostProfileOrder,true,true> postprofileordered(PostProfile.GetNb());
	PostProfile=PostProfileManager->GetPostProfileCursor();
	for(PostProfile.Start(),idx=1;!PostProfile.End();PostProfile.Next(), idx++)
	{
		pporder=new GFactoryPostProfileOrder;
		pporder->Fac=PostProfile();
		postprofileordered.InsertPtr(pporder);
	}
	dlg.changePostProfile(Init<GFactoryPostProfileOrder,GPostProfile,QPostProfileItem>(RCursor<GFactoryPostProfileOrder>(postprofileordered),dlg.PostProfile,dlg.EnablePostProfile));
	dlg.changeLang(Init<GFactoryLang,GLang,QLangItem>(Langs->GetLangsCursor(),dlg.Langs,dlg.EnableLang));
	dlg.changeDocAnalyse(Init<GFactoryDocAnalyse,GDocAnalyse,QDocAnalyseItem>(DocAnalyseManager->GetDocAnalysesCursor(),dlg.DocAnalyses,dlg.EnableDocAnalyse,dlg.CurrentDocAnalyse,DocAnalyseManager->GetCurrentMethod()));
	dlg.changeEngine(Init<GFactoryEngine,GEngine,QEngineItem>(EngineManager->GetEnginesCursor(),dlg.Engines,dlg.EnableEngine));
	dlg.changeMetaEngine(Init<GFactoryMetaEngine,GMetaEngine,QMetaEngineItem>(EngineManager->GetMetaEnginesCursor(),dlg.MetaEngines,dlg.EnableMetaEngine,dlg.CurrentMetaEngine,EngineManager->GetCurrentMethod()));

	if(dlg.exec())
	{
		// read the plugins path
		if (strcmp(pluginsPath, dlg.PluginsPath->url()))
		{
			pluginsPath=RString(dlg.PluginsPath->url().ascii());
			QMessageBox::information(this,"Plugins Path has changed","You changed the plugins path, please restart KGALILEICenter.");
		}

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
			Done<QPostProfileItem>(dlg.PostProfile,this);
			Done<QLangItem>(dlg.Langs,this);
			Done<QDocAnalyseItem>(dlg.DocAnalyses,this);
			Done<QPostGroupItem>(dlg.PostGroups,this);
			Done<QEngineItem>(dlg.Engines);
			Done<QMetaEngineItem>(dlg.MetaEngines,this);
		}
		catch(GException& e)
		{
			std::cerr<<e.GetMsg()<<std::endl;
		}
		// Set current method
		try
		{
			ProfilingManager->SetCurrentMethod(dlg.CurrentProfileCalc->currentText());
		}
		catch(GException)
		{
		}
		try
		{
			GroupingManager->SetCurrentMethod(dlg.CurrentGrouping->currentText());
		}
		catch(GException)
		{
		}
		try
		{
			GroupCalcManager->SetCurrentMethod(dlg.CurrentGroupCalc->currentText());
		}
		catch(GException)
		{
		}
		try
		{
			LinkCalcManager->SetCurrentMethod(dlg.CurrentLinkCalc->currentText());
		}
		catch(GException)
		{
		}
		try
		{
			DocAnalyseManager->SetCurrentMethod(dlg.CurrentDocAnalyse->currentText());
		}
		catch(GException)
		{
		}
		try
		{
			EngineManager->SetCurrentMethod(dlg.CurrentMetaEngine->currentText());
		}
		catch(GException)
		{
		}
	}
}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionOptions(void)
{
	char tmp[10];

	QSessionOptions dlg(this, "Session Options");
	sprintf(tmp,"%f",SessionParams.GetDouble("NullSimLevel"));
	dlg.NullSim->setText(tmp);
	dlg.SBDiffBehaviour->setValue(SessionParams.GetUInt("DiffBehaviourMinDocs"));
	dlg.SBSameBehaviour->setValue(SessionParams.GetUInt("SameBehaviourMinDocs"));
	dlg.PathtoBinary->setMode(KFile::Directory);
	dlg.PathtoBinary->setURL(ToQString(SessionParams.GetString("PathtoBinary")));
	dlg.CBDebugSim->setChecked(SessionParams.GetBool("DebugSim"));
	dlg.CBDebugBehaviour->setChecked(SessionParams.GetBool("DebugBehaviour"));
	dlg.CBDebugMinSim->setChecked(SessionParams.GetBool("DebugMinSim"));
	dlg.CBAutomaticMinSim->setChecked(SessionParams.GetBool("AutomaticMinSim"));
	dlg.CBFixedMinSim->setChecked(!SessionParams.GetBool("AutomaticMinSim"));
	sprintf(tmp,"%f",SessionParams.GetDouble("MinSim"));
	dlg.MinSim->setText(tmp);

	if(dlg.exec())
	{
		SessionParams.Set("DiffBehaviourMinDocs",dlg.SBDiffBehaviour->value());
		SessionParams.Set("SameBehaviourMinDocs", dlg.SBSameBehaviour->value());
		SessionParams.Set("NullSimLevel", atof(dlg.NullSim->text()));
		SessionParams.Set("PathtoBinary",RString(dlg.PathtoBinary->url().ascii()));
		SessionParams.Set("DebugSim", dlg.CBDebugSim->isChecked());
		SessionParams.Set("DebugBehaviour", dlg.CBDebugBehaviour->isChecked());
		SessionParams.Set("DebugMinSim", dlg.CBDebugMinSim->isChecked());
		SessionParams.Set("AutomaticMinSim", dlg.CBAutomaticMinSim->isChecked());
		SessionParams.Set("MinSim",atof(dlg.MinSim->text()));
		QMessageBox::information(this," Warning "," Remember: the session has to be restarted to take your modifications into account !");
		if (dlg.CBDebugSim->isChecked()||dlg.CBDebugMinSim->isChecked())
			QMessageBox::information(this," Warning "," Debug similarities mode if available for IFF configuration only!");
	}
}

