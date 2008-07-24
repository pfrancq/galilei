/*

	GALILEI Research Project

	KViewTopics.h

	Window to manipulate the topics - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
#ifndef KViewTopicsH
#define KViewTopicsH


//-----------------------------------------------------------------------------
// forward declaration for GALILEI
namespace GALILEI
{
	class GTopic;
	class QListViewItemType;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qdragobject.h>
#include <qlistview.h>
class QLineEdit;
class QPushButton;
class QCheckBox;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"



//-----------------------------------------------------------------------------
// Drag object
class QDragDoc : public QDragObject
{
public:
	QListViewItemType* Src;

	QDragDoc(QWidget* dragSource,QListViewItem* src,const char* name=0);

	virtual const char * format(int i) const;
	virtual QByteArray encodedData(const char*) const;

	static bool canDecode(const QMimeSource* e );
};


//-----------------------------------------------------------------------------
/**
*/
class QTopics : public QListView
{
	Q_OBJECT

	int dragging;

	QDragDoc* Cur;

public:

	/**
	* Construct.
	* @param parent         Parent of the widget.
	* @param name           Name of the widget.
	*/
	QTopics(QWidget* parent,const char* name=0);


	void contentsDragEnterEvent( QDragEnterEvent *evt );
	void contentsDropEvent( QDropEvent *evt );
	void contentsMousePressEvent( QMouseEvent *evt );
	void contentsMouseMoveEvent( QMouseEvent * );


protected slots:

	void slotRightButton(QListViewItem* item,const QPoint& pt,int p);

	void slotNewTopic(void);
	void slotDelete(void);
	void slotSaveTopics(void);

public:

	/**
	* Destruct of the widget.
	*/
	~QTopics(void);
};



//-----------------------------------------------------------------------------
/**
* The KViewTopics class represents a window to manipulate the topics of the
* system.
* @author Pascal Francq and David Wartel.
* @short Topics' Window.
*/
class KViewTopics : public KView
{
	Q_OBJECT

	/**
	* List view of topics containing the the documents.
	*/
	QTopics* Topics;

	/**
	* Edit field for the text to search (if the return key is pressed, the
	* search is launched).
	*/
	QLineEdit* What;

	/**
	* Button to press to launch the search.
	*/
	QPushButton* Search;

	/**
	*/
	QCheckBox* Desc;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewTopics(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the current topic selected in this window.
	* @returns Pointer to GTopic or 0 if no topic is currently selected.
	*/
	GTopic* GetCurrentTopic(void);

	/**
	* Construct the topics widget.
	*/
	void ConstructTopics(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param type            Type.
	*/
	virtual void update(tObjType type);

protected slots:

	/**
	* Slot that search the next item containing the text of the edit field.
	*/
	void FindNext(void);

public:

	/**
	* Destruct the main view.
	*/
	~KViewTopics(void);
};


//-----------------------------------------------------------------------------
#endif
