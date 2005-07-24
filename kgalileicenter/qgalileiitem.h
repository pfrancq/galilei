/*

	GALILEI Research Project

	QGALILEItem.h

	Specific Item - Header.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef QGALILEIITEM_H
#define QGALILEIITEM_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocanalyse.h>
#include <gfilter.h>
#include <glinkcalc.h>
#include <gpostdoc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <ggroupcalc.h>
#include <ggrouping.h>
#include <gpostgroup.h>
#include <glang.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gprofilecalc.h>
#include <gstatscalc.h>
#include <gprofilessims.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>

class QFilterItem : public QListViewItem
{
public:
	GFactoryFilter* Fac;
	bool Enable;
	QFilterItem(QListView* lst,GFactoryFilter* fac,const char* desc, QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QProfileCalcItem : public QListViewItem
{
public:
	GFactoryProfileCalc* Fac;
	bool Enable;
	QProfileCalcItem(QListView* lst,GFactoryProfileCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QGroupingItem : public QListViewItem
{
public:
	GFactoryGrouping* Fac;
	bool Enable;
	QGroupingItem(QListView* lst,GFactoryGrouping* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QGroupCalcItem : public QListViewItem
{
public:
	GFactoryGroupCalc* Fac;
	bool Enable;
	QGroupCalcItem(QListView* lst,GFactoryGroupCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QStatsCalcItem : public QListViewItem
{
public:
	GFactoryStatsCalc* Fac;
	bool Enable;
	QStatsCalcItem(QListView* lst,GFactoryStatsCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QLinkCalcItem : public QListViewItem
{
public:
	GFactoryLinkCalc* Fac;
	bool Enable;
	QLinkCalcItem(QListView* lst,GFactoryLinkCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QPostDocItem : public QListViewItem
{
public:
	GFactoryPostDoc* Fac;
	bool Enable;
	QPostDocItem(QListView* lst,GFactoryPostDoc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QPreProfileItem : public QListViewItem
{
public:
	GFactoryPreProfile* Fac;
	bool Enable;
	QPreProfileItem(QListView* lst,GFactoryPreProfile* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QPostProfileItem : public QListViewItem
{
public:
	GFactoryPostProfile* Fac;
	bool Enable;
	QPostProfileItem(QListView* lst,GFactoryPostProfile* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};

class QLangItem : public QListViewItem
{
public:
	GFactoryLang* Fac;
	bool Enable;
	QLangItem(QListView* lst,GFactoryLang* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QDocAnalyseItem : public QListViewItem
{
public:
	GFactoryDocAnalyse* Fac;
	bool Enable;
	QDocAnalyseItem(QListView* lst,GFactoryDocAnalyse* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QPostGroupItem : public QListViewItem
{
public:
	GFactoryPostGroup* Fac;
	bool Enable;
	QPostGroupItem(QListView* lst,GFactoryPostGroup* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};

class QEngineItem : public QListViewItem
{
public:
	GFactoryEngine* Fac;
	bool Enable;
	QEngineItem(QListView* lst,GFactoryEngine* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};

class QMetaEngineItem : public QListViewItem
{
public:
	GFactoryMetaEngine* Fac;
	bool Enable;
	QMetaEngineItem(QListView* lst,GFactoryMetaEngine* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QProfilesSimsItem : public QListViewItem
{
public:
	GFactoryProfilesSims* Fac;
	bool Enable;
	QProfilesSimsItem(QListView* lst,GFactoryProfilesSims* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


//------------------------------------------------------------------------------
#endif
