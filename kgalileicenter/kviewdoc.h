/*

	GALILEI Research Project

	KViewDoc.h

	Window for manipulating a specific document - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#ifndef KViewDocH
#define KViewDocH


//-----------------------------------------------------------------------------
// forward class declaration for GALILEI
namespace R
{
	class RXMLStruct;
}
using namespace R;
namespace GALILEI
{
	class GDoc;
	class GDocXML;
	class QGDocXML;
}
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qtabwidget.h>


//---------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//---------------------------------------------------------------------------
/**
* The KViewDoc class represents the window to describe a specific document.
* @author Pascal Francq
* @short Document Window.
*/
class KViewDoc : public KView
{
	Q_OBJECT

	/**
	* Document to describe.
	*/
	GDoc* Document;

	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Feedback of the profiles on the document.
	*/
	QListView* Fdbks;

	/**
	* Feedback of the profiles on the document if from link analysis.
	*/
	QListView* FdbksLinks;

	/**
	* Results of the analyse.
	*/
	QListView* Results;

	/**
	* General information about the document.
	*/
	QListView* General;

	/**
	* Listview item containing the MIME type of the file.
	*/
	QListViewItem* InfoMIME;

	/**
	* Widget to show XML structure representing the content of the document.
	*/
	QGDocXML* XML;

	/**
	* XML structure representing the content of the document.
	*/
	RXMLStruct* Struct;

	/**
	* Delete the Document when deleting the window?
	*/
	bool bDelDoc;

	/**
	* DocXML Computed?
	*/
	bool bDocXML;

public:

	/**
	* Constructor for the view
	* @param document       Document to represent.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewDoc(GDoc* document,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Constructor for the view
	* @param file           Document to represent.
	* @param mime           Mime Type of the document.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewDoc(const char* file,const char* mime,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gDoc);}

	/**
	* Construct the feedbacks' widget.
	*/
	void ConstructFdbks(void);

	/**
	* Construct the results' widget.
	*/
	void ConstructResults(void);

	/**
	* Construct the general information's widget.
	*/
	void ConstructGeneral(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	virtual void resizeEvent(QResizeEvent *);

public:

	/**
	* Get the doc of this window.
	* @return Pointer to a GALILEI::GDoc.
	*/
	GDoc* GetDoc(void) const {return(Document);}

	/**
	* Look if the DocXML is already been computed for
	*/
	bool IsDocXML(void) const {return(bDocXML);}

	/**
	* Create a GDocXML structure for the current document and show it in the
	* corresponding tab.
	*/
	void CreateDocXML(void);

	/**
	* Save the GDocXML structure for the current document.
	* @param name           File where to store the file.
	*/
	void SaveDocXML(const char* name);

	/**
	* Analyse a GDocXML structure of the current document and show the results.
	*/
	void AnalyseDocXML(void);

	/**
	* Destructor.
	*/
	~KViewDoc(void);
};


//-----------------------------------------------------------------------------
#endif
