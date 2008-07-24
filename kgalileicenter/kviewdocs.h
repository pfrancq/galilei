/*

	GALILEI Research Project

	KViewDocs.h

	Window to show all the documents - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef KViewDocsH
#define KViewDocsH


//-----------------------------------------------------------------------------
// foward declaration for GALILEI
namespace GALILEI
{
	class GDoc;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
// foward class declaration
class QListView;


//-----------------------------------------------------------------------------
/**
* The KViewDocs class provides a window to list all the documents
* treated by the system and classify by their language.
* @auhtor Pascal Francq and David Wartel
* @short Documents Window.
*/
class KViewDocs : public KView
{
	Q_OBJECT

private:

	/**
	* Listview (tree view) of documents.
	*/
	QListView* Docs;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewDocs(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the current document selected in this window.
	* @returns Pointer to GDoc or 0 if no document is currently selected.
	*/
	GDoc* GetCurrentDoc(void);

	/**
	* Creates the listview of documents
	*/
	void CreateDocsListView(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewDocs(void);
};


//-----------------------------------------------------------------------------
#endif
