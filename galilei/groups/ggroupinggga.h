/*

	GALILEI Research Project

	GGroupingGGA.h

	Heuristic using a GGA - Header.

	(C) 2002 by P. Francq.

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
#ifndef GGroupingGGAH
#define GGroupingGGAH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GGroupingGGA provides a representation for a method to group some
* subprofiles using a grouping genetic algorithm.
* @author Pascal Francq
* @short GGA Method.
*/
class GGroupingGGA : public GGrouping
{
protected:

	/**
	* Size of the population of the GA.
	*/
	unsigned int PopSize;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSimLevel;

	/**
	* Maximal number of generation to run.
	*/
	unsigned int MaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool Step;

	/**
	* Number of generation for each step.
	*/
	unsigned int StepGen;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingGGA(GSession* s) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param s*             C string coding the settings.
	*/
	virtual void SetSettings(const char* s);

	/**
	* Set the size of the population.
	* @param s              Size.
	*/
	void SetPopSize(unsigned int s) {PopSize=s;}

	/**
	* Get the size of the population.
	* @returns Size.
	*/
	unsigned int GetPopSize(void) const {return(PopSize);}

	/**
	* Set the minimum level of similarity for the groups.
	* @param m              Minimum level.
	*/
	void SetMinSimLevel(double m) {MinSimLevel=m;}

	/**
	* Get the minimum level of similarity for the groups.
	* @returns double representing the minimum level.
	*/
	double GetMinSimLevel(void) const {return(MinSimLevel);}

	/**
	* Set the Maximal number of generation.
	* @param max            Maximal number.
	*/
	void SetMaxGen(unsigned int max) {MaxGen=max;}

	/**
	* Get the Maximal number of generation.
	* @returns Maximal number.
	*/
	unsigned int GetMaxGen(void) const {return(MaxGen);}

	/**
	* Set the status of the step mode.
	* @param s              Status.
	*/
	void SetStep(bool s) {Step=s;}

	/**
	* Get the status of the step mode.
	* @returns bool value representing the status.
	*/
	bool GetStep(void) const {return(Step);}

	/**
	* Set the number of generations for each step.
	* @param n              Number of generations.
	*/
	void SetStepGen(unsigned int n) {StepGen=n;}

	/**
	* Get the number of generations for each step.
	* @returns unsigned int representing the number of generations.
	*/
	unsigned int GetStepGen(void) const {return(StepGen);}

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

protected:

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* subprofiles of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp) const;

	/**
	* Test if a group is still coherent if a subprofile is added to it.
	* Actually, this means verify that all the subprofiles of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param sub            SubProfile eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp,const GSubProfile* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp);

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) throw(GException);

public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingGGA(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
