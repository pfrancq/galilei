/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef QSessionProgressDlgH
#define QSessionProgressDlgH


//-----------------------------------------------------------------------------
// forward class declaration for R/GALILEI projects
#include <gslot.h>
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QProgressDialog>


//-----------------------------------------------------------------------------
// forward declaration
class QGALILEIWin;


//-----------------------------------------------------------------------------
/**
* The QSessionProgressDlg provides a generic task for doing something in the
* session.
*/
class QSessionProgress : public QProgressDialog, public GALILEI::GSlot
{
		Q_OBJECT

protected:

	/**
	 * Main Window.
	 */
	QGALILEIWin* Win;

	/**
	 * Constructor.
	 */
	QSessionProgress(QGALILEIWin* win,const QString& title);

public:

	/**
	 * Main run of the thread.
	 * @see DoIt.
	 */
	bool run(void);

private:

	/**
	 * Do the real task. This is the method to overload for child classes.
	 */
	virtual void DoIt(void)=0;

	/**
	* Start a job.
	* @param job             Description of the job.
	*/
	virtual void StartJob(const R::RString& job);

	/**
	* End of the last started job.
	* @param msg             Eventually, a message to print.
	*/
	virtual void EndJob(const R::RString& msg=R::RString::Null);

	/**
	* Method called each time a new language is analyzed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GALILEI::GLang* lang);

	/**
	* The treatment for a specific concept type will begin.
	* @param type            Concept type.
	*/
	virtual void NextConceptType(const GALILEI::GConceptType* type);

	/**
	* The treatment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GALILEI::GDoc* doc);

	/**
	* The treatment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GALILEI::GProfile* prof);

	/**
	* Forward a warning.
	* @param msg             Message.
	*/
	virtual void Warning(const R::RString& msg);

	/**
	* Forward an error.
	* @param msg             Message.
	*/
	virtual void Error(const R::RString& msg);

	/**
	* Forward an alert.
	* @param msg             Message.
	*/
	virtual void Alert(const R::RString& msg);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);

	/**
	* This is used by the caller to interact. Typically, it is used to give the
	* control of the process to the GUI.
	*/
	virtual void Interact(void);
};


//-----------------------------------------------------------------------------
/**
* Create a session.
*/
class QCreateSession : public QSessionProgress
{
	GALILEI::GSession* &Session;
	R::RString Name;
public:
	QCreateSession(QGALILEIWin* win,GALILEI::GSession* &session,const R::RString& name)
		: QSessionProgress(win,"Connect to database '"+R::ToQString(name)+"'"), Session(session), Name(name) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyze a document.
*/
class QAnalyzeDoc : public QSessionProgress
{
	GALILEI::GDoc* Doc;
public:
	QAnalyzeDoc(QGALILEIWin* win,GALILEI::GDoc* doc) : QSessionProgress(win,"Analyze a document"), Doc(doc) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Index the documents.
*/
class QIndexDocs : public QSessionProgress
{
public:
	QIndexDocs(QGALILEIWin* win) : QSessionProgress(win,"Index the documents") {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyze all the documents.
*/
class QAnalyzeDocs : public QSessionProgress
{
public:
	QAnalyzeDocs(QGALILEIWin* win) : QSessionProgress(win,"Analyze Documents") {}
	virtual void DoIt(void);
};


////-----------------------------------------------------------------------------
///**
//* Compute the profiles.
//*/
//class QComputeProfiles : public QSessionProgress
//{
//public:
//	QComputeProfiles(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
//
////-----------------------------------------------------------------------------
///**
//* Compute a given profile.
//*/
//class QComputeProfile : public QSessionProgress
//{
//	GProfile* Profile;
//public:
//	QComputeProfile(QGALILEIWin* win,GProfile* profile) : QSessionProgress(app), Profile(profile) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Groups the profiles.
//*/
//class QGroupProfiles : public QSessionProgress
//{
//public:
//	QGroupProfiles(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
//
////-----------------------------------------------------------------------------
///**
//* Groups the documents.
//*/
//class QGroupDocs : public QSessionProgress
//{
//public:
//	QGroupDocs(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal subjects.
//* @param save           Save modified elements.
//*/
//class QCreateIdealSubjects : public QSessionProgress
//{
//public:
//	QCreateIdealSubjects(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal communities.
//* @param save           Save modified elements.
//*/
//class QCreateIdealCommunities : public QSessionProgress
//{
//public:
//	QCreateIdealCommunities(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal topics.
//*/
//class QCreateIdealTopics : public QSessionProgress
//{
//public:
//	QCreateIdealTopics(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal topics from the classes.
//*/
//class QCreateIdealTopicsFromClasses : public QSessionProgress
//{
//public:
//	QCreateIdealTopicsFromClasses(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal classes.
//*/
//class QCreateIdealClasses : public QSessionProgress
//{
//public:
//	QCreateIdealClasses(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Create the ideal classes using documents.
//*/
//class QCreateIdealDocsClasses : public QSessionProgress
//{
//public:
//	QCreateIdealDocsClasses(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Repair the subjects.
//*/
//class QRepairSubjects : public QSessionProgress
//{
//public:
//	QRepairSubjects(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Make a feedback cycle.
//*/
//class QMakeFdbks : public QSessionProgress
//{
//public:
//	QMakeFdbks(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};
//
//
////-----------------------------------------------------------------------------
///**
//* Make a assessment cycle.
//*/
//class QMakeAssessments : public QSessionProgress
//{
//public:
//	QMakeAssessments(QGALILEIWin* win) : QSessionProgress(app) {}
//	virtual void DoIt(void);
//};


//-----------------------------------------------------------------------------
/**
* Compute the trust.
*/
class QComputeTrust : public QSessionProgress
{
public:
	QComputeTrust(QGALILEIWin* win) : QSessionProgress(win,"Compute Trust") {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the suggestions.
*/
class QComputeSugs : public QSessionProgress
{
public:
	QComputeSugs(QGALILEIWin* win) : QSessionProgress(win,"Compute Suggestions") {}
	virtual void DoIt(void);
};


////-----------------------------------------------------------------------------
///**
//* Run a tool.
//*/
//class QRunTool : public QSessionProgress
//{
//	/**
//	 * Name of the tool.
//	 */
//	RString Tool;
//
//	/**
//	 * List of the tool.
//	 */
//	RString List;
//
//public:
//	QRunTool(QGALILEIWin* win,const RString& tool,const RString& list) : QSessionProgress(app), Tool(tool), List(list) {}
//	virtual void DoIt(void);
//};


//-----------------------------------------------------------------------------
/**
* Compute all the elements.
*/
class QComputeAll : public QSessionProgress
{
public:
	QComputeAll(QGALILEIWin* win) : QSessionProgress(win,"Compute Complete Session") {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
#endif
