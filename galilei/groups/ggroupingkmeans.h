                                                                                                                                                                                                                                                                           /*

	GALILEI Research Project

	GGroupingKMeans.h

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
#ifndef GGroupingKMeansH
#define GGroupingKMeansH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
#include <rmath/random.h>
using namespace RMath;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include<groups/ggrouping.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GGroupingKMeans provides a representation for a method to group
some
* subprofiles using the KMeans algorithm adapted to cosinus distance.
* @author David Wartel
* @short KMeansCos Grouping.
*/

class GGroupingKMeans : public GGrouping
{

public:

	/**
	* enum of initial conditions
	*/
	enum Initial {Refined,Random, Scattered, Relevant};


protected:

	/**
	*
	*/
	Initial initial;

	/**
	* value for decimals error
	*/
	double Epsilon;


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
	* check if the global similarity is required.
	*/
	bool GlobalSim;

	/**
	* Container of subprofiles considered as prototypes
	*/
	RContainer<GSubProfile,unsigned int,false,false>* protos;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<GGroup,unsigned int,false,false>* grpstemp2;

	/*
	* Similarities between all profiles;
	*/
	GProfilesSim* ProfSim;

	/**
	*  random parameters
	*/
	RRandom * Rand;


public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingKMeans(GSession* s,const char* n) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void)=0;

	/**
	* Set the settings for the method using a string.
	* @param s*             C string coding the settings.
	*/
	virtual void SetSettings(const char* s)=0;

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc) =0;

	/**
	* return true if the subprofile is a valid proto;
	*/
	bool IsValidProto(RContainer<GSubProfile,unsigned int,false,false>* prototypes,GSubProfile* s) throw(bad_alloc);

	/**
	* returns the 'nbsub' most relevant subprofiles.
	*/
	void RelevantInitSubProfiles(int nbsub);

	/**
	* init the protos to the most scattered subprofiles.
	* @param nbsub         number of subprofiles to get.
	*/
	void ScatteredInitSubProfiles(int nbsub);

	/**
	* init the protos with random subprofiles.
	* @param nbsub         number of subprofiles to get.
	*/
	void RandomInitSubProfiles(RContainer<GSubProfile,unsigned int,false,true>* datase, unsigned int groupsnumber);

	/**
	* init the protos to the refined points.
	* @param nbsub         number of subsamples to get.
	* @param level           level  (in %) of the full dataset for the subsamples
	*/
	void RefiningCenters(int nbsub, int level);

	/**
	*  get the initial conditions
	*/
	Initial GetInit(void) {return(initial);};

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
	* Get the number of groups
	* @return int value.
	*/
	int GetGroupsNumber(void) const {return(GroupsNumber);}

	/**
	* Get the status of the 'GlobalSim' criterion
	* @return bool value.
	*/
	bool GetGlobalSim(void) const {return(GlobalSim);}

	/**
	* Get the maximum number of iterations.
	* @return int value.
	*/
	int GetIterNumber(void) const {return(IterNumber);}

	/**
	* sets the initial conditions
	*/
	void SetInit(Initial i) {initial=i;};

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
	* Set the number of tests
	* @param i              number to set.
	*/
	void SetGroupsNumber(int  i) {GroupsNumber=i;}

	/**
	* Set the status of the 'GlobalSim' criterion.
	* @param s              global?
	*/
	void SetGlobalSim(bool s) {GlobalSim=s;}

	/**
	* Set the number of iteration.
	* @param i              number to set              .
	*/
	void SetIterationsNumber(int  i) {IterNumber=i;}

	/**
	* evalute the number of groups by first fit
	*/
	void EvaluateGroupsNumber(void);

	/**
	* find the maximum and minimum number of group;
	*/
	void FindGroupsNumberLimits(void);

	/**
	* Initialisation of the method.
	*/
	void InitCenters(void) throw(bad_alloc);

	/**
	* Calculates the cost function for a kmeanscos clustering
	*/
	virtual double CostFunction(RContainer<GGroup,unsigned int,false,false>* grps)=0;

	/**
	*  reallocate the subprofiles to prototypes
	*/
	virtual void ReAllocate(RContainer<GSubProfile,unsigned int,false,true>* dataset)=0;

	/**
	*  recenters the prototypes
	*/
	virtual void ReCenter(void)=0;

	/**
	* executes the grouping part of K-Means algorithm
	* @param init           intialization step?
	*/
	virtual void Execute(RContainer<GSubProfile,unsigned int,false,true>* dataset, unsigned int maxiter)=0;

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
	* return the variance of a given clustering
	*/
	virtual double Distortion(RContainer<GGroup,unsigned int,false,false>* grps);

	/**
	* return the variance of a given clustering
	*/
	virtual double GroupVariance(GGroup* grp);

	/**
	* returns the group containing the subprofile s;
	*/
	GGroup* FindGroup(RContainer<GGroup,unsigned int,false,false>* grps, GSubProfile* s);

	/**
	* Verify the confition of KMeansMod (no empty clusters)
	* return true if the conditons are respected, else false
	*/
	bool  VerifyKMeansMod(void);

	/**
	*  couts the content of a group
	*/
	virtual void ShowGroups(RContainer<GGroup,unsigned int,true,true>* grs);


protected:

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* /*grp*/){return (false);};

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) throw(GException) =0;


public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingKMeans(void);

};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

