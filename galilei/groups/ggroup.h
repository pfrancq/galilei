/*

	GALILEI Research Project

	GGroup.h

	Generic Group - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
// include files for R Libraries Project
#include <ircontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdescriptionobject.h>
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
* @tparam cObj                Object contained.
* @tparam cGroup              Group.
* @tparam type                Type of the object.
* @author Pascal Francq
* @short Generic Group.
*/
template<class cObj,class cGroup,tObjType type>
	class GGroup : protected R::RContainer<cObj,false,true>, public GDescriptionObject<cGroup>
{
public:
	using GDescriptionObject<cGroup>::Clear;
	using GDescriptionObject<cGroup>::Id;
	using GDescriptionObject<cGroup>::Copy;
	using GDescriptionObject<cGroup>::State;
	using GDescriptionObject<cGroup>::Session;

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
	* @param session         Session.
	* @param id              Identifier.
	* @param blockid         Identifier of the block.
	* @param name            Name of the group.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GGroup(GSession* session,size_t id,size_t blockid,const R::RString& name,const R::RDate& u,const R::RDate& c);

	/**
	 * Get the data associated to the corresponding ideal subject.
	 * @return
	 */
	GSubjectData<cObj>* GetData(void) const;

	/**
	* Compare two groups by comparing their identifier.
	* @see R::RContainer
	* @param grp             Group.
	* @return int
	*/
	int Compare(const GGroup& grp) const;

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
	void InsertObj(cObj* obj);

	/**
	* Insert a set of  objects in the group. If the group is a Group, the
	* method modifies the assignment of the object (the 'Group' pointer).
	* of the object is set to null).
	* @param objs           Objects to insert.
	*/
	void InsertObjs(R::RCursor<cObj> objs);

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
	* @param disk            Clear the reference to the corresponding block on
	*                        disk.
	*/
	void Clear(bool disk);

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
	friend class GSession;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <ggroup.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
