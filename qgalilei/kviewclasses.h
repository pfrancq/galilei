/*

	GALILEI Research Project

	KViewClasses.h

	Window for manipulating the classes - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef KViewClassesH
#define KViewClassesH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gclass.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
//include files for current application
#include <ui_kviewclasses.h>
class QGALILEIWin;  // forward declaration


//-----------------------------------------------------------------------------
/**
* The KViewClasses class provides a window to manipulate the classes of the
* system.
* @author Pascal Francq.
* @short Classes Window.
*/
class KViewClasses : public QMdiSubWindow, public Ui_KViewClasses
{
	class Item;

	Q_OBJECT

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

	/**
	 * All the descriptor must be shown.
	 */
	bool AllDescriptors;

public:

	/**
	* Construct the widget.
	*/
	KViewClasses(QGALILEIWin* win);
	
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

protected:

	/**
	 * Build the description of a class.
	 * @param c              Class.
	 * @return QString containing the concepts.
	 */
	QString buildDesc(GClass* c);

	/**
	 * Build a class.
	 * @param c              The class.
	 * @param parent         Parent item.
	 */
	void buildClass(GClass* c,Item* parent);

public slots:

	/**
	 * Update the list.
	 */
	void update(void);

protected slots:

	/**
	 * Change the details level.
	 * @param state          State of the check button (checked implies full
	 *                       details).
	 */
	void changeDetailsLevel(int state);

public:

	/**
	* Destruct the widget.
	*/
	~KViewClasses(void);
};


//-----------------------------------------------------------------------------
#endif
