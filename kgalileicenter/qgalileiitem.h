/*

	GALILEI Research Project

	QGALILEItem.h

	Specific Item - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <filters/gfilter.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <sessions/gstatscalc.h>
#include <docs/glinkcalc.h>
#include <langs/glang.h>
#include <docs/gdocanalyse.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>

class QFilterItem : public QListViewItem
{
public:
	GFactoryFilter* Fac;
	bool Enable;
	QFilterItem(QListView* lst,GFactoryFilter* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QProfileCalcItem : public QListViewItem
{
public:
	GFactoryProfileCalc* Fac;
	bool Enable;
	QProfileCalcItem(QListView* lst,GFactoryProfileCalc* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QGroupingItem : public QListViewItem
{
public:
	GFactoryGrouping* Fac;
	bool Enable;
	QGroupingItem(QListView* lst,GFactoryGrouping* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QGroupCalcItem : public QListViewItem
{
public:
	GFactoryGroupCalc* Fac;
	bool Enable;
	QGroupCalcItem(QListView* lst,GFactoryGroupCalc* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QStatsCalcItem : public QListViewItem
{
public:
	GFactoryStatsCalc* Fac;
	bool Enable;
	QStatsCalcItem(QListView* lst,GFactoryStatsCalc* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QLinkCalcItem : public QListViewItem
{
public:
	GFactoryLinkCalc* Fac;
	bool Enable;
	QLinkCalcItem(QListView* lst,GFactoryLinkCalc* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QLangItem : public QListViewItem
{
public:
	GFactoryLang* Fac;
	bool Enable;
	QLangItem(QListView* lst,GFactoryLang* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


class QDocAnalyseItem : public QListViewItem
{
public:
	GFactoryDocAnalyse* Fac;
	bool Enable;
	QDocAnalyseItem(QListView* lst,GFactoryDocAnalyse* fac,const char* desc)
		: QListViewItem(lst,desc), Fac(fac), Enable(Fac->GetPlugin())
	{
	}
};


//------------------------------------------------------------------------------
#endif
