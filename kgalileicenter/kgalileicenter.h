/*

	GALILEI Research Project

	KGALILEICenter.h

	Main Window - Header.

	(C) 2001 by Pascal Francq

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
#ifndef KGALILEICenterH
#define KGALILEICenterH
 

//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <urlmanagers/gurlmanagerkde.h>
namespace GALILEI
{
	class GDocOptions;
	class GProfOptions;
	class GGroupingOptions;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qstring.h>
class QWorkspace;
class QListViewItem;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kmainwindow.h>
class KAction;
class KToggleAction;
class KActionMenu;
class KConfig;


//-----------------------------------------------------------------------------
// forward declaration of the KGALILEICenter classes
class KDoc;
class KView;


//-----------------------------------------------------------------------------
/**
* The base class for KGALILEICenter application windows. It sets up the main
* window and reads the config file as well as providing a menubar, toolbar and
* statusbar. In initView(), your main view is created as the MDI child window
* manager. Child windows are created in createClient(), which gets a document
* instance as it's document to display whereby one document can have several
* views.The MDI child is an instance of KGALILEICenterView, the document an
* instance of KGALILEICenterDoc.
* KGALILEICenterApp reimplements the methods that KTMainWindow provides for
* main window handling and supports full session management as well as keyboard
* accelerator configuration by using KAccel.
* @see KMainWindow
* @see KApplication
* @see KConfig
* @see KAccel
*
* @author Pascal Francq.
* @version $Revision$
* @short Main Window.
*/
class KGALILEICenterApp : public KMainWindow, public GURLManagerKDE
{
	Q_OBJECT

	/**
	* The configuration object of the application.
	*/
	KConfig* Config;

	/**
	* Database server's host.
	*/
	QString dbHost;

	/**
	* Database's name.
	*/
	QString dbName;

	/**
	* Name of the user used to access the database.
	*/
	QString dbUser;

	/**
	* Password used to access the database.
	*/
	QString dbPwd;

	/**
	* Label to hold an image representing the status of the database connection.
	*/
	QLabel* dbStatus;

	/**
	* pWorkspace is the MDI frame widget that handles MDI child widgets.
	* Inititalized in initView()
	*/
	QWorkspace* pWorkspace;

	/**
	* The printer instance.
	*/
	QPrinter* Printer;

	/**
	* A pointer to the "document" which represents a specific GALILEI System.
	*/
	KDoc* Doc;

	/**
	* Options of the documents.
	*/
	GDocOptions* DocOptions;

public:

	/**
	* Construtor of KGALILEICenterApp, calls all init functions to create the
	* application.
	* @see initMenuBar initToolBar
	*/
	KGALILEICenterApp(void);

protected:

	/**
	* Initializes the KActions of the application.
	*/
	void initActions(void);

	/**
	* Sets up the statusbar for the main window by initialzing a statuslabel.
	*/
	void initStatusBar(void);

	/**
	* Creates the main view of the KTMainWindow instance and initializes the
	* MDI view area including any needed connections.
	*/
	void initView(void);

	/**
	* Set the parameters of the system to a document.
	* @param doc            Document to assign.
	*/
	void setDocParams(KDoc* doc);

	/**
	* Creates a new child window. The document that will be connected to it has
	* to be created before and the instances filled.
	* Then call createClient() to get a new MDI child window.
	* @see KDoc#addView
	* @param doc            Pointer to the document instance.
	* @param view           The view.
	*/
	void createClient(KDoc* doc,KView* view);

	/**
	* Save general Options like all bar positions and status as well as the
	* geometry and the recent file list to the configuration file.
	*/
	void saveOptions(void);

	/**
	* Read general Options again and initialize all variables like the recent
	* file list.
	*/
	void readOptions(void);

	/**
	* Saves the window properties for each open window during session end to
	* the session config file, including saving the currently opened file by a
	* temporary filename provided by KApplication.
	* @see KTMainWindow#saveProperties
	*/
	virtual void saveProperties(KConfig* _cfg);

	/**
	* Reads the session config file and restores the application's state
	* including the last opened files and documents by reading the temporary
	* files saved by saveProperties().
	* @see KTMainWindow#readProperties
	*/
	virtual void readProperties(KConfig* _cfg);

	/**
	* queryClose is called by KTMainWindow on each closeEvent of a window.
	* Against the default implementation (only returns true), this overridden
	* function retrieves all modified documents from the open document list and
	* asks the user to select which files to save before exiting the
	* application.
	* @see KTMainWindow#queryClose
	* @see KTMainWindow#closeEvent
	*/
	virtual bool queryClose(void);

	/**
	* queryExit is called by KTMainWindow when the last window of the
	* application is going to be closed during the closeEvent().
	* Against the default implementation that just returns true, this calls
	* saveOptions() to save the settings of the last window's properties.
	* @see KTMainWindow#queryExit
	* @see KTMainWindow#closeEvent
	*/
	virtual bool queryExit(void);

	/**
	* Verify the menu entries to see which one must be activated.
	*/
	void UpdateMenusEntries(void);

	/**
	* Disable all menu actions.
	*/
	void DisableAllActions(void);

	/**
	* Event filter to catch close events for MDI child windows and is installed
	* in createClient() on every child window.
	* Closing a window calls the eventFilter first which removes the view from
	* the connected documents' view list. If the last view is going to be
	* closed, the eventFilter() tests if the document is modified; if yes, it
	* asks the user to save the document. If the document title contains
	* "Untitled", slotFileSaveAs() gets called to get a save name and path.
	*/
	virtual bool eventFilter(QObject* object,QEvent* event);

private slots:

	/**
	* Connect to a MySQL database.
	*/
	void slotSessionConnect(void);

	/**
	* Compute an entire session.
	*/
	void slotSessionCompute(void);

	/**
	* Disconnect to a MySQL database.
	*/
	void slotSessionDisconnect(void);

	/**
	* Test something.
	*/
	void slotSessionTest(void);

	/**
	* Closes all documents and quits the application.
	*/
	void slotSessionQuit(void);

	/**
	* Show the users' window.
	*/
	void slotShowUsers(void);

	/**
	* Opens a dialog box to add a new user.
	*/
	void slotAddUser(void);

	/**
	* Opens a dialog box to add a new profile.
	*/
	void slotAddProfile(void);

	/**
	* Compute a profile.
	*/
	void slotProfileCalc(void);

	/**
	* slot to calculate all the profiles
	*/
	void slotProfilesCalc(void);

	/**
	* Show the groups' window.
	*/
	void slotShowGroups(void);

	/**
	* Compute the groups.
	*/
	void slotGroupsCalc(void);

	/**
	* Compare the current grouping with a theoritic one given in a file.
	*/
	void slotGroupingCompare(void);

	/**
	* Show the documents' window.
	*/
	void slotShowDocs(void);

	/**
	* Opens a dialog box to add a document.
	*/
	void slotAddDoc(void);

	/**
	* Analyse all the documents.
	*/
	void slotDocsAnalyse(void);

	/**
	* Compute statistics on all the documents.
	*/
	void slotDocsStats(void);

	/**
	* Opens a dialog box to add a judgement.
	*/
	void slotAddJudgement(void);

	/**
	* Opens a dialog box to add a judgement.
	*/
	void slotEditJudgement(void);

	/**
	* Create a XML structure for the current document.
	*/
	void slotCreateXML(void);

	/**
	* Analyse the XML structure of the current document.
	*/
	void slotAnalyseXML(void);

	/**
	* Do some computing on the stems in french.
	*/
	void slotTextFrench(void);

	/**
	* Do some computing on the stems in english.
	*/
	void slotTextEnglish(void);

	/**
	* Initialize the GA.
	*/
	void slotGAInit(void);

	/**
	* Start the GA.
	*/
	void slotGAStart(void);

	/**
	* Pause the GA.
	*/
	void slotGAPause(void);

	/**
	* Stop the GA.
	*/
	void slotGAStop(void);

	/**
	* Toggles the toolbar.
	*/
	void slotViewToolBar(void);

	/**
	* Toggles the statusbar.
	*/
	void slotViewStatusBar(void);

	/**
	* Configure all the plugins of the system.
	*/
	void slotPlugins(void);

	/**
	* Load a program text file.
	*/
	void slotLoadProgram(void);

	/**
	* Called when the option about the profile's has to be changed.
	*/
	void slotDocsOptions(void);

	/**
	* Gets called when the window menu is activated; recreates the window menu
	* with all opened window titles.
	*/
	void windowMenuAboutToShow(void);

	/**
	* Activates the MDI child widget when it gets selected from the window
	* menu. */
	void windowMenuActivated(int id);

	/**
	*/
	void slotWindowTile(void);

	/**
	*/
	void slotWindowCascade(void);

	/**
	* Changes the statusbar contents for the standard label permanently, used
	* to indicate current actions.
	* @param text   The text that is displayed in the statusbar
	*/
	void slotStatusMsg(const QString& text);

public slots:

	/**
	* Called when a window is activated.
	*/
	void slotWindowActivated(QWidget* w);

	/**
	* This slot is called when a QListViewItem containing some GALILEI object
	* is selected with a double click.
	* @param item           Item that has been double clicked.
	*/
	void slotHandleItem(QListViewItem* item);

public:

	/**
	* Destructor.
	*/
	~KGALILEICenterApp(void);

public:

	// KAction pointers to enable/disable actions
	KToggleAction* sessionAlwaysCalc;
	KAction* sessionConnect;
	KAction* sessionCompute;
	KAction* sessionDisconnect;
	KAction* sessionTest;
	KAction* sessionQuit;

	KToggleAction* profileAlwaysCalc;
	KAction* showUsers;
	KAction* addUser;
	KAction* addProfile;
	KAction* profilesCalc;
	KAction* profileCalc;

	KToggleAction* groupAlwaysCalc;
	KAction* showGroups;
	KAction* groupsCalc;
	KAction* groupingCompare;

	KToggleAction* docAlwaysCalc;
	KAction* showDocs;
	KAction* addDoc;
	KAction* docsAnalyse;
	KAction* docsStats;
	KAction* addJudgement;
	KAction* editJudgement;
	KAction* createXML;
	KAction* analyseXML;

	KAction* textFrench;
	KAction* textEnglish;

	KAction* gaInit;
	KAction* gaStart;
	KAction* gaPause;
	KAction* gaStop;

	KAction* plugins;
	KAction* docsOptions;

	KAction* loadProgram;

	KAction* windowTile;
	KAction* windowCascade;
	KToggleAction* viewToolBar;
	KToggleAction* viewStatusBar;
	KActionMenu* windowMenu;


};


//-----------------------------------------------------------------------------
#endif
