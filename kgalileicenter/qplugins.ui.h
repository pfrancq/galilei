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

}


void QPlugins::changeGroupCalc( QListViewItem * item )
{

}


void QPlugins::slotAboutGrouping()
{
        	if(!Groupings->currentItem()) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Fac->About();
}


void QPlugins::slotAboutLinkCalc()
{

}


void QPlugins::slotAboutGroupCalc()
{

}


void QPlugins::slotConfigGrouping()
{
        	if(!Groupings->currentItem()) return;
QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Fac->Configure();
}


void QPlugins::slotConfigLinkCalc()
{

}


void QPlugins::slotConfigGroupCalc()
{

}


void QPlugins::slotGroupingEnable( bool state )
{
        	if(!Groupings->currentItem()) return;
	QGroupingItem* f=dynamic_cast<QGroupingItem*>(Groupings->currentItem());
	f->Enable=state;
}


void QPlugins::slotGroupCalcEnable( bool state )
{

}


void QPlugins::slotLinkCalcEnable( bool state )
{

}
