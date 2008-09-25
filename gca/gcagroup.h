/*

	Genetic Community Algorithm

	GCAGroup.h

	Group - Header.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GCAGroupH
#define GCAGroupH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroups.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>
#include <gcaobj.h>
#include <gcachromo.h>
#include <gcainst.h>


//-----------------------------------------------------------------------------
/**
* The RGroupBP provides a representation for a group for the IR Problem.
* @author Pascal Francq
* @short IR Group.
*/
class GCAGroup : public R::RGroup<GCAGroup,GCAObj,GCAChromo>
{
protected:

	/**
	* Average similarity between each object and the centroid.
	*/
	double AvgIntraSim;

	/**
	 * Average agreement ratio between the objects of the group.
	 */
	double AvgAgreement;

	/**
	 * Average disagreement ratio between the object of the group.
	 */
	double AvgDisagreement;

	/**
	* Centroid of the group.
	*/
	GCAObj* Centroid;

	/**
	 * Must the group be re-evaluated?
	 */
	bool ToEval;

	/**
	 * Maximum similarity with the last object called by CanInsert.
	 */
	double LastMaxSim;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GCAGroup(GCAGroup* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	* @param data           Data needed for the group.
	*/
	GCAGroup(GCAChromo* owner,const size_t id);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Verify if an object has the same parent as another object of the group.
	* @param obj            Pointer to the object to insert.
	*/
	bool HasSameUser(const GCAObj* obj) const;

	/**
	* Look if an object can be insert in the group. In practice, the object
	* must have a minimum similarity and maximum disagreement with the objects
	* of the group. Moreover, two objects having the same parents cannot be
	* grouped together.
	* not allowed.
	* @param obj             Object to insert.
	*/
	virtual bool CanInsert(const GCAObj* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj             Object to insert.
	*/
	virtual void PostInsert(const GCAObj* obj);

	/**
	* Method call after an object was deleted from the group.
	* @param obj             Object to delete.
	*/
	virtual void PostDelete(const GCAObj* obj);

	/**
	 * Get the maximum similarity with the last object called with CanInsert.
	 */
	inline double GetLastMaxSim(void) const {return(LastMaxSim);}

private:

	/**
	* Compute the sum of the similarities of a given object to all the others.
	* If the object is itself in the group, '1.0' is added to the sum.
	* @param obj             Object.
	*/
	double ComputeSumSim(GCAObj* obj);

	/**
	* Compute the centroid of the group, i.a. the object which is the most
	* similar to all the other objects.
	*/
	void ComputeCentroid(void);

public:

	/**
	* Get the centroid of the group.
	* @return Pointer to GCAObj
	*/
	inline GCAObj* GetCentroid(void)
	{
		if(!Centroid)
			ComputeCentroid();
		return(Centroid);
	}

	/**
	 * Compute the sum of the average intra-similarity of the objects and the
	 * average of the agreement and disagreement.
	 * objects.
	 * @param dist           Sum of the average intra-similarity.
	 * @param agree          Sum of the average agreement ratios.
	 * @param disagree       Sum of the average disagreement ratios.
	 */
	void Evaluate(double& dist,double& agree,double& disagree);

	/**
	 * Set the centroid of the group. This method must be used with caution.
	 * @param obj            Object becoming the centroid.
	 */
	void SetCentroid(GCAObj* obj);

	/**
	* Get the average intra-similarity of the group.
	*/
	inline double GetAvgIntraSim(void) {return(AvgIntraSim);}

	/**
	* Compute the similarities of a given object to the centroid of the group.
	* @param obj             Object.
	*/
	inline double ComputeRelSim(const GCAObj* obj) {return(Owner->Instance->GetSim(GetCentroid()->GetElementId(),obj->GetElementId()));}

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	GCAGroup& operator=(const GCAGroup& grp);

	/**
	* Get the maximal value of the ratio of same feedbacks of an object and
	* the ones of the group.
	* @param obj            Object to test.
	* @return double
	*/
//	double GetMaxRatioSame(GCAObj* obj);

	/**
	* Destruct the group.
	*/
	virtual ~GCAGroup(void);

	// friend classes
	friend class GCAChromo;
 	friend class GCAInst;
};


//-----------------------------------------------------------------------------
#endif
