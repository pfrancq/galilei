/*

	GALILEI Research Project

	KViewStem.h

	Basic Window for the application - Header.

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
#ifndef KViewStemH
#define KViewStemH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rdblhashcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
namespace GALILEI
{
	class GLang;
};


//-----------------------------------------------------------------------------
// include files for Qt
class QListView;
class QTabWidget;
class QComboBox;
class QLabel;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KViewStat class provides the view widget for statistics about a set of
* documents.
* @author Pascal Francq.
* @version $Revision$
* @short Window for Statistics.
*/
class KViewStems : public KView
{
	class Word;
	class GrWord;
	class GrWordId;

	Q_OBJECT

	/**
	* Label of the indexes.
	*/
	QLabel* Indexes;

	/**
	* Combobox representing the index on the first hash table.
	*/
	QComboBox* Index;

	/**
	* Combobox representing the index on the second hash table.
	*/
	QComboBox* Index2;

	/**
	* Widget to handle the different information of the document.
	*/
	QTabWidget* Infos;

	/**
	* Theoritical Groups of words (Roots).
	*/
	QListView* thGroups;

	/**
	* Pratical Groups of words (Stems).
	*/
	QListView* prGroups;

	/**
	* Language to use.
	*/
	GALILEI::GLang* Lang;

	/**
	* Roots used in the dictionnary.
	*/
	RDblHashContainer<GrWord,unsigned,27,27,true>* Roots;

	/**
	* Computed Stems for the dictionnary.
	*/
	RDblHashContainer<GrWord,unsigned,27,27,true>* Stems;

	/**
	* Words defined in the dictionnary.
	*/
	RDblHashContainer<Word,unsigned,27,27,true>* Words;

	/**
	* Precision of the stemming.
	*/
	double Precision;

	/**
	* Recall of the stemming.
	*/
	double Recall;

	/**
	* Total index between for stemming.
	*/
	double Total;

	/**
	* Compute the number of words of grp2 that are in grp1.
	* @param grp1           First Group.
	* @param grp2           Second Group.
	*/
	unsigned int GetNbWords(GrWord* grp1,GrWord* grp2);

	/**
	* Compute the Recall and the Precision.
	*/
	void ComputeRecallPrecision(void);

	/**
	* Compute the Total based on the Adjusted Rand Index (Hubert and Arabie).
	*/
	void ComputeTotal(void);

public:

	/**
	* Constructor for the view.
	* @param code           Code of the language to use.
	* @param filename       Filename of the MORPH file.
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KViewStems(const char* code,const char* filename,KDoc* doc,QWidget* parent,const char* name,int wflags);

	/**
	* Return the type of the window.
	*/
	virtual GViewType getType(void) {return(gStems);}

	/**
	* Gets called to redraw the document contents if it has been modified.
	* @param cmd            Specify why? (0=Docs,1=Users,2=Groups)
	*/
	virtual void update(unsigned int cmd);

protected:

	/**
	* Load the filename.
	* @param filename       Name of the file to load.
	*/
	void LoadFile(const char* filename);

	/**
	* Make the comparison by computing the values.
	*/
	void Compute(void);

	/**
	* Construct the theoritical part of the stems.
	* @param index          Index of the first hash table to show.
	* @param index2         Index of the second hash table to show.
	*/
	void ConstructTh(char index,char index2);

	/**
	* Construct the pratical part of the stems.
	* @param index          Index of the first hash table to show.
	* @param index2         Index of the second hash table to show.
	*/
	void ConstructPr(char index,char index2);

	/**
	* Called when the main window is resize by the user.
	*/
	void resizeEvent(QResizeEvent *);

protected slots:

	/**
	* Slot called when the index of the first hash table is changed.
	*/
	void slotIndex(int index);

	/**
	* Slot called when the index of the second hash table is changed.
	*/
	void slotIndex2(int index);

public:

	/**
	* Destructor for the view.
	*/
	~KViewStems(void);
};


//-----------------------------------------------------------------------------
#endif
