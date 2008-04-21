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
//include files for Qt
#include <qlistview.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// foward class declaration
class GGroup;
class GGroupHistory;
class GUser;
class GProfile;
class GWeightInfosHistory;
class GDoc;
class GEngineDoc;
class GSubject;
class GLang;


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
	* Pointer to the element representend by the QListViewItemType.
	*/
	union
	{
		GGroup* Group;
		GUser* User;
		GProfile* Profile;
		GWeightInfosHistory* Giwwh;
		GGroupHistory* GroupHistory;
		GDoc* Doc;
		GEngineDoc* EngineDoc;
		GSubject* Subject;
		GLang* Lang;
	} Obj;


	/**
	* three level availables: 0:black,  1: blue, 2:red.
	*/
	unsigned int Level;

	/**
	* Enumeration type of the item.
	*/
	enum tType {tNothing,tGroup,tUser,tProfile,tGiwwh,tGroupHistory,tDocument,tEngineDoc,tSubject,tLang};

	/**
	* type of the item
	*/
	tType Type;

	/**
	* default constructor, attached to a listview.
	*/
	QListViewItemType(QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* default constructor, attached to a listviewitem.
	*/
	QListViewItemType(QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	*  constructor for a user, attached to a listviewitem.
	*/
	QListViewItemType(GUser* user, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a user, attached to a listview.
	*/
	QListViewItemType(GUser* user, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a group, attached to a listview.
	*/
	QListViewItemType(GGroup* group, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);
	
	/**
	* constructor for a group, attached to a listviewitem.
	*/
	QListViewItemType(GGroup* group, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a subject, attached to a listviewitem.
	*/
	QListViewItemType(GSubject* subject, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a profile, attached to a listviewitem.
	*/
	QListViewItemType(GProfile* profile, QListViewItem* parent,QString str1=QString::null,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a profile, attached to a listview.
	*/
	QListViewItemType(GProfile* profile, QListView* parent,QString str1=QString::null,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a vector of words, attached to a listview.
	*/
	QListViewItemType(GWeightInfosHistory* giwwh, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a vector of words, attached to a listviewitem.
	*/
	QListViewItemType(GWeightInfosHistory* giwwh, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a historic group, attached to a listview.
	*/
	QListViewItemType(GGroupHistory* grouphistory, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a historic group, attached to a listviewitem.
	*/
	QListViewItemType(GGroupHistory* grouphistory, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null,QString str4=QString::null);

	/**
	* constructor for a doc, attached to a listviewitem.
	*/
	QListViewItemType(GDoc* doc, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a doc, attached to a listview.
	*/
	QListViewItemType(GDoc* doc, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a enginedoc, attached to a listviewitem.
	*/
	QListViewItemType(GEngineDoc* engineDoc, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a enginedoc, attached to a listview.
	*/
	QListViewItemType(GEngineDoc* engineDoc, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a enginedoc, attached to a listviewitem.
	*/
	QListViewItemType(GLang* lang, QListViewItem* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* constructor for a enginedoc, attached to a listview.
	*/
	QListViewItemType(GLang* lang, QListView* parent,QString str1,QString str2=QString::null,QString str3=QString::null);

	/**
	* method to color the item.
	*/
	void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align);

	bool acceptDrop( const QMimeSource *mime ) const;

	/**
	* destructor
	*/
	~QListViewItemType(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
