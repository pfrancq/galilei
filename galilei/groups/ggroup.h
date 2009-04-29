/*

	GALILEI Research Project

	GGroup.h

	Generic Group - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gweightinfosobj.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gmeasure.h>
#include <gstorage.h>
#include <gsubject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a generic group of objects represented by GWeightInfos.
* @param cObj                Object contained.
* @param cGroup              Group.
* @param type                Type of the object.
* @author Pascal Francq
* @short Generic Group.
*/
template<class cObj,class cGroup,tObjType type>
	class GGroup : protected R::RContainer<cObj,false,true>, public GWeightInfosObj
{
	using GWeightInfosObj::Id;
	using GWeightInfosObj::CopyInfos;
	using GWeightInfosObj::AddRefs;
	using GWeightInfosObj::DelRefs;
	using GWeightInfosObj::State;

protected:

	/**
	* Date of the update (objects have changed).
	*/
	R::RDate Updated;

	/**
	* Date of last objects computing.
	*/
	R::RDate Computed;

	/**
	 * Data associated to the corresponding ideal subject.
	 */
	GSubjectData<cObj>* Data;

public:

	/**
	* Construct a group with a specific identifier.
	* @param id              Identifier.
	* @param name            Name of the group.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	* @param size           Size of the vector.
	*/
	GGroup(size_t id,const R::RString& name,const R::RDate& u,const R::RDate& c,size_t size);

	/**
	* Compare two groups by comparing their identifier.
	* @see R::RContainer
	* @param grp             Group.
	* @return int
	*/
	int Compare(const GGroup& grp) const;

	/**
	* Compare two groups by comparing their identifier.
	* @see R::RContainer
	* @param grp             Pointer to a group.
	* @return int
	*/
	int Compare(const GGroup* grp) const;

	/**
	* Compare the identifier of a group with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Get the date of the last update of the objects.
	* @returns R::RDate.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Get the date of the last analysis of the objects.
	* @returns R::RDate.
	*/
	R::RDate GetComputed(void) const;

	/**
	 * Get the similarity measure that must be used when computing the
	 * similarity between the objects grouped. An example for profiles:
	 * @code
	 * virtual GMeasure* GetMinMeasure(void)
	 * {
	 * 	   return(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities"));
	 * }
	 * @endcode
	 */
	virtual GMeasure* GetSimMeasure(void) const=0;

	/**
	 * Look if a given object is in the group.
	 * @param obj           Object.
	 */
	bool IsIn(const cObj* obj) const;

	/**
	* Delete an object from the group. If the group is a Group, the
	* method modifies the assignment of the object (the 'Group' pointer).
	* of the object is set to null).
	* @param obj            Object to delete.
	*/
	void DeleteObj(cObj* obj);

	/**
	* Insert a object in the group. If the group is a Group, the
	* method modifies the assignment of the object (the 'Group' pointer).
	* of the object is set to null).
	* @param obj            Object to insert.
	*/
	void InsertObj(cObj* sp);

	/**
	* Insert a object in the group. This signature is needed by a generic
	* k-Means.
	* @param obj            Object to insert.
	* @see R::RGroupingKMeans.
	*/
	void InsertPtr(cObj* obj);

	 /**
	* Delete all objects.
	*/
	void DeleteObjs(void);

	/**
	* Get a cursor over the objects.
	*/
	R::RCursor<cObj> GetObjs(void) const;

	/**
	* Get a cursor over the objects. This signature is needed by a generic
	* k-Means.
	* @see R::RGroupingKMeans.
	*/
	R::RCursor<cObj> GetCursor(void) const;

	/**
	* Get the number of objects in the group.
	*/
	size_t GetNbObjs(void) const;

	/**
	* Compute the relevant object, i.e. the object with the highest
	* average similarity with all the other objects.
	* @returns Pointer to cObj representing the relevant one.
	*/
	cObj* RelevantObj(void) const;

	/**
	* Compute the relevant object, i.e. the object with the highest
	* average similarity with all the other objects.
	* @param avgsim          Variable that will contain the average similarity
	*                        of the objects with the centroid.
	* @returns Pointer to cObj representing the relevant one.
	*/
	cObj* RelevantObj(double& avgsim) const;

	/**
	* Compute the sum of the similarities of a given object to all the
	* others.
	* @param measure         The measure used to compute the similarities.
	* @param obj             Object used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GMeasure* measure,const cObj* obj) const;

	/**
	* Clear the vector representing the group.
	*/
	void Clear(void);

	/**
	* Assign a new description to the group.
	* @param infos           Pointer to the information.
	* @param computed        The update is called after a computation (and not
	*                        after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>& infos);

	/**
	* Compute the number of objects of a given group that are also in a
	* subject.
	* @param subject         Subject.
	*/
	size_t GetNbObjs(const GSubject* subject) const;

	/**
	* This method is call by a object when it was modified.
	* @param obj             Object modified.
	*/
	void HasUpdate(cObj* obj);

	/**
	* Destruct the group.
	*/
	virtual ~GGroup(void);

	friend class GSubjects;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <ggroup.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
