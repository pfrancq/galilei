/*

	GALILEI Research Project

	GGroupingKCos.h

	Heuristic using Similarity - Header.

	(C) 2001 by David Wartel.

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
#ifndef GGroupingKCosH
#define GGroupingKCosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroupingsim.h>
#include <profiles/gprofilessim.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GGroupingKCos provides a representation for a method to group
some
* subprofiles using the KMeans algorithm adapted to cosinus distance.
* @author David Wartel
* @short KMeansCos Grouping.
*/

class GGroupingKCos :   public GGroupingSim
{

protected:

	/**
	* value for decimals error
	*/
	double Epsilon;

	/**
	* Random initial conditions or not.
	*/
	bool Random;

	/**
	* Number of groups.
	*/
	unsigned int GroupsNumber;

	/**
	* Number of iterations
	*/
	unsigned int IterNumber;

	/**
	* Number of tests
	*/
	unsigned int NbTests;

	/**
	* 'true' if the algorithm must find the number of groups, 'false' otherwise.
	*/
	bool FindGroupsNumber;

	/**
	* Maximum number of groups
	*/
	unsigned int MaxNbGroups;

	/**
	* Minimum number of groups
	*/
	unsigned int MinNbGroups;

	/**
	* check if the global similarity is required for the KMeansCos algorithm..
	*/
	bool GlobalSimi;

	/**
	* Container of subprofiles considered as prototypes
	*/
	RContainer<GSubProfile,unsigned int,false,false>* protos;

	/**
	* Container of subprofiles considered as prototypes,
	* needed to calculate the error between two iterations
	*/
	RContainer<GSubProfile,unsigned int,false,false>* protoserror;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<GGroup,unsigned int,false,false>* grpstemp;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<GGroup,unsigned int,false,false>* grpstemp2;

	/**
	* Container of the final groupment
	*/
	RContainer<GGroup,unsigned int,false,false>* grpsfinal;

	/*
	* Similarities between all profiles;
	*/
	GProfilesSim* ProfSim;


public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingKCos(GSession* s) throw(bad_alloc);

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
	* return true if the subprofile is a valid proto;
	*/
	bool IsValidProto(GSubProfile* s) throw(bad_alloc);

	/**
	* returns the 'nbsub' most relevant subprofiles.
	*/
	void RelevantInitSubProfiles(int nbsub);

	/**
	* Get the status of the 'random' criterion
	* @return bool value.
	*/
	bool GetRandom(void) const {return(Random);}

	/**
	* Get the status of 'FindGroupsNumber'.
	* @return bool value
	*/
	int GetFindGroupsNumber(void) {return(FindGroupsNumber);}

	/**
	* Get the number of tests
	* @return int value.
	*/
	int GetTestsNumber(void) const {return(NbTests);}

	/**
	* Get the status of the 'GlobalSim' criterion
	* @return bool value.
	*/
	bool GetGlobalSimi(void) const {return(GlobalSimi);}

	/**
	* Get the maximum number of iterations.
	* @return int value.
	*/
	int GetIterNumber(void) const {return(IterNumber);}

	/**
	* Set the status of the 'random' criterion.
	* @param s              random?
	*/
	void SetRandom(bool s) {Random=s;}

	/**
	* Set the status of 'FindGroupsNumber' criterion.
	* @param s              Find Groups Number?
       */
	 void SetFindGroupsNumber(bool s) {FindGroupsNumber=s;}


	 /**
	* Set the number of tests
	* @param i              number to set.
	*/
	void SetTestsNumber(int  i) {NbTests=i;}

	/**
	* Set the status of the 'GlobalSim' criterion.
	* @param s              global?
	*/
	void SetGlobalSimi(bool s) {GlobalSimi=s;}

	/**
	* Set the number of iteration.
	* @param i              number to set              .
	*/
	void SetIterationsNumber(int  i) {IterNumber=i;}

	/**
	* returns the group containing the subprofile s;
	*/
	GGroup* FindGroup(GSubProfile* s);

	/**
	* evalute the number of groups by first fit
	*/
	void EvaluateGroupsNumber(void);

	/**
	* find the maximum and minimum number of group;
	*/
	void FindGroupsNumberLimits(void);

	/**
	* Calculates the cost function for a kmeanscos clustering
	*/
	double CostFunction(void);

	/**
	*  reallocate the subprofiles to prototypes
	*/
	void ReAllocate(void);

	/**
	*  recenters the prototypes
	*/
	void ReCenter(void);

	/**
	*  calculates the error between two iterations
	*/
	int CalcError(void);

	/**
	* returns the sum of similarity between s and all the subprofiles
	*/
	double SumSimilarity(GSubProfile * s);

	/**
	* returns the sum of similarity between s and all the subprofiles of the group
	*/
	double GroupSumSimilarity(GSubProfile * s, GGroup *g) ;

	/**
	* return the chosen similarity between two subprofiles
	*/
	double Similarity(GSubProfile* s1, GSubProfile* s2);

	/**
	* returns the Calinsky index for the clustering
	*/
	double CalcCalinsky(void);

	/**
	* returns the knearestneighboors measure.
	*/
	double TestMeasure(int k);

	/**
	* returns the "recouvrement" rate.
	*/
	double StatMeasure(void);

	/**
	*  returns the variance of a grouping.
	*/
	double Variance(RContainer<GGroup,unsigned int,false,false>* grps)  ;


	/**
	* pal measure
	*/
	double PalMeasure(void);


protected:

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* /*grp*/){return (false);};

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) throw(GException);


public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingKCos(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

