/*

	GALILEI Research Project

	KViewStem.h

	Basic Window for the application - Header.

	(C) 2001 by Pascal Francq

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
#ifndef KViewStemH
#define KViewStemH


//-----------------------------------------------------------------------------
// include files for GALILEI
namespace GALILEI
{
	class GLang;
};


//-----------------------------------------------------------------------------
// include files for Qt
#include <qtabwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewStat class provides the view widget for statistics about a set of
* documents.
* @author Pascal Francq.
* @version $Revision$
* @short Window for Statistics.
*/
class KViewStems : public KView
{
	class Word;
	class GrWord;

	Q_OBJECT

	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Theoritical Groups of words (Roots).
	*/
	QListView* thGroups;

	/**
	* Pratical Groups of words (Stems).
	*/
	QListView* prGroups;

	/**
	* Language to use.
	*/
	GALILEI::GLang* Lang;

public:

	/**
	* Constructor for the view.
	* @param code           Code of the language to use.
	* @param filename       Filename of the MORPH file.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStems(const char* code,const char* filename,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gStems);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Load the filename.
	* @param filename       Name of the file to load.
	*/
	void LoadFile(const char* filename);

	/**
	* Make the comparison by computing the values.
	*/
	void Compute(void);

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the view.
	*/
	~KViewStems(void);
};


//-----------------------------------------------------------------------------
#endif
