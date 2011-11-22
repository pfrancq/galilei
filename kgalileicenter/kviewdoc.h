/*

	GALILEI Research Project

	KViewDoc.h

	Window for manipulating a specific document - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef KViewDocH
#define KViewDocH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rxmlstruct.h>
#include <gdoc.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kviewdoc.h>
#include <ui_addfdbkdlg.h>
#include <kgalileicenter.h>


//---------------------------------------------------------------------------
/**
* The KViewDoc class represents the window to describe a specific document.
* @author Pascal Francq
* @short Document Window.
*/
class KViewDoc : public QMdiSubWindow, public Ui_KViewDoc
{
	Q_OBJECT

	/**
	 * Application.
	 */
	KGALILEICenter* App;

	/**
	* Document to describe.
	*/
	GDoc* Document;

	/**
	* Delete the document when deleting the window?
	*/
	bool bDelDoc;

public:

	/**
	* Constructor for the view
	* @param app            Application.
	* @param doc            Document to represent.
	*/
	KViewDoc(KGALILEICenter* app,GDoc* doc);

	/**
	* Constructor for the view
	* @param app            Application.
	* @param file           Document to represent.
	* @param mime           Mime Type of the document.
	*/
	KViewDoc(KGALILEICenter* app,const RURI& file,const RString& mime);

private:

	/**
	 * Set up the view.
	 */
	void setUp(void);

public slots:

	/**
	* Add a new feedback.
	*/
	void newFdbk(void);

	/**
	 * Update the view.
	 */
	void update(void);

public:

	/**
	* Analyze a document and show the results.
	*/
	void AnalyzeDoc(void);

	/**
	* Destruct the widget.
	*/
	~KViewDoc(void);
};


//-----------------------------------------------------------------------------
/**
 * Dialog box used to choose a feedback of a given profile of a particular
 * user.
 * @short Adding Feedback
 * @author Pascal Francq.
 */
class AddFdbkDlg : public KDialog, public Ui_AddFdbkDlg
{
	Q_OBJECT

	/**
	 * Current user.
	 */
	GUser* User;

	/**
	 * Current profile.
	 */
	GProfile* Prof;

public:

	/**
	 * Construct the dialog box.
	 * @param parent         Parent wisget.
	 */
	AddFdbkDlg(QWidget* parent);

private:

	/**
	 * Build the list of all profiles for the current user.
	 */
	void FillProfiles(void);

private slots:

	/**
	 * The current user was changed.
	 */
	void slotChangeUser(const QString& string);

	/**
	 * The current profile was changed.
	 * @param string
	 */
	void slotChangeProfile(const QString& string);

	friend class KViewDoc;
};


//-----------------------------------------------------------------------------
#endif
