/*

	GALILEI Research Project

	KViewStats.h

	Window for the running some instruction to the session - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef KViewStatsH
#define KViewStatsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmltag.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qsplitter.h>
#include <qtabwidget.h>
#include <qmultilineedit.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewStats class provides the view widget for running some instructions on
* a session.
* @author Pascal Francq.
* @version $Revision$
* @short Statistics Window for Instructions.
*/
class KViewStats : public KView
{
	Q_OBJECT

	/**
	* Tree Used to show the results.
	*/
	QListView* Res;

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStats(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gInsts);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Construct a tag in the list view.
	* @param t              Tag to construct.
	* @param parent         Parent that holds the tag.
	*/
	void ConstructTag(RXMLTag* t,QListViewItem* parent);

	/**
	* Compute the statistics.
	*/
	void ComputeStats(void);

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the view.
	*/
	~KViewStats(void);
};


//-----------------------------------------------------------------------------
#endif
