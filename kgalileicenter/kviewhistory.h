
/*

	GALILEI Research Project

	KViewHistory.h

	Window to display history of groups - Header.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#ifndef KViewHistoryH
#define KViewHistoryH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromcriterion.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <historic/gweightinfoshistory.h>
#include <historic/ggroupshistory.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QtWidgets
#include <frontend/kde/qggroupshistory.h>
#include <frontend/kde/qlistviewitemtype.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>
#include <qmenubar.h>

//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewHistory class provides the view widget to display the evolution
* of groups.
* @author Pascal Francq.
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
	* the first historic groups
	*/
	 unsigned int MinGen;

	/**
	* the last historic groups
	*/
	 unsigned int MaxGen;

	/**
	* the first historic groups
	*/
	 R::RString MinDate;

	/**
	* the last historic groups
	*/
	 R::RString MaxDate;

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
	* Listview to display the groups relationship.
	*/
	QListView* RelationShip;

	/**
	* Toolbar of theview.
	*/
	QMenuBar* ToolBar;
	
	/**
	* Identificator of the current showed solution.
	*/
	unsigned int CurId;

	/**
	* The container of subprofiles
	*/
	R::RContainer<GSubProfile,false,true>* SubProfiles;

	/**
	* The container of groups
	*/
	GGroupsHistoryManager* Groups;

	/**
	* selected items from Solutions.
	*/
	R::RContainer<GWeightInfosHistory,false,true>* SelectedSubProfiles;

	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSims* Sims;

	/**
	* historic is loaded by date if true, else by Id.
	*/
	bool bDate;


public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param p              Parameters.
	* @param global         Global Similarities.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	* @param minid          lowest limit of historical groups id.
	* @param maxid          highest limit of historical groups id.
	* @param mindate          lowest date limit of historical groups.
	* @param maxdate          highest date limit of historical groups.
	* @param checkModifiedGroups check for modified groups.
	* @param checkNewProfiles check for new profiles.
	* @param ckeckWellGroupedProfiles ckec for well-grouped profiles.
	*/
	KViewHistory(KDoc* doc, bool global,QWidget* parent,const char* name,int wflags, unsigned int minid, unsigned int maxid,const char* mindate, const char* maxdate, bool bdate);

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
	* function to update simsview when the selected set has changed in tabview.
	*/
	 void slotSelectedSetChanged(QListViewItem* item);

	/**
	* function to update simsview when the selected set has changed in tabview.
	*/
	 void slotCheckModifiedGroups(void) ;

	/**
	* function to update simsview when the selected set has changed in tabview.
	*/
	 void slotCheckWellGroupedSubProfs(void);

	/**
	* function to update simsview when the selected set has changed in tabview.
	*/
	 void slotCheckNewProfiles(void);

	/**
	* function to show similarities
	*/
	 void slotViewSimilarities(void);

	/**
	*  funtion to show relationship.
	*/
	void slotViewRelationShip(void);

	/**
	* function to show similarities
	*/
	 void slotClearSimilarities(void) {SimsView->clear(); SelectedSubProfiles->Clear();};

	/**
	*  funtion to show relationship.
	*/
	void slotClearRelationShip(void) {RelationShip->clear();};
 
public:

	/**
	* Update the SimsView listview..
	*/
	void DisplaySimilarities(void);

	/**
	* Update the SimsView listview..
	*/
	void DisplayRelationShip(GGroupHistory* grpitem);

	/**
	* Update the SimsView listview..
	*/
	void DisplayChildrenRelationShip(GGroupHistory* grpitem,QListViewItemType* attach);

	/**
	* Destructor for the view.
	*/
	virtual ~KViewHistory(void);
	
};


//-----------------------------------------------------------------------------
#endif
