/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <knuminput.h>
#include <iostream>

void DlgConfig_Qt::ChangeGeneralMode(int m)
{
    //Use single query
    if(m==1)
    {
	CombinaisonOptions->setEnabled(false);
    }
    //Use combinaison of query
    if(m==2) 
    {
	CombinaisonOptions->setEnabled(true);
	PUpperPCLabel->setEnabled(false);
	PUpperPC->setEnabled(false);
	PUpperAbsLabel->setEnabled(false);
	PUpperAbs->setEnabled(false);
    }
    //Use Ranch combinaison of query
    if(m==0)
    {
	CombinaisonOptions->setEnabled(true);
	if(CombinaisonOptions->selectedId()==1)
	{
		PUpperPCLabel->setEnabled(true);
		PUpperPC->setEnabled(true);
	}
	else
	{
		PUpperAbsLabel->setEnabled(true);
		PUpperAbs->setEnabled(true);
	}
    }
}

void DlgConfig_Qt::ChangeOptionMode(int m)
{
    if(m==1)
    {
	//set percentage values enabled
	PLowerPCLabel->setEnabled(true);
	PLowerPC->setEnabled(true);
	if(QueryModeOption->selectedId()==0)
	{
	    PUpperPCLabel->setEnabled(true);
	    PUpperPC->setEnabled(true);
	}
	else
	{
	    PUpperPCLabel->setEnabled(false);
	    PUpperPC->setEnabled(false);
	}
	//set absolute value enabled
	PLowerAbsLabel->setEnabled(false);
	PLowerAbs->setEnabled(false);
	PUpperAbsLabel->setEnabled(false);
	PUpperAbs->setEnabled(false);
    }
    if(m==0)
    {
	//set percentage values enabled
	PLowerPCLabel->setEnabled(false);
	PLowerPC->setEnabled(false);
	PUpperPCLabel->setEnabled(false);
	PUpperPC->setEnabled(false);
	
	//set absolute value enabled
	PLowerAbsLabel->setEnabled(true);
	PLowerAbs->setEnabled(true);
	if(QueryModeOption->selectedId()==0)
	{
	    PUpperAbsLabel->setEnabled(true);
	    PUpperAbs->setEnabled(true);
	}
	else
	{
	    PUpperAbsLabel->setEnabled(false);
	    PUpperAbs->setEnabled(false);
	}
	
    }
}


void DlgConfig_Qt::LimitActivatedChanged( int s )
{
    if(s==QButton::On)
    {
	MaxResNb->setEnabled(true);
    }
    if(s==QButton::Off)
    {
	MaxResNb->setEnabled(false);
    }
 //   if(s==Qbutton:NoChange)
}

