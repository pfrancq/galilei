/*

	GALILEI Research Project

	GGroupingSim.h

	Heuristic using Similarity - Header.

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
#ifndef GGroupingSimH
#define GGroupingSimH


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
* The GGroupingSim provides a representation for a method to group some
* subprofiles using the notion of similarity.
* @author Pascal Francq
* @short Similarity Grouping.
*/
class GGroupingSim : public GGrouping
{
protected:

	/**
	* Full Similarity for the groups.
	*/
	bool FullSim;

	/**
	* Level of similarity for the groupement.
	*/
	double LevelSim;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingSim(GSession* s) throw(bad_alloc);

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
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Get the status of the full similarity.
	* @return bool value.
	*/
	bool GetFullSim(void) const {return(FullSim);}

	/**
	* Set the status of the full similarity.
	* @param s              Full similarity?
	*/
	void SetFullSim(bool s) {FullSim=s;}

	/**
	* Get the level of similarity to used.
	* @return double value.
	*/
	double GetLevelSim(void) const {return(LevelSim);}

	/**
	* Set the level of similarity for the grouping.
	* @param l              Level to used.
	*/
	void SetLevelSim(double l) {LevelSim=l;}

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
	virtual ~GGroupingSim(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
