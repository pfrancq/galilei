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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI
{
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
	* Evaluation of the chromosomes. Actually, it is just the average of the
	* intern similarities.
	*/
	virtual void Evaluate(void);

	/**
	* Does a local optimisation for the chromosome.
	*/
	virtual void LocalOptimisation(void);

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
