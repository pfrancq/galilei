/*

	GALILEI Research Project

	KViewGroup.h

	Window to manipulate a specific group - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef KViewGroupH
#define KViewGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
namespace GALILEI
{
	class GGroup;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewGroup class represents a window to manipulate a specific groups.
* @author Pascal Francq.
* @version $Revision$
* @short Group' Window.
*/
class KViewGroup : public KView
{
	Q_OBJECT

	/**
	* Group represented by the view.
	*/
	GGroup* Group;

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

public:

	/**
	* Constructor for the view
	* @param grp            Corresponding group.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewGroup(GGroup* grp,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gGroup);}

	/**
	* Get the group of this window.
	* @return Pointer to a GALILEI::GGroup.
	*/
	GGroup* GetGroup(void) {return(Group);}

	/**
	* Construct the groups' widget.
	*/
	void ConstructProfiles(void);

	/**
	* Construct the general information's widget.
	*/
	void ConstructGeneral(void);

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
};


//-----------------------------------------------------------------------------
#endif
