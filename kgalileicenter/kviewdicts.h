/*

	GALILEI Research Project

	KViewDicts.h

	Window to show all the dictionnaries - Header.

	Copyright 2006 by the Université Libre de Bruxelles.

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
#ifndef KViewDictsH
#define KViewDictsH


//-----------------------------------------------------------------------------
// foward declaration for GALILEI
namespace GALILEI
{
	class GDoc;
	class GLang;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
// foward class declaration
class QListView;
class QListViewItem;


//-----------------------------------------------------------------------------
/**
* The KViewDicts class provides a window to list all the documents
* treated by the system and classify by their language.
* @auhtor Pascal Francq and David Wartel
* @short Documents Window.
*/
class KViewDicts : public KView
{
	Q_OBJECT

private:

	/**
	* Listview (tree view) of dictionnaries.
	*/
	QListView* Dicts;

	/**
	* Current dictionnaries
	*/
	QListView* Dict;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewDicts(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gDocs);}

	/**
	* Creates the listview of dictionnaries
	*/
	void CreateDictsListView(void);

protected slots:

	/**
	* Show a given dictionnary when a double-click on a type.
	* @param item            Language.
	*/
	void slotDoubleClick(QListViewItem* item);

protected:

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewDicts(void);
};


//-----------------------------------------------------------------------------
#endif
