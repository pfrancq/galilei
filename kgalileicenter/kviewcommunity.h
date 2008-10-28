/*

	GALILEI Research Project

	KViewCommunity.h

	Window to manipulate a specific group - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#ifndef KViewCommunityH
#define KViewCommunityH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
namespace GALILEI
{
	class GCommunity;
	class QGWeightInfos;
}


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewCommunity class represents a window to manipulate a specific groups.
* @author Pascal Francq.
* @short Group' Window.
*/
class KViewCommunity : public KView
{
	Q_OBJECT

	/**
	* Group represented by the view.
	*/
	GALILEI::GCommunity* Group;

	/**
	* Widget to handle the different information of the group.
	*/
	QTabWidget* Infos;

	/**
	*  Widget of profiles containing in the group.
	*/
	QListView* Profiles;

	/**
	* Window to show general informations.
	*/
	QListView* General;

	/**
	* Vector representing the group.
	*/
	QGWeightInfos* Vector;

	/**
	*  Widget of documents assessed as relevant by memebers of the group.
	*/
	QListView* Docs;

	/**
	* List of documents assessed as relevant.
	*/
	R::RContainer<GALILEI::GDoc,false,true> OkDocs;

public:

	/**
	* Constructor for the view
	* @param grp            Corresponding group.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewCommunity(GALILEI::GCommunity* grp,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the group of this window.
	* @return Pointer to a GALILEI::GCommunity.
	*/
	GALILEI::GCommunity* GetGroup(void) {return(Group);}

	/**
	* Construct the profiles widget.
	*/
	void ConstructProfiles(void);

	/**
	* Construct the general information widget.
	*/
	void ConstructGeneral(void);

	/**
	* Construct the documents' widget.
	*/
	void ConstructDocs(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected slots:

	/**
	* Local menu of documents was asked.
	*/
	void askDocsMenu(QListViewItem*,const QPoint&,int);

	/**
	* slot for the menu.
	*/
	void slotMenu(int id);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);
};


//-----------------------------------------------------------------------------
#endif
