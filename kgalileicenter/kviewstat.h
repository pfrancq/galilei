/*

	GALILEI Research Project

	KViewStat.h

	Window for the running some instruction to the session - Header.

	(C) 2002 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
#ifndef KViewStatH
#define KViewStatH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QLineEdit;
class QMultiLineEdit;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
// forward class declaration
class KStatInst;


//-----------------------------------------------------------------------------
/**
* The KViewStat class provides the view widget for running some instructions on
* a session.
* @author Pascal Francq.
* @version $Revision$
* @short Window for Instructions.
*/
class KViewStat : public KView, RContainer<KStatInst,unsigned int,true,true>
{
	Q_OBJECT

	/**
	* Width of the label.
	*/
	int LabelWidth;

	/**
	* Command to execute.
	*/
	QLineEdit* Cmd;

	/**
	* Results of the command.
	*/
	QMultiLineEdit* Result;

	/**
	* Buffer containing the information.
	*/
	char Buffer[10000];

public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStat(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gInsts);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

public slots:

	/**
	* Slot called when a new command is to execute.
	*/
	void slotNewCmd(void);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the view.
	*/
	~KViewStat(void);
};


//-----------------------------------------------------------------------------
#endif
