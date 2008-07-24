/*

	GALILEI Research Project

	QGALILEItem.h

	Specific Item - Header.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <gpostcommunity.h>
#include <gposttopic.h>
#include <glang.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gprofilecalc.h>
#include <gstatscalc.h>
#include <gstorage.h>
#include <gmeasure.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>

class QFilterItem : public QListViewItem
{
public:
	GFactoryFilter* Fac;
	bool Enable;
	bool WasEnable;
	QFilterItem(QListView* lst,GFactoryFilter* fac,const char* desc, QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QProfileCalcItem : public QListViewItem
{
public:
	GFactoryProfileCalc* Fac;
	bool Enable;
	bool WasEnable;
	QProfileCalcItem(QListView* lst,GFactoryProfileCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QGroupProfilesItem : public QListViewItem
{
public:
	GFactoryGroupProfiles* Fac;
	bool Enable;
	bool WasEnable;
	QGroupProfilesItem(QListView* lst,GFactoryGroupProfiles* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QGroupDocsItem : public QListViewItem
{
public:
	GFactoryGroupDocs* Fac;
	bool Enable;
	bool WasEnable;
	QGroupDocsItem(QListView* lst,GFactoryGroupDocs* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QCommunityCalcItem : public QListViewItem
{
public:
	GFactoryCommunityCalc* Fac;
	bool Enable;
	bool WasEnable;
	QCommunityCalcItem(QListView* lst,GFactoryCommunityCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QTopicCalcItem : public QListViewItem
{
public:
	GFactoryTopicCalc* Fac;
	bool Enable;
	bool WasEnable;
	QTopicCalcItem(QListView* lst,GFactoryTopicCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QStatsCalcItem : public QListViewItem
{
public:
	GFactoryStatsCalc* Fac;
	bool Enable;
	bool WasEnable;
	QStatsCalcItem(QListView* lst,GFactoryStatsCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QLinkCalcItem : public QListViewItem
{
public:
	GFactoryLinkCalc* Fac;
	bool Enable;
	bool WasEnable;
	QLinkCalcItem(QListView* lst,GFactoryLinkCalc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QPostDocItem : public QListViewItem
{
public:
	GFactoryPostDoc* Fac;
	bool Enable;
	bool WasEnable;
	QPostDocItem(QListView* lst,GFactoryPostDoc* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QPreProfileItem : public QListViewItem
{
public:
	GFactoryPreProfile* Fac;
	bool Enable;
	bool WasEnable;
	QPreProfileItem(QListView* lst,GFactoryPreProfile* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QPostProfileItem : public QListViewItem
{
public:
	GFactoryPostProfile* Fac;
	bool Enable;
	bool WasEnable;
	QPostProfileItem(QListView* lst,GFactoryPostProfile* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};

class QLangItem : public QListViewItem
{
public:
	GFactoryLang* Fac;
	bool Enable;
	bool WasEnable;
	QLangItem(QListView* lst,GFactoryLang* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QDocAnalyseItem : public QListViewItem
{
public:
	GFactoryDocAnalyse* Fac;
	bool Enable;
	bool WasEnable;
	QDocAnalyseItem(QListView* lst,GFactoryDocAnalyse* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QPostCommunityItem : public QListViewItem
{
public:
	GFactoryPostCommunity* Fac;
	bool Enable;
	bool WasEnable;
	QPostCommunityItem(QListView* lst,GFactoryPostCommunity* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QPostTopicItem : public QListViewItem
{
public:
	GFactoryPostTopic* Fac;
	bool Enable;
	bool WasEnable;
	QPostTopicItem(QListView* lst,GFactoryPostTopic* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QEngineItem : public QListViewItem
{
public:
	GFactoryEngine* Fac;
	bool Enable;
	bool WasEnable;
	QEngineItem(QListView* lst,GFactoryEngine* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};

class QMetaEngineItem : public QListViewItem
{
public:
	GFactoryMetaEngine* Fac;
	bool Enable;
	bool WasEnable;
	QMetaEngineItem(QListView* lst,GFactoryMetaEngine* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


class QStorageItem : public QListViewItem
{
public:
	GFactoryStorage* Fac;

	QStorageItem(QListView* lst,GFactoryStorage* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac)
	{
	}
};


class QMeasureItem : public QListViewItem
{
public:
	GFactoryMeasure* Fac;
	bool Enable;
	bool WasEnable;
	QMeasureItem(QListView* lst,GFactoryMeasure* fac,const char* desc,QListViewItem* after=0)
		: QListViewItem(lst, after,desc), Fac(fac), Enable(Fac->GetPlugin()), WasEnable(Fac->GetPlugin())
	{
	}
};


//------------------------------------------------------------------------------
#endif
