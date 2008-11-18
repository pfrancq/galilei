/*

	GALILEI Research Project

	KViewDicts.h

	Window to show all the dictionaries - Header.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

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
// include files for R/GALILEI
#include <rcontainer.h>
#include <gconcepttype.h>
#include <grelation.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kviewdicts.h>


//-----------------------------------------------------------------------------
/**
* The KViewDicts class provides a window to list all the dictionaries.
* @author Pascal Francq
* @short Dictionaries Window.
*/
class KViewDicts : public QMdiSubWindow, public Ui_KViewDicts
{
	Q_OBJECT

	/**
	* Current concept type.
	*/
	GConceptType* CurDict;

	/**
	*/
	RContainer<GRelation,false,false> Rels;

public:

	/**
	* Constructor for the view
	*/
	KViewDicts(void);

	/**
	* Creates the list of dictionaries.
	*/
	void create(void);

private:

	/**
	 * Build a string representing a concept.
	*/
	QString BuildConcept(GConcept* concept);

protected slots:

	/**
	 * Update the window.
	 */
	void update(void);

	/**
	* A given dictionary was select.
	* @param item            Item.
	* @param col             Column.
	*/
	void selectDict(QTreeWidgetItem* item,int col);

	/**
	* a given concept was selected.
	* @param item            Item.
	* @param col             Column.
	*/
	void selectConcept(QTreeWidgetItem* item,int col);

	/**
	 * The user want to create a new concept.
	 */
	void newConcept(void);

	/**
	 * The user want to delete the current concept.
	 */
	void delConcept(void);

	/**
	 * The user want to create a new relation.
	 */
	void newRelation(void);

	/**
	 * The user want to delete the current relation.
	 */
	void delRelation(void);

public:

	/**
	* Destruct for the main view.
	*/
	~KViewDicts(void);
};


//-----------------------------------------------------------------------------
#endif
