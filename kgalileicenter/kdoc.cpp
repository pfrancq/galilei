/*

	GALILEI Research Project

	KDoc.cpp

	Document representing a GALILEI session - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
// include files for GALILEI
#include <gsession.h>
using namespace GALILEI;
using namespace R;


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
KDoc::KDoc(KGALILEICenterApp* owner)
	: QObject(), Session(0), WinDocs(0), WinUsers(0), WinGroups(0), Owner(owner)
{
	pViewList = new QList<KView>;
	pViewList->setAutoDelete(true);
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
	pViewList->remove(view);
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
bool KDoc::canCloseFrame(KView* /*pFrame*/)
{
	KView *w;

	for(w=pViewList->first();w!=0;w=pViewList->next())
	{
		if(!w->canClose())
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
KDoc::~KDoc(void)
{
	delete pViewList;
	delete Session;
}
