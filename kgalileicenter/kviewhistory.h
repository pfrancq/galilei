/*

	GALILEI Research Project

	KViewHistory.h

	Window to display history of groups - Header.

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
#ifndef KViewHistoryH
#define KViewHistoryH


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
#include <infos/giwordsweightshistory.h>
#include <groups/ggroupshistory.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>
#include <galilei/qggroupshistory.h>

//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewHistory class provides the view widget to display the evolution
* of groups.
* @author Pascal Francq.
* @version $Revision$
* @short Window for GA.
*/
class KViewHistory : public KView
{
	Q_OBJECT

	/**
	*  global smilarities, or not.
	*/
	bool Global;

	/**
	*  lang .
	*/
	GLang* Lang;

	/**
	* the first historic groups
	*/
	 unsigned int MinGen;

	/**
	* the last historic groups
	*/
	 unsigned int MaxGen;

	/**
	* The tab widget.
	*/
	QTabWidget* TabWidget;

	/**
	* The solutions.
	*/
	QGGroupsHistory* Solution;

	/**
	* Listview to display the similarities.
	*/
	QListView* SimsView;
	
	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The container of subprofiles
	*/
	RStd::RContainer<GSubProfile, unsigned int, false,true>* SubProfiles;

	/**
	* The container of groups
	*/
	RStd::RContainer<GGroupsHistory, unsigned int, false,true>* Groups;

	/**
	* selected items from Solutions.
	*/
	RStd::RContainer<GIWordsWeightsHistory, unsigned int, false, true>* SelectedItems;

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
	* @param minid          lowest limit of historical groups.
	* @param maxid          highest limit of historical groups.
	*/
	KViewHistory(KDoc* doc,const char* l, bool global,QWidget* parent,const char* name,int wflags, unsigned int minid, unsigned int maxid);

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

private slots:

	/**
	* function to udate simsview when the selected set has changed in tabview.
	*/
	 void slotSelectedSetChanged(QListViewItem* item);
	 
public:


	/**
	* Update the SimsView listview..
	*/
	void DisplaySimilarities(void);

	/**
	* check if a group has changed since last historic.
	*/
	void CheckModifiedGroups(GGroupsHistory* grps);

	/**
	* Set the subject of the groups (dominant subject)
	*/
	void SetGroupsSubject(GGroupsHistory* grps);

	/**
	* check if the subprofiles are well-grouped
	*/
	void CheckWellGroupedSubProfiles(GGroupsHistory* grps);

	/**
	* Destructor for the view.
	*/
	virtual ~KViewHistory(void);
	
};


//-----------------------------------------------------------------------------
#endif
