/*

	GALILEI Research Project

	GGroupingKProtos.h

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
#ifndef GGroupingKProtosH
#define GGroupingKProtosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>
#include <profiles/gprofilessim.h>
#include <groups/ggroupingparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;



//-----------------------------------------------------------------------------
/**
* The GKProtosParam represents all the parameter used in the KMeans module.
* @short GKProtos Parameters.
*/
class GKProtosParams : public GGroupingParams
{
public:

	/**
	* Number of generation for each step.
	*/
	unsigned int NbSubSamples;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	unsigned int SubSamplesRate;

	/**
	* maximum number of kmeans check in initialization.
	*/
	unsigned int VerifyKMeansLimit;

	/**
	* Number of Groups.
	*/
	unsigned int NbGroups;

	/**
	* Number of Prototypes.
	*/
	unsigned int NbProtos;

	/**
	* Maximal number of iterations.
	*/
	unsigned int MaxIters;

	/**
	* Distance parameter.
	*/
	double Lambda;

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
	GKProtosParams& operator=(const GKProtosParams& src);

	/**
	* Constructor.
	*/
	GKProtosParams(void);

	/**
	* Constructor.
	*/
	~GKProtosParams(void) {;}
};



//-----------------------------------------------------------------------------
/**
* The GGroupingKProtos provides a representation for a method to group
some
* subprofiles using the KMeans algorithm adapted to cosinus distance.
* @author David Wartel
* @short KMeansCos Grouping.
*/

class GGroupingKProtos  : public GGrouping

{
protected:

	/**
	* KMeans Parameters
	*/
	GKProtosParams* Params;
	/**
	* container of prototypes.
	*/
	RStd::RContainer<GSubProfile, unsigned int, false, false>* Protos;

	/**
	* container of centers.
	*/
	RStd::RContainer<GSubProfile, unsigned int, false, false>* Centers;
	
	/**
	* container of centers needed to calculate error.
	*/
	RStd::RContainer<GSubProfile, unsigned int, false, false>* CentersError;

	/**
	* groupement.
	*/
	RStd::RContainer<GGroup,unsigned int, false, true>* Grps;


protected:

	/**
	* Make the grouping for a specific Language.
	*/
	void Run(void) throw(GException);

	virtual bool IsValid(GGroup* /*grp*/) {return (true);};


public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GGroupingKProtos(GSession* s, GKProtosParams* params) throw(bad_alloc);

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
	* Initialization of the algorithm (find centers and prototypes).
	*/
	void Initialization (void);

	/**
	* ReAllocation step.
	*/
	void ReAllocate(void);

	/**
	* ReAllocation step.
	*/
	void ReCenter(void); 
	
	/**
	* Calculates the Distance between a subprofile and a group
	*/
	double Distance(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp, bool center);
	
	/**
	* returns the similarity between two subprofiles.
	*/
	double Similarity(GSubProfile* s1, GSubProfile* s2);

	/**
	* find the group, in the container of group, conatining a given subprofile
	*/
	GGroup* FindGroup(RContainer<GGroup,unsigned int,false,true>* grps, GSubProfile* s);
	
	/**
	* returns the nearest prototype to a given subprofile inside a given group 
	*/
	GSubProfile* NearestPrototype(GSubProfile* s,RContainer<GSubProfile,unsigned int,false,true> *grp);
	
	/**
	* calculates the centers error between two iterations.
	*/
	int CalcError(void);

	/*
	* displays infos about KMeans Protos config.
	*/
	void DisplayInfos(void);
	
	/*
	* Destructor.
	*/
	virtual ~GGroupingKProtos(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

