/*

	GALILEI Research Project

	GWeightInfo.h

	Weighted information entity - Header.

	Copyright 2002-2003 by the Universit� Libre de Bruxelles.

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
* The GWeightInfo class provides a representation for an information entity
* associated with a weight.
* @author Pascal Francq
* @short Weighted Information Entity.
*/
class GWeightInfo : public GInfo
{
protected:

	/**
	* Weight associated wih the information entity.
	*/
	double Weight;

public:

	/**
	* Constructor of a weighed information entity. The weight is suppose to be
	* null.
	* @param id              Identificator of the information entity.
	* @param type            Type of the information entity.
	*/
	GWeightInfo(unsigned int id,GInfoType type=infoWord) throw(std::bad_alloc);

	/**
	* Constructor of a weighed information entity.
	* @param id             Identificator of the information entity.
	* @param w              Weight of the information entity.
	* @param type           Type of the information.
	*/
	GWeightInfo(unsigned int id,double w,GInfoType type=infoWord) throw(std::bad_alloc);

	/**
	* Copy constructor of a weighed information entity.
	* @param w              Pointer to a weighted information entity.
	*/
	GWeightInfo(const GWeightInfo& w) throw(std::bad_alloc);

	/**
	* Copy constructor of a weighed information entity.
	* @param w              Pointer to a weighted information entity.
	*/
	GWeightInfo(const GWeightInfo* w) throw(std::bad_alloc);

	/**
	* Copy constructor of a weighed information entity. The weight is suppose to be
	* null.
	* @param w              Pointer to an information entity.
	*/
	GWeightInfo(const GInfo& w) throw(std::bad_alloc);

	/**
	* Copy constructor of a weighed information entity. The weight is suppose to be
	* null.
	* @param w              Pointer to an information entity.
	*/
	GWeightInfo(const GInfo* w) throw(std::bad_alloc);

	/**
	* Compare two weighted information entities by comparing their identificator.
	* @see R::RContainer
	* @param i               Weighted information entity.
	* @return int
	*/
	int Compare(const GWeightInfo& calc) const;

	/**
	* Compare two weighted information entities by comparing their identificator.
	* @see R::RContainer
	* @param i               Pointer to a weighted information entity.
	* @return int
	*/
	int Compare(const GWeightInfo* calc) const;

	/**
	* Get the weight of the word.
	* @return Weight as double.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of the word.
	* @param w              Weight.
	*/
	void SetWeight(double w);

	/**
	* Assignment operator for weighted information entities.
	* @param i               Weighted information entity.
	*/
	GWeightInfo& operator=(const GWeightInfo& i) throw(std::bad_alloc);

	/**
	* Assignment operator for weighted information entities. The weight is suppose to be
	* null.
	* @param i               Information entity.
	*/
	GWeightInfo& operator=(const GInfo& i) throw(std::bad_alloc);

	/**
	* Add a weight to the current one.
	* @param w              Weight to add.
	*/
	GWeightInfo& operator+=(double w);

	/**
	* Substract a weight to the current one.
	* @param w              Weight to add.
	*/
	GWeightInfo& operator-=(double w);

	/**
	* Compute the transformed weight of a given information entity using a
	* inverse frequency factor of the object type (idf,isf or ivf). The inverse
	* frenquency of an informatio entity is linked to a specific language.
	* @param ObjType        Type of the object.
	* @param lang           Language (information entity space).
	* @param max            Maximal value of the vector containing the word.
	* @return double.
	*/
	double GetQueryWeight(tObjType ObjType,GLang* lang,double max) const throw(GException);

	/**
	* Destructor of a weighted information entity.
	*/
	virtual ~GWeightInfo(void);
};


//------------------------------------------------------------------------------
/**
* The GWordWeightCursor class provides a way to go trough a container of
* weighted information entities.
* @short Weighted Information Entity Cursor.
*/
CLASSCURSOR(GWeightInfoCursor,GWeightInfo,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
