/*

	GALILEI Research Project

	KViewClasses.cpp

	Window for manipulating the users - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R
#include <rqt.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <ggalileiapp.h>
#include <guser.h>
#include <gclass.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QInputDialog>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewclasses.h>
#include <kgalileicenter.h>



//------------------------------------------------------------------------------
//
// class Object
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class KViewClasses::Item : public QTreeWidgetItem
{
public:
	GClass* Class;

	Item(QTreeWidgetItem* parent,GClass* c,const QString& desc)
		: QTreeWidgetItem(parent, QStringList()<<desc<<ToQString(c->GetName()+" ("+RString::Number(c->GetId())+")"))
	{
		Class=c;
		setIcon(0,KIconLoader::global()->loadIcon("text-xml",KIconLoader::Small));
	}

	Item(QTreeWidget* parent,GClass* c,const QString& desc)
		: QTreeWidgetItem(parent, QStringList()<<desc<<ToQString(c->GetName()+" ("+RString::Number(c->GetId())+")"))
	{
		Class=c;
		setIcon(0,KIconLoader::global()->loadIcon("text-xml",KIconLoader::Small));
	}
};



//-----------------------------------------------------------------------------
//
// class KViewClasses
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewClasses::KViewClasses(void)
	: QMdiSubWindow(), Ui_KViewClasses(), AllDescriptors(false)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Classes");
	connect(ShowAll,SIGNAL(stateChanged(int)),this,SLOT(changeDetailsLevel(int)));
	update();
}


//-----------------------------------------------------------------------------
QString KViewClasses::buildDesc(GClass* c)
{
	QString ret;

	RCursor<GWeightInfo> Cur(c->GetVector().GetInfos());
	bool Comma=false;
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if((!AllDescriptors)&&(c->GetParent())&&(c->GetParent()->GetVector().IsIn(Cur()->GetConcept())))
			continue;
		if(Comma)
			ret+=",";
		else
			Comma=true;
		ret+=ToQString(Cur()->GetConcept()->GetName());
	}
	return(ret);
}


//-----------------------------------------------------------------------------
void KViewClasses::buildClass(GClass* c,Item* parent)
{
	Item* item(new Item(parent,c,buildDesc(c)));
	RCursor<GClass> Cur(c->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		buildClass(Cur(),item);
}


//-----------------------------------------------------------------------------
void KViewClasses::update(void)
{
	List->clear();
	RCursor<GClass> Cur(GALILEIApp->GetSession()->GetTopClasses());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Item* item(new Item(List,Cur(),buildDesc(Cur())));
		RCursor<GClass> Cur2(Cur()->GetNodes());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			buildClass(Cur2(),item);
	}
	List->resizeColumnToContents(0);
	List->resizeColumnToContents(1);
}


//-----------------------------------------------------------------------------
void KViewClasses::changeDetailsLevel(int state)
{
	if(state==Qt::Checked)
		AllDescriptors=true;
	else
		AllDescriptors=false;
	update();
}


//-----------------------------------------------------------------------------
KViewClasses::~KViewClasses(void)
{
}
