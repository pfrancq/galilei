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
