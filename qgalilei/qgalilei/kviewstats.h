	/*

	GALILEI Research Project

	KViewStats.h

	Window for the running some instruction to the session - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef KViewStatsH
#define KViewStatsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmltag.h>
#include <rconfig.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kviewstats.h>


//-----------------------------------------------------------------------------
/**
* The KViewStats class provides the view widget for running some instructions on
* a session.
* @author Pascal Francq.
* @short Statistics Window for Instructions.
*/
class KViewStats : public QMdiSubWindow, public Ui_KViewStats
{
	Q_OBJECT

public:

	/**
	* Constructor for the view.
	*/
	KViewStats(void);

	/**
	 * Compute the statistics.
	 */
	void ComputeStats(void);

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

public slots:

	/**
	 * Slot called each time a tab is changed.
	 * @param index          Index of the tab.
	 */
	void tabChanged(int index);

	/**
	 * The sorting check bow was checked.
	 * @param state          State.
	 */
	void checkSorting(int state);

public:

	/**
	* Destruct the view.
	*/
	~KViewStats(void);
};


//-----------------------------------------------------------------------------
#endif
