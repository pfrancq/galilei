/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <memory>
using namespace std;


//-----------------------------------------------------------------------------
// forward class declaration for GALIEI
#include <gslot.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtCore/QThread>
#include <kprogressdialog.h>
#include <kgalileicenter.h>


//-----------------------------------------------------------------------------
// include files for current application
class QSessionProgressDlg;


//-----------------------------------------------------------------------------
/**
* The QSessionThread provides a generic thread for doing something in the
* session.
*/
class QSessionThread : public QThread
{
	Q_OBJECT

protected:

	/**
	 * Parent widget.
	 */
	QSessionProgressDlg* Parent;

	/**
	 * Application.
	 */
	KGALILEICenter* App;

	/**
	 * Constructor.
	 */
	QSessionThread(KGALILEICenter* app);

	/**
	 * Set the parent widget.
	 * @param parent         Parent widget.
	 */
	void Set(QSessionProgressDlg* parent);

	/**
	 * Main run of the thread.
	 * @see DoIt.
	 */
	virtual void run(void);

	/**
	 * Do the real task. This is the method to overload for child classes.
	 */
	virtual void DoIt(void)=0;

signals:

	/**
	 * The signal is emitted when the job was done without any errors.
	 */
	void finish(void);

	friend class QSessionProgressDlg;
};


//-----------------------------------------------------------------------------
/**
* Create a session.
*/
class QCreateSession : public QSessionThread
{
	GSession* &Session;
	RString Name;
public:
	QCreateSession(KGALILEICenter* app,GSession* &session,const RString& name) : QSessionThread(app), Session(session), Name(name) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyze a document.
*/
class QAnalyzeDoc : public QSessionThread
{
	GDoc* Doc;
public:
	QAnalyzeDoc(KGALILEICenter* app,GDoc* doc) : QSessionThread(app), Doc(doc) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyze all the documents.
*/
class QAnalyzeDocs : public QSessionThread
{
public:
	QAnalyzeDocs(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the profiles.
*/
class QComputeProfiles : public QSessionThread
{
public:
	QComputeProfiles(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};



//-----------------------------------------------------------------------------
/**
* Compute a given profile.
*/
class QComputeProfile : public QSessionThread
{
	GProfile* Profile;
public:
	QComputeProfile(KGALILEICenter* app,GProfile* profile) : QSessionThread(app), Profile(profile) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Groups the profiles.
*/
class QGroupProfiles : public QSessionThread
{
public:
	QGroupProfiles(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};



//-----------------------------------------------------------------------------
/**
* Groups the documents.
*/
class QGroupDocs : public QSessionThread
{
public:
	QGroupDocs(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal subjects.
* @param save           Save modified elements.
*/
class QCreateIdealSubjects : public QSessionThread
{
public:
	QCreateIdealSubjects(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal communities.
* @param save           Save modified elements.
*/
class QCreateIdealCommunities : public QSessionThread
{
public:
	QCreateIdealCommunities(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal topics.
*/
class QCreateIdealTopics : public QSessionThread
{
public:
	QCreateIdealTopics(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal topics from the classes.
*/
class QCreateIdealTopicsFromClasses : public QSessionThread
{
public:
	QCreateIdealTopicsFromClasses(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal classes.
*/
class QCreateIdealClasses : public QSessionThread
{
public:
	QCreateIdealClasses(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal classes using documents.
*/
class QCreateIdealDocsClasses : public QSessionThread
{
public:
	QCreateIdealDocsClasses(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Test the subjects.
*/
class QTestSubjects : public QSessionThread
{
public:
	QTestSubjects(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Make a feedback cycle.
*/
class QMakeFdbks : public QSessionThread
{
public:
	QMakeFdbks(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Make a assessment cycle.
*/
class QMakeAssessments : public QSessionThread
{
public:
	QMakeAssessments(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the trust.
*/
class QComputeTrust : public QSessionThread
{
public:
	QComputeTrust(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the suggestions.
*/
class QComputeSugs : public QSessionThread
{
public:
	QComputeSugs(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Run a tool.
*/
class QRunTool : public QSessionThread
{
	/**
	 * Name of the tool.
	 */
	RString Tool;

	/**
	 * List of the tool.
	 */
	RString List;

public:
	QRunTool(KGALILEICenter* app,const RString& tool,const RString& list) : QSessionThread(app), Tool(tool), List(list) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute all the elements.
*/
class QComputeAll : public QSessionThread
{
public:
	QComputeAll(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Index the documents.
*/
class QIndexDocs : public QSessionThread
{
public:
	QIndexDocs(KGALILEICenter* app) : QSessionThread(app) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* The QSessionProgressDlg provides a dialog box to show the progress of
* something done on a session.
* @author Pascal Francq
* @short Progress Dialog Box.
*/
class QSessionProgressDlg : public KProgressDialog, public GSlot
{
	Q_OBJECT

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Is something running?
	*/
	bool Running;

	/**
	 * Return results.
	 */
	int Ret;

	/**
	 * Application.
	 */
	KGALILEICenter* App;

	/**
	 * End message.
	 */
	QString EndMsg;

public:

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param c              Caption of the dialog box.
	* @param cancel         Cancel Button?
	*/
	QSessionProgressDlg(KGALILEICenter* parent,const QString& c,bool cancel=true);

	/**
	* Run a thread "in" this dialog box.
	* @param task            Task to execute.
	*/
	bool Run(QSessionThread* task);

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
	virtual void NextGroupLang(const GLang* lang);

	/**
	* The treatment for a specific concept type will begin.
	* @param type            Concept type.
	*/
	virtual void NextConceptType(const GConceptType* type);

	/**
	* The treatment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

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

	/**
	 * The 'Cancel' Button was called.
	 */
	virtual void reject(void);

public slots:

	/**
	* Put the dialog in the final state.
	*/
	void Finish(void);

public:

	/**
	* Destruct.
	*/
	virtual ~QSessionProgressDlg(void);

	friend class QSessionThread;
};


//-----------------------------------------------------------------------------
#endif
