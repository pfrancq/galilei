/*

	GALILEI Research Project

	KViewPrg.h

	Window to run a program - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#include <sessions/galilei.h>
#include <sessions/gslot.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// forward declaration
class QMultiLineEdit;


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
	Q_OBJECT

	/**
	* Widget to see the output.
	*/
	QMultiLineEdit* Output;

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the program.
	* @param wflags         Flags.
	*/
	KViewPrg(KDoc* doc,QWidget* parent,const char* name,int wflags) throw(std::bad_alloc,RException);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gPrg);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc) throw(std::bad_alloc,RException);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof) throw(std::bad_alloc,RException);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang) throw(std::bad_alloc,RException);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const char* str) throw(std::bad_alloc,RException);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the view.
	*/
	~KViewPrg(void);
};


//-----------------------------------------------------------------------------
#endif
