/*

	GALILEI Research Project

	KDoc.cpp

	Document representing a GALILEI session - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <profiles/gprofilecalcvector.h>
#include <groups/ggroupingsim.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>
#include <kaction.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kio/job.h>
#include <kio/netaccess.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdoc.h"
#include "kview.h"
#include "kviewdocs.h"
#include "kviewusers.h"
#include "kviewgroups.h"
#include "kgalileicenter.h"



//-----------------------------------------------------------------------------
//
// KDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KDoc::KDoc(KGALILEICenterApp* owner,GSession* session)
	: QObject(), Session(session), Stat(0), Sim(0), WinDocs(0),
	  WinUsers(0), WinGroups(0), Owner(owner)
{
	pViewList = new QList<KView>;
	pViewList->setAutoDelete(false);
	Stat=new GProfileCalcVector(session,25);
	Sim=new GGroupingSim(session);
}


//-----------------------------------------------------------------------------
GProfileCalc* KDoc::GetCurComputeProfile(void)
{
	return(Stat);
}


//-----------------------------------------------------------------------------
GGrouping* KDoc::GetCurGrouping(void)
{
	return(Sim);
}


//-----------------------------------------------------------------------------
void KDoc::addView(KView* view)
{
	pViewList->append(view);
	switch(view->getType())
	{
		case gUsers:
			WinUsers=(KViewUsers*)view;
			Owner->showUsers->setEnabled(false);
			break;
		case gDocs:
			WinDocs=(KViewDocs*)view;
			Owner->showDocs->setEnabled(false);
			break;
		case gGroups:
			WinGroups=(KViewGroups*)view;
			Owner->showGroups->setEnabled(false);
			break;
		default:
			break;
	};
	changedViewList();
}


//-----------------------------------------------------------------------------
void KDoc::removeView(KView* view)
{
	pViewList->remove(view);
	switch(view->getType())
	{
		case gUsers:
			WinUsers=0;
			Owner->showUsers->setEnabled(true);
			break;
		case gDocs:
			WinDocs=0;
			Owner->showDocs->setEnabled(true);
			break;
		case gGroups:
			WinGroups=0;
			Owner->showGroups->setEnabled(true);
			break;
		default:
			break;
	};
	if(!pViewList->isEmpty())
			changedViewList();
		else
			deleteContents();
	Owner->slotWindowActivated(0);
}


//-----------------------------------------------------------------------------
void KDoc::changedViewList(void)
{
}


//-----------------------------------------------------------------------------
bool KDoc::isLastView(void)
{
	return((int)pViewList->count()==1);
}


//-----------------------------------------------------------------------------
void KDoc::updateAllViews(unsigned int cmd)
{
	KView *w;

	for(w=pViewList->first();w!=0;w=pViewList->next())
	{
		w->update(cmd);
	}
}


//-----------------------------------------------------------------------------
void KDoc::closeDocument(void)
{
	KView *w;

	if(!isLastView())
	{
		for(w=pViewList->first();w!=0;w=pViewList->next())
		{
			if(!w->close())
 				break;
		}
	}
	if(isLastView())
	{
		w=pViewList->first();
		w->close();
	}
}


//-----------------------------------------------------------------------------
bool KDoc::newDocument(void)
{
	// TODO: Add your document initialization code here
	modified=false;
	return(true);
}


//-----------------------------------------------------------------------------
void KDoc::deleteContents(void)
{
	// TODO: Add implementation to delete the document contents
}


//-----------------------------------------------------------------------------
bool KDoc::canCloseFrame(KView* pFrame)
{
	if(!isLastView())
		return(true);
	return(true);
}


//-----------------------------------------------------------------------------
KDoc::~KDoc(void)
{
	if(Stat) delete Stat;
	if(Sim) delete Sim;
	delete pViewList;
	delete Session;
}
