/*

	GALILEI Research Project

	KViewGroupsEvaluation.h

	Window to manipulate theoritical groups - Header.

	(C) 2001 by David Wartel

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
#ifndef KViewGroupsEvaluationH
#define KViewGroupsEvaluationH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/gevaluategroupingmethods.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QListView;
class QTabWidget;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"



//-----------------------------------------------------------------------------
/**
* The KViewGroupsEvaluation class represents a window to display the evaluation 
* of a clustering
* @author David Wartel.
* @version $Revision$
* @short Groups Evaluation Window.
*/
class KViewGroupsEvaluation : public KView
{
	Q_OBJECT

	/**
	* Evaluation Measure.
	*/
	QListView* evaluation;

private:


	/**
	* container of evaluation methods.
	*/
	GEvaluateGroupingMethods* methods;


public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	*/
	KViewGroupsEvaluation(KDoc* doc,QWidget* parent,const char* name, int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gGroupsEvaluation);}

	/**
	* registers the evaluation methods.
	*/
	void RegisterEvaluationMethods(void);

	/**
	* display the evaluations.
	*/
	void Run(void);

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
	~KViewGroupsEvaluation(void);
};


//-----------------------------------------------------------------------------
#endif
