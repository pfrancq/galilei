/*

	GALILEI Research Project

	KGALILEICenter.h

	Main Window - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <galilei.h>
#include <groups/gir.h>
#include <groups/ggroupingkcos.h>
#include <groups/ggroupingsupkmeans.h>
#include <groups/ggroupingkprotos.h>
#include <groups/ggroupingcure.h>
#include <groups/ggroupingsim.h>
#include <groups/ggroupcalcrelevant.h>
#include <groups/ggroupcalcgravitation.h>
#include <profiles/gprofilecalcfeedback.h>
#include <profiles/gprofilecalcreweighting.h>
#include <profiles/gprofilecalcvector.h>
#include <urlmanagers/gurlmanagerkde.h>
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
	* Parameters of the GA.
	*/
	GIRParams IRParams;

	/**
	* Parameters of the KMeans.
	*/
	GKMeansParams KMeansParams;

	/**
	* Parameters of the KMeans.
	*/
	GSupKMeansParams SupKMeansParams;

	/**
	* Parameters of the KProtos.
	*/
	GKProtosParams KProtosParams;

	/**
	* Parameters of Cure.
	*/
	GCureParams CureParams;

	/**
	* Parameters of First-Fit.
	*/
	GSimParams SimParams;

	/**
	* Parameters of the feedback computing method.
	*/
	GFeedbackParams FeedbackParams;

	/**
	* Parameters for reweighting calc method.
	*/
	GReWeightingParams ReWeightingParams;

	/**
	* Parameters for statistical calc method.
	*/
	GStatisticalParams StatisticalParams;

	/**
	* Parameters for 'relevant' group description computing
	*/
	GCalcRelevantParams CalcRelevantParams;

	/**
	* Parameters for 'gravitation' group description computing
	*/
	GCalcGravitationParams CalcGravitationParams;

    /**
	* Current Profile Description Method
	*/
	RStd::RString CurrentProfileDesc;

	/**
	* Current Computing Method
	*/
	RStd::RString CurrentComputingMethod;

	/**
	* Current Grouping Method
	*/
	RStd::RString CurrentGroupingMethod;

	/**
	* Current Group Descrition Method
	*/
	RStd::RString CurrentGroupCalcMethod;

	/**
	* Database server's host.
	*/
	RStd::RString dbHost;

	/**
	* Database's name.
	*/
	RStd::RString dbName;

	/**
	* Name of the user used to access the database.
	*/
	RStd::RString dbUser;

	/**
	* Password used to access the database.
	*/
	RStd::RString dbPwd;

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
	* Container of subprofiles description method.
	*/
	RStd::RContainer<RStd::RString,unsigned int,true,true>* ProfileDesc;

	/**
	* Container of computing method.
	*/
	RStd::RContainer<RStd::RString,unsigned int,true,true>* ComputingMethod;

	/**
	* Container of grouping method.
	*/
	RStd::RContainer<RStd::RString,unsigned int,true,true>* GroupingMethod;

	/**
	* Container of group description method.
	*/
	RStd::RContainer<RStd::RString,unsigned int,true,true>* GroupCalcMethod;


public:

	/**
	* Construtor of KGALILEICenterApp, calls all init functions to create the
	* application.
	* @see initMenuBar initToolBar
	*/
	KGALILEICenterApp(void);

	/**
	* Returns a pointer to the document connected to the view.
	*/
	KDoc* getDocument(void) const;

	/**
	* Return the current subprofile description method used.
	*/
	RStd::RString GetCurrentProfileDesc(void) {return(CurrentProfileDesc);}

	/**
	* Set the current subprofile description method used.
	*/
	void SetCurrentProfileDesc(RStd::RString str) {CurrentProfileDesc=str;}

	/**
	* Return the current method used for computed the profile.
	*/
	RStd::RString GetCurrentComputingMethod(void) {return(CurrentComputingMethod);}

	/**
	* Set the current method used for computed the profile.
	*/
	void SetCurrentComputingMethod(RStd::RString str) {CurrentComputingMethod=str;}

	/**
	* Return the current grouping method used.
	*/
	RStd::RString GetCurrentGroupingMethod(void) {return(CurrentGroupingMethod);}

	/**
	* Set the current grouping method used.
	*/
	void SetCurrentGroupingMethod(RStd::RString str) {CurrentGroupingMethod=str;}

	/**
	* Return the Current group description method used.
	*/
	RStd::RString GetCurrentGroupCalcMethod(void) {return(CurrentGroupCalcMethod);}

	/**
	* Set the Current group description method used.
	*/
	void SetCurrentGroupCalcMethod(RStd::RString str) {CurrentGroupCalcMethod=str;}

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
	* Write the option corresponding to a similarity measure.
	* @param s              Pointer to the measure.
	*/
	void saveOptions(GSimMeasure* s);

	/**
	* Write the option corresponding to a promethee criterion.
	* @param n              Name of the criterion.
	* @param c              Pointer to the criterion.
	*/
	void saveOptions(const char* n,RPromethee::RPromCriterionParams& c);

	/**
	* Save general Options like all bar positions and status as well as the
	* geometry and the recent file list to the configuration file.
	*/
	void saveOptions(void);

	/**
	* Read the option corresponding to a similarity measure.
	* @param s              Pointer to the measure.
	*/
	void readOptions(GSimMeasure* s);

	/**
	* Read the option corresponding to a promethee criterion.
	* @param n              Name of the criterion.
	* @param c              Pointer to the criterion.
	*/
	void readOptions(const char* n,RPromethee::RPromCriterionParams& c);

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
	* AutoConnect to the default MySQL database.
	* @param host           The host name.
	* @param user           The user name.
	* @param passwd         The passwd.
	* @param db             The database.
	*/
	void slotSessionAutoConnect(const char* host,const char* user,const char* passwd,const char* db);

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
	* Test something.
	*/
	void slotSessionTest(void);

	/**
	* Launch the statistique module.
	*/
	void slotSessionStat(void);

	/**
	* Closes all documents and quits the application.
	*/
	void slotSessionQuit(void);

	/**
	* Create a new database with all required tables and fiels + all the value of 'stopkwds' tables.
	*/
	void slotCreateDatabase(void);

	/**
	*  retrieve shell StdOut -------------------------------------------!!
	*/
	void slotStdout(KProcess* proc,char* buffer,int buflen);

	/**
	*  retrieve shell Stderr  -------------------------------------------!!
	*/
	void slotStderr(KProcess* proc,char* buffer,int buflen);

	/**
	*  retrieve the end signal emmited by a shell process.
	*/
	void slotProcessExited(KProcess*);

	
	/**
	* Show the users' window.
	*/
	void slotShowUsers(void);

	/**
	* Compute a profile.
	*/
	void slotProfileCalc(void);

	/**
	* slot to calculate all the profiles
	*/
	void slotProfilesCalc(void);

	/**
	* Show the groups window.
	*/
	void slotShowGroups(void);

	/**
	* Compute the groups.
	*/
	void slotGroupsCalc(void);

	/**
	* Compare the current grouping with a theoritic one given in a file.
	*/
	void slotGroupingCompareFromFile(void);

	/**
	* Compare the current grouping with the grouping in memory.
	*/
	void slotGroupingCompare(void);

	/**
	* evaluate a clustering.
	*/
	void slotGroupsEvaluation(void);

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
	* Compute statistics on all the documents.
	*/
	void slotDocsStats(void);

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
	* Analyse chromosomes store during a GA.
	*/
	void slotGAAnalyse(void);

	/**
	* Save the chromosomes of the actual generation.
	*/
	void slotGASave(void);

	/**
	* Show the chromosomes stored.
	*/
	void slotGAShow(void);

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
	* Run a program text file.
	*/
	void slotRunProgram(void);

	/**
	* Run some instructions.
	*/
	void slotRunInsts(void);

	/**
	* Run some instructions.
	*/
	void slotRunQuery(void);

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

	/**
	* Slot called when a R session must be launched.
	*/
	void slotRRunR(void);

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

	friend class QPluginsDlg;

public:

	// KAction pointers to enable/disable actions
	KToggleAction* sessionAlwaysCalc;
	KAction* sessionConnect;
	KAction* sessionCompute;
	KAction* createDatabase;
	KAction* sessionDisconnect;
	KAction* fillEmptyDb;
	KAction* sessionTest;
	KAction* sessionStat;
	KAction* sessionQuit;
	

	KToggleAction* profileAlwaysCalc;
	KToggleAction* profileAlwaysSave;
	KAction* showUsers;
	KAction* profilesCalc;
	KAction* profileCalc;

	KToggleAction* groupAlwaysCalc;
	KToggleAction* groupAlwaysSave;
	KAction* showGroups;
	KAction* groupsCalc;
	KAction* groupingCompareFromFile;
	KAction* groupingCompare;
	KAction* groupsEvaluation;
	KAction* mixIdealGroups;

	KToggleAction* docAlwaysCalc;
	KAction* showDocs;
	KAction* docAnalyse;
	KAction* docsAnalyse;
	KAction* docsStats;
	KAction* createXML;
	KAction* saveXML;
	KAction* analyseXML;

	KAction* textFrench;
	KAction* textEnglish;

	KAction* gaInit;
	KAction* gaStart;
	KAction* gaPause;
	KAction* gaStop;
	KAction* gaSave;
	KAction* gaAnalyse;
	KAction* gaShow;

	KAction* rRunR;

	KAction* plugins;
	KAction* docsOptions;

	KAction* runProgram;
	KAction* runInsts;
	KAction* runQuery;

	KAction* windowTile;
	KAction* windowCascade;
	KToggleAction* viewToolBar;
	KToggleAction* viewStatusBar;
	KActionMenu* windowMenu;
};


//-----------------------------------------------------------------------------
#endif
