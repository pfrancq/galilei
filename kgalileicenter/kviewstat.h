/*

	GALILEI Research Project

	KViewStat.h

	Basic Window for the application - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef KViewStatH
#define KViewStatH


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewStat class provides the view widget for statistics about a set of
* documents.
* @author Pascal Francq.
* @version $Revision$
* @short Window for Statistics.
*/
class KViewStat : public KView
{
	Q_OBJECT

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStat(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gDocsStat);}

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
	* Destructor for the view.
	*/
	~KViewStat(void);
};


//-----------------------------------------------------------------------------
#endif
