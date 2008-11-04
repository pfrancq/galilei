/*

	GALILEI Research Project

	QListViewItemType.h

	QListview item to representing a GALILEI object - Header.

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



//---------------------------------------------------------------------------
#ifndef QListViewItemTypeH
#define QListViewItemTypeH


//---------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//---------------------------------------------------------------------------
//include files for Qt
#include <qlistview.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The QListViewItemType class provides a representation for a given GALILEI
* object that appear in a QListView.
* @author Pascal Francq and David Wartel.
* @short GALILEI objects' ListViewItem.
*/
class QListViewItemType : public QListViewItem
{
public:

	/**
	* Pointer to the element represented by the QListViewItemType.
	*/
	union
	{
		GCommunity* Community;
		GTopic* Topic;
		GUser* User;
		GProfile* Profile;
		GDoc* Doc;
		GEngineDoc* EngineDoc;
		GSubject* Subject;
		GLang* Lang;
	} Obj;


	/**
	* three level available: 0:black,  1: blue, 2:red.
	*/
	size_t Level;

	/**
	* type of the item
	*/
	tObjType Type;

	/**
	* Default constructor, attached to a listview.
	*/
	QListViewItemType(QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Default constructor, attached to a listviewitem.
	*/
	QListViewItemType(QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a user, attached to a listviewitem.
	*/
	QListViewItemType(GUser* user, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a user, attached to a listview.
	*/
	QListViewItemType(GUser* user, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a group, attached to a listview.
	*/
	QListViewItemType(GCommunity* group, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a group, attached to a listviewitem.
	*/
	QListViewItemType(GCommunity* group, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a topic, attached to a listview.
	*/
	QListViewItemType(GTopic* topic, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a topic, attached to a listviewitem.
	*/
	QListViewItemType(GTopic* topic, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Cconstructor for a subject, attached to a listviewitem.
	*/
	QListViewItemType(GSubject* subject, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a profile, attached to a listviewitem.
	*/
	QListViewItemType(GProfile* profile, QListViewItem* parent,QString str1=QString::null,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* Constructor for a profile, attached to a listview.
	*/
	QListViewItemType(GProfile* profile, QListView* parent,QString str1=QString::null,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* Constructor for a doc, attached to a listviewitem.
	*/
	QListViewItemType(GDoc* doc, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a doc, attached to a listview.
	*/
	QListViewItemType(GDoc* doc, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a enginedoc, attached to a listviewitem.
	*/
	QListViewItemType(GEngineDoc* engineDoc, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a enginedoc, attached to a listview.
	*/
	QListViewItemType(GEngineDoc* engineDoc, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a enginedoc, attached to a listviewitem.
	*/
	QListViewItemType(GLang* lang, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Constructor for a enginedoc, attached to a listview.
	*/
	QListViewItemType(GLang* lang, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* Method to color the item.
	*/
	void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align);

	/**
	 * Method called to see if a drop can be accepted.
	 * @param mime           MIME type.
	 */
	bool acceptDrop( const QMimeSource *mime ) const;

	/**
	* Destruct.
	*/
	~QListViewItemType(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
