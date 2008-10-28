/*

	GALILEI Research Project

	QGWeightInfos.h

	Widget to display the description of an object - Header.

	Copyright 2008 by the Université libre de Bruxelles.

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
#ifndef QGWeightInfosH
#define QGWeightInfosH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfos.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The QGWeightInfos class provides a Qt Widget to display the description of a
 * GWeightInfos object. It shows the name of the different element.
 * @author Pascal Francq
 * @short Description Displayer
 */
class QGWeightInfos : public QListView
{
	/**
	 * Object for which the description must be shown
	 */
	GWeightInfos* Object;

	/**
	 * Current session.
	 */
	GSession* Session;

	/**
	 * Index space.
	 */
	GConceptType* IndexSpace;

public:

	/**
	 * Construct a widget that can display the description of an object.
	 * @param parent         Parent widget.
	 * @param obj            Object to display.
	 * @param session        Corresponding session.
	 */
	QGWeightInfos(QWidget* parent,GWeightInfos* obj,GSession* session);

	/**
	 * Set the current object to display. The widget is redraw.
	 * @param obj            Object to display.
	 */
	void SetObject(GWeightInfos* obj);
};


} //------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
