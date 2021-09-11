/*

	GALILEI Research Project

	KViewDicts.h

	Window to show all the dictionaries - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gstatement.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kviewdicts.h>


//---------------------------------------------------------------------------
// forward declaration
class QGALILEIWin;

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
	 * Window.
	 */
	QGALILEIWin* Win;

public:

	/**
	* Constructor for the view
	*/
	KViewDicts(QGALILEIWin* win);

	/**
	* Creates the list of dictionaries.
	*/
	void create(void);

	/**
	 * Create the options in a configuration structure.
	 * @param config         Configuration structure.
    */
	static void createOptions(R::RConfig& config);

	/**
	 * Read the options of a configuration structure.
	 * @param config         Configuration structure.
	 */
	static void readOptions(R::RConfig& config);

	/**
	 * Save the options of a configuration structure.
	* @param config         Configuration structure.
	 */
	static void saveOptions(R::RConfig& config);

	/**
	 * Resize the window.
    * @param resizeEvent    Event.
    */
	virtual void resizeEvent(QResizeEvent* resizeEvent);

private:

	/**
	* Build a string representing a concept.
	* @param concept        Concept for which the statements must be build.
	*/
	QString buildConcept(GConcept* concept);

	/**
	* Build a string representing an object.
	* @param obj             Object for which the statements must be build.
	*/
	QString buildObject(GObject* obj);

	/**
	 * Build some statements for a given concept.
    * @param concept        Concept for which the statements must be build.
    * @param what           What kind of statements.
    */
	void buildStatements(GConcept* concept,tStatementElement what);

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
	* A given concept was selected.
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
	 * The user want to create a new statement.
	 */
	void newStatement(void);

	/**
	 * The user want to delete the current statement.
	 */
	void delStatement(void);

public:

	/**
	* Destruct for the main view.
	*/
	~KViewDicts(void);
};


//-----------------------------------------------------------------------------
#endif
