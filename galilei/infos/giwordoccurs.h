/*

	GALILEI Research Project

	GIWordOccurs.h

	Word Occurences Information - Header.

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
#ifndef GIWordOccursH
#define GIWordOccursH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/giword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GInfo class provides a representation for a basic information entity.
* @author Pascal Francq.
* @short Basic Information.
*/
class GIWordOccurs : public GALILEI::GIWord
{
protected:

	/**
	* The number of occurences of the word.
	*/
	unsigned int Occurs;

public:

	/**
	* The constructor.
	*/
	GIWordOccurs(unsigned int id);

	/**
	* Compare two information. This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Compute a similarity between two information objects. If the two objects
	* are the same, the similarity must be 1 and if there is nothing in common,
	* the similarity must be 0.
	* @return a value between 0 and 1.
	*/
	virtual float Similarity(const GInfo* i) const;

	/**
	* Compute a dissimilarity between two information objects. If the two
	* objects are the same, the dissimilarity must be 0 and if there is nothing
	* in common, the similarity must be 1.
	* @return a value between 0 and 1.
	*/
	virtual float DisSimilarity(const GInfo* i) const;

	/**
	* Return the name of the class.
	*/
	virtual const RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* The destructor.
	*/
	virtual ~GIWordOccurs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
