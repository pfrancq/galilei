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
#include <sessions/gslot.h>
#include <rdb/rmysql.h>
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
* @param cmd            What to load.
* @param langs          Languages.
* @param umng           URL Manager.
* @param dmng           Document Analyse Manager.
* @param pmng           Profiling Manager.
* @param gmng           Grouping Manager.
* @param gcmng          Group Computing Manager.
* @param smng           Statistical Manager.
* @param lmng           Linking Manager.
* @param emng           Engine Manager.
*/
class QLoadSession : public QSessionThread
{
	GLangManager* Langs;
	GFilterManager* Umng;
	GDocAnalyseManager* Dmng;
	GProfileCalcManager* Pmng;
	GGroupingManager* Gmng;
	GGroupCalcManager* GCmng;
	GStatsCalcManager* Smng;
	GLinkCalcManager* Lmng;
	GPostDocManager* PDmng;
	GPostGroupManager* PGmng;
	GEngineManager* Emng;
public:
	QLoadSession(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng,GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng, GPostGroupManager* pgmng, GEngineManager* emng);
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
	GFilterManagerKDE* FilterManager;
	
	RDb* Db;
	int CurrentDocId;
public:
	QFillDB(RString dbName,RString host,RString user,RString pass,RString catDir, GFilterManagerKDE* mng) : DbName(dbName), Host(host), User(user),Pass(pass),CatDirectory(catDir), FilterManager(mng),Db(0),CurrentDocId(0) {}
	virtual void DoIt(void);
private:
	int CreateCategory(RString name,int parentId);
	void ParseDocDir(RString path,int parentId, int level);
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
* @param modified   Recompute only modified elements or all.
* @param save       Save the computed elements.
*/
class QAnalyzeDocs : public QSessionThread
{
	bool Modified;
	bool Save;
public:
	QAnalyzeDocs(bool modified=true,bool save=true) : Modified(modified), Save(save) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Compute the profiles.
* @param modified       Recompute only modified elements or all.
* @param save           Save modified elements.
* @param saveLinks      Save modified links informations
*/
class QComputeProfiles : public QSessionThread
{
	bool Modified;
	bool Save;
	bool SaveLinks;
public:
	QComputeProfiles(bool modified,bool save,bool saveLinks) : Modified(modified), Save(save), SaveLinks(saveLinks) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Groups the profiles.
* @param modified       Recompute only modified elements or all.
* @param save           Save modified elements.
*/
class QGroupProfiles : public QSessionThread
{
	bool Modified;
	bool Save;
	bool SaveHistory;
public:
	QGroupProfiles(bool modified,bool save, bool savehistory) : Modified(modified), Save(save), SaveHistory(savehistory) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Run the Post-clustering algorithm.
*/
class QComputePostGroup : public QSessionThread
{
public:
	QComputePostGroup(void) {}
	virtual void DoIt(void);
};


//-----------------------------------------------------------------------------
/**
* Export the documents/words matrix in R.
* @param type           type of export ("Profiles", "Documents" or "Groups")
* @param filrname       export file name
* @param lang           lang of the export
* @param label          display words id aned vectors id ?
*/
class QExportMatrix : public QSessionThread
{
	const char* Type;
	const char* Name;
	GLang* Lang;
	bool Label;
public:
	QExportMatrix(const char* type, const char* filename, GLang* lang, bool label)
		: Type(type), Name(filename), Lang(lang), Label(label) {}
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
	bool Modified;
	bool Save;
	bool SaveLinks;
	bool SaveHistory;
public:
	QComputeAll(bool modified,bool save,bool saveLinks, bool savehistory)
		: Modified(modified), Save(save), SaveLinks(saveLinks), SaveHistory(savehistory) {}
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

public:

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param s              Session from which to load.
	* @param c              Caption of the dialog box.
	*/
	QSessionProgressDlg(QWidget* parent,GSession* s,const char* c) throw(std::bad_alloc,RException);

	/**
	* Run a thread "in" this dialog box.
	* @param task            Task to execute.
	*/
	bool Run(QSessionThread* task);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang) throw(std::bad_alloc,RException);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc) throw(std::bad_alloc,RException);

	/**
	* The traitment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof) throw(std::bad_alloc,RException);

	/**
	* The export for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfileExport(const GProfile* prof) throw(std::bad_alloc,RException);

	/**
	* The export for a specific group will begin.
	* @param grp           Group.
	*/
	virtual void receiveNextGroupExport(const GGroup* grp) throw(std::bad_alloc,RException);

	/**
	* The export for a specific document will begin.
	* @param doc           Document
	*/
	virtual void receiveNextDocumentExport(const GDoc* doc) throw(std::bad_alloc,RException);

	/**
	* The traitment for a specific chromosome will begin.
	* @param id             Identificator of the chromosome.
	*/
	virtual void receiveNextChromosome(unsigned int id) throw(std::bad_alloc,RException);

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
