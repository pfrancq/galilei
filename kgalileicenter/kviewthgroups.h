/*

	GALILEI Research Project

	KViewThGroups.h

	Window to manipulate theoritical groups - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef KViewThGroupsH
#define KViewThGroupsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QListView;
class QTabWidget;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"



//-----------------------------------------------------------------------------
/**
* The KViewThGroups class represents a window to manipulate the groups of the
* system.
*
* @author Pascal Francq and David Wartel.
* @short Theoritical Groups' Window.
*/
class KViewThGroups : public KView
{
	Q_OBJECT

	/**
	* Ideal Groups.
	*/
	GGroups* Groups;

	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Theoritical Groups.
	*/
	QListView* thGroups;

	/**
	* Pratical Groups.
	*/
	QListView* prGroups;

	/**
	* Delete the groups
	*/
	bool DeleteGroups;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param filename       Name of the file containing the ideal groupement.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewThGroups(KDoc* doc,const char* filename,QWidget* parent,const char* name,int wflags);

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param idealgroup     The ideal groupement.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewThGroups(KDoc* doc,GGroups* idealgroup,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gThGroups);}

	/**
	* Get the current group selected in this window.
	* @returns Pointer to GGroup or 0 if no group is currently selected.
	*/
	GGroup* GetCurrentGroup(void);

	/**
	* Construct the groups' widget.
	*/
	void ConstructThGroups(void);

	/**
	* Construct the groups' widget.
	*/
	void ConstructGroups(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Load the groups from the file.
	* @param filename       Name of the file containing the ideal groupement.
	*/
	void LoadGroups(const char* filename);

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewThGroups(void);
};


//-----------------------------------------------------------------------------
#endif
