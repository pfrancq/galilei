/*

	GALILEI Research Project

	GChromoIR.h

	Chromosome for an IR Problem - Header

	(C) 2001 by P. Francq.

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
#ifndef GChromoIRH
#define GChromoIRH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rchromog.h>
#include <rpromethee/rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GChromoIR class provides a representation for a chromosome for the IR
* Problem.
* @author Pascal Francq
* @short IR Chromosome.
*/
class GChromoIR : public RGGA::RChromoG<GInstIR,GChromoIR,GFitnessIR,GThreadDataIR,GGroupIR,GObjIR,GGroupDataIR>
{
	/**
	* Similarities between the subprofiles to group.
	*/
	GProfilesSim* Sims;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Average similarity of the groups.
	*/
	double AvgSim;

	/**
	* Average number of profiles in the groups.
	*/
	double AvgProf;

	/**
	* Factor representing the part of subprofiles having common OK documents
	* and that are in the same group.
	*/
	double OKFactor;

	/**
	* Factor representing the part of subprofiles having common documents but
	* with different judgements and that are in the same group.
	*/
	double DiffFactor;

	/**
	* Factor representing the part of subprofiles that don't want to be alone
	* but that are really alone.
	*/
	double SocialFactor;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GObjIR** thObjs1;

	/**
	* Temporary array of Objects (Thread dependent data).
	*/
	GObjIR** thObjs2;

	/**
	* Array representing the pairs of groups not tested for merging.
	*/
	bool** Pairs;

	/**
	* Number of rows allocated by pairs.
	*/
	unsigned int NbRows;

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identificator of the chromosome.
	*/
	GChromoIR(GInstIR* inst,unsigned int id) throw(bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data.
	*/
	virtual void Init(GThreadDataIR* thData) throw(bad_alloc);

	/**
	* Compute the sum of the similarities of a given profile to all the others.
	* @param obj            Profile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GObjIR** grp,unsigned int nb,GObjIR* obj);

	/**
	* Compute the relevant profile of the group, i.a. the profile which is the
	* most similar to all the others profiles.
	* @rel                  Relevant profile computed.
	* @returns Sum of similarities to the relevant profile.
	*/
	double ComputeRelevant(GObjIR** grp,unsigned int nb,GSubProfile* &rel);

	/**
	* Look if two groups were merged together.
	* @param grp1           First group.
	* @param grp2           Second group.
	* @return bool.
	*/
	bool MergeGroups(GGroupIR* grp1,GGroupIR* grp2);

	/**
	* Look if a group was be divided.
	* @param grp            Group to analyse.
	* @return bool.
	*/
	bool DivideGroup(GGroupIR* grp);

	/**
	* Construct the chromosome to be the same as grps.
	*/
	void ConstructChromo(GGroups* grps);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Evaluation of the chromosomes. Actually, it is just the average of the
	* intern similarities.
	*/
	virtual void Evaluate(void);

	/**
	* Do the standard crossover of the GGA and do a reorganisation after.
	*/
	virtual bool Crossover(GChromoIR* parent1,GChromoIR* parent2);

	/**
	* Do the standard mutation of the GGA and do a reorganisation after.
	*/
	virtual bool Mutation(void);

	/**
	* Do the standard mutation of the GGA.
	*/
	virtual bool Modify(void);

	/**
	* Set if the pair of groups (id1,id2) was tested for merging or not.
	* @param id1            Identificator of the first group.
	* @param id2            Identificator of the second group.
	* @param s              If true, the pair must be tested.
	*/
	void Set(unsigned int id1,unsigned int id2,bool s);

	/**
	* Verify if the pair of groups (id1,id2) was tested for merging or not.
	* @param id1            Identificator of the first group.
	* @param id2            Identificator of the second group.
	* @returns If true, the pair must be tested.
	*/
	bool Get(unsigned int id1,unsigned int id2);

	/**
	* Set if the all pair containing the group id1 was tested for merging or
	* not.
	* @param id1            Identificator of the group.
	* @param s              If true, the pair must be tested.
	*/
	void Set(unsigned int id1,bool s);

	/**
	* Does a reorganisation of the chromosome.
	*/
	void ReOrganisation(void);

	/**
	* Look if two subprofiles are in the same group or not.
	* @param obj1           Identificator of the first subprofile.
	* @param obj2           Identificator of the second subprofile.
	* @return true if they are in the same group, else false.
	*/
	bool SameGroup(unsigned int obj1,unsigned int obj2) const;

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	GChromoIR& operator=(const GChromoIR& chromo);

	/**
	* Destructor.
	*/
	virtual ~GChromoIR(void);

	// friend classes
	friend class GGroupIR;
	friend class GInstIR;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
