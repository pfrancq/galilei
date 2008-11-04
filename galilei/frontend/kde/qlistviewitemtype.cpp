/*

	GALILEI Research Project

	QListViewItemType.cpp

	QListview item to representing a GALILEI object - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroup.h>
#include <guser.h>
#include <gprofile.h>
#include <gdoc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QtWidgets
#include <qlistviewitemtype.h>



//-----------------------------------------------------------------------------
//
// class QListViewItemType
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(QListView* parent,QString str1,QString str2,QString str3)
		: QListViewItem(parent,str1,str2,str3) , Type(otNoClass)
{
	Obj.User=0;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3) , Type(otNoClass)
{
	Obj.User=0;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GUser* user, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3) , Type(otUser)
{
	Obj.User=user;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GUser* user, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3) , Type(otUser)
{
	Obj.User=user;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GCommunity* group, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otCommunity)
{
	Obj.Community=group;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GCommunity* group, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otCommunity)
{
	Obj.Community=group;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GTopic* topic, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otTopic)
{
	Obj.Topic=topic;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GTopic* topic, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otTopic)
{
	Obj.Topic=topic;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GSubject* subject, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otSubject)
{
	Obj.Subject=subject;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GProfile* profile, QListViewItem* parent,QString str1,QString str2,QString str3,QString str4)
	: QListViewItem(parent,ToQString(profile->GetName()),ToQString(profile->GetUser()->GetFullName()),str1,str2,str3,str4), Type(otProfile)
{
	Obj.Profile=profile;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GProfile* profile, QListView* parent,QString str1,QString str2,QString str3,QString str4)
	: QListViewItem(parent,ToQString(profile->GetName()),ToQString(profile->GetUser()->GetFullName()),str1,str2,str3,str4), Type(otProfile)
{
	Obj.Profile=profile;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GDoc* doc, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otDoc)
{
	Obj.Doc=doc;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GDoc* doc, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otDoc)
{
	Obj.Doc=doc;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GEngineDoc* engineDoc, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otEngineDoc)
{
	Obj.EngineDoc=engineDoc;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GEngineDoc* engineDoc, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otEngineDoc)
{
	Obj.EngineDoc=engineDoc;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GLang* lang, QListViewItem* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otLang)
{
	Obj.Lang=lang;
	Level=0;
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::QListViewItemType(GLang* lang, QListView* parent,QString str1,QString str2,QString str3)
	: QListViewItem(parent,str1,str2,str3), Type(otLang)
{
	Obj.Lang=lang;
	Level=0;
}


//-----------------------------------------------------------------------------
void GALILEI::QListViewItemType::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
	// declaration of new colorr
	QColor color;
	// En fonction du niveau d'alerte, on met du rouge, vert, bleu ...
	switch(Level)
	{
		case(0):
			color=QColor(0,0,0);  break;
		case(1):
			color=QColor(255,0,0); break;
		case(2):
			color=QColor(0,0,255); break;
		default :
			color=QColor(0,0,0);
	}
	// save the current color config
	QColorGroup _cg( cg );
	QColor oldText=_cg.text();
	// update colors
	_cg.setColor( QColorGroup::Text, color );
	// call the QLIstView paint function
	QListViewItem::paintCell( p, _cg, column, width, align );
	//reset the colors
	_cg.setColor( QColorGroup::Text, oldText );
}

//-----------------------------------------------------------------------------
bool QListViewItemType::acceptDrop( const QMimeSource *mime ) const
{
	if(mime->provides("galilei"))
		return(TRUE);
    return(FALSE);
}


//-----------------------------------------------------------------------------
GALILEI::QListViewItemType::~QListViewItemType(void)
{
}
