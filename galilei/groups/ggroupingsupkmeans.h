/*

	GALILEI Research Project

	GGroupingSupKMeans.h

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
#ifndef GGroupingSupKMeansH
#define GGroupingSupKMeansH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
//tmp
#include <rga/robjs.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include<galilei.h>
#include<groups/ggrouping.h>
#include<groups/ggroupingparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GGrouping;


//-----------------------------------------------------------------------------
/**
* The GSupKMeansParam represents all the parameter used in the KMeans module.
* @short GSupKMeans Parameters.
*/
class GSupKMeansParams : public GGroupingParams
{
public:

	/**
	* enum of initial conditions
	*/
	enum Initial {Random, Relevant};

	/**
	* Initial conditions.
	*/
	Initial InitMode;

	/*
	* Minimum number of common OK documents needed to force two profiles to be
	* in the same group.
	*/
	unsigned int NbPcSame;

	/**
	* Maximum number of common  documents with different judgements in a same
	* group.
	*/
	unsigned int NbPcDiff;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	double MinSim;

	/**
	* Maximum number of iterations
	*/
	unsigned int NbIters;

	/**
	* if true, a 'hard constraint' KMeans is followed by a 'traditionnal' kmeans.
	*/
	bool DoubleKMeans;

	/**
	* if true, the algorithm is used as initilaization step
	*/
	 bool UsedAsInitialization;

	/**
	* number of sample  if used as 'initilization step'
	*/
	 unsigned int NbSamples;

	 /**
	 * if  profile A and profile B are grouped together with a frequency
	 * superior to 'SameGroupRate', they MUST be grouped together.
	 */
	unsigned int SameGroupRate;

	/**
	* if true: displays information with 'cout'
	*/
	bool Debug;

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*);

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GSupKMeansParams& operator=(const GSupKMeansParams& src);

	/**
	* Constructor.
	*/
	GSupKMeansParams(void);
};

//-----------------------------------------------------------------------------
/**
* The GGroupingSupKMeans provides a representation for a method to group
* some subprofiles using the KMeans algorithm adapted to cosinus distance .
* This method determines the nulmber of groups thanks to hard constaints.
* @author David Wartel
* @short SupKMeans Grouping.
*/

class GGroupingSupKMeans  : public GGrouping
{

	/**
	* KMeans Parameters
	*/
	GSupKMeansParams* Params;

	/**
	* KMeans Parameters
	*/
	GKMeansParams* Params2;

	/**
	* Container of cObj considered as prototypes
	*/
	RContainer<GSubProfile,unsigned int,false,false>* Protos;

	/**
	* Container of cObj considered as prototypes
	*/
	RContainer<GSubProfile,unsigned int,false,false>* ProtosDblKMeans;

	/**
	* Container of cObj considered as prototypes
	*/
	RContainer<GGroups,unsigned int,false,false>* FoundGroups;

	/**
	* hard constraints container.
	*/
	RContainer<GGroup,unsigned int,false,false>* HardConstraints;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param grps          Ideal groupment.
	*/
	GGroupingSupKMeans(GSession* s, GSupKMeansParams* p, GKMeansParams* p2) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param s*             C string coding the settings.
	*/
	void SetSettings(const char* s);

	/**
	* is the group valid ?
	*/
	bool IsValid(GGroup* ) {return(true);} ;


protected:

	/**
	* Make the grouping for a specific Language.
	*/
	void Run(void) throw(GException);


public:


	/**
	* Make the grouping for a specific Language.
	*/
	void AdaptMinSimToDatabase(void) ;

	/*
	*  find a group to put the subprofile into
	* @param togroup        subprofile to group
	*/
	GGroup* FindGroup(GSubProfile* togroup);

	/**
	* check the 'same judgements on same documents' hard constraint
	*/
	bool IsAPcSameValidGroup(GGroup* group, GSubProfile* sub) ;

	/**
	* check the 'different jugements on same documents" hard constraint
	*/
	bool IsAPcDiffValidGroup(GGroup* group, GSubProfile* sub) ;

	/**
	*  check the 'minimum of similarity' hard constraint
	*/
	bool IsAMinSimValidGroup(GGroup* group, GSubProfile* sub) ;

	/*
	* compute the distance between two subprofiles.
	*/
	double SubProfDistance(GSubProfile* s1, GSubProfile* s2) ;

	/**
	* find the closest group from a given subprofile.
	* @param subprof        given subprofile
	* @param hardconstraint if true, hard constraint must be checked.
	*/
	GGroup* ClosestGroup(GSubProfile* subprof, bool hardconstraint);

	/**
	*  execute the algorithm.
	*/
	unsigned int Execute(bool doublekmeans);

	/**
	* ReCentering step (whixh includes the error calculation).
	*/
	unsigned int  ReCenterCalcError(bool doublekmeanst);

	/**
	* Re-Allocating step
	*/
	void ReAllocate(bool  hardconstraint);

	/**
	*  calculates the distance between a given group and a given subprofile
	* @param group          given group
	* @param sub              given subprofile
	* @param hardconstraint if true, hardconstraint must be checked
	*/
	double Distance(GGroup* g, GSubProfile* sub, bool  hardconstraint);

	/**
	* Initialization step
	*/
	void Init(void) throw(bad_alloc);

	/**
	* create hard constraints from the samples if the algoritm is used as
	* initialization step.
	*/
	void SetConstraintsFromSamples(unsigned int** corelationtable);

	/**
	* display the groups
	*/
	void DisplayGroups(void);

	/**
	* Destructor.
	*/
	virtual ~GGroupingSupKMeans(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

