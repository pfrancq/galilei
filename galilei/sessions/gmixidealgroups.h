/*

	GALILEI Research Project

	GMixIdealGroups.h

	Representation of groups of subprofiles and groups of documents.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
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
#ifndef GMixIdealGroupsH
#define GMixIdealGroupsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
class GGroupsLang;


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
	GGroups* IdealGroups;

	/**
	* Mixed groups.
	*/
	GGroups* MixedGroups;

	/**
	* Current Language.
	*/
	GLang* CurrentLang;

	/**
	* Tamporary array of pointer to groups.
	*/
	GGroup** Tab;

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
	* @param sess           Session.
	* @param idealgroups    Groups to mix.
	* @param nbgroups       Number of groups needed.
	* @param level          Maximum number of each type of mix.
	* @param m              Merge same?
	* @param s              Split category?
	* @param r              Random grouping?
	* @param i              Ideal group should be integrated.
	*/
	GMixIdealGroups(GSession* sess,GGroups* idealgroups, int nbgroups=10, int level=3, bool m=true, bool s=true, bool r=true, bool i=true);

	/**
	* Set the settings.
	* @param s              C String representing the settings.
	*/
	void SetSettings(const char* s);

	/**
	* Get the ideal group of a given subprofile.
	* @param s              Pointer to the subprofile.
	* @return GGroup pointer.
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

	/**
	*  Destructor.
	*/
	~GMixIdealGroups(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
