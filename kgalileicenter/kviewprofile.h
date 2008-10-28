/*

	GALILEI Research Project

	KViewProfile.h

	Window to manipulate a given profile - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
#ifndef KViewProfileH
#define KViewProfileH



//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
// forward class declaration
class QTabWidget;
class QListView;
namespace GALILEI
{
	class GProfile;
	class QGProfile;
	class GInOutputBase;
	class QGWeightInfos;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The KViewProfile class provides a window to manipulate a given profile.
* @author Pascal Francq and David Wartel
* @short Profile's Window.
*/
class KViewProfile : public KView
{
	Q_OBJECT

	/**
	* Profile corresponding to the window.
	*/
	GALILEI::GProfile* Profile;

	/**
	* Widget to handle the different information of the profile.
	*/
	QTabWidget* Infos;

	/**
	* Widget to handle the different information of the profile.
	*/
	QTabWidget* InfosSub;

	/**
	* Widget to handle info about the user of the profile.
	*/
	QListView* User;

	/**
	* Widget to handle info about the profile.
	*/
	QGProfile* General;

	/**
	* Feedback over the documents.
	*/
	QListView* Fdbks;

	/**
	* Feedback over the documents obtained by link analyze.
	*/
	QListView* FdbksLinks;

	/**
	* Widget to handle the groups of the profile.
	*/
	QListView* Groups;

	/**
	* Results of the analyse.
	*/
	class QGWeightInfos* Results;

public:

	/**
	* Constructor for the view
	* @param profile        Profile to show.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewProfile(GALILEI::GProfile* profile,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Get the profile of this window.
	* @return Pointer to a GALILEI::GProfile.
	*/
	GProfile* GetProfile(void) {return(Profile);}

	/**
	* Construct the feedbacks' widget.
	*/
	void ConstructFdbks(void);

	/**
	* Constructs the links' widget.
	*/
	void ConstructLinks(void);

	/**
	* Construct the groups' widget.
	*/
	void ConstructGroups(void);

	/**
	* Construct the user' widget.
	*/
	void ConstructUser(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups,3=Links)
	*/
	virtual void update(tObjType type);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Compute the current profile.
	*/
	void ComputeProfile(void);

	/**
	* Destructor for the main view.
	*/
	~KViewProfile(void);
};


//-----------------------------------------------------------------------------
#endif
