/*

	GALILEI Research Project

	KViewDebug.h

	Window to manipulate debugging information - Header.

	Copyright 2007 by the Université Libre de Bruxelles.

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
#ifndef KViewDebugH
#define KViewDebugH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qlistviewitemtype.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qvariant.h>
#include <qdialog.h>


//-----------------------------------------------------------------------------
// forward declaration
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewMetaEngine class represents a window to manipulate a specific groups.
* @author Pascal Francq.
* @version $Revision: 466 $
* @short Group' Window.
*/
class KViewDebug : public KView
{
	Q_OBJECT

    QLabel* textLabel1;
    QLineEdit* Object;
    QPushButton* Run;
    QLabel* textLabel2;
    QLineEdit* Info;
    QTextEdit* Results;

    QVBoxLayout* Form1Layout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout2;
    
public:

	/**
	* Constructor for the view
	* @param grp            Corresponding group.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewDebug(KDoc* doc,QWidget* parent,const char* name,int wflags);
			
	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gNothing);}

protected slots:

    virtual void languageChange(void);
    
    virtual void run(void);
	
public:
	
	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

	virtual ~KViewDebug(void);
};


//-----------------------------------------------------------------------------
#endif
