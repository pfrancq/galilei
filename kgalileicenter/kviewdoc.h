/*

	GALILEI Research Project

	KViewDoc.h

	Window for manipulating a specific document - Header.

	(C) 2001 by Pascal Francq and David Wartel

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef KViewDocH
#define KViewDocH


//-----------------------------------------------------------------------------
// forward class declaration for GALILEI
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
* @author Pascal Francq and David Wartel
* @version $Revision$
* @short Document's Window.
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
	* Results of the analyse.
	*/
	QListView* Results;

	/**
	* General information about the document.
	*/
	QListView* General;

	/**
	* Widget to show XML structure representing the content of the document.
	*/
	QGDocXML* XML;

	/**
	* XML structure representing the content of the document.
	*/
	GDocXML* Struct;

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
	* Create a GDocXML structure for the current document and show it in the
	* corresponding tab.
	*/
	void CreateDocXML(void);

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
