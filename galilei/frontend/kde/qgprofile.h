/*

	GALILEI Research Project

	QGProfile.h

	Widget to show a profile - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
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
#ifndef QGProfileH
#define QGProfileH


//-----------------------------------------------------------------------------
// include files for Qt
#include <qwidget.h>
class QListView;



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class GProfile;


//-----------------------------------------------------------------------------
/**
* The QGProfile class provides a widget to show a profile.
* @author Pascal Francq
* @short Profile's Widget.
*/
class QGProfile : public QWidget
{
	Q_OBJECT

	/**
	* Corresponding profile.
	*/
	GProfile* Profile;

	/**
	* Widget to handle info about the profile.
	*/
	QListView* General;

public:

	/**
	* Constructor 
	* @param parent         parent widget.
	* @param profile        Profile.
	*/
	QGProfile(QWidget* parent,GProfile* profile);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public slots:

	/**
	* Slot called when the profile was changed after a computation.
	*/
	void slotProfileChanged(void);

public:

	/**
	* Destructor.
	*/
	~QGProfile(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif