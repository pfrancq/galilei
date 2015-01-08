/*

	GALILEI Project.

	QGALILEIWin.h

	Main windows - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QGALILEIWin_H
#define QGALILEIWin_H


//-----------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMainWindow>


//-----------------------------------------------------------------------------
// include files for current project
#include <ui_qgalileiwin.h>


//-----------------------------------------------------------------------------
// forward declaration
class QGALILEI;
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
 * The QGALILEIWin provides the main window.
 */
class QGALILEIWin : public QMainWindow, public Ui_QGALILEIWin
{
	Q_OBJECT

	/**
	 * GALILEI Application.
	 */
	QGALILEI* App;

	/**
	* GALILEI session.
	*/
	GALILEI::GSession* Session;

	/**
	 * Name of the current session.
	 */
	R::RString SessionName;

	/**
	 * Last tool category used.
	 */
	R::RString ToolCat;

	/**
	 * Last tool used.
	 */
	R::RString Tool;

public:

	/**
	 * Constructor.
    * @param app            Application.
    */
	QGALILEIWin(QGALILEI* app);

	/**
	* Returns a pointer to the current session.
	*/
	GALILEI::GSession* getSession(void) const {return(Session);}

	/**
	 * Get the application.
    * @return a pointer of QGALILEI.
    */
	QGALILEI* GetApp(void) const {return(App);}

	/**
	 * Method that perform an update to every child window of a given class.
    */
	template<class cWin> void updateWins(void)
	{
		QList<QMdiSubWindow*> List(Desktop->subWindowList());
		QListIterator<QMdiSubWindow*> i(List);
		while(i.hasNext())
		{
			cWin* win(dynamic_cast<cWin*>(i.next()));
			if(win)
				 win->update();
		}
	}

private:

	/**
	 * Connect the menu entries with the slots.
    */
	void connectMenus(void);

	/**
	 * Modify the enabled status of the menu entries dependinf if a session is
	 * connected or not.
    */
	void sessionConnected(void);

public slots :

	/**
	* Connect to a session.
	*/
	void sessionConnect(void);

	/**
	 * Disconnect from a session.
	 */
	void sessionDisconnect(void);

	/**
	 * Compute an entire session.
	 */
	void computeSession(void);

	/**
	 * Show a console.
	 */
	void console(void);

	/**
	 * Run a script file.
	 */
	void runScript(void);

	/**
	 * Compute all active statistics.
	 */
	void statistics(void);

	/**
	 * Create a new session.
	 */
	void createSession(void);

	/**
	 * Import documents in the current database.
	 */
	void importDocs(void);

	/**
	 * Run a specific tool.
	 */
	void runTool(void);

	/**
	 * Show the dictionaries.
	 */
	void showDicts(void);

	/**
	 * Show the documents.
	 */
	void showDocs(void);

	/**
	 * Show a document.
	 */
	void showDoc(GDoc* doc);

	/**
	 * Create for each document a corresponding file containing the keywords in
	 * order to index them with ht://Dig.
	 */
	void exportDocDecs(void);

	/**
	* Clear the documents.
	*/
	void clearDocs(void);

	/**
	 * Analyze all the documents.
	 */
	void analyzeDocs(void);

	/**
	 * Index the documents.
	 */
	void indexDocs(void);

	/**
	 * Export the documents.
	 */
	void exportDocs(void);

	/**
	 * Analyze a document.
	 */
	void analyzeDoc(void);

	 /**
	  * Show the classes.
	  */
	 void showClasses(void);

	/**
	 * Show a given class.
	 * @param class      Class.
	 */
	void showClass(GClass* theclass);

	/**
	 * Clear the topics.
	 */
	void clearTopics(void);

	/**
	 * Compute the topics.
	 */
	void groupDocs(void);

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
	void clearProfiles(void);

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
	void computeProfiles(void);

	/**
	 * Compute the current profile.
	 */
	void computeProfile(void);

	/**
	 * Clear the communities.
	 */
	void clearCommunities(void);

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
	void groupProfiles(void);


	/**
	 * Query the meta engine
	 */
	void queryMetaEngine(void);

	/**
	 * Compute the trust.
	 */
	void computeTrust(void);

	/**
	 * Compute the suggestions.
	 */
	void computeSugs(void);

	/**
	 * Repair the subjects.
	 */
	void repairSubjects(void);

	/**
	 * Initialize the simulation by creating the ideal subjects.
	 */
	void initSimulation(void);

	/**
	 * Create ideal topics.
	 */
	void createIdealTopics(void);

	/**
	 * Create ideal topics from classes.
	 */
	void createIdealTopicsClasses(void);

	/**
	 * Create ideal classes.
	 */
	void createIdealClasses(void);

	/**
	 * Create ideal classes.
	 */
	void createIdealClassesDocs(void);

	/**
	 * Create ideal communities.
	 */
	void createIdealCommunities(void);

	/**
	 * Make a feedback cycle.
	 */
	void feedbackCycle(void);

	/**
	 * Make a assessments cycle.
	 */
	void assessmentCycle(void);

	/**
	 * Compare the current communities with the ideal ones.
	 */
	void compareIdealCommunities(void);

	/**
	 * Compare the current topics with the ideal ones.
	 */
	void compareIdealTopics(void);

	/**
	 * Compare the current classes with the ideal ones.
	 */
	void compareIdealClasses(void);

	/**
	 * Set the preferences.
	 */
	void configQGALILEI(void);

	/**
	 * Set the options for the session.
	 */
	void configSession(void);

	/**
	 * Show an about dialog box.
    */
	void about(void);

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
	* Destruct the main window.
	*/
	~QGALILEIWin(void);
};


//-----------------------------------------------------------------------------
#endif

