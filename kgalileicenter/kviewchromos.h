/*

	GALILEI Research Project

	KViewChromos.h

	Window to manipulate a set of chromosomes - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef KViewChromosH
#define KViewChromosH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/gchromoir.h>
#include <groups/ggroups.h>
#include <groups/gir.h>
#include <groups/ginstir.h>
#include <galilei/qggroupsir.h>
using namespace GALILEI;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewChromos class represents a window to manipulate a set of chromosomes
* store in the db to compare them in terms of measures.
* @author Pascal Francq.
* @version $Revision$
* @short Chromosomes Window.
*/
class KViewChromos : public KView
{
	Q_OBJECT

	/**
	* Window to show the different chromosomes.
	*/
	QListView* General;

	/**
	* Ideal Groups handled by the system.
	*/
	RContainer<GGroups,unsigned int,true,true> IdealGroups;

	/**
	* Language of the chromosomes to load.
	*/
	GLang* Lang;

	/**
	* Use global similarities or not.
	*/
	bool Global;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param l              Lang to group.
	* @param global         Global Similarities.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewChromos(KDoc* doc,const char* l,bool global,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gChromos);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

	/**
	* Construct Chromosomes.
	*/
	void ConstructChromosomes(void);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor.
	*/
	virtual ~KViewChromos(void);
};


//-----------------------------------------------------------------------------
#endif
