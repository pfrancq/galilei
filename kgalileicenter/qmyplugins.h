/*

	GALILEI Research Project

	QMyPlugins.h

	Dialog box for plugin-ins - Header.

	Copyright 2007 by the Université libre de Bruxelles.

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
#ifndef QMyPluginsH
#define QMyPluginsH

//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>

//-----------------------------------------------------------------------------
// include files for current project
#include "qplugins.h"


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>


//-----------------------------------------------------------------------------
// forward declaration of the KGALILEICenter classes
class KGALILEICenterApp;


//-----------------------------------------------------------------------------
class QMyPlugins : public QPlugins
{
	Q_OBJECT
public:

	struct Tab
	{
		RString Type;
		QListView* List;
		QString Current;

		Tab(const RString& t,QListView* w) : Type(t), List(w) {}
		int Compare(const Tab& tab) const {return(Type.Compare(tab.Type));}
		int Compare(const Tab* tab) const {return(Type.Compare(tab->Type));}
		int Compare(const RString type) const {return(Type.Compare(type));}
	};

	RContainer<Tab,true,true> Tabs;

	int CurrentIndex;
	
	QMyPlugins(KGALILEICenterApp* app,QString title);

public slots:

	void changeMeasure(QListViewItem * item );

	void slotAboutMeasure();

	void slotChangeCat(QListViewItem*);
	void slotConfigMeasure();
	void changeCurrent(const QString& string);
	void slotMeasureEnable( bool state );
};


//-----------------------------------------------------------------------------
#endif
