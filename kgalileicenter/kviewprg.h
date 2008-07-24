/*

	GALILEI Research Project

	KViewPrg.h

	Window to run a program - Header.

	Copyright 2002 by the Universit�Libre de Bruxelles.

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
#ifndef KViewPrgH
#define KViewPrgH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gslot.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// forward declaration
#include <qtextedit.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewPrg class provides the view widget for running an application.
* @author Pascal Francq.
* @short Window for Programs.
*/
class KViewPrg : public KView, public GSlot
{
	class MyThread;

	Q_OBJECT

	/**
	* Widget to see the output.
	*/
	QTextEdit* Output;

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
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the program.
	* @param wflags         Flags.
	*/
	KViewPrg(KDoc* doc,QWidget* parent,RString name,int wflags);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* Method called by GCommunitying each time a new language is analysed.
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

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* The program is finished.
	*/
	void endPrg(void);

	/**
	* Look if the window can be closed.
	*/
	bool canClose(void);

	/**
	* Destructor for the view.
	*/
	~KViewPrg(void);
};


//-----------------------------------------------------------------------------
#endif
