/*

	KGALILEICenter

	QPlugInsList.cpp

	Widget to configure a list of plug-ins - Implementation.

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
// include files for R/GALILEI Project
#include <ggalileiapp.h>
#include <rqt.h>
#include <gdocanalyse.h>
#include <gfilter.h>
#include <glinkcalc.h>
#include <gpostdoc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <gpostcommunity.h>
#include <gposttopic.h>
#include <glang.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gprofilecalc.h>
#include <gstatscalc.h>
#include <gstorage.h>
#include <gmeasure.h>
#include <gtool.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
// include files for current application
#include <qpluginslist.h>



//------------------------------------------------------------------------------
//
// class QPlugIn
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * The QPlugIn represents a plug-in in the list.
 */
class QPlugIn : public QListWidgetItem
{
public:
	GPluginConfig* PlugIn;          // Plug-in Configuration.
	bool Enable;                    // Must the plug-in be enabled?
	bool WasEnable;                 // Was the plug-in enabled ?

	QPlugIn(QListWidget* lst,GPluginConfig* fac,const QString& desc)
		: QListWidgetItem(desc,lst), PlugIn(fac), Enable(fac->IsCreated()), WasEnable(fac->IsCreated())	{}
};



//------------------------------------------------------------------------------
//
// class QPlugInsList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QPlugInsList::QPlugInsList(QWidget* parent)
	: QWidget(parent), Ui_QPlugInsList()
{
	setupUi(this);
}


//-----------------------------------------------------------------------------
template<class Factory,class Manager>
	void QPlugInsList::init(Manager* mng,bool current,bool enable,bool updown)
{
	QPlugIn* def;
	QPlugIn* cur;
	QString str;
	int idx;

	// Init part
	HasCurrent=current;
	HasUpDown=updown;
	Current->setVisible(current);
	Enable->setVisible(enable);
	Up->setVisible(updown);
	Down->setVisible(updown);
	R::RCursor<Factory> Cur(mng->GetFactories());
	Factory* Fac=mng->GetCurrentFactory(false);

	// Goes through the plug-ins
	def=cur=0;
	if(current)
		Current->insertItem(0,"None");
	cur=0;
	for(Cur.Start(),idx=1;!Cur.End();Cur.Next(),idx++)
	{
		str=ToQString(Cur()->GetName());
		str+=" [";
		str+=ToQString(Cur()->GetLib());
		str+="]";
		cur=new QPlugIn(List,Cur(),str);
		if(current)
		{
			Current->insertItem(idx,ToQString(Cur()->GetName()));
			if(Cur()==Fac)
				Current->setCurrentIndex(idx);
		}
		if(!def)
			def=cur;
	}
	if(def)
	{
		List->setCurrentItem(def,QItemSelectionModel::Select);
		if(enable)
			Enable->setEnabled(true);
		if(current)
			Current->setEnabled(true);
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::init(PlugInType type,const RString& cat)
{
	Type=type;
	Cat=cat;
	switch(Type)
	{
		case Storages:
			setObjectName("Storage");
			init<GFactoryStorage,GStorageManager>(GALILEIApp->GetManager<GStorageManager>("Storage"),true,false,false);
			break;
		case Filters:
			setObjectName("Filters");
			init<GFactoryFilter,GFilterManager>(GALILEIApp->GetManager<GFilterManager>("Filter"),false,true,false);
			break;
		case Langs:
			setObjectName("Languages");
			init<GFactoryLang,GLangManager>(GALILEIApp->GetManager<GLangManager>("Lang"),false,true,false);
			break;
		case PostProfiles:
			setObjectName("PostProfiles");
			init<GFactoryPostProfile,GPostProfileManager>(GALILEIApp->GetManager<GPostProfileManager>("PostProfile"),false,true,true);
			break;
		case PostCommunities:
			setObjectName("PostCommunities");
			init<GFactoryPostCommunity,GPostCommunityManager>(GALILEIApp->GetManager<GPostCommunityManager>("PostCommunity"),false,true,true);
			break;
		case ComputeSugs:
			setObjectName("ComputeSugs");
			init<GFactoryComputeSugs,GComputeSugsManager>(GALILEIApp->GetManager<GComputeSugsManager>("ComputeSugs"),false,true,true);
			break;
		case ComputeTrust:
			setObjectName("ComputeTrust");
			init<GFactoryComputeTrust,GComputeTrustManager>(GALILEIApp->GetManager<GComputeTrustManager>("ComputeTrust"),false,true,true);
			break;
		case ProfileCalcs:
			setObjectName("ProfileCalcs");
			init<GFactoryProfileCalc,GProfileCalcManager>(GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc"),true,true,false);
			break;
		case GroupProfiles:
			setObjectName("GroupProfiles");
			init<GFactoryGroupProfiles,GGroupProfilesManager>(GALILEIApp->GetManager<GGroupProfilesManager>("GroupProfiles"),true,true,false);
			break;
		case GroupDocs:
			setObjectName("GroupDocs");
			init<GFactoryGroupDocs,GGroupDocsManager>(GALILEIApp->GetManager<GGroupDocsManager>("GroupDocs"),true,true,false);
			break;
		case CommunityCalcs:
			setObjectName("CommunityCalcs");
			init<GFactoryCommunityCalc,GCommunityCalcManager>(GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc"),true,true,false);
			break;
		case TopicCalcs:
			setObjectName("TopicCalcs");
			init<GFactoryTopicCalc,GTopicCalcManager>(GALILEIApp->GetManager<GTopicCalcManager>("TopicCalc"),true,true,false);
			break;
		case StatsCalcs:
			setObjectName("StatsCalcs");
			init<GFactoryStatsCalc,GStatsCalcManager>(GALILEIApp->GetManager<GStatsCalcManager>("StatsCalc"),false,true,false);
			break;
		case Tools:
			setObjectName("Tools");
			init<GFactoryTool,GToolManager>(GALILEIApp->GetManager<GToolManager>("Tool"),false,true,false);
			break;
		case LinkCalcs:
			setObjectName("LinkCalcs");
			init<GFactoryLinkCalc,GLinkCalcManager>(GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc"),true,true,false);
			break;
		case PostDocs:
			setObjectName("PostDocs");
			init<GFactoryPostDoc,GPostDocManager>(GALILEIApp->GetManager<GPostDocManager>("PostDoc"),false,true,true);
			break;
		case PreProfiles:
			setObjectName("PreProfiles");
			init<GFactoryPreProfile,GPreProfileManager>(GALILEIApp->GetManager<GPreProfileManager>("PreProfile"),false,true,true);
			break;
		case PostTopics:
			setObjectName("PostTopics");
			init<GFactoryPostTopic,GPostTopicManager>(GALILEIApp->GetManager<GPostTopicManager>("PostTopic"),false,true,true);
			break;
		case DocAnalyses:
			setObjectName("DocAnalyses");
			init<GFactoryDocAnalyse,GDocAnalyseManager>(GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse"),true,true,false);
			break;
		case Engines:
			setObjectName("Engines");
			init<GFactoryEngine,GEngineManager>(GALILEIApp->GetManager<GEngineManager>("Engine"),false,true,false);
			break;
		case MetaEngines:
			setObjectName("MetaEngines");
			init<GFactoryMetaEngine,GMetaEngineManager>(GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine"),true,true,false);
			break;
		case Measures:
			setObjectName(ToQString(Cat));
			init<GFactoryMeasure,GTypeMeasureManager>(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetMeasureCategory(Cat),true,true,false);
			break;
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::apply(GSession* session)
{
	for(int i=0;i<List->count();i++)
	{
		QPlugIn* item=dynamic_cast<QPlugIn*>(List->item(i));
		item->PlugIn->SetLevel(i);
		if(HasUpDown)
			item->PlugIn->SetInt("Level",i);
		item->PlugIn->Apply();

		if(session)
		{
			if(item->Enable)
			{
				if(!item->WasEnable)
					item->PlugIn->Create(session);
			}
			else
			{
				if(item->WasEnable)
					item->PlugIn->Delete(session);
			}
		}
		else
		{
			if(item->Enable)
			{
				if(!item->WasEnable)
					item->PlugIn->Create();
			}
			else
			{
				if(item->WasEnable)
					item->PlugIn->Delete();
			}
		}
	}

	if(HasCurrent)
	{
		switch(Type)
		{
			case Storages:
				GALILEIApp->GetManager<GStorageManager>("Storage")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case GroupDocs:
				GALILEIApp->GetManager<GGroupDocsManager>("GroupDocs")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case GroupProfiles:
				GALILEIApp->GetManager<GGroupProfilesManager>("GroupProfiles")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case CommunityCalcs:
				GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case TopicCalcs:
				GALILEIApp->GetManager<GTopicCalcManager>("TopicCalc")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case MetaEngines:
				GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case DocAnalyses:
				GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case LinkCalcs:
				GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case ProfileCalcs:
				GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			case Measures:
				GALILEIApp->GetManager<GMeasureManager>("Measures")->GetMeasureCategory(Cat)->SetCurrentMethod(FromQString(Current->currentText()),false);
				break;
			default:
				std::cout<<objectName().toLatin1().data()<<" is not treated"<<std::endl;
		}
	}
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotConfigure(void)
{
	 if(!List->currentItem()) return;
	 QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	 if(!f) return;
	 f->PlugIn->Configure();
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotAbout(void)
{
	 if(!List->currentItem()) return;
	 QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	 if(!f) return;
	 f->PlugIn->About();
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotChange(QListWidgetItem* act,QListWidgetItem*)
{
	if(!act) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(act);
	if(!f) return;
	Configure->setEnabled(f->PlugIn->HasConfigure());
	About->setEnabled(f->PlugIn->HasAbout());
	Enable->setChecked(f->Enable);
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotEnable(bool state)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	f->Enable=state;
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotUp(void)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	int pos=List->currentRow();
	if(!pos) return;
	List->takeItem(pos);
	List->insertItem(pos-1,f);
}


//-----------------------------------------------------------------------------
void QPlugInsList::slotDown(void)
{
	if(!List->currentItem()) return;
	QPlugIn* f=dynamic_cast<QPlugIn*>(List->currentItem());
	if(!f) return;
	int pos=List->currentRow();
	if(pos==List->count()-1) return;
	List->takeItem(pos);
	List->insertItem(pos+1,f);
}
