/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qcheckbox.h>

#include "qgalileiitem.h"

void QPlugins::init()
{
    PostDocs->setSorting(-1);
    PostGroups->setSorting(-1);
}

void QPlugins::changeFilter(QListViewItem* item)
{
        	if(!item) return;
	QFilterItem* f=dynamic_cast<QFilterItem*>(item);
	EnableFilter->setChecked(f->Enable);
	ConfigFilter->setEnabled(f->Fac->HasConfigure());
	AboutFilter->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutFilter()
{
    	if(!Filters->currentItem()) return;
	QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigFilter()
{
        	if(!Filters->currentItem()) return;
	QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotFilterEnable( bool state )
{
        	if(!Filters->currentItem()) return;
	QFilterItem* f=dynamic_cast<QFilterItem*>(Filters->currentItem());
	f->Enable=state;
}


void QPlugins::changeProfileCalc( QListViewItem * item )
{
        	if(!item) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(item);
	EnableProfileCalc->setChecked(f->Enable);
	ConfigProfileCalc->setEnabled(f->Fac->HasConfigure());
	AboutProfileCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutProfileCalc()
{
    	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigProfileCalc()
{
        	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotProfileCalcEnable( bool state )
{
        	if(!ProfileCalcs->currentItem()) return;
	QProfileCalcItem* f=dynamic_cast<QProfileCalcItem*>(ProfileCalcs->currentItem());
	f->Enable=state;
}


void QPlugins::changeGrouping( QListViewItem * item)
{
        	if(!item) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(item);
	EnableGrouping->setChecked(f->Enable);
	ConfigGrouping->setEnabled(f->Fac->HasConfigure());
	AboutGrouping->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeLinkCalc( QListViewItem * item)
{
    if(!item) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(item);
	EnableLinkCalc->setChecked(f->Enable);
	ConfigLinkCalc->setEnabled(f->Fac->HasConfigure());
	AboutLinkCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::changeGroupCalc( QListViewItem * item )
{
       	if(!item) return;
	QGroupCalcItem* f=dynamic_cast<QGroupCalcItem*>(item);
	EnableGroupCalc->setChecked(f->Enable);
	ConfigGroupCalc->setEnabled(f->Fac->HasConfigure());
	AboutGroupCalc->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutGrouping()
{
        	if(!Groupings->currentItem()) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Fac->About();
}


void QPlugins::slotAboutLinkCalc()
{
    	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	f->Fac->About();
}


void QPlugins::slotAboutGroupCalc()
{
        	if(!GroupCalcs->currentItem()) return;
	QGroupCalcItem* f=dynamic_cast<QGroupCalcItem*>(GroupCalcs->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigGrouping()
{
        	if(!Groupings->currentItem()) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotConfigLinkCalc()
{
        	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotConfigGroupCalc()
{
        	if(!GroupCalcs->currentItem()) return;
	QGroupCalcItem* f=dynamic_cast<QGroupCalcItem*>(GroupCalcs->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotGroupingEnable( bool state )
{
        	if(!Groupings->currentItem()) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Enable=state;
}


void QPlugins::slotGroupCalcEnable( bool state )
{
        	if(!GroupCalcs->currentItem()) return;
	QGroupCalcItem* f=dynamic_cast<QGroupCalcItem*>(GroupCalcs->currentItem());
	f->Enable=state;
}


void QPlugins::slotLinkCalcEnable( bool state )
{
        	if(!LinkCalcs->currentItem()) return;
	QLinkCalcItem* f=dynamic_cast<QLinkCalcItem*>(LinkCalcs->currentItem());
	f->Enable=state;
}


void QPlugins::changeStatCalc( QListViewItem *  item)
{
       	if(!item) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(item);
	EnableStat->setChecked(f->Enable);
	ConfigStat->setEnabled(f->Fac->HasConfigure());
	AboutStat->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutStatCalc()
{
    	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigStatCalc()
{
        	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotStatCalcEnable( bool state )
{
        	if(!Stats->currentItem()) return;
	QStatsCalcItem* f=dynamic_cast<QStatsCalcItem*>(Stats->currentItem());
	f->Enable=state;
}


void QPlugins::slotPostDocEnable( bool state )
{
    if(!PostDocs->currentItem()) return;
    QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
    f->Enable=state;
}

void QPlugins::slotConfigPostDoc()
{
    if(!PostDocs->currentItem()) return;
    QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
    f->Fac->Configure();
}

void QPlugins::changePostDoc( QListViewItem * item )
{
              if(!item) return;
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(item);
	EnablePostDoc->setChecked(f->Enable);
	ConfigPostDoc->setEnabled(f->Fac->HasConfigure());
	AboutPostDoc->setEnabled(f->Fac->HasAbout());
}

void QPlugins::slotAboutPostDoc()
{
    if(!PostDocs->currentItem()) return;
    QPostDocItem* f=dynamic_cast<QPostDocItem*>(PostDocs->currentItem());
    f->Fac->About();
}


void QPlugins::changeLang(QListViewItem* item)
{
        	if(!item) return;
	QLangItem* f=dynamic_cast<QLangItem*>(item);
	EnableLang->setChecked(f->Enable);
	ConfigLang->setEnabled(f->Fac->HasConfigure());
	AboutLang->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutLang()
{
    	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigLang()
{
        	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotLangEnable( bool state )
{
        	if(!Langs->currentItem()) return;
	QLangItem* f=dynamic_cast<QLangItem*>(Langs->currentItem());
	f->Enable=state;
}

void QPlugins::changeDocAnalyse(QListViewItem* item)
{
        	if(!item) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(item);
	EnableDocAnalyse->setChecked(f->Enable);
	ConfigDocAnalyse->setEnabled(f->Fac->HasConfigure());
	AboutDocAnalyse->setEnabled(f->Fac->HasAbout());
}


void QPlugins::slotAboutDocAnalyse()
{
    	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	f->Fac->About();
}


void QPlugins::slotConfigDocAnalyse()
{
        	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotDocAnalyseEnable( bool state )
{
        	if(!DocAnalyses->currentItem()) return;
	QDocAnalyseItem* f=dynamic_cast<QDocAnalyseItem*>(DocAnalyses->currentItem());
	f->Enable=state;
}


void QPlugins::slotPostGroupEnable( bool state )
{
         	 if(!PostGroups->currentItem()) return;
	QPostGroupItem* f=dynamic_cast<QPostGroupItem*>(PostGroups->currentItem());
	f->Enable=state;
}


void QPlugins::slotConfigPostGroup()
{
                if(!PostGroups->currentItem()) return;
	QPostGroupItem* f=dynamic_cast<QPostGroupItem*>(PostGroups->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotAboutPostGroup()
{
                if(!PostGroups->currentItem()) return;
	QPostGroupItem* f=dynamic_cast<QPostGroupItem*>(PostGroups->currentItem());
	f->Fac->About();
}


void QPlugins::changePostGroup(QListViewItem * item)
{
    if(!item) return;
	QPostGroupItem* f=dynamic_cast<QPostGroupItem*>(item);
	EnablePostGroup->setChecked(f->Enable);
	ConfigPostGroup->setEnabled(f->Fac->HasConfigure());
	AboutPostGroup->setEnabled(f->Fac->HasAbout());
}


void QPlugins::postDocMoveDown( void )
{
  QListViewItem* f=PostDocs->currentItem();
  if (!f) return;
  if (f->itemBelow())
     f->moveItem(f->itemBelow());
 }

void QPlugins::postDocMoveUp( void )
{
       QListViewItem* f=PostDocs->currentItem();
    if (!f) return;
    if (f->itemAbove() && f->itemAbove()->itemAbove())
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

void QPlugins::postGroupMoveDown( void )
{
  QListViewItem* f=PostGroups->currentItem();
  if (!f) return;
  if (f->itemBelow())
     f->moveItem(f->itemBelow());
 }

void QPlugins::postGroupMoveUp( void )
{
    QListViewItem* f=PostGroups->currentItem();
    if (!f) return;
    if (f->itemAbove() && f->itemAbove()->itemAbove())
    {
	f->moveItem(f->itemAbove()->itemAbove());
    }
    else
    {
	PostGroups->takeItem(f);
	PostGroups->insertItem(f);
	PostGroups->setSelected(f,true);
    }
}

void QPlugins::updateLevels(void)
{
    QListViewItemIterator it(PostGroups);
    unsigned int level=0;
    for (level=0; it.current();++it, level++)
    {
	QPostGroupItem* f=dynamic_cast<QPostGroupItem*>(it.current());
	f->Fac->Set("Level",level);
    }
     QListViewItemIterator it2(PostDocs);
     for (level=0; it2.current();++it2, level++)
    {
	QPostDocItem* f=dynamic_cast<QPostDocItem*>(it2.current());
	f->Fac->Set("Level",level);
    } 
  }
