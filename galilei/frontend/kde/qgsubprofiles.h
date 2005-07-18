/*

	GALILEI Research Project

	QGSubProfileVector.h

	Widget to show a subprofile desciption in vector model - Header.

	Copyright 2002 by the Universit�Libre de Bruxelles.

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
// include files for R Library
#include <rcontainer.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qtabwidget.h>
#include <qlistview.h>
#include <qcombobox.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GProfile;
class GSession;


//-----------------------------------------------------------------------------
/**
* The QGSubProfileVector class provides a widget to represent the
* description of a subprofile in the vector model.
* @author Pascal Francq
* @short Subprofiles Description Widget.
*/
class QGSubProfiles : public QWidget
{
	Q_OBJECT

	class SubProfile;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Corresponding profile.
	*/
	GProfile* Profile;

	/**
	* Container of subprofile's widget.
	*/
	R::RContainer<SubProfile,true,false> Desc;

	/**
	* Current SubProfile showed.
	*/
	SubProfile* Current;

	/**
	*  Combo Box to choose the language of the profile.
	*/
	QComboBox* Lang;

public:

	/**
	* Constructor
	* @param parent         parent widget.
	* @param session        Session.
	* @param profile        Profile.
	*/
	QGSubProfiles(QTabWidget* parent,GSession* session,GProfile* profile);

public slots:

	/**
	* Slot called when the language was changed.
	* @param index          Index of the language choosen.
	*/
	void slotLangChanged(int index);

	/**
	* Slot called when the profile was changed after a computation.
	*/
	void slotProfileChanged(void);

public:

	/**
	* Destructor
	*/
	~QGSubProfiles(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
