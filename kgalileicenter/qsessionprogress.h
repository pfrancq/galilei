/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef QSessionProgressDlgH
#define QSessionProgressDlgH


//-----------------------------------------------------------------------------
// forward class declaration for GALIEI
#include <sessions/gslot.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qsemimodal.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>


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
	QSessionProgressDlg(QWidget* parent,GSession* s,const char* c);

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
	void LoadSession(unsigned int cmd,GLangs* langs,GURLManager* umng, GDocAnalyseManager* dmng,GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng,  GPostGroupManager* pgmng) throw(GException,bad_alloc);

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
	*/
	void AnalyseDocs(bool modified=true);

	/**
	* Compute a profile.
	* @param prof           Profile to compute.
	*/
	void ComputeProfile(GProfile* prof);

	/**
	* Compute the profiles.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void ComputeProfiles(bool modified,bool save);

	/**
	* Apply the postgroup treatment.
	*/
	void PostGroupCalc(void);

	/**
	* Groups the profiles.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void GroupProfiles(bool modified,bool save);

	/**
	* Compute all the elements.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void ComputeAll(bool modified,bool save);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc);

	/**
	* The traitment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof);

	/**
	* The traitment for a specific chromosome will begin.
	* @param id             Identificator of the chromosome.
	*/
	virtual void receiveNextChromosome(unsigned int id);

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
