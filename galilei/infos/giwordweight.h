/*

	GALILEI Research Project

	GIWordWeight.h

	Weight of a word - Header.

	(C) 2002 by P. Francq.

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
#ifndef GIWordWeightH
#define GIWordWeightH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <infos/giword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GIWordWeight class provides a representation for a weight of a word in a
* given collection of words, for example a document.
* @author Pascal Francq
* @short Word Weight.
*/
class GIWordWeight : public GIWord
{
	/**
	* Frequence of the word.
	*/
	double Weight;

public:

	/**
	* Constructor.
	* @param id             Identificator of the word.
	*/
	GIWordWeight(const unsigned int id) throw(bad_alloc);

	/**
	* Constructor.
	* @param id             Identificator of the word.
	* @param w              Weight of the word.
	*/
	GIWordWeight(const unsigned int id,double w) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordWeight &calc) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordWeight *calc) const;

	/**
	* Get the weight of the word.
	* @return Weight as double.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Get the weight of the word with the query expression for a specific
	* dictionnary.
	* @param ObjType        Type of the reference.
	* @param dict           Dictionnary.
	* @param max            Maximal value of the vector containing the word.
	* @return double.
	*/
	double GetQueryWeight(tObjType ObjType,GDict* dict,double max) const;

	/**
	* Set the weight of the word.
	* @param w              Weight.
	*/
	void SetWeight(double w) {Weight=w;}

	/**
	* Add a weight to the current one.
	* @param w              Weight to add.
	*/
	void AddWeight(double w) {Weight+=w;}

	/**
	* Substract a weight to the current one.
	* @param w              Weight to add.
	*/
	void SubstractWeight(double w) {Weight-=w;}

	/**
	* Destructor.
	*/
	virtual ~GIWordWeight(void);
};


//-----------------------------------------------------------------------------
/**
* The GWordWeightCursor class provides a way to go trough a vector of pairs
* (keyword/weight).
* @short Word Weight Cursor
*/
CLASSCURSOR(GIWordWeightCursor,GIWordWeight,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
