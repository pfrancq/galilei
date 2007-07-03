/*

	Genetic Community Algorithm

	GGCAGroup.h

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
#ifndef GGCAGroupH
#define GGCAGroupH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroups.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <ggca.h>
#include <ggcaobj.h>
#include <ggcachromo.h>
#include <ggcainst.h>


//-----------------------------------------------------------------------------
/**
* The GGCAGroupData provides a representation of the information needed to
* construct a group in the IR Problem.
* @author Pascal Francq
* @short Data for IR Group.
*/
class GGCAGroupData
{
public:
	/**
	* Constructor of the group data.
	*/
	GGCAGroupData(void) {}
};


//-----------------------------------------------------------------------------
/**
* The RGroupBP provides a representation for a group for the IR Problem.
* @author Pascal Francq
* @short IR Group.
*/
class GGCAGroup : public R::RGroup<GGCAGroup,GGCAObj,GGCAGroupData,GGCAChromo>
{
protected:

	/**
	* Sum of the distances for the most relevant profile of the group to all
	* the other profiles.
	*/
	double BestSumDist;

	double AgreementSum;
	double DisagreementSum;

	/**
	* Relevant object of the group.
	*/
	GGCAObj* Relevant;

	/**
	*/
	bool Dirty;

	bool ToEval;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GGCAGroup(GGCAGroup* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identificator of the group.
	* @param data           Data needed for the group.
	*/
	GGCAGroup(GGCAChromo* owner,const unsigned int id,const GGCAGroupData* data);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Verify if an object correspond to a user already in this group.
	* @param obj            Pointer to the object to insert.
	*/
	bool HasSameUser(const GGCAObj* obj) const;

	/**
	* Look if an object can be insert in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual bool CanInsert(const GGCAObj* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual void PostInsert(const GGCAObj* obj);

	/**
	* Method call after an object was deleted from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual void PostDelete(const GGCAObj* obj);

private:

	/**
	* Compute the sum of the distances of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumDist(GGCAObj* obj);

public:

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	*/
	void ComputeRelevant(void);

	/**
	* Get the relevant profile of the group, i.a. the profile which is the must
	* similar to all the others profiles.
	* @return Pointer to GGCAObj
	*/
	inline GGCAObj* GetRelevant(void) {return(Relevant);}

	void Evaluate(double& dist,double& agree,double& disagree);

	/**
	*/
	void SetRelevant(GGCAObj* obj);

	/**
	* Get the average similarity of the group.
	* @returns double representing the average similarity.
	*/
	inline double GetSumDist(void) {return(BestSumDist);}

	/**
	* Compute the similarities of a given profile to the relevant profile of the
	* group.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	inline double ComputeRelSim(const GGCAObj* obj) {return(Owner->Instance->GetSim(Relevant->GetSubProfile(),obj->GetSubProfile()));}

	/**
	* Compute a homogeneity for a given profile to the group. Actually, it uses
	* the similarity with the relevant profile of the group.
	* @param obj            Profile used as reference.
	*/
	inline double ComputeHomogeneity(const GGCAObj* obj) {return(ComputeRelSim(obj));}

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	GGCAGroup& operator=(const GGCAGroup& grp);

	/**
	* Get the maximal value of the ratio of same feedbacks of a subprofile and
	* the ones of the group.
	* @param obj            Object to test.
	* @return double
	*/
	double GetMaxRatioSame(GGCAObj* obj);

	/**
	* Destruct the group.
	*/
	virtual ~GGCAGroup(void);

	// friend classes
//	friend class GGCAChromo;
 	friend class GGCAInst;
};


//-----------------------------------------------------------------------------
#endif
