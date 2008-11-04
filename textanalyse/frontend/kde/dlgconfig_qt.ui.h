/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <knuminput.h>
#include <kurlrequester.h>


void DlgConfig_Qt::toggleStruct( bool toggle )
{
	StructIsContent->setEnabled(toggle);
	WeightStruct->setEnabled(toggle && StructIsContent->isChecked());
	ExtractValues->setEnabled(toggle);
	WeightValues->setEnabled(toggle && ExtractValues->isChecked());	
	Indexes->setEnabled(toggle);
	FullIndex->setEnabled(toggle);
}


void DlgConfig_Qt::toggleFullIndex(bool toggle )
{
	if(toggle)
	{
	    ExtractValues->setEnabled(false);
	    ExtractValues->setChecked(true);
    	    WeightValues->setEnabled(true);
	}
	else
	     ExtractValues->setEnabled(true);
}
