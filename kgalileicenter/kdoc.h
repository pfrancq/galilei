/*

	GALILEI Research Project

	KDoc.h

	Document representing a GALILEI session - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
namespace GALILEI
{
	class GProfileCalc;
	class GProfileCalcVector;
	class GGrouping;
	class GGroupingSim;
	class GSession;
}


//-----------------------------------------------------------------------------
// include files for Qt
#include <qobject.h>
#include <qlist.h>


//-----------------------------------------------------------------------------
// forward declaration
class KView;
class KViewDocs;
class KViewGroups;
class KViewUsers;
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
* @version $Revision$
*/
class KDoc : public QObject
{
	Q_OBJECT

	/**
	* The modified flag of the current document.
	*/
	bool modified;

	/**
	* The list of the views currently connected to the document.
	*/
	QList<KView>* pViewList;

	/**
	* GALILEI Session. The document has to desallocate it.
	*/
	GALILEI::GSession* Session;

	/**
	* Statistical method.
	*/
	GALILEI::GProfileCalcVector* Stat;

	/**
	* Grouping method based on similarity.
	*/
	GALILEI::GGroupingSim* Sim;

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
	KViewGroups* WinGroups;

	/**
	* Owner of the document.
	*/
	KGALILEICenterApp* Owner;

public:

	/**
	* Constructor for the fileclass of the application.
	* @param owner          Owner of the document.
	* @param session        GALILEI session.
	*/
	KDoc(KGALILEICenterApp* owner,GALILEI::GSession* session);

	/**
	* Get the session corresponding of the document.
	* @returns Pointer to a GALILEI::GSession.
	*/
	GALILEI::GSession* GetSession(void) {return(Session);}

	/**
	* Get the statistical method.
	* @returns Pointer to a GALILEI::GProfileCalcVector.
	*/
	GALILEI::GProfileCalcVector* GetStat(void) {return(Stat);}

	/**
	* Get the current method use to compute the profiles.
	* @returns Pointer to a GALILEI::GProfileCalc.
	*/
	GALILEI::GProfileCalc* GetCurComputeProfile(void);

	/**
	* Get the grouping method based on similarity.
	* @returns Pointer to a GALILEI::GProfileCalcList.
	*/
	GALILEI::GGroupingSim* GetGroupingSim(void) {return(Sim);}

	/**
	* Get the current method use to compute the profiles.
	* @returns Pointer to a GALILEI::GGrouping.
	*/
	GALILEI::GGrouping* GetCurGrouping(void);

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
	KViewGroups* GetWinGroups(void) const {return(WinGroups);}

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
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	void updateAllViews(unsigned int cmd);

public:

	/**
	* Destructor for the fileclass of the application.
	*/
	~KDoc(void);
};


//-----------------------------------------------------------------------------
#endif
