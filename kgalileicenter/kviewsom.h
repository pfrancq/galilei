/*

	GALILEI Research Project

	KViewSom.h

	Window to display a self Organizing Map - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef KViewSomH
#define KViewSomH


//-----------------------------------------------------------------------------
// include files for R
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <knuminput.h>


//---------------------------------------------------------------------------
// include files for current application
#include "kview.h"
#include "ksommap.h"


//---------------------------------------------------------------------------
/**
* The KViewSom class represents the window to describe a specific document.
* @author David Wartel
* @version $Revision$
* @short Document Window.
*/
class KViewSom : public KView
{
	Q_OBJECT

	/**
	* listview to display info about neurons
	*/
	QListView* NeuronInfos;

	/**
	* listview to display general info about the map
	*/
	QListView* MapInfos;

	/**
	*  the map widget
	*/
	KSomMap* SomMap;

	/**
	* load map button
	*/
	QPushButton* loadMapButton;

	/**
	* show labels button
	*/
	QPushButton* showLabelsButton;

	/**
	* options button
	*/
	QPushButton* optionsButton;

	/**
	* nblabel input
	*/
	KIntNumInput* NbLabels;

	/**
	* id of the map
	*/
	unsigned int MapId;

	/**
	* is a map loaded?
	*/
	bool bMapLoaded;
	
	/**
	* name of the data ( group? documents? profiles?)
	*/
	R::RString  DataName;

public slots:

	/**
	* slot to redraw the map
	*/
	void slotDrawMap(void){SomMap->DrawMap();}

	/**
	* slot called when a neruron is clicked
	*/
	void slotClickedNeuron(int i);

	/**
	* slot called to show labels
	*/
	void slotHideShowLabels(void);

	/**
	* slot called to change map color
	*/
	void slotChooseColor(void);

	/**
	* slot called to init map
	*/
	void slotInitMap(void) {Init();}

	/**
	* slot called to init map
	*/
	void slotMapOptions(void);

public:

	/**
	* Constructor for the view
	* @param document       Document to represent.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param dataname       Name of the data
	* @param wflags         Flags.
	*/
	KViewSom(KDoc* doc,QWidget* parent,const char* name, const char* dataname, int wflags, unsigned int mapid);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gDoc);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

	/**
	* initialize the map
	*/
	void Init(void);

	/**
	* fills the map information
	* 
	*/
	void FillMapInfos();

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	virtual void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor.
	*/
	~KViewSom(void);
};


//-----------------------------------------------------------------------------
#endif
