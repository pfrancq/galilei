/*

	GALILEI Research Project

	KGALILEICenter_dlg.cpp

	Main Window - Implementation of the Dialog part.

	Copyright 2002 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <docs/gdocoptions.h>
#include <galilei/gurlmanagerkde.h>
#include <galilei/qlistviewitemtype.h>
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <groups/ggrouping.h>
#include <profiles/gprofilecalc.h>
using namespace GALILEI;


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


//-----------------------------------------------------------------------------
// application specific includes
#include "kgalileicenter.h"
#include "kdoc.h"
#include "kview.h"
#include "kviewusers.h"
#include "kviewdoc.h"
#include "kviewprofile.h"
#include "qdocsoptionsdlg.h"
#include "qsessionoptions.h"
#include "qplugins.h"
#include "qgalileiitem.h"



//-----------------------------------------------------------------------------
//
// class KGALILEICenterApp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotDocsOptions(void)
{
	QString tmp;

	QDocsOptionsDlg dlg(this,0,true);
	dlg.cbStaticLang->setChecked(DocOptions->StaticLang);
	tmp.setNum(DocOptions->MinStopWords);
	dlg.txtMinStop->setText(tmp);
	dlg.txtMinWordSize->setValue(DocOptions->MinWordSize);
	dlg.txtMinStemSize->setValue(DocOptions->MinStemSize);
	dlg.txtMinOcc->setValue(DocOptions->MinOccur);
	dlg.txtMinDocs->setValue(DocOptions->MinDocs);
	dlg.txtMaxDocs->setValue(DocOptions->MaxDocs);
	dlg.txtMinOccC->setValue(DocOptions->MinOccurCluster);
	dlg.txtNbIteration->setValue(DocOptions->NbIteration);
	dlg.txtWindowSize->setValue(DocOptions->WindowSize);
	dlg.txtNbDocsMin->setValue(DocOptions->NbDocsMin);
	tmp.setNum(DocOptions->MinConfidence);
	dlg.txtMinConf->setText(tmp);
	dlg.cbDistance->setChecked(DocOptions->Distance);
	dlg.cbNonWord->setChecked(DocOptions->NonLetterWords);
	dlg.frameNonWord->setEnabled(DocOptions->NonLetterWords);
	dlg.cbUseLink->setChecked(DocOptions->UseLink);
	dlg.cbExternalLink->setChecked(DocOptions->UseExternalLink);
	dlg.cbRedirection->setChecked(DocOptions->UseRedirection);

	if(dlg.exec())
	{
		double d=dlg.txtMinStop->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			DocOptions->MinStopWords=d;
		DocOptions->StaticLang=dlg.cbStaticLang->isChecked();
		DocOptions->MinWordSize=dlg.txtMinWordSize->value();
		DocOptions->MinStemSize=dlg.txtMinStemSize->value();
		DocOptions->MinOccur=dlg.txtMinOcc->value();
		DocOptions->MinDocs=dlg.txtMinDocs->value();
		DocOptions->MaxDocs=dlg.txtMaxDocs->value();
		DocOptions->MinOccurCluster=dlg.txtMinOccC->value();
		DocOptions->NbIteration=dlg.txtNbIteration->value();
		DocOptions->WindowSize=dlg.txtWindowSize->value();
		DocOptions->NbDocsMin=dlg.txtNbDocsMin->value();
		d=dlg.txtMinConf->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			DocOptions->MinConfidence=d;
		DocOptions->Distance=dlg.cbDistance->isChecked();
		DocOptions->NonLetterWords=dlg.cbNonWord->isChecked();
		DocOptions->UseLink=dlg.cbUseLink->isChecked();
		DocOptions->UseExternalLink=dlg.cbExternalLink->isChecked();
		DocOptions->UseRedirection=dlg.cbRedirection->isChecked();

		cout<< "use sturct : "<<DocOptions->Distance<<" et a la source : "<<dlg.cbDistance->isChecked()<<endl;
		cout<< "option docs :  "<<DocOptions->UseLink<<" et a la source : "<< dlg.cbUseLink->isChecked() <<endl;;
	}
}                                


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotPlugins(void)
{
	GFactoryFilterCursor Filter;
	GFactoryProfileCalcCursor ProfileCalc;
	GFactoryLinkCalcCursor LinkCalc;
	GFactoryGroupCalcCursor GroupCalc;
	GFactoryGroupingCursor Grouping;
	GFactoryStatsCalcCursor StatsCalc;
	QPlugins dlg(this,"Plugins Dialog");
	QString str;
	QListViewItem* def;
	QListViewItem* cur;
	int idx;

	// Goes through filters
	def=cur=0;
	Filter=URLManager.GetFiltersCursor();
	for(Filter.Start();!Filter.End();Filter.Next())
	{
		str=Filter()->GetName();
		str+=" [";
		str+=Filter()->GetLib();
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
	ProfileCalc=ProfilingManager.GetProfileCalcsCursor();
	dlg.CurrentProfileCalc->insertItem("None",0);
	for(ProfileCalc.Start(),idx=1;!ProfileCalc.End();ProfileCalc.Next(),idx++)
	{
		str=ProfileCalc()->GetName();
		str+=" [";
		str+=ProfileCalc()->GetLib();
		str+="]";
		cur=new QProfileCalcItem(dlg.ProfileCalcs,ProfileCalc(),str);
		dlg.CurrentProfileCalc->insertItem(ProfileCalc()->GetName(),idx);
		if((ProfileCalc()->GetPlugin())&&(ProfileCalc()->GetPlugin()==ProfilingManager.GetCurrentMethod()))
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
	Grouping=GroupingManager.GetGroupingsCursor();
	dlg.CurrentGrouping->insertItem("None",0);
	for(Grouping.Start(),idx=1;!Grouping.End();Grouping.Next(),idx++)
	{
		str=Grouping()->GetName();
		str+=" [";
		str+=Grouping()->GetLib();
		str+="]";
		cur=new QGroupingItem(dlg.Groupings,Grouping(),str);
		dlg.CurrentGrouping->insertItem(Grouping()->GetName(),idx);
		if((Grouping()->GetPlugin())&&(Grouping()->GetPlugin()==GroupingManager.GetCurrentMethod()))
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
	GroupCalc=GroupCalcManager.GetGroupCalcsCursor();
	dlg.CurrentGroupCalc->insertItem("None",0);
	for(GroupCalc.Start(),idx=1;!GroupCalc.End();GroupCalc.Next(),idx++)
	{
		str=GroupCalc()->GetName();
		str+=" [";
		str+=GroupCalc()->GetLib();
		str+="]";
		cur=new QGroupCalcItem(dlg.GroupCalcs,GroupCalc(),str);
		dlg.CurrentGroupCalc->insertItem(GroupCalc()->GetName(),idx);
		if((GroupCalc()->GetPlugin())&&(GroupCalc()->GetPlugin()==GroupCalcManager.GetCurrentMethod()))
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
	StatsCalc=StatsCalcManager.GetStatsCalcsCursor();
	for(StatsCalc.Start();!StatsCalc.End();StatsCalc.Next())
	{
		str=StatsCalc()->GetName();
		str+=" [";
		str+=StatsCalc()->GetLib();
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
	LinkCalc=LinkCalcManager.GetLinkCalcsCursor();
	dlg.CurrentLinkCalc->insertItem("None",0);
	for(LinkCalc.Start(),idx=1;!LinkCalc.End();LinkCalc.Next(),idx++)
	{
		str=LinkCalc()->GetName();
		str+=" [";
		str+=LinkCalc()->GetLib();
		str+="]";
		cur=new QLinkCalcItem(dlg.LinkCalcs,LinkCalc(),str);
		dlg.CurrentLinkCalc->insertItem(LinkCalc()->GetName(),idx);
		if((LinkCalc()->GetPlugin())&&(LinkCalc()->GetPlugin()==LinkCalcManager.GetCurrentMethod()))
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

	if(dlg.exec())
	{
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
				item2->Fac->Create();
			else
				item2->Fac->Delete();
			item2=dynamic_cast<QProfileCalcItem*>(item2->itemBelow());
		}
		try
		{
			ProfilingManager.SetCurrentMethod(dlg.CurrentProfileCalc->currentText());
		}
		catch(GException)
		{
		}


		// Goes through grouping method
		QGroupingItem* item3=dynamic_cast<QGroupingItem*>(dlg.Groupings->firstChild());
		while(item3)
		{
			if(item3->Enable)
				item3->Fac->Create();
			else
				item3->Fac->Delete();
			item3=dynamic_cast<QGroupingItem*>(item3->itemBelow());
		}
		try
		{
			GroupingManager.SetCurrentMethod(dlg.CurrentGrouping->currentText());
		}
		catch(GException)
		{
		}

		// Goes through groups computing method
		QGroupCalcItem* item4=dynamic_cast<QGroupCalcItem*>(dlg.GroupCalcs->firstChild());
		while(item4)
		{
			if(item4->Enable)
				item4->Fac->Create();
			else
				item4->Fac->Delete();
			item4=dynamic_cast<QGroupCalcItem*>(item4->itemBelow());
		}
		try
		{
			GroupCalcManager.SetCurrentMethod(dlg.CurrentGroupCalc->currentText());
		}
		catch(GException)
		{
		}

		// Goes through statistics
		QStatsCalcItem* item5=dynamic_cast<QStatsCalcItem*>(dlg.Stats->firstChild());
		while(item5)
		{
			if(item5->Enable)
				item5->Fac->Create();
			else
				item5->Fac->Delete();
			item5=dynamic_cast<QStatsCalcItem*>(item5->itemBelow());
		}

		// Goes through the linking method
		QLinkCalcItem* item6=dynamic_cast<QLinkCalcItem*>(dlg.LinkCalcs->firstChild());
		while(item6)
		{
			if(item6->Enable)
				item6->Fac->Create();
			else
				item6->Fac->Delete();
			item6=dynamic_cast<QLinkCalcItem*>(item6->itemBelow());
		}
		try
		{
			LinkCalcManager.SetCurrentMethod(dlg.CurrentLinkCalc->currentText());
		}
		catch(GException)
		{
		}
	}

}


//-----------------------------------------------------------------------------
void KGALILEICenterApp::slotSessionOptions(void)
{
	QSessionOptions dlg(this, "Session Options");
	dlg.SBDiffBehaviour->setValue(SessionParams.GetUInt("DiffBehaviourMinDocs"));
	dlg.SBSameBehaviour->setValue(SessionParams.GetUInt("SameBehaviourMinDocs"));

	if(dlg.exec())
	{
		SessionParams.Set("DiffBehaviourMinDocs",dlg.SBDiffBehaviour->value());
		SessionParams.Set("SameBehaviourMinDocs", dlg.SBSameBehaviour->value());
	}
}
