/*

	GALILEI Research Project

	QGSubProfileDescs.h

	Widget to show all the subprofiles for specific description type - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		

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
#ifndef QGSubProfileDescsH
#define QGSubProfileDescsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qcombobox.h>
#include <qtabwidget.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GProfile;
class GSession;
class QGSubProfileDesc;


//-----------------------------------------------------------------------------
/**
* The QGSubProfiles class provides a widget to show a profile with a particular
* description type. It handles a set of GALILEI::QGSubProfileDesc widgets to
* hold the information for each language.
* @author Pascal Francq
* @version $Revision$
* @short SubProfiles' Widget.
*/
class QGSubProfileDescs : public QWidget
{
	Q_OBJECT

	/**
	* Corresponding session.
	*/
	GSession* Session;

	/**
	* Corresponding profile.
	*/
	GProfile* Profile;

	/**
	* Container of subprofile's widget.
	*/
	R::RContainer<QGSubProfileDesc,unsigned int,true,false> SubProfilesDesc;

	/**
	* Current SubProfile showed.
	*/
	QGSubProfileDesc* Current;

	/**
	*  Combo Box to choose the language of the profile.
	*/
	QComboBox* Lang;

	/**
	* Type of the description used.
	*/
	tSubProfileDesc DescType;

public:

	/**
	* Constructor 
	* @param parent         parent widget.
	* @param session        Session.
	* @param profile        Profile.
	* @param t              Type of the description
	*/
	QGSubProfileDescs(QTabWidget* parent,GSession* session,GProfile* profile,tSubProfileDesc t);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

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
	* Destructor.
	*/
	~QGSubProfileDescs(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
