/*

	GALILEI Research Project

	KGALILEICenter.h

	Main Window - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#ifndef KGALILEICenterH
#define KGALILEICenterH
 

//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <infos/glangmanager.h>
#include <docs/glinkcalcmanager.h>
#include <docs/gdocanalysemanager.h>
#include <docs/gpostdocmanager.h>
#include <engines/genginemanager.h>
#include <profiles/gprofilecalcmanager.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalcmanager.h>
#include <sessions/gsession.h>
#include <groups/gpostgroupmanager.h>
#include <sessions/gstatscalcmanager.h>
#include <frontend/kde/gfiltermanagerkde.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
class QWorkspace;
class QListViewItem;
class QLabel;

class QSessionProgressDlg;

//-----------------------------------------------------------------------------
// include files for KDE
#include <qstring.h>
#include <kprocess.h>
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
* @short Main Window.
*/
class KGALILEICenterApp : public KMainWindow
{
	Q_OBJECT

	/**
	* Languages.
	*/
	GLangManager* Langs;

	/**
	* URL Manager.
	*/
	GFilterManagerKDE* URLManager;

	/**
	* Doc Analyse Manager.
	*/
	GDocAnalyseManager* DocAnalyseManager;

	/**
	* Profiling Method Manager.
	*/
	GProfileCalcManager* ProfilingManager;

	/**
	* Grouping Method Manager.
	*/
	GGroupingManager* GroupingManager;

	/**
	* Computing Method Manager.
	*/
	GGroupCalcManager* GroupCalcManager;

	/**
	* Statistics Manager.
	*/
	GStatsCalcManager* StatsCalcManager;

	/**
	* Linking Manager.
	*/
	GLinkCalcManager* LinkCalcManager;

	/**
	* PostDocManager Manager.
	*/
	GPostDocManager* PostDocManager;

	/**
	* PostGroup Manager.
	*/
	GPostGroupManager* PostGroupManager;
	
	/**
	* Engine & meta engine Manager.
	*/
	GEngineManager* EngineManager;

	/**
	* The configuration object of the application.
	*/
	KConfig* Config;

	/**
	* Parameters of the session.
	*/
	GSessionParams SessionParams;

	/**
	* Database server's host.
	*/
	R::RString dbHost;

	/**
	* Database's name.
	*/
	R::RString dbName;

	/**
	* Name of the user used to access the database.
	*/
	R::RString dbUser;

	/**
	* Password used to access the database.
	*/
	R::RString dbPwd;

	/**
	* Enconding of the database.
	*/
	R::RString dbEncoding;

	/**
	* path to plugins.
	*/
	R::RString pluginsPath;
	
	/**
	* the path where the sql file for the database creation are stored
	*/
	R::RString CreateDbSQLpath;
	
	/**
	* specifies if the stoplists table must be filled when a new Db is created
	*/
	bool CreateDbUseStopList;
	
	/**
	* specifies if the users table must be filled when a new Db is created
	*/
	bool CreateDbUseUsers;

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
	* A pointer to the "document" which represents a specific GALILEI System.
	*/
	KDoc* Doc;

	/**
	* the list containing all the error messages
	*/
	QString ErrMsgList;

	/**
	* progressDlg -> maintain the progress of database creation
	*/
	QSessionProgressDlg *d;

public:

	/**
	* Construtor of KGALILEICenterApp, calls all init functions to create the
	* application.
	* @see initMenuBar initToolBar
	*/
	KGALILEICenterApp(void) throw(GException);

	/**
	* Returns a pointer to the document connected to the view.
	*/
	KDoc* getDocument(void) const;

	/**
	* Returns a pointer to the current session.
	*/
	GSession* getSession(void) const;

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
	* Read GALILEI Options again and initialize all variables like the recent
	* file list.
	*/
	void readGALILEIOptions(void);

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
	* slot to fill database from repesitory.
	*/
	void slotFillEmptyDb(void);

	/**
	* Statistics window.
	*/
	void slotSessionStats(void);

	/**
	* Closes all documents and quits the application.
	*/
	void slotSessionQuit(void);

	/**
	* Create a new database with all required tables and fiels + all the value of 'stopkwds' tables.
	*/
	void slotCreateDatabase(void);

	/**
	* slot to export a matrix from database.
	*/
	void slotExportMatrix(void);

	/**
	* Show the users' window.
	*/
	void slotShowUsers(void);

	/**
	* slot to calculate all the profiles
	*/
	void slotProfilesCalc(void);

	/**
	* slot to choose the self organizing map
	*/
	void slotChooseSOM(void);

	/**
	* slot to view the self organizing map
	*/
	void slotViewSOM(QListViewItem* item);

	/**
	* Show the groups window.
	*/
	void slotShowGroups(void);

	/**
	* Compute the groups.
	*/
	void slotGroupsCalc(void);

	/**
	* Compute the groups.
	*/
	void slotPostGroupCalc(void);

	/**
	* Compare the current grouping with a theoritic one given in a file.
	*/
	void slotGroupingCompareFromFile(void);

	/**
	* Compare the current grouping with the grouping in memory.
	*/
	void slotGroupingCompare(void);

	/**
	* mix the ideal groupment.
	*/
	void slotMixIdealGroups(void);

	/**
	* Show the documents window.
	*/
	void slotShowDocs(void);

	/**
	* Analyse a document.
	*/
	void slotDocAnalyse(void);

	/**
	* Analyse all the documents.
	*/
	void slotDocsAnalyse(void);

	/**
	* Create a DocXML structure for the current document.
	*/
	void slotCreateXML(void);

	/**
	* Save a DocXML structure in a document.
	*/
	void slotSaveXML(void);

	/**
	* Analyse the XML structure of the current document.
	*/
	void slotAnalyseXML(void);
	
	/**
	* Query the meta engine
	*/
	void slotQueryMetaEngine(void);

	/**
	* Construct the XML structure of the MIME types based on the one of KDE.
	*/
	void slotFillMIMETypes(void);

	/**
	* Do some computing on the stems in french.
	*/
	void slotTextFrench(void);

	/**
	* Do some computing on the stems in english.
	*/
	void slotTextEnglish(void);

	/**
	* Show the chromosomes stored.
	*/
	void slotShowHistory(void);

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
	* configure the parameters of the sessions
	*/
	void slotSessionOptions(void);

	/**
	* Run a program text file.
	*/
	void slotRunProgram(void);

	/**
	* Gets called when the window menu is activated; recreates the window menu
	* with all opened window titles.
	*/
	void windowMenuAboutToShow(void);

	/**
	* Activates the MDI child widget when it gets selected from the window
	* menu.
	*/
	void windowMenuActivated(int id);

	/**
	* Called when the windows must be tiled.
	*/
	void slotWindowTile(void);

	/**
	* Called when the windows must be cascaded.
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
	KAction* sessionStats;
	KAction* createDatabase;
	KAction* sessionExportMatrix;
	KAction* sessionDisconnect;
	KAction* fillEmptyDb;
	KAction* sessionQuit;


	KToggleAction* profileAlwaysCalc;
	KToggleAction* profileAlwaysSave;
	KToggleAction* linkAlwaysSave;
	KAction* showUsers;
	KAction* profilesCalc;

	KToggleAction* groupAlwaysCalc;
	KToggleAction* groupAlwaysSave;
	KToggleAction* groupHistorySave;
	KAction* showGroups;
	KAction* groupsCalc;
	KAction* postgroupCalc;
	KAction* somView;
	KAction* groupingCompareFromFile;
	KAction* groupingCompare;
	KAction* mixIdealGroups;
	KAction* showGroupsHistory;

	KToggleAction* docAlwaysCalc;
	KToggleAction* docAlwaysSave;
	KAction* showDocs;
	KAction* docAnalyse;
	KAction* docsAnalyse;
	KAction* createXML;
	KAction* saveXML;
	KAction* analyseXML;
	KAction* queryMetaEngine;
	KAction* fillMIMETypes;

	KAction* textFrench;
	KAction* textEnglish;

	KAction* plugins;
	KAction* sessionOptions;

	KAction* runProgram;

	KAction* windowTile;
	KAction* windowCascade;
	KToggleAction* viewToolBar;
	KToggleAction* viewStatusBar;
	KActionMenu* windowMenu;

	friend class QSessionProgressDlg;
};


//-----------------------------------------------------------------------------
#endif
