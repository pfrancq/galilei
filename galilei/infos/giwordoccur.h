/*

	GALILEI Research Project

	GIWordOccur.h

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
#ifndef GIWordOccurH
#define GIWordOccurH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIWordOccur class provides a representation for the number of occurencies
* of a word in the document.
* @author Pascal Francq.
* @short Word's Occurences Information.
*/
class GIWordOccur : public GIWord
{
protected:

	/**
	* The number of occurences of the word.
	*/
	unsigned int Occurs;

public:

	/**
	* The constructor.
	* @param id             Identificator of the word.
	*/
	GIWordOccur(const unsigned int id);

	/**
	* The constructor.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	GIWordOccur(const unsigned int id,const unsigned int nb);

	/**
	* Compare two information. This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const GIWordOccur* i) const
		{return(Id-i->Id);}

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const GIWordOccur& i) const
		{return(Id-i.Id);}

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const unsigned int id) const
		{return(Id-id);}

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
	* @returns the name of the class.
	*/
	virtual const RString ClassName(void) const;

	/**
	* @returns the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* @returns the number of occurences of the word.
	*/
	unsigned int GetNbOccurs(void) {return(Occurs);}

	/**
	* Increase the number of occurences of the word.
	*/
	void IncOccurs(void) {Occurs++;}

	/**
	* The destructor.
	*/
	virtual ~GIWordOccur(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
