/*

	GALILEI Research Project

	GGroupingCure.h

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
#ifndef GGroupingCureH
#define GGroupingCureH


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
#include <groups/ggroupingparams.h>
#include <groups/gcomparegrouping.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GKCureParam represents all the parameter used in the Cure module.
* @short GKCureParameters.
*/
class GCureParams : public GGroupingParams
{
public:

	/**
	* Number of Groups.
	*/
	unsigned int NbGroups;

	/**
	* Number of Protos per group.
	*/
	unsigned int NbProtos;

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
	GCureParams& operator=(const GCureParams& src);

	/**
	* Constructor.
	*/
	GCureParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupingCure provides a representation for a method to group
some
* subprofiles using the Cure algorithm adapted to cosinus distance.
* @author David Wartel
* @short Cure Grouping.
*/

class GGroupingCure : public GGrouping 
{


protected:

	/**
	* Cure parameters
	*/
	GCureParams* Params;

	/*
	* Similarities between all profiles.
	*/
	GProfilesSim* ProfSim;

	/**
	* container of representatives
	*/
	RContainer<GSubProfile,unsigned int,false,false>* Prototypes;

	/**
	* container of groups.
	*/
	RContainer<GGroup,unsigned int,false,true> * Grps;



public :

	/**
	* constructor
	*/
	GGroupingCure(GSession* s, GCureParams* p) throw(bad_alloc);

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
	* init Groups with one subprofile per group.
	*/
	void InitGroups(void);

	/**
	* init the container of prototypes.
	*/
	void InitProtos(void);

	/**
	*  displays informations about CURE config.
	*/
	void DisplayInfos(void);

	/**
	* runs the algorithm
	*/
	void Run(void) throw(GException);

	/**
	* validity check from ggrouping, unused in this method.
	*/
	bool IsValid(GGroup* grp) {if(grp) return(true); else return(false);} ;

	/**
	* merges groups
	* @param g1             group to merge.
	* @param g2             group to merge.
	*/
	void MergeGroups(GGroup* g1, GGroup* g2);

	/**
	* find the prototypes of a group and inserts them into prototypes container.
	*/
	void CreateGroupsPrototypes(GGroup* g);

	/**
	* returns the distance between two groups
	* @param g1             first group
	* @param g2             second group
	*/
	double GroupsDistance(GGroup* g1, GGroup* g2);

	/**
	* returns the distance between two subprofiles
	*/
	double SubProfDistance(GSubProfile* s1, GSubProfile* s2);

	/**
	* returns the similarity between two subprofiles
	* @param s1             first subprofile
	* @param s2             second subprofile
	*/
	double Similarity(GSubProfile* s1, GSubProfile* s2);

	/**
	* calculate the cost function.
	* @param grps           the group the cost function is applied on
	*/
	double CostFunction(RContainer<GGroup,unsigned int,false,true>* grps);

	/*
	* temporary function to show groups
	*/
	void ShowGroups(RContainer<GGroup,unsigned int,false,true>*gs);

	/**
	* destructor
	*/
	~GGroupingCure(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

