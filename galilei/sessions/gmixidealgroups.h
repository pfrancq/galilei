/*

	GALILEI Research Project

	GMixIdealGroups.h

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Universit� Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
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
#ifndef GMixIdealGroupsH
#define GMixIdealGroupsH


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
	R::RRandom* Rand;

	/**
	* Ideal groupment.
	*/
	R::RContainer<GGroups,unsigned int,true,true>* IdealGroups;

	/**
	* Mixed groups.
	*/
	R::RContainer<GGroups,unsigned int,true,true> MixedGroups;

	/**
	* Current Language.
	*/
	GGroups* Current;

	/**
	* Tamporary array of pointer to groups.
	*/
	GGroup** Tab;

	/**
	* Temporary array of pointer to languages.
	*/
	GGroups** Tabs;

	/**
	* Temporary array of pointer to subprofiles.
	*/
	GSubProfile** TabS;

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
	* Want to merge ?
	*/
	bool Merge;

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
	* @param nbgroups       Number of groups needed
	* @param nbsubprofiles  Number of subprofiles.
	* @param level          maximum number of each type of mix.
	* @param m              merge same?
	* @param s              split category?
	* @param r              random grouping?
	*/
	GMixIdealGroups(GSession* sess,R::RContainer<GGroups,unsigned int,true,true>* idealgroups, int nbgroups=10, int level=3, bool m=true, bool s=true, bool r=true, bool i=true);

	/**
	* set the settings.
	*/
	void SetSettings(const char* s);

	/**
	* Get the ideal group of a given subprofile.
	* @param s              Pointer to the subprofile.
	* @reutns GGroup pointer.
	*/
	GGroup* GetIdealGroup(GSubProfile* s);

	/**
	* Merge groups.
	* @returns true if merge possible.
	*/
	bool MergeGroups(void);

	/**
	* Split groups in two groups.
	* @returns true if split possible.
	*/
	bool SplitGroups(void);

	/**
	* Random groups.
	*/
	void RandomGroups(void);

	/**
	* Mix the groups.
	* @param g              Slot to receive messages.
	*/
	void Run(GSlot* g);

	/**
	* Copy the ideal groupment in the mixedgroups container.
	* @param mingroups      Minimal number of groups needed.
	*/
	void InitMixedGroups(unsigned int mingroups);

	/**
	* save the mixed groups in database.
	* @param nbmixedgroups  number of the mixed groups.
	*/
	void StockInDatabase(int nbmixedgroups);

	/**
	* Randomly reorder the groups of Current in Tabs.
	*/
	void RandOrderTab(void);

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
