/*

	GALILEI Research Project

	QGDocXML.cpp

	Widget to show a XML structure of a document - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
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
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// includes files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qgdocxml.h>
#include <frontend/kde/rqt.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// QListViewItemXMLTag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QListViewItemXMLTag : public QListViewItem
{
public:
	RXMLTag*  Tag;

	QListViewItemXMLTag(QListView* v,RXMLTag* t,const char* c1)
		: QListViewItem(v,c1), Tag(t) {}

	QListViewItemXMLTag(QListView* v,QListViewItem* a,RXMLTag* t,const char* c1)
		: QListViewItem(v,a,c1), Tag(t) {}

	QListViewItemXMLTag(QListViewItem* v,RXMLTag* t,const char* c1)
		: QListViewItem(v,c1), Tag(t) {}

		QListViewItemXMLTag(QListViewItem* v, QListViewItem* v2,RXMLTag* t,const char* c1)
		: QListViewItem(v,v2,c1), Tag(t) {}

};



//-----------------------------------------------------------------------------
//
// QGDocXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::QGDocXML::QGDocXML(QWidget* parent,GDocXML* docxml)
	: QSplitter(QSplitter::Horizontal,parent), DocXML(docxml)
{
	// Left Part - Structure
	Struct = new QListView(this,"DocXML Structure");
	Struct->addColumn("tag name");
	Struct->setRootIsDecorated(true);
	Struct->setSorting(0,false);
	Struct->setSorting(1,false);
	Struct->setSorting(2,false);
	connect(Struct,SIGNAL(selectionChanged(QListViewItem*)),this,SLOT(slotSelectionTag(QListViewItem*)));
	if(DocXML)
		ConstructTag(DocXML->GetTag("rdf:RDF"),new QListViewItemXMLTag(Struct,DocXML->GetTag("rdf:RDF"),"rdf:RDF"));

	// Right Part - Attributes / Content
	TagInfos=new QTabWidget(this);
	TagAttributes=new QListView(TagInfos);
	TagAttributes->addColumn("Attribute name");
	TagAttributes->addColumn("Attribute value");
		TagInfos->insertTab(TagAttributes,QIconSet(QPixmap(KGlobal::iconLoader()->loadIcon("xml_attribute.png",KIcon::Small))),"Attributes");
	TagContent=new QMultiLineEdit(TagInfos);
	TagContent->setReadOnly(true);
	TagInfos->insertTab(TagContent,QPixmap(KGlobal::iconLoader()->loadIcon("xml_contents.png",KIcon::Small)),"Contents");
}


//-----------------------------------------------------------------------------
void GALILEI::QGDocXML::ConstructTag(RXMLTag* t,QListViewItem* parent)
{
	R::RCursor<RXMLTag> Cur=t->GetXMLTagsCursor();
	QListViewItem* ptr=0,*ptr2;
	QListViewItem* prec=0;

	parent->setPixmap(0,QPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("xml_element.png",KIcon::Small))));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(!prec)
		{
			prec=ptr=new QListViewItemXMLTag(parent,Cur(),ToQString(Cur()->GetName()));
		}
		else
		{
			prec=ptr=new QListViewItemXMLTag(parent,prec,Cur(),ToQString(Cur()->GetName()));
		}
		if(!Cur()->GetContent().IsEmpty())
		{
			ptr2=new QListViewItemXMLTag(ptr,0,Cur()->GetContent());
			ptr2->setPixmap(0,QPixmap(QPixmap(KGlobal::iconLoader()->loadIcon("xml_text.png",KIcon::Small))));
		}
		ConstructTag(Cur(),ptr);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::QGDocXML::slotSelectionTag(QListViewItem* item)
{
	QListViewItemXMLTag* t=(QListViewItemXMLTag*)item;
	R::RCursor<RXMLAttr> Cur;

	if(t->Tag)
	{
		TagInfos->setTabEnabled(TagAttributes,true);
		TagAttributes->clear();
		Cur=t->Tag->GetXMLAttrCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
			new QListViewItem(TagAttributes,ToQString(Cur()->GetName()),ToQString(Cur()->GetValue()));
		TagInfos->setTabEnabled(TagContent,false);
	}
	else
	{
		TagInfos->setTabEnabled(TagContent,true);
		TagContent->setText(t->text(0));
		TagInfos->setTabEnabled(TagAttributes,false);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::QGDocXML::SetDocXML(GDocXML* docxml)
{
	Struct->clear();
	DocXML=docxml;
	if(DocXML)
		ConstructTag(DocXML->GetTag("rdf:RDF"),new QListViewItemXMLTag(Struct,DocXML->GetTag("rdf:RDF"),"rdf:RDF"));
}


//-----------------------------------------------------------------------------
GALILEI::QGDocXML::~QGDocXML(void)
{
}
