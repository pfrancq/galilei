/*

	GALILEI Research Project

	KViewGroups.h

	Window to manipulate the groups - Header.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef KViewGroupsH
#define KViewGroupsH


//-----------------------------------------------------------------------------
// forward declaration for GALILEI
namespace GALILEI
{
	class GGroup;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QListView;
class QLineEdit;
class QPushButton;
class QCheckBox;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewGroups class represents a window to manipulate the groups of the
* system.
* @author Pascal Francq and David Wartel.
* @short Groups' Window.
*/
class KViewGroups : public KView
{
	Q_OBJECT

	/**
	*  List view of groups containing the the profile.
	*/
	QListView* Groups;

	/**
	* Edit field for the text to search (if the return key is pressed, the
	* search is launched).
	*/
	QLineEdit* What;

	/**
	* Button to press to launch the search.
	*/
	QPushButton* Search;

	/**
	*/
	QCheckBox* Desc;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewGroups(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gGroups);}

	/**
	* Get the current group selected in this window.
	* @returns Pointer to GGroup or 0 if no group is currently selected.
	*/
	GGroup* GetCurrentGroup(void);

	/**
	* Construct the groups widget.
	*/
	void ConstructGroups(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected slots:

	/**
	* Slot that search the next item containing the text of the edit field.
	*/
	void FindNext(void);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewGroups(void);
};


//-----------------------------------------------------------------------------
#endif
