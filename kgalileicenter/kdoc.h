/*

	GALILEI Research Project

	KDoc.h

	Document representing a GALILEI session - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#ifndef KDocH
#define KDocH


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qobject.h>
#include <qptrlist.h>


//-----------------------------------------------------------------------------
// forward declaration
class KView;
class KViewDocs;
class KViewCommunities;
class KViewUsers;
class KViewTopics;
class KGALILEICenterApp;


//-----------------------------------------------------------------------------
/**
* KDoc provides a document object for a document-view model.
*
* The KDoc class provides a document object that can be used in
* conjunction with the classes KGALILEICenterApp and KGALILEICenterView to
* create a document-view model for MDI (Multiple Document Interface) KDE 2
* applications based on KApplication and KTMainWindow as main classes and
* QWorkspace as MDI manager widget.
* Thereby, the document object is created by the KGALILEICenterApp instance
* (and kept in a document list) and contains the document structure with the
* according methods for manipulating the document data by KGALILEICenterView
* objects. Also, KDoc contains the methods for serialization of
* the document data from and to files.
* @author Pascal Francq
*/
class KDoc : public QObject//, public GStorageMySQL
{
	Q_OBJECT

	/**
	* The modified flag of the current document.
	*/
	bool modified;

	/**
	* The list of the views currently connected to the document.
	*/
	QPtrList<KView>* pViewList;

	/**
	* GALILEI Session.
	*/
	GALILEI::GSession* Session;

	/**
	* Window of the documents.
	*/
	KViewDocs* WinDocs;

	/**
	* Window of the users.
	*/
	KViewUsers* WinUsers;

	/**
	* Window of the groups.
	*/
	KViewCommunities* WinCommunities;

	/**
	* Window of the topics.
	*/
	KViewTopics* WinTopics;

	/**
	* Owner of the document.
	*/
	KGALILEICenterApp* Owner;

public:

	/**
	* Constructor for the fileclass of the application.
	* @param owner          Owner of the document.
	* @param host           The host name.
	* @param user           The user name.
	* @param passwd         The passwd.
	* @param db             The database.
	* @param encoding       Encoding of the database.
	* @param all            All objects must be loaded?
	* @param filter         Date filter.
	*/
	KDoc(KGALILEICenterApp* owner);

	/**
	* Set the session corresponding of the document.
	* @param session         Pointer to the session.
	*/
	void SetSession(GSession* session) {Session=session;}

	/**
	* Get the session corresponding of the document.
	* @returns Pointer to a GALILEI::GSession.
	*/
	GALILEI::GSession* GetSession(void) {return(Session);}

	/**
	* Get the window showing the documents.
	* @param Pointer to KViewDocs or 0 if the window is not open.
	*/
	KViewDocs* GetWinDocs(void) const {return(WinDocs);}

	/**
	* Get the window showing the users.
	* @param Pointer to KViewUsers or 0 if the window is not open.
	*/
	KViewUsers* GetWinUsers(void) const {return(WinUsers);}

	/**
	* Get the window showing the groups.
	* @param Pointer to KViewGroups or 0 if the window is not open.
	*/
	KViewCommunities* GetWinGroups(void) const {return(WinCommunities);}

	/**
	* Get the window showing the topics.
	* @param Pointer to KViewTopics or 0 if the window is not open.
	*/
	KViewTopics* GetWinTopics(void) const {return(WinTopics);}

	/**
	* Adds a view to the document which represents the document contents.
	* Usually this is your main view.
	*/
	void addView(KView* view);

	/**
	* Removes a view from the list of currently connected views.
	*/
	void removeView(KView* view);

	/**
	* Gets called if a view is removed or added.
	*/
	void changedViewList(void);

	/**
	* Returns the first view instance.
	*/
	KView* firstView(void)
		{return(pViewList->first());};

	/**
	* Returns true, if the requested view is the last view of the document.
	*/
	bool isLastView(void);

	/**
	* This method gets called when the user is about to close a frame window.
	* It checks, if more than one view is connected to the document (then the
	* frame can be closed), if pFrame is the last view and the document is
	* modified, the user gets asked if he wants to save the document.
	*/
	bool canCloseFrame(KView* pFrame);

	/**
	* Sets the modified flag for the document after a modifying action on the
	* view connected to the document.
	*/
	void setModified(bool _m=true)
		{modified=_m;};

	/**
	* Returns if the document is modified or not. Use this to determine if your
	* document needs saving by the user on closing.
	*/
	bool isModified(void)
		{return modified;};

	/**
	* Deletes the document's contents.
	*/
	void deleteContents(void);

	/**
	* Initializes the document generally.
	*/
	bool newDocument(void);

	/**
	* closes the acutal document.
	*/
	void closeDocument(void);

public slots:

	/**
	* Calls repaint() on all views connected to the document object and is
	* called by the view by which the document has been changed. As this view
	* normally repaints itself, it is excluded from the paintEvent.
	* @param type            Type.
	*/
	void updateAllViews(tObjType type);

public:

	/**
	* Destruct the fileclass of the application.
	*/
	virtual ~KDoc(void);
};


//-----------------------------------------------------------------------------
#endif
