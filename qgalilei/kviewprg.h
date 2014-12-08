/*

	GALILEI Research Project

	KViewPrg.h

	Window to run a program - Header.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef KViewPrgH
#define KViewPrgH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gslot.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kviewprg.h>


//-----------------------------------------------------------------------------
/**
* The KViewPrg class provides the view widget for running an application.
* @author Pascal Francq.
* @short Window for Programs.
*/
class KViewPrg : public QMdiSubWindow, public Ui_KViewPrg, public GSlot
{
	class MyThread;

	Q_OBJECT

	/**
	* Name of the program.
	*/
	RString Name;

	/**
	* Thread running the program.
	*/
	MyThread* Prg;

public:

	/**
	* Constructor for the view.
	* @param name           Name of the program.
	*/
	KViewPrg(const RString& name);

	/**
	* The treatment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* Method called by GCommunitying each time a new language is analyzed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const RString& str);

	/**
	* Verify if Qt has nothing to do.
	*/
	virtual void Interact(void);

	/**
	* Run the program.
	*/
	virtual void run(void);

	/**
	* See if a thread is running.
	*/
	bool isRunning(void) {return(Prg);}

	/**
	* The program is finished.
	*/
	void endPrg(void);

	/**
	* Look if the window can be closed.
	* @param event           Close event.
	*/
	virtual void closeEvent(QCloseEvent* event);

public:

	/**
	* Destruct the view.
	*/
	~KViewPrg(void);
};


//-----------------------------------------------------------------------------
#endif
