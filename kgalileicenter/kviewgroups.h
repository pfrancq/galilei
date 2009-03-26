/*

	GALILEI Research Project

	KViewGroups.h

	Window to list topics or communities - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef KViewGroupsH
#define KViewGroupsH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gtopic.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
//include files for current application
#include <ui_kviewgroups.h>


//-----------------------------------------------------------------------------
/**
 * The KViewGroups class provides a template for a widget that shows all the
 * topics or the communities.
 * @author Pascal Francq.
 * @short Generic Groups Window.
 */
template<class cGroup>
	class KViewGroups : public QMdiSubWindow, public Ui_KViewGroups
{
protected:

	/**
	 * Construct the window.
	 */
	KViewGroups(void);
};


//-----------------------------------------------------------------------------
/**
 * The KViewTopics class represents a window to manipulate the topics of the
 * system.
 * @author Pascal Francq.
 * @short Topics Window.
 */
class KViewTopics : public KViewGroups<GTopic>
{
	Q_OBJECT

public:

	/**
	* Construct the view.
	*/
	KViewTopics(void);

public slots:

	/**
	 * Update the window.
	 */
	void update(void);

	/**
	* Slot that search the next item containing the text of the edit field.
	*/
	void FindNext(void);
};


//-----------------------------------------------------------------------------
/**
 * The KViewCommunities class represents a window to manipulate the communities
 * of the system.
 * @author Pascal Francq.
 * @short Communities Window.
 */
class KViewCommunities : public KViewGroups<GCommunity>
{
	Q_OBJECT

public:

	/**
	* Construct the view.
	*/
	KViewCommunities(void);

public slots:

	/**
	 * Update the window.
	 */
	void update(void);

	/**
	* Slot that search the next item containing the text of the edit field.
	*/
	void FindNext(void);
};


//------------------------------------------------------------------------------
#endif
