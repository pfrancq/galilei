/*

	GALILEI Research Project

	GGroupIR.h

	Group of an IR GA - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GGroupIRH
#define GGroupIRH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rgroups.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>


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
class GGroupIR : public RGGA::RGroup<GGroupIR,GObjIR,GGroupDataIR,GChromoIR>
{
protected:
	bool Done;

	/**
	* Average similarity between the profiles of the group.
	*/
	double AvgSim;

	/**
	* Relevant object of the group.
	*/
	GObjIR* Relevant;

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
	* Verify if the group is not violating the integrity of the system.
	* @return true if the group is correct, false else.
	*/
	virtual bool Verify(void);

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

	/**
	* Compute the average similarity of the group.
	* @returns result.
	*/
	double ComputeAvgSim(void);

	/**
	* Compute the sum of the similarities of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GObjIR* obj);

	/**
	* Compute the sum of the distances of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumDist(GObjIR* obj);

	/**
	* Compute the maximum similarity of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeMaxSim(GObjIR* obj);

	/**
	* Compute the minimum similarity of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeMinSim(GObjIR* obj);

	/**
	* Compute the average of the similarities of a given profile to the one of the group.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeAvgSim(GObjIR* obj);

	/**
	* Compute the maximum similarity between subprofiles of two groups.
	* @param grp            The group used for the comparison.
	* @returns result.
	*/
	double ComputeMaxSim(GGroupIR* grp);

	/**
	* Compute the maximum similarity between subprofiles of a group and a list
	* of subprofiles.
	* @param grp            Array of subprofiles.
	* @param nb             Number of subprofiles.
	* @returns result.
	*/
	double ComputeMaxSim(GObjIR** grp,unsigned int nb);

	/**
	* Compute the minimum similarity in the group.
	* @returns result.
	*/
	double ComputeMinSim(void);

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	* @returns Sum of similarities to the relevant profile.
	*/
	double ComputeRelevantSum(void);

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	* @returns Sum of distances to the relevant profile.
	*/
	double ComputeRelevantSumDist(void);

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	* @returns Minimum of similarities to the relevant profile.
	*/
	double ComputeRelevantMin(void);

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	* @returns Max of similarities to the relevant profile.
	*/
	double ComputeRelevantMax(void);

	/**
	* Compute the most non-relevant profile of the group, i.a. the profile
	* which is the les similar to all the others profiles.
	* @returns Sum of similarities to the non-relevant profile.
	*/
	double ComputeNonRelevant(void);

	/**
	* Compute the most relevant profile of the group, i.a. the profile which is
	* the most similar to all the others profiles.
	*/
	void ComputeRelevant(void);

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	GGroupIR& operator=(const GGroupIR& grp);

	/**
	* Get the average similarity of the group.
	* @returns double representing the average similarity.
	*/
	double GetAvgSim(void) const {return(AvgSim);}

	/**
	* Destruct the group.
	*/
	virtual ~GGroupIR(void);

	// friend classes
	friend class GChromoIR;
 	friend class GInstIR;
};


//-----------------------------------------------------------------------------
/**
* The GGroupIRCursor class provides a way to go trough a set of group in a GGA.
* @short GGA Group Cursor.
*/
CLASSCURSOR(GGroupIRCursor,GGroupIR,unsigned int)


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
