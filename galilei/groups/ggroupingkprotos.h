/*

	GALILEI Research Project

	GGroupingKProtos.h

	Heuristic using Similarity - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GGroupingKProtosH
#define GGroupingKProtosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;

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
* The GGroupingKProtos provides a representation for a method to group some
* subprofiles using the notion of similarity.
* @author Pascal Francq
* @short Similarity Grouping.
*/
class GGroupingKProtos : public GGrouping
{
protected:
	/**
	* random initial conditions or not.
	*/
	bool Random;

	/**
	* the algorithm must find the number of groups or not.
	*/
	bool FindGroupsNumber;

	/**
	* Level of similarity for the groupement.
	*/
	int GroupsNumber;

	/**
	* Number of protos per group;.
	*/
	int ProtosNumber;

	/**
	*Number of iterations
	*/
	int IterNumber;

	/**
	* Number of tests
	*/
	int NbTests;

	/**
	*Maximum number of groups
	*/
	int MaxNbGroups;

	/**
	*Minimum number of groups
	*/
	int MinNbGroups;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingKProtos(GSession* s) throw(bad_alloc);

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
	bool GetRandom(void) const {return(Random);}

	/**
	* Set the status of the full similarity.
	* @param s              Full similarity?
	*/
	void SetRandom(bool s) {Random=s;}

	/**
	* Set the status of FindGroupsNumber.
	* @param s              Full similarity?
	*/
	void SetFindGroupsNumber(bool s) {FindGroupsNumber=s;}

	/**
	* Get the number of groups.
	* @return int value.
	*/
	int GetGroupsNumber(void) const {return(GroupsNumber);}

	/**
	* Get the number o fprotos per group.
	* @return int value.
	*/
	int GetProtosNumber(void) const {return(ProtosNumber);}

	/**
	* Get the status of FindGroupsNumber.
	* @param s              Full similarity?
	*/
	int GetFindGroupsNumber(void) {return(FindGroupsNumber);}


	/**
	* Get the maximum number of groups
	* @return int value.
	*/
	int GetMaxNbGroups(void) const {return(MaxNbGroups);}

	/**
	* Get the minimum number of groups
	* @return int value.
	*/
	int GetMinNbGroups(void) const {return(MinNbGroups);}

	/**
	* Get the number of tests
	* @return int value.
	*/
	int GetTestsNumber(void) const {return(NbTests);}

	/**
	* Get the maximum number of iterations.
	* @return int value.
	*/
	int GetIterNumber(void) const {return(IterNumber);}

	/**
	* Set the level of similarity for the grouping.
	* @param l              Level to used.
	*/
	void SetGroupsNumber(int  i) {GroupsNumber=i;}

	/**
	* Set the number of protos per group.
	* @param i              number to used.
	*/
	void SetProtosNumber(int  i) {ProtosNumber=i;}

	/**
	* Set the maximum number of groups.
	* @param i              Level to used.
	*/
	void SetMaxNbGroups(int  i) {MaxNbGroups=i;}


	/**
	* Set the minimum number of groups.
	* @param i              Level to used.
	*/
	void SetMinNbGroups(int  i) {MinNbGroups=i;}


	/**
	* Set the number of tests
	* @param i              nulber to set.
	*/
	void SetTestsNumber(int  i) {NbTests=i;}

	/**
	* Set the level of similarity for the grouping.
	* @param l              Level to used.
	*/
	void SetIterationsNumber(int  i) {IterNumber=i;}

protected:

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
	virtual ~GGroupingKProtos(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
