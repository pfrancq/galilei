/*

	GALILEI Research Project

	KViewUsers.h

	Window for manipulating the users - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef KViewUsersH
#define KViewUsersH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei/qlistviewitemtype.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//-----------------------------------------------------------------------------
//include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewUsers class provides a window to manipulate the users of the system.
* @author Pascal Francq.
* @version $Revision$
* @short Users' Window.
*/
class KViewUsers : public KView
{
	Q_OBJECT

	/**
	* Listview  of users.
	*/
	QListView* Users;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param wflags         Flags.
	*/
	KViewUsers(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gUsers);}

	/**
	* Get the current user selected in this window.
	* @returns Pointer to GUser or 0 if no user is currently selected.
	*/
	GUser* GetCurrentUser(void);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

	/**
	* Create the list view with all the users.
	*/
	void CreateUsersListView(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewUsers(void);
};


//-----------------------------------------------------------------------------
#endif
