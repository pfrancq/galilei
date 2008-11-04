/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qcheckbox.h>
#include <kurlrequester.h>
#include "qgalileiitem.h"

void QPlugins::init()
{
    PostDocs->setSorting(-1);
    PostProfile->setSorting(-1);
    PostCommunity->setSorting(-1);
}

void QPlugins::changeFilter(QListViewItem* item)
{
 if(!item) return;
 QFilterItem* f=dynamic_cast<QFilterItem*>(item);
 if(!f) return;
 EnableFilter->setChecked(f->Enable);
 ConfigFilter->setEnabled(f->Fac->HasConfigure());
 AboutFilter->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutFilter()
{
 if(!Filters->currentItem()) return;
 QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
 if(!f) return;
 f->Fac->About();
}


void QPlugins::slotConfigFilter()
{
 if(!Filters->currentItem()) return;
 QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
 if(!f) return;
 f->Fac->Configure();
}


void QPlugins::slotFilterEnable( bool state )
{
 if(!Filters->currentItem()) return;
 QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
 if(!f) return;
 f->Enable=state;
}


void QPlugins::changeProfileCalc( QListViewItem * item )
{
 if(!item) return;
 QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(item);
 if(!f) return;
 EnableProfileCalc->setChecked(f->Enable);
 ConfigProfileCalc->setEnabled(f->Fac->HasConfigure());
 AboutProfileCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutProfileCalc()
{
	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigProfileCalc()
{
	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotProfileCalcEnable( bool state )
{
	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::changeGroupProfiles( QListViewItem * item)
{
	if(!item) return;
	QGroupProfilesItem* f=dynamic_cast<QGroupProfilesItem*>(item);
	if(!f) return;
	EnableGroupProfiles->setChecked(f->Enable);
	ConfigGroupProfiles->setEnabled(f->Fac->HasConfigure());
	AboutGroupProfiles->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeGroupDocs( QListViewItem * item)
{
	if(!item) return;
	QGroupDocsItem* f=dynamic_cast<QGroupDocsItem*>(item);
	if(!f) return;
	EnableGroupDocs->setChecked(f->Enable);
	ConfigGroupDocs->setEnabled(f->Fac->HasConfigure());
	AboutGroupDocs->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeLinkCalc( QListViewItem * item)
{
	if(!item) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(item);
	if(!f) return;
	EnableLinkCalc->setChecked(f->Enable);
	ConfigLinkCalc->setEnabled(f->Fac->HasConfigure());
	AboutLinkCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeCommunityCalc( QListViewItem * item )
{
	if(!item) return;
	QCommunityCalcItem* f=dynamic_cast<QCommunityCalcItem*>(item);
	if(!f) return;
	EnableCommunityCalc->setChecked(f->Enable);
	ConfigCommunityCalc->setEnabled(f->Fac->HasConfigure());
	AboutCommunityCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeTopicCalc( QListViewItem * item )
{
	if(!item) return;
	QTopicCalcItem* f=dynamic_cast<QTopicCalcItem*>(item);
	if(!f) return;
	EnableTopicCalc->setChecked(f->Enable);
	ConfigTopicCalc->setEnabled(f->Fac->HasConfigure());
	AboutTopicCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeMetaEngine( QListViewItem * item)
{
	if(!item) return;
	QMetaEngineItem* f=dynamic_cast<QMetaEngineItem*>(item);
	if(!f) return;
	EnableMetaEngine->setChecked(f->Enable);
	ConfigMetaEngine->setEnabled(f->Fac->HasConfigure());
	AboutMetaEngine->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutGroupProfiles()
{
	if(!GroupProfiles->currentItem()) return;
	QGroupProfilesItem* f=dynamic_cast<QGroupProfilesItem*>(GroupProfiles->currentItem());
	if(!f) return;
	f->Fac->About();
}

void QPlugins::slotAboutGroupDocs()
{
	if(!GroupDocs->currentItem()) return;
	QGroupDocsItem* f=dynamic_cast<QGroupDocsItem*>(GroupDocs->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotAboutLinkCalc()
{
	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotAboutCommunityCalc()
{
	if(!CommunityCalc->currentItem()) return;
	QCommunityCalcItem* f=dynamic_cast<QCommunityCalcItem*>(CommunityCalc->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotAboutTopicCalc()
{
	if(!TopicCalc->currentItem()) return;
	QTopicCalcItem* f=dynamic_cast<QTopicCalcItem*>(TopicCalc->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotAboutMetaEngine()
{
	if(!MetaEngines->currentItem()) return;
	QMetaEngineItem* f=dynamic_cast<QMetaEngineItem*>(MetaEngines->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigGroupProfiles()
{
	if(!GroupProfiles->currentItem()) return;
	QGroupProfilesItem* f=dynamic_cast<QGroupProfilesItem*>(GroupProfiles->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotConfigGroupDocs()
{
	if(!GroupDocs->currentItem()) return;
	QGroupDocsItem* f=dynamic_cast<QGroupDocsItem*>(GroupDocs->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotConfigLinkCalc()
{
	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotConfigCommunityCalc()
{
	if(!CommunityCalc->currentItem()) return;
	QCommunityCalcItem* f=dynamic_cast<QCommunityCalcItem*>(CommunityCalc->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotConfigTopicCalc()
{
	if(!TopicCalc->currentItem()) return;
	QTopicCalcItem* f=dynamic_cast<QTopicCalcItem*>(TopicCalc->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotConfigMetaEngine()
{
	if(!MetaEngines->currentItem()) return;
	QMetaEngineItem* f=dynamic_cast<QMetaEngineItem*>(MetaEngines->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotGroupProfilesEnable( bool state )
{
	if(!GroupProfiles->currentItem()) return;
	QGroupProfilesItem* f=dynamic_cast<QGroupProfilesItem*>(GroupProfiles->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotCommunityCalcEnable( bool state )
{
	if(!CommunityCalc->currentItem()) return;
	QCommunityCalcItem* f=dynamic_cast<QCommunityCalcItem*>(CommunityCalc->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotGroupDocsEnable( bool state )
{
	if(!GroupDocs->currentItem()) return;
	QGroupDocsItem* f=dynamic_cast<QGroupDocsItem*>(GroupDocs->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotTopicCalcEnable( bool state )
{
	if(!TopicCalc->currentItem()) return;
	QTopicCalcItem* f=dynamic_cast<QTopicCalcItem*>(TopicCalc->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotLinkCalcEnable( bool state )
{
	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotMetaEngineEnable( bool state )
{
	if(!MetaEngines->currentItem()) return;
	QMetaEngineItem* f=dynamic_cast<QMetaEngineItem*>(MetaEngines->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::changeStatCalc( QListViewItem *  item)
{
	if(!item) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(item);
	if(!f) return;
	EnableStat->setChecked(f->Enable);
	ConfigStat->setEnabled(f->Fac->HasConfigure());
	AboutStat->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutStatCalc()
{
	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigStatCalc()
{
	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotStatCalcEnable( bool state )
{
	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotPostDocEnable( bool state )
{
	if(!PostDocs->currentItem()) return;
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotConfigPostDoc()
{
	if(!PostDocs->currentItem()) return;
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::changePostDoc( QListViewItem * item )
{
	if(!item) return;
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(item);
	if(!f) return;
	EnablePostDoc->setChecked(f->Enable);
	ConfigPostDoc->setEnabled(f->Fac->HasConfigure());
	AboutPostDoc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutPostDoc()
{
	if(!PostDocs->currentItem()) return;
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::changePreProfile( QListViewItem * item )
{
	if(!item) return;
	QPreProfileItem* f=dynamic_cast<QPreProfileItem*>(item);
	if(!f) return;
	EnablePreProfile->setChecked(f->Enable);
	ConfigPreProfile->setEnabled(f->Fac->HasConfigure());
	AboutPreProfile->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changePostProfile( QListViewItem * item )
{
	if(!item) return;
	QPostProfileItem* f=dynamic_cast<QPostProfileItem*>(item);
	if(!f) return;
	EnablePostProfile->setChecked(f->Enable);
	ConfigPostProfile->setEnabled(f->Fac->HasConfigure());
	AboutPostProfile->setEnabled(f->Fac->HasAbout());
}

void QPlugins::slotAboutPreProfile()
{
	if(!PreProfile->currentItem()) return;
	QPreProfileItem* f=dynamic_cast<QPreProfileItem*>(PreProfile->currentItem());
	if(!f) return;
	f->Fac->About();
}

void QPlugins::slotAboutPostProfile()
{
	if(!PostProfile->currentItem()) return;
	QPostProfileItem* f=dynamic_cast<QPostProfileItem*>(PostProfile->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigPreProfile()
{
	if(!PreProfile->currentItem()) return;
	QPreProfileItem* f=dynamic_cast<QPreProfileItem*>(PreProfile->currentItem());
	if(!f) return;
	f->Fac->Configure();
}

void QPlugins::slotConfigPostProfile()
{
	if(!PostProfile->currentItem()) return;
	QPostProfileItem* f=dynamic_cast<QPostProfileItem*>(PostProfile->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotPreProfileEnable( bool state )
{
	if(!PreProfile->currentItem()) return;
	QPreProfileItem* f=dynamic_cast<QPreProfileItem*>(PreProfile->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotPostProfileEnable( bool state )
{
	if(!PostProfile->currentItem()) return;
	QPostProfileItem* f=dynamic_cast<QPostProfileItem*>(PostProfile->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::changeLang(QListViewItem* item)
{
	if(!item) return;
	QLangItem* f=dynamic_cast<QLangItem*>(item);
	if(!f) return;
	EnableLang->setChecked(f->Enable);
	ConfigLang->setEnabled(f->Fac->HasConfigure());
	AboutLang->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutLang()
{
	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigLang()
{
	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotLangEnable( bool state )
{
	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::changeDocAnalyse(QListViewItem* item)
{
	if(!item) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(item);
	if(!f) return;
	EnableDocAnalyse->setChecked(f->Enable);
	ConfigDocAnalyse->setEnabled(f->Fac->HasConfigure());
	AboutDocAnalyse->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutDocAnalyse()
{
	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::slotConfigDocAnalyse()
{
	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotDocAnalyseEnable( bool state )
{
	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotPostCommunityEnable( bool state )
{
	if(!PostCommunity->currentItem()) return;
	QPostCommunityItem* f=dynamic_cast<QPostCommunityItem*>(PostCommunity->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotConfigPostCommunity()
{
	if(!PostCommunity->currentItem()) return;
	QPostCommunityItem* f=dynamic_cast<QPostCommunityItem*>(PostCommunity->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotAboutPostCommunity()
{
	if(!PostCommunity->currentItem()) return;
	QPostCommunityItem* f=dynamic_cast<QPostCommunityItem*>(PostCommunity->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::changePostCommunity(QListViewItem * item)
{
    if(!item) return;
	QPostCommunityItem* f=dynamic_cast<QPostCommunityItem*>(item);
	if(!f) return;
	EnablePostCommunity->setChecked(f->Enable);
	ConfigPostCommunity->setEnabled(f->Fac->HasConfigure());
	AboutPostCommunity->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotPostTopicEnable( bool state )
{
	if(!PostTopic->currentItem()) return;
	QPostTopicItem* f=dynamic_cast<QPostTopicItem*>(PostTopic->currentItem());
	if(!f) return;
	f->Enable=state;
}


void QPlugins::slotConfigPostTopic()
{
	if(!PostTopic->currentItem()) return;
	QPostTopicItem* f=dynamic_cast<QPostTopicItem*>(PostTopic->currentItem());
	if(!f) return;
	f->Fac->Configure();
}


void QPlugins::slotAboutPostTopic()
{
	if(!PostTopic->currentItem()) return;
	QPostTopicItem* f=dynamic_cast<QPostTopicItem*>(PostTopic->currentItem());
	if(!f) return;
	f->Fac->About();
}


void QPlugins::changePostTopic(QListViewItem * item)
{
	if(!item) return;
	QPostTopicItem* f=dynamic_cast<QPostTopicItem*>(item);
	if(!f) return;
	EnablePostTopic->setChecked(f->Enable);
	ConfigPostTopic->setEnabled(f->Fac->HasConfigure());
	AboutPostTopic->setEnabled(f->Fac->HasAbout());
}


void QPlugins::postDocMoveDown( void )
{
	QListViewItem* f=PostDocs->currentItem();
	if(!f) return;
	if(f->itemBelow())
		f->moveItem(f->itemBelow());
}


void QPlugins::postDocMoveUp( void )
{
	QListViewItem* f=PostDocs->currentItem();
	if(!f) return;
	if(f->itemAbove() && f->itemAbove()->itemAbove())
	{
		f->moveItem(f->itemAbove()->itemAbove());
	}
	else
	{
		PostDocs->takeItem(f);
		PostDocs->insertItem(f);
		PostDocs->setSelected(f,true);
	}
}


void QPlugins::preProfileMoveDown( void )
{
	QListViewItem* f=PreProfile->currentItem();
	if (!f) return;
	if(f->itemBelow())
		f->moveItem(f->itemBelow());
}


void QPlugins::preProfileMoveUp( void )
{
	QListViewItem* f=PreProfile->currentItem();
	if(!f) return;
	if(f->itemAbove() && f->itemAbove()->itemAbove())
	{
		f->moveItem(f->itemAbove()->itemAbove());
	}
	else
	{
		PreProfile->takeItem(f);
		PreProfile->insertItem(f);
		PreProfile->setSelected(f,true);
	}
}


void QPlugins::postProfileMoveDown( void )
{
	QListViewItem* f=PostProfile->currentItem();
	if(!f) return;
	if(f->itemBelow())
	{
		f->moveItem(f->itemBelow());
	}
}


void QPlugins::postProfileMoveUp( void )
{
	QListViewItem* f=PostProfile->currentItem();
	if(!f) return;
	if(f->itemAbove() && f->itemAbove()->itemAbove())
	{
		f->moveItem(f->itemAbove()->itemAbove());
	}
	else
	{
		PostProfile->takeItem(f);
		PostProfile->insertItem(f);
		PostProfile->setSelected(f,true);
	}
}


void QPlugins::postCommunityMoveDown( void )
{
	QListViewItem* f=PostCommunity->currentItem();
	if(!f) return;
	if(f->itemBelow())
		f->moveItem(f->itemBelow());
}


void QPlugins::postCommunityMoveUp( void )
{
	QListViewItem* f=PostCommunity->currentItem();
	if(!f) return;
	if(f->itemAbove() && f->itemAbove()->itemAbove())
	{
		f->moveItem(f->itemAbove()->itemAbove());
	}
	else
	{
		PostCommunity->takeItem(f);
		PostCommunity->insertItem(f);
		PostCommunity->setSelected(f,true);
	}
}


void QPlugins::postTopicMoveDown( void )
{
	QListViewItem* f=PostTopic->currentItem();
	if(!f) return;
	if(f->itemBelow())
		f->moveItem(f->itemBelow());
}


void QPlugins::postTopicMoveUp( void )
{
	QListViewItem* f=PostTopic->currentItem();
	if(!f) return;
	if(f->itemAbove() && f->itemAbove()->itemAbove())
	{
		f->moveItem(f->itemAbove()->itemAbove());
	}
	else
	{
		PostTopic->takeItem(f);
		PostTopic->insertItem(f);
		PostTopic->setSelected(f,true);
	}
}

void QPlugins::updateLevels(void)
{
	unsigned int level;

	QListViewItemIterator it(PostCommunity);
	for(level=0;it.current();++it,level++)
	{
		QPostCommunityItem* f=dynamic_cast<QPostCommunityItem*>(it.current());
		f->Fac->SetUInt("Level",level);
	}

	QListViewItemIterator it2(PostDocs);
	for(level=0; it2.current();++it2, level++)
	{
		QPostDocItem* f=dynamic_cast<QPostDocItem*>(it2.current());
		f->Fac->SetUInt("Level",level);
	}

	QListViewItemIterator it3(PreProfile);
	for(level=0; it3.current();++it3, level++)
	{
		QPreProfileItem* f=dynamic_cast<QPreProfileItem*>(it3.current());
		f->Fac->SetUInt("Level",level);
	}

	QListViewItemIterator it4(PostProfile);
	for(level=0; it4.current();++it4, level++)
	{
		QPostProfileItem* f=dynamic_cast<QPostProfileItem*>(it4.current());
		f->Fac->SetUInt("Level",level);
	}
}


void QPlugins::changeEngine(QListViewItem* item)
{
 if(!item) return;
 QEngineItem* f=dynamic_cast<QEngineItem*>(item);
 if(!f) return;
 EnableEngine->setChecked(f->Enable);
 ConfigEngine->setEnabled(f->Fac->HasConfigure());
 AboutEngine->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutEngine()
{
 if(!Engines->currentItem()) return;
 QEngineItem* f=dynamic_cast<QEngineItem*>(Engines->currentItem());
 if(!f) return;
 f->Fac->About();
}


void QPlugins::slotConfigEngine()
{
 if(!Engines->currentItem()) return;
 QEngineItem* f=dynamic_cast<QEngineItem*>(Engines->currentItem());
 if(!f) return;
 f->Fac->Configure();
}


void QPlugins::slotEngineEnable( bool state )
{
 if(!Engines->currentItem()) return;
 QEngineItem* f=dynamic_cast<QEngineItem*>(Engines->currentItem());
 if(!f) return;
 f->Enable=state;
}


void QPlugins::changeStorage( QListViewItem * item )
{
 if(!item) return;
 QStorageItem* f=dynamic_cast<QStorageItem*>(item);
 if(!f) return;
 ConfigStorage->setEnabled(f->Fac->HasConfigure());
 AboutStorage->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutStorage()
{
 if(!Storages->currentItem()) return;
 QStorageItem* f=dynamic_cast<QStorageItem*>(Storages->currentItem());
 if(!f) return;
 f->Fac->About();
}


void QPlugins::slotConfigStorage()
{
 if(!Storages->currentItem()) return;
 QStorageItem* f=dynamic_cast<QStorageItem*>(Storages->currentItem());
 if(!f) return;
 f->Fac->Configure();
}
