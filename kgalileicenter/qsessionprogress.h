/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qsemimodal.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kgalileicenter.h"


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

public:

	/**
	* Constructor.
	* @param parent         Parent widget.
	* @param s              Session from which to load.
	* @param c              Caption of the dialog box.
	*/
	QSessionProgressDlg(QWidget* parent,GSession* s,const char* c) throw(std::bad_alloc,RException);

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
	*/
	void LoadSession(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng,GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng,  GPostGroupManager* pgmng) throw(GException,std::bad_alloc);

	/**
	* Create a XML structure.
	* @param xml            The structure that will hold the result.
	* @param doc            The corresponding document.
	*/
	void CreateDocXML(GDocXML* &xml,GDoc* doc);

	/**
	* Analyse a XML structure.
	* @param xml            The structure that will hold the result.
	* @param doc            The corresponding document.
	*/
	void AnalyseXML(GDocXML* &xml,GDoc* doc);

	/**
	* Analyse all the documents.
	* @param modified   Recompute only modified elements or all.
	* @param save       Save the computed elements.
	*/
	void AnalyseDocs(bool modified=true,bool save=true);

	/**
	* Compute the profiles.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	* @param saveLinks      Save modified links informations
	*/
	void ComputeProfiles(bool modified,bool save,bool saveLinks);

	/**
	* Groups the profiles.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void GroupProfiles(bool modified,bool save, bool savehistory);

	/**
	* Run the Post-clustering algorithm.
	*/
	void ComputePostGroup(void);

	/**
	* export the doucuments/words matrix
	* @param type           type of export ("Profiles", "Documents" or "Groups")
	* @param filrname       export file name
	* @param lang           lang of the export
	* @param label          display words id aned vectors id ?
	*/
	void ExportMatrix(const char* type, const char* filename, GLang* lang, bool label);

	/**
	* Compute all the elements.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void ComputeAll(bool modified,bool save,bool saveLinks, bool savehistory);

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

private:

	/**
	* Analayse next MIME path for MIME types.
	* @param path           Path to a KDE desktop files.
	* @param xml            XML structure holding the actual mime types.
	*/
	void receiveNextMIMEPath(const char* path,RXMLStruct& xml);

public:

	/**
	* Re-creates a XML file using the KDE MIME types.
	* @param path           Path to the root KDE desktop files.
	*/
	void FillMIMETypes(const char* path);

	/**
	* Put some text.
	* @param text           Text to show.
	*/
	void PutText(const char* text);

	/**
	* Begin.
	*/
	void Begin(void);

	/**
	* Finish.
	*/
	void Finish(void);

	/**
	* Destructor.
	*/
	virtual ~QSessionProgressDlg(void);
};


//-----------------------------------------------------------------------------
#endif
