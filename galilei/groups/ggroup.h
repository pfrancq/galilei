/*

	GALILEI Research Project

	GGroup.h

	Generic Group - Header.

	Copyright 2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GGroupH
#define GGroupH

//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gweightinfos.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gmeasure.h>
#include <gstorage.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a generic group of objects represented by GWeightInfos.
* @param cObj                Object contained.       
* @author Pascal Francq
* @short Generic Group.
*/
template<class cObj,class cGroup,tObjType type>
	class GGroup : protected R::RContainer<cObj,false,true>, public GWeightInfos
{
protected:

	/**
	* Identificator of the group.
	*/
	unsigned int Id;

	/**
	* Date of the update (objiles have changed).
	*/
	R::RDate Updated;

	/**
	* Date of last objile computing.
	*/
	R::RDate Computed;

public:

	/**
	* Construct a group with a specific identificator.
	* @param id              Identificator.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GGroup(unsigned int id,const R::RDate& u,const R::RDate& c);

	/**
	* Compare two groups by comparing their identificator.
	* @see R::RContainer
	* @param grp             Group.
	* @return int
	*/
	int Compare(const GGroup& grp) const;

	/**
	* Compare two groups by comparing their identificator.
	* @see R::RContainer
	* @param grp             Pointer to a group.
	* @return int
	*/
	int Compare(const GGroup* grp) const;

	/**
	* Compare the idenfiticator of a group with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;
		
	/**
	* Get the date of the last update of the subobjile.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Get the date of the last analysis of the subobjile.
	* @returns R::RDate.
	*/
	R::RDate GetComputed(void) const;

	/**
	* Get the identificator of the group.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id              Identifier.
	*/
	void SetId(unsigned int id);

	/**
	* Load information from the current storage.
	*/
	virtual void LoadInfos(void) const;

	/**
	 * Get the similarity measure that must be used when computing the
	 * similarity between the objects grouped. Un example for profiles:
	 * @code
	 * virtual GMeasure* GetMinMeasure(void)
	 * {
	 * 	   return(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities"));
	 * }
	 * @endcode
	 */
	virtual GMeasure* GetSimMeasure(void) const=0;
	
	/**
	 * Look if a given objile is in the group.
	 * @param obj           Profile.µ
	 */
	bool IsIn(const cObj* obj) const;
	
	/**
	* Delete a objile from the group. If the group is a Group, the
	* method modifies the assignation of the objile (the 'Group' pointer).
	* of the objile is set to null).
	* @param obj            Profile to delete.
	*/
	void DeleteObj(cObj* obj);

	/**
	* Insert a objile in the group. If the group is a Group, the
	* method modifies the assignation of the objile (the 'Group' pointer).
	* of the objile is set to null).
	* @param obj            Profile to insert.
	*/
	void InsertObj(cObj* sp);

	/**
	* Insert a objile in the group. This signature is needed by a generic
	* k-Means.
	* @param obj            Profile to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(cObj* obj);

	 /**
	* Delete all objiles.
	*/
	void DeleteObjs(void);

	/**
	* Get a cursor over the objects.
	*/
	R::RCursor<cObj> GetObjs(void) const;

	/**
	* Get a cursor over the objiles. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	R::RCursor<cObj> GetCursor(void) const;

	/**
	* Get the number of objiles in the group.
	*/
	unsigned int GetNbObjs(void) const;

	/**
	* Compute the relevant objile, i.e. the objile whith the highest
	* average similarity with all the other objiles.
	* @returns Pointer to cObj representing the relevant one.
	*/
	cObj* RelevantObj(void) const;

	/**
	* Compute the sum of the similarities of a given objile to all the
	* others.
	* @param measure         The measure used to compute the similarities.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GMeasure* measure,const cObj* obj) const;

	/**
	* Clear the vector representing the group.
	*/
	void Clear(void);

	/**
	* Update the group by assigning it a set of information.
	* @param infos           Pointer to the information.
	* @param computed        The update is called after a computation (and not
	*                        after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed);

	/**
	* This method is call by a objile when it was modified.
	* @param obj            Profile modified.
	*/
	void HasUpdate(cObj* obj);

	/**
	* Destructor of a group.
	*/
	virtual ~GGroup(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <ggroup.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
