
/*

	GALILEI Research Project

	QGDocXML.h

	Widget to show a XML structure of a document - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef QGDocXMLH
#define QGDocXMLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qsplitter.h>
#include <qtabwidget.h>
#include <qmultilineedit.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The QGDocXML class provides a representation of a XML structure corresponding
* to a document content.
* @author Pascal Francq and Julien Lamoral
* @short QDocXML's Widget.
*/
class QGDocXML : public QSplitter
{
	Q_OBJECT

	/**
	* XML structure.
	*/
	GDocXML* DocXML;

	/**
	* Tree Used to show the structure.
	*/
	QListView* Struct;

	/**
	* Different infos about a tag.
	*/
	QTabWidget* TagInfos;

	/**
	* Eventually, the content.
	*/
	QMultiLineEdit* TagContent;

	/**
	* Eventually, the content.
	*/
	QListView* TagAttributes;

public:

	/**
	* Constructor.
	* @param parent         Parent of the widget.
	* @param docxml         The structure to handle.
	*/
	QGDocXML(QWidget* parent,GDocXML* docxml=0);

protected:

	/**
	* Construct a tag in the list view.
	* @param t              Tag to construct.
	* @param parent         Parent that holds the tag.
	*/
	void ConstructTag(R::RXMLTag* t,QListViewItem* parent);

protected slots:

	/**
	* Slot to called when another tag is choosen in the list view.
	* @param item           Item selected.
	*/
	void slotSelectionTag(QListViewItem* item);

public:

	/**
	* Assign a new DocXML to the widget and redraw it.
	* @param docxml         The structure to handle.
	*/
	void SetDocXML(GDocXML* docxml);

	/**
	* Destructor.
	*/
	~QGDocXML(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
