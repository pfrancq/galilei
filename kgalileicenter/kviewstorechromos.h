/*

	GALILEI Research Project

	KViewStoreChromos.h

	Window for the GA - Header.

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
#ifndef KViewStoreChromosH
#define KViewStoreChromosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rgasignals.h>
#include <rga/qgamonitor.h>
#include <rga/qxmlcontainer.h>
using namespace RGA;
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupinggga.h>
#include <galilei/qggroupsir.h>
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
* The KViewGA class provides the view widget for a genetic algorithm used for
* the groupment of the profile.
* @author Pascal Francq.
* @version $Revision$
* @short Window for GA.
*/
class KViewStoreChromos : public KView
{
	Q_OBJECT

	/**
	* The tab widget.
	*/
	QTabWidget* TabWidget;

	/**
	* The solutions.
	*/
	QGGroupsIR* Sol;

	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The GA that will be used.
	*/
	GInstIR* Instance;

	/**
	* SubProfiles that must be grouped again.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true>* SubProfiles;

	/**
	* Objects representing SubProfiles that must be grouped again.
	*/
	RGA::RObjs<GObjIR>* Objs;

	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSim* Sims;

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param l              Lang to group.
	* @param p              Parameters.
	* @param global         Global Similarities.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStoreChromos(KDoc* doc,const char* l,GIRParams* p,bool global,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gGA);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identificator.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

	/**
	* The function that handle the resize event.
	*/
	virtual void resizeEvent(QResizeEvent*);

public:

	/**
	* Destructor for the view.
	*/
	virtual ~KViewStoreChromos(void);
};


//-----------------------------------------------------------------------------
#endif
