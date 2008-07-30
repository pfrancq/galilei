/*

	GALILEI Research Project

	KViewThCommunities.h

	Window to manipulate theoretical communities - Header.

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
#ifndef KViewThCommunitiesH
#define KViewThCommunitiesH


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
* The KViewThCommunities class represents a window to manipulate the groups of the
* system.
*
* @author Pascal Francq and David Wartel.
* @short Theoretical Communities Window.
*/
class KViewThCommunities : public KView
{
	Q_OBJECT

	/**
	* Ideal Communities.
	*/
	GSubjects* Subjects;

	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Theoretical Communities.
	*/
	QListView* thCommunities;

	/**
	* Practical Communities.
	*/
	QListView* prCommunities;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param idealgroup     The ideal clustering.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewThCommunities(KDoc* doc,GSubjects* subjects,QWidget* parent,const char* name,int wflags);

	/**
	* Get the current group selected in this window.
	* @returns Pointer to GCommunity or 0 if no group is currently selected.
	*/
	GCommunity* GetCurrentGroup(void);

	/**
	* Construct the groups' widget.
	*/
	void ConstructThCommunities(void);

	/**
	* Construct the groups' widget.
	*/
	void ConstructCommunities(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewThCommunities(void);
};


//-----------------------------------------------------------------------------
#endif
