/*

	GALILEI Research Project

	GGroupVector.h

	Group in the Vector Model - Header.

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
#ifndef GGroupVectorH
#define GGroupVectorH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroup.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupVector provides a representation for a group described as a vector
* corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Vector model group.
*/
class GGroupVector : public GGroup, public GIWordsWeights
{
public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	*/
	GGroupVector(const unsigned int id,GLang* lang) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const;

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const;

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	* Get the vector representing the sub-profile.
	* @returns Pointer to GIWordsWeights.
	*/
	GIWordsWeights* GetVector(void) {return(this);}

	/**
	* Compute similarity between groups.
	*/
	virtual double Similarity(const GGroup* desc) const;

	/**
	* Compute similarity between groups using a global approach.
	*/
	virtual double GlobalSimilarity(const GGroup* desc) const;

	/**
	* Compute similarity between a subprofile and a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute similarity between a subprofile and a document using a global
	* approach.
	*/
	virtual double GlobalSimilarity(const GDoc* doc) const;

	/**
	* Get a cursor over the vector.
	* @return GIWordWeightCursor element.
	*/
	GIWordWeightCursor& GetVectorCursor(void);

	/**
	* Add a new word in the container of words.
	* @param  word          The word who will be aded.
	*/
	void AddWord(GIWordWeight* word);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
