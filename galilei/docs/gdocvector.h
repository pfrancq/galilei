/*

	GALILEI Research Project

	GDocVector.h

	Document in the Vector model - Header.

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
#ifndef GDocVectorH
#define GDocVectorH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <docs/gdoc.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocVector class provides a representation of a document in the vector
* model.
* @author Pascal Francq
* @short Vector Model Document.
*/
class GDocVector : public GDoc, public GIWordsWeights
{
public:

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param title          Name of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param t              MIME type of the document.
	* @param u              String representing the date of the last update.
	* @param a              String representing the date of the last analysis.
	* @param f              Number of fails.
	* @param n              Total number of words.
	* @param ndiff          Number of different words.
	* @param v              Total number of valid words.
	* @param vdiff          Number of different valid words.
	* @param nbf            Number of Feedbacks.
	*/
	GDocVector(const char* url,const char* name,unsigned int id,GLang* lang,GMIMEFilter* t,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf=100) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* @return Identificator of the document.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* This function clears the information related to the document.
	* @param l              Must the language be removed.
	*/
	virtual void ClearInfos(bool l);

	/**
	* Set the information for the document.
	* @param l              Language of the document.
	* @param n              Total number of words.
	* @param nd             Total number of different words.
	* @param v              Total number of valid words.
	* @param vd             Total number of different valid words.
	*/
	virtual void SetInfos(GLang *l,unsigned int n,unsigned int nd,unsigned int v,unsigned int vd);

	/**
	* Add a word with a certain occurences in the document.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	void AddWord(const unsigned int id,const double nb);

	/**
	* Get a Cursor on the weights of the document.
	* @return GWordWeightCursor.
	*/
	GIWordWeightCursor& GetWordWeightCursor(void);

	/**
	* Compute similarity between document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute similarity between document using the idf factors.
	*/
	virtual double GlobalSimilarity(const GDoc* doc) const;

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const;

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const;

	/**
	* Destruct the document.
	*/
	virtual ~GDocVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
