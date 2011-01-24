/*

	GALILEI Research Project

	KViewProfile.h

	Window to manipulate a given profile - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef KViewProfileH
#define KViewProfileH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <gprofile.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kviewprofile.h>
#include <kgalileicenter.h>


//-----------------------------------------------------------------------------
/**
* The KViewProfile class provides a window to manipulate a given profile.
* @author Pascal Francq and David Wartel
* @short Profile Window.
*/
class KViewProfile : public QMdiSubWindow, public Ui_KViewProfile
{
	Q_OBJECT

	/**
	* Profile corresponding to the window.
	*/
	GProfile* Profile;

	/**
	 * Application.
	 */
	KGALILEICenter* App;

public:

	/**
	* Construct the widget.
	* @param app            Application.
	* @param profile        Profile to show.
	*/
	KViewProfile(KGALILEICenter* app,GProfile* profile);

public slots:

	/**
	* Called to redraw the window.
	*/
	virtual void updateProfile(void);

	/**
	* Called to redraw the window.
	*/
	virtual void updateCommunity(void);

public:

	/**
	* Compute the current profile.
	*/
	void ComputeProfile(void);

	/**
	* Destruct the widget.
	*/
	~KViewProfile(void);
};


//-----------------------------------------------------------------------------
#endif
