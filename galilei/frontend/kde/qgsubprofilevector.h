/*

	GALILEI Research Project

	QGSubProfileVector.h

	Widget to show a subprofile desciption in vector model - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef QGSubProfileVectorH
#define QGSubProfileVectorH


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
#include <qlistview.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qgsubprofiledesc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GIWordWeigth;
class GSubProfileVector;


//-----------------------------------------------------------------------------
/**
* The QGSubProfileVector class provides a widget to represent the
* description of a subprofile in the vector model.
* @author Pascal Francq
* @short Vector Subprofile Description Widget.
*/
class QGSubProfileVector : public QGSubProfileDesc
{
	Q_OBJECT

	/**
	* Description of the SubProfile.
	*/
	GSubProfileVector* Desc;

	/**
	* Information on the sub-profile.
	*/
	QListView* Info;

	/**
	* Vector representing the sub-profile.
	*/
	QListView* Vector;

	/**
	* Temporary C string used to construct a word.
	*/
	char tmpWord[50];

public:

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param session        Session.
	* @param desc           Description.
	*/
	QGSubProfileVector(QWidget* parent,GSession* session,GSubProfileVector* desc);

	/**
	* Get the type of the description.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const {return(sdVector);}

	/**
	* Get the name of the type.
	* @returns Pointer to a C string.
	*/
	virtual const char* GetTypeName(void) const {return("Vector Description");}

public:

	/**
	* Construct the the subprofile.
	*/
	virtual void Construct(void);

	/**
	* Destructor
	*/
	~QGSubProfileVector(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
