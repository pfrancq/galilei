/*

	GALILEI Research Project

	KViewGroups.h

	Window to manipulate the groups - Header.

	(C) 2001 by Pascal Francq and David Wartel

	Version $Revision$

	Last Modify: $Date$

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


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewGroups class represents a window to manipulate the groups of the
* system.
* @author Pascal Francq and David Wartel.
* @version $Revision$
* @short Groups' Window.
*/
class KViewGroups : public KView
{
	Q_OBJECT

	/**
	*  list view of groups containing the the profile
	*/
	QListView* Groups;

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
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewGroups(void);
};


//-----------------------------------------------------------------------------
#endif
