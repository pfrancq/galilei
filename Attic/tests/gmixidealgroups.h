/*

	GALILEI Research Project

	GMixIdealGroups.h

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef GMixIdealGroupsH
#define GMixIdealGroupsH


//-----------------------------------------------------------------------------
// include files for R
#include <rmath/random.h>
#include <rstd/rcontainer.h>
using namespace RMath;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GMixidealGroups provides a set of mixed groups coming from
* the ideal groupment.
* @author Wartel David
* @short Mix ideal groupement into many mixed groups.
*/
class GMixIdealGroups
{
protected:


	/**
	* Random number.
	*/
	RRandom* Rand;

	/**
	* Ideal groupment.
	*/
	RContainer<GGroups,unsigned int,true,true>* IdealGroups;

	/**
	* Mixed groups.
	*/
	RContainer<GGroups,unsigned int,true,true>* MixedGroups;

	/**
	* Container used to create subjects catagories.
	*/
	RContainer<GGroupIdParentId,unsigned int,true,true>* Parents;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Want a random mix ?
	*/
	bool Random;

	/**
	* Want the ideal groupment ?
	*/
	bool Ideal;

	/**
	* Want to merge same category ?
	*/
	bool MergeSame;

	/**
	* Want to merge different categories? 
	*/
	bool MergeDiff;

	/**
	* Want to split ?
	*/
	bool Split;

	/**
	* Number of mixed groups required.
	*/
	unsigned int NbMixedGroups;

	/**
	* Number of specific mutations (mergediff or mergesame or split) in one mixedgroups.
	*/
	unsigned int Level;

public:

	/**
	* Constructor
	* @param sess           opened session
	* @param idealgroups    groups to mix
	* @param nbgroups       number of groups needed
	* @param level          maximum number of each type of mix.
	* @param ms             merge same category?
	* @param md             merge different category?
	* @param s              split category?
	* @param r              random grouping?
	*/
	GMixIdealGroups(GSession* sess, RContainer<GGroupIdParentId,unsigned int,true,true>* parents, RContainer<GGroups,unsigned int,true,true>* idealgroups, int nbgroups=10, int level=3, bool ms=true, bool md=true, bool s=true, bool r=true, bool i=true);

	/**
	* set the settings.
	*/
	void SetSettings(const char* s);

	/**
	* Merge groups.
	* @param i              groups from same category?
	*/
	void MergeGroups(bool i);
	
	/**
	* Split groups in two groups.
	*/
	void SplitGroups(void);

	/**
	* Random groups.
	*/
	void RandomGroups(void);

	/**
	* Mix the groups.
	*/
	void Run(void);

	/**
	* Copy the ideal groupment in the mixedgroups container.
	*/
	void InitMixedGroups(void);

	/**
	* save the mixed groups in database.
	* @param nbmixedgroups  number of the mixed groups.
	*/
	void StockInDatabase(int nbmixedgroups);

	void IShow(void);

	void Show(void);

	/**
	*  Destructor.
	*/
	~GMixIdealGroups(void);


};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
