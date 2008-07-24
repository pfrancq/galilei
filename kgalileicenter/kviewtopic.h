/*

	GALILEI Research Project

	KViewTopic.h

	Window to manipulate a specific topic - Header.

	Copyright 2008 by the Université Libre de Bruxelles.

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
#ifndef KViewTopicH
#define KViewTopicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
namespace GALILEI
{
	class GTopic;
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
* The KViewTopic class represents a window to manipulate a specific topic.
* @author Pascal Francq.
* @short Topic Window.
*/
class KViewTopic : public KView
{
	Q_OBJECT

	/**
	* Topic represented by the view.
	*/
	GALILEI::GTopic* Topic;

	/**
	* Widget to handle the different information of the topic.
	*/
	QTabWidget* Infos;

	/**
	*  Widget of documents containing in the topic.
	*/
	QListView* Docs;

	/**
	* Window to show general informations.
	*/
	QListView* General;

	/**
	* Vector representing the group.
	*/
	QListView* Vector;

public:

	/**
	* Constructor for the view
	* @param top            Corresponding topic.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewTopic(GALILEI::GTopic* top,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the topic of this window.
	* @return Pointer to a GALILEI::Topic.
	*/
	GALILEI::GTopic* GetTopic(void) {return(Topic);}

	/**
	* Construct the groups widget.
	*/
	void ConstructDescription(void);

	/**
	* Construct the documents widget.
	*/
	void ConstructDocs(void);

	/**
	* Construct the general information widget.
	*/
	void ConstructGeneral(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected slots:

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
