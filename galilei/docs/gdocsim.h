/*

	GALILEI Research Project

	GDocSim.h

	Similarity of a document - Header.

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
#ifndef GDocSimH
#define GDocSimH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfDocRef class provides a representation for a similarity between a 
* a document.
* @author Pascal Francq
* @short Document Similarity.
*/
//-----------------------------------------------------------------------------
class GDocSim
{
	/**
	*Document.
	*/
	GDoc* Doc;

	/**
	* Similarity.
	*/
	double Sim;

public:

	/**
	* Constructor.
	* @param d               Document.
	* @param s               Similariy.
	*/
	GDocSim(GDoc* d,double s);

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GDocSim* d) const;

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GDocSim& d) const;

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GDoc* d) const;

	/**
	* Get the document.
	* @returns Pointer to the document.
	*/
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	* Get the similarity.
	* @returns double.
	*/
	double GetSim(void) const {return(Sim);}
};


//-----------------------------------------------------------------------------
/**
* The GDocSim class provides a way to go trough a set of similarity of
* documents.
* @short Document Similarities Cursor
*/
CLASSCURSOR(GDocSimCursor,GDocSim,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
