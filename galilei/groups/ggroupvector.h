/*

	GALILEI Research Project

	GGroupVector.h

	Group in the Vector Model - Header.

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
	* Get the vector representing the sub-profile.
	* @returns Pointer to GIWordsWeights.
	*/
	GIWordsWeights* GetVector(void) {return(this);}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
