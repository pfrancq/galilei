/*

	GALILEI Research Project

	GGroupingGGA.h

	Heuristic using a GGA - Header.

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
#ifndef GGroupingGGAH
#define GGroupingGGAH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

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
	* GA Objects.
	*/
	RGA::RObjs<GObjIR>* Objs;

	/**
	* Instance of the GA.
	*/
	GInstIR* Instance;

	/**
	* Parameters of the GA used.
	*/
	GIRParams* Params;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param p              Parameters.
	*/
	GGroupingGGA(GSession* s,GIRParams* p) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* "Global PopSize MaxGen StepGen Step MinSimLevel MinCommonOK MinCommonDiff"
	* " ParamsSim.P ParamsSim.Q ParamsSim.Weight ParamsNb.P ParamsNb.Q ParamsNb.Weight "
	* " ParamsOK.P ParamsOK.Q ParamsOK.Weight ParamsDiff.P ParamsDiff.Q ParamsDiff.Weight"
	* @param s*             C string coding the settings.
	*/
	virtual void SetSettings(const char* s);

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Construct the groups of the session based on a chromosome.
	*/
	void ConstructGroupsFromChromo(GChromoIR* chromo) throw(bad_alloc);

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
