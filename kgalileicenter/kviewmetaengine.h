/*

	GALILEI Research Project

	KViewMetaEngine.h

	Window to manipulate a specific group - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Valery Vandaele(vavdaele@ulb.ac.be).

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
#ifndef KViewMetaEngineH
#define KViewMetaEngineH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <frontend/kde/qlistviewitemtype.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewMetaEngine class represents a window to manipulate a specific groups.
* @author Pascal Francq.
* @version $Revision$
* @short Group' Window.
*/
class KViewMetaEngine : public KView
{
	Q_OBJECT

	/**
	* Listview  of users.
	*/
	QListView* Results;

	/**
	* The number of results to display.
	*/
	KIntSpinBox* NbRes;
	
	/**
	* LineEdit to enter the query
	*/
	QLineEdit* TxtQuery;
	
	/**
	* The button to initiate the search
	*/
	QPushButton* Search;
	
	/**
	* Label to display the number of results
	*/
	QLabel* ResLabel;


public:

	/**
	* Constructor for the view
	* @param grp            Corresponding group.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewMetaEngine(KDoc* doc,QWidget* parent,const char* name,int wflags);
			
	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gMetaEngine);}

	/**
	* Get the current Engine doc selected in this window.
	* @returns Pointer to GEngineDoc or 0 if none is currently selected.
	*/
	GEngineDoc* GetCurrentEngineDoc(void);

protected slots:
	/**
	* Preocess the query and display results
	*/
	void QueryEngine(void);
	
protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

	/**
	* Create the list view with all the users.
	*/
	void CreateMetaEngineResultsListView(void);
	
	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

};


//-----------------------------------------------------------------------------
#endif
