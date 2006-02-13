/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
#ifndef QSessionProgressDlgH
#define QSessionProgressDlgH


//-----------------------------------------------------------------------------
// forward class declaration for GALIEI
#include <gslot.h>
#include <rmysql.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qsemimodal.h>
#include <qthread.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kgalileicenter.h"


//-----------------------------------------------------------------------------
/**
* The QSessionThread provides a generic thread for doing something in the
* session.
*/
class QSessionThread : public QThread
{
protected:
	QSessionProgressDlg* Parent;
	GSession* Session;
public:
	QSessionThread(void);
	void Set(GSession* session,QSessionProgressDlg* parent);
	virtual void run(void);
	virtual void DoIt(void)=0;
};


//-----------------------------------------------------------------------------
/**
* Load a session.
*/
class QLoadSession : public QSessionThread
{

public:
	QLoadSession(void);
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create a new Database.
* @param dbname         The name of the Db.
* @param host           The host of the database.
* @param user           The user of the database.
* @param pass           The password of the database.
* @param UseStopList    Specifies if the StopList must be dumped.
* @param UseUsers       Specifies if the Users list must be dumped.
*/
class QCreateDB : public QSessionThread
{
	RString DbName;
	RString Host;
	RString User;
	RString Pass;
	RString SQLPath;
	bool UseStopList;
	bool UseUsers;

public:
	QCreateDB(RString dbName,RString host,RString user,RString pass,RString path,bool useSL, bool useUsr) : DbName(dbName), Host(host), User(user),Pass(pass),SQLPath(path),UseStopList(useSL),UseUsers(useUsr) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create a new Database.
* @param dbname         The name of the Db.
* @param host           The host of the database.
* @param user           The user of the database.
* @param pass           The password of the database.
* @param UseStopList    Specifies if the StopList must be dumped.
* @param UseUsers       Specifies if the Users list must be dumped.
*/
class QFillDB : public QSessionThread
{
	RString DbName;
	RString Host;
	RString User;
	RString Pass;
	RString CatDirectory;
	int Depth;
	RString ParentName;
	GFilterManager* FilterManager;

	RDb* Db;
	int CurrentDocId;
public:
	QFillDB(RString dbName,RString host,RString user,RString pass,RString catDir,int depth, RString parentName, GFilterManager* mng) : DbName(dbName), Host(host), User(user),Pass(pass),CatDirectory(catDir),Depth(depth),ParentName(parentName), FilterManager(mng),Db(0),CurrentDocId(0) {}
	virtual void DoIt(void);
private:
	int CreateCategory(RString name,int parentId);
	void ParseDocDir(RString path,int parentId, int level);
	void InsertDocument(RString path,int parentId);
};


//-----------------------------------------------------------------------------
/**
* Create a XML structure.
* @param xml            The structure that will hold the result.
* @param doc            The corresponding document.
*/
class QCreateDocXML : public QSessionThread
{
	GDocXML* &XML;
	GDoc* Doc;
public:
	QCreateDocXML(GDocXML* &xml,GDoc* doc) : XML(xml), Doc(doc) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyse a XML structure.
* @param xml            The structure that will hold the result.
* @param doc            The corresponding document.
*/
class QAnalyzeXML : public QSessionThread
{
	GDocXML* &XML;
	GDoc* Doc;
public:
	QAnalyzeXML(GDocXML* &xml,GDoc* doc) : XML(xml), Doc(doc) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analyse all the documents.
*/
class QAnalyzeDocs : public QSessionThread
{
public:
	QAnalyzeDocs(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the profiles.
*/
class QComputeProfiles : public QSessionThread
{
public:
	QComputeProfiles(void) {}
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
	QComputeProfile(GProfile* profile) : Profile(profile) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Groups the profiles.
*/
class QGroupProfiles : public QSessionThread
{
public:
	QGroupProfiles(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Post-Groups.
*/
class QPostGroupProfiles : public QSessionThread
{
public:
	QPostGroupProfiles(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Create the ideal groups.
* @param save           Save modified elements.
*/
class QCreateIdealGroups : public QSessionThread
{
public:
	QCreateIdealGroups(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Make a feedback cycle.
*/
class QMakeFdbks : public QSessionThread
{
public:
	QMakeFdbks(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Make a assessment cycle.
*/
class QMakeAssessments : public QSessionThread
{
public:
	QMakeAssessments(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute all the elements.
* @param modified       Recompute only modified elements or all.
* @param save           Save modified elements.
*/
class QComputeAll : public QSessionThread
{
public:
	QComputeAll(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Analayse next MIME path for MIME types.
* @param path           Path to a KDE desktop files.
* @param xml            XML structure holding the actual mime types.
*/
class QFillMIMETypes : public QSessionThread
{
	const char* Path;
public:
	QFillMIMETypes(const char* path) : Path(path) {}
	virtual void DoIt(void);
private:
	void receiveNextMIMEPath(const char* path,RXMLStruct& xml);
};


//-----------------------------------------------------------------------------
/**
* The QSessionProgressDlg provides a dialog box to show the progress of
* something done on a session.
* @author Pascal Francq
* @short Progress Dialog Box.
*/
class QSessionProgressDlg : public QSemiModal, public GSlot
{
	Q_OBJECT

	QFrame* Line;
	char tmpStr[250];

	/**
	* 'OK' button to close the dialog box. Must be disable when beginning
	* computation and enabled at the end.
	*/
	QPushButton* btnOk;

	/**
	* Label to show the message of the current process.
	*/
	QLabel* txtRem;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Thread running the task.
	*/
	QSessionThread* Task;

	/**
	* Is something running?
	*/
	bool Running;

public:

	static unsigned int NbJobs;
	static QSessionProgressDlg* Main;

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param s              Session from which to load.
	* @param c              Caption of the dialog box.
	* @param cancel         Cancel Button?
	*/
	QSessionProgressDlg(QWidget* parent,GSession* s,const char* c,bool cancel=true);

	/**
	* Run a thread "in" this dialog box.
	* @param task            Task to execute.
	*/
	bool Run(QSessionThread* task);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The traitment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* The traitment for a specific chromosome will begin.
	* @param id             Identificator of the chromosome.
	*/
	virtual void NextChromosome(unsigned int id);

public slots:

	/**
	* Slot when button is pressed.
	*/
	void receiveButton();

public:

	/**
	* Put some text.
	* @param text            Text to show.
	*/
	void PutText(const char* text);

	/**
	* Put the dialog in the beginning state.
	*/
	void Begin(void);

	/**
	* Put the dialog in the final state.
	*/
	void Finish(bool Cancel);

	/**
	* Destructor.
	*/
	virtual ~QSessionProgressDlg(void);
};


//-----------------------------------------------------------------------------
#endif
