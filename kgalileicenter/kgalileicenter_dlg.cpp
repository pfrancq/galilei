/*

	GALILEI Research Project

	KGALILEICenter_dlg.cpp

	Main Window - Implementation of the Dialog part.

	Copyright 2002 by the Universit�Libre de Bruxelles.

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


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qvalidator.h>
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
void KGALILEICenterApp::slotPlugins(void)
{
	GFactoryLangCursor Lang;
	GFactoryFilterCursor Filter;
	GFactoryProfileCalcCursor ProfileCalc;
	GFactoryLinkCalcCursor LinkCalc;
	GFactoryGroupCalcCursor GroupCalc;
	GFactoryGroupingCursor Grouping;
	GFactoryStatsCalcCursor StatsCalc;
	GFactoryPostDocCursor PostDoc;
	GFactoryPostDocOrder* pdorder;
	GFactoryDocAnalyseCursor DocAnalyse;
	GFactoryPostGroupCursor PostGroup;
	GFactoryPostGroupOrder* pgorder;
	GFactoryEngineCursor Engine;
	GFactoryMetaEngineCursor MetaEngine;
	QPlugins dlg(this,"Plugins Dialog");
	QString str;
	QListViewItem* def;
	QListViewItem* cur;
	unsigned int i;
	int idx;


	//set the plugins path
	dlg.PluginsPath->setMode(KFile::Directory);
	dlg.PluginsPath->setURL(ToQString(pluginsPath));

	// Goes through filters
	def=cur=0;
	Filter=URLManager->GetFiltersCursor();
	for(Filter.Start();!Filter.End();Filter.Next())
	{
		str=ToQString(Filter()->GetName());
		str+=" [";
		str+=ToQString(Filter()->GetLib());
		str+="]";
		cur=new QFilterItem(dlg.Filters,Filter(),str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Filters->setSelected(def,true);
		dlg.changeFilter(def);
		dlg.EnableFilter->setEnabled(true);
	}

	// Goes through the profiles computing method
	def=cur=0;
	ProfileCalc=ProfilingManager->GetProfileCalcsCursor();
	dlg.CurrentProfileCalc->insertItem("None",0);
	for(ProfileCalc.Start(),idx=1;!ProfileCalc.End();ProfileCalc.Next(),idx++)
	{
		str=ToQString(ProfileCalc()->GetName());
		str+=" [";
		str+=ToQString(ProfileCalc()->GetLib());
		str+="]";
		cur=new QProfileCalcItem(dlg.ProfileCalcs,ProfileCalc(),str);
		dlg.CurrentProfileCalc->insertItem(ToQString(ProfileCalc()->GetName()),idx);
		if((ProfileCalc()->GetPlugin())&&(ProfileCalc()->GetPlugin()==ProfilingManager->GetCurrentMethod()))
			dlg.CurrentProfileCalc->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.ProfileCalcs->setSelected(def,true);
		dlg.changeProfileCalc(def);
		dlg.EnableProfileCalc->setEnabled(true);
		dlg.CurrentProfileCalc->setEnabled(true);
	}

	// Goes through the grouping method
	def=cur=0;
	Grouping=GroupingManager->GetGroupingsCursor();
	dlg.CurrentGrouping->insertItem("None",0);
	for(Grouping.Start(),idx=1;!Grouping.End();Grouping.Next(),idx++)
	{
		str=ToQString(Grouping()->GetName());
		str+=" [";
		str+=ToQString(Grouping()->GetLib());
		str+="]";
		cur=new QGroupingItem(dlg.Groupings,Grouping(),str);
		dlg.CurrentGrouping->insertItem(ToQString(Grouping()->GetName()),idx);
		if((Grouping()->GetPlugin())&&(Grouping()->GetPlugin()==GroupingManager->GetCurrentMethod()))
			dlg.CurrentGrouping->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Groupings->setSelected(def,true);
		dlg.changeGrouping(def);
		dlg.EnableGrouping->setEnabled(true);
		dlg.CurrentGrouping->setEnabled(true);
	}

	// Goes through the group computing method
	def=cur=0;
	GroupCalc=GroupCalcManager->GetGroupCalcsCursor();
	dlg.CurrentGroupCalc->insertItem("None",0);
	for(GroupCalc.Start(),idx=1;!GroupCalc.End();GroupCalc.Next(),idx++)
	{
		str=ToQString(GroupCalc()->GetName());
		str+=" [";
		str+=ToQString(GroupCalc()->GetLib());
		str+="]";
		cur=new QGroupCalcItem(dlg.GroupCalcs,GroupCalc(),str);
		dlg.CurrentGroupCalc->insertItem(ToQString(GroupCalc()->GetName()),idx);
		if((GroupCalc()->GetPlugin())&&(GroupCalc()->GetPlugin()==GroupCalcManager->GetCurrentMethod()))
			dlg.CurrentGroupCalc->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.GroupCalcs->setSelected(def,true);
		dlg.changeGroupCalc(def);
		dlg.EnableGroupCalc->setEnabled(true);
		dlg.CurrentGroupCalc->setEnabled(true);
	}

	// Goes through statistics
	def=cur=0;
	StatsCalc=StatsCalcManager->GetStatsCalcsCursor();
	for(StatsCalc.Start();!StatsCalc.End();StatsCalc.Next())
	{
		str=ToQString(StatsCalc()->GetName());
		str+=" [";
		str+=ToQString(StatsCalc()->GetLib());
		str+="]";
		cur=new QStatsCalcItem(dlg.Stats,StatsCalc(),str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Stats->setSelected(def,true);
		dlg.changeStatCalc(def);
		dlg.EnableStat->setEnabled(true);
	}

	// Goes through the linking method
	def=cur=0;
	LinkCalc=LinkCalcManager->GetLinkCalcsCursor();
	dlg.CurrentLinkCalc->insertItem("None",0);
	for(LinkCalc.Start(),idx=1;!LinkCalc.End();LinkCalc.Next(),idx++)
	{
		str=ToQString(LinkCalc()->GetName());
		str+=" [";
		str+=ToQString(LinkCalc()->GetLib());
		str+="]";
		cur=new QLinkCalcItem(dlg.LinkCalcs,LinkCalc(),str);
		dlg.CurrentLinkCalc->insertItem(ToQString(LinkCalc()->GetName()),idx);
		if((LinkCalc()->GetPlugin())&&(LinkCalc()->GetPlugin()==LinkCalcManager->GetCurrentMethod()))
			dlg.CurrentLinkCalc->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.LinkCalcs->setSelected(def,true);
		dlg.changeLinkCalc(def);
		dlg.EnableLinkCalc->setEnabled(true);
		dlg.CurrentLinkCalc->setEnabled(true);
	}

	// Goes through PostGroups
	def=cur=0;
	PostGroup=PostGroupManager->GetPostGroupsCursor();
	RContainer<GFactoryPostGroupOrder,true,true>* postgroupsordered;
	postgroupsordered=new RContainer<GFactoryPostGroupOrder,true,true>(PostGroup.GetNb());
	for(PostGroup.Start(),idx=1;!PostGroup.End();PostGroup.Next(), idx++)
	{
		pgorder=new GFactoryPostGroupOrder;
		pgorder->Fac=PostGroup();
		postgroupsordered->InsertPtr(pgorder);
	}
	for (i=postgroupsordered->NbPtr; i;i--)
	{
		str=ToQString(postgroupsordered->GetPtrAt(i-1)->Fac->GetName());
		str+=" [";
		str+=ToQString(postgroupsordered->GetPtrAt(i-1)->Fac->GetLib());
		str+="]";
		cur=new QPostGroupItem(dlg.PostGroups,postgroupsordered->GetPtrAt(i-1)->Fac,str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.PostGroups->setSelected(def,true);
		dlg.changePostGroup(def);
		dlg.EnablePostGroup->setEnabled(true);
	}

	// Goes through the PostDoc method
	def=cur=0;
	PostDoc=PostDocManager->GetPostDocsCursor();
	RContainer<GFactoryPostDocOrder,true,true>* postdocsordered;
	postdocsordered=new RContainer<GFactoryPostDocOrder,true,true>(PostDoc.GetNb());
	for(PostDoc.Start(),idx=1;!PostDoc.End();PostDoc.Next(),idx++)
	{
		pdorder=new GFactoryPostDocOrder;
		pdorder->Fac=PostDoc();
		postdocsordered->InsertPtr(pdorder);
	}
	for (i=postdocsordered->NbPtr; i;i--)
	{
		str=ToQString(postdocsordered->GetPtrAt(i-1)->Fac->GetName());
		str+=" [";
		str+=ToQString(postdocsordered->GetPtrAt(i-1)->Fac->GetLib());
		str+="]";
		cur=new QPostDocItem(dlg.PostDocs,postdocsordered->GetPtrAt(i-1)->Fac,str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.PostDocs->setSelected(def,true);
		dlg.changePostDoc(def);
		dlg.EnablePostDoc->setEnabled(true);
	}

	// Goes through languages
	def=cur=0;
	Lang=Langs->GetLangsCursor();
	for(Lang.Start();!Lang.End();Lang.Next())
	{
		str=ToQString(Lang()->GetName());
		str+=" [";
		str+=ToQString(Lang()->GetLib());
		str+="]";
		cur=new QLangItem(dlg.Langs,Lang(),str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Langs->setSelected(def,true);
		dlg.changeLang(def);
		dlg.EnableLang->setEnabled(true);
	}

	// Goes through the document analysis method
	def=cur=0;
	DocAnalyse=DocAnalyseManager->GetDocAnalysesCursor();
	dlg.CurrentDocAnalyse->insertItem("None",0);
	for(DocAnalyse.Start(),idx=1;!DocAnalyse.End();DocAnalyse.Next(),idx++)
	{
		str=ToQString(DocAnalyse()->GetName());
		str+=" [";
		str+=ToQString(DocAnalyse()->GetLib());
		str+="]";
		cur=new QDocAnalyseItem(dlg.DocAnalyses,DocAnalyse(),str);
		dlg.CurrentDocAnalyse->insertItem(ToQString(DocAnalyse()->GetName()),idx);
		if((DocAnalyse()->GetPlugin())&&(DocAnalyse()->GetPlugin()==DocAnalyseManager->GetCurrentMethod()))
			dlg.CurrentDocAnalyse->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.DocAnalyses->setSelected(def,true);
		dlg.changeDocAnalyse(def);
		dlg.EnableDocAnalyse->setEnabled(true);
		dlg.CurrentDocAnalyse->setEnabled(true);
	}

	// Goes through engines
	def=cur=0;
	Engine=EngineManager->GetEnginesCursor();
	for(Engine.Start();!Engine.End();Engine.Next())
	{
		str=ToQString(Engine()->GetName());
		str+=" [";
		str+=ToQString(Engine()->GetLib());
		str+="]";
		cur=new QEngineItem(dlg.Engines,Engine(),str);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.Engines->setSelected(def,true);
		dlg.changeEngine(def);
		dlg.EnableEngine->setEnabled(true);
	}

	// Goes through the meta engine method
	def=cur=0;
	MetaEngine=EngineManager->GetMetaEnginesCursor();
	dlg.CurrentMetaEngine->insertItem("None",0);
	for(MetaEngine.Start(),idx=1;!MetaEngine.End();MetaEngine.Next(),idx++)
	{
		str=ToQString(MetaEngine()->GetName());
		str+=" [";
		str+=ToQString(MetaEngine()->GetLib());
		str+="]";
		cur=new QMetaEngineItem(dlg.MetaEngines,MetaEngine(),str);
		dlg.CurrentMetaEngine->insertItem(ToQString(MetaEngine()->GetName()),idx);
		if((MetaEngine()->GetPlugin())&&(MetaEngine()->GetPlugin()==EngineManager->GetCurrentMethod()))
			dlg.CurrentMetaEngine->setCurrentItem(idx);
		if(!def)
			def=cur;
	}
	if(def)
	{
		dlg.MetaEngines->setSelected(def,true);
		dlg.changeMetaEngine(def);
		dlg.EnableMetaEngine->setEnabled(true);
		dlg.CurrentMetaEngine->setEnabled(true);
	}
	
	if(dlg.exec())
	{
		// read the plugins path
		if (strcmp(pluginsPath, dlg.PluginsPath->url()))
		{
			pluginsPath=RString(dlg.PluginsPath->url().ascii());
			QMessageBox::information(this,"Plugins Path has changed","You changed the plugins path, please restart KGALILEICenter.");
		}
		// Goes through filters
		QFilterItem* item=dynamic_cast<QFilterItem*>(dlg.Filters->firstChild());
		while(item)
		{
			if(item->Enable)
				item->Fac->Create();
			else
				item->Fac->Delete();
			item=dynamic_cast<QFilterItem*>(item->itemBelow());
		}

		// Goes through profiles computing method
		QProfileCalcItem* item2=dynamic_cast<QProfileCalcItem*>(dlg.ProfileCalcs->firstChild());
		while(item2)
		{
			if(item2->Enable)
				item2->Fac->Create(getSession());
			else
				item2->Fac->Delete(getSession());
			item2=dynamic_cast<QProfileCalcItem*>(item2->itemBelow());
		}
		try
		{
			ProfilingManager->SetCurrentMethod(dlg.CurrentProfileCalc->currentText());
		}
		catch(GException)
		{
		}

		// Goes through grouping method
		QGroupingItem* item3=dynamic_cast<QGroupingItem*>(dlg.Groupings->firstChild());
		while(item3)
		{
			if(item3->Enable)
				item3->Fac->Create(getSession());
			else
				item3->Fac->Delete(getSession());
			item3=dynamic_cast<QGroupingItem*>(item3->itemBelow());
		}
		try
		{
			GroupingManager->SetCurrentMethod(dlg.CurrentGrouping->currentText());
		}
		catch(GException)
		{
		}

		// Goes through groups computing method
		QGroupCalcItem* item4=dynamic_cast<QGroupCalcItem*>(dlg.GroupCalcs->firstChild());
		while(item4)
		{
			if(item4->Enable)
				item4->Fac->Create(getSession());
			else
				item4->Fac->Delete(getSession());
			item4=dynamic_cast<QGroupCalcItem*>(item4->itemBelow());
		}
		try
		{
			GroupCalcManager->SetCurrentMethod(dlg.CurrentGroupCalc->currentText());
		}
		catch(GException)
		{
		}

		// Goes through statistics
		QStatsCalcItem* item5=dynamic_cast<QStatsCalcItem*>(dlg.Stats->firstChild());
		while(item5)
		{
			if(item5->Enable)
				item5->Fac->Create(getSession());
			else
				item5->Fac->Delete(getSession());
			item5=dynamic_cast<QStatsCalcItem*>(item5->itemBelow());
		}

		// Goes through the linking method
		QLinkCalcItem* item6=dynamic_cast<QLinkCalcItem*>(dlg.LinkCalcs->firstChild());
		while(item6)
		{
			if(item6->Enable)
				item6->Fac->Create(getSession());
			else
				item6->Fac->Delete(getSession());
			item6=dynamic_cast<QLinkCalcItem*>(item6->itemBelow());
		}
		try
		{
			LinkCalcManager->SetCurrentMethod(dlg.CurrentLinkCalc->currentText());
		}
		catch(GException)
		{
		}

		// Goes through the PostDoc method
		QPostDocItem* item7=dynamic_cast<QPostDocItem*>(dlg.PostDocs->firstChild());
		while(item7)
		{
			if(item7->Enable)
				item7->Fac->Create(getSession());
			else
				item7->Fac->Delete(getSession());
			item7=dynamic_cast<QPostDocItem*>(item7->itemBelow());
		}

		// Goes through the languages method
		QLangItem* item8=dynamic_cast<QLangItem*>(dlg.Langs->firstChild());
		while(item8)
		{
			if(item8->Enable)
			{
				try
				{
					item8->Fac->Create(getSession());
				}
				catch(GException &e)
				{
					QMessageBox::information(this,"Error",e.GetMsg());
				}
			}
			else
				item8->Fac->Delete(getSession());
			item8=dynamic_cast<QLangItem*>(item8->itemBelow());
		}

		// Goes through groups computing method
		QDocAnalyseItem* item9=dynamic_cast<QDocAnalyseItem*>(dlg.DocAnalyses->firstChild());
		while(item9)
		{
			if(item9->Enable)
				item9->Fac->Create(getSession());
			else
				item9->Fac->Delete(getSession());
			item9=dynamic_cast<QDocAnalyseItem*>(item9->itemBelow());
		}
		try
		{
			DocAnalyseManager->SetCurrentMethod(dlg.CurrentDocAnalyse->currentText());
		}
		catch(GException)
		{
		}

		// Goes through PostGroups
		QPostGroupItem* item10=dynamic_cast<QPostGroupItem*>(dlg.PostGroups->firstChild());
		while(item10)
		{
			if(item10->Enable)
				item10->Fac->Create(getSession());
			else
				item10->Fac->Delete(getSession());
			item10=dynamic_cast<QPostGroupItem*>(item10->itemBelow());
		}
		
		// Goes through engines
		QEngineItem* item11=dynamic_cast<QEngineItem*>(dlg.Engines->firstChild());
		while(item11)
		{
			if(item11->Enable)
				item11->Fac->Create();
			else
				item11->Fac->Delete();
			item11=dynamic_cast<QEngineItem*>(item11->itemBelow());
		}
		
		// Goes through the meta engine method
		QMetaEngineItem* item12=dynamic_cast<QMetaEngineItem*>(dlg.MetaEngines->firstChild());
		while(item12)
		{
			if(item12->Enable)
				item12->Fac->Create(getSession());
			else
				item12->Fac->Delete(getSession());
			item12=dynamic_cast<QMetaEngineItem*>(item12->itemBelow());
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
	dlg.CBGroupsHistory->setChecked(SessionParams.GetBool("SaveGroupsHistory"));
	dlg.CBProfilesHistory->setChecked(SessionParams.GetBool("SaveProfilesHistory"));
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
		SessionParams.Set("SaveGroupsHistory", dlg.CBGroupsHistory->isChecked());
		SessionParams.Set("SaveProfilesHistory", dlg.CBProfilesHistory->isChecked());
		SessionParams.Set("PathtoBinary",RString(dlg.PathtoBinary->url().ascii()));
		SessionParams.Set("DebugSim", dlg.CBDebugSim->isChecked());
		SessionParams.Set("DebugBehaviour", dlg.CBDebugBehaviour->isChecked());
		SessionParams.Set("DebugMinSim", dlg.CBDebugMinSim->isChecked());
		SessionParams.Set("AutomaticMinSim", dlg.CBAutomaticMinSim->isChecked());
		SessionParams.Set("MinSim",atof(dlg.MinSim->text()));
		if(dlg.CBProfilesHistory->isChecked()&&!dlg.CBGroupsHistory->isChecked())
		{
			QMessageBox::information(this,"Historic Options","historical groups will be saved (needed to save histoical profiles)");
			SessionParams.Set("SaveGroupsHistory", true);
		}
		QMessageBox::information(this," Warning "," Remember: the session has to be restarted to take your modifications into account !");
		if (dlg.CBDebugSim->isChecked()||dlg.CBDebugMinSim->isChecked())
			QMessageBox::information(this," Warning "," Debug similarities mode if available for IFF configuration only!");
	}
}

