/*

	GALILEI Research Project

	KViewEvaluateGroup.h

	Window to see differents statistique for the ideal group of profile and document.

	(C) 2001 by Julien lamoral

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
#ifndef KViewEvaluateGroupH
#define KViewEvaluateGroupH



//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
// forward class declaration
class QTabWidget;
class QListView;


//-----------------------------------------------------------------------------
/**
* The KViewProfile class provides a window to see similarity statistique.
* @author Lamoral Julien
* @short Similarity statistique Window.
*/
class KViewEvaluateGroup : public KView
{
	Q_OBJECT


	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Widget to handle info about the statistique for the ideal group
	* of profiles and documents.
	*/
	QListView* General;

	/**
	* Widget to handle info about the varinace for the ideal group
	* of profiles .
	*/
	QListView* VarianceSubProf;

	/**
	* Widget to handle info about the varinace for the ideal group
	* of  documents.
	*/
	QListView* VarianceDoc;

	/**
	* Widget to handle info about the similarity for the ideal group
	* of  documents.
	*/
	QListView* SimilarityDoc;

	/**
	* Widget to handle info about the similarity for the ideal group
	* of  profiles.
	*/
	QListView* SimilaritySubProf;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewEvaluateGroup(KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gProfile);}

	/**
	* Construct the General widget.
	*/
	void ConstructGeneral(void);

	/**
	* Construct the Doc Smilarity widget.
	*/
	void ConstructSimilarityDoc(void);
	
	/**
	* Construct the subprofile similarity widget.
	*/
	void ConstructSimilaritySubProf(void);

	/**
	* Construct the variance for subprofiles widget.
	*/
	void ConstructVarianceSubProf(void);

	/**
	* Construct the variance for document widget.
	*/
	void ConstructVarianceDoc(void);

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int );

protected:

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

public:

	/**
	* Destructor for the main view.
	*/
	~KViewEvaluateGroup(void);
};


//-----------------------------------------------------------------------------
#endif
