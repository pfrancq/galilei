/*

	GALILEI Research Project

	GWeightInfo.h

	Information entity representing a word associated with a weight - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GWeightInfoH
#define GWeightInfoH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/ginfo.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWeightInfo class provides a representation for an informaation entity
* representing a word associated with a weight.
* @author Pascal Francq
* @short Weighted Word Information.
*/
class GWeightInfo : public GInfo
{
	/**
	* Weight associated wih the word.
	*/
	double Weight;

public:

	/**
	* Constructor of an information entity representing a weighted word. The
	* weight is suppose to be null.
	* @param id             Identificator of the word.
	*/
	GWeightInfo(const unsigned int id,GInfoType type=infoWord) throw(bad_alloc);

	/**
	* Constructor.
	* @param id             Identificator of the word.
	* @param w              Weight of the word.
	* @param type           Type of the information.
	*/
	GWeightInfo(const unsigned id,double w,GInfoType type=infoWord) throw(bad_alloc);

	/**
	* Constructor.
	* @param w              Pointer to a word weight.
	*/
	GWeightInfo(const GWeightInfo* w) throw(bad_alloc);

	/**
	* Compare method used by R::RContainer.
	*/
	int Compare(const GWeightInfo& calc) const;

	/**
	* Compare method used by R::RContainer.
	*/
	int Compare(const GWeightInfo* calc) const;

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
	* Destructor of an information entity representing a weighted word.
	*/
	virtual ~GWeightInfo(void);

	// friend classes
	friend class GWeightInfos;
};


//------------------------------------------------------------------------------
/**
* The GWordWeightCursor class provides a way to go trough a vector of pairs
* (keyword/weight).
* @short Word Weight Cursor
*/
CLASSCURSOR(GWeightInfoCursor,GWeightInfo,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
