/*

	GALILEI Research Project

	QGSubProfileDesc.h

	Widget to show a generic subprofile description - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		

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



//-----------------------------------------------------------------------------
#ifndef QGSubProfileDescH
#define QGSubProfileDescH


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;
class GSubProfileDesc;


//-----------------------------------------------------------------------------
/**
* The QGSubProfileDesc class provides a widget to represent a generic
* description of a subprofile.
* @author Pascal Francq
* @short Generic Subprofile Description.
*/
class QGSubProfileDesc : public QWidget
{
	Q_OBJECT

protected:
	
	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param session        Session.
	* @param desc           Description.
	*/
	QGSubProfileDesc(QWidget* parent,GSession* session);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const QGSubProfileDesc* sub) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const QGSubProfileDesc& sub) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const tSubProfileDesc t) const;

	/**
	* Get the type of the description.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const=0;

	/**
	* Get the name of the type.
	* @returns Pointer to a C string.
	*/
	virtual const char* GetTypeName(void) const=0;

	/**
	* Construct the the subprofile.
	*/
	virtual void Construct(void) {}

public:

	/**
	* Destructor
	*/
	~QGSubProfileDesc(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
