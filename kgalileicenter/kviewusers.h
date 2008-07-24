/*

	GALILEI Research Project

	KViewUsers.h

	Window for manipulating the users - Header.

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
#ifndef KViewUsersH
#define KViewUsersH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qlistviewitemtype.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QPushButton;


//-----------------------------------------------------------------------------
//include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewUsers class provides a window to manipulate the users of the system.
* @author Pascal Francq.
* @short Users' Window.
*/
class KViewUsers : public KView
{
	Q_OBJECT

	/**
	* Listview  of users.
	*/
	QListView* Users;

	/**
	* Button to press to create a new user.
	*/
	QPushButton* NewUser;

	/**
	* Button to press to modify a user.
	*/
	QPushButton* ModifyUser;

	/**
	* Button to press to create a new profile.
	*/
	QPushButton* NewProfile;

	/**
	* Button to press to modify a profile.
	*/
	QPushButton* ModifyProfile;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param wflags         Flags.
	*/
	KViewUsers(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the current user selected in this window.
	* @returns Pointer to GUser or 0 if no user is currently selected.
	*/
	GUser* GetCurrentUser(void);

protected:

	/**
	* Create the list view with all the users.
	*/
	void CreateUsersListView(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected slots:

	/**
	* Slot that adds a new user.
	*/
	void slotAddUser(void);

	/**
	* Slot that modifies a user.
	*/
	void slotModifyUser(void);

	/**
	 * Slot that add a profile.
	 */
	void slotAddProfile(void);

	/**
	 * Slot that modify a modify.
	 */
	void slotModifyProfile(void);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewUsers(void);
};


//-----------------------------------------------------------------------------
#endif
