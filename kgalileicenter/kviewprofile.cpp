/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
// includes files for Qt
#include <qpixmap.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <qgprofile.h>
#include <guser.h>
#include <gprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <gsession.h>
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <gweightinfo.h>
#include <gconcepttype.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewprofile.h"
#include "qsessionprogress.h"
#include "kdoc.h"
#include <qtable.h>



//-----------------------------------------------------------------------------
//
// class KViewProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewProfile::KViewProfile(GProfile* profile,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Profile(profile), Fdbks(0)
{
	// Window initialisation
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	setCaption("Profile \""+QString(ToQString(Profile->GetName()))+"\"");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QGProfile(Infos,Profile);
	Infos->insertTab(General,"General Information");

	// Initialisation of the User Widget
	User = new QListView(Infos);
	Infos->insertTab(User,"User");
	User->addColumn("Variable");
	User->addColumn("Value");
	ConstructUser();

	// Initialisation of the Groups Widget
	Groups=new QListView(Infos);
	Infos->insertTab(Groups,"Groups");
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	connect(Groups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructGroups();

	// Initialisation of the Feedbacks Widget
	Fdbks = new QListView(Infos);
	Infos->insertTab(Fdbks,"Documents");
	Fdbks->addColumn(QString("Document"));
	Fdbks->addColumn(QString("URL"));
	Fdbks->addColumn(QString("Date"));
	Fdbks->setRootIsDecorated(true);
	connect(Fdbks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructFdbks();

	// Initialisation of the Links Widget
	FdbksLinks = new QListView(Infos);
	Infos->insertTab(FdbksLinks,"Links");
	FdbksLinks->addColumn(QString("Document"));
	FdbksLinks->addColumn(QString("URL"));
	FdbksLinks->addColumn(QString("Date"));
	FdbksLinks->setRootIsDecorated(true);
	connect(FdbksLinks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructLinks();

	// Initialisation of the AnalyseResults Widget
	Results = new QListView(Infos);
	Infos->insertTab(Results,"Description");
	Results->addColumn("Concept");
	Results->addColumn("Concept Type");
	Results->addColumn("Weight");
	ConstructResults();
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructFdbks(void)
{
	QListViewItem *p;
	RDate d;
	RCursor<GFdbk> Docs;
	GDoc* doc;

	if(!Fdbks) return;

	// Init different judgements
	Fdbks->clear();
	QListViewItemType* ok= new QListViewItemType(Fdbks, "Relevant Documennts");
	ok->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "Fuzzy Relevant Documents");
	ko->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("remove.png",KIcon::Small)));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "Irrelevant Documents");
	hs->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));

	// Add Judgements from profiles.
	Docs=Profile->GetFdbks();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		switch(Docs()->GetFdbk())
		{
			case djOK:
				p=ok;
				break;
			case djKO:
				p=ko;
				break;
			case djOutScope:
				p=hs;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		doc=GSession::Get()->GetDoc(Docs()->GetDocId());
		d=doc->GetUpdated();
		QListViewItemType* prof = new QListViewItemType(doc,p,ToQString(doc->GetName()),ToQString(doc->GetURL()),ToQString(d));
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructResults(void)
{
	class LocalItem : QListViewItem
	{
	public:
		double Val;

		LocalItem(QListView* v,QString name,QString type,double d) : QListViewItem(v,name,type,QString::number(d)), Val(d) {}
		virtual int compare( QListViewItem *i, int col, bool ascending ) const
    	{
			if(col==1)
			{
				double d=Val-static_cast<LocalItem*>(i)->Val;
				if(d==0.0) return(key(0, ascending ).compare( i->key(0, ascending)));
				if(d>0)
					return(1);
				return(-1);
			}
			return(key( col, ascending ).lower().compare( i->key( col, ascending).lower()));
    	}
	};
	Results->clear();
	RCursor<GWeightInfo> Words(Profile->GetInfos());
	for (Words.Start();!Words.End();Words.Next())
	{
		QString name=ToQString(Doc->GetSession()->GetStorage()->LoadConcept(Words()->GetId(),Words()->GetType()));
		QString type=ToQString(Words()->GetType()->GetDescription());
		new LocalItem(Results,name,type,Words()->GetWeight());
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructLinks(void)
{
	QListViewItem *p;
	RDate d;
	RString iconName="";
	RCursor<GFdbk> Docs;
	GDoc* doc;

	if(!FdbksLinks) return;

	// Init different judgements for document from link analysis.
	FdbksLinks->clear();
	QListViewItemType* lh= new QListViewItemType(FdbksLinks, "Hub Documents");
	lh->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* la= new QListViewItemType(FdbksLinks, "Authority Documents");
	la->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));

	// Add Judgements from profiles.
	Docs=Profile->GetFdbks();
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		p=0;
		if(Docs()->GetFdbk()&djAutority)
		{
			p=la;
			iconName="konquerorAutho.png";
		}
		if(Docs()->GetFdbk()&djHub)
		{
			p=lh;
			iconName="konquerorHub.png";
		}
		if(!p) continue;
		doc=GSession::Get()->GetDoc(Docs()->GetDocId());
		d=doc->GetUpdated();
		QListViewItemType* prof = new QListViewItemType(doc,p,ToQString(doc->GetName()),ToQString(doc->GetURL()),ToQString(d));
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon(ToQString(iconName),KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructGroups(void)
{
	Groups->clear();
	GCommunity* gr=GSession::Get()->GetCommunity(Profile->GetGroupId(),true,true);
	if(!gr) return;
	RCursor<GProfile> Prof(gr->GetObjs());
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		QListViewItemType* subitem=new QListViewItemType(Prof(),Groups,ToQString(Profile->GetAttached()));
		subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructUser(void)
{
	const GUser* usr=Profile->GetUser();

	User->clear();
	new QListViewItem(User,"ID",ToQString(RString::Number(usr->GetId())));
	new QListViewItem(User,"Full Name",ToQString(usr->GetFullName()));
	new QListViewItem(User,"Name",ToQString(usr->GetName()));
}


//-----------------------------------------------------------------------------
void KViewProfile::update(tObjType type)
{
	switch(type)
	{
		case otProfile:
			General->slotProfileChanged();
			Results->clear();
			ConstructResults();
			break;
		case otCommunity:
			ConstructGroups();
			break;
		case otFdbk:
			ConstructLinks();
			break;
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Compute Profile");
	if(!Dlg.Run(new QComputeProfile(Profile)))
		return;
	General->slotProfileChanged();
	Results->clear();
	ConstructResults();
}


//-----------------------------------------------------------------------------
KViewProfile::~KViewProfile(void)
{
}
