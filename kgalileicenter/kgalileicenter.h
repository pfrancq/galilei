/*

	GALILEI Research Project

	KGALILEICenter.h

	Main Window - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef KGALILEICenterH
#define KGALILEICenterH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>
#include <gsession.h>
#include <gslot.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtCore/QList>
#include <QtGui/QMdiArea>
#include <QtGui/QLabel>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kxmlguiwindow.h>


//-----------------------------------------------------------------------------
/**
 * This class represents the main window.
 * @author Pascal Francq.
 * @short Main Window.
 */
class KGALILEICenter : public KXmlGuiWindow, public GSlot, public GGALILEIApp
{
	Q_OBJECT

	/**
	 * Desktop of the application.
	 */
	QMdiArea* Desktop;

	/**
	 * Label to hold an image representing the status of the database connection.
	 */
	QLabel* Status;

	/**
	* A pointer to the "document" which represents a specific GALILEI System.
	*/
	GSession* Doc;

	/**
	* Debugger.
	*/
	R::RDebugXML* Debug;

	/**
	 * Path for programs.
	 */
	RString PrgPath;

	/**
	 * Menu item specifying if the results must be saved.
	 */
	KToggleAction* aSessionSave;

	/**
	 * Connect to the session.
	 */
	KAction* aSessionConnect;

	/**
	 * All available actions once a session is connected.
	 */
	QList<KAction*> Actions;

public:

	/**
	* Construct the KGALILEICenterApp
	* @param argc            Number of arguments.
	* @param argv            Values of arguments.
	*/
	KGALILEICenter(int argc, char *argv[]);

	/**
	* Returns a pointer to the current session.
	*/
	GSession* getSession(void) const {return(Session);}

private:

	/**
	 * Create an action for a given menu item.
	 * @param title          Title of the menu item.
	 * @param name           Name of the action (as appearing in .rc file).
	 * @param slot           Corresponding slot.
	 * @param icon           Icon associated.
	 * @param key            Shortcut associated.
	 */
	KAction* addAction(const char* title,const char* name,const char* slot,const char* icon=0,const char* key=0);

	/**
	 * Initializes the KActions of the application.
	 */
	void initActions(void);

	/**
	 * Call when the session is connected or not. Basically, it enable/disable
	 * the menu items.
	 * @param connected      Connected?
	 */
	void sessionConnected(bool connected);

	/**
	 * Read the options of a configuration file.
	 */
	void readOptions(void);

	/**
	 * Save the options of a configuration file.
	 */
	void saveOptions(void);


private slots:

	/**
	 * Emit a signal that documents were re-computed.
	 */
	void emitDocsChanged(void);

	/**
	 * Emit a signal that topics were re-computed.
	 */
	void emitTopicsChanged(void);

	/**
	 * Emit a signal that profiles were re-computed.
	 */
	void emitProfilesChanged(void);

	/**
	 * Emit a signal that communities were re-computed.
	 */
	void emitCommunitiesChanged(void);

	/**
	 * Close all documents and quits the application.
	 */
	void sessionQuit(void);

	/**
	* Connect to a MySQL database.
	*/
	void sessionConnect(void);

	/**
	 * Disconnect to a MySQL database.
	 */
	void sessionDisconnect(void);

	/**
	 * Compute an entire session.
	 */
	void sessionCompute(void);

	/**
	 * Ask for a given debugging information.
	 */
	void sessionDebug(void);

	/**
	 * Show a program console.
	 */
	void programConsole(void);

	/**
	 * Create a new database.
	 */
	void createDatabase(void);

	/**
	 * Import documents in the current database.
	 */
	void importDocs(void);

	/**
	 * Run a program text file.
	 */
	void runProgram(void);

	/**
	 * Compute all active statistics.
	 */
	void sessionStats(void);

	/**
	* Changes the status bar contents for the standard label permanently, used
	* to indicate current actions.
	* @param text            Text that is displayed in the status bar.
	*/
	void statusMsg(const QString& text);

	/**
	 * The global save flag was changed.
	 */
	void saveModifier(void);

	/**
	 * Set the preferences.
	 */
	void optionsPreferences(void);

	/**
	 * Show the dictionaries.
	 */
	void showDicts(void);

	/**
	* Clear the documents.
	*/
	void docsClear(void);

	/**
	 * Show the documents.
	 */
	void showDocs(void);

	/**
	 * Export the documents.
	 */
	void exportDocs(void);

	/**
	 * Index the documents.
	 */
	void indexDocs(void);

	/**
	 * Show a document.
	 */
	void showDoc(GDoc* doc);

	/**
	 * Analyze a document.
	 */
	void docAnalyse(void);

	/**
	 * Analyze all the documents.
	 */
	void docsAnalyse(void);

	/**
	 * Post-analyze all the documents.
	 */
	void postDocsAnalyse(void);

	/**
	 * Create for each document a corresponding file containing the keywords in
	 * order to index them with ht://Dig.
	 */
	void docsIndexer(void);

	/**
	 * Create a DocXML structure for the current document.
	 */
	void createXML(void);

	/**
	 * Save a DocXML structure in a document.
	 */
	void saveXML(void);

	/**
	 * Query the meta engine
	 */
	void queryMetaEngine(void);

	/**
	 * Clear the topics.
	 */
	void topicsClear(void);

	/**
	 * Compute the topics.
	 */
	void topicsCalc(void);

	/**
	 * Do a post topics.
	 */
	void postTopicsCalc(void);

	/**
	 * Show the topics.
	 */
	void showTopics(void);

	/**
	 * Show a given topic.
	 * @param topic          Topic.
	 */
	void showTopic(GTopic* topic);

	/**
	 * Clear the profiles.
	 */
	void profilesClear(void);

	/**
	 * Show the users.
	 */
	void showUsers(void);

	/**
	 * Show a given profile.
	 * @param profile        Profile to show.
	 */
	void showProfile(GProfile* profile);

	/**
	 * Compute the profiles.
	 */
	void profilesCalc(void);

	/**
	 * Perform post-profiles methods.
	 */
	void postProfilesCalc(void);

	/**
	 * Compute the current profile.
	 */
	void profileCalc(void);

	/**
	 * Clear the communities.
	 */
	void communitiesClear(void);

	/**
	 * Show the communities.
	 */
	void showCommunities(void);

	/**
	 * Show a given community.
	 * @param community      Community.
	 */
	void showCommunity(GCommunity* community);

	/**
	 * Compute the communities.
	*/
	void communitiesCalc(void);

	/**
	 * Perform post-communities methods.
	 */
	void postCommunities(void);

	/**
	 * Create ideal topics.
	 */
	void topicsCreate(void);

	/**
	 * Create ideal topics from classes.
	 */
	void topicsClassesCreate(void);

	/**
	 * Create ideal classes.
	 */
	void classesCreate(void);

	/**
	 * Create ideal subjects.
	 */
	void subjectsCreate(void);

	/**
	 * Create ideal communities.
	 */
	void communitiesCreate(void);

	/**
	 * Make a feedback cycle.
	 */
	void doFdbks(void);

	/**
	 * Make a assessments cycle.
	 */
	void doAssessments(void);

	/**
	 * Compute the suggestions.
	 */
	void computeSugs(void);

	/**
	 * Run a specific tool.
	 */
	void runTool(void);

	/**
	 * Compare the current communities with the ideal ones.
	 */
	void communitiesCompare(void);

	/**
	 * Compare the current topics with the ideal ones.
	 */
	void topicsCompare(void);

	/**
	 * Compare the current classes with the ideal ones.
	 */
	void classesCompare(void);

	 /**
	  * Show the classes.
	  */
	 void showClasses(void);

	/**
	 * Show a given class.
	 * @param class      Class.
	 */
	void showClass(GClass* theclass);

	 //	/**
	 //	* Show help on program.
	 //	*/
	 //	void slotHelpProgram(void);

signals:

	/**
	 * The documents were re-computed.
	 */
	void docsChanged(void);

	/**
	 * The topics were re-computed.
	 */
	void topicsChanged(void);

	/**
	 * The profiles were re-computed.
	 */
	void profilesChanged(void);

	/**
	 * The communities were re-computed.
	 */
	void communitiesChanged(void);

	/**
	 * The classes were re-computed.
	 */
	void classesChanged(void);

public:

	/**
	* Destruct the application.
	*/
	~KGALILEICenter(void);

	friend class Configure;
};


//-----------------------------------------------------------------------------
#endif
