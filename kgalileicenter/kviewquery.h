/*

	GALILEI Research Project

	KViewQuery.h

	Window for the running some instruction to the session - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef KViewQueryH
#define KViewQueryH


//-----------------------------------------------------------------------------
// include file for Galilei Project
#include <galilei.h>
using namespace GALILEI;


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
#include "galilei.h"



//-----------------------------------------------------------------------------
/**
* The KViewQuery class provides the view widget for running some query on
* a session to find group most similar whith te query.
* @author Julien Lamoral
* @version $Revision$
* @short Window for Query.
*/
class KViewQuery : public KView
{
	Q_OBJECT

	/**
	* Width of the label.
	*/
	int LabelWidth;

	/**
	* Query to execute.
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

	/**
	* The words of the query in wordweights.
	*/	
	GIWordsWeights* QueryWord;



public:

	/**
	* Constructor for the view.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewQuery(KDoc* doc,QWidget* parent,const char* name,int wflags);

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
	void slotNewQuery(void);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

	/**
	* Gets called to add a word in the giwordsweight container
	* @param word           The word who will be aded
	* @param wheight        The weight of the word
	*/
	void AddWord(char* word,bool weight);

public:

	/**
	* Destructor for the view.
	*/
	~KViewQuery(void);
};


//-----------------------------------------------------------------------------
#endif
