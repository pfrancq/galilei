/*

	GALILEI Research Project

	KView.h

	Basic Window for the application - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

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
#ifndef KViewH
#define KViewH


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>


//-----------------------------------------------------------------------------
// include files for current application
class KDoc;


//-----------------------------------------------------------------------------
// Enum declaration
/**
* Defines all the possible type of view for a document.
*/
enum GViewType{gNothing,gUsers,gDocs,gDoc,gProfile,gGroups,gGroup,gThGroups, gGroupsEvaluation,gDocsStat,gStems,gGA,gPrg,gR,gInsts,gChromos};


//-----------------------------------------------------------------------------
/**
* The KView class provides the view widget for the document
* instance connected to it and is displayed as a MDI child window in the main
* view area of the KGALILEICenterApp class instance. The KGALILEICenterApp
* class also has an eventFilter() method that gets installed on every
* KView instance to control events of the type QEvent::Close.
* The document connected to the view instance keeps a list of all view that
* represent the document contents as there can be more than one view. Views get
* created in KGALILEICenterApp::createClient() and automatically added to the
* list of views.
* The KView class inherits QWidget as a base. Another possible
* inheritance besides specialized widgets could be QMainWindow so that you can
* easily set up the main area of your view by setting another view as main
* widget (QMainWindow::setMainWidget() ).
* @author Pascal Francq.
* @short Generic Window.
*/
class KView : public QWidget
{
	Q_OBJECT

protected:

	/**
	* The document connected to the view, specified in the constructor.
	*/
	KDoc* Doc;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KView(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Returns a pointer to the document connected to the view.
	*/
	KDoc* getDocument(void) const;

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void)=0;

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd)=0;

	/**
	* contains the implementation for printing functionality and gets called by
	* KGALILEICenterApp::slotFilePrint().
	*/
	void print(QPrinter* pPrinter);

protected:

	/**
	* Overwritten QWidget::closeEvent() to catch closing views. Does nothing,
	* as the closeEvents for KGALILEICenterView's are processed by
	* KGALILEICenterApp::eventFilter(), so this overwitten closeEvent is
	* necessary and has to be empty. Don't overwrite this method !
	*/
	virtual void closeEvent(QCloseEvent* e);

public:

	/**
	* Destructor for the main view.
	*/
	virtual ~KView(void);
};


//-----------------------------------------------------------------------------
#endif
