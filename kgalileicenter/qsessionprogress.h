/*

	GALILEI Research Project

	QSessionProgressDlg.h

	Dialog Box to show the progress of the something done on a session - Header.

	(C) 2001 by P. Francq.

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
#include <sessions/gsession.h>
#include <groups/ggrouping.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qsemimodal.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>


//-----------------------------------------------------------------------------
/**
* The QSessionProgressDlg provides a dialog box to show the progress of
* something done on a session.
* @author Pascal Francq
* @short Progress Dialog Box.
*/
class QSessionProgressDlg : public QSemiModal, public GGroupingSignalsReceiver, public GSessionSignalsReceiver
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
	*/
	void LoadSession(unsigned int cmd);

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
	* @param method         Method used to compute.
	*/
	void ComputeProfile(GProfile* prof,GProfileCalc* method);

	/**
	* Compute the profiles.
	* @param method         Method used to compute.
	* @param modified       Recompute only modified elements or all.
	*/
	void ComputeProfiles(GProfileCalc* method,bool modified=true);

	/**
	* Groups the profiles.
	* @param method         Method used to group.
	* @param modified       Recompute only modified elements or all.
	*/
	void GroupProfiles(GGrouping* method,bool modified=true);

	/**
	* Compute all the elements.
	* @param method         Method used to compute.
	* @param methodgrp      Method used to group.
	* @param modified       Recompute only modified elements or all.
	*/
	void ComputeAll(GProfileCalc* method,GGrouping* grpmethod,bool modified=true);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(GLang* lang);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof);

	/**
	* Destructor.
	*/
	virtual ~QSessionProgressDlg(void);
};


//-----------------------------------------------------------------------------
#endif
