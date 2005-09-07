/*

	GALILEI Research Project

	GGroupIR.h

	Group of an IR GA - Header.

	Copyright 2002 by the Universit�Libre de Bruxelles.

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
#ifndef GGroupIRH
#define GGroupIRH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroups.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gir.h>
#include <gobjir.h>
#include <gchromoir.h>
#include <ginstir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupDataIR provides a representation of the information needed to
* construct a group in the IR Problem.
* @author Pascal Francq
* @short Data for IR Group.
*/
class GGroupDataIR
{
public:
	/**
	* Constructor of the group data.
	*/
	GGroupDataIR(void) {}
};


//-----------------------------------------------------------------------------
/**
* The RGroupBP provides a representation for a group for the IR Problem.
* @author Pascal Francq
* @short IR Group.
*/
class GGroupIR : public R::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>
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
	GObjIR* Relevant;

	/**
	*/
	bool Dirty;

	bool ToEval;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GGroupIR(GGroupIR* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identificator of the group.
	* @param data           Data needed for the group.
	*/
	GGroupIR(GChromoIR* owner,const unsigned int id,const GGroupDataIR* data);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Look if an object can be insert in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual bool CanInsert(const GObjIR* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual void PostInsert(const GObjIR* obj);

	/**
	* Method call after an object was deleted from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual void PostDelete(const GObjIR* obj);

private:

	/**
	* Compute the sum of the distances of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumDist(GObjIR* obj);

public:

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	*/
	void ComputeRelevant(void);

	/**
	* Get the relevant profile of the group, i.a. the profile which is the must
	* similar to all the others profiles.
	* @return Pointer to GObjIR
	*/
	inline GObjIR* GetRelevant(void) {return(Relevant);}

	void Evaluate(double& dist,double& agree,double& disagree);

	/**
	*/
	void SetRelevant(GObjIR* obj);

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
	inline double ComputeRelSim(const GObjIR* obj) {return(Owner->Instance->GetSim(Relevant->GetSubProfile(),obj->GetSubProfile()));}

	/**
	* Compute a homogeneity for a given profile to the group. Actually, it uses
	* the similarity with the relevant profile of the group.
	* @param obj            Profile used as reference.
	*/
	inline double ComputeHomogeneity(const GObjIR* obj) {return(ComputeRelSim(obj));}

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	GGroupIR& operator=(const GGroupIR& grp);

	/**
	* Get the maximal value of the ratio of same feedbacks of a subprofile and
	* the ones of the group.
	* @param obj            Object to test.
	* @return double
	*/
	double GetMaxRatioSame(GObjIR* obj);

	/**
	* Destruct the group.
	*/
	virtual ~GGroupIR(void);

	// friend classes
//	friend class GChromoIR;
 	friend class GInstIR;
};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
